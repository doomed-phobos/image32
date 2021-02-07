#ifndef COLOR_H
#define COLOR_H
#include "config.h"
#include <cstdint>

namespace img32
{
   typedef uint8_t channel_t;
   typedef uint32_t color_t;
   
   enum {
      ColorShiftR = 0,
      ColorShiftG = 8,
      ColorShiftB = 16,
      ColorShiftA = 24,
   };

   /// Funcion que empaqueta los canales individuales de un color dentro de un UINT32
   constexpr inline color_t IMG32_DLL rgba(channel_t r, channel_t g, channel_t b, channel_t a = 255)
   {
      return color_t((r << ColorShiftR) |
                     (g << ColorShiftG) |
                     (b << ColorShiftB) |
                     (a << ColorShiftA));
   }

   constexpr inline color_t IMG32_DLL bgra(channel_t b, channel_t g, channel_t r, channel_t a = 255) {return rgba(b, g, r, a);}
   constexpr inline color_t IMG32_DLL argb(channel_t a, channel_t r, channel_t g, channel_t b) {return rgba(r, g, b, a);}

   //////////////////// Retorna los canales de un color //////////////////////////
   constexpr inline channel_t IMG32_DLL getR(color_t c) {return (c >> ColorShiftR) & 0xff;}
   constexpr inline channel_t IMG32_DLL getG(color_t c) {return (c >> ColorShiftG) & 0xff;}
   constexpr inline channel_t IMG32_DLL getB(color_t c) {return (c >> ColorShiftB) & 0xff;}
   constexpr inline channel_t IMG32_DLL getA(color_t c) {return (c >> ColorShiftA) & 0xff;}
   ///////////////////////////////////////////////////////////////////////////////

   constexpr inline color_t IMG32_DLL combine_to_solid_color(color_t a, color_t b)
   {
      float alphaA = getA(a) / 255.f;
      channel_t resR = alphaA*getR(a) + (1-alphaA)*getR(b);
      channel_t resG = alphaA*getG(a) + (1-alphaA)*getG(b);
      channel_t resB = alphaA*getB(a) + (1-alphaA)*getB(b);
      
      return rgba(resR, resG, resB);
   }

   constexpr inline color_t IMG32_DLL rgba_to_gray(color_t c) {
      channel_t r = getR(c);
      channel_t g = getG(c);
      channel_t b = getB(c);
      channel_t a = getA(c);
      pixel_t grayscale = ((0.2126f * r) + (0.7152f * g) + (0.0722f * b));

      return rgba(grayscale, grayscale, grayscale, a);
   }
} // namespace img32


#endif