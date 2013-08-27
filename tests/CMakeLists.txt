enable_testing()
find_package(Check REQUIRED)
find_package(Eina REQUIRED)
find_package(Eet REQUIRED)
find_package(Evas REQUIRED)
find_package(Ecore REQUIRED)
find_package(Ecore COMPONENTS Ecore-x)
find_package(Ecore COMPONENTS Ecore-Evas)
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
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/alloc
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/project_manager
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/project_manager/edit
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/logger
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/settings
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/common
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui/modal_window
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui/dialogs
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui/editors
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui/blocks
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui/workspace
   ${CMAKE_CURRENT_SOURCE_DIR}. ../src/ui/notification
)

set(TET_CC_FILES
   ../src/efl_tet.c
   ../src/alloc/alloc.c
   ../src/project_manager/edje_compile.c
   ../src/project_manager/widget_manager.c
   ../src/project_manager/project_manager.c
   ../src/project_manager/edit/style_edit.c
   ../src/project_manager/edit/image_edit_api.c
   ../src/project_manager/edit/colorclasses_edit_api.c
   ../src/logger/logger.c
   ../src/settings/settings.c
   ../src/ui/ui_panes.c
   ../src/ui/ui_menu.c
   ../src/ui/ui_main_window.c
   ../src/ui/ui_blocks_communication.c
   ../src/ui/editors/style_editor.c
   ../src/ui/editors/image_editor.c
   ../src/ui/editors/colorclass_editor.c
   ../src/ui/editors/program_editor.c
   ../src/ui/dialogs/open_file_dialog.c
   ../src/ui/dialogs/font_viewer_dialog.c
   ../src/ui/dialogs/add_part_dialog.c
   ../src/ui/dialogs/add_state_dialog.c
   ../src/ui/dialogs/about_window.c
   ../src/ui/workspace/ui_ruler.c
   ../src/ui/workspace/ui_workspace.c
   ../src/ui/workspace/ui_popup.c
   ../src/ui/workspace/ui_groupspace.c
   ../src/ui/modal_window/modal_window.c
   ../src/ui/blocks/ui_block.c
   ../src/ui/blocks/ui_property.c
   ../src/ui/blocks/ui_widget_list.c
   ../src/ui/blocks/ui_signal_list.c
   ../src/ui/blocks/ui_states_list.c
   ../src/ui/workspace/ui_highlight.c
   ../src/ui/workspace/ui_ruler.c
   ../src/ui/workspace/ui_workspace.c
   ../src/ui/workspace/ui_popup.c
   ../src/ui/workspace/ui_groupspace.c
   ../src/ui/notification/notify.c
)

add_library(proj STATIC ${TET_CC_FILES})

set(TEST_LIBS
   proj
   check
   ${LIBS}
   ${EINA_LIBRARIES}
   ${ECORE_LIBRARIES}
   ${ECORE_X_LIBRARIES}
   ${ECORE_EVAS_LIBRARIES}
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

set(TEST_NAME test_edje_compile)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_efl_tet)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_font_viewer_dialog)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

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

set(TEST_NAME test_project_manager)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

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

set(TEST_NAME test_ui_state_list)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_widget_list)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_ui_workspace)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

set(TEST_NAME test_widget_manager)
add_executable(${TEST_NAME} ${TEST_NAME}/${TEST_NAME}.c)
target_link_libraries(${TEST_NAME} ${TEST_LIBS})
add_test(${TEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME})

