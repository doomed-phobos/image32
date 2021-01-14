#include "image_priv.h"

#include "string.h"
#include "file.h"
#include "time.h"

#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <cstdio>
//TODO: ARREGLAR LA VELOCIDAD DE DECODIFICACION DE UNA IMAGEN
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
      longjmp(((error_mgr*)cinfo->err)->setjmp_buf, 1);
   }

   bool JpgIO::decode(ImgIO* io, Image* dstImg)
   {
      FILE* file = open_file(io->filename(), "rb");
      jpeg_decompress_struct dinfo;
      error_mgr err;

      dinfo.err = jpeg_std_error(&err.jerr);
      err.jerr.error_exit = default_errorfn;

      if(setjmp(err.setjmp_buf)) {
         // Obteniendo el mensaje de error para luego enviarlo a IO
         char buf[JMSG_LENGTH_MAX];
         err.jerr.format_message((j_common_ptr)&dinfo, buf);
         io->onError(format_to_string("jpeglib: %s", buf).c_str());
         jpeg_destroy_decompress(&dinfo);
         return false;
      }

      jpeg_create_decompress(&dinfo);
      jpeg_stdio_src(&dinfo, file);
      (void)jpeg_read_header(&dinfo, true);

      if(dinfo.jpeg_color_space == JCS_GRAYSCALE)
         dinfo.out_color_space = JCS_GRAYSCALE;
      else
         dinfo.out_color_space = JCS_RGB;

      (void)jpeg_start_decompress(&dinfo);

      int width = dinfo.output_width;
      int height = dinfo.output_height;
      *dstImg = Image(ImageInfo::Make(width, height, io->colorType())); 

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
         jpeg_read_scanlines(&dinfo, buffer, buffer_height);
         uint8_t* src_address;
         address_t dst_address;

         src_address = ((uint8_t**)buffer)[0];
         color_t r, g, b;

         for (int x=0; x<dstImg->width(); x++) {
            dst_address = dstImg->writable_addr32(x, dinfo.output_scanline-1);
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
      
      for(int i = 0; i < (int)buffer_height; i++)
         delete[] buffer[i];
      delete[] buffer;

      jpeg_finish_decompress(&dinfo);
      jpeg_destroy_decompress(&dinfo);
      fclose(file);
      return true;
   }
} // namespace img32