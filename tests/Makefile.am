MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4

TESTS = \
eflete_test \
part_dialog_test \
compile_dialog_test \
save_file_dialog_test \
open_file_dialog_test \
alloc_test \
colorclass_editor_test \
edje_compile_test \
sound_editor_test \
image_editor_test \
logger_test \
container_test \
modal_window_test \
notify_test \
project_manager_test \
config_test \
cursor_test \
style_editor_test \
style_dialog_test \
ui_block_test \
highlight_test \
ui_main_window_test \
ui_signal_list_test \
ui_states_list_test \
ui_state_dialog_test \
ui_widget_list_test \
groupedit_test \
ui_workspace_test \
widget_manager_test \
animator_test \
live_view_test \
live_view_prop_test \
live_widget_test \
about_window_test \
colorselector_test \
shortcuts_test

check_PROGRAMS = ${TESTS}

FLAGS = \
-I${top_srcdir}/src \
-I${top_builddir}/src \
-I${top_srcdir}/src/alloc \
-I${top_srcdir}/src/external \
-I${top_srcdir}/src/preference \
-I${top_srcdir}/src/common \
-I${top_srcdir}/src/logger \
-I${top_srcdir}/src/loop_manager \
-I${top_srcdir}/src/project_manager \
-I${top_srcdir}/src/config \
-I${top_srcdir}/src/ui \
-I${top_srcdir}/src/ui/container \
-I${top_srcdir}/src/ui/blocks \
-I${top_srcdir}/src/ui/dialogs \
-I${top_srcdir}/src/ui/editors \
-I${top_srcdir}/src/ui/modal_window \
-I${top_srcdir}/src/ui/notification \
-I${top_srcdir}/src/ui/workspace \
-I${top_srcdir}/src/ui/workspace/groupedit \
-I${top_srcdir}/src/ui/live_view \
-I${top_srcdir}/src/ui/live_view/elementary \
-I${top_srcdir}/src/ui/signal_list \
-I${top_srcdir}/src/ui/shortcuts \
-I${top_srcdir}/tests \
@EFL_CFLAGS@ \
@ELEMENTARY_CFLAGS@ \
@EWE_CFLAGS@ \
@EFLETE_DEFINE@ \
@CHECK_CFLAGS@

LD = $(top_builddir)/src/libete.a \
@CHECK_LIBS@ \
@EFL_LIBS@ \
@EWE_LIBS@ \
@ELEMENTARY_LIBS@

eflete_test_SOURCES = \
test_eflete/test_eflete.c \
test_eflete/app_init.c \
test_eflete/app_shutdown.c \
test_eflete/win_layout_get.c \
test_eflete/app_data_get.c \
test_eflete/app_free.c \
test_eflete/main_window_get.c \
test_eflete/colorselector_get.c
eflete_test_CFLAGS = ${FLAGS}
eflete_test_LDADD = ${LD}

part_dialog_test_SOURCES = \
test_part_dialog/test_part_dialog.c \
test_part_dialog/part_dialog_add.c
part_dialog_test_CFLAGS = ${FLAGS}
part_dialog_test_LDADD = ${LD}

save_file_dialog_test_SOURCES = \
test_save_file_dialog/test_save_file_dialog.c \
test_save_file_dialog/save_as_edc_file.c \
test_save_file_dialog/save_as_edj_file.c
save_file_dialog_test_CFLAGS = ${FLAGS}
save_file_dialog_test_LDADD = ${LD}

compile_dialog_test_SOURCES = \
test_compile_dialog/test_compile_dialog.c \
test_compile_dialog/compile_dialog.c
compile_dialog_test_CFLAGS = ${FLAGS}
compile_dialog_test_LDADD = ${LD}

open_file_dialog_test_SOURCES = \
test_open_file_dialog/test_open_file_dialog.c \
test_open_file_dialog/open_edj_file.c
open_file_dialog_test_CFLAGS = ${FLAGS}
open_file_dialog_test_LDADD = ${LD}

alloc_test_SOURCES = \
test_alloc/test_alloc.c \
test_alloc/mem_malloc.c \
test_alloc/mem_calloc.c
alloc_test_CFLAGS = ${FLAGS}
alloc_test_LDADD = ${LD}

colorclass_editor_test_SOURCES = \
test_colorclass_editor/test_colorclass_editor.c \
test_colorclass_editor/colorclass_viewer_add.c
colorclass_editor_test_CFLAGS = ${FLAGS}
colorclass_editor_test_LDADD = ${LD}

edje_compile_test_SOURCES = \
test_edje_compile/test_edje_compile.c \
test_edje_compile/compile.c \
test_edje_compile/decompile.c
edje_compile_test_CFLAGS = ${FLAGS}
edje_compile_test_LDADD = ${LD}

widget_manager_test_SOURCES = \
test_widget_manager/test_widget_manager.c \
test_widget_manager/wm_part_add.c \
test_widget_manager/wm_class_add.c \
test_widget_manager/wm_class_free.c \
test_widget_manager/wm_part_current_state_set.c \
test_widget_manager/wm_part_del.c \
test_widget_manager/wm_part_type_get.c \
test_widget_manager/wm_program_signals_list_get.c \
test_widget_manager/wm_program_signals_list_free.c \
test_widget_manager/wm_style_add.c \
test_widget_manager/wm_style_copy.c \
test_widget_manager/wm_style_free.c \
test_widget_manager/wm_style_object_find.c \
test_widget_manager/wm_widget_add.c \
test_widget_manager/wm_widget_free.c \
test_widget_manager/wm_widget_list_free.c \
test_widget_manager/wm_widget_list_new.c \
test_widget_manager/wm_widget_list_layouts_load.c \
test_widget_manager/wm_layouts_list_objects_load.c \
test_widget_manager/wm_widget_list_objects_load.c
widget_manager_test_CFLAGS = ${FLAGS}
widget_manager_test_LDADD = ${LD}

sound_editor_test_SOURCES = \
test_sound_editor/test_sound_editor.c \
test_sound_editor/sound_editor_window_add.c
sound_editor_test_CFLAGS = ${FLAGS}
sound_editor_test_LDADD = ${LD}

image_editor_test_SOURCES = \
test_image_editor/test_image_editor.c \
test_image_editor/image_editor_callback_add.c \
test_image_editor/image_editor_file_choose.c \
test_image_editor/image_editor_window_add.c
image_editor_test_CFLAGS = ${FLAGS}
image_editor_test_LDADD = ${LD}

logger_test_SOURCES = \
test_logger/test_logger.c \
test_logger/logger_init.c \
test_logger/logger_shutdown.c
logger_test_CFLAGS = ${FLAGS}
logger_test_LDADD = ${LD}

modal_window_test_SOURCES =  \
test_modal_window/test_modal_window.c \
test_modal_window/mw_add.c \
test_modal_window/mw_about_add.c \
test_modal_window/mw_title_set.c \
test_modal_window/mw_info_text_set.c \
test_modal_window/mw_icon_set.c
modal_window_test_CFLAGS = ${FLAGS}
modal_window_test_LDADD = ${LD}

notify_test_SOURCES = \
test_notify/test_notify.c \
test_notify/noti_error_show_test.c \
test_notify/noti_warning_show_test.c \
test_notify/noti_info_show_test.c
notify_test_CFLAGS = ${FLAGS}
notify_test_LDADD = ${LD}

project_manager_test_SOURCES = \
test_project_manager/test_project_manager.c \
test_project_manager/pm_export_to_edc.c \
test_project_manager/pm_open_project_edj.c \
test_project_manager/pm_project_close.c \
test_project_manager/pm_save_as_project_edj.c \
test_project_manager/pm_save_project_edj.c \
test_project_manager/pm_save_project_to_swap.c
project_manager_test_CFLAGS = ${FLAGS}
project_manager_test_LDADD = ${LD}

config_test_SOURCES = \
test_config/test_config.c \
test_config/config_init.c\
test_config/config_shutdown.c\
test_config/profile_load.c \
test_config/profile_save.c \
test_config/profile_get.c \
test_config/profiles_get.c \
test_config/config_save.c \
test_config/config_load.c \
test_config/config_get.c \
test_config/config_panes_sizes_data_update.c
config_test_CFLAGS = ${FLAGS}
config_test_LDADD = ${LD}

cursor_test_SOURCES = \
test_cursor/test_cursor.c \
test_cursor/cursor_main_set.c \
test_cursor/cursor_main_get.c \
test_cursor/cursor_type_set.c \
test_cursor/cursor_type_get.c
cursor_test_CFLAGS = ${FLAGS}
cursor_test_LDADD = ${LD}

style_editor_test_SOURCES = \
test_style_editor/test_style_editor.c \
test_style_editor/style_editor_window_add.c
style_editor_test_CFLAGS = ${FLAGS} -DEDC_PATH=\"${srcdir}/test_style_editor/data/\"
style_editor_test_LDADD = ${LD}

style_dialog_test_SOURCES = \
test_style_dialog/test_style_dialog.c \
test_style_dialog/style_dialog_add.c
style_dialog_test_CFLAGS = ${FLAGS}
style_dialog_test_LDADD = ${LD}

container_test_SOURCES = \
test_container/test_container.c \
test_container/container_add.c \
test_container/container_container_size_set.c \
test_container/container_container_size_get.c \
test_container/container_min_size_set.c \
test_container/container_max_size_set.c \
test_container/container_handler_size_set.c \
test_container/container_handler_size_get.c \
test_container/container_style_get.c \
test_container/container_style_set.c \
test_container/container_border_hide.c \
test_container/container_border_show.c \
test_container/container_padding_size_set.c \
test_container/container_padding_size_get.c \
test_container/container_content_set.c \
test_container/container_content_unset.c \
test_container/container_confine_set.c \
test_container/container_confine_unset.c
container_test_CFLAGS = ${FLAGS}
container_test_LDADD = ${LD}

ui_block_test_SOURCES = \
test_ui_block/test_ui_block.c \
test_ui_block/ui_block_add.c \
test_ui_block/ui_block_graph_vision.c \
test_ui_block/ui_block_live_view.c \
test_ui_block/ui_block_property.c \
test_ui_block/ui_block_signal_list.c \
test_ui_block/ui_block_state_list.c \
test_ui_block/ui_block_title_text.c \
test_ui_block/ui_block_title_visible.c \
test_ui_block/ui_block_content_visible.c \
test_ui_block/ui_block_widget_list.c \
test_ui_block/ui_block_ws_set.c
ui_block_test_CFLAGS = ${FLAGS}
ui_block_test_LDADD = ${LD}

highlight_test_SOURCES = \
test_highlight/test_highlight.c \
test_highlight/highlight_add.c \
test_highlight/highlight_handler_disabled_set.c \
test_highlight/highlight_handler_align_hide.c \
test_highlight/highlight_handler_align_show.c \
test_highlight/highlight_handler_align_visible_get.c \
test_highlight/highlight_handler_mode_set.c \
test_highlight/highlight_object_follow.c \
test_highlight/highlight_object_unfollow.c
highlight_test_CFLAGS = ${FLAGS}
highlight_test_LDADD = ${LD}

ui_main_window_test_SOURCES = \
test_ui_main_window/test_ui_main_window.c \
test_ui_main_window/new_theme_create.c \
test_ui_main_window/register_callbacks.c \
test_ui_main_window/ui_edj_load.c \
test_ui_main_window/ui_main_window_add.c \
test_ui_main_window/ui_main_window_del.c \
test_ui_main_window/ui_menu_add.c \
test_ui_main_window/ui_menu_base_disabled_set.c \
test_ui_main_window/ui_menu_disable_set.c \
test_ui_main_window/ui_menu_locked_set.c \
test_ui_main_window/ui_panes_add.c \
test_ui_main_window/ui_panes_hide.c \
test_ui_main_window/ui_panes_show.c \
test_ui_main_window/ui_panes_left_panes_min_size_toggle.c \
test_ui_main_window/ui_part_back.c \
test_ui_main_window/ui_part_select.c \
test_ui_main_window/ui_state_select.c \
test_ui_main_window/ui_style_back.c \
test_ui_main_window/ui_style_clicked.c \
test_ui_main_window/ui_style_delete.c \
test_ui_main_window/add_callbacks_wd.c \
test_ui_main_window/code_edit_mode_switch.c
ui_main_window_test_CFLAGS = ${FLAGS}
ui_main_window_test_LDADD = ${LD}

#preference_test_SOURCES = \
#test_preference/test_preference.c \
#test_preference/preference_add_test.c
#preference_test_CFLAGS = ${FLAGS}
#preference_test_LDADD = ${LD}

ui_signal_list_test_SOURCES = \
test_ui_signal_list/test_ui_signal_list.c \
test_ui_signal_list/ui_signal_list_add.c \
test_ui_signal_list/ui_signal_list_data_set.c \
test_ui_signal_list/ui_signal_list_data_unset.c
ui_signal_list_test_CFLAGS = ${FLAGS}
ui_signal_list_test_LDADD = ${LD}

ui_states_list_test_SOURCES = \
test_ui_states_list/test_ui_states_list.c \
test_ui_states_list/ui_states_list_selected_state_get.c \
test_ui_states_list/ui_states_list_selected_state_del.c \
test_ui_states_list/ui_states_list_state_add.c \
test_ui_states_list/ui_states_list_add.c \
test_ui_states_list/ui_states_list_data_set.c \
test_ui_states_list/ui_states_list_data_unset.c \
test_ui_states_list/ui_states_list_part_get.c
ui_states_list_test_CFLAGS = ${FLAGS}
ui_states_list_test_LDADD = ${LD}

ui_widget_list_test_SOURCES = \
test_ui_widget_list/test_ui_widget_list.c \
test_ui_widget_list/ui_widget_list_add.c \
test_ui_widget_list/ui_widget_list_class_data_reload.c \
test_ui_widget_list/ui_widget_list_data_set.c \
test_ui_widget_list/ui_widget_list_layouts_reload.c \
test_ui_widget_list/ui_widget_list_part_add.c \
test_ui_widget_list/ui_widget_list_selected_part_above.c \
test_ui_widget_list/ui_widget_list_selected_part_below.c \
test_ui_widget_list/ui_widget_list_selected_part_del.c \
test_ui_widget_list/ui_widget_list_selected_part_get.c \
test_ui_widget_list/ui_widget_list_part_selected_set.c \
test_ui_widget_list/ui_widget_list_selected_parts_get.c \
test_ui_widget_list/ui_widget_list_title_set.c
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
test_ui_workspace/workspace_edit_object_part_rename.c \
test_ui_workspace/workspace_edit_object_set.c \
test_ui_workspace/workspace_edit_object_unset.c \
test_ui_workspace/workspace_edit_object_visible_set.c \
test_ui_workspace/workspace_highlight_set.c \
test_ui_workspace/workspace_highlight_unset.c \
test_ui_workspace/workspace_zoom_factor_get.c \
test_ui_workspace/workspace_zoom_factor_set.c \
test_ui_workspace/workspace_separate_mode_set.c \
test_ui_workspace/workspace_separate_mode_get.c \
test_ui_workspace/workspace_edit_object_part_restack.c \
test_ui_workspace/workspace_highlight_align_visible_get.c \
test_ui_workspace/workspace_highlight_align_visible_set.c \
test_ui_workspace/workspace_object_area_visible_get.c \
test_ui_workspace/workspace_object_area_visible_set.c
ui_workspace_test_CFLAGS = ${FLAGS}
ui_workspace_test_LDADD = ${LD}

groupedit_test_SOURCES = \
test_groupedit/test_groupedit.c \
test_groupedit/groupedit_add.c \
test_groupedit/groupedit_bg_set.c \
test_groupedit/groupedit_bg_unset.c \
test_groupedit/groupedit_edit_object_part_above.c \
test_groupedit/groupedit_edit_object_part_add.c \
test_groupedit/groupedit_edit_object_part_below.c \
test_groupedit/groupedit_edit_object_part_del.c \
test_groupedit/groupedit_edit_object_part_draw_get.c \
test_groupedit/groupedit_edit_object_part_select.c \
test_groupedit/groupedit_edit_object_part_state_add.c \
test_groupedit/groupedit_edit_object_part_state_del.c \
test_groupedit/groupedit_edit_object_part_state_set.c \
test_groupedit/groupedit_edit_object_parts_separated.c \
test_groupedit/groupedit_edit_object_parts_separated_is.c \
test_groupedit/groupedit_edit_object_recalc_all.c \
test_groupedit/groupedit_edit_object_set.c \
test_groupedit/groupedit_edit_object_unset.c \
test_groupedit/groupedit_part_object_area_get.c \
test_groupedit/groupedit_part_object_area_visible_get.c \
test_groupedit/groupedit_part_object_area_visible_set.c \
test_groupedit/groupedit_part_visible_set.c
groupedit_test_CFLAGS = ${FLAGS}
groupedit_test_LDADD = ${LD}

animator_test_SOURCES = \
test_animator/test_animator.c \
test_animator/animator_window_add_test.c
animator_test_CFLAGS = ${FLAGS}
animator_test_LDADD = ${LD}

live_view_test_SOURCES = \
test_live_view/test_live_view.c \
test_live_view/live_view_add.c \
test_live_view/live_view_free.c \
test_live_view/live_view_widget_style_set.c \
test_live_view/live_view_widget_style_unset.c \
test_live_view/live_view_theme_update.c
live_view_test_CFLAGS = ${FLAGS}
live_view_test_LDADD = ${LD}

live_view_prop_test_SOURCES = \
test_live_view_prop/test_live_view_prop.c \
test_live_view_prop/live_view_property_add.c \
test_live_view_prop/live_view_property_style_set.c \
test_live_view_prop/live_view_property_style_unset.c
live_view_prop_test_CFLAGS = ${FLAGS}
live_view_prop_test_LDADD = ${LD}

live_widget_test_SOURCES = \
test_live_widget/test_live_widget.c \
test_live_widget/live_widget_create.c \
test_live_widget/live_widget_del.c
live_widget_test_CFLAGS = ${FLAGS}
live_widget_test_LDADD = ${LD}

about_window_test_SOURCES = \
test_about_window/test_about_window.c \
test_about_window/about_window_add.c
about_window_test_CFLAGS = ${FLAGS}
about_window_test_LDADD = ${LD}

ui_state_dialog_test_SOURCES = \
test_ui_state_dialog/test_ui_state_dialog.c \
test_ui_state_dialog/state_dialog_state_add.c \
test_ui_state_dialog/state_dialog_state_del.c
ui_state_dialog_test_CFLAGS = ${FLAGS}
ui_state_dialog_test_LDADD = ${LD}

shortcuts_test_SOURCES = \
test_shortcuts/test_shortcuts.c \
test_shortcuts/shortcuts_init.c \
test_shortcuts/shortcuts_shutdown.c \
test_shortcuts/shortcuts_profile_load.c \
test_shortcuts/shortcuts_main_add.c \
test_shortcuts/shortcuts_main_del.c
shortcuts_test_CFLAGS = ${FLAGS}
shortcuts_test_LDADD = ${LD}

colorselector_test_SOURCES = \
test_colorselector/test_colorselector.c \
test_colorselector/colorselector_add.c \
test_colorselector/colorselector_color_get.c
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
