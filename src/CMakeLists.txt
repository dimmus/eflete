configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/cmakeconfig.h.in
   ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/project_manager
   ${CMAKE_CURRENT_SOURCE_DIR}/logger
   ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

set(TET_CC_FILES
   main.c
   project_manager/edje_compile.c
   project_manager/widget_manager.c
   project_manager/project_manager.c
   logger/logger.c
   ui/ui_panes.c
   ui/ui_menu.c
   ui/ui_main_window.c
   ui/ui_settings.c
   ui/ui.c
)

add_executable(${PROJECT_NAME} ${TET_CC_FILES})
target_link_libraries(${PROJECT_NAME}
   ${EINA_LIBRARIES}
   ${ECORE_LIBRARIES}
   ${ECORE_X_LIBRARIES}
   ${ECORE_FILE_LIBRARIES}
   ${ECORE_EVAS_LIBRARIES}
   ${EVAS_LIBRARIES}
   ${EET_LIBRARIES}
   ${EDJE_LIBRARIES}
   ${EFREET_LIBRARIES}
   ${ELEMENTARY_LIBRARIES}
)
