#include "io_none.h"

#include <cstdio>

namespace img32
{
   bool NoneIO::decode(Image* dstImg)
   {
      *dstImg = Image::Make(0, 0, ColorSpace::UNKNOWN);
      puts("Extension de imagen no soportada!");
      return false;
   }
} // namespace img32
