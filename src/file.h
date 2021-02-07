#ifndef FILE_H
#define FILE_H
#include "image_priv.h"

#include <string>
#include <cstdint>
#include <memory>

struct FileDeleter
{
   void operator()(FILE* file) {
      if(file)
         fclose(file);
   }
};

/// Archivo que llama a fclose cuando sale del contexto
typedef std::unique_ptr<FILE, FileDeleter> FileHandle;

FileHandle open_file(std::string filename, std::string mode);
std::string get_extension_from_file(const std::string& filename);

void offset(FILE* file, long offset);
uint8_t read8(FILE* file);

namespace little_endian
{
   uint16_t read16(FILE* file);
   uint32_t read32(FILE* file);
   bool write16(FILE* file, uint16_t word);
   bool write32(FILE* file, uint32_t dword);
} // namespace little_endian

namespace img32
{
   ImageFormat get_image_format_by_extension(const std::string& filename);
   ImageFormat get_image_format_by_content(const std::string& filename);
   ImageFormat get_image_format_by_content(const uint8_t buf[8]);
} // namespace img32


#endif