#ifndef IO_H
#define IO_H
#include "image.h"

class IO
{
public:
   virtual image_t decode(const char filename[]) = 0;
};

#endif