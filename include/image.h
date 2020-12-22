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
      UNKNOWN,
      JPEG,
      PNG,
      WEBP,
      BMP
   };

   enum class PixelFormat
   {
      RGBA,
      RGB,
      BGRA
   };

   class Image
   {
   private:
      uint32_t m_width;
      uint32_t m_height;
      PixelFormat m_pixelFormat;
      address_t* m_rows;
      address_t m_bits;
      std::vector<uint8_t> m_buffer;
   
      inline address_t getLineAddress(int y) const {
         assert(y >= 0 && y <= height());
         return m_rows[y];
      }
   public:
      ///@brief Crea una clase Image con formato de pixel RGB por defecto
      Image();

      inline address_t address(int x, int y) const {
         return (address_t)(getLineAddress(y) + x);
      }

      void setPixelFormat(const PixelFormat pf);

      address_t getPixelAddress(int x, int y) const;
      address_t getPixels() const;

      uint32_t width() const;
      uint32_t height() const;
      PixelFormat pixelFormat() const;

      static Image Make(const int width, const int height, PixelFormat pf);
      static Image Make(const int width, const int height) {
         return Make(width, height, PixelFormat::RGB);
      }
   };

   /// @brief Read a image with JPG, BMP, PNG format
   bool image_from_filename(Image* dstImg, const char filename[]);
} // namespace img32

#endif