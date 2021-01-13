#ifndef IO_JPEG_H
#define IO_JPEG_H
#include "io.h"

namespace img32
{
   class JpegIO : public ImgIO
   {
   public:
      JpegIO(const char filename[]);
      virtual bool decode(Image* dstImg, ColorType ct) override;
   private:
      FILE* m_file;
   };
} // namespace img32

#endif