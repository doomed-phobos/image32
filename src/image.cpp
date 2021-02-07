#include "image_priv.h"

#include "image32/io.h"

#include <cassert>

namespace img32
{
   Image::Image()
   {}

   Image::Image(const ImageInfo& info) :
   m_info(info)
   {
      m_totalSize = m_info.getRowStrideBytes()*height();

      m_buffer.resize(m_totalSize);
   }

   const void* Image::getPixels() const
   {
      return m_buffer.buffer();
   }

   void* Image::getPixels()
   {
      return m_buffer.buffer();
   }

   void Image::setPixels(uint8_t* pixels)
   {
      std::copy(pixels, pixels + m_totalSize, m_buffer.begin());
   }

   const_address_t Image::addr32() const
   {
      assert(m_info.getBytesPerPixel() == 4);
      return reinterpret_cast<const_address_t>(m_buffer.buffer());
   }

   const_address_t Image::addr32(int x, int y) const
   {
      assert(x < width());
      assert(y < height());
      return (const_address_t)((const char*)addr32() + y*rowBytes() + (x << 2));
   }

   address_t Image::writable_addr32(int x, int y) const
   {
      return const_cast<address_t>(addr32(x, y));
   }

   void Image::fill(color_t c)
   {
      address_t first = writable_addr32(0, 0);
      std::fill(first, first+width(), c);

      for(int y = 1; y < height(); y++)
         std::copy(first, first+width(), writable_addr32(0, y));
   }

   void Image::putPixel(int x, int y, color_t c)
   {
      *writable_addr32(x, y) = c;
   }

   color_t Image::getPixel(int x, int y) const
   {
      return *addr32(x, y);
   }

   bool Image::isValid() const
   {
      return width() > 0 && height() > 0;
   }

   bool Image::loadFromFilename(const char filename[], ColorType ct)
   {
      ImageIO io(filename);
      return io.decode(this, ct);
   }

   bool Image::saveToFilename(const_charp filename, const EncoderOptions& eo)
   {
      ImageIO io(filename);
      return io.encode(*this, eo);
   }

   void Image::reset(const ImageInfo& info)
   {
      *this = Image(info);
   }
} // namespace img32