#include "image32/image32.h"
using namespace img32;

#include <iostream>

class ErrorDelegate : public IOErrorDelegate
{
public:
   virtual void OnError(const_charp msg) override {puts(msg);}
};

int main()
{
   Image img(ImageInfo::Make(3, 1, RGBA_8888));
   uint8_t pixels[] = {
      255, 0, 0, 255,
      0, 255, 0, 255,
      0, 0, 255, 255
   };
   ErrorDelegate err;

   EncoderOptions eo;
   eo.colortype = EncoderOptions::RGBA_ColorType;
   eo.jpg_quality = 100;
   eo.jpg_background = rgba(255, 0, 0);
   eo.bmp_background = rgba(0, 255, 0);

   ImageIO io("C:\\Users\\Usuario\\Desktop\\image.jpg");
   io.setErrorDelegate(&err);
   img.setPixels(pixels);
   io.encode(img, eo);
   //saveToFilename("C:\\Users\\Usuario\\Desktop\\image.bmp", eo);

   return 0;
}