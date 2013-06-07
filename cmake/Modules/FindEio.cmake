# - Try to find eio
# Once done this will define
#  EIO_FOUND - System has eio
#  EIO_INCLUDE_DIRS - The eio include directories
#  EIO_LIBRARIES - The libraries needed to use eio
#  EIO_DEFINITIONS - Compiler switches required for using eio

find_package(PkgConfig)
if ("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER "2.8.1")
   # "QUIET" was introduced in 2.8.2
   set(_QUIET QUIET)
endif ()
pkg_check_modules(PC_LIBEIO ${_QUIET} eio)
set(EIO_DEFINITIONS ${PC_LIBEIO_CFLAGS_OTHER})

find_path(EIO_INCLUDE_DIR Eio.h
          HINTS ${PC_LIBEIO_INCLUDEDIR} ${PC_LIBEIO_INCLUDE_DIRS}
          PATH_SUFFIXES eio )

find_library(EIO_LIBRARY NAMES eio
             HINTS ${PC_LIBEIO_LIBDIR} ${PC_LIBEIO_LIBRARY_DIRS} )

set(EIO_LIBRARIES ${EIO_LIBRARY} )
set(EIO_INCLUDE_DIRS ${EIO_INCLUDE_DIR} "${EIO_INCLUDE_DIR}/eio" )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set EIO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(eio  DEFAULT_MSG
                                  EIO_LIBRARY EIO_INCLUDE_DIR)

mark_as_advanced(EIO_INCLUDE_DIR EIO_LIBRARY )
