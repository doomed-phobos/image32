cmake_minimum_required(VERSION 3.2)

project(image32 CXX C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(IMG32_EXAMPLES "Allow building examples?" ON)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(3rd_party)