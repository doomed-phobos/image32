# Image32
# Copyright (C) 2020-2021  surreal-Ceres
#
# Used this library under the terms of MIT license
# Read LICENSE for more information

set(IMG32_DEFINITIONS)
set(INCLDIR ${PROJECT_SOURCE_DIR}/include)

if(WIN32)
   list(APPEND IMG32_DEFINITIONS -DIMG32_WIN)
   if(MSVC)
      list(APPEND IMG32_DEFINITIONS -D_CRT_SECURE_NO_WARNINGS)
   endif()
endif()

set(IMG32_SOURCES
   image_priv.h
   image.cpp
   image_info.cpp
   io.cpp
   io_jpeg.cpp
   io_png.cpp
   io_bmp.cpp
   file.cpp
   string.cpp)

add_library(image32_static STATIC
${IMG32_SOURCES})
   
target_link_libraries(image32_static
                     libjpeg
                     ${ZLIB_LIBRARIES}
                     png_static)

target_include_directories(image32_static PUBLIC
                           ${PNG_INCLUDE_DIRS}
                           ${INCLDIR})

target_compile_definitions(image32_static PUBLIC ${IMG32_DEFINITIONS})