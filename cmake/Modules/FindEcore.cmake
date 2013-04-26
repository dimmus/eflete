# - Try to find ecore
# Once done this will define
#  ECORE_FOUND - System has ecore
#  ECORE_INCLUDE_DIRS - The ecore include directories
#  ECORE_LIBRARIES - The libraries needed to use ecore
#  ECORE_DEFINITIONS - Compiler switches required for using ecore

# Use FIND_PACKAGE( Ecore COMPONENTS ... ) to enable modules
IF( Ecore_FIND_COMPONENTS )
  FOREACH( component ${Ecore_FIND_COMPONENTS} )
    STRING( TOUPPER ${component} _COMPONENT )
    SET( ECORE_USE_${_COMPONENT} 1 )
    MESSAGE(STATUS "Find: ECORE_USE_" ${_COMPONENT})
  ENDFOREACH( component )
ENDIF( Ecore_FIND_COMPONENTS )

find_package(PkgConfig)
if ("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER "2.8.1")
   # "QUIET" was introduced in 2.8.2
   set(_QUIET QUIET)
endif ()
pkg_check_modules(PC_LIBECORE ${_QUIET} ecore)
set(ECORE_DEFINITIONS ${PC_LIBECORE_CFLAGS_OTHER})

find_path(ECORE_INCLUDE_DIR Ecore.h
          HINTS ${PC_LIBECORE_INCLUDEDIR} ${PC_LIBECORE_INCLUDE_DIRS}
          PATH_SUFFIXES ecore )

find_library(ECORE_LIBRARY NAMES ecore
             HINTS ${PC_LIBECORE_LIBDIR} ${PC_LIBECORE_LIBRARY_DIRS} )

set(ECORE_LIBRARIES ${ECORE_LIBRARY} )
set(ECORE_INCLUDE_DIRS ${ECORE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ecore  DEFAULT_MSG
                                  ECORE_LIBRARY ECORE_INCLUDE_DIR)

mark_as_advanced( ECORE_INCLUDE_DIR ECORE_LIBRARY )

if (ECORE_USE_ECORE-X)
   pkg_check_modules(PC_LIBECORE_X ${_QUIET} ecore-x)
   set(ECORE_X_DEFINITIONS ${PC_LIBECORE_X_CFLAGS_OTHER})

   find_path(ECORE_X_INCLUDE_DIR Ecore_X.h
             HINTS ${PC_LIBECORE_X_INCLUDEDIR} ${PC_LIBECORE_X_INCLUDE_DIRS}
             PATH_SUFFIXES ecore )

   find_library(ECORE_X_LIBRARY NAMES ecore_x
                HINTS ${PC_LIBECORE_X_LIBDIR} ${PC_LIBECORE_X_LIBRARY_DIRS} )

   set(ECORE_X_LIBRARIES ${ECORE_X_LIBRARY} )
   set(ECORE_X_INCLUDE_DIRS ${ECORE_X_INCLUDE_DIR} )

   include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_X_FOUND to TRUE
# if all listed variables are TRUE
   find_package_handle_standard_args(ecore_x  DEFAULT_MSG
                                     ECORE_X_LIBRARY ECORE_X_INCLUDE_DIR)

   mark_as_advanced( ECORE_X_INCLUDE_DIR ECORE_X_LIBRARY )
endif (ECORE_USE_ECORE-X)

if (ECORE_USE_ECORE-EVAS)
   pkg_check_modules(PC_LIBECORE_EVAS ${_QUIET} ecore-evas)
   set(ECORE_EVAS_DEFINITIONS ${PC_LIBECORE_EVAS_CFLAGS_OTHER})

   find_path(ECORE_EVAS_INCLUDE_DIR Ecore_Evas.h
             HINTS ${PC_LIBECORE_EVAS_INCLUDEDIR} ${PC_LIBECORE_EVAS_INCLUDE_DIRS}
             PATH_SUFFIXES ecore )

   find_library(ECORE_EVAS_LIBRARY NAMES ecore_evas
                HINTS ${PC_LIBECORE_EVAS_LIBDIR} ${PC_LIBECORE_EVAS_LIBRARY_DIRS} )

   set(ECORE_EVAS_LIBRARIES ${ECORE_EVAS_LIBRARY} )
   set(ECORE_EVAS_INCLUDE_DIRS ${ECORE_EVAS_INCLUDE_DIR} )

   include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_EVAS_FOUND to TRUE
# if all listed variables are TRUE
   find_package_handle_standard_args(ecore_evas  DEFAULT_MSG
                                     ECORE_EVAS_LIBRARY ECORE_EVAS_INCLUDE_DIR)

   mark_as_advanced( ECORE_EVAS_INCLUDE_DIR ECORE_EVAS_LIBRARY )
endif (ECORE_USE_ECORE-EVAS)

if (ECORE_USE_ECORE-FILE)
   pkg_check_modules(PC_LIBECORE_FILE ${_QUIET} ecore-file)
   set(ECORE_FILE_DEFINITIONS ${PC_LIBECORE_FILE_CFLAGS_OTHER})

   find_path(ECORE_FILE_INCLUDE_DIR Ecore_File.h
             HINTS ${PC_LIBECORE_FILE_INCLUDEDIR} ${PC_LIBECORE_FILE_INCLUDE_DIRS}
             PATH_SUFFIXES ecore )

   find_library(ECORE_FILE_LIBRARY NAMES ecore_file
                HINTS ${PC_LIBECORE_FILE_LIBDIR} ${PC_LIBECORE_FILE_LIBRARY_DIRS} )

   set(ECORE_FILE_LIBRARIES ${ECORE_FILE_LIBRARY} )
   set(ECORE_FILE_INCLUDE_DIRS ${ECORE_FILE_INCLUDE_DIR} )

   include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set ECORE_FILE_FOUND to TRUE
# if all listed variables are TRUE
   find_package_handle_standard_args(ecore_file  DEFAULT_MSG
                                     ECORE_FILE_LIBRARY ECORE_FILE_INCLUDE_DIR)

   mark_as_advanced( ECORE_FILE_INCLUDE_DIR ECORE_FILE_LIBRARY )
endif (ECORE_USE_ECORE-FILE)
