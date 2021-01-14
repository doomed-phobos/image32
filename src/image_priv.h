#ifndef IMAGE_PRIV_H
#define IMAGE_PRIV_H
#include "image32/image.h"
#include "image32/io.h"
#include "image32/color.h"

namespace img32
{
   enum class ImageFormat
   {
      UNKNOWN,
      JPEG,
      PNG,
      BMP
   };

//////////////////////////////////////////////////////////////////////////////////////
   class FileIO
   {
   public:
      virtual bool decode(ImgIO* io, Image* dstImg) = 0;
   };

   class NoneIO : public FileIO
   {
   public:
      virtual bool decode(ImgIO* io, Image* dstImg) override {return false;}
   };

   class JpgIO : public FileIO
   {
   public:
      virtual bool decode(ImgIO* io, Image* dstImg) override;
   private:
   };

   class PngIO : public FileIO
   {
   public:
      virtual bool decode(ImgIO* io, Image* dstImg) override;
   };

   class BmpIO : public FileIO
   {
   public:
      virtual bool decode(ImgIO* io, Image* dstImg) override;
   };
//////////////////////////////////////////////////////////////////////////////////////

   inline void SetPixelsIntoAddress(uint32_t* src_addr, ColorType ct, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
   {
      switch(ct) {
      case RGB_888x:
         *src_addr = rgba(r, g, b);
         break;
      case RGBA_8888:
         *src_addr = rgba(r, g, b, a);
         break;
      case ARGB_8888:
         *src_addr = rgba(a, r, g, b);
         break;
      case BGRA_8888:
         *src_addr = rgba(b, g, r, a);
         break;
      }
   }
} // namespace img32

#endif