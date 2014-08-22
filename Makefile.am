MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4
if CHECK_FOUND
SUBDIRS = src data doc . tests/edj_build tests po
else
SUBDIRS = src data doc . po
endif

.PHONY: doc update-po report report_debug coverage
if HAVE_DOXYGEN
doc:
	$(MAKE) $(AM_MAKEFLAGS) -C doc doc

report:
	echo "Cleaning report files..." && rm -rf tests/*.log tests/html/* *.tmp report.html
	$(MAKE) $(AM_MAKEFLAGS) check || true
	cd tests && doxygen Doxyfile && cd ..
	${top_srcdir}/tests/tests_report_generator.py -o report.html ${top_builddir}/tests/html/ ${top_builddir}/tests/

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

EXTRA_DIST = config.rpath
