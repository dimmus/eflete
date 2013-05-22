set(IMG_DIR "images/")
add_custom_target(${PROJECT_NAME}.edj
   command edje_cc -v -id ${IMG_DIR} ../data/${PROJECT_NAME}.edc ${CMAKE_BINARY_DIR}/data/${PROJECT_NAME}.edj)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.edj)
