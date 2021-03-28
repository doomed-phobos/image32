#include "result.h"

namespace img32
{
   const char* ResultToString(Result result)
   {
      const char* str;
      switch(result) {
         /// Read File
         case Result::InvalidFile:                 str = "Invalid File";                  break;
         case Result::EmptyFile:                   str = "Empty File";                    break;
         case Result::NotIsAFile:                  str = "Not is a File";                 break;
         case Result::UninitializedFile:           str = "Uninitialized File";            break;
         /// Write File
         case Result::WriteError:                  str = "Write Error";                   break;
         /// Image
         case Result::UnsupportedFormat:           str = "Unsupported Format";            break;
         case Result::UnsupportedBitCount:         str = "Unsupported BitCount";          break;
         case Result::UnsupportedBmpCompression:   str = "Unsupported BitCompression";    break;
         case Result::ErrorAllocationMemory:       str = "Error allocating memory";       break;
         /// General 
         case Result::Success:                     str = "Success";                       break;
         case Result::Failed:                      str = "Failed";                        break;
      }

      return str;
   }
} // namespace img32
