dnl EFLETE_COLOR
dnl will check if terminal supports color and if color is wanted by user.
dnl
dnl Used Variables:
dnl     WANT_COLOR: if no, forces no color output.
dnl     TERM: used to check if color should be enabled.
dnl
dnl Defined Variables:
dnl     GREEN: to be used in positive/yes conditions
dnl     RED: to be used in negative/no conditions
dnl     RESET: to reset color
dnl     want_color: yes or no
AC_DEFUN([EFLETE_COLOR],
[dnl
case "$TERM" in
   xterm|xterm-color|xterm-256color|Eterm|aterm|kterm|rxvt*|screen|gnome|interix)
      want_color="${WANT_COLOR:-yes}"
      ;;
   *)
      want_color="no"
      ;;
esac

if test "${want_color}" = "yes"; then
   if test `echo -e x` = x; then
      echoopt=-e
   else
      echoopt=
   fi

   GREEN=`echo $echoopt "\033@<:@1;32m"`
   RED=`echo $echoopt "\033@<:@1;31m"`
   RESET=`echo $echoopt "\033@<:@0m"`

else
   GREEN=""
   RED=""
   RESET=""
fi
])
dnl EFLETE_VERSION(major, minor, micro, release)
dnl This setup EFLETE version information and should be called BEFORE AC_INIT().
dnl
dnl release parameter is 'dev' to use from SVN or libtool -release field.
dnl It may be empty if not dev (svn/live build) and no -release is to be used.
dnl
dnl Examples:
dnl     EFLETE_VERSION(0, 0, 1, dev)
dnl     EFL_VERSION(0, 0, 1, ver-1234)
dnl This will define couple of m4 symbols:
dnl     v_maj = given major number (first parameter)
dnl     v_min = given minor number (second parameter)
dnl     v_mic = given micro number (third parameter)
dnl     v_rev = if release, it's 0, otherwise it's dev_version.
dnl     v_rel = if release, it's -release followed by fourth parameter,
dnl             otherwise it's empty. (mostly for libtool)
dnl     efl_version = if release, it's major.minor.micro, otherwise it's
dnl             major.minor.micro.dev_version
dnl     dev_version = development version (svn revision).
dnl     def_build_profile = dev or release based on 'dev' release parameter.
AC_DEFUN([EFLETE_VERSION],
[dnl
m4_define([v_maj], [$1])dnl
m4_define([v_min], [$2])dnl
m4_define([v_mic], [$3])dnl
m4_define([dev_version], m4_esyscmd([(git rev-list --count HEAD 2>/dev/null || echo 0) | tr -d '\n']))dnl
m4_define([v_rev], m4_if($4, dev, [dev_version], [0]))dnl
m4_define([v_rel], [])dnl
m4_define([def_build_profile], m4_if($4, dev, [dev], [release]))dnl
m4_define([eflete_version], m4_if($4, dev, [v_maj.v_min.v_mic.v_rev], [v_maj.v_min.v_mic]))dnl
dnl m4_define([eflete_version], [v_maj.v_min.v_mic])
])

dnl EFLETE_INIT()
dnl Will AC_DEFINE() the following:
dnl     VMAJ = v_maj
dnl     VMIN = v_min
dnl     VMIC = v_mic
dnl     VREV = v_rev
dnl Will AC_SUBST() the following:
dnl     VMAJ = v_maj
dnl     VMIN = v_min
dnl     VMIC = v_mic
dnl     EFL_LTLIBRARY_FLAGS="-no-undefined -version-info ..."
dnl     EFL_LTMODULE_FLAGS="-no-undefined -avoid-version"
dnl Will define the following m4:
dnl     lt_cur = libtool 'current' field of libtool's -version-info
dnl     lt_rev = libtool 'revision' field of libtool's -version-info
dnl     lt_age = libtool 'age' field of libtool's -version-info
AC_DEFUN([EFLETE_INIT],
[dnl
AC_REQUIRE([EFLETE_COLOR])dnl
AC_DEFINE_UNQUOTED([VMAJ], [v_maj], [Major version])dnl
AC_DEFINE_UNQUOTED([VMIN], [v_min], [Minor version])dnl
AC_DEFINE_UNQUOTED([VMIC], [v_mic], [Micro version])dnl
AC_DEFINE_UNQUOTED([VREV], [v_rev], [Revison])dnl
VMAJ=v_maj
VMIN=v_min
VMIC=v_mic
AC_SUBST([VMAJ])dnl
AC_SUBST([VMIN])dnl
AC_SUBST([VMIC])dnl
dnl
dnl TODO: warning - lt_cur:
dnl the previous code assumed v_maj + v_min, but this will be a problem when
dnl we bump v_maj and reset v_min. 1 + 7 == 7 + 1, so if v_maj is bumped
dnl we multiply it by 100.
dnl m4_define([lt_cur], m4_if(m4_cmp(v_maj, 1), 0, m4_eval(v_maj + v_min), m4_eval(v_maj * 100 + v_min)))dnl
dnl m4_define([lt_rev], v_mic)dnl
dnl m4_define([lt_age], v_min)dnl
dnl
dnl EFLETE_LTLIBRARY_FLAGS="-no-undefined -version-info lt_cur:lt_rev:lt_age v_rel"
dnl AC_SUBST(EFLETE_LTLIBRARY_FLAGS)dnl
dnl EFLETE_LTMODULE_FLAGS="-no-undefined -avoid-version"
dnl AC_SUBST([EFLETE_LTMODULE_FLAGS])dnl
dnl AC_MSG_NOTICE([Initialized AC_PACKAGE_NAME (AC_PACKAGE_VERSION) development=dev_version v_rel])
])

