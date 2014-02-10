AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

AM_V_EDJ = $(am__v_EDJ)
am__v_EDJ = @echo "  EDJ   " $@;

EDJE_CC = @edje_cc@
EDJE_FLAGS = -id $(builddir) -fd $(builddir)

check: Makefile
	$(AM_V_EDJ)$(EDJE_CC) $(EDJE_FLAGS) \
	$(builddir)/radio.edc \
	$(builddir)/radio.edj
clean-local:
	rm -f *.edj
