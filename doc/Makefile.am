MAINTAINERCLEANFILES = Makefile.in

if HAVE_DOXYGEN
directory = $(top_srcdir)/doc/man/man3/

.PHONY: doc

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

doc: doxyfile.stamp
endif
