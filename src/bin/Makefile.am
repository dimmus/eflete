MAINTAINERCLEANFILES = Makefile.in

LOCALE_DIR = @LOCALE_DIR@

bin_PROGRAMS = eflete

noinst_LIBRARIES = libete.a
libete_a_SOURCES = \
../../src/bin/eflete.c \
../../src/bin/common/string_common.c \
../../src/bin/common/validator.c \
../../src/bin/project_manager/group_manager.c \
../../src/bin/project_manager/widget_manager.c \
../../src/bin/project_manager/project_manager.c \
../../src/bin/logger/logger.c \
../../src/bin/config/config.c \
../../src/bin/external/syntax_color.c \
../../src/bin/external/enventor_module.c \
../../src/bin/editor/diff.c \
../../src/bin/editor/change.c \
../../src/bin/editor/new_history.c \
../../src/bin/editor/default.c \
../../src/bin/editor/editor_general.c \
../../src/bin/editor/editor_group.c \
../../src/bin/editor/editor_part.c \
../../src/bin/editor/editor_states.c \
../../src/bin/editor/editor_text.c \
../../src/bin/ui/history_ui.c \
../../src/bin/ui/group_navigator.c \
../../src/bin/ui/cursor.c \
../../src/bin/ui/menu.c \
../../src/bin/ui/main_window.c \
../../src/bin/ui/popup.c \
../../src/bin/ui/project_common.c \
../../src/bin/ui/project_close.c \
../../src/bin/ui/project_export.c \
../../src/bin/ui/connector.c \
../../src/bin/ui/splash.c \
../../src/bin/ui/shortcuts/shortcuts.c \
../../src/bin/ui/container/container.c \
../../src/bin/ui/preference/preference.c \
../../src/bin/ui/editors/style_editor.c \
../../src/bin/ui/editors/image_editor.c \
../../src/bin/ui/editors/sound_editor.c \
../../src/bin/ui/editors/colorclass_manager.c \
../../src/bin/ui/editors/animator.c \
../../src/bin/ui/editors/animator_program_editor.c \
../../src/bin/ui/editors/animator_program_sequence.c \
../../src/bin/ui/dialogs/part_dialog.c \
../../src/bin/ui/dialogs/style_dialog.c \
../../src/bin/ui/dialogs/about_window.c \
../../src/bin/ui/dialogs/colorsel.c \
../../src/bin/ui/dialogs/item_dialog.c \
../../src/bin/ui/modal_window/modal_window.c \
../../src/bin/ui/project_navigator.c \
../../src/bin/ui/tabs.c \
../../src/bin/ui/tab_home_common.c \
../../src/bin/ui/tab_home_open.c \
../../src/bin/ui/tab_home_new.c \
../../src/bin/ui/tab_home_import_edj.c \
../../src/bin/ui/tab_home_import_edc.c \
../../src/bin/ui/tab_home_info.c \
../../src/bin/ui/blocks/ui_block.c \
../../src/bin/ui/property.c \
../../src/bin/ui/property_group.c \
../../src/bin/ui/property_image.c \
../../src/bin/ui/property_sound.c \
../../src/bin/ui/blocks/ui_widget_list.c \
../../src/bin/ui/blocks/ui_signal_list.c \
../../src/bin/ui/blocks/ui_states_list.c \
../../src/bin/ui/workspace/workspace.c \
../../src/bin/ui/workspace/groupedit_calc.c \
../../src/bin/ui/workspace/groupedit.c \
../../src/bin/ui/workspace/groupedit_box_layouts.c \
../../src/bin/ui/workspace/highlight.c \
../../src/bin/ui/live_view/demo.c \
../../src/bin/ui/live_view/live_view_prop.c \
../../src/bin/ui/live_view/live_view.c \
../../src/bin/ui/live_view/live_widget.c \
../../src/bin/ui/live_view/live_prog_edit.c \
../../src/bin/ui/live_view/elementary/live_widget_common.c \
../../src/bin/ui/live_view/elementary/live_custom_layout.c \
../../src/bin/ui/live_view/elementary/live_actionslider.c \
../../src/bin/ui/live_view/elementary/live_bg.c \
../../src/bin/ui/live_view/elementary/live_bubble.c \
../../src/bin/ui/live_view/elementary/live_button.c \
../../src/bin/ui/live_view/elementary/live_calendar.c \
../../src/bin/ui/live_view/elementary/live_check.c \
../../src/bin/ui/live_view/elementary/live_clock.c \
../../src/bin/ui/live_view/elementary/live_ctxpopup.c \
../../src/bin/ui/live_view/elementary/live_colorselector.c \
../../src/bin/ui/live_view/elementary/live_datetime.c \
../../src/bin/ui/live_view/elementary/live_entry.c \
../../src/bin/ui/live_view/elementary/live_frame.c \
../../src/bin/ui/live_view/elementary/live_gengrid.c \
../../src/bin/ui/live_view/elementary/live_genlist.c \
../../src/bin/ui/live_view/elementary/live_label.c \
../../src/bin/ui/live_view/elementary/live_layout.c \
../../src/bin/ui/live_view/elementary/live_list.c \
../../src/bin/ui/live_view/elementary/live_map.c \
../../src/bin/ui/live_view/elementary/live_multibuttonentry.c \
../../src/bin/ui/live_view/elementary/live_notify.c \
../../src/bin/ui/live_view/elementary/live_panel.c \
../../src/bin/ui/live_view/elementary/live_panes.c \
../../src/bin/ui/live_view/elementary/live_popup.c \
../../src/bin/ui/live_view/elementary/live_radio.c \
../../src/bin/ui/live_view/elementary/live_scroller.c \
../../src/bin/ui/live_view/elementary/live_segment_control.c \
../../src/bin/ui/live_view/elementary/live_separator.c \
../../src/bin/ui/live_view/elementary/live_slider.c \
../../src/bin/ui/live_view/elementary/live_spinner.c \
../../src/bin/ui/live_view/elementary/live_progressbar.c \
../../src/bin/ui/live_view/elementary/live_toolbar.c \
../../src/bin/ui/live_view/elementary/live_naviframe.c \
../../src/bin/ui/notification/notify.c

eflete_SOURCES  = ../../src/bin/main.c

eflete_CPPFLAGS = \
-I${top_srcdir}/src/lib \
-I${top_srcdir}/src/bin/alloc \
-I${top_srcdir}/src/bin/editor \
-I${top_srcdir}/src/bin/external \
-I${top_srcdir}/src/bin/preference \
-I${top_srcdir}/src/bin/common \
-I${top_srcdir}/src/bin/logger \
-I${top_srcdir}/src/bin/project_manager \
-I${top_srcdir}/src/bin/config \
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
@EFL_CFLAGS@ \
@ELEMENTARY_CFLAGS@ \
@EFLETE_DEFINE@

libete_a_CPPFLAGS = ${eflete_CPPFLAGS} \
   -DLOCALE_DIR=\"@LOCALE_DIR@\"

eflete_LDADD = \
libete.a \
@EFL_LIBS@ \
@ELEMENTARY_LIBS@ \
${top_builddir}/src/lib/libewe.la \
@LTLIBINTL@

if HAVE_ENVENTOR
eflete_CPPFLAGS += @ENVENTOR_CFLAGS@
eflete_LDADD += @ENVENTOR_LIBS@
endif
