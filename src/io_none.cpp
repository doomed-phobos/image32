#include "io_none.h"

#include <cstdio>

namespace img32
{
   bool NoneIO::decode(image_t* dstImg)
   {
      puts("Extension de imagen no soportada!");
      return false;
   }
} // namespace img32
