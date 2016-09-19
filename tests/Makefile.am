MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4

TESTS = \
ewe_init_test \
ewe_ruler_test \
eflete_test \
alloc_test \
diff_test \
change_test \
history_test \
logger_test \
project_manager_test \
config_test \
cursor_test

check_PROGRAMS = ${TESTS}

FLAGS = \
-I${top_srcdir}/src/lib \
-I${top_srcdir}/src/bin \
-I${top_srcdir}/src/bin/alloc \
-I${top_srcdir}/src/bin/external \
-I${top_srcdir}/src/bin/common \
-I${top_srcdir}/src/bin/editor \
-I${top_srcdir}/src/bin/logger \
-I${top_srcdir}/src/bin/loop_manager \
-I${top_srcdir}/src/bin/project_manager \
-I${top_srcdir}/src/bin/config \
-I${top_srcdir}/src/bin/history \
-I${top_srcdir}/src/bin/ui \
-I${top_srcdir}/src/bin/ui/blocks \
-I${top_srcdir}/src/bin/ui/dialogs \
-I${top_srcdir}/src/bin/ui/editors \
-I${top_srcdir}/src/bin/ui/notification \
-I${top_srcdir}/src/bin/ui/live_view \
-I${top_srcdir}/src/bin/ui/live_view/elementary \
-I${top_srcdir}/tests \
@EFL_CFLAGS@ \
@EFLETE_DEFINE@ \
@CHECK_CFLAGS@

LD = $(top_builddir)/src/bin/libete.a \
${top_builddir}/src/lib/libewe.a \
@CHECK_LIBS@ \
@EFL_LIBS@



ewe_init_test_SOURCES = \
../tests/test_ewe_init/test_ewe_init.c \
../tests/test_ewe_init/ewe_init.c
ewe_init_test_CFLAGS = ${FLAGS}
ewe_init_test_LDADD = ${LD}

ewe_ruler_test_SOURCES = \
../tests/test_ewe_ruler/test_ewe_ruler.c \
../tests/test_ewe_ruler/ewe_ruler_add.c \
../tests/test_ewe_ruler/ewe_ruler_scale_add.c \
../tests/test_ewe_ruler/ewe_ruler_scale_del.c \
../tests/test_ewe_ruler/ewe_ruler_scale_visible_get.c \
../tests/test_ewe_ruler/ewe_ruler_scale_visible_set.c \
../tests/test_ewe_ruler/ewe_ruler_scale_middle_mark_get.c \
../tests/test_ewe_ruler/ewe_ruler_scale_middle_mark_set.c \
../tests/test_ewe_ruler/ewe_ruler_horizontal_get.c \
../tests/test_ewe_ruler/ewe_ruler_horizontal_set.c \
../tests/test_ewe_ruler/ewe_ruler_step_set.c \
../tests/test_ewe_ruler/ewe_ruler_step_get.c \
../tests/test_ewe_ruler/ewe_ruler_value_step_set.c \
../tests/test_ewe_ruler/ewe_ruler_value_step_get.c \
../tests/test_ewe_ruler/ewe_ruler_zero_offset_set.c \
../tests/test_ewe_ruler/ewe_ruler_zero_offset_get.c \
../tests/test_ewe_ruler/ewe_ruler_format_get.c \
../tests/test_ewe_ruler/ewe_ruler_format_set.c \
../tests/test_ewe_ruler/ewe_ruler_style_get.c \
../tests/test_ewe_ruler/ewe_ruler_style_set.c \
../tests/test_ewe_ruler/ewe_ruler_marker_style_get.c \
../tests/test_ewe_ruler/ewe_ruler_marker_style_set.c \
../tests/test_ewe_ruler/ewe_ruler_marker_absolute_get.c \
../tests/test_ewe_ruler/ewe_ruler_marker_absolute_set.c \
../tests/test_ewe_ruler/ewe_ruler_marker_relative_get.c \
../tests/test_ewe_ruler/ewe_ruler_marker_relative_set.c \
../tests/test_ewe_ruler/ewe_ruler_marker_visible_get.c \
../tests/test_ewe_ruler/ewe_ruler_marker_visible_set.c \
../tests/test_ewe_ruler/ewe_ruler_marker_add.c \
../tests/test_ewe_ruler/ewe_ruler_marker_del.c

ewe_ruler_test_CFLAGS = ${FLAGS}
ewe_ruler_test_LDADD = ${LD}

eflete_test_SOURCES = \
../tests/test_eflete/test_eflete.c \
../tests/test_eflete/app_init.c \
../tests/test_eflete/app_shutdown.c
eflete_test_CFLAGS = ${FLAGS}
eflete_test_LDADD = ${LD}

alloc_test_SOURCES = \
../tests/test_alloc/test_alloc.c \
../tests/test_alloc/mem_malloc.c \
../tests/test_alloc/mem_calloc.c
alloc_test_CFLAGS = ${FLAGS}
alloc_test_LDADD = ${LD}

diff_test_SOURCES = \
../tests/test_diff/test_diff.c \
../tests/test_diff/diff_free.c \
../tests/test_diff/diff_update.c \
../tests/test_diff/diff_undo_redo.c
diff_test_CFLAGS = ${FLAGS}
diff_test_LDADD = ${LD}

change_test_SOURCES = \
../tests/test_change/test_change.c \
../tests/test_change/change_add.c \
../tests/test_change/change_diff_add.c \
../tests/test_change/change_diff_merge_add.c \
../tests/test_change/change_free.c
change_test_CFLAGS = ${FLAGS}
change_test_LDADD = ${LD}

history_test_SOURCES = \
../tests/test_history/test_history.c \
../tests/test_history/history_add.c \
../tests/test_history/history_del.c \
../tests/test_history/history_change_add.c \
../tests/test_history/history_undo_redo.c
history_test_CFLAGS = ${FLAGS}
history_test_LDADD = ${LD}

logger_test_SOURCES = \
../tests/test_logger/test_logger.c \
../tests/test_logger/logger_init.c \
../tests/test_logger/logger_shutdown.c
logger_test_CFLAGS = ${FLAGS}
logger_test_LDADD = ${LD}

project_manager_test_SOURCES = \
../tests/test_project_manager/test_project_manager.c \
../tests/test_project_manager/pm_project_import_edj.c \
../tests/test_project_manager/pm_project_import_edc.c \
../tests/test_project_manager/pm_project_thread_cancel.c \
../tests/test_project_manager/pm_project_open.c \
../tests/test_project_manager/pm_project_save.c \
../tests/test_project_manager/pm_project_close.c \
../tests/test_project_manager/pm_project_meta_data_set.c \
../tests/test_project_manager/pm_project_meta_data_get.c
project_manager_test_CFLAGS = ${FLAGS}
project_manager_test_LDADD = ${LD}

config_test_SOURCES = \
../tests/test_config/test_config.c \
../tests/test_config/config_init.c\
../tests/test_config/config_shutdown.c\
../tests/test_config/profile_load.c \
../tests/test_config/profile_save.c \
../tests/test_config/profile_get.c \
../tests/test_config/profiles_get.c \
../tests/test_config/config_save.c \
../tests/test_config/config_load.c \
../tests/test_config/config_get.c \
../tests/test_config/config_panes_sizes_data_update.c
config_test_CFLAGS = ${FLAGS}
config_test_LDADD = ${LD}

cursor_test_SOURCES = \
../tests/test_cursor/test_cursor.c \
../tests/test_cursor/cursor_main_set.c \
../tests/test_cursor/cursor_main_get.c \
../tests/test_cursor/cursor_type_set.c \
../tests/test_cursor/cursor_type_get.c
cursor_test_CFLAGS = ${FLAGS}
cursor_test_LDADD = ${LD}

live_widget_test_SOURCES = \
../tests/test_live_widget/test_live_widget.c \
../tests/test_live_widget/live_widget_create.c \
../tests/test_live_widget/live_widget_del.c
live_widget_test_CFLAGS = ${FLAGS}
live_widget_test_LDADD = ${LD}

coverage:
	echo "<html><body>" > ../coverage.html
	lcov --zerocounters --directory ../src
	lcov -i --directory ../src -c -o coverage_base.info
	+make check || true
	lcov --directory ../src -c -o coverage.info
	lcov --add-tracefile coverage_base.info --add-tracefile coverage.info --output-file coverage_total.info
	genhtml coverage_total.info -o ../coverage/cov_ALL -t Total_coverage
	echo "<p><a href='coverage/cov_ALL/index.html'>Full</a></p>" >> ../coverage.html
	for i in ${TESTS}; \
	do \
		echo "Processing module: $$i..." ; \
		lcov --zerocounters --directory ../src ; \
		./$$i 2>/dev/null >/dev/null ; \
		rm coverage.info ; \
		lcov --directory ../src -c -o coverage.info ; \
		genhtml coverage.info -o ../coverage/cov_$$i -t $$i ; \
		echo "<p><a href='coverage/cov_$$i/index.html'>$$i</a></p>" >> ../coverage.html ; \
		echo "" ; \
	done;
	echo "</body></html>" >> ../coverage.html

EXTRA_DIST = \
	tests_report_generator.py \
	test_common.h \
	utc_common.h \
	test_ewe_init/test_ewe_init.h \
	test_ewe_ruler/test_ewe_ruler.h \
	test_eflete/test_eflete.h \
	test_alloc/test_alloc.h \
	test_diff/test_diff.h \
	test_change/test_change.h \
	test_history/test_history.h \
	test_logger/test_logger.h \
	test_project_manager/test_project_manager.h \
	test_config/test_config.h \
	test_cursor/test_cursor.h \
	test_live_widget/test_live_widget.h
