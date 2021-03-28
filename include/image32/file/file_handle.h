#ifndef _FILE_HANDLE_H
#define _FILE_HANDLE_H
#include "file_format.h"
#include "result.h"

#include <string>
#include <cstdint>
#include <cstdio>

namespace img32
{
   class FileHandle
   {
   public:
      FileHandle();
      FileHandle(const char* filename, const char* mode);
      FileHandle(FileHandle&& other);
      ~FileHandle();

      bool isValid() const;

      Result getStatus() const;
      FILE* getStdFile() const;
      FileFormat getFileFormatByExtension();
      FileFormat getFileFormatByContent();

      FileHandle& operator=(FileHandle&& other);
   private:
      FileHandle(const FileHandle&);
      FileFormat& operator=(const FileHandle&);
      FileFormat getFileFormatByContent(const uint8_t buf[8]);

      FILE* m_file;
      std::string m_filename;
      std::string m_mode;
   };
} // namespace img32


#endif