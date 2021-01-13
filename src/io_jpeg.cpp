#include "io_jpeg.h"

#include "image_priv.h"

#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C"
{
#include "jpeglib.h"
}

struct error_mgr
{
   jpeg_error_mgr jerr;
   jmp_buf setjmp_buf;
};

namespace img32
{
   void default_errorfn(j_common_ptr cinfo)
   {
      jpeg_error_mgr* err = cinfo->err;

      char buf[JMSG_LENGTH_MAX];
      err->format_message(cinfo, buf);

      printf("jpeglib: %s\n", buf);
      //longjmp(((error_mgr*)err)->setjmp_buf, 1);
   }

   JpegIO::JpegIO(const char filename[])
   {
      m_file = fopen(filename, "rb");
   }

   bool JpegIO::decode(Image* dstImg, ColorType ct)
   {
      if(!m_file) return false;

      jpeg_decompress_struct dinfo;
      error_mgr err;

      dinfo.err = jpeg_std_error(&err.jerr);
      err.jerr.error_exit = default_errorfn;

      if(setjmp(err.setjmp_buf)) {
         jpeg_destroy_decompress(&dinfo);
         return false;
      }

      jpeg_create_decompress(&dinfo);
      jpeg_stdio_src(&dinfo, m_file);
      jpeg_save_markers(&dinfo, JPEG_APP0+2, 0xffff);
      (void)jpeg_read_header(&dinfo, TRUE);

      if(dinfo.jpeg_color_space == JCS_GRAYSCALE)
         dinfo.out_color_space = JCS_GRAYSCALE;
      else
         dinfo.out_color_space = JCS_RGB;

      (void)jpeg_start_decompress(&dinfo);

      int width = dinfo.output_width;
      int height = dinfo.output_height;
      *dstImg = Image(ImageInfo::Make(width, height, ct)); 
      printf("Jpeg colorspace: %d\n", dinfo.jpeg_color_space);

      JDIMENSION buffer_height = dinfo.rec_outbuf_height;
      JSAMPARRAY buffer = new JSAMPROW[buffer_height];
      if (!buffer) {
         jpeg_destroy_decompress(&dinfo);
         return false;
      }

      for (int c=0; c<(int)buffer_height; c++) {
         buffer[c] = new JSAMPLE[dinfo.output_width * dinfo.output_components];
         if (!buffer[c]) {
            for (c--; c>=0; c--)
               delete[] buffer[c];
            delete[] buffer;
            jpeg_destroy_decompress(&dinfo);
            return false;
         }
      }

      while(dinfo.output_scanline < dinfo.output_height) {
         JDIMENSION num_scanlines = jpeg_read_scanlines(&dinfo, buffer, buffer_height);
         uint8_t* src_address;
         address_t dst_address;

            for (int y=0; y<(int)num_scanlines; y++) {
               src_address = ((uint8_t**)buffer)[y];
               color_t r, g, b;

               for (int x=0; x<dstImg->width(); x++) {
                  dst_address = dstImg->writable_addr32(x, dinfo.output_scanline-1+y);
                  if(dinfo.out_color_space == JCS_RGB) {
                     r = *(src_address++);
                     g = *(src_address++);
                     b = *(src_address++);
                  }else if(dinfo.out_color_space == JCS_GRAYSCALE){
                     int grayscale = *(src_address++);
                     r = grayscale;
                     g = grayscale;
                     b = grayscale;
                  }

                  SetPixelsIntoAddress(dst_address, dstImg->colorType(), r, g, b, 255);
               }
            }
      }
      
      for(int i = 0; i < (int)buffer_height; i++)
         delete[] buffer[i];
      delete[] buffer;

      jpeg_finish_decompress(&dinfo);
      jpeg_destroy_decompress(&dinfo);
      fclose(m_file);
      return true;
   }
} // namespace img32