#ifndef FILE_H
#define FILE_H
#include <cstdint>
#include <iosfwd>

void offset(FILE* file, long offset);
uint8_t read8(FILE* file);

namespace little_endian
{
   uint16_t read16(FILE* file);
   uint32_t read32(FILE* file);
} // namespace little_endian

namespace big_endian
{
   uint16_t read16(FILE* file);
   uint32_t read32(FILE* file);
} // namespace big_endian


#endif