MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4

TESTS = \
ewe_init_test \
ewe_combobox_test \
ewe_ruler_test \
eflete_test \
alloc_test \
colorclass_manager_test \
diff_test \
change_test \
new_history_test \
sound_editor_test \
image_manager_test \
logger_test \
container_test \
modal_window_test \
project_manager_test \
config_test \
cursor_test \
style_editor_test \
highlight_test \
ui_main_window_test \
groupedit_test \
ui_workspace_test \
animator_test \
live_widget_test \
about_window_test \
colorselector_test \
shortcuts_test


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
-I${top_srcdir}/src/bin/ui/container \
-I${top_srcdir}/src/bin/ui/blocks \
-I${top_srcdir}/src/bin/ui/dialogs \
-I${top_srcdir}/src/bin/ui/editors \
-I${top_srcdir}/src/bin/ui/modal_window \
-I${top_srcdir}/src/bin/ui/notification \
-I${top_srcdir}/src/bin/ui/preference \
-I${top_srcdir}/src/bin/ui/workspace \
-I${top_srcdir}/src/bin/ui/workspace/groupedit \
-I${top_srcdir}/src/bin/ui/live_view \
-I${top_srcdir}/src/bin/ui/live_view/elementary \
-I${top_srcdir}/src/bin/ui/shortcuts \
-I${top_srcdir}/tests \
@EFL_CFLAGS@ \
@ELEMENTARY_CFLAGS@ \
@ENVENTOR_CFLAGS@ \
@EFLETE_DEFINE@ \
@CHECK_CFLAGS@

LD = $(top_builddir)/src/bin/libete.a \
@CHECK_LIBS@ \
@EFL_LIBS@ \
@ELEMENTARY_LIBS@ \
${top_builddir}/src/lib/libewe.la \
@ENVENTOR_LIBS@

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

ewe_combobox_test_SOURCES = \
../tests/test_ewe_combobox/test_ewe_combobox.c \
../tests/test_ewe_combobox/ewe_combobox_add.c \
../tests/test_ewe_combobox/ewe_combobox_item_add.c \
../tests/test_ewe_combobox/ewe_combobox_item_del.c \
../tests/test_ewe_combobox/ewe_combobox_item_title_get.c \
../tests/test_ewe_combobox/ewe_combobox_item_title_set.c \
../tests/test_ewe_combobox/ewe_combobox_items_list_get.c \
../tests/test_ewe_combobox/ewe_combobox_select_item_get.c \
../tests/test_ewe_combobox/ewe_combobox_select_item_set.c
ewe_combobox_test_CFLAGS = ${FLAGS}
ewe_combobox_test_LDADD = ${LD}

eflete_test_SOURCES = \
../tests/test_eflete/test_eflete.c \
../tests/test_eflete/app_init.c \
../tests/test_eflete/app_shutdown.c \
../tests/test_eflete/win_layout_get.c \
../tests/test_eflete/main_window_get.c \
../tests/test_eflete/colorselector_get.c
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

new_history_test_SOURCES = \
../tests/test_new_history/test_new_history.c \
../tests/test_new_history/history_add.c \
../tests/test_new_history/history_del.c \
../tests/test_new_history/history_change_add.c \
../tests/test_new_history/history_undo_redo.c
new_history_test_CFLAGS = ${FLAGS}
new_history_test_LDADD = ${LD}

colorclass_manager_test_SOURCES = \
../tests/test_colorclass_manager/test_colorclass_manager.c \
../tests/test_colorclass_manager/colorclass_manager_add.c
colorclass_manager_test_CFLAGS = ${FLAGS}
colorclass_manager_test_LDADD = ${LD}

sound_editor_test_SOURCES = \
../tests/test_sound_editor/test_sound_editor.c \
../tests/test_sound_editor/sound_editor_window_add.c

sound_editor_test_CFLAGS = ${FLAGS}
sound_editor_test_LDADD = ${LD}

image_manager_test_SOURCES = \
../tests/test_image_manager/test_image_manager.c \
../tests/test_image_manager/image_manager_add.c
image_manager_test_CFLAGS = ${FLAGS}
image_manager_test_LDADD = ${LD}

logger_test_SOURCES = \
../tests/test_logger/test_logger.c \
../tests/test_logger/logger_init.c \
../tests/test_logger/logger_shutdown.c
logger_test_CFLAGS = ${FLAGS}
logger_test_LDADD = ${LD}

modal_window_test_SOURCES =  \
../tests/test_modal_window/test_modal_window.c \
../tests/test_modal_window/mw_add.c \
../tests/test_modal_window/mw_title_set.c \
../tests/test_modal_window/mw_info_text_set.c \
../tests/test_modal_window/mw_icon_set.c
modal_window_test_CFLAGS = ${FLAGS}
modal_window_test_LDADD = ${LD}

project_manager_test_SOURCES = \
../tests/test_project_manager/test_project_manager.c \
../tests/test_project_manager/pm_project_import_edj.c \
../tests/test_project_manager/pm_project_import_edc.c \
../tests/test_project_manager/pm_project_thread_cancel.c \
../tests/test_project_manager/pm_project_thread_free.c \
../tests/test_project_manager/pm_project_open.c \
../tests/test_project_manager/pm_project_save.c \
../tests/test_project_manager/pm_project_close.c \
../tests/test_project_manager/pm_project_meta_data_set.c \
../tests/test_project_manager/pm_project_meta_data_get.c \
../tests/test_project_manager/pm_style_resource_export.c
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

style_editor_test_SOURCES = \
../tests/test_style_editor/test_style_editor.c \
../tests/test_style_editor/style_editor_window_add.c
style_editor_test_CFLAGS = ${FLAGS} -DEDC_PATH=\"${srcdir}/test_style_editor/data/\"
style_editor_test_LDADD = ${LD}

container_test_SOURCES = \
../tests/test_container/test_container.c \
../tests/test_container/container_add.c \
../tests/test_container/container_container_size_set.c \
../tests/test_container/container_container_size_get.c \
../tests/test_container/container_min_size_set.c \
../tests/test_container/container_max_size_set.c \
../tests/test_container/container_handler_size_set.c \
../tests/test_container/container_handler_size_get.c \
../tests/test_container/container_style_get.c \
../tests/test_container/container_style_set.c \
../tests/test_container/container_border_hide.c \
../tests/test_container/container_border_show.c \
../tests/test_container/container_padding_size_set.c \
../tests/test_container/container_padding_size_get.c \
../tests/test_container/container_content_set.c \
../tests/test_container/container_content_unset.c
container_test_CFLAGS = ${FLAGS}
container_test_LDADD = ${LD}

highlight_test_SOURCES = \
../tests/test_highlight/test_highlight.c \
../tests/test_highlight/highlight_add.c \
../tests/test_highlight/highlight_handler_disabled_set.c \
../tests/test_highlight/highlight_handler_align_hide.c \
../tests/test_highlight/highlight_handler_align_show.c \
../tests/test_highlight/highlight_handler_align_visible_get.c \
../tests/test_highlight/highlight_handler_mode_set.c \
../tests/test_highlight/highlight_object_follow.c \
../tests/test_highlight/highlight_object_unfollow.c
highlight_test_CFLAGS = ${FLAGS}
highlight_test_LDADD = ${LD}

ui_main_window_test_SOURCES = \
../tests/test_ui_main_window/test_ui_main_window.c \
../tests/test_ui_main_window/ui_main_window_add.c \
../tests/test_ui_main_window/ui_main_window_del.c \
../tests/test_ui_main_window/ui_menu_add.c \
../tests/test_ui_main_window/ui_menu_disable_set.c \
../tests/test_ui_main_window/ui_menu_items_list_disable_set.c \
../tests/test_ui_main_window/ui_part_select.c \
../tests/test_ui_main_window/ui_state_select.c \
../tests/test_ui_main_window/ui_style_back.c \
../tests/test_ui_main_window/ui_style_delete.c
ui_main_window_test_CFLAGS = ${FLAGS}
ui_main_window_test_LDADD = ${LD}

#preference_test_SOURCES = \
#../tests/test_preference/test_preference.c \
#../tests/test_preference/preference_add.c \
#../tests/test_preference/preferences_project_autosave_update.c
#preference_test_CFLAGS = ${FLAGS}
#preference_test_LDADD = ${LD}

ui_workspace_test_SOURCES = \
../tests/test_ui_workspace/test_ui_workspace.c \
../tests/test_ui_workspace/workspace_add.c \
../tests/test_ui_workspace/workspace_edit_object_part_state_set.c \
../tests/test_ui_workspace/workspace_edit_object_recalc.c \
../tests/test_ui_workspace/workspace_zoom_factor_get.c \
../tests/test_ui_workspace/workspace_zoom_factor_set.c \
../tests/test_ui_workspace/workspace_separate_mode_set.c \
../tests/test_ui_workspace/workspace_separate_mode_get.c \
../tests/test_ui_workspace/workspace_highlight_align_visible_get.c \
../tests/test_ui_workspace/workspace_highlight_align_visible_set.c \
../tests/test_ui_workspace/workspace_object_area_visible_get.c \
../tests/test_ui_workspace/workspace_object_area_visible_set.c
ui_workspace_test_CFLAGS = ${FLAGS}
ui_workspace_test_LDADD = ${LD}

groupedit_test_SOURCES = \
../tests/test_groupedit/test_groupedit.c \
../tests/test_groupedit/groupedit_add.c \
../tests/test_groupedit/groupedit_edit_object_part_above.c \
../tests/test_groupedit/groupedit_edit_object_part_below.c \
../tests/test_groupedit/groupedit_edit_object_part_draw_get.c \
../tests/test_groupedit/groupedit_edit_object_part_select.c \
../tests/test_groupedit/groupedit_edit_object_parts_separated.c \
../tests/test_groupedit/groupedit_edit_object_parts_separated_is.c \
../tests/test_groupedit/groupedit_edit_object_recalc_all.c
groupedit_test_CFLAGS = ${FLAGS}
groupedit_test_LDADD = ${LD}

animator_test_SOURCES = \
../tests/test_animator/test_animator.c \
../tests/test_animator/animator_window_add_test.c
animator_test_CFLAGS = ${FLAGS}
animator_test_LDADD = ${LD}

live_widget_test_SOURCES = \
../tests/test_live_widget/test_live_widget.c \
../tests/test_live_widget/live_widget_create.c \
../tests/test_live_widget/live_widget_del.c
live_widget_test_CFLAGS = ${FLAGS}
live_widget_test_LDADD = ${LD}

about_window_test_SOURCES = \
../tests/test_about_window/test_about_window.c \
../tests/test_about_window/about_window_add.c
about_window_test_CFLAGS = ${FLAGS}
about_window_test_LDADD = ${LD}

shortcuts_test_SOURCES = \
../tests/test_shortcuts/test_shortcuts.c \
../tests/test_shortcuts/shortcuts_init.c \
../tests/test_shortcuts/shortcuts_shutdown.c \
../tests/test_shortcuts/shortcuts_profile_load.c
shortcuts_test_CFLAGS = ${FLAGS}
shortcuts_test_LDADD = ${LD}

colorselector_test_SOURCES = \
../tests/test_colorselector/test_colorselector.c \
../tests/test_colorselector/colorselector_add.c \
../tests/test_colorselector/colorselector_color_get.c
colorselector_test_CFLAGS = ${FLAGS}
colorselector_test_LDADD = ${LD}

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
