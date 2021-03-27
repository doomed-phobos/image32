#ifndef ENCODER_OPTIONS_H
#define ENCODER_OPTIONS_H
#include "color.h"

namespace img32
{
   struct IMG32_DLL EncoderOptions
   {
      enum ColorType {
         Alpha_BitMask        = 0x08,
         RGB_ColorType        = 0x02,
         RGBA_ColorType       = 0x02 | Alpha_BitMask,
         Grayscale_ColorType  = 0x04,
         GrayscaleA_ColorType = 0x04 | Alpha_BitMask
      };

      ColorType colortype;
      /// Si es true, se arregla la exportacion de grayscale
      /// para imagenes pequeñas.
      /// De lo contrario, la exportacion no sería perfecta
      bool fix_grayscale;

      //========================
      // JPG
      //========================
      int jpg_quality;
      color_t jpg_background;

      color_t bmp_background;

      /// Retorna una estructura EncoderOptions con valores por defecto:
      /// colortype = RGBA
      /// fix_grayscale = false
      /// jpg_quality = 100
      /// jpg_background = White
      static EncoderOptions Default() {
         EncoderOptions opt;
         opt.colortype = RGB_ColorType;
         opt.jpg_quality = 100;
         opt.jpg_background = rgba(255, 255, 255);
         opt.fix_grayscale = false;
         opt.bmp_background = rgba(255, 255, 255);
         return opt;
      }
   };
} // namespace img32

#endif