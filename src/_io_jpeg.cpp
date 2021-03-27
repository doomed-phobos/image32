#include "io_priv.h"

#include "string.h"
#include "file.h"
#include "time.h"

#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C"
{
#include "jpeglib.h"
}

namespace img32::priv
{
   struct error_mgr
   {
      jpeg_error_mgr jerr;
      jmp_buf setjmp_buf;
      //Si se coloca antes de jmp_buf, causa un error
      ImageIOPriv* io;
   };

   void jpg_error_exit(j_common_ptr cinfo)
   {
      error_mgr* err = (error_mgr*)cinfo->err;
      char buf[JMSG_LENGTH_MAX];
      
      err->jerr.format_message(cinfo, buf);
      err->io->onError(format_to_string("jpeglib: %s", buf).c_str());
      longjmp(err->setjmp_buf, 1);
   }

   bool ImageIOPriv::jpg_decode(Image* dstImg, ColorType ct)
   {
      FileHandle file = open_file(filename(), "rb");
      jpeg_decompress_struct dinfo;
      error_mgr err;

      err.io = this;
      dinfo.err = jpeg_std_error(&err.jerr);
      err.jerr.error_exit = jpg_error_exit;

      if(setjmp(err.setjmp_buf)) {
         jpeg_destroy_decompress(&dinfo);
         return false;
      }

      jpeg_create_decompress(&dinfo);
      jpeg_stdio_src(&dinfo, file.get());
      (void)jpeg_read_header(&dinfo, true);

      if(dinfo.jpeg_color_space == JCS_GRAYSCALE)
         dinfo.out_color_space = JCS_GRAYSCALE;
      else
         dinfo.out_color_space = JCS_RGB;

      (void)jpeg_start_decompress(&dinfo);

      int width = dinfo.output_width;
      int height = dinfo.output_height;
      dstImg->reset(ImageInfo::Make(width, height, ct)); 

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
         channel_t r, g, b;

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
            
            set_pixels_into_address(dst_address, dstImg->colorType(), r, g, b, 255);
         }
      }
      
      for(int i = 0; i < (int)buffer_height; i++)
         delete[] buffer[i];
      delete[] buffer;

      jpeg_finish_decompress(&dinfo);
      jpeg_destroy_decompress(&dinfo);
      return true;
   }

   bool ImageIOPriv::jpg_encode(const Image& srcImg, const EncoderOptions& options)
   {
      FileHandle file = open_file(filename(), "wb");
      jpeg_compress_struct cinfo;
      error_mgr err;
      JSAMPARRAY buffer;
      JDIMENSION buffer_height;

      err.io = this;
      cinfo.err = jpeg_std_error(&err.jerr);
      err.jerr.error_exit = jpg_error_exit;

      jpeg_create_compress(&cinfo);
      jpeg_stdio_dest(&cinfo, file.get());

      cinfo.image_width = srcImg.width();
      cinfo.image_height = srcImg.height();

      
      if((options.colortype & ~EncoderOptions::Alpha_BitMask) == EncoderOptions::Grayscale_ColorType) {
         cinfo.input_components = 1;
         cinfo.in_color_space = JCS_GRAYSCALE;
      }else {
         cinfo.input_components = 3;
         cinfo.in_color_space = JCS_RGB;
      }

      jpeg_set_defaults(&cinfo);
      jpeg_set_quality(&cinfo, options.jpg_quality, TRUE);

      jpeg_start_compress(&cinfo, true);

      buffer_height = 1;
      buffer = new JSAMPROW[buffer_height];
      if(!buffer) {
         jpeg_destroy_compress(&cinfo);
         return false;
      }
      
      for(int c = 0; c < (int)buffer_height; c++) {
         buffer[c] = new JSAMPLE[cinfo.image_width * cinfo.num_components];
         if(!buffer[c]) {
            for(c--; c >= 0; c--)
               delete[] buffer[c];
            delete[] buffer;
            jpeg_destroy_compress(&cinfo);
            return false;
         }
      }

      while(cinfo.next_scanline < cinfo.image_height) {
         uint8_t* dst_address = ((uint8_t**)buffer)[0];
         const_address_t src_address;

         for(int x = 0; x < (int)srcImg.width(); x++) {
            color_t c = srcImg.getPixel(x, cinfo.next_scanline);

            if(options.colortype & EncoderOptions::Alpha_BitMask) {
               if(getA(c) < 255)
                  c = combine_to_solid_color(c, options.jpg_background);
            }else
               if(getA(c) < 255)
                  c = options.jpg_background;              

            if((options.colortype & ~EncoderOptions::Alpha_BitMask) == EncoderOptions::RGB_ColorType) {
               *(dst_address++) = getR(c);
               *(dst_address++) = getG(c);
               *(dst_address++) = getB(c);
            }else {
               *(dst_address++) = (options.fix_grayscale ? rgba_to_gray(c) : c);
            }
         }
         jpeg_write_scanlines(&cinfo, buffer, buffer_height);
      }

      delete[] buffer;
      jpeg_finish_compress(&cinfo);
      jpeg_destroy_compress(&cinfo);

      return true;
   }
} // namespace img32::priv