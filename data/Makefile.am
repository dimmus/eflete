AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = @edje_cc@
EDJE_FLAGS = -v -id $(top_srcdir)/data/themes/default/images -fd $(top_srcdir)/data/themes
EDJE_FLAGS_TEMPLATE = -v -id $(top_srcdir)/data/themes/default/images/template -fd $(top_srcdir)/data/themes

filesdir = $(datadir)/eflete/themes/default
files_DATA = eflete.edj template.edj

imagesdir = $(datadir)/eflete/images
images_DATA = \
   ${srcdir}/images/*.png \
   ${srcdir}/images/*.svg

eflete.edj: Makefile
	$(EDJE_CC) $(EDJE_FLAGS) \
	$(top_srcdir)/data/themes/default/eflete.edc \
	$(top_builddir)/data/eflete.edj
template.edj: Makefile
	$(EDJE_CC) $(EDJE_FLAGS_TEMPLATE) \
	$(top_srcdir)/data/themes/default/template.edc \
	$(top_builddir)/data/template.edj
clean-local:
	rm -f *.edj
