#include "image.h"

#include "io_png.h"
#include "io_jpeg.h"
#include "io_webp.h"
#include "io_none.h"
#include "io_bmp.h"

#include <memory>
#include <string>

#define BMP_MAGIC_NUMBER 0x4D42
#define JPG_MAGIC_NUMBER 0xD8FF
#define PNG_MAGIC_NUMBER1 0x474E5089
#define PNG_MAGIC_NUMBER2 0x0A1A0A0D
#define TIFF_MAGIC_NUMBER1 0x002A4949
#define TIFF_MAGIC_NUMBER2 0x00
#define ARRAYSIZE(buf) sizeof(buf) / sizeof(buf[0])

//===================================================================
img32::ImageFormat get_image_format(const std::string& filename);
img32::ImageFormat get_image_format(const uint8_t buf[8]);
//===================================================================

img32::ImageFormat get_image_format(const std::string& filename)
{
   FILE* file = fopen(filename.c_str(), "rb");
   uint8_t buf[8];
   
   fread(buf, 1, ARRAYSIZE(buf), file);

   return get_image_format(buf);
}

img32::ImageFormat get_image_format(const uint8_t buf[8])
{
   #define IS_MAGIC_WORD(offset, word)          \
      (buf[0+offset] == (word & 0xff) &&        \
      (buf[1+offset] == ((word & 0xff00) >> 8)))
   #define IS_MAGIC_DWORD(offset, dword)              \
      (buf[0+offset] == (dword & 0xff) &&             \
      (buf[1+offset] == ((dword & 0xff00) >> 8)) &&   \
      (buf[2+offset] == ((dword & 0xff0000) >> 16)) &&\
      (buf[3+offset] == ((dword & 0xff000000) >> 24)))

   img32::ImageFormat format = img32::ImageFormat::UNKNOWN;

   if(IS_MAGIC_WORD(0, JPG_MAGIC_NUMBER)) format = img32::ImageFormat::JPEG;
   if(IS_MAGIC_WORD(0, BMP_MAGIC_NUMBER)) format = img32::ImageFormat::BMP;
   if(IS_MAGIC_DWORD(0, PNG_MAGIC_NUMBER1) && (IS_MAGIC_DWORD(4, PNG_MAGIC_NUMBER2))) format = img32::ImageFormat::PNG;

   return format;
}

namespace img32
{
   Image::Image() :
   m_width(0),
   m_height(0),
   m_rows(nullptr),
   m_pixelFormat(PixelFormat::RGB)
   {}

   void Image::setPixelFormat(const PixelFormat pf)
   {
      m_pixelFormat = pf;
   }

   Image Image::Make(const int width, const int height, PixelFormat pf)
   {
      Image image;
      image.m_width = width;
      image.setPixelFormat(pf);
      image.m_height = height;
      uint8_t colors = (pf == PixelFormat::BGRA || pf == PixelFormat::RGBA ? 4 : 3);

      std::size_t for_rows = sizeof(address_t) * height;
      std::size_t rowstride_bytes = width * colors;
      std::size_t required_size = for_rows + rowstride_bytes*height;
      image.m_buffer.resize(required_size);
      image.m_rows = (address_t*)&image.m_buffer[0];
      image.m_bits = (address_t)(&image.m_buffer[0] + for_rows);

      address_t addr = image.m_bits;
      for(int y = 0; y < height; y++) {
         image.m_rows[y] = addr;
         addr = (address_t)(((uint8_t*)addr) + rowstride_bytes);
      }

      return image;
   }

   uint32_t Image::width() const
   {
      return m_width;
   }
   
   uint32_t Image::height() const
   {
      return m_height;
   }

   PixelFormat Image::pixelFormat() const
   {
      return m_pixelFormat;
   }

   address_t Image::getPixels() const
   {
      return getPixelAddress(0, 0);
   }

   address_t Image::getPixelAddress(int x, int y) const
   {
      assert(x >= 0 && x <= width());
      assert(y >= 0 && y <= height());
      return address(x, y);
   }

   bool image_from_filename(Image* dstImg, const char filename[])
   {
      std::unique_ptr<IO> io(new NoneIO);
      printf("PixelFOrmat: %d\n", dstImg->pixelFormat());
      
      switch(get_image_format(filename)) {
      case ImageFormat::JPEG:
         io.reset(new JpegIO(filename));
         break;
      case ImageFormat::PNG:
         io.reset(new PngIO(filename));
         break;
      case ImageFormat::WEBP:
         io.reset(new WebpIO(filename));
         break;
      case ImageFormat::BMP:
         io.reset(new BmpIO(filename));
         break;
      }

      return io->decode(dstImg);
   }
} // namespace img32