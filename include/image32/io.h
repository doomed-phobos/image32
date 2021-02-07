#ifndef IO_H
#define IO_H
#include "image.h"
#include "encoder_options.h"
//TODO: Seguir con la escritura de imagenes
namespace img32
{ 
   namespace priv {class ImageIOPriv;}

   class IOErrorDelegate
   {
   public:
      virtual void OnError(const_charp msg) = 0;
   };

   class IMG32_DLL ImageIO
   {
   public:
      ImageIO(const_charp filename);
      ~ImageIO();

      void setErrorDelegate(IOErrorDelegate* delegate);

      bool decode(Image* dstImg, ColorType ct = RGBA_8888);
      bool encode(const Image& srcImg, const EncoderOptions& options);

      void reset(const_charp filename);
   private:
      priv::ImageIOPriv* m_impl;
   };
} // namespace img32

#endif