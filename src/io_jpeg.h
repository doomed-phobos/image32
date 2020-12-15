#ifndef IO_JPEG_H
#define IO_JPEG_H
#include "io.h"

#include <cstdio>

class JpegIO : public IO
{
public:
   JpegIO(const char filename[]);
   virtual bool decode(image_t* dstImg) override;
private:
   FILE* m_file;
};

#endif