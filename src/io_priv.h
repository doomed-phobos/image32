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
      m_filename(filename),
      m_delegate(nullptr) {}

      void setErrorDelegate(IOErrorDelegate* delegate) {m_delegate = delegate;}

      bool decode(Image* dstImg, ColorType ct) {
         switch(get_image_format_by_content(m_filename)) {
         case ImageFormat::JPEG:
            return jpg_decode(dstImg, ct);
            break;
         case ImageFormat::BMP:
            return bmp_decode(dstImg, ct);
            break;
         case ImageFormat::PNG:
            return png_decode(dstImg, ct);
            break;
         }
         
         return false;
      }

      bool encode(const Image& srcImg, const EncoderOptions& options) {
         switch(get_image_format_by_extension(m_filename)) {
         case ImageFormat::JPEG:
            return jpg_encode(srcImg, options);
            break;
         case ImageFormat::PNG:
            return png_encode(srcImg, options);
            break;
         case ImageFormat::BMP:
            return bmp_encode(srcImg, options);
            break;
         }
         
         return false;
      }

      void onError(const_charp msg) {
         if(m_delegate)
            m_delegate->OnError(msg);
      }

      const_charp filename() const {return m_filename;}
   
      void reset(const_charp filename) {
         m_filename = filename;
      }
   private:
      bool jpg_encode(const Image& srcImg, const EncoderOptions& options);
      bool png_encode(const Image& srcImg, const EncoderOptions& options);
      bool bmp_encode(const Image& srcImg, const EncoderOptions& options);

      bool png_decode(Image* dstImg, ColorType ct);
      bool bmp_decode(Image* dstImg, ColorType ct);
      bool jpg_decode(Image* dstImg, ColorType ct);

      IOErrorDelegate* m_delegate;
      const_charp m_filename;
   };
} // namespace img32::priv

#endif