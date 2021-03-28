#ifndef RESULT_H_
#define RESULT_H_

namespace img32
{
   enum class Result
   {
      /// Read File
      InvalidFile,
      EmptyFile,
      NotIsAFile,
      UninitializedFile,
      /// Write File
      WriteError,
      /// Image
      UnsupportedFormat,
      UnsupportedBitCount,
      UnsupportedBmpCompression,
      ErrorAllocationMemory,
      /// General
      Success,
      Failed
   };

   const char* ResultToString(Result result);
} // namespace img32


#endif