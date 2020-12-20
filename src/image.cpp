#include "image.h"

#include "io_png.h"
#include "io_jpeg.h"
#include "io_none.h"

#include <memory>
#include <string>

//===================================================================
img32::ImageFormat get_image_format(const std::string& filename);
std::string get_extension(const std::string& filename);
std::string string_to_lower(const std::string& str);
//===================================================================

img32::ImageFormat get_image_format(const std::string& filename)
{
   std::string ext = string_to_lower(get_extension(filename));
   img32::ImageFormat format;

   if(ext == "jpg" || ext == "jpeg") format = img32::ImageFormat::JPEG;
   if(ext == "png") format = img32::ImageFormat::PNG;

   return format;
}

std::string string_to_lower(const std::string& original)
{
   std::string result(original);

   auto it = result.begin();
   
   while(it != result.end()) {
      *it = tolower(*it);
      it++;
   }

   return result;
}

std::string get_extension(const std::string& filename)
{
   std::string::const_reverse_iterator rit;
   std::string ext;

   for(rit = filename.rbegin(); rit < filename.rend(); rit++) {
      if(*rit == '.') break;
   }

   if(rit != filename.rend()) {
      std::copy(std::string::const_iterator(rit.base()), filename.end(), std::back_inserter(ext));
   }

   return ext;
}

namespace img32
{
   Image::Image() :
   m_width(0),
   m_height(0),
   m_rows(nullptr),
   m_colorSpace(ColorSpace::UNKNOWN)
   {}

   Image Image::Make(const int width, const int height, ColorSpace cs)
   {
      Image image;
      image.m_colorSpace = cs;
      image.m_width = width;
      image.m_height = height;

      std::size_t for_rows = sizeof(address_t) * height;
      std::size_t rowstride_bytes = width * 4;
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

   ColorSpace Image::colorSpace() const
   {
      return m_colorSpace;
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
      
      if(get_image_format(filename) == ImageFormat::JPEG) io.reset(new JpegIO(filename));
      else if(get_image_format(filename) == ImageFormat::PNG) io.reset(new PngIO(filename));

      return io->decode(dstImg);
   }
} // namespace img32