#ifndef IO_H
#define IO_H
#include "image.h"

class IO
{
public:
   virtual bool decode(image_t* dstImg) = 0;
};

#endif