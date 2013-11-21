AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = @edje_cc@
EDJE_FLAGS = -v -id $(srcdir) -fd $(srcdir)

check: Makefile
	$(EDJE_CC) $(EDJE_FLAGS) \
	$(srcdir)/radio.edc \
	$(builddir)/radio.edj
clean-local:
	rm -f *.edj
