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

template_edjdir = $(datadir)/eflete/themes/default/template/edj
template_edj_DATA = \
   $(top_srcdir)/data/template/edj/*.edj

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
	${srcdir}/images/eflete.svg \
	${srcdir}/images/toolbar_icons/*.png

aresourcesdir = $(datadir)/eflete/

shortcutdir = $(datadir)/applications
shortcut_DATA = eflete.desktop

icondir = $(datadir)/icons
icon_DATA = ${srcdir}/images/eflete.svg

colorsdir = $(datadir)/eflete/themes/default/color
colors_DATA = color.eet

ewedir = $(datadir)/ewe/themes
ewe_DATA = ewe.edj

ewe_deps = \
	$(top_srcdir)/data/*.edc \
	$(top_srcdir)/data/themes/ewe/*.edc \
	$(top_srcdir)/data/themes/ewe/widgets/*.edc \
	$(top_srcdir)/data/themes/ewe/widgets/images/*.png \
	$(top_srcdir)/data/themes/ewe/fonts/*.ttf

eflete_deps = \
	$(top_srcdir)/data/*.edc \
	$(top_srcdir)/data/layouts/*.edc \
	$(top_srcdir)/data/layouts/edje/*.edc \
	$(top_srcdir)/data/fonts/*.ttf \
	$(top_srcdir)/data/layouts/images/*.png

eflete_elm_deps = \
	$(top_srcdir)/data/*.edc \
	$(top_srcdir)/data/themes/default/*.edc \
	$(top_srcdir)/data/themes/default/widgets/*.edc \
	$(top_srcdir)/data/themes/default/widgets/layouts/*.edc \
	$(top_srcdir)/data/themes/default/images/*.png \
	$(top_srcdir)/data/fonts/*.ttf \
	$(top_srcdir)/data/themes/default/images/splash/*.png

color_eet_deps = \
	$(top_srcdir)/data/color/color.src

ewe.edj: Makefile $(ewe_deps)
	$(AM_V_EDJ)$(EDJE_CC) \
	-fd $(top_srcdir)/data/themes/ewe/fonts \
	-id $(top_srcdir)/data/themes/ewe/widgets/images \
	$(top_srcdir)/data/themes/ewe/default.edc \
	$(top_builddir)/data/ewe.edj

eflete.edj: Makefile $(eflete_deps)
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS) \
	$(top_srcdir)/data/layouts/eflete.edc \
	$(top_builddir)/data/eflete.edj

eflete_elm.edj: Makefile $(eflete_elm_deps)
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS_DEFAULT) \
	$(top_srcdir)/data/themes/default/default.edc \
	$(top_builddir)/data/eflete_elm.edj

color.eet: Makefile $(color_eet_deps)
	$(AM_V_EET)$(EET_EET) -e \
	$(top_builddir)/data/color.eet color \
	$(top_srcdir)/data/color/color.src 1

clean-local:
	rm -f *.edj *.eet

EXTRA_DIST = $(aimages_DATA) $(ewe_deps) $(eflete_deps) $(eflete_elm_deps) \
	$(color_eet_deps) $(template_edj_DATA) $(template_edc_DATA) $(template_img_DATA) $(template_snd_DATA)
