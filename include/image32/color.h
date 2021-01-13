#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace img32
{
   typedef uint32_t color_t;
   typedef uint8_t channel_t;

   enum {
      ColorShiftR = 0,
      ColorShiftG = 8,
      ColorShiftB = 16,
      ColorShiftA = 24,
   };

   /// Funcion que empaqueta los canales individuales de un color dentro de un UINT32
   constexpr inline color_t rgba(channel_t r, channel_t g, channel_t b, channel_t a = 255)
   {
      return uint32_t((r << ColorShiftR) |
                      (g << ColorShiftG) |
                      (b << ColorShiftB) |
                      (a << ColorShiftA));
   }

   //////////////////// Retorna los canales de un color //////////////////////////
   constexpr inline channel_t getR(color_t c) {return (c >> ColorShiftR) & 0xff;}
   constexpr inline channel_t getG(color_t c) {return (c >> ColorShiftG) & 0xff;}
   constexpr inline channel_t getB(color_t c) {return (c >> ColorShiftB) & 0xff;}
   constexpr inline channel_t getA(color_t c) {return (c >> ColorShiftA) & 0xff;}
   ///////////////////////////////////////////////////////////////////////////////
} // namespace img32


#endif