MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4

TESTS = part_dialog_test \
alloc_test \
colorclass_editor_test \
edje_compile_test \
image_editor_test \
logger_test \
modal_window_test \
notify_test \
project_manager_test \
settings_test \
style_editor_test \
ui_block_test \
ui_highlight_test \
ui_main_window_test \
ui_ruler_test \
ui_signal_list_test \
ui_state_list_test \
ui_widget_list_test \
ui_workspace_test \
widget_manager_test

check_PROGRAMS = ${TESTS}

FLAGS = \
-I${top_srcdir}/src \
-I${top_builddir}/src \
-I${top_srcdir}/src/alloc \
-I${top_srcdir}/src/common \
-I${top_srcdir}/src/logger \
-I${top_srcdir}/src/loop_manager \
-I${top_srcdir}/src/project_manager \
-I${top_srcdir}/src/settings \
-I${top_srcdir}/src/ui \
-I${top_srcdir}/src/ui/blocks \
-I${top_srcdir}/src/ui/dialogs \
-I${top_srcdir}/src/ui/editors \
-I${top_srcdir}/src/ui/modal_window \
-I${top_srcdir}/src/ui/notification \
-I${top_srcdir}/src/ui/workspace \
-I${top_srcdir}/src/ui/live_view \
-I${top_srcdir}/tests \
@EINA_CFLAGS@ \
@EET_CFLAGS@ \
@EVAS_CFLAGS@ \
@ECORE_CFLAGS@ \
@EIO_CFLAGS@ \
@EFREET_CFLAGS@ \
@EDJE_CFLAGS@ \
@ELEMENTARY_CFLAGS@ \
@EFLETE_DEFINE@ \
@CHECK_CFLAGS@

LD = $(top_builddir)/src/libete.a \
@CHECK_LIBS@ \
@EINA_LIBS@ \
@EET_LIBS@ \
@EVAS_LIBS@ \
@ECORE_LIBS@ \
@EIO_LIBS@ \
@EFREET_LIBS@ \
@EDJE_LIBS@ \
@ELEMENTARY_LIBS@

part_dialog_test_SOURCES = test_part_dialog/test_part_dialog.c
part_dialog_test_CFLAGS = ${FLAGS}
part_dialog_test_LDADD = ${LD}

alloc_test_SOURCES = test_alloc/test_alloc.c
alloc_test_CFLAGS = ${FLAGS}
alloc_test_LDADD = ${LD}

colorclass_editor_test_SOURCES = test_colorclass_editor/test_colorclass_editor.c
colorclass_editor_test_CFLAGS = ${FLAGS}
colorclass_editor_test_LDADD = ${LD}

edje_compile_test_SOURCES = test_edje_compile/test_edje_compile.c
edje_compile_test_CFLAGS = ${FLAGS}
edje_compile_test_LDADD = ${LD}

widget_manager_test_SOURCES = test_widget_manager/test_widget_manager.c
widget_manager_test_CFLAGS = ${FLAGS}
widget_manager_test_LDADD = ${LD}

image_editor_test_SOURCES = test_image_editor/test_image_editor.c
image_editor_test_CFLAGS = ${FLAGS}
image_editor_test_LDADD = ${LD}

logger_test_SOURCES = test_logger/test_logger.c
logger_test_CFLAGS = ${FLAGS}
logger_test_LDADD = ${LD}

modal_window_test_SOURCES = test_modal_window/test_modal_window.c
modal_window_test_CFLAGS = ${FLAGS}
modal_window_test_LDADD = ${LD}

notify_test_SOURCES = \
test_notify/test_notify.c \
test_notify/noti_error_show_test.c \
test_notify/noti_warning_show_test.c \
test_notify/noti_info_show_test.c
notify_test_CFLAGS = ${FLAGS}
notify_test_LDADD = ${LD}

project_manager_test_SOURCES = test_project_manager/test_project_manager.c
project_manager_test_CFLAGS = ${FLAGS}
project_manager_test_LDADD = ${LD}

settings_test_SOURCES = test_settings/test_settings.c
settings_test_CFLAGS = ${FLAGS}
settings_test_LDADD = ${LD}

style_editor_test_SOURCES = test_style_editor/test_style_editor.c
style_editor_test_CFLAGS = ${FLAGS} -DEDC_PATH=\"${srcdir}/test_style_editor/data/\"
style_editor_test_LDADD = ${LD}

ui_block_test_SOURCES = test_ui_block/test_ui_block.c
ui_block_test_CFLAGS = ${FLAGS}
ui_block_test_LDADD = ${LD}

ui_groupspace_test_SOURCES = test_ui_groupspace/test_ui_groupspace.c
ui_groupspace_test_CFLAGS = ${FLAGS}
ui_groupspace_test_LDADD = ${LD}

ui_highlight_test_SOURCES = test_ui_highlight/test_ui_highlight.c
ui_highlight_test_CFLAGS = ${FLAGS}
ui_highlight_test_LDADD = ${LD}

ui_main_window_test_SOURCES = test_ui_main_window/test_ui_main_window.c
ui_main_window_test_CFLAGS = ${FLAGS}
ui_main_window_test_LDADD = ${LD}

ui_ruler_test_SOURCES = test_ui_ruler/test_ui_ruler.c
ui_ruler_test_CFLAGS = ${FLAGS}
ui_ruler_test_LDADD = ${LD}

ui_signal_list_test_SOURCES = test_ui_signal_list/test_ui_signal_list.c
ui_signal_list_test_CFLAGS = ${FLAGS}
ui_signal_list_test_LDADD = ${LD}

ui_state_list_test_SOURCES = test_ui_state_list/test_ui_state_list.c
ui_state_list_test_CFLAGS = ${FLAGS}
ui_state_list_test_LDADD = ${LD}

ui_widget_list_test_SOURCES = test_ui_widget_list/test_ui_widget_list.c
ui_widget_list_test_CFLAGS = ${FLAGS}
ui_widget_list_test_LDADD = ${LD}

ui_workspace_test_SOURCES = \
test_ui_workspace/test_ui_workspace.c \
test_ui_workspace/workspace_add.c \
test_ui_workspace/workspace_edit_object_get.c \
test_ui_workspace/workspace_edit_object_part_above.c \
test_ui_workspace/workspace_edit_object_part_add.c \
test_ui_workspace/workspace_edit_object_part_below.c \
test_ui_workspace/workspace_edit_object_part_del.c \
test_ui_workspace/workspace_edit_object_part_state_add.c \
test_ui_workspace/workspace_edit_object_part_state_del.c \
test_ui_workspace/workspace_edit_object_part_state_set.c \
test_ui_workspace/workspace_edit_object_recalc.c \
test_ui_workspace/workspace_edit_object_set.c \
test_ui_workspace/workspace_edit_object_unset.c \
test_ui_workspace/workspace_edit_object_visible_set.c \
test_ui_workspace/workspace_highlight_set.c \
test_ui_workspace/workspace_highlight_unset.c \
test_ui_workspace/workspace_background_image_set.c \
test_ui_workspace/workspace_legend_visible_set.c \
test_ui_workspace/workspace_zoom_factor_get.c \
test_ui_workspace/workspace_zoom_factor_set.c

ui_workspace_test_CFLAGS = ${FLAGS}
ui_workspace_test_LDADD = ${LD}
