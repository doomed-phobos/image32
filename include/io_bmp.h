#ifndef IO_BMP_H
#define IO_BMP_H
#include "io.h"

#include <cstdio>

namespace img32
{
   class BmpIO : public IO
   {
   public:
      BmpIO(const char filename[]);
      virtual bool decode(Image* dstImg) override;
   private:
      FILE* m_file;
   };
} // namespace img32


#endif