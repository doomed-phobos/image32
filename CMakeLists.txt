# Image32
# Copyright (C) 2020-2021  surreal-Ceres
#
# Used this library under the terms of MIT license
# Read LICENSE for more information

cmake_minimum_required(VERSION 3.2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_SOURCE_DIR}/cmake/cxx_flags_override.cmake)

project(image32 CXX C)

option(IMG32_EXAMPLES "Allow building examples?" ON)

add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(examples)