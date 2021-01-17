#include "file.h"

#include "string.h"

#ifdef _WIN32
   #include <windows.h>
#endif

#include <cstdio>

#define BMP_MAGIC_NUMBER 0x4D42
#define JPG_MAGIC_NUMBER 0xD8FF
#define PNG_MAGIC_NUMBER1 0x474E5089
#define PNG_MAGIC_NUMBER2 0x0A1A0A0D
#ifndef _WIN32
#define ARRAYSIZE(buf) sizeof(buf) / sizeof(buf[0])
#endif

FileHandle open_file(std::string filename, std::string mode)
{
#ifdef _WIN32
   return FileHandle(_wfopen(from_utf8(filename).c_str(), from_utf8(mode).c_str()));
#else
   return FileHandle(fopen(filename.c_str(), mode.c_str()));
#endif
}

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

namespace img32
{
   ImageFormat get_image_format(const std::string& filename)
   {
         FILE* file = fopen(filename.c_str(), "rb");
         uint8_t buf[8];
         
         fread(buf, 1, ARRAYSIZE(buf), file);

         return get_image_format(buf);
   }

   ImageFormat get_image_format(const uint8_t buf[8])
   {
      #define IS_MAGIC_WORD(offset, word)          \
         (buf[0+offset] == (word & 0xff) &&        \
         (buf[1+offset] == ((word & 0xff00) >> 8)))
      #define IS_MAGIC_DWORD(offset, dword)              \
         (buf[0+offset] == (dword & 0xff) &&             \
         (buf[1+offset] == ((dword & 0xff00) >> 8)) &&   \
         (buf[2+offset] == ((dword & 0xff0000) >> 16)) &&\
         (buf[3+offset] == ((dword & 0xff000000) >> 24)))

      ImageFormat format = ImageFormat::UNKNOWN;

      if(IS_MAGIC_WORD(0, JPG_MAGIC_NUMBER)) format = ImageFormat::JPEG;
      if(IS_MAGIC_WORD(0, BMP_MAGIC_NUMBER)) format = ImageFormat::BMP;
      if(IS_MAGIC_DWORD(0, PNG_MAGIC_NUMBER1) && (IS_MAGIC_DWORD(4, PNG_MAGIC_NUMBER2))) format = ImageFormat::PNG;

      return format;
   }
} // namespace img32
