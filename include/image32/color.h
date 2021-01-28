#ifndef COLOR_H
#define COLOR_H

namespace img32
{
   enum {
      ColorShiftR = 0,
      ColorShiftG = 8,
      ColorShiftB = 16,
      ColorShiftA = 24,
   };

   /// Funcion que empaqueta los canales individuales de un color dentro de un UINT32
   constexpr inline color_t rgba(channel_t r, channel_t g, channel_t b, channel_t a = 255)
   {
      return color_t((r << ColorShiftR) |
                     (g << ColorShiftG) |
                     (b << ColorShiftB) |
                     (a << ColorShiftA));
   }

   constexpr inline color_t bgra(channel_t b, channel_t g, channel_t r, channel_t a = 255) {return rgba(b, g, r, a);}
   constexpr inline color_t argb(channel_t a, channel_t r, channel_t g, channel_t b) {return rgba(r, g, b, a);}

   //////////////////// Retorna los canales de un color //////////////////////////
   constexpr inline channel_t getR(color_t c) {return (c >> ColorShiftR) & 0xff;}
   constexpr inline channel_t getG(color_t c) {return (c >> ColorShiftG) & 0xff;}
   constexpr inline channel_t getB(color_t c) {return (c >> ColorShiftB) & 0xff;}
   constexpr inline channel_t getA(color_t c) {return (c >> ColorShiftA) & 0xff;}
   ///////////////////////////////////////////////////////////////////////////////

   constexpr inline bool isTransparent(color_t c) {return getA(c) == 0;}

   constexpr inline color_t combine_to_solid_color(color_t a, color_t b)
   {
      float alphaA = getA(a) / 255.f;
      channel_t resR = alphaA*getR(a) + (1-alphaA)*getR(b);
      channel_t resG = alphaA*getG(a) + (1-alphaA)*getG(b);
      channel_t resB = alphaA*getB(a) + (1-alphaA)*getB(b);
      
      return rgba(resR, resG, resB);
   }
} // namespace img32


#endif