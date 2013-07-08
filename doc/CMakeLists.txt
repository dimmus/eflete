if(DOXYGEN_FOUND)
   set(top_srcdir ${CMAKE_BINARY_DIR})
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
         ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
   add_custom_target(doc command ${DOXYGEN_EXECUTABLE}
         ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
#   add_dependencies(${PROJECT_NAME} doc)
endif(DOXYGEN_FOUND)


