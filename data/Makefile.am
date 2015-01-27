AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

AM_V_EDJ = $(am__v_EDJ)
am__v_EDJ = @echo "  EDJ   " $@;
AM_V_EET = $(am__v_EET)
am__v_EET = @echo "  EET   " $@;

EET_EET = @eet_eet@
EDJE_CC = @edje_cc@
EDJE_FLAGS = -id $(top_srcdir)/data/layouts/images -fd $(top_srcdir)/data/fonts
EDJE_FLAGS_DEFAULT = -id $(top_srcdir)/data/themes/default/images -id $(top_srcdir)/data/themes/default/images/splash -fd $(top_srcdir)/data/fonts
EDJE_FLAGS_RESOURCES = -id $(top_srcdir)/data/resources/images

filesdir = $(datadir)/eflete/themes/default
files_DATA = eflete_elm.edj

template_edcdir = $(datadir)/eflete/themes/default/template/edc
template_edc_DATA = \
   $(top_srcdir)/data/template/widgets/*.edc \
   $(top_srcdir)/data/template/macros.edc \
   $(top_srcdir)/data/template/fonts.edc \
   $(top_srcdir)/data/template/colorclasses.edc

template_imgdir = $(datadir)/eflete/themes/default/template/images
template_img_DATA = $(top_srcdir)/data/template/images/*

template_snddir = $(datadir)/eflete/themes/default/template/sounds
template_snd_DATA = $(top_srcdir)/data/template/sounds/*

alayoutsdir = $(datadir)/eflete/layouts
alayouts_DATA = eflete.edj

aimagesdir = $(datadir)/eflete/images
aimages_DATA = \
   ${srcdir}/images/*.png \
   ${srcdir}/images/toolbar_icons/*.png

aresourcesdir = $(datadir)/eflete/
aresources_DATA = eflete_resources.edj

shortcutdir = $(datadir)/applications
shortcut_DATA = eflete.desktop

icondir = $(datadir)/icons
icon_DATA = ${srcdir}/images/eflete.svg

colorsdir = $(datadir)/eflete/themes/default/color
colors_DATA = color.eet

filesdir = $(datadir)/ewe/themes
files_DATA = ewe.edj

ewe.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
	-fd $(top_srcdir)/data/themes/ewe/fonts \
	-id $(top_srcdir)/data/themes/ewe/widgets/images \
	$(top_srcdir)/data/themes/ewe/default.edc \
	$(top_builddir)/data/ewe.edj

eflete.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS) \
	$(top_srcdir)/data/layouts/eflete.edc \
	$(top_builddir)/data/eflete.edj

eflete_resources.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS_RESOURCES) \
	$(top_srcdir)/data/resources/resources.edc \
	$(top_builddir)/data/eflete_resources.edj

eflete_elm.edj: Makefile
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS_DEFAULT) \
	$(top_srcdir)/data/themes/default/default.edc \
	$(top_builddir)/data/eflete_elm.edj

color.eet: Makefile
	$(AM_V_EET)$(EET_EET) -e \
	$(top_builddir)/data/color.eet color \
	$(top_srcdir)/data/color/color.src 1

clean-local:
	rm -f *.edj *.eet
