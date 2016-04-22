AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

AM_V_EDJ = $(am__v_EDJ)
am__v_EDJ = @echo "  EDJ   " $@;

EDJE_CC = @edje_cc@
EDJE_FLAGS = -id $(builddir) -fd $(builddir) -l license.txt

check: Makefile
	$(AM_V_EDJ)$(EDJE_CC) $(EDJE_FLAGS) \
	$(builddir)/radio.edc \
	$(builddir)/radio.edj
	for i in \
	$(builddir)/history.edj \
	$(builddir)/history_diff_add.edj \
	$(builddir)/test_project_manager.edj \
	$(builddir)/project_manager.edj \
	$(builddir)/project_thread_cancel.edj \
	;do cp $(builddir)/radio.edj $$i; done
clean-local:
	rm -f *.edj

EXTRA_DIST = \
	radio_base.png \
	radio2.png \
	radio.png \
	radio.edc.in \
	PTS55F.ttf \
	license.txt \
	kbd-tap.wav


