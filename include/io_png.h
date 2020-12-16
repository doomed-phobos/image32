#ifndef IO_PNG_H
#define IO_PNG_H
#include "io.h"

namespace img32
{
   class PngIO : public IO
   {
   public:
      PngIO(const char filename[]);
      virtual bool decode(image_t* dstImg) override;
   };
} // namespace img32


#endif