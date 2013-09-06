enable_testing()
find_package(Check REQUIRED)
find_package(Eina REQUIRED)
find_package(Eet REQUIRED)
find_package(Evas REQUIRED)
find_package(Ecore REQUIRED)
find_package(Ecore COMPONENTS Ecore-x)
find_package(Ecore COMPONENTS Ecore-Evas)
find_package(Ecore COMPONENTS Ecore-File)
find_package(Eio REQUIRED)
find_package(Edje REQUIRED)
find_package(Efreet REQUIRED)
find_package(Elementary REQUIRED)

configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/../src/cmakeconfig.h.in
   ${CMAKE_CURRENT_SOURCE_DIR}/../src/config.h
)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-D__TESTING__)

add_library(proj STATIC ${TET_CC_FILES})

set(TEST_LIBS
   proj
   check
   ${LIBS}
   ${EINA_LIBRARIES}
   ${ECORE_LIBRARIES}
   ${ECORE_X_LIBRARIES}
   ${ECORE_EVAS_LIBRARIES}
   ${ECORE_FILE_LIBRARIES}
   ${EVAS_LIBRARIES}
   ${EET_LIBRARIES}
   ${EIO_LIBRARIES}
   ${EDJE_LIBRARIES}
   ${EFREET_LIBRARIES}
   ${ELEMENTARY_LIBRARIES}
)

set(TEST_NAME test_addpart)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_alloc)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_colorclasses_edit_api)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( colorapi_edj
   command edje_cc -v -id ./edj_build ./edj_build/radio.edc ${TEST_NAME}/data/radio.edj)
add_dependencies(${PROJECT_NAME} colorapi_edj)

set(TEST_NAME test_edje_compile)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( ecompile_edj
   command edje_cc -v -id ./edj_build ./edj_build/radio.edc ${TEST_NAME}/data/radio.edj)
add_dependencies(${PROJECT_NAME} ecompile_edj)

set(TEST_NAME test_efl_tet)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_font_viewer_dialog)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( font_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} font_edj)

set(TEST_NAME test_logger)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_modal_window)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_notify)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( notify_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} notify_edj)

set(TEST_NAME test_project_manager)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
add_custom_target( prman_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} prman_edj)

set(TEST_NAME test_settings)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_block)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_highlight)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_main_window)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_ruler)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_signal_list)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( signall_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} signall_edj)

set(TEST_NAME test_ui_state_list)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( statel_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} statel_edj)

set(TEST_NAME test_ui_widget_list)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( widgetl_edj
   command edje_cc -v -id ./edj_build ./edj_build/radio.edc ${TEST_NAME}/data/radio.edj)
add_dependencies(${PROJECT_NAME} widgetl_edj)

set(TEST_NAME test_ui_workspace)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_widget_manager)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
add_custom_target( wman_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} wman_edj)

set(TEST_NAME test_ui_groupspace)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
file(MAKE_DIRECTORY ${TEST_NAME}/data)
add_custom_target( groupspace_edj
   command edje_cc -v -id ./edj_build ./edj_build/check.edc ${TEST_NAME}/data/check.edj)
add_dependencies(${PROJECT_NAME} groupspace_edj)

set(TEST_NAME test_colorclass_editor)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
add_custom_target( colorclass_editor_edj
   command edje_cc -v -id ${TEST_NAME}/data/ ${TEST_NAME}/data/naviframe.edc ${TEST_NAME}/data/naviframe.edj)
add_dependencies(${PROJECT_NAME} colorclass_editor_edj)

set(TEST_NAME test_style_editor)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
add_custom_target( style_editor_edj
   command edje_cc -v -id ${TEST_NAME}/data/ ${TEST_NAME}/data/naviframe.edc ${TEST_NAME}/data/naviframe.edj)
add_dependencies(${PROJECT_NAME} style_editor_edj)

set(TEST_NAME test_image_editor)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})
add_custom_target( image_editor_edj
   command edje_cc -v -id ${TEST_NAME}/data/ ${TEST_NAME}/data/naviframe.edc ${TEST_NAME}/data/naviframe.edj)
add_dependencies(${PROJECT_NAME} image_editor_edj)
