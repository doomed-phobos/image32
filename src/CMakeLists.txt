add_library(image32 STATIC
image.cpp
io_jpeg.cpp)

target_link_libraries(image32 libjpeg)
target_include_directories(image32 PUBLIC "${PROJECT_SOURCE_DIR}/include")