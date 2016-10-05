
MAINTAINERCLEANFILES = Makefile.in

EOLIAN_FLAGS = @DEPS_EOLIAN_FLAGS@ \
	       -I$(top_srcdir)/src/lib

include $(top_srcdir)/Makefile_Eolian_Helper.am

AM_CPPFLAGS = \
-I. \
-I$(top_builddir) \
-I$(top_srcdir) \
-I$(top_srcdir)/src/lib \
-I$(top_builddir)/src/lib \
-I${top_srcdir}/src/bin/logger \
-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
-DLOCALE_DIR=\"@LOCALE_DIR@\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DEWE_THEME=\"${datadir}/$(PACKAGE)/themes/ewe.edj\" \
-DEFL_EO_API_SUPPORT \
-DEFL_BETA_API_SUPPORT \
-DELM_INTERNAL_API_ARGESFSDFEFC \
-DEWE_BUILD \
@EFL_CFLAGS@

noinst_LTLIBRARIES = libewe.a

libewe_a_SOURCES = \
ewe_main.c \
ewe_ruler.c

BUILT_SOURCES = \
ewe_ruler.eo.c \
ewe_ruler.eo.h

EXTRA_DIST = ewe_ruler.eo \
ewe_widget_ruler.h \
ewe_private.h \
ewe_main.h \
ewe_ruler.h \
ewe_ruler_legacy.h \
ewe_ruler_eo.h \
ewe_ruler_common.h \
Ewe.h
