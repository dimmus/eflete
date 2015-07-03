MAINTAINERCLEANFILES = Makefile.in

ACLOCAL_AMFLAGS = -I m4

TESTS = \
ewe_init_test \
ewe_tabs_test \
ewe_combobox_test \
ewe_entry_test \
ewe_ruler_test \
ewe_statusbar_test \
eflete_test \
part_dialog_test \
alloc_test \
colorclass_manager_test \
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
history_test \
colorselector_test \
shortcuts_test \
ui_item_dialog_test


check_PROGRAMS = ${TESTS}

FLAGS = \
-I${top_srcdir}/src/lib \
-I${top_srcdir}/src/bin \
-I${top_srcdir}/src/bin/alloc \
-I${top_srcdir}/src/bin/external \
-I${top_srcdir}/src/bin/common \
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
-I${top_srcdir}/src/bin/ui/signal_list \
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
utc_common.h \
test_ewe_init/test_ewe_init.c \
test_ewe_init/test_ewe_init.h \
test_ewe_init/ewe_init.c
ewe_init_test_CFLAGS = ${FLAGS}
ewe_init_test_LDADD = ${LD}

ewe_entry_test_SOURCES = \
utc_common.h \
test_ewe_entry/test_ewe_entry.c \
test_ewe_entry/test_ewe_entry.h \
test_ewe_entry/ewe_entry_add.c \
test_ewe_entry/ewe_entry_regex_check.c \
test_ewe_entry/ewe_entry_regex_set.c \
test_ewe_entry/ewe_entry_regex_unset.c \
test_ewe_entry/ewe_entry_regex_glow.c \
test_ewe_entry/ewe_entry_regex_error_get.c \
test_ewe_entry/ewe_entry_regex_autocheck.c
ewe_entry_test_CFLAGS = ${FLAGS}
ewe_entry_test_LDADD = ${LD}

ewe_ruler_test_SOURCES = \
utc_common.h \
test_ewe_ruler/test_ewe_ruler.c \
test_ewe_ruler/test_ewe_ruler.h \
test_ewe_ruler/ewe_ruler_add.c \
test_ewe_ruler/ewe_ruler_scale_add.c \
test_ewe_ruler/ewe_ruler_scale_del.c \
test_ewe_ruler/ewe_ruler_scale_visible_get.c \
test_ewe_ruler/ewe_ruler_scale_visible_set.c \
test_ewe_ruler/ewe_ruler_scale_middle_mark_get.c \
test_ewe_ruler/ewe_ruler_scale_middle_mark_set.c \
test_ewe_ruler/ewe_ruler_horizontal_get.c \
test_ewe_ruler/ewe_ruler_horizontal_set.c \
test_ewe_ruler/ewe_ruler_step_set.c \
test_ewe_ruler/ewe_ruler_step_get.c \
test_ewe_ruler/ewe_ruler_value_step_set.c \
test_ewe_ruler/ewe_ruler_value_step_get.c \
test_ewe_ruler/ewe_ruler_zero_offset_set.c \
test_ewe_ruler/ewe_ruler_zero_offset_get.c \
test_ewe_ruler/ewe_ruler_format_get.c \
test_ewe_ruler/ewe_ruler_format_set.c \
test_ewe_ruler/ewe_ruler_style_get.c \
test_ewe_ruler/ewe_ruler_style_set.c \
test_ewe_ruler/ewe_ruler_marker_style_get.c \
test_ewe_ruler/ewe_ruler_marker_style_set.c \
test_ewe_ruler/ewe_ruler_marker_absolute_get.c \
test_ewe_ruler/ewe_ruler_marker_absolute_set.c \
test_ewe_ruler/ewe_ruler_marker_relative_get.c \
test_ewe_ruler/ewe_ruler_marker_relative_set.c \
test_ewe_ruler/ewe_ruler_marker_visible_get.c \
test_ewe_ruler/ewe_ruler_marker_visible_set.c \
test_ewe_ruler/ewe_ruler_marker_add.c \
test_ewe_ruler/ewe_ruler_marker_del.c

ewe_ruler_test_CFLAGS = ${FLAGS}
ewe_ruler_test_LDADD = ${LD}

ewe_combobox_test_SOURCES = \
utc_common.h \
test_ewe_combobox/test_ewe_combobox.c \
test_ewe_combobox/test_ewe_combobox.h \
test_ewe_combobox/ewe_combobox_add.c \
test_ewe_combobox/ewe_combobox_item_add.c \
test_ewe_combobox/ewe_combobox_item_del.c \
test_ewe_combobox/ewe_combobox_item_title_get.c \
test_ewe_combobox/ewe_combobox_item_title_set.c \
test_ewe_combobox/ewe_combobox_items_list_get.c \
test_ewe_combobox/ewe_combobox_select_item_get.c \
test_ewe_combobox/ewe_combobox_select_item_set.c
ewe_combobox_test_CFLAGS = ${FLAGS}
ewe_combobox_test_LDADD = ${LD}

ewe_tabs_test_SOURCES = \
utc_common.h \
test_ewe_tabs/test_ewe_tabs.c \
test_ewe_tabs/test_ewe_tabs.h \
test_ewe_tabs/ewe_tabs_item_append.c \
test_ewe_tabs/ewe_tabs_item_prepend.c \
test_ewe_tabs/ewe_tabs_items_list_get.c \
test_ewe_tabs/ewe_tabs_add.c \
test_ewe_tabs/ewe_tabs_item_content_set.c \
test_ewe_tabs/ewe_tabs_item_content_unset.c \
test_ewe_tabs/ewe_tabs_item_content_get.c \
test_ewe_tabs/ewe_tabs_item_icon_set.c \
test_ewe_tabs/ewe_tabs_item_icon_unset.c \
test_ewe_tabs/ewe_tabs_item_button_set.c \
test_ewe_tabs/ewe_tabs_item_button_unset.c \
test_ewe_tabs/ewe_tabs_active_item_set.c \
test_ewe_tabs/ewe_tabs_active_item_get.c \
test_ewe_tabs/ewe_tabs_item_title_set.c \
test_ewe_tabs/ewe_tabs_item_title_get.c \
test_ewe_tabs/ewe_tabs_item_del.c \
test_ewe_tabs/ewe_tabs_item_disabled_set.c \
test_ewe_tabs/ewe_tabs_item_disabled_get.c \
test_ewe_tabs/ewe_tabs_orient_horizontal_set.c \
test_ewe_tabs/ewe_tabs_orient_horizontal_get.c \
test_ewe_tabs/ewe_tabs_elm_widget_style_set.c \
test_ewe_tabs/ewe_tabs_elm_widget_style_get.c
ewe_tabs_test_CFLAGS = ${FLAGS}
ewe_tabs_test_LDADD = ${LD}

ewe_statusbar_test_SOURCES = \
utc_common.h \
test_ewe_statusbar/test_ewe_statusbar.c \
test_ewe_statusbar/test_ewe_statusbar.h \
test_ewe_statusbar/ewe_statusbar_add.c \
test_ewe_statusbar/ewe_statusbar_clear.c \
test_ewe_statusbar/ewe_statusbar_item_append.c \
test_ewe_statusbar/ewe_statusbar_item_content_get.c \
test_ewe_statusbar/ewe_statusbar_item_content_set.c \
test_ewe_statusbar/ewe_statusbar_item_content_unset.c \
test_ewe_statusbar/ewe_statusbar_item_id_get.c \
test_ewe_statusbar/ewe_statusbar_item_insert_after.c \
test_ewe_statusbar/ewe_statusbar_item_insert_before.c \
test_ewe_statusbar/ewe_statusbar_item_label_get.c \
test_ewe_statusbar/ewe_statusbar_item_label_set.c \
test_ewe_statusbar/ewe_statusbar_item_prepend.c \
test_ewe_statusbar/ewe_statusbar_item_remove.c \
test_ewe_statusbar/ewe_statusbar_item_statusbar_get.c \
test_ewe_statusbar/ewe_statusbar_item_width_get.c \
test_ewe_statusbar/ewe_statusbar_item_width_set.c \
test_ewe_statusbar/ewe_statusbar_items_align_get.c \
test_ewe_statusbar/ewe_statusbar_items_align_set.c \
test_ewe_statusbar/ewe_statusbar_items_list_get.c \
test_ewe_statusbar/ewe_statusbar_items_padding_get.c \
test_ewe_statusbar/ewe_statusbar_items_padding_set.c \
test_ewe_statusbar/ewe_statusbar_items_swap.c
ewe_statusbar_test_CFLAGS = ${FLAGS}
ewe_statusbar_test_LDADD = ${LD}




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

alloc_test_SOURCES = \
test_alloc/test_alloc.c \
test_alloc/mem_malloc.c \
test_alloc/mem_calloc.c
alloc_test_CFLAGS = ${FLAGS}
alloc_test_LDADD = ${LD}

colorclass_manager_test_SOURCES = \
test_colorclass_manager/test_colorclass_manager.c \
test_colorclass_manager/colorclass_manager_add.c
colorclass_manager_test_CFLAGS = ${FLAGS}
colorclass_manager_test_LDADD = ${LD}

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
test_widget_manager/wm_style_data_reload.c \
test_widget_manager/wm_style_object_find.c \
test_widget_manager/wm_styles_build_alias.c \
test_widget_manager/wm_widget_add.c \
test_widget_manager/wm_widget_free.c \
test_widget_manager/wm_widget_list_free.c \
test_widget_manager/wm_widget_list_new.c \
test_widget_manager/wm_layouts_list_new.c \
test_widget_manager/wm_layouts_list_objects_load.c \
test_widget_manager/wm_widget_list_objects_load.c \
test_widget_manager/wm_part_by_name_find.c
widget_manager_test_CFLAGS = ${FLAGS}
widget_manager_test_LDADD = ${LD}

sound_editor_test_SOURCES = \
test_sound_editor/test_sound_editor.c \
test_sound_editor/sound_editor_window_add.c \
test_sound_editor/sound_editor_file_choose.c \
test_sound_editor/sound_editor_added_sounds_free.c

sound_editor_test_CFLAGS = ${FLAGS}
sound_editor_test_LDADD = ${LD}

image_editor_test_SOURCES = \
test_image_editor/test_image_editor.c \
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
test_project_manager/pm_project_import_edj.c \
test_project_manager/pm_project_import_edc.c \
test_project_manager/pm_project_thread_cancel.c \
test_project_manager/pm_project_thread_free.c \
test_project_manager/pm_project_open.c \
test_project_manager/pm_project_save.c \
test_project_manager/pm_project_close.c \
test_project_manager/pm_project_meta_data_set.c \
test_project_manager/pm_project_meta_data_get.c \
test_project_manager/pm_style_resource_export.c
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
test_ui_block/ui_block_history.c \
test_ui_block/ui_block_live_view.c \
test_ui_block/ui_block_property.c \
test_ui_block/ui_block_signal_list.c \
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
test_ui_main_window/register_callbacks.c \
test_ui_main_window/ui_main_window_add.c \
test_ui_main_window/ui_main_window_del.c \
test_ui_main_window/ui_menu_add.c \
test_ui_main_window/ui_menu_disable_set.c \
test_ui_main_window/ui_menu_items_list_disable_set.c \
test_ui_main_window/ui_panes_add.c \
test_ui_main_window/ui_panes_hide.c \
test_ui_main_window/ui_panes_show.c \
test_ui_main_window/ui_panes_left_panes_min_size_toggle.c \
test_ui_main_window/ui_part_back.c \
test_ui_main_window/ui_part_select.c \
test_ui_main_window/ui_state_select.c \
test_ui_main_window/ui_style_back.c \
test_ui_main_window/ui_style_delete.c \
test_ui_main_window/add_callbacks_wd.c \
test_ui_main_window/code_edit_mode_switch.c
ui_main_window_test_CFLAGS = ${FLAGS}
ui_main_window_test_LDADD = ${LD}

#preference_test_SOURCES = \
#test_preference/test_preference.c \
#test_preference/preference_add.c \
#test_preference/preferences_project_autosave_update.c
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
test_ui_widget_list/ui_widget_list_title_set.c \
test_ui_widget_list/ui_widget_list_tab_activate.c
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
test_ui_workspace/workspace_object_area_visible_set.c \
test_ui_workspace/workspace_edit_object_part_item_selected_set.c \
test_ui_workspace/workspace_edit_object_part_item_add.c \
test_ui_workspace/workspace_edit_object_part_item_del.c
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
test_live_view_prop/live_view_property_free.c \
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

history_test_SOURCES = \
test_history/test_history.c \
test_history/history_init.c \
test_history/history_term.c \
test_history/history_clear.c \
test_history/history_module_del.c \
test_history/history_module_add.c \
test_history/history_diff_add.c \
test_history/history_undo.c \
test_history/history_redo.c \
test_history/history_genlist_get.c \
test_history/history_module_depth_set.c \
test_history/history_module_depth_get.c
history_test_CFLAGS = ${FLAGS}
history_test_LDADD = ${LD}

ui_item_dialog_test_SOURCES = \
test_ui_item_dialog/test_ui_item_dialog.c \
test_ui_item_dialog/item_dialog_add.c
ui_item_dialog_test_CFLAGS = ${FLAGS}
ui_item_dialog_test_LDADD = ${LD}

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
