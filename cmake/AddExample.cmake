function(img32_add_example FILENAME)
   add_executable(${FILENAME} "${FILENAME}.cpp")
   target_link_libraries(${FILENAME} image32_static)
endfunction()