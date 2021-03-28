# Image32
# Copyright (C) 2020-2021  surreal-Ceres
#
# Used this library under the terms of MIT license
# Read LICENSE for more information

set(IMG32_DEFINITIONS)
set(INCLDIRS
   ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_SOURCE_DIR}/include/image32)

if(MSVC)
   list(APPEND IMG32_DEFINITIONS -D_CRT_SECURE_NO_WARNINGS)
endif()

set(IMG32_SOURCES
   file/file.cpp
   file/file_handle.cpp
   codec/libbmp.cpp
   codec/image_codec.cpp
   codec/image_decoder.cpp
   codec/jpeg_decoder.cpp
   codec/png_decoder.cpp
   codec/bmp_decoder.cpp
   codec/unknown_decoder.h
   result.cpp
   color_info.cpp
   string.cpp
   image_info.cpp
   image_data.cpp
   palette.cpp
   image_priv.h
   #io_priv.h
   #image.cpp
   #image_buffer.cpp
   #io.cpp
   #io_png.cpp
   #io_bmp.cpp
)

add_library(image32_static STATIC
${IMG32_SOURCES})
   
target_link_libraries(image32_static
                     ${JPEG_LIBRARIES}
                     ${ZLIB_LIBRARIES}
                     ${PNG_LIBRARIES})
                     
target_include_directories(image32_static PRIVATE
                           ${PNG_INCLUDE_DIRS}
                           ${JPEG_INCLUDE_DIRS})

target_include_directories(image32_static PUBLIC
                           ${INCLDIRS})

target_compile_definitions(image32_static PUBLIC ${IMG32_DEFINITIONS})