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

namespace img32
{
   ImageFormat get_image_format(const std::string& filename);
   ImageFormat get_image_format(const uint8_t buf[8]);
} // namespace img32


#endif