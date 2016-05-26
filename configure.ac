EFLETE_VERSION([0], [6], [0], [dev])
AC_INIT([eflete], [eflete_version], [enlightenment-devel@lists.sourceforge.net],
        [eflete])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
AC_PROG_RANLIB
AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([1.13 dist-bzip2 subdir-objects color-tests foreign])

LT_INIT([win32-dll disable-static pic-only])

dnl m4_define([package], "EFL\ Edje\ Theme\ Editor")
m4_define([efl_version], [1.17])
m4_define([efl_version_max], [1.17.9999])

m4_define([enventor_version], [0.5.0])

EWE_INIT
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

# Force the helper to try external eolian-gen
AM_CONDITIONAL([HAVE_EOLIAN_GEN], [true])

#===============================================================================

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([EFL],
   [
    eina >= efl_version          eina < efl_version_max
    eet >= efl_version           eet < efl_version_max
    evas >= efl_version          evas < efl_version_max
    ecore >= efl_version         ecore < efl_version_max
    ecore-input >= efl_version   ecore-input < efl_version_max
    edje >= efl_version          edje < efl_version_max
    eo >= efl_version            eo < efl_version_max
    ecore-imf >= efl_version     ecore-imf < efl_version_max
    ecore-con >= efl_version     ecore-con < efl_version_max
    ecore-audio >= efl_version   ecore-audio < efl_version_max
    eio >= efl_version           eio < efl_version_max
    eldbus >= efl_version        eldbus < efl_version_max
    efreet >= efl_version        efreet < efl_version_max
    efreet-mime >= efl_version   efreet-mime < efl_version_max
    elementary >= efl_version    elementary < efl_version_max
   ],
   [have_efl="${GREEN}yes${RESET}"],
   [have_efl="${RED}no${RESET} (version < efl_version or > efl_version_max)"]
)

PKG_CHECK_MODULES([CHECK], [check],
   [have_check="${GREEN}yes${RESET}"],
   [have_check="${RED}no${RESET}"])

PKG_CHECK_MODULES([ENVENTOR], [enventor >= enventor_version],
   [have_enventor="yes"],
   [have_enventor="no"])

AM_CONDITIONAL([CHECK_FOUND], [test "x$have_check" = "x${GREEN}yes${RESET}"])
AC_SUBST([CHECK_FOUND])
AM_COND_IF([CHECK_FOUND],
   [run_test="make check"],
   [run_test="to run tests you should install 'check'"])

#Usage: EFLETE_WITH_BIN(package, binary, default_value). See eflete_binary.m4
EFLETE_WITH_BIN([edje], [edje_cc], [edje_cc])
EFLETE_WITH_BIN([eet], [eet-eet], [eet])
EWE_WITH_BIN([edje], [edje_cc], [edje_cc])
EWE_WITH_BIN([eolian], [eolian-gen], [eolian_gen])

dnl locale, gettext
AM_GNU_GETTEXT_VERSION(0.18.1)
AM_GNU_GETTEXT([external])

AC_CHECK_PROGS([DOXYGEN], [doxygen])

dnl custom flags

dnl --enable-tizen
HAVE_TIZEN_DEF="#undef"
want_tizen="no"
AC_ARG_ENABLE([tizen],
   [AC_HELP_STRING([--enable-tizen], [enable GUI for Tizen SDK. @<:@default=disabled@:>@])],
   [want_tizen=$enableval], [])

if test "x$want_tizen" = "xyes"; then
   AC_DEFINE(HAVE_TIZEN, 1, [Support Tizen SDK GUI.])
   TIZEN_DEF="#define"
   summary_tizen="${GREEN}yes${RESET}"
fi
if test "x$want_tizen" = "xno"; then
   summary_tizen="${RED}no${RESET}"
fi

AM_CONDITIONAL([WANT_TIZEN], [test "x${want_tizen}" = "xyes"])

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

dnl --enable-todo/--disable-todo
EFLETE_TODO_DEF="#undef"
want_eflete_todo="no"
AC_ARG_ENABLE([todo],
   [AC_HELP_STRING([--enable-todo], [show TODO. @<:@default=disabled@:>@])],
   [want_eflete_todo=$enableval], [])

if test "x$want_eflete_todo" = "xyes"; then
   AC_DEFINE(SHOW_TODO, 1, [Eflete TODO.])
   EFLETE_TODO_DEF="#define"
fi

dnl --enable-audio/--disable-audio
EFLETE_AUDIO_DEF="#undef"
dont_want_eflete_audio="no"
AC_ARG_ENABLE([audio],
   [AC_HELP_STRING([--disable-audio], [enable Eflete audio support. @<:@default=enabled@:>@])],
   [dont_want_eflete_audio="yes"], [dont_want_eflete_audio="no"])

if test "x$dont_want_eflete_audio" = "xno"; then
   AC_DEFINE(HAVE_AUDIO, 1, [Eflete audio.])
   EFLETE_AUDIO_DEF="#define"
fi

dnl --enable-enventor/--disable-enventor
HAVE_ENVENTOR_DEF="#undef"
want_enventor="no"
AC_ARG_ENABLE([enventor],
   [AC_HELP_STRING([--enable-enventor], [enable the Enventor support in the Eflete. @<:@default=disabled@:>@])],
   [want_enventor=$enableval], [])

if test "x$have_enventor" = "xno"; then
   summary_enventor="${RED}no${RESET} (version < enventor_version)"
fi
if test "x$have_enventor" = "xyes" -a "x$want_enventor" = "xyes"; then
   AC_DEFINE(HAVE_ENVENTOR, 1, [Enventor is installed])
   HAVE_ENVENTOR_DEF="#define"
   summary_enventor="${GREEN}yes${RESET}"
fi
if test "x$want_enventor" = "xno" -a "x$have_enventor" = "xyes"; then
   summary_enventor="${RED}no${RESET}"
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

DEPS_EOLIAN_FLAGS=`${PKG_CONFIG} --variable=eolian_flags eo efl evas edje ecore elementary`
AC_SUBST([DEPS_EOLIAN_FLAGS])

EFLETE_DEFINE=" -DEFLETE_EDJ=\\\"${datadir}/eflete/layouts/eflete.edj\\\" "
EFLETE_DEFINE+=" -DEFLETE_THEME=\\\"${datadir}/eflete/themes/default/eflete_elm.edj\\\" "
EFLETE_DEFINE+=" -DEFLETE_EDJ_PATH=\\\"${datadir}/eflete/themes/default/\\\" "
EFLETE_DEFINE+=" -DEFLETE_IMG_PATH=\\\"${datadir}/eflete/images/\\\" "
EFLETE_DEFINE+=" -DEFLETE_TEMPLATE_EDJ_PATH=\\\"${datadir}/eflete/themes/default/template/edj\\\" "
EFLETE_DEFINE+=" -DEFLETE_TEMPLATE_EDC_PATH=\\\"${datadir}/eflete/themes/default/template/edc\\\" "
EFLETE_DEFINE+=" -DEFLETE_TEMPLATE_IMAGES_PATH=\\\"${datadir}/eflete/themes/default/template/images\\\" "
EFLETE_DEFINE+=" -DEFLETE_TEMPLATE_SOUNDS_PATH=\\\"${datadir}/eflete/themes/default/template/sounds\\\" "
EFLETE_DEFINE+=" -DEFLETE_SETT_PATH=\\\"${HOME}/.config/eflete/\\\" "
AC_SUBST([EFLETE_DEFINE])

AM_PROG_CC_C_O

AM_CONDITIONAL([HAVE_ENVENTOR], [test "x$have_enventor" = "xyes" -a "x$want_enventor" = "xyes"])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([doc/Doxyfile]) ])
AC_CONFIG_FILES([
po/Makefile.in
Makefile
src/Makefile
src/bin/Makefile
src/lib/Makefile
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
AC_CONFIG_COMMANDS( [cp3], [cp ${srcdir}/tests/edj_build/kbd-tap.wav ./tests/edj_build/kbd-tap.wav])
AC_CONFIG_COMMANDS( [cp4], [cp ${srcdir}/tests/edj_build/license.txt ./tests/edj_build/license.txt])
AC_CONFIG_COMMANDS( [cp5], [cp ${srcdir}/tests/edj_build/PTS55F.ttf ./tests/edj_build/PTS55F.ttf])

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
echo ""
echo "Features:"
echo "  tizen..........: ${summary_tizen}"
dnl echo "  enventor.......: ${summary_enventor}"
echo ""
echo "edje-cc path.....: ${edje_cc}"
echo ""
echo "Testing:"
echo "  check..........: ${have_check}"
echo "  run tests......: ${run_test}"
echo ""
echo "Debug output.....: ${want_eflete_debug}"
echo ""
echo "Installation.....: make install (as root if needed, with 'su' or 'sudo')"
echo "  CFLAGS.........: ${CFLAGS}"
echo "  LDFLAGS........: ${LDFLAGS}"
echo "  prefix.........: ${prefix}"
echo ""
if test "x$dont_want_eflete_audio" = "xyes"; then
   echo " __________________________________________"
   echo "/ Ouch! You have disabled audio support.   \\"
   echo "| Some audio-related features are disabled |"
   echo "\\ and sound editor is turned into pumpkin. /"
   echo " ------------------------------------------"
   echo "        \\   ~__~"
   echo "         \\  (xX)\\_______"
   echo "            (__)\\       )\\/\\"
   echo "                ||----w |"
   echo "                ||     ||"
   echo ""
fi

if test "x$have_enventor" = "xyes" -a "x$want_enventor" = "xno"; then
   echo " __________________________________________"
   echo "/ Enventor dependency is available on your \\"
   echo "| system, but you are building without     |"
   echo "| enventor support. Don't forget to        |"
   echo "| --enable-enventor if you want Enventor   |"
   echo "\\ integration for Eflete.                  /"
   echo " ------------------------------------------"
   echo "        \\   ^__^"
   echo "         \\  (oo)\\_______"
   echo "            (__)\\       )\\/\\"
   echo "                ||----w |"
   echo "                ||     ||"
   echo ""
fi

if test "x$want_eflete_todo" = "xyes"; then
   echo " _________________________________________  "
   echo "/To do.                                   \\"
   echo "|To do.                                   | "
   echo "\\To do, to do, to do, to do, to doooooo.../"
   echo " -----------------------------------------  "
   echo "          (O_/ __ \\_O)  "
   echo "          / (o)__(o) \\  "
   echo "         (__.--\\/--.__) "
   echo "        ====(__/\\__)===="
   echo "              \`--'      "
   echo "              _||_      "
   echo "            /'....'\\    "
   echo "           | :    : |   "
   echo ""
fi
