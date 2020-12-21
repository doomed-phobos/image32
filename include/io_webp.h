#ifndef IO_WEBP_H
#define IO_WEBP_H
#include "io.h"

#include <cstdio>

namespace img32
{
   class WebpIO : public IO
   {
   public:
      WebpIO(const char filename[]);
      virtual bool decode(Image* dstImg) override;
   private:
      FILE* m_file;
   };
} // namespace img32


#endif