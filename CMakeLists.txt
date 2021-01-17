# Image32
# Copyright (C) 2020-2021  surreal-Ceres
#
# Used this library under the terms of MIT license
# Read LICENSE for more information

cmake_minimum_required(VERSION 3.2)

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Ot /RTC1")
endif()

project(image32 CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

option(IMG32_EXAMPLES "Allow building examples?" ON)

add_subdirectory(3rd_party)
add_subdirectory(src)
add_subdirectory(examples)