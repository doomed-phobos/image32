#ifndef IO_PRIV_H
#define IO_PRIV_H
#include "image32/io.h"
#include "file.h"

namespace img32::priv
{
   class ImageIOPriv
   {
   public:
      ImageIOPriv(const_charp filename) :
      ImageIOPriv(filename, RGBA_8888) {}
      
      ImageIOPriv(const_charp filename, ColorType ct) :
      m_filename(filename),
      m_colorType(ct),
      m_delegate(nullptr) {}

      void setErrorDelegate(IOErrorDelegate* delegate) {m_delegate = delegate;}

      bool decode(Image* dstImg) {
         switch(get_image_format(m_filename)) {
         case ImageFormat::JPEG:
            return jpg_decode(dstImg);
            break;
         case ImageFormat::BMP:
            return bmp_decode(dstImg);
            break;
         case ImageFormat::PNG:
            return png_decode(dstImg);
            break;
         }
         
         return false;
      }

      void onError(const_charp msg) {
         if(m_delegate)
            m_delegate->OnError(msg);
      }

      const_charp filename() const {return m_filename;}
      ColorType colorType() const {return m_colorType;}
   private:
      bool png_decode(Image* dstImg);
      bool bmp_decode(Image* dstImg);
      bool jpg_decode(Image* dstImg);

      IOErrorDelegate* m_delegate;
      const_charp m_filename;
      ColorType m_colorType;
   };
} // namespace img32::priv

#endif