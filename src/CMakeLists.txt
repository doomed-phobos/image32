add_executable(image32 main.cpp
image.cpp
io_jpeg.cpp
string.cpp)
target_link_libraries(image32 libjpeg)