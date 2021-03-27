#include "io_priv.h"

#include "file.h"
#include "string.h"

#include "png.h"

namespace img32::priv
{
   void png_show_error(png_structp png, png_const_charp error_msg)
   {
      ImageIOPriv* impl = (ImageIOPriv*)png_get_error_ptr(png);
      impl->onError(format_to_string("libpng: %s", error_msg).c_str());
   }
   
   bool ImageIOPriv::png_decode(Image* dstImg, ColorType ct)
   {
      FileHandle hfile = open_file(filename(), "rb");
      FILE* file = hfile.get();
      png_bytepp rows_pointer;
      png_uint_32 width, height;
      int bit_depth = 0;
      int color_type = 0;
      int sig_read = 0;

      png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)this, png_show_error, png_show_error);
      if(!png) return false;

      png_infop info = png_create_info_struct(png);
      if(!info) return false;

      if(setjmp(png_jmpbuf(png))) {
         png_destroy_read_struct(&png, &info, nullptr);
         return false;
      }

      png_init_io(png, file);
      png_set_sig_bytes(png, sig_read);
      png_read_info(png, info);
      png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type,
                   NULL, NULL, NULL);

      png_set_strip_16(png);
      png_set_packing(png);
      
      if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
         png_set_expand_gray_1_2_4_to_8(png);
      if(color_type == PNG_COLOR_TYPE_PALETTE)
         png_set_palette_to_rgb(png);

      int number_passes = png_set_interlace_handling(png);
   
      png_read_update_info(png, info);

      png_color_16p png_transp_color = NULL;
      png_get_tRNS(png, info, NULL, NULL, &png_transp_color);
   
      dstImg->reset(ImageInfo::Make(width, height, ct));
      
      rows_pointer = (png_bytepp)png_malloc(png, sizeof(png_bytep)*height);
      for(uint32_t y = 0; y < height; y++)
         rows_pointer[y] = (png_bytep)png_malloc(png, png_get_rowbytes(png, info));
      
      for(int pass = 0; pass < number_passes; pass++)
         for(uint32_t y = 0; y < height; y++)
            png_read_rows(png, rows_pointer+y, nullptr, 1);

      for(uint32_t y = 0; y < height; y++) {
         uint8_t* src_address = rows_pointer[y];
         channel_t r, g, b, a;
         for(uint32_t x = 0; x < width; x++) {
            uint32_t* dst_address = dstImg->writable_addr32(x, y);

            if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB_ALPHA) {
               r = *(src_address++);
               g = *(src_address++);
               b = *(src_address++);
               a = *(src_address++);
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB) {
               r = *(src_address++);
               g = *(src_address++);
               b = *(src_address++);
               if(png_transp_color &&
                  r == png_transp_color->red &&
                  g == png_transp_color->green &&
                  b == png_transp_color->blue) {
                     a = 0;
               }else 
                  a = 255;
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY) {
               uint32_t gray = *(src_address++);
               r = gray;
               g = gray;
               b = gray;

               if(png_transp_color &&
                  gray == png_transp_color->gray) {
                     a = 0;
               }else
                  a = 255;
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY_ALPHA) {
               uint32_t gray = *(src_address++);
               r = gray;
               g = gray;
               b = gray;
               a = *(src_address++);
            }

            set_pixels_into_address(dst_address, dstImg->colorType(), r, g, b, a);
         }
         png_free(png, rows_pointer[y]);
      }

      png_free(png, rows_pointer);
      png_destroy_read_struct(&png, &info, nullptr);
      return true;
   }

   bool ImageIOPriv::png_encode(const Image& srcImg, const EncoderOptions& options)
   {
      FileHandle hFile = open_file(filename(), "wb");
      FILE* file = hFile.get();
      png_structp png;
      png_infop info;
      png_bytep row_pointer;
      int color_type = 0;

      png = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)this, png_show_error, png_show_error);
      if(!png) return false;

      info = png_create_info_struct(png);
      if(!info) {
         png_destroy_write_struct(&png, nullptr);
         return false;
      }

      if(setjmp(png_jmpbuf(png))) {
         png_destroy_write_struct(&png, &info);
         return false;
      }

      png_init_io(png, file);
      
      switch(options.colortype) {
      case EncoderOptions::RGB_ColorType:             color_type = PNG_COLOR_TYPE_RGB;        break;
      case EncoderOptions::RGBA_ColorType:            color_type = PNG_COLOR_TYPE_RGBA;       break;
      case EncoderOptions::Grayscale_ColorType:       color_type = PNG_COLOR_TYPE_GRAY;       break;
      case EncoderOptions::GrayscaleA_ColorType:      color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
      }

      png_set_IHDR(png, info, srcImg.width(), srcImg.height(), 8,
         color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
         PNG_FILTER_TYPE_BASE);

      png_write_info(png, info);
      png_set_packing(png);

      row_pointer = (png_bytep)png_malloc(png, png_get_rowbytes(png, info));
      for(png_uint_32 y = 0; y < srcImg.height(); y++) {
         uint8_t* dst_address = row_pointer;
         for(png_uint_32 x = 0; x < srcImg.width(); x++) {
            color_t c = srcImg.getPixel(x, y);

            if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGBA) {
               *(dst_address++) = getR(c);
               *(dst_address++) = getG(c);
               *(dst_address++) = getB(c);
               *(dst_address++) = getA(c);
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB) {
               *(dst_address++) = getR(c);
               *(dst_address++) = getG(c);
               *(dst_address++) = getB(c);
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY) {
               *(dst_address++) = (options.fix_grayscale ? rgba_to_gray(c) : c);
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_GRAY_ALPHA) {
               *(dst_address++) = (options.fix_grayscale ? rgba_to_gray(c) : c);
               *(dst_address++) = getA(c);
            }
         }

         png_write_rows(png, &row_pointer, 1);
      }

      png_write_end(png, info);
      png_free(png, row_pointer);

      return true;
   }
} // namespace img32::priv