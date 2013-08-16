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

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/alloc
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/project_manager
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/logger
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/settings
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/ui
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/ui/dialogs
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/ui/blocks
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/ui/workspace
   ${CMAKE_CURRENT_SOURCE_DIR}. ../../src/ui/notification
)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} compile_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} logger_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} alloc_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} efltet_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} prmanager_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uiworkspace_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uipopup_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uigrspace_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} notify_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uiruler_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} widgetmanager_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uimain_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uipanes_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uimenu_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uiblock_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} fontviewer_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} setting_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} openfile_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} imgviewer_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} colorviewer_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uiblockcommun_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uiwidgetlist_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uistyledialog_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uiproperty_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uisignallist_clib)
set(LIBS ${LIBS} ${CHECK_LIBRARIES} uistatelist_clib)

add_executable(test_compile test_compile.c)
target_link_libraries(test_compile 
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
add_test(test_compile ${CMAKE_CURRENT_BINARY_DIR}/test_compile)
