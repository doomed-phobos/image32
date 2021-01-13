set(IMG32_DEFINITIONS)

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
   io_jpeg.cpp
   io_png.cpp
   io_bmp.cpp
   file.cpp)

add_library(image32_static STATIC
${IMG32_SOURCES})
   
target_link_libraries(image32_static
                     libjpeg
                     ${ZLIB_LIBRARIES}
                     png_static)

target_include_directories(image32_static PUBLIC
                           ${PNG_INCLUDE_DIRS}
                           ${PROJECT_SOURCE_DIR}/include)

target_compile_definitions(image32_static PUBLIC ${IMG32_DEFINITIONS})