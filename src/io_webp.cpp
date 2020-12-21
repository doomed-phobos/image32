#include "io_webp.h"

//#include "webp/decode.h"

#include <cstdio>
#include <vector>

namespace img32
{
   WebpIO::WebpIO(const char filename[])
   {
      m_file = fopen(filename, "rb");
   }

   bool WebpIO::decode(Image* dstImg)
   {
      // TODO: Soon
      return false;
   }
} // namespace img32
