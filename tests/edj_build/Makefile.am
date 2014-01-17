AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = @edje_cc@
EDJE_FLAGS = -v -id $(builddir) -fd $(builddir)

check: Makefile
	$(EDJE_CC) $(EDJE_FLAGS) \
	$(builddir)/radio.edc \
	$(builddir)/radio.edj
clean-local:
	rm -f *.edj
