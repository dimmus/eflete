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
EDJE_FLAGS_TIZEN = -id $(top_srcdir)/data/themes/tizen/images -id $(top_srcdir)/data/themes/tizen/images/splash -fd $(top_srcdir)/data/themes/tizen/fonts
EDJE_FLAGS_RESOURCES = -id $(top_srcdir)/data/resources/images
EDJE_FLAGS_EWE = -fd $(top_srcdir)/data/themes/ewe/fonts -id $(top_srcdir)/data/themes/ewe/widgets/images
EDJE_FLAGS_EWE_TIZEN = -fd $(top_srcdir)/data/themes/tizen/fonts -id $(top_srcdir)/data/themes/ewe_tizen/widgets/images -id  $(top_srcdir)/data/themes/tizen/images

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
if WANT_TIZEN
aimages_DATA = \
	${srcdir}/images/*.png \
	${srcdir}/images/eflete.ico \
	${srcdir}/images/toolbar_icons/*.png
else
aimages_DATA = \
	${srcdir}/images/*.png \
	${srcdir}/images/eflete.svg \
	${srcdir}/images/toolbar_icons/*.png
endif

asounds_DATA = ${srcdir}/sounds/*.wav
asoundsdir = ${datadir}/eflete/sounds

aresourcesdir = $(datadir)/eflete/

shortcutdir = $(datadir)/applications
if WANT_TIZEN
shortcut_DATA = component_designer.desktop
else
shortcut_DATA = eflete.desktop
endif

icondir = $(datadir)/icons
if WANT_TIZEN
icon_DATA = ${srcdir}/themes/tizen/images/General/cd_icon.png
else
icon_DATA = ${srcdir}/images/eflete.svg
endif

colorsdir = $(datadir)/eflete/themes/default/color
colors_DATA = color.eet

ewedir = $(datadir)/eflete/themes/default
ewe_DATA = ewe.edj

if WANT_TIZEN
build_opt = $(EDJE_FLAGS_TIZEN) $(top_srcdir)/data/themes/tizen/default.edc
build_ewe_opt = $(EDJE_FLAGS_EWE_TIZEN) $(top_srcdir)/data/themes/ewe_tizen/default.edc
else
build_opt = $(EDJE_FLAGS_DEFAULT) $(top_srcdir)/data/themes/default/default.edc
build_ewe_opt = $(EDJE_FLAGS_EWE) $(top_srcdir)/data/themes/ewe/default.edc
endif

ewe_deps = \
	$(top_srcdir)/data/*.edc \
	$(top_srcdir)/data/themes/ewe/*.edc \
	$(top_srcdir)/data/themes/ewe/widgets/*.edc \
	$(top_srcdir)/data/themes/ewe/widgets/images/*.png \
	$(top_srcdir)/data/themes/ewe/fonts/*.ttf \
	$(top_srcdir)/data/themes/ewe_tizen/*.edc \
	$(top_srcdir)/data/themes/ewe_tizen/widgets/*.edc \
	$(top_srcdir)/data/themes/ewe_tizen/widgets/images/*.png \
	$(top_srcdir)/data/themes/tizen/fonts/*.ttf

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
	$(top_srcdir)/data/themes/default/images/splash/*.png \
	$(top_srcdir)/data/themes/tizen/*.edc \
	$(top_srcdir)/data/themes/tizen/widgets/*.edc \
	$(top_srcdir)/data/themes/tizen/widgets/layouts/*.edc \
	$(top_srcdir)/data/themes/tizen/images/*.png \
	$(top_srcdir)/data/themes/tizen/images/splash/*.png \
	$(top_srcdir)/data/fonts/*.ttf

color_eet_deps = \
	$(top_srcdir)/data/color/color.src

ewe.edj: Makefile $(ewe_deps)
	$(AM_V_EDJ)$(EDJE_CC) $(build_ewe_opt) \
	$(top_builddir)/data/ewe.edj

eflete.edj: Makefile $(eflete_deps)
	$(AM_V_EDJ)$(EDJE_CC) \
		$(EDJE_FLAGS) \
	$(top_srcdir)/data/layouts/eflete.edc \
	$(top_builddir)/data/eflete.edj

eflete_elm.edj: Makefile $(eflete_elm_deps)
	$(AM_V_EDJ)$(EDJE_CC) $(build_opt) \
	$(top_builddir)/data/eflete_elm.edj

color.eet: Makefile $(color_eet_deps)
	$(AM_V_EET)$(EET_EET) -e \
	$(top_builddir)/data/color.eet color \
	$(top_srcdir)/data/color/color.src 1

clean-local:
	rm -f *.edj *.eet

EXTRA_DIST = $(aimages_DATA) $(asounds_DATA) $(ewe_deps) $(eflete_deps) $(eflete_elm_deps) \
	$(color_eet_deps) $(template_edj_DATA) $(template_edc_DATA) $(template_img_DATA) $(template_snd_DATA)
