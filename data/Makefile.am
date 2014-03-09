AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

AM_V_EDJ = $(am__v_EDJ)
am__v_EDJ = @echo "  EDJ   " $@;

EDJE_CC = @edje_cc@
EDJE_FLAGS = -id $(top_srcdir)/data/layouts/images -fd $(top_srcdir)/data/themes
EDJE_FLAGS_DEFAULT = -id $(top_srcdir)/data/themes/default/images -fd $(top_srcdir)/data/themes/default/fonts
EDJE_FLAGS_TEMPLATE = -id $(top_srcdir)/data/template/images -fd $(top_srcdir)/data/themes

filesdir = $(datadir)/eflete/themes/default
files_DATA = template.edj default.edj

alayoutsdir = $(datadir)/eflete/layouts
alayouts_DATA = eflete.edj

aimagesdir = $(datadir)/eflete/images
aimages_DATA = \
   ${srcdir}/layouts/images/*.png ${srcdir}/layouts/images/*.jpg

shortcutdir = $(datadir)/applications
shortcut_DATA = eflete.desktop

icondir = $(datadir)/icons
icon_DATA = ${srcdir}/layouts/images/eflete.svg

eflete.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS) \
	$(top_srcdir)/data/layouts/eflete.edc \
	$(top_builddir)/data/eflete.edj

template.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS_TEMPLATE) \
	$(top_srcdir)/data/template/template.edc \
	$(top_builddir)/data/template.edj

default.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS_DEFAULT) \
	$(top_srcdir)/data/themes/default/default.edc \
	$(top_builddir)/data/default.edj

clean-local:
	rm -f *.edj
