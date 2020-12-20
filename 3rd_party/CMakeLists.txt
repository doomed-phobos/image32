set(SKIP_INSTALL_ALL ON)

set(ZLIB_LIBRARIES zlibstatic CACHE PATH "")
set(ZLIB_INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/zlib
   ${CMAKE_BINARY_DIR}/3rd_party/zlib CACHE PATH "")
set(PNG_LIBRARIES png_static CACHE PATH "")
set(PNG_INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/libpng
   ${CMAKE_BINARY_DIR}/3rd_party/libpng CACHE PATH "")

include_directories(${PNG_INCLUDE_DIRS})

#===================================================
# zlib
#===================================================
set(ZLIB_TEST OFF CACHE BOOL "Build zlib test")
add_subdirectory(zlib)

#===================================================
# libpng
#===================================================
set(PNG_SHARED OFF CACHE BOOL "Build shared lib")
set(PNG_STATIC ON CACHE BOOL "Build static lib")
set(PNG_TESTS OFF CACHE BOOL "Build libpng tests")
set(PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used")
add_subdirectory(libpng)