#include "file.h"

#include "string.h"

#ifdef _WIN32
   #include <windows.h>
   static constexpr char g_path_separator = '\\';
#else
   #define ARRAYSIZE(buf) sizeof(buf) / sizeof(buf[0])
   static constexpr char g_path_separator = '/';
#endif

#include <cstdio>

#define BMP_MAGIC_NUMBER 0x4D42
#define JPG_MAGIC_NUMBER 0xD8FF
#define PNG_MAGIC_NUMBER1 0x474E5089
#define PNG_MAGIC_NUMBER2 0x0A1A0A0D

FileHandle open_file(std::string filename, std::string mode)
{
#ifdef _WIN32
   return FileHandle(_wfopen(from_utf8(filename).c_str(), from_utf8(mode).c_str()));
#else
   return FileHandle(fopen(filename.c_str(), mode.c_str()));
#endif
}

std::string get_extension_from_file(const std::string& filename)
{
   std::string::const_reverse_iterator rit;
   std::string result;

   for(rit = filename.rbegin(); rit != filename.rend(); ++rit) {
      if(*rit == '.') break;
      if(*rit == g_path_separator) return std::string();
   }

   if(rit != filename.rend())
      std::copy(std::string::const_iterator(rit.base()), filename.end(), std::back_inserter(result));

   return result;
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

   bool write16(FILE* file, uint16_t word)
   {
      uint8_t b2 = (word >> 8) & 0xff;
      uint8_t b1 = word & 0xff;

      if(fputc(b1, file) == b1)
         if(fputc(b2, file) == b2)
            return true;
      
      return false;
   }

   bool write32(FILE* file, uint32_t dword)
   {
      uint8_t b4 = (dword >> 24) & 0xff;
      uint8_t b3 = (dword >> 16) & 0xff;
      uint8_t b2 = (dword >> 8)  & 0xff;
      uint8_t b1 = dword & 0xff;

      if(fputc(b1, file) == b1)
         if(fputc(b2, file) == b2)
            if(fputc(b3, file) == b3)
               if(fputc(b4, file) == b4)
                  return true;
      
      return false;
   }
} // namespace little_endian

namespace img32
{
   ImageFormat get_image_format_by_extension(const std::string& filename)
   {
      std::string ext = string_to_lower(get_extension_from_file(filename));

      if(ext == "jpg" || ext == "jpeg") return ImageFormat::JPEG;
      if(ext == "png") return ImageFormat::PNG;
      if(ext == "bmp") return ImageFormat::BMP;

      return ImageFormat::UNKNOWN;
   }

   ImageFormat get_image_format_by_content(const std::string& filename)
   {
      FileHandle file;
      if(file = open_file(filename, "rb"); !file) return ImageFormat::UNKNOWN;
      uint8_t buf[8];
      
      fread(buf, 1, ARRAYSIZE(buf), file.get());

      return get_image_format_by_content(buf);
   }

   ImageFormat get_image_format_by_content(const uint8_t buf[8])
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
