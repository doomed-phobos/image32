#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>

struct image_t
{
   typedef uint8_t pixel_t;

   uint32_t width;
   uint32_t height;
   pixel_t* pixels;
};

#endif