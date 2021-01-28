#ifndef IMAGE_INFO_H
#define IMAGE_INFO_H
#include "config.h"

#include <cctype> 

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

   /// Clase que crea informacion de una imagen a partir de su ancho, alto y tipo de color
   class IMG32_API ImageInfo
   {
   public:
      ImageInfo();
      ImageInfo(int width, int height, ColorType ct);

      void changeColorType(const ColorType& ct) {m_colorType = ct;}

      int getBytesPerPixel() const;
      int getRowStrideBytes() const;

      bool validRowBytes(size_t rowBytes) const;

      int shiftPerPixel() const;
      size_t minRowBytes() const;
      int width() const {return m_width;}
      int height() const {return m_height;}
      ColorType colorType() const {return m_colorType;}

      static ImageInfo Make(int width, int height, ColorType ct);
   private:
      int m_width;
      int m_height;
      ColorType m_colorType;
   };
} // namespace img32


#endif