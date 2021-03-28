#include "bmp_decoder.h"

#include "image_data.h"

namespace img32
{
   BmpDecoder::BmpDecoder(const ColorType& colorType, CodecErrorCallback callback) :
      ImageDecoder(colorType, callback)
   {}

   BmpDecoder::~BmpDecoder()
   {
      bmp_destroy_decoder(&m_bmp);
   }

   Result BmpDecoder::init(FILE* file)
   {
      m_bmp = bmp_create_decoder((void*)this, bmp_show_error, NULL);

      if(setjmp(bmp_setjmp(m_bmp))) {
         bmp_destroy_decoder(&m_bmp);
         return Result::Failed;
      }

      bmp_init_io(m_bmp, file);

      return Result::Success;
   }   

   void BmpDecoder::bmp_show_error(bmp_struct_ptr bmp_ptr, bmp_const_charp error_msg)
   {
      BmpDecoder* self = reinterpret_cast<BmpDecoder*>(bmp_get_error_ptr(bmp_ptr));
      self->onError(error_msg);
   }   

   Result BmpDecoder::decode(ImageData* imgData)
   {
      int width;
      int height;
      int color_type;

      bmp_read_info(m_bmp);

      bmp_get_info(m_bmp, &width, &height, &color_type, NULL, NULL);
      printf("ColorType: %d\n", color_type);

      if(m_colorType == Unchanged_ColorType) {
         m_colorType = (color_type == BMP_COLOR_TYPE_RGB ? BGRA_8888_ColorType : Indexed_ColorType); 
      } else if(m_colorType != Indexed_ColorType) {
         if(color_type == BMP_COLOR_TYPE_INDEXED)
            bmp_set_palette_to_rgb(m_bmp);
      }

      imgData->reset(ImageInfo::Make(width, height, ColorInfo::Make(m_colorType)));

      bmp_read_image(m_bmp, imgData);

      return Result::Success;
   }
} // namespace img32
