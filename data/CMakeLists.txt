set(IMG_DIR "themes/default/images/")
add_custom_target(${PROJECT_NAME}.edj
   command edje_cc -v -id ${IMG_DIR} ../data/themes/default/${PROJECT_NAME}.edc ${CMAKE_BINARY_DIR}/data/themes/default/${PROJECT_NAME}.edj)
add_custom_target(template.edj
   command edje_cc -v -id ${IMG_DIR}/template/ ../data/themes/default/template.edc ${CMAKE_BINARY_DIR}/data/themes/default/template.edj)
add_dependencies(${PROJECT_NAME} template.edj)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.edj)
