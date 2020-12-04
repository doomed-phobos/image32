#ifndef IO_H
#define IO_H
#include "image.h"

class IO
{
public:
   virtual image_t decode() = 0;
};

IO* image_from_filename();

#endif