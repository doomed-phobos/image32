#ifndef IO_H
#define IO_H
#include "image.h"

#include <cstdio>

namespace img32
{ 
   class IOErrorDelegate
   {
   public:
      virtual void OnError(const char* msg) = 0;
   };

   class IMG32_API ImgIO
   {
   public:
      ImgIO(const char* filename);
      ImgIO(const char* filename, ColorType ct);

      void setError(IOErrorDelegate* delegate) {m_delegate = delegate;}

      bool decode(Image* dstImg);

      ColorType colorType() const {return m_colorType;}

      const char* filename() const {return m_filename;}

      void onError(const char* msg);
   private:
      IOErrorDelegate* m_delegate;
      ColorType m_colorType;
      const char* m_errMsg;
      const char* m_filename;
      FILE* m_file;
   };
} // namespace img32

#endif