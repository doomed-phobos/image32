#include "image_priv.h"
//TODO: Error al abrir imagenes con color de modo PALETTE
#include "file.h"
#include "string.h"

#include "png.h"

namespace img32
{
   void png_show_error(png_structp png, png_const_charp error_msg)
   {
      ImgIO* err = (ImgIO*)png_get_error_ptr(png);
      err->onError(format_to_string("libpng: %s", error_msg).c_str());
   }

   bool PngIO::decode(ImgIO* io, Image* dstImg)
   {
      FILE* file = open_file(io->filename(), "rb");
      png_uint_32 width = 0;
      png_uint_32 height = 0;
      png_bytepp rows_pointer;
      int bit_depth = 0;
      int color_type = 0;
      int sig_read = 0;
      png_color_16p png_transp = NULL;

      png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, (ImgIO*)io, png_show_error, png_show_error);
      if(!png) return false;
      png_set_option(png, PNG_SKIP_sRGB_CHECK_PROFILE, PNG_OPTION_ON);

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
                   nullptr, NULL, NULL);

      png_set_expand(png);

      if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
         png_set_expand_gray_1_2_4_to_8(png);
      if(color_type == PNG_COLOR_TYPE_PALETTE)
         png_set_palette_to_rgb(png);
      if(png_get_valid(png, info, PNG_INFO_tRNS))
         png_set_tRNS_to_alpha(png);

      if(bit_depth == 16)
         png_set_strip_16(png);
      png_set_packing(png);

      int number_passes = png_set_interlace_handling(png);

      png_read_update_info(png, info);

      int imgWidth = png_get_image_width(png, info);
      int imgHeight = png_get_image_height(png, info);

      *dstImg = Image(ImageInfo::Make(imgWidth, imgHeight, io->colorType()));

      rows_pointer = (png_bytepp)png_malloc(png, sizeof(png_bytep)*height);
      for(uint32_t y = 0; y < height; y++)
         rows_pointer[y] = (png_bytep)png_malloc(png, png_get_rowbytes(png, info));

      for(int pass = 0; pass < number_passes; pass++)
         for(uint32_t y = 0; y < height; y++)
            png_read_rows(png, rows_pointer+y, nullptr, 1);

      for(uint32_t y = 0; y < height; y++) {
         uint8_t* src_address = rows_pointer[y];
         color_t r, g, b, a;
         for(uint32_t x = 0; x < width; x++) {
         address_t dst_address = dstImg->writable_addr32(x, y);
            if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGBA) {
                  r = *(src_address++);
                  g = *(src_address++);
                  b = *(src_address++);
                  a = *(src_address++);
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB) {
                  r = *(src_address++);
                  g = *(src_address++);
                  b = *(src_address++);
                  if(png_transp &&
                  r == png_transp->red &&
                  g == png_transp->green &&
                  b == png_transp->blue) {
                     a = 0;
                  }else a = 255;
            }else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_PALETTE) {
                  r = *(src_address++);
                  g = *(src_address++);
                  b = *(src_address++);
                  a = *(src_address++);
            }
            
            SetPixelsIntoAddress(dst_address, dstImg->colorType(), r, g, b, a);
         }
         png_free(png, rows_pointer[y]);
      }

      png_free(png, rows_pointer);
      png_destroy_read_struct(&png, &info, nullptr);
      fclose(file);
      return true;
   }
} // namespace img32
