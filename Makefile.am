AM_DISTCHECK_CONFIGURE_FLAGS = --with-tests=regular

if CHECK_FOUND
SUBDIRS = src data doc . tests/edj_build tests po
else
SUBDIRS = src data doc . po
endif

ACLOCAL_AMFLAGS = -I m4

MAINTAINERCLEANFILES = Makefile.in \
	$(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz \
	$(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.bz2

EXTRA_DIST = \
	README \
	AUTHORS \
	COPYING \
	m4/eflete.m4 \
	m4/eflete_binary.m4 \
	m4/ewe.m4 \
	m4/ewe_binary.m4

authorsdir = $(datadir)/eflete
authors_DATA = AUTHORS

.PHONY: doc update-po report report_debug coverage
if HAVE_DOXYGEN
doc:
	$(MAKE) $(AM_MAKEFLAGS) -C doc doc

report:
	echo "Cleaning report files..." && rm -rf tests/*.log tests/html/* *.tmp report.html
	$(MAKE) $(AM_MAKEFLAGS) check || true
	cd tests && doxygen Doxyfile && cd ..
	${top_srcdir}/tests/tests_report_generator.py -o report.html ${top_builddir}/tests/html/ ${top_builddir}/tests/

report_failed:
	echo "Cleaning report files..." && rm -rf tests/*.log tests/html/* *.tmp report.html
	$(MAKE) $(AM_MAKEFLAGS) check || true
	cd tests && doxygen Doxyfile && cd ..
	${top_srcdir}/tests/tests_report_generator.py -i -n -o report.html ${top_builddir}/tests/html/ ${top_builddir}/tests/

report_failed_and_notfound:
	echo "Cleaning report files..." && rm -rf tests/*.log tests/html/* *.tmp report.html
	$(MAKE) $(AM_MAKEFLAGS) check || true
	cd tests && doxygen Doxyfile && cd ..
	${top_srcdir}/tests/tests_report_generator.py -i -o report.html ${top_builddir}/tests/html/ ${top_builddir}/tests/

report_debug:
	echo "Cleaning report files..." && rm -rf tests/*.log tests/html/* *.tmp report.html
	$(MAKE) $(AM_MAKEFLAGS) check || true
	cd tests && doxygen Doxyfile && cd ..
	${top_srcdir}/tests/tests_report_generator.py --tmp -o report.html ${top_builddir}/tests/html/ ${top_builddir}/tests/
endif

update-po:
	$(MAKE) $(AM_MAKEFLAGS) -C po update-po

coverage:
	$(MAKE) $(AM_MAKEFLAGS) -C tests coverage
