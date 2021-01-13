#ifndef PIXEL_FORMAT_H
#define PIXEL_FORMAT_H

namespace img32
{
   /// El modo de encodificacion/decodificacion de un pixel
   ///
   /// Por defecto se usa RGBA_8888
   enum ColorType
   {
      ARGB_8888, //Pixel con 8 bits para A, R, G, B; dentro de un uint32
      RGBA_8888, //Pixel con 8 bits para R, G, B, A; dentro de un uint32
      RGB_888x,  //Pixel con 8 bits para R, G, B; dentro de un uint32
      BGRA_8888, //Pixel con 8 bits para B, G, R, A; dentro de un uint32
   };
} // namespace img32


#endif