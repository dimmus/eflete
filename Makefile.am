MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4
if CHECK_FOUND
SUBDIRS = src data doc . tests/edj_build tests po
else
SUBDIRS = src data doc . po
endif

if HAVE_DOXYGEN
.PHONY: doc update-po
doc:
	$(MAKE) $(AM_MAKEFLAGS) -C doc doc
endif

update-po:
	$(MAKE) $(AM_MAKEFLAGS) -C po update-po

EXTRA_DIST = config.rpath
