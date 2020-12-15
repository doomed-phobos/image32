#include "image.h"

#include "io_jpeg.h"

#include <memory>

bool image_from_filename(image_t* dstImg, const char filename[])
{
   std::unique_ptr<IO> io(new JpegIO(filename));

   return io->decode(dstImg);
}