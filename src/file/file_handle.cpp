#include "file/file_handle.h"

#include "../file/file.h"
#include "../string.h"

#define BMP_MAGIC_NUMBER 0x4D42
#define JPG_MAGIC_NUMBER 0xD8FF
#define PNG_MAGIC_NUMBER1 0x474E5089
#define PNG_MAGIC_NUMBER2 0x0A1A0A0D

namespace img32
{
   FileHandle::FileHandle() :
   m_filename(""),
   m_mode(""),
   m_file(nullptr)
   {
   }

   FileHandle::FileHandle(const char* filename, const char* mode) :
   m_filename(filename),
   m_mode(mode),
   m_file(fopen(filename, mode))
   {
   }

   FileHandle::FileHandle(FileHandle&& other)
   {
      m_filename = std::move(other.m_filename);
      m_mode = std::move(other.m_mode);
      m_file = std::move(other.m_file); other.m_file = nullptr;
   }

   FileHandle::~FileHandle()
   {
      if(m_file)
         fclose(m_file);
   }

   Result FileHandle::getStatus() const
   {
      if(m_mode.find('r') != std::string::npos) {
         if(is_directory(m_filename)) return Result::NotIsAFile;
         if(!m_file) return Result::InvalidFile;
         if(get_file_size(m_filename) == 0) return Result::EmptyFile;
      }else if (m_mode.find('w') != std::string::npos) {
         if(!m_file) return Result::WriteError;
      }else {
         return Result::UninitializedFile;
      }

      return Result::Success;
   }

   FILE* FileHandle::getStdFile() const
   {
      return m_file;
   }

   bool FileHandle::isValid() const
   {
      return getStatus() == Result::Success;
   }

   FileFormat FileHandle::getFileFormatByExtension()
   {
      std::string ext = string_to_lower(get_extension_from_file(m_filename));

      if(ext == "jpg" || ext == "jpeg") return FileFormat::Jpeg;
      if(ext == "png") return FileFormat::Png;
      if(ext == "bmp") return FileFormat::Bmp;

      return FileFormat::Unsupported;
   }

   FileFormat FileHandle::getFileFormatByContent()
   {
      uint8_t buf[8];
      long oldCursor = ftell(m_file);
      fseek(m_file, 0, SEEK_SET);
      fread(buf, 1, 8, m_file);
      fseek(m_file, oldCursor, SEEK_SET);

      return getFileFormatByContent(buf);
   }

   FileHandle& FileHandle::operator=(FileHandle&& other)
   {
      if(this != &other) {
         m_filename = std::move(other.m_filename);
         m_mode = std::move(other.m_mode);
         m_file = std::move(other.m_file); other.m_file = nullptr;
      }

      return *this;
   }

   FileFormat FileHandle::getFileFormatByContent(const uint8_t buf[8])
   {
      #define IS_MAGIC_WORD(offset, word)          \
         (buf[0+offset] == (word & 0xff) &&        \
         (buf[1+offset] == ((word & 0xff00) >> 8)))
      #define IS_MAGIC_DWORD(offset, dword)              \
         (buf[0+offset] == (dword & 0xff) &&             \
         (buf[1+offset] == ((dword & 0xff00) >> 8)) &&   \
         (buf[2+offset] == ((dword & 0xff0000) >> 16)) &&\
         (buf[3+offset] == ((dword & 0xff000000) >> 24)))

      FileFormat format = FileFormat::Unsupported;

      if(IS_MAGIC_WORD(0, JPG_MAGIC_NUMBER)) format = FileFormat::Jpeg;
      if(IS_MAGIC_WORD(0, BMP_MAGIC_NUMBER)) format = FileFormat::Bmp;
      if(IS_MAGIC_DWORD(0, PNG_MAGIC_NUMBER1) && (IS_MAGIC_DWORD(4, PNG_MAGIC_NUMBER2))) format = FileFormat::Png;

      return format;
   }
} // namespace img32
