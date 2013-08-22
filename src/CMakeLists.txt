configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/cmakeconfig.h.in
   ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)
add_definitions(-DHAVE_CONFIG_H)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/alloc
   ${CMAKE_CURRENT_SOURCE_DIR}/project_manager
   ${CMAKE_CURRENT_SOURCE_DIR}/project_manager/edit
   ${CMAKE_CURRENT_SOURCE_DIR}/logger
   ${CMAKE_CURRENT_SOURCE_DIR}/settings
   ${CMAKE_CURRENT_SOURCE_DIR}/common
   ${CMAKE_CURRENT_SOURCE_DIR}/ui
   ${CMAKE_CURRENT_SOURCE_DIR}/ui/modal_window
   ${CMAKE_CURRENT_SOURCE_DIR}/ui/editors
   ${CMAKE_CURRENT_SOURCE_DIR}/ui/dialogs
   ${CMAKE_CURRENT_SOURCE_DIR}/ui/blocks
   ${CMAKE_CURRENT_SOURCE_DIR}/ui/workspace
   ${CMAKE_CURRENT_SOURCE_DIR}/ui/notification
)

set(TET_CC_FILES
   efl_tet.c
   alloc/alloc.c
   project_manager/edje_compile.c
   project_manager/widget_manager.c
   project_manager/project_manager.c
   project_manager/edit/style_edit.c
   project_manager/edit/image_edit_api.c
   project_manager/edit/colorclasses_edit_api.c
   logger/logger.c
   settings/settings.c
   ui/ui_panes.c
   ui/ui_menu.c
   ui/ui_main_window.c
   ui/ui_blocks_communication.c
   ui/editors/style_editor.c
   ui/editors/image_editor.c
   ui/editors/colorclass_editor.c
   ui/dialogs/open_file_dialog.c
   ui/dialogs/font_viewer_dialog.c
   ui/dialogs/add_part_dialog.c
   ui/dialogs/add_state_dialog.c
   ui/workspace/ui_ruler.c
   ui/workspace/ui_workspace.c
   ui/workspace/ui_popup.c
   ui/workspace/ui_groupspace.c
   ui/modal_window/modal_window.c
   ui/blocks/ui_block.c
   ui/blocks/ui_property.c
   ui/blocks/ui_widget_list.c
   ui/blocks/ui_signal_list.c
   ui/blocks/ui_states_list.c
   ui/workspace/ui_highlight.c
   ui/workspace/ui_ruler.c
   ui/workspace/ui_workspace.c
   ui/workspace/ui_popup.c
   ui/workspace/ui_groupspace.c
   ui/notification/notify.c
)

add_executable(${PROJECT_NAME} ${TET_CC_FILES})
target_link_libraries(${PROJECT_NAME}
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

INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES (EDJE REQUIRED edje>=1.0.0)
PKG_CHECK_MODULES (ECORE REQUIRED ecore>=1.0.0)
PKG_CHECK_MODULES (ECORE_EVAS REQUIRED ecore-evas>=1.0.0)
PKG_CHECK_MODULES (EFLDEPS REQUIRED
  eina>=1.0.999.63568
  evas>=1.0.0
  ecore>=1.0.999.59763
  ecore-file>=1.0.0
  ecore-evas>=1.0.999.59763
  edje>=1.0.0)
PKG_CHECK_MODULES (EINA REQUIRED eina>=1.0.0)
PKG_CHECK_MODULES (ECORE_X ecore-x>=1.0.0)
PKG_CHECK_MODULES (EVAS REQUIRED evas>=1.0.0)
