#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>
#include <vector>
#include <cassert>

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
      RGB,
      INDEXED
   };

   class Image
   {
   private:
      uint32_t m_width;
      uint32_t m_height;
      ColorSpace m_colorSpace;
      address_t* m_rows;
      address_t m_bits;
      std::vector<uint8_t> m_buffer;
   
      inline address_t getLineAddress(int y) const {
         assert(y >= 0 && y <= height());
         return m_rows[y];
      }
   public:
      Image();

      inline address_t address(int x, int y) const {
         return (address_t)(getLineAddress(y) + x);
      }

      address_t getPixelAddress(int x, int y) const;
      address_t getPixels() const;

      uint32_t width() const;
      uint32_t height() const;
      ColorSpace colorSpace() const;

      static Image Make(const int width, const int height, ColorSpace cs);
   };

   /// @brief Read a image with JPG format
   bool image_from_filename(Image* dstImg, const char filename[]);
} // namespace img32

#endif