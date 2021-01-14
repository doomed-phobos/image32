#include "image32/io.h"

#include "image_priv.h"
#include "file.h"

#include "time.h"

#include <memory>

namespace img32
{
   ImgIO::ImgIO(const char* filename) :
   ImgIO(filename, RGBA_8888)
   {}

   ImgIO::ImgIO(const char* filename, ColorType ct) :
   m_filename(filename),
   m_colorType(ct),
   m_delegate(nullptr)
   {}

   void ImgIO::onError(const char* msg)
   {
      if(m_delegate)
         m_delegate->OnError(msg);
   }

   bool ImgIO::decode(Image* dstImg)
   {
	  Timer time;
      std::unique_ptr<FileIO> io(new NoneIO);
      
      switch(get_image_format(m_filename)) {
      case ImageFormat::JPEG:
         io.reset(new JpgIO);
         break;
      case ImageFormat::BMP:
         io.reset(new BmpIO);
         break;
      case ImageFormat::PNG:
         io.reset(new PngIO);
         break;
      }

      return io->decode(this, dstImg);
   }
} // namespace img32
