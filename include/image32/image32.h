/// Image32
///
/// Copyright (C) 2020-2021  surrealCeres
///
/// Used this library under the terms of *** license
/// Read LICENSE for more information

#ifndef IMAGE32_H
#define IMAGE32_H

#ifdef IMG32_WIN
   #ifdef IMG32_SHARED
      #define IMG32_API __declspec(dllexport)
   #endif
#endif

#ifndef IMG32_API
   #define IMG32_API
#endif

#include "image.h"
#include "color.h"

#endif