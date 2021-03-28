#ifndef _IMAGE_PRIV_H
#define _IMAGE_PRIV_H
#include "image_data.h"
#include "color.h"

namespace img32
{
   inline void SetPixelAddressByColorType(address_t addr, ColorType ct, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
   {
      switch(ct) {
      case RGB_888x_ColorType:   *addr =  rgba(r, g, b);    break;
      case RGBA_8888_ColorType:  *addr = rgba(r, g, b, a);  break;
      case ARGB_8888_ColorType:  *addr = rgba(a, r, g, b);  break;
      case BGRA_8888_ColorType:  *addr = rgba(b, g, r, a);  break;
      }
   }

   inline void AddColorPaletteByColorType(Palette* palette, ColorType ct, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
   {
      switch(ct) {
      case RGB_888x_ColorType:   palette->addColor(rgba(r, g, b));      break;
      case RGBA_8888_ColorType:  palette->addColor(rgba(r, g, b, a));   break;
      case ARGB_8888_ColorType:  palette->addColor(rgba(a, r, g, b));   break;
      case BGRA_8888_ColorType:  palette->addColor(rgba(b, g, r, a));   break;
      }
   }
} // namespace img32


#endif