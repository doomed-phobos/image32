#ifndef IO_BMP_H
#define IO_BMP_H
#include "io.h"

namespace img32
{
   class BmpIO : public ImgIO
   {
   public:
      BmpIO(const char filename[]);
      virtual bool decode(Image* dstImg, ColorType ct) override;
   private:
      FILE* m_file;
   };
} // namespace img32


#endif