#ifndef IO_PNG_H
#define IO_PNG_H
#include "io.h"

namespace img32
{
   class PngIO : public ImgIO
   {
   public:
      PngIO(const char filename[]);
      virtual bool decode(Image* dstImg, ColorType ct) override;
   private:
      FILE* m_file;
   };
} // namespace img32


#endif