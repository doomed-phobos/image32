#include "image32/io.h"

#include "io_priv.h"
#include "time.h"

namespace img32
{
   ImageIO::ImageIO(const_charp filename) :
   m_impl(new priv::ImageIOPriv(filename)) {}

   ImageIO::~ImageIO() {delete m_impl;}

   void ImageIO::setErrorDelegate(IOErrorDelegate* delegate)
   {
      m_impl->setErrorDelegate(delegate);
   }

   bool ImageIO::decode(Image* dstImg, ColorType ct)
   {
      Timer timer;
      return m_impl->decode(dstImg, ct);
   }

   bool ImageIO::encode(const Image& srcImg, const EncoderOptions& options)
   {
      Timer timer;
      return m_impl->encode(srcImg, options);
   }

   void ImageIO::reset(const_charp filename)
   {
      m_impl->reset(filename);
   }
} // namespace img32
