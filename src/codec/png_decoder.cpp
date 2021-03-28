#include "png_decoder.h"

#include "../image_priv.h"
#include "../string.h"

namespace img32
{
   PngDecoder::PngDecoder(const ColorType& colorType, CodecErrorCallback callback) :
      ImageDecoder(colorType, callback)
   {}

   PngDecoder::~PngDecoder()
   {
      png_destroy_read_struct(&m_png, &m_info, nullptr);
   }

   Result PngDecoder::init(FILE* file)
   {
      m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)this, png_show_error, png_show_error);
      if(!m_png) return Result::Failed;

      m_info = png_create_info_struct(m_png);
      if(!m_info) return Result::Failed;

      if(setjmp(png_jmpbuf(m_png))) {
         png_destroy_read_struct(&m_png, &m_info, nullptr);
         return Result::Failed;
      }

      png_init_io(m_png, file);

      return Result::Success;
   }

   void PngDecoder::png_show_error(png_structp png, png_const_charp error_msg)
   {
      PngDecoder* self = reinterpret_cast<PngDecoder*>(png_get_error_ptr(png));
      self->onError(format_to_string("libpng: %s", error_msg).c_str());
   }

   Result PngDecoder::decode(ImageData* imgData)
   {
      png_color_16p png_transp_color = nullptr;
      png_uint_32 width;
      png_uint_32 height;
      png_bytepp rows_pointer;
      int bit_depth;
      int color_type;

      png_read_info(m_png, m_info);
      png_get_IHDR(m_png, m_info, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
      png_set_strip_16(m_png);
      png_set_packing(m_png);

      if(m_colorType == Unchanged_ColorType) {
         switch(png_get_color_type(m_png, m_info)) {
         case PNG_COLOR_TYPE_RGB: m_colorType = RGB_888x_ColorType; break;
         case PNG_COLOR_TYPE_RGBA: m_colorType = RGBA_8888_ColorType; break;
         case PNG_COLOR_TYPE_PALETTE: m_colorType = Indexed_ColorType; break;
         }
      }else if(m_colorType != Indexed_ColorType){
         if(color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(m_png);

         if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            png_set_expand_gray_1_2_4_to_8(m_png);
      }

      int number_passes = png_set_interlace_handling(m_png);

      png_read_update_info(m_png, m_info);

      imgData->reset(ImageInfo::Make(width, height, ColorInfo::Make(m_colorType)));

      png_colorp palette;
      int num_palette;
      if(m_colorType == Indexed_ColorType && png_get_PLTE(m_png, m_info, &palette, &num_palette)) {
         Palette* imgPalette = imgData->getPalette();

         imgPalette->resize(num_palette);

         for(int c = 0; c < num_palette; c++)
            imgPalette->setColor(c, rgba(palette[c].red, palette[c].green, palette[c].blue));
         
         png_bytep trans = NULL;
         int num_trans;
         png_get_tRNS(m_png, m_info, &trans, &num_trans, NULL);
         
         for(int i = 0; i < num_trans; i++) {
            int c = imgPalette->getColor(i);
            int r = getR(c);
            int g = getG(c);
            int b = getB(c);

            imgPalette->setColor(i, rgba(r, g, b, trans[i]));
         }
      }else {
         png_get_tRNS(m_png, m_info, NULL, NULL, &png_transp_color);
      }

      rows_pointer = (png_bytepp)png_malloc(m_png, sizeof(png_bytep)*height);
      for(uint32_t y = 0; y < height; y++)
         rows_pointer[y] = (png_bytep)png_malloc(m_png, png_get_rowbytes(m_png, m_info));
   
      for(int pass = 0; pass < number_passes; pass++)
         for(uint32_t y = 0; y < height; y++)
            png_read_rows(m_png, rows_pointer+y, nullptr, 1);

      for(uint32_t y = 0; y < height; y++) {
         uint8_t* src_address = rows_pointer[y];
         uint8_t r, g, b, a;   
      
         if(png_get_color_type(m_png, m_info) == PNG_COLOR_TYPE_RGBA) {
            for(uint32_t x = 0; x < width; x++) {
               uint32_t* dst_address = imgData->writable_addr32(x, y);
               r = *(src_address++);
               g = *(src_address++);
               b = *(src_address++);
               a = *(src_address++);
               SetPixelAddressByColorType(dst_address, m_colorType, r, g, b, a);
            }
         }else if(png_get_color_type(m_png, m_info) == PNG_COLOR_TYPE_RGB) {
            for(uint32_t x = 0; x < width; x++) {
               uint32_t* dst_address = imgData->writable_addr32(x, y);
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

               SetPixelAddressByColorType(dst_address, m_colorType, r, g, b, a);
            }
         }else if(png_get_color_type(m_png, m_info) == PNG_COLOR_TYPE_GRAY) {
            for(uint32_t x = 0; x < width; x++) {
               uint32_t* dst_address = imgData->writable_addr32(x, y);
               uint32_t grayscale = *(src_address++);
               r = grayscale;
               g = grayscale;
               b = grayscale;

               if(png_transp_color &&
                  grayscale == png_transp_color->gray) {
                  a = 0;
               }else
                  a = 255;

               SetPixelAddressByColorType(dst_address, m_colorType, r, g, b, a);
            }
         }else if(png_get_color_type(m_png, m_info) == PNG_COLOR_TYPE_GRAY_ALPHA) {
            for(uint32_t x = 0; x < width; x++) {
               uint32_t* dst_address = imgData->writable_addr32(x, y);
               uint32_t grayscale = *(src_address++);
               r = grayscale;
               g = grayscale;
               b = grayscale;
               a = *(src_address++);

               SetPixelAddressByColorType(dst_address, m_colorType, r, g, b, a);
            }
         }else if(png_get_color_type(m_png, m_info) == PNG_COLOR_TYPE_PALETTE) {
            for (int x = 0; x < width; x++) {
               uint8_t* dst_address = imgData->writable_addr8(x, y);
               *(dst_address++) = *(src_address++);
            }
         }

         png_free(m_png, rows_pointer[y]);
      }

      png_free(m_png, rows_pointer);
      return Result::Success;
   }
} // namespace img32
