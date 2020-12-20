#ifndef IO_PNG_H
#define IO_PNG_H
#include "io.h"

#include <cstdio>

namespace img32
{
   class PngIO : public IO
   {
   public:
      PngIO(const char filename[]);
      virtual bool decode(Image* dstImg) override;
   private:
      FILE* m_file;
   };
} // namespace img32


#endif