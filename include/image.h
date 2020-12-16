#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>

namespace img32
{
   typedef uint8_t pixel_t;
   typedef pixel_t* address_t;

   enum class ImageFormat
   {
      JPEG,
      PNG
   };

   enum class ColorSpace
   {
      UNKNOWN,
      GRAYSCALE,
      RGB
   };

   struct image_t
   {
      uint32_t width;
      uint32_t height;
      ColorSpace colorSpace;
      address_t pixels;
   };

   /// @brief Read a image with JPG format
   bool image_from_filename(image_t* dstImg, const char filename[]);
} // namespace img32

#endif