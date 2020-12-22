#include "io_png.h"

#include "png.h"

#define GETA(color) ((color >> 24) & 0xff)
#define GETR(color) ((color >> 0) & 0xff)
#define GETG(color) ((color >> 8) & 0xff)
#define GETB(color) ((color >> 16) & 0xff)

namespace img32
{
   PngIO::PngIO(const char filename[])
   {
      m_file = fopen(filename, "rb");
   }

   void png_show_error(png_structp png, png_const_charp error_msg)
   {
      printf("libpng: %s\n", error_msg);
   }

   bool PngIO::decode(Image* dstImg)
   {
      if(!m_file) return false;
      png_uint_32 width = 0;
      png_uint_32 height = 0;
      png_bytepp rows_pointer;
      int bit_depth = 0;
      int color_type = 0;
      int interlace_type = 0;
      int sig_read = 0;
      png_color_16p png_transp = NULL;

      png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_show_error, png_show_error);
      if(!png) return false;
      png_set_option(png, PNG_SKIP_sRGB_CHECK_PROFILE, PNG_OPTION_ON);

      png_infop info = png_create_info_struct(png);
      if(!info) return false;

      if(setjmp(png_jmpbuf(png))) {
         puts("Error reading PNG file");
         return false;
      }

      png_init_io(png, m_file);
      png_set_sig_bytes(png, sig_read);
      png_read_info(png, info);
      png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type,
                   &interlace_type, NULL, NULL);
      png_set_strip_16(png);
      png_set_packing(png);

      if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
         png_set_expand_gray_1_2_4_to_8(png);

      int number_passes = png_set_interlace_handling(png);

      png_read_update_info(png, info);

      int imgWidth = png_get_image_width(png, info);
      int imgHeight = png_get_image_height(png, info);

      *dstImg = Image::Make(imgWidth, imgHeight, dstImg->pixelFormat());

      png_get_tRNS(png, info, nullptr, nullptr, &png_transp);

      rows_pointer = (png_bytepp)png_malloc(png, sizeof(png_bytep)*height);
      for(int y = 0; y < height; y++)
         rows_pointer[y] = (png_bytep)png_malloc(png, png_get_rowbytes(png, info));

      for(int pass = 0; pass < number_passes; pass++)
         for(int y = 0; y < height; y++)
            png_read_rows(png, rows_pointer+y, nullptr, 1);

      for(int y = 0; y < height; y++) {
         uint8_t* src_address = rows_pointer[y];
         uint8_t* dst_address = (uint8_t*)dstImg->getPixelAddress(0, y);
         int r, g, b, a;
         for(int x = 0; x < width; x++) {
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
                  int rgba = *(src_address++);
                  r = GETB(rgba);
                  g = GETG(rgba);
                  b = GETR(rgba);
                  a = GETA(rgba);
            }

            switch(dstImg->pixelFormat()) {
            case PixelFormat::RGB:
               *(dst_address++) = r;
               *(dst_address++) = g;
               *(dst_address++) = b;
               break;
            case PixelFormat::RGBA:
               *(dst_address++) = r;
               *(dst_address++) = g;
               *(dst_address++) = b;
               *(dst_address++) = a;
               break;
            case PixelFormat::BGRA:
               *(dst_address++) = b;
               *(dst_address++) = g;
               *(dst_address++) = r;
               *(dst_address++) = a;
               break;
            }
         }
         png_free(png, rows_pointer[y]);
      }

      png_free(png, rows_pointer);

      return true;
   }
} // namespace img32
