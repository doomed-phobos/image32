#ifndef _CODEC_UNKNOWN_DECODER_H
#define _CODEC_UNKNOWN_DECODER_H
#include "image_decoder.h"

namespace img32
{
   class UnknownDecoder : public ImageDecoder
   {
   public:
      UnknownDecoder(CodecErrorCallback callback) : ImageDecoder(Unknown_ColorType, callback) {}

      virtual Result decode(ImageData* imgData) override {
         onError("No existe un decodifador para el format que usted intenta pasar");
         return Result::UnsupportedFormat;
      }
   };
} // namespace img32


#endif