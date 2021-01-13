cmake_minimum_required(VERSION 3.2)

project(image32 CXX C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

option(IMG32_EXAMPLES "Allow building examples?" ON)

add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(examples)