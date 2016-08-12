MAINTAINERCLEANFILES = Makefile.in

LOCALE_DIR = @LOCALE_DIR@

bin_PROGRAMS = eflete

includesub_HEADERS = \
	eflete.h \
	alloc/alloc.h \
	common/common_macro.h \
	common/signals.h \
	common/string_common.h \
	common/validator.h \
	common/widget_macro.h \
	common/widget_list.h \
	common/win32.h \
	config/config.h \
	editor/banned_edje_edit_api.h \
	editor/change.h \
	editor/default.h \
	editor/diff.h \
	editor/editor.h \
	editor/editor_macro.h \
	editor/editor_private.h \
	editor/history.h \
	external/enventor_module.h \
	external/syntax_color.h \
	logger/logger.h \
	project_manager/group_manager.h \
	project_manager/project_manager.h \
	project_manager/project_manager_private.h \
	project_manager/resource_manager.h \
	ui/cursor.h \
	ui/history_ui.h \
	ui/main_window.h \
	ui/project_common.h \
	ui/project_navigator.h \
	ui/property/property.h \
	ui/property/property_common.h \
	ui/property/property_private.h \
	ui/sound_player/sound_player.h \
	ui/tabs.h \
	ui/tabs_private.h \
	ui/live_view/demo.h \
	ui/live_view/live_widget.h \
	ui/live_view/live_widgets.h \
	ui/live_view/elementary/live_elementary_widgets.h \
	ui/modal_window/modal_window.h \
	ui/preference/preference.h \
	ui/shortcuts/shortcuts.h \
	ui/workspace/groupview.h \
	ui/workspace/groupview_private.h \
	ui/workspace/highlight.h\
	ui/workspace/container.h \
	ui/workspace/demo_group.h \
	ui/workspace/group_navigator.h \
	ui/workspace/workspace.h
includesubdir = $(pkgincludedir)-@VMAJ@

noinst_LIBRARIES = libete.a
libete_a_SOURCES = \
../../src/bin/eflete.c \
../../src/bin/common/string_common.c \
../../src/bin/common/validator.c \
../../src/bin/common/widget_list.c \
../../src/bin/project_manager/group_manager.c \
../../src/bin/project_manager/project_manager.c \
../../src/bin/project_manager/project_manager_open.c \
../../src/bin/project_manager/project_manager_import_edj.c \
../../src/bin/project_manager/project_manager_import_edc.c \
../../src/bin/project_manager/project_manager_export_resources.c \
../../src/bin/project_manager/project_manager_export_edc.c \
../../src/bin/project_manager/project_manager_export_edj.c \
../../src/bin/project_manager/resource_manager.c \
../../src/bin/logger/logger.c \
../../src/bin/config/config.c \
../../src/bin/external/syntax_color.c \
../../src/bin/external/enventor_module.c \
../../src/bin/editor/diff.c \
../../src/bin/editor/change.c \
../../src/bin/editor/history.c \
../../src/bin/editor/default.c \
../../src/bin/editor/editor_general.c \
../../src/bin/editor/editor_group.c \
../../src/bin/editor/editor_part.c \
../../src/bin/editor/editor_states.c \
../../src/bin/editor/editor_text.c \
../../src/bin/editor/editor_program.c \
../../src/bin/ui/history_ui.c \
../../src/bin/ui/cursor.c \
../../src/bin/ui/menu.c \
../../src/bin/ui/main_window.c \
../../src/bin/ui/popup.c \
../../src/bin/ui/project_common.c \
../../src/bin/ui/project_close.c \
../../src/bin/ui/project_export.c \
../../src/bin/ui/splash.c \
../../src/bin/ui/shortcuts/shortcuts.c \
../../src/bin/ui/style_manager.c \
../../src/bin/ui/sound_manager.c \
../../src/bin/ui/modal_window/modal_window.c \
../../src/bin/ui/project_navigator.c \
../../src/bin/ui/image_manager.c \
../../src/bin/ui/colorclass_manager.c \
../../src/bin/ui/tabs.c \
../../src/bin/ui/tab_home_common.c \
../../src/bin/ui/tab_home_open.c \
../../src/bin/ui/tab_home_new.c \
../../src/bin/ui/tab_home_import_edj.c \
../../src/bin/ui/tab_home_import_edc.c \
../../src/bin/ui/tab_home_info.c \
../../src/bin/ui/property/property.c \
../../src/bin/ui/property/property_common.c \
../../src/bin/ui/property/property_color_control.c \
../../src/bin/ui/property/property_common_image.c \
../../src/bin/ui/property/property_common_tween.c \
../../src/bin/ui/property/property_image_selector.c \
../../src/bin/ui/property/property_dummy.c \
../../src/bin/ui/property/property_color.c \
../../src/bin/ui/property/property_image.c \
../../src/bin/ui/property/property_sound.c \
../../src/bin/ui/property/property_textblock.c \
../../src/bin/ui/property/property_group.c \
../../src/bin/ui/sound_player/sound_player.c \
../../src/bin/ui/property/property_demo.c \
../../src/bin/ui/workspace/workspace.c \
../../src/bin/ui/workspace/groupview_calc.c \
../../src/bin/ui/workspace/groupview.c \
../../src/bin/ui/workspace/highlight.c \
../../src/bin/ui/workspace/demo_group.c \
../../src/bin/ui/workspace/group_navigator.c \
../../src/bin/ui/workspace/container.c \
../../src/bin/ui/live_view/demo.c \
../../src/bin/ui/live_view/live_widget.c \
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
../../src/bin/ui/live_view/elementary/live_naviframe.c

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
-I${top_srcdir}/src/bin/ui/property \
-I${top_srcdir}/src/bin/ui/sound_player \
-I${top_srcdir}/src/bin/ui/shortcuts \
-I${top_srcdir}/src/bin/ui/container \
-I${top_srcdir}/src/bin/ui/blocks \
-I${top_srcdir}/src/bin/ui/editors \
-I${top_srcdir}/src/bin/ui/modal_window \
-I${top_srcdir}/src/bin/ui/notification \
-I${top_srcdir}/src/bin/ui/workspace \
-I${top_srcdir}/src/bin/ui/live_view \
-I${top_srcdir}/src/bin/ui/live_view/elementary \
-I${top_srcdir}/src/bin/ui/preference \
@EFL_CFLAGS@ \
@EFLETE_DEFINE@

libete_a_CPPFLAGS = ${eflete_CPPFLAGS} \
   -DLOCALE_DIR=\"@LOCALE_DIR@\"

eflete_LDADD = \
libete.a \
@EFL_LIBS@ \
${top_builddir}/src/lib/libewe.la \
@LTLIBINTL@

#Enable after Enventor compabitility will be implement
#if HAVE_ENVENTOR
#eflete_CPPFLAGS += @ENVENTOR_CFLAGS@
#eflete_LDADD += @ENVENTOR_LIBS@
#endif
