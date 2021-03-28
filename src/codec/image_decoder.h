#ifndef _IMAGE_DECODER_H
#define _IMAGE_DECODER_H
#include "image_error.h"
#include "color_info.h"
#include "result.h"

#include <cstdio>

namespace img32
{
   class ImageData;

   class ImageDecoder
   {
   public:
      ImageDecoder(const ColorType& colorType, CodecErrorCallback callback);
      virtual ~ImageDecoder() {}

      virtual Result init(FILE* file) {return Result::Success;}
      virtual Result decode(ImageData* imgData) = 0;
   protected:
      void onError(const char* msg);

      ColorType m_colorType;
      CodecErrorCallback m_callback;
   };
} // namespace img32

#endif