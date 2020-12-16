add_subdirectory(libjpeg)

##################### libpng ######################
set(SKIP_INSTALL_ALL ON)
set(PNG_SHARED OFF CACHE BOOL "Build shared lib")
set(PNG_STATIC ON CACHE BOOL "Build static lib")
set(PNG_TESTS OFF CACHE BOOL "Build libpng tests")
add_subdirectory(libpng)
###################################################