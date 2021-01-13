#ifndef IMAGE_PRIV_H
#define IMAGE_PRIV_H
#include "image32/image.h"
#include "image32/color.h"

namespace img32
{
   enum class ImageFormat
   {
      UNKNOWN,
      JPEG,
      PNG,
      BMP
   };

   inline void SetPixelsIntoAddress(void* src_addr, ColorType ct, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
   {
      address_t addr = (address_t)src_addr;
      switch(ct) {
      case RGB_888x:
         *(addr++) = rgba(r, g, b);
         break;
      case RGBA_8888:
         *(addr++) = rgba(r, g, b, a);
         break;
      case ARGB_8888:
         *(addr++) = rgba(a, r, g, b);
         break;
      case BGRA_8888:
         *(addr++) = rgba(b, g, r, a);
         break;
      }
   }
} // namespace img32


#endif