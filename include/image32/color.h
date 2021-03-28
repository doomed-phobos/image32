#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace img32
{
   enum {
      ColorShiftR = 0,
      ColorShiftG = 8,
      ColorShiftB = 16,
      ColorShiftA = 24,
   };

   /// Funcion que empaqueta los canales individuales de un color dentro de un UINT32
   constexpr inline uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
   {
      return uint32_t((r << ColorShiftR) |
                      (g << ColorShiftG) |
                      (b << ColorShiftB) |
                      (a << ColorShiftA));
   }
   
   //////////////////// Retorna los canales de un color //////////////////////////
   constexpr inline uint8_t getR(uint32_t c) {return (c >> ColorShiftR) & 0xff;}
   constexpr inline uint8_t getG(uint32_t c) {return (c >> ColorShiftG) & 0xff;}
   constexpr inline uint8_t getB(uint32_t c) {return (c >> ColorShiftB) & 0xff;}
   constexpr inline uint8_t getA(uint32_t c) {return (c >> ColorShiftA) & 0xff;}
   ///////////////////////////////////////////////////////////////////////////////

   constexpr inline uint32_t combine_to_solid_color(uint32_t a, uint32_t b)
   {
      float alphaA = getA(a) / 255.f;
      uint8_t resR = alphaA*getR(a) + (1-alphaA)*getR(b);
      uint8_t resG = alphaA*getG(a) + (1-alphaA)*getG(b);
      uint8_t resB = alphaA*getB(a) + (1-alphaA)*getB(b);
      
      return rgba(resR, resG, resB);
   }
} // namespace img32


#endif