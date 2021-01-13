#include "image_priv.h"

#include "file.h"
#include "io_png.h"
#include "io_jpeg.h"
#include "io_none.h"
#include "io_bmp.h"

#include <cassert>
#include <memory>

namespace img32
{
   Image::Image(const ImageInfo& info) :
   m_info(info)
   {
      m_requiredSize = m_info.getRowStrideBytes()*height();

      m_pixels.resize(m_requiredSize);
   }

   void Image::setPixels(address_t pixels)
   {
      std::move(pixels, pixels + m_requiredSize, m_pixels.begin());
   }

   const_address_t Image::addr32() const
   {
      assert(m_info.getBytesPerPixel() == 4);
      return reinterpret_cast<const_address_t>(&m_pixels[0]);
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

   bool Image::loadFromFilename(const char filename[], ColorType ct)
   {
	  if(!filename) return false;
      std::unique_ptr<ImgIO> io(new NoneIO());
      //TODO: Escribir los pixeles para BMP, como en JpegIO
      switch(get_image_format(filename)) {
      case ImageFormat::JPEG:
         io.reset(new JpegIO(filename));
         break;
      case ImageFormat::BMP:
         io.reset(new BmpIO(filename));
         break;
      case ImageFormat::PNG:
         io.reset(new PngIO(filename));
         break;
      }

      return io->decode(this, ct);
   }
} // namespace img32