add_custom_target(${PROJECT_NAME}.edj
   command edje_cc ../data/${PROJECT_NAME}.edc ${CMAKE_BINARY_DIR}/data/${PROJECT_NAME}.edj)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.edj)
