MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = eflete

noinst_LIBRARIES = libete.a
libete_a_SOURCES = \
eflete.c \
alloc/alloc.c \
loop_manager/loop_manager.c \
project_manager/edje_compile.c \
project_manager/widget_manager.c \
project_manager/project_manager.c \
logger/logger.c \
settings/settings.c \
ui/ui_panes.c \
ui/ui_menu.c \
ui/ui_main_window.c \
ui/ui_blocks_communication.c \
ui/editors/style_editor.c \
ui/editors/image_editor.c \
ui/editors/colorclass_editor.c \
ui/editors/program_editor.c \
ui/dialogs/open_file_dialog.c \
ui/dialogs/save_file_dialog.c \
ui/dialogs/part_dialog.c \
ui/dialogs/state_dialog.c \
ui/dialogs/style_dialog.c \
ui/dialogs/about_window.c \
ui/dialogs/colorsel.c \
ui/modal_window/modal_window.c \
ui/blocks/ui_block.c \
ui/blocks/ui_property.c \
ui/blocks/ui_widget_list.c \
ui/blocks/ui_signal_list.c \
ui/blocks/ui_states_list.c \
ui/workspace/ui_ruler.c \
ui/workspace/ui_workspace.c \
ui/workspace/groupedit_calc.c \
ui/workspace/groupedit.c \
ui/workspace/highlight.c \
ui/demo/ui_demospace.c \
ui/notification/notify.c

eflete_SOURCES  = main.c

eflete_CPPFLAGS = \
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
-I${top_srcdir}/src/ui/demo \
@EINA_CFLAGS@ \
@EET_CFLAGS@ \
@EVAS_CFLAGS@ \
@ECORE_CFLAGS@ \
@EIO_CFLAGS@ \
@EFREET_CFLAGS@ \
@EDJE_CFLAGS@ \
@ELEMENTARY_CFLAGS@ \
@TET_DEFINE@

libete_a_CPPFLAGS = ${eflete_CPPFLAGS}

eflete_LDADD = \
libete.a \
@EINA_LIBS@ \
@EET_LIBS@ \
@EVAS_LIBS@ \
@ECORE_LIBS@ \
@EIO_LIBS@ \
@EFREET_LIBS@ \
@EDJE_LIBS@ \
@ELEMENTARY_LIBS@
