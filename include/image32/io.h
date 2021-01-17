#ifndef IO_H
#define IO_H
#include "image.h"

namespace img32
{ 
   namespace priv {class ImageIOPriv;}

   class IOErrorDelegate
   {
   public:
      virtual void OnError(const_charp msg) = 0;
   };

   class IMG32_API ImageIO
   {
   public:
      ImageIO(const_charp filename);
      ImageIO(const_charp filename, ColorType ct);

      void setErrorDelegate(IOErrorDelegate* delegate);

      bool decode(Image* dstImg);
   private:
      priv::ImageIOPriv* m_impl;
   };
} // namespace img32

#endif