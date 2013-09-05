cmake_minimum_required(VERSION 2.8)
project(efltet C)

set(VERMAJ 0)
set(VERMIN 0)
set(VERMIC 8)
set(PACKAGE_VERSION ${VERMAJ}.${VERMIN}.${VERMIC})
set(PACKAGE "Theme Editor Tool")

option(debug "debug build" 0)
option(test "build tests" 1) #default value for Jenkins

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/src
   ${CMAKE_CURRENT_SOURCE_DIR}/src/alloc
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/edit
   ${CMAKE_CURRENT_SOURCE_DIR}/src/logger
   ${CMAKE_CURRENT_SOURCE_DIR}/src/settings
   ${CMAKE_CURRENT_SOURCE_DIR}/src/common
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/modal_window
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editors
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/blocks
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/notification
)

set(TET_CC_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/src/efl_tet.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/alloc/alloc.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/edje_compile.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/widget_manager.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/project_manager.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/edit/style_edit.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/edit/image_edit_api.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/project_manager/edit/colorclasses_edit_api.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/logger/logger.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/settings/settings.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_panes.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_menu.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_main_window.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_blocks_communication.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editors/style_editor.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editors/image_editor.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editors/colorclass_editor.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/editors/program_editor.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/open_file_dialog.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/save_file_dialog.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/font_viewer_dialog.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/add_part_dialog.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/add_state_dialog.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/add_style_dialog.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/dialogs/about_window.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_ruler.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_workspace.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_demospace.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_popup.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_groupspace.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/modal_window/modal_window.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/blocks/ui_block.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/blocks/ui_property.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/blocks/ui_widget_list.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/blocks/ui_signal_list.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/blocks/ui_states_list.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_highlight.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_ruler.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_workspace.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_popup.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/workspace/ui_groupspace.c
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/notification/notify.c
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Eina REQUIRED)
   if(EINA_FOUND)
      set(have_eina "yes")
   else(EINA_FOUND)
      message(FATAL_ERROR "Can not find a Eina library.")
   endif(EINA_FOUND)
find_package(Eet REQUIRED)
   if(EET_FOUND)
      set(have_eet "yes")
   else(EET_FOUND)
      message(FATAL_ERROR "Can not find a Eet library.")
   endif(EET_FOUND)
find_package(Evas REQUIRED)
   if(EVAS_FOUND)
      set(have_evas "yes")
   else(EVAS_FOUND)
      message(FATAL_ERROR "Can not find a Evas library.")
   endif(EVAS_FOUND)
find_package(Ecore REQUIRED)
   if(ECORE_FOUND)
      set(have_ecore "yes")
   else(ECORE_FOUND)
      message(FATAL_ERROR "Can not find a Ecore library.")
   endif(ECORE_FOUND)
find_package(Ecore COMPONENTS Ecore-x)
   if(ECORE_X_FOUND)
      set(have_ecore_x "yes")
   else(ECORE_X_FOUND)
      message(FATAL_ERROR "Can not find a Ecore library component - Ecore_X.")
   endif(ECORE_X_FOUND)
find_package(Ecore COMPONENTS Ecore-Evas)
   if(ECORE_EVAS_FOUND)
      set(have_ecore_evas "yes")
   else(ECORE_EVAS_FOUND)
      message(FATAL_ERROR "Can not find a Ecore library component - Ecore_Evas.")
   endif(ECORE_EVAS_FOUND)
find_package(Ecore COMPONENTS Ecore-File)
   if(ECORE_FILE_FOUND)
      set(have_ecore_file "yes")
   else(ECORE_FILE_FOUND)
      message(FATAL_ERROR "Can not find a Ecore library component - Ecore_File.")
   endif(ECORE_FILE_FOUND)
find_package(Eio REQUIRED)
   if(EIO_FOUND)
      set(have_eio "yes")
   else(EIO_FOUND)
      message(FATAL_ERROR "Can not find a Eio library.")
   endif(EIO_FOUND)
find_package(Edje REQUIRED)
   if(EDJE_FOUND)
      set(have_edje "yes")
   else(EDJE_FOUND)
      message(FATAL_ERROR "Can not find a Edje library.")
   endif(EDJE_FOUND)
find_package(Efreet REQUIRED)
   if(EFREET_FOUND)
      set(have_efreet "yes")
   else(EFREET_FOUND)
      message(FATAL_ERROR "Can not find a Efreet library.")
   endif(EFREET_FOUND)
find_package(Elementary REQUIRED)
   if(ELEMENTARY_FOUND)
      set(have_elementary "yes")
   else(ELEMENATRY_FOUND)
      message(FATAL_ERROR "Can not find a Elementary library.")
   endif(ELEMENTARY_FOUND)
find_package(Doxygen)
   if(DOXYGEN_FOUND)
      set(build_doc "yes")
   else(DOXYGEN_FOUND)
      set(build_doc "no")
   endif(DOXYGEN_FOUND)
find_package(Check)
   if(${test} AND CHECK_FOUND)
      set(build_test "yes")
   else()
      set(build_test "no")
   endif()

set(CMAKE_C_FLAGS "-g -Wall -Wextra -Werror -O2")

# target 'clear'
IF("${CMAKE_GENERATOR}" MATCHES "Make")
   ADD_CUSTOM_TARGET(fsm_clean
      COMMAND ${CMAKE_MAKE_PROGRAM} clean
      WORKING_DIRECTORY ${CMAKE_CURRENT_DIR}
      )
   ADD_DEPENDENCIES(fsm_clean FsmCompilerClean)
ENDIF("${CMAKE_GENERATOR}" MATCHES "Make")

include_directories(
   ${EINA_INCLUDE_DIRS}
   ${ECORE_INCLUDE_DIRS}
   ${EVAS_INCLUDE_DIRS}
   ${EET_INCLUDE_DIRS}
   ${EIO_INCLUDE_DIRS}
   ${EDJE_INCLUDE_DIRS}
   ${EFREET_INCLUDE_DIRS}
   ${ELEMENTARY_INCLUDE_DIRS}
   )

if (prefix)
   set(CMAKE_INSTALL_PREFIX ${prefix})
else()
   set(prefix ${CMAKE_INSTALL_PREFIX})
endif()

if (NOT ${debug})
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
   set(BIN_PATH ${CMAKE_INSTALL_PREFIX}/bin)
   set(DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/efltet)
   set(DOC_PATH ${CMAKE_INSTALL_PREFIX}/man)
   set(BUILD_TYPE "Release Build")
   set(BUILD_PATH ${CMAKE_INSTALL_PREFIX})
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
else()
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
   set(BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR})
   set(DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/data)
   set(DOC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/doc)
   set(BUILD_TYPE "Debug Build")
   set(BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

add_definitions("-DPACKAGE=\"${PACKAGE}\"")
file(MAKE_DIRECTORY $ENV{HOME}/.${PROJECT_NAME})
#FIXME: bad define
add_definitions("-DTET_EDJ=\"${DATA_PATH}/themes/default/${PROJECT_NAME}.edj\"")
add_definitions("-DTET_IMG_PATH=\"${DATA_PATH}/images/\"")
add_definitions("-DTET_EDJ_PATH=\"${DATA_PATH}/themes/default/\"")
add_definitions("-DTET_SETT_PATH=\"$ENV{HOME}/.efltet/\"")

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(doc)
if (${build_test})
   add_subdirectory(tests)
endif()

#install
if (NOT ${debug})
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/images DESTINATION ${DATA_PATH} FILES_MATCHING PATTERN "*.jpg" PATTERN "*.png")
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/themes/default/${PROJECT_NAME}.edj DESTINATION ${DATA_PATH}/themes/default)
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/themes/default/template.edj DESTINATION ${DATA_PATH}/themes/default)
   install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/build/${PROJECT_NAME} DESTINATION ${BIN_PATH})
else()
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/images DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/data FILES_MATCHING PATTERN "*.jpg" PATTERN "*.png")
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/themes/default/${PROJECT_NAME}.edj DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/data/themes/default)
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/themes/default/template.edj DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/data/themes/default)
endif()

if (NOT ${debug})
   # Install desctop file
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/efltet.desktop DESTINATION /usr/local/share/applications)
   # Install icons
   install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/images/efltet.svg DESTINATION /usr/local/share/icons)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#OUTPUT
message("")
message("")
message("------------------------------------------------------------------------")
message(${PACKAGE} " " ${PACKAGE_VERSION})
message("------------------------------------------------------------------------")
message("")
message("Configuration Options Summary:")
message("")
message("  eina...........: " ${have_eina})
message("  eet............: " ${have_eet})
message("  evas...........: " ${have_evas})
message("  ecore..........: " ${have_ecore})
message("    ecore-x......: " ${have_ecore_x})
message("    ecore-evas...: " ${have_ecore_evas})
message("    ecore-file...: " ${have_ecore_file})
message("  eio............: " ${have_eio})
message("  edje...........: " ${have_edje})
message("  efreet.........: " ${have_efreet})
message("  elementary.....: " ${have_elementary})
message("")
message("  Documentation..: " ${build_doc})
message("    Graphviz.....: " ${have_graphviz})
message("    Build doc....: make doc")
message("")
message("Compilation......: make")
message("  CPPFLAGS.......: " ${CMAKE_CXX_FLAGS})
message("  CFLAGS.........: " ${CMAKE_C_FLAGS})
message("  LDFLAGS........: " )
message("")
message("Testing..........: " ${build_test})
if(${build_test})
   message("  Test command...: ctest")
endif()
message("")
message("Installation.....: make install (as root if needed, with 'su' or 'sudo')")
message("  type...........: " ${BUILD_TYPE})
if (NOT ${debug})
   message("  prefix.........: " ${prefix})
else()
   message("  path...........: " ${BUILD_PATH})
endif()
message("")
enable_testing()
