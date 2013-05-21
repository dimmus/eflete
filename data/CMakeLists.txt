add_custom_target(${PROJECT_NAME}.edj
   command edje_cc ../data/${PROJECT_NAME}.edc ${CMAKE_BINARY_DIR}/data/${PROJECT_NAME}.edj)
add_custom_target(ui_ruler.edj
   command edje_cc ../data/ui_ruler.edc ${CMAKE_BINARY_DIR}/data/ui_ruler.edj)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.edj)
add_dependencies(${PROJECT_NAME} ui_ruler.edj)
