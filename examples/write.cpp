#include "image32/image32.h"

#include <iostream>

int main()
{
   img32::ImageCodec codec(img32::Unchanged_ColorType);
   img32::ImageData data;

   codec.decode(&data, "C:\\Users\\Usuario\\Desktop\\Imagenes\\YlzsO.png");

   uint32_t c = data.getPalette()->getColor(14);

   return 0;
}