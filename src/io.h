#ifndef IO_H
#define IO_H
#include "image32/image.h"

#include <cstdio>

namespace img32
{ 
   /// La clase base que decodifica las archivos de imagenes
   class ImgIO
   {
   public:
      virtual bool decode(Image* dstImg, ColorType ct) = 0;
      bool decode(Image* dstImg) {return decode(dstImg, ColorType::RGBA_8888);}
   };
} // namespace img32

#endif