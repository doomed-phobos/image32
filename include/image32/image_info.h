#ifndef IMAGE_INFO_H
#define IMAGE_INFO_H
#include "color_type.h"

#include <cctype> 

namespace img32
{
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