#include "file.h"

#include <cstdio>

void offset(FILE* file, long offset)
{
   fseek(file, offset, SEEK_SET);
}

uint8_t read8(FILE* file)
{
   return fgetc(file);
}

namespace little_endian
{
   uint16_t read16(FILE* file)
   {
      uint8_t b1 = read8(file);
      uint8_t b2 = read8(file);

      return ((b2 << 8) | b1);
   }

   uint32_t read32(FILE* file)
   {
      uint8_t b1 = read8(file);
      uint8_t b2 = read8(file);
      uint8_t b3 = read8(file);
      uint8_t b4 = read8(file);

      return ((b4 << 24) |
              (b3 << 16) |
              (b2 << 8)  |
               b1);
   }
} // namespace little_endian

namespace big_endian
{
   uint16_t read16(FILE* file)
   {
      uint8_t b2 = read8(file);
      uint8_t b1 = read8(file);

      return ((b2 << 8) | b1);
   }
   uint32_t read32(FILE* file)
   {
      uint8_t b4 = read8(file);
      uint8_t b3 = read8(file);
      uint8_t b2 = read8(file);
      uint8_t b1 = read8(file);

      return ((b4 << 24) |
              (b3 << 16) |
              (b2 << 8)  |
               b1);
   }
} // namespace big_endian