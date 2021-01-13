#include "image32/image_info.h"

#include <cassert>

namespace img32
{
   static inline int colorTypeBytesPerPixel(ColorType ct)
   {
      switch(ct) {
      case RGBA_8888:   return 4; break;
      case RGB_888x:    return 4; break;
      case BGRA_8888:   return 4; break;
      case ARGB_8888:   return 4; break;
      default:          return 0; break;
      } 
   }

   static inline int colorTypeShiftPerPixel(ColorType ct)
   {
      switch(ct) {
      case RGBA_8888:   return 2; break;
      case RGB_888x:    return 2; break;
      case BGRA_8888:   return 2; break;
      case ARGB_8888:   return 2; break;
      default:          return 0; break;
      } 
   }

   ImageInfo::ImageInfo() :
   m_width(0),
   m_height(0),
   m_colorType(RGBA_8888)
   {}

   ImageInfo::ImageInfo(int width, int height, ColorType ct) :
   m_width(width),
   m_height(height),
   m_colorType(ct)
   {
      assert(m_width > 0 && m_height > 0);
   }

   bool ImageInfo::validRowBytes(size_t rowBytes) const
   {
      int shift = shiftPerPixel();
      size_t alignedRowBytes = rowBytes >> shift << shift;
      return rowBytes == alignedRowBytes;
   }

   int ImageInfo::shiftPerPixel() const {return colorTypeShiftPerPixel(m_colorType);}
   size_t ImageInfo::minRowBytes() const {return width() * getBytesPerPixel();}
   int ImageInfo::getBytesPerPixel() const {return colorTypeBytesPerPixel(m_colorType);}
   int ImageInfo::getRowStrideBytes() const {return getBytesPerPixel() * m_width;}

   ImageInfo ImageInfo::Make(int width, int height, ColorType ct)
   {
      ImageInfo info(width, height, ct);
      return info;
   }
   //TODO: Hacer que se almacene correctamente el tamaño para una imagen
} // namespace img32
