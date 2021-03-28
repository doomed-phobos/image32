#ifndef _PNG_DECODER_H
#define _PNG_DECODER_H
#include "image_decoder.h"

#include <png.h>

namespace img32
{
   class PngDecoder : public ImageDecoder
   {
   public:
      PngDecoder(const ColorType& colorType, CodecErrorCallback callback);
      ~PngDecoder();

      Result init(FILE* file);
      virtual Result decode(ImageData* imgData) override;
   private:
      static void png_show_error(png_structp png, png_const_charp error_msg);

      png_structp m_png;
      png_infop m_info;
   };
} // namespace img32


#endif