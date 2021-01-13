#ifndef IO_NONE_H
#define IO_NONE_H
#include "io.h"

namespace img32
{
   class NoneIO : public ImgIO
   {
   public:
      virtual bool decode(Image* dstImg, ColorType ct) override {
         puts("Image extension not supported!");
         return false;
      }
   };
} // namespace img32


#endif