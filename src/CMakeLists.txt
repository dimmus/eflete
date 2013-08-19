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
   main.c
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

add_library(compile_clib project_manager/edje_compile.c)
add_library(prmanager_clib project_manager/project_manager.c)
add_library(logger_clib logger/logger.c)
add_library(alloc_clib alloc/alloc.c)
add_library(efltet_clib efl_tet.c)
add_library(uiworkspace_clib ui/workspace/ui_workspace.c)
add_library(notify_clib ui/notification/notify.c)
add_library(uipopup_clib ui/workspace/ui_popup.c)
add_library(uigrspace_clib ui/workspace/ui_groupspace.c)
add_library(uiruler_clib ui/workspace/ui_ruler.c)
add_library(widgetmanager_clib project_manager/widget_manager.c)
add_library(uimain_clib ui/ui_main_window.c)
add_library(uimenu_clib ui/ui_menu.c)
add_library(uipanes_clib ui/ui_panes.c)
add_library(uiblock_clib ui/blocks/ui_block.c)
add_library(fontviewer_clib ui/dialogs/font_viewer_dialog.c)
add_library(setting_clib settings/settings.c)
add_library(colorviewer_clib ui/dialogs/colorclass_viewer_dialog.c)
add_library(imgviewer_clib ui/dialogs/image_viewer_dialog.c)
add_library(openfile_clib ui/dialogs/open_file_dialog.c)
add_library(uiwidgetlist_clib ui/blocks/ui_widget_list.c)
add_library(uiblockcommun_clib ui/ui_blocks_communication.c)
add_library(uistyledialog_clib ui/dialogs/style_viewer_dialog.c)
add_library(uiproperty_clib ui/blocks/ui_property.c)
add_library(uisignallist_clib ui/blocks/ui_signal_list.c)
add_library(uistatelist_clib ui/blocks/ui_states_list.c)

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
