#ifndef _CODEC_IMAGE_CODEC_H
#define _CODEC_IMAGE_CODEC_H
#include "image_error.h"
#include "file/file_handle.h"
#include "color_info.h"

namespace img32
{
   class ImageData;

   class ImageCodec
   {
   public:
      ImageCodec(const ColorType& colorType);

      void setErrorCallback(CodecErrorCallback callback);
      Result decode(ImageData* imgData, const char* filename);
   private:
      ColorType m_colorType;
      CodecErrorCallback m_callback;
   };
} // namespace img32

#endif