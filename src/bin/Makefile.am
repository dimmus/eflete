MAINTAINERCLEANFILES = Makefile.in

LOCALE_DIR = @LOCALE_DIR@

bin_PROGRAMS = eflete

noinst_LIBRARIES = libete.a
libete_a_SOURCES = \
eflete.c \
alloc/alloc.c \
project_manager/widget_manager.c \
project_manager/project_manager.c \
logger/logger.c \
config/config.c \
external/syntax_color.c \
history/history.c \
history/history_attribute.c \
history/history_state.c \
history/history_part.c \
history/history_ui.c \
ui/cursor.c \
ui/panes.c \
ui/menu.c \
ui/main_window.c \
ui/connector.c \
ui/splash.c \
ui/wizard/wizard_common.c \
ui/wizard/wizard_import_edj.c \
ui/wizard/wizard_import_edc.c \
ui/shortcuts/shortcuts.c \
ui/container/container.c \
ui/preference/preference.c \
ui/editors/style_editor.c \
ui/editors/image_editor.c \
ui/editors/sound_editor.c \
ui/editors/colorclass_editor.c \
ui/editors/animator.c \
ui/editors/animator_program_editor.c \
ui/editors/animator_program_sequence.c \
ui/dialogs/part_dialog.c \
ui/dialogs/state_dialog.c \
ui/dialogs/style_dialog.c \
ui/dialogs/about_window.c \
ui/dialogs/colorsel.c \
ui/dialogs/item_dialog.c \
ui/modal_window/modal_window.c \
ui/blocks/ui_block.c \
ui/blocks/ui_property.c \
ui/blocks/ui_widget_list.c \
ui/blocks/ui_signal_list.c \
ui/blocks/ui_states_list.c \
ui/workspace/ui_workspace.c \
ui/workspace/groupedit_calc.c \
ui/workspace/groupedit.c \
ui/workspace/groupedit_box_layouts.c \
ui/workspace/highlight.c \
ui/live_view/live_view_prop.c \
ui/live_view/live_view.c \
ui/live_view/live_widget.c \
ui/live_view/live_prog_edit.c \
ui/live_view/elementary/live_widget_common.c \
ui/live_view/elementary/live_custom_layout.c \
ui/live_view/elementary/live_actionslider.c \
ui/live_view/elementary/live_bg.c \
ui/live_view/elementary/live_bubble.c \
ui/live_view/elementary/live_button.c \
ui/live_view/elementary/live_calendar.c \
ui/live_view/elementary/live_check.c \
ui/live_view/elementary/live_clock.c \
ui/live_view/elementary/live_gengrid.c \
ui/live_view/elementary/live_genlist.c \
ui/live_view/elementary/live_label.c \
ui/live_view/elementary/live_list.c \
ui/live_view/elementary/live_notify.c \
ui/live_view/elementary/live_panel.c \
ui/live_view/elementary/live_panes.c \
ui/live_view/elementary/live_radio.c \
ui/live_view/elementary/live_scroller.c \
ui/live_view/elementary/live_separator.c \
ui/live_view/elementary/live_spinner.c \
ui/live_view/elementary/live_toolbar.c \
ui/live_view/elementary/live_naviframe.c \
ui/notification/notify.c

eflete_SOURCES  = main.c

eflete_CPPFLAGS = \
-I${top_srcdir}/src/lib \
-I${top_srcdir}/src/bin/alloc \
-I${top_srcdir}/src/bin/external \
-I${top_srcdir}/src/bin/preference \
-I${top_srcdir}/src/bin/common \
-I${top_srcdir}/src/bin/logger \
-I${top_srcdir}/src/bin/project_manager \
-I${top_srcdir}/src/bin/config \
-I${top_srcdir}/src/bin/history \
-I${top_srcdir}/src/bin/ui \
-I${top_srcdir}/src/bin/ui/shortcuts \
-I${top_srcdir}/src/bin/ui/container \
-I${top_srcdir}/src/bin/ui/blocks \
-I${top_srcdir}/src/bin/ui/dialogs \
-I${top_srcdir}/src/bin/ui/editors \
-I${top_srcdir}/src/bin/ui/modal_window \
-I${top_srcdir}/src/bin/ui/notification \
-I${top_srcdir}/src/bin/ui/workspace \
-I${top_srcdir}/src/bin/ui/live_view \
-I${top_srcdir}/src/bin/ui/live_view/elementary \
-I${top_srcdir}/src/bin/ui/preference \
-I${top_srcdir}/src/bin/ui/wizard \
@EFL_CFLAGS@ \
@ELEMENTARY_CFLAGS@ \
@ENVENTOR_CFLAGS@ \
@EFLETE_DEFINE@

libete_a_CPPFLAGS = ${eflete_CPPFLAGS} \
   -DLOCALE_DIR=\"@LOCALE_DIR@\"

eflete_LDADD = \
libete.a \
@EFL_LIBS@ \
@ELEMENTARY_LIBS@ \
${top_builddir}/src/lib/libewe.la \
@ENVENTOR_LIBS@ \
@LTLIBINTL@
