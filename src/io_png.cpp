#include "io_priv.h"

#include "file.h"
#include "string.h"

#include "png.h"
//TODO: Error al abrir imagenes con color de modo PALETTE

#define PNG_DEBUG

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
            address_t dst_address = dstImg->writable_addr32(x, y);

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
      FileHandle file = open_file(filename(), "wb");
      png_structp png;
      png_infop info;

      if(!(png = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)this, png_show_error, png_show_error))) return false;

      return true;
   }
} // namespace img32::priv
