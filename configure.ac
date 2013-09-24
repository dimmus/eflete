m4_define([v_maj], [0])
m4_define([v_min], [1])
m4_define([v_mic], [8])
m4_define([p_name], "Edje\ Theme\ Editor")
m4_define([project_version], [v_maj.v_min.v_mic])

AC_INIT([eflete], [project_version])
AM_INIT_AUTOMAKE([1.11 dist-bzip2 subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
PKG_CHECK_MODULES([CHECK], [check], [have_check="yes"], [have_check="no"])
AC_PROG_RANLIB
AM_SILENT_RULES([yes])

AC_SUBST(P_NAME,"p_name")
AC_SUBST(P_VERSION,"project_version")
AC_SUBST(V_MAJ,"v_maj")
AC_SUBST(V_MIN,"v_min")
AC_SUBST(V_MIC,"v_mic")

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([EINA], [eina >= 1.7.8], [have_eina="yes"], [have_eina="no"])
PKG_CHECK_MODULES([ELEMENTARY], [elementary >= 1.7.8], [have_elementary="yes"], [have_elementary="no"])
PKG_CHECK_MODULES([EET], [eet >= 1.7.8], [have_eet="yes"], [have_eet="no"])
PKG_CHECK_MODULES([EVAS], [evas >= 1.7.8], [have_evas="yes"], [have_evas="no"])
PKG_CHECK_MODULES([ECORE], [ecore >= 1.7.8], [have_ecore="yes"], [have_ecore="no"])
PKG_CHECK_MODULES([EIO], [eio >= 1.7.8], [have_eio="yes"], [have_eio="no"])
PKG_CHECK_MODULES([EDJE], [edje >= 1.7.8], [have_edje="yes"], [have_edje="no"])
PKG_CHECK_MODULES([EFREET], [efreet >= 1.7.8], [have_efreet="yes"], [have_efreet="no"])

AC_CHECK_PROGS([DOXYGEN], [doxygen])

EFL_WITH_BIN([edje], [edje-cc], [edje_cc])

TET_DEFINE=" -DTET_EDJ=\\\"${datadir}/eflete/themes/default/eflete.edj\\\" "
TET_DEFINE+=" -DTET_EDJ_PATH=\\\"${datadir}/eflete/themes/default/\\\" "
TET_DEFINE+=" -DTET_IMG_PATH=\\\"${datadir}/eflete/images/\\\" "
TET_DEFINE+=" -DTET_SETT_PATH=\\\"${HOME}/.eflete/\\\" "
AC_SUBST([TET_DEFINE])

AC_DEFINE(HAVE_CONFIG_H)

AM_PROG_CC_C_O

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([doc/Doxyfile]) ])
AC_CONFIG_FILES([
Makefile
src/Makefile
src/config.h
data/Makefile
data/eflete.desktop
tests/Makefile
tests/edj_build/Makefile
doc/Makefile
])

AM_CONDITIONAL([CHECK_FOUND], [test "x$have_check" = "xyes"])
AC_SUBST([CHECK_FOUND])
AM_CONDITIONAL([PREFIX], [test "x$prefix" = "xNONE"])
AM_COND_IF([PREFIX], [], [
AC_SUBST([PREFIX],"${prefix}/bin/")
])

echo ""
echo "Configuration Options Summary:"
echo ""
echo "  eina...........: ${have_eina}"
echo "  eet............: ${have_eet}"
echo "  evas...........: ${have_evas}"
echo "  ecore..........: ${have_ecore}"
echo "  eio............: ${have_eio}"
echo "  edje...........: ${have_edje}"
echo "  efreet.........: ${have_efreet}"
echo "  elementary.....: ${have_elementary}"
echo ""
echo "Testing:"
echo "  check..........: ${have_check}"
AM_COND_IF([CHECK_FOUND],
[
echo "  run tests......: make check"
],
[
echo "  run tests......: to run tests you should install check:"
echo "                   sudo apt-get install check"
])
echo ""
echo "Installation.....: sudo make install"
echo "  CFLAGS.........: ${CFLAGS}"
echo "  prefix.........: ${prefix}"
echo ""
AC_OUTPUT
