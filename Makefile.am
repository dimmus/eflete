MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4
if CHECK_FOUND
SUBDIRS = src data doc . tests/edj_build tests
else
SUBDIRS = src data doc .
endif

if HAVE_DOXYGEN
.PHONY: doc
doc:
	$(MAKE) $(AM_MAKEFLAGS) -C doc doc
endif
