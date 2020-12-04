#ifndef IO_JPEG_H
#define IO_JPEG_H
#include "io.h"

class JpegIO : public IO
{
public:
   virtual image_t decode(const char filename[]) override;
};

#endif