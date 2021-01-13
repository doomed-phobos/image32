add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(SKIP_INSTALL_ALL ON)

#===================================================
# libjpeg
#===================================================
add_subdirectory(libjpeg)

#===================================================
# zlib
#===================================================
set(ZLIB_TEST OFF CACHE BOOL "Build zlib test")
set(ZLIB_INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/zlib
   ${CMAKE_CURRENT_BINARY_DIR}/zlib CACHE PATH "")
add_subdirectory(zlib)
set(ZLIB_LIBRARIES
zlibstatic CACHE INTERNAL "")

#===================================================
# libpng
#===================================================
set(PNG_INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/libpng
   ${CMAKE_CURRENT_BINARY_DIR}/libpng CACHE PATH "")
set(PNG_SHARED OFF CACHE BOOL "Build shared lib")
set(PNG_STATIC ON CACHE BOOL "Build static lib")
set(PNG_TESTS OFF CACHE BOOL "Build libpng tests")
set(PNG_BUILD_ZLIB ON CACHE BOOL "Custom zlib Location, else find_package is used")
add_subdirectory(libpng)
