#include "../file/file.h"
#include "../string.h"

#include <sys/stat.h>
#include <windows.h>

static constexpr const char g_path_separator = '\\';

bool is_directory(const std::string& filename)
{
   DWORD attr = ::GetFileAttributesW(from_utf8(filename).c_str());

   return ((attr != INVALID_FILE_ATTRIBUTES) && 
           (attr & FILE_ATTRIBUTE_DIRECTORY));
}

size_t get_file_size(const std::string& filename)
{
   struct _stat stat;
#ifdef _WIN32
   return (_wstat(from_utf8(filename).c_str(), &stat) == 0) ? stat.st_size : 0;
#else
   return (_stat(filename, &stat) == 0) ? stat.st_size : 0;
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