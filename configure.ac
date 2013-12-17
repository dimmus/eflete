m4_define([v_maj], [0])
m4_define([v_min], [1])
m4_define([v_mic], [0-alpha])
m4_define([p_name], "EFL\ Edje\ Theme\ Editor")
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

PKG_CHECK_MODULES([_EINA], [eina], [have_eina="yes"], [have_eina="no"])
if test "x${have_eina}" = "xyes"; then
   PKG_CHECK_MODULES([EINA], [eina >= 1.8.0], [have_ver_eina=""], [have_ver_eina=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_ELEMENTARY], [elementary], [have_elementary="yes"], [have_elementary="no"])
if test "x${have_elementary}" = "xyes"; then
   PKG_CHECK_MODULES([ELEMENTARY], [elementary >= 1.8.0], [have_ver_elementary=""], [have_ver_elementary=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_EET], [eet], [have_eet="yes"], [have_eet="no"])
if test "x${have_eet}" = "xyes"; then
   PKG_CHECK_MODULES([EET], [eet >= 1.8.0], [have_ver_eet=""], [have_ver_eet=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_EVAS], [evas], [have_evas="yes"], [have_evas="no"])
if test "x${have_evas}" = "xyes"; then
   PKG_CHECK_MODULES([EVAS], [evas >= 1.8.0], [have_ver_evas=""], [have_ver_evas=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_ECORE], [ecore], [have_ecore="yes"], [have_ecore="no"])
if test "x${have_ecore}" = "xyes"; then
   PKG_CHECK_MODULES([ECORE], [ecore >= 1.8.0], [have_ver_ecore=""], [have_ver_ecore=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_EIO], [eio], [have_eio="yes"], [have_eio="no"])
if test "x${have_eio}" = "xyes"; then
   PKG_CHECK_MODULES([EIO], [eio >= 1.8.0], [have_ver_eio=""], [have_ver_eio=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_EDJE], [edje], [have_edje="yes"], [have_edje="no"])
if test "x${have_edje}" = "xyes"; then
   PKG_CHECK_MODULES([EDJE], [edje >= 1.8.0], [have_ver_edje=""], [have_ver_edje=" (version < 1.8.0)"])
fi
PKG_CHECK_MODULES([_EFREET], [efreet], [have_efreet="yes"], [have_efreet="no"])
if test "x${have_efreet}" = "xyes"; then
   PKG_CHECK_MODULES([EFREET], [efreet >= 1.8.0], [have_ver_efreet=""], [have_ver_efreet=" (version < 1.8.0)"])
fi


AC_CHECK_PROGS([DOXYGEN], [doxygen])

EFL_WITH_BIN([edje], [edje-cc], [edje_cc])

TET_DEFINE=" -DTET_EDJ=\\\"${datadir}/eflete/layouts/eflete.edj\\\" "
TET_DEFINE+=" -DTET_THEME=\\\"${datadir}/eflete/themes/default/default.edj\\\" "
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

if test "x$have_eina" = "xno" ||
   test "x$have_elementary" = "xno" ||
   test "x$have_eet" = "xno" ||
   test "x$have_evas" = "xno" ||
   test "x$have_ecore" = "xno" ||
   test "x$have_eio" = "xno" ||
   test "x$have_edje" = "xno" ||
   test "x$have_efreet" = "xno"; then
AC_MSG_ERROR("Needed EFL packages not found")
fi

if test  "x$have_ver_eina" != "x" ||
   test  "x$have_ver_elementary" != "x" ||
   test  "x$have_ver_eet" != "x" ||
   test  "x$have_ver_evas" != "x" ||
   test  "x$have_ver_ecore" != "x" ||
   test  "x$have_ver_eio" != "x" ||
   test  "x$have_ver_edje" != "x" ||
   test  "x$have_ver_efreet" != "x"; then
AC_MSG_ERROR("EFL version must be >= 1.8.0")
fi

AC_OUTPUT

echo ""
echo "Configuration Options Summary:"
echo ""
echo "  eina...........: ${have_eina} ${have_ver_eina}"
echo "  eet............: ${have_eet} ${have_ver_eet}"
echo "  evas...........: ${have_evas} ${have_ver_evas}"
echo "  ecore..........: ${have_ecore} ${have_ver_ecore}"
echo "  eio............: ${have_eio} ${have_ver_eio}"
echo "  edje...........: ${have_edje} ${have_ver_edje}"
echo "  efreet.........: ${have_efreet} ${have_ver_efreet}"
echo "  elementary.....: ${have_elementary} ${have_ver_elementary}"
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
