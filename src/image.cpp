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
   bool image_from_filename(image_t* dstImg, const char filename[])
   {
      std::unique_ptr<IO> io(new NoneIO);
      
      if(get_image_format(filename) == ImageFormat::JPEG) io.reset(new JpegIO(filename));
      else if(get_image_format(filename) == ImageFormat::PNG) io.reset(new PngIO(filename));

      return io->decode(dstImg);
   }
} // namespace img32