#include "image.h"

#include "io_jpeg.h"

#include <memory>

image_t image_from_filename(const char filename[])
{
   std::unique_ptr<IO> io(new JpegIO);

   return io->decode(filename);
}