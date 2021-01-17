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

namespace img32
{
   typedef const char* const_charp;
} // namespace img32


#endif