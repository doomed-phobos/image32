#include "image_decoder.h"

namespace img32
{
   ImageDecoder::ImageDecoder(const ColorType& colorType, CodecErrorCallback callback) :
      m_colorType(colorType),
      m_callback(callback)
   {}

   void ImageDecoder::onError(const char* msg)
   {
      if(m_callback)
         m_callback(msg);
   }
} // namespace img32
