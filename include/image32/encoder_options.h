#ifndef ENCODER_OPTIONS_H
#define ENCODER_OPTIONS_H
#include "color.h"

namespace img32
{
   struct EncoderOptions
   {
      enum ColorType {
         RGBA_ColorType,
         GRAYSCALE_ColorType,
         INDEXED_ColorType
      };


      //========================
      // JPG
      //========================
      ColorType colortype;
      int jpg_quality;
      // Color que reemplazará a los colores transparentes
      color_t jpg_background;
   };
} // namespace img32

#endif