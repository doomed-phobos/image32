#ifndef CONFIG_H
#define CONFIG_H

#ifdef IMG32_WIN
   #ifdef IMG32_SHARED
      #define IMG32_DLL __declspec(dllexport)
   #else
      #define IMG32_DLL
   #endif

   #ifdef _MSC_VER
      #pragma warning(disable: 4996)
   #endif
#endif

#include <cstdint>

namespace img32
{
   typedef const char* const_charp;
   typedef uint32_t pixel_t;
   typedef pixel_t* address_t;
   typedef const pixel_t* const_address_t;
} // namespace img32


#endif