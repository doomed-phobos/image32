add_library(image32 STATIC
image.cpp
io_jpeg.cpp
io_none.cpp
io_png.cpp
io_webp.cpp
io_bmp.cpp
file.cpp)

target_link_libraries(image32 
                      libjpeg
                      zlibstatic
                      png_static)
#                      webp)
target_include_directories(image32 PUBLIC
                           "${PROJECT_SOURCE_DIR}/include"
                           ${PNG_INCLUDE_DIRS})
#                           ${WEBP_INCLUDE_DIRS})