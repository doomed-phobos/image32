set(SKIP_INSTALL_ALL ON)

set(ZLIB_INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/zlib
   ${CMAKE_BINARY_DIR}/3rd_party/zlib CACHE PATH "")
set(PNG_INCLUDE_DIRS
   ${CMAKE_CURRENT_SOURCE_DIR}/libpng
   ${CMAKE_BINARY_DIR}/3rd_party/libpng CACHE PATH "")
#set(WEBP_INCLUDE_DIRS
#   ${CMAKE_CURRENT_SOURCE_DIR}/libwebp/src CACHE PATH "")

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

#===================================================
# libwebp
#===================================================
#set(WEBP_BUILD_ANIM_UTILS OFF CACHE BOOL "Build animation utilities.")
#set(WEBP_BUILD_CWEBP OFF CACHE BOOL "Build the cwebp command line tool.")
#set(WEBP_BUILD_DWEBP OFF CACHE BOOL "Build the dwebp command line tool.")
#set(WEBP_BUILD_GIF2WEBP OFF CACHE BOOL "Build the gif2webp conversion tool.")
#set(WEBP_BUILD_IMG2WEBP OFF CACHE BOOL "Build the img2webp animation tool.")
#set(WEBP_BUILD_VWEBP OFF CACHE BOOL "Build the vwebp viewer tool.")
#set(WEBP_BUILD_WEBPINFO OFF CACHE BOOL "Build the webpinfo command line tool.")
#set(WEBP_BUILD_WEBPMUX OFF CACHE BOOL "Build the webpmux command line tool.")
#set(WEBP_BUILD_EXTRAS OFF CACHE BOOL "Build extras.")
#add_subdirectory(libwebp)