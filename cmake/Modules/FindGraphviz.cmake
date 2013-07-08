# - Try to find graphviz
# Once done this will define
#  GRAPHVIZ_FOUND - System has graphviz
#  GRAPHVIZ_INCLUDE_DIRS - The graphviz include directories
#  GRAPHVIZ_LIBRARIES - The libraries needed to use graphviz
#  GRAPHVIZ_DEFINITIONS - Compiler switches required for using graphviz

find_package(PkgConfig)
if ("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER "2.8.1")
   # "QUIET" was introduced in 2.8.2
   set(_QUIET QUIET)
endif ()

pkg_check_modules(PC_LIBGRAPHVIZ ${_QUIET} libgvc)
set(GRAPHVIZ_DEFINITIONS ${PC_LIBGRAPHVIZ_CFLAGS_OTHER})

find_path(GRAPHVIZ_INCLUDE_DIR graph.h
          HINTS ${PC_LIBGRAPHVIZ_INCLUDE_DIRS} )

find_library(GRAPHVIZ_LIBRARY NAMES libgvc
             HINTS ${PC_LIBGRAPHVIZ_INCLUDE_DIRS} )

set(GRAPHVIZ_LIBRARIES ${GRAPHVIZ_LIBRARY} )
set(GRAPHVIZ_INCLUDE_DIRS ${PC_LIBGRAPHVIZ_INCLUDEDIR} ${GRAPHVIZ_INCLUDE_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GRAPHVIZ_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(graphviz DEFAULT_MSG
                                  GRAPHVIZ_LIBRARY GRAPHVIZ_INCLUDE_DIR)

mark_as_advanced(GRAPHVIZ_INCLUDE_DIR GRAPHVIZ_LIBRARY )
