#ifndef _BMP_DECODER_H
#define _BMP_DECODER_H
#include "image_decoder.h"

#include "libbmp.h"

#include <csetjmp>

namespace img32
{
   class BmpDecoder : public ImageDecoder
   {
   public:
      BmpDecoder(const ColorType& colorType, CodecErrorCallback callback);
      ~BmpDecoder();

      Result init(FILE* file);
      virtual Result decode(ImageData* imgData) override;
   private:
      static void bmp_show_error(bmp_struct_ptr bmp_ptr, bmp_const_charp error_msg);

      bmp_struct_ptr m_bmp;
   };
} // namespace img32


#endif