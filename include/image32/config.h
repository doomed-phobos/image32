#ifndef CONFIG_H
#define CONFIG_H

#ifdef IMG32_WIN
   #ifdef IMG32_SHARED
      #define IMG32_API __declspec(dllexport)
   #endif
#endif

#ifndef IMG32_API
   #define IMG32_API
#endif

#include <cstdint>

namespace img32
{
   typedef const char* const_charp;
   typedef uint8_t channel_t;
   typedef uint32_t color_t;
   typedef color_t* address_t;
   typedef const color_t* const_address_t;
} // namespace img32


#endif