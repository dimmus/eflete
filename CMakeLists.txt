cmake_minimum_required(VERSION 2.8)
project(efl_tet C)

set(VERMAJ 0)
set(VERMIN 0)
set(VERMIC 1)
set(PACKAGE_VERSION ${VERMAJ}.${VERMIN}.${VERMIC})
set(PACKAGE "EFL Edje Edit Tool")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Eina REQUIRED)
find_package(Eet REQUIRED)
find_package(Evas REQUIRED)
find_package(Ecore REQUIRED)
find_package(Ecore COMPONENTS Ecore-x)
find_package(Ecore COMPONENTS Ecore-File)
find_package(Ecore COMPONENTS Ecore-Evas)
find_package(Edje REQUIRED)
find_package(Efreet REQUIRED)
find_package(Elementary REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -Werror -O2")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -O2")


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
   ${EDJE_INCLUDE_DIRS}
   ${ELEMENTARY_INCLUDE_DIRS}
   )

add_definitions("-DPACKAGE=\"${PACKAGE}\"")
#FIXME: bad define
add_definitions("-DTET_EDJ=\"${CMAKE_CURRENT_SOURCE_DIR}/data/${PROJECT_NAME}.edj\"")
add_definitions("-DTET_IMG_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/data/image/\"")
add_definitions("-DTET_EDJ_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/data/\"")

#add_subdirectory(po)
add_subdirectory(src)
add_subdirectory(data)
