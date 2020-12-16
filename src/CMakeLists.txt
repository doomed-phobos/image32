add_library(image32 STATIC
image.cpp
io_jpeg.cpp
io_none.cpp
io_png.cpp)

target_link_libraries(image32 libjpeg png)
target_include_directories(image32 PUBLIC "${PROJECT_SOURCE_DIR}/include")