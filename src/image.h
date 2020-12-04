#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>
typedef uint8_t pixel_t;
typedef pixel_t* address_t;

struct image_t
{

   uint32_t width;
   uint32_t height;
   address_t pixels;
};

image_t image_from_filename(const char filename[]);

#endif