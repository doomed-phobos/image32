#ifndef _JPEG_DECODER_H
#define _JPEG_DECODER_H
#include "image_decoder.h"

#include <csetjmp>

extern "C"
{
#include "jpeglib.h"
}

namespace img32
{

   class JpegDecoder : public ImageDecoder
   {
   public:
      JpegDecoder(const ColorType& colorType, CodecErrorCallback callback);
      ~JpegDecoder();

      Result init(FILE* file);
      virtual Result decode(ImageData* imgData) override;
   private:
      struct error_mgr
      {
         jpeg_error_mgr jerr;
         jmp_buf setjmp_buf;
         // TODO: Si se coloca antes de jmp_buf, causa un error
         JpegDecoder* decoder;
      };
      
      static void jpeg_error_exit(j_common_ptr cinfo);

      jpeg_decompress_struct m_dinfo;
      error_mgr m_err;
   };
} // namespace img32


#endif