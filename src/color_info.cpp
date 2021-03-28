#include "color_info.h"

namespace img32
{
   ColorInfo::ColorInfo(ColorType ct) :
      m_colorType(ct)
   {}

   int ColorInfo::bytesPerPixel() const
   {
      switch(m_colorType) {
      case BGRA_8888_ColorType:
      case RGB_888x_ColorType:
      case ARGB_8888_ColorType:
      case RGBA_8888_ColorType:
         return 4;
         break;
      case Indexed_ColorType:    return 1; break;
      }

      return 0;
   }

   ColorType ColorInfo::colorType() const
   {
      return m_colorType;
   }

   ColorInfo ColorInfo::MakeUnknown()
   {
      return ColorInfo(Unknown_ColorType);
   }

   ColorInfo ColorInfo::MakeUnchanged()
   {
      return ColorInfo(RGB_888x_ColorType);
   }

   ColorInfo ColorInfo::Make(ColorType ct)
   {
      return ColorInfo(ct);
   }
} // namespace img32
