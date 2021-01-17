#include "image32/io.h"

#include "io_priv.h"
#include "time.h"

namespace img32
{
   ImageIO::ImageIO(const_charp filename) :
   ImageIO(filename, RGBA_8888)
   {}

   ImageIO::ImageIO(const_charp filename, ColorType ct) :
   m_impl(new priv::ImageIOPriv(filename, ct))
   {}

   void ImageIO::setErrorDelegate(IOErrorDelegate* delegate)
   {
      m_impl->setErrorDelegate(delegate);
   }

   bool ImageIO::decode(Image* dstImg)
   {
	   Timer time;
      return m_impl->decode(dstImg);
   }
} // namespace img32
