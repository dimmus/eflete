EFLETE_VERSION([0], [1], [10])
AC_INIT([eflete], [eflete_version], [enlightenment-devel@lists.sourceforge.net],
        [eflete])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
AC_PROG_RANLIB
AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([1.13 dist-bzip2 subdir-objects color-tests])

dnl m4_define([package], "EFL\ Edje\ Theme\ Editor")
m4_define([efl_version], [1.9.2])
m4_define([elm_version], [1.9.2])
m4_define([ewe_version], [0.0.3])

EFLETE_INIT
AC_CONFIG_HEADERS([eflete_config.h])
AH_TOP([
#ifndef EFLETE_CONFIG_H
#define EFLETE_CONFIG_H
])
AH_BOTTOM([
#endif /* EFLETE_CONFIG_H */
])
AC_C___ATTRIBUTE__

#= Check OS ====================================================================
AC_CANONICAL_HOST

os_version="" dnl to set the OS version
os_remark="" dnl to set the remark for current OS

case "$host_os" in
   cegcc*)
      AC_MSG_ERROR([ceGCC compiler is not supported anymore. Exiting...])
   ;;
   mingw32ce*)
      os_remark="For this OS Eflete not been tested. Applicationmay be unstable!"
   ;;
   mingw*|cygwin*)
      # TODO: check cygwin* here
      os_remark="For this OS Eflete not been tested. Applicationmay be unstable!"
   ;;
   darwin*)
      os_remark="For this OS Eflete not been tested. Applicationmay be unstable!"
   ;;
   linux*)
      os_remark=""
   ;;
   *)
      AC_MSG_ERROR([Unknown OS. Exiting...])
   ;;
esac

#===============================================================================

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([EFL],
   [
    eina >= efl_version
    eet >= efl_version
    evas >= efl_version
    ecore >= efl_version
    ecore-input >= efl_version
    edje >= efl_version
    eo >= efl_version
    ecore-imf >= efl_version
    ecore-con >= efl_version
    eio >= efl_version
    eldbus >= efl_version
    efreet >= efl_version
    efreet-mime >= efl_version
   ],
   [have_efl="${GREEN}yes${RESET}"],
   [have_efl="${RED}no${RESET} (version < efl_version)"]
)

PKG_CHECK_MODULES([ELEMENTARY], [elementary >= elm_version],
   [have_elm="${GREEN}yes${RESET}"],
   [have_elm="${RED}no${RESET} (version < elm_version)"])

PKG_CHECK_MODULES([EWE], [ewe >= ewe_version],
   [have_ewe="${GREEN}yes${RESET}"],
   [have_ewe="${RED}no${RESET} (version < ewe_version)"])

PKG_CHECK_MODULES([CHECK], [check],
   [have_check="${GREEN}yes${RESET}"],
   [have_check="${RED}no${RESET}"])
AM_CONDITIONAL([CHECK_FOUND], [test "x$have_check" = "x${GREEN}yes${RESET}"])
AC_SUBST([CHECK_FOUND])
AM_COND_IF([CHECK_FOUND],
   [run_test="make check"],
   [run_test="to run tests you should install 'check'"])

#Usage: EFLETE_WITH_BIN(package, binary, default_value). See eflete_binary.m4
EFLETE_WITH_BIN([edje], [edje_cc], [edje_cc])

dnl locale, gettext
AM_GNU_GETTEXT_VERSION(0.18.1)
AM_GNU_GETTEXT([external])

AC_CHECK_PROGS([DOXYGEN], [doxygen])

dnl custom flags
dnl --enable-debug/--disable-debug
EFLETE_DEBUG_DEF="#undef"
want_eflete_debug="no"
AC_ARG_ENABLE([debug],
   [AC_HELP_STRING([--enable-debug], [enable Eflete debug support. @<:@default=disabled@:>@])],
   [want_eflete_debug=$enableval], [])

if test "x$want_eflete_debug" = "xyes"; then
   AC_DEFINE(HAVE_EFLETE_DEBUG, 1, [Eflete debug.])
   EFLETE_DEBUG_DEF="#define"
fi
AC_SUBST(EFLETE_DEBUG_DEF)

#===============================================================================
if test "x${prefix}" = "xNONE"; then
   LOCALE_DIR="${ac_default_prefix}/share/locale"
else
   LOCALE_DIR="${prefix}/share/locale"
fi
AC_SUBST(LOCALE_DIR)
#===============================================================================

EFLETE_DEFINE=" -DEFLETE_EDJ=\\\"${datadir}/eflete/layouts/eflete.edj\\\" "
EFLETE_DEFINE+=" -DEFLETE_THEME=\\\"${datadir}/eflete/themes/default/default.edj\\\" "
EFLETE_DEFINE+=" -DEFLETE_EDJ_PATH=\\\"${datadir}/eflete/themes/default/\\\" "
EFLETE_DEFINE+=" -DEFLETE_IMG_PATH=\\\"${datadir}/eflete/images/\\\" "
EFLETE_DEFINE+=" -DEFLETE_SETT_PATH=\\\"${HOME}/.eflete/\\\" "
EFLETE_DEFINE+=" -DEFLETE_SWAP_PATH=\\\"${HOME}/.eflete/swap/\\\" "
AC_SUBST([EFLETE_DEFINE])

AM_PROG_CC_C_O

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([doc/Doxyfile]) ])
AC_CONFIG_FILES([
po/Makefile.in
Makefile
src/Makefile
data/Makefile
data/eflete.desktop
tests/Makefile
tests/edj_build/Makefile
tests/edj_build/radio.edc
tests/Doxyfile
doc/Makefile
])
AC_CONFIG_COMMANDS( [cp], [cp ${srcdir}/tests/edj_build/radio.png ./tests/edj_build/radio.png])
AC_CONFIG_COMMANDS( [cp1], [cp ${srcdir}/tests/edj_build/radio2.png ./tests/edj_build/radio2.png])
AC_CONFIG_COMMANDS( [cp2], [cp ${srcdir}/tests/edj_build/radio_base.png ./tests/edj_build/radio_base.png])

AC_OUTPUT

echo ""
echo ""
echo ""
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo ""
echo "Configuration Options Summary:"
echo "  os.............: ${host_os}"
echo "                   ${os_remark}"
echo "  efl............: ${have_efl}"
echo "  elementary.....: ${have_elm}"
echo "  ewe............: ${have_ewe}"
echo ""
echo "edje-cc path.....: ${edje_cc}"
echo
echo "Testing:"
echo "  check..........: ${have_check}"
echo "  run tests......: ${run_test}"
echo ""
echo "Debug output.....: ${want_eflete_debug}"
echo ""
echo "Installation.....: cd build"
echo "                   make install (as root if needed, with 'su' or 'sudo')"
echo "  CFLAGS.........: ${CFLAGS}"
echo "  LDFLAGS........: ${LDFLAGS}"
echo "  prefix.........: ${prefix}"
echo ""
