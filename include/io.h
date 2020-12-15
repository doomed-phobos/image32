#ifndef IO_H
#define IO_H
#include "image.h"

namespace img32
{
   class IO
   {
   public:
      virtual bool decode(image_t* dstImg) = 0;
   };
} // namespace img32

#endif