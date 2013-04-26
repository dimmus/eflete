#define PACKAGE "EFL Edje Edit Tool"
#define PACKAGE_NAME PACKAGE
#define VERSION "0.0.1"
#define VERMAJ 0
#define VERMIN 0
#define VERMIC 1

/* #undef HAVE___ATTRIBUTE__ */
#ifdef HAVE___ATTRIBUTE__
#define __UNUSED__ __attribute__((unused))
#else
#define __UNUSED__
#endif

/* #undef HAVE_PATH_MAX */
#ifndef HAVE_PATH_MAX
#define PATH_MAX 4096
#endif

/* #undef HAVE_ECORE_X */

/* #undef ENABLE_NLS */
