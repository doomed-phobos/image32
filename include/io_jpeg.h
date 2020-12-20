#ifndef IO_JPEG_H
#define IO_JPEG_H
#include "io.h"

#include <cstdio>

namespace img32
{
   class JpegIO : public IO
   {
   public:
      JpegIO(const char filename[]);
      virtual bool decode(Image* dstImg) override;
   private:
      FILE* m_file;
   };
} // namespace img32

#endif