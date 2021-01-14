#ifndef IMAGE_H
#define IMAGE_H
#include "image_info.h"

#include <cstdint>
#include <vector>

namespace img32
{
   typedef uint32_t pixel_t;
   typedef pixel_t* address_t;
   typedef const pixel_t* const_address_t;

   /// Clase principal de esta librería.
   /// Para cargar desde un archivo use Image::loadFromFilename
   class IMG32_API Image
   {
   public:
      /// Crear una imagen vacia (Width = 0 y Height = 0)
      Image() {}
      /// Crea una clase Image con formato de pixel 'RGB' por defecto
      Image(const ImageInfo& info);

      /// Establece los pixeles de esta imagen
      /// 
      /// Los pixeles deben ser calculados con img32::rgba
      /// 
      /// Asegurese que el arreglo de pixeles coincida con el total
      /// de pixeles (RowStrideBytes * ImageHeight)
      void setPixels(address_t pixels);
      const void* getPixels() const {return &m_pixels[0];}

      const_address_t addr32() const;
      const_address_t addr32(int x, int y) const;
      address_t writable_addr32(int x, int y) const;

      size_t rowBytes() const {return m_info.minRowBytes();}
      int width() const {return m_info.width();}
      int height() const {return m_info.height();}
      ColorType colorType() const {return m_info.colorType();}
      int getBytesPerPixel() const {return m_info.getBytesPerPixel();}

      /// Carga una imagen con un tipo determinado de color dentro
      /// de un ImgIO sin ninguna configuracion
      bool loadFromFilename(const char filename[], ColorType ct);
      bool loadFromFilename(const char filename[]) {
         return loadFromFilename(filename, colorType());
      }
   private:
      size_t m_requiredSize;
      ImageInfo m_info;
      std::vector<uint32_t> m_pixels;
   };
} // namespace img32

#endif