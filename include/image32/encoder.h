#ifndef ENCODER_H
#define ENCODER_H

enum JpgColorType
{
   JpgColorType_RGB,
   JpgColorType_Grayscale
};
//TODO: Crear la codificacion y EncoderOptions
struct EncoderOptions
{
   //========================
   // JPG
   //========================
   JpgColorType jpg_colortype;
   int jpg_quality;
};

#endif