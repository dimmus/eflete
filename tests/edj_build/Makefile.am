AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

AM_V_EDJ = $(am__v_EDJ)
am__v_EDJ = @echo "  EDJ   " $@;

EDJE_CC = @edje_cc@
EDJE_FLAGS = -id $(builddir) -fd $(builddir) -l license.txt

check: Makefile
	$(AM_V_EDJ)$(EDJE_CC) $(EDJE_FLAGS) \
	$(builddir)/radio.edc \
	$(builddir)/radio.edj
	for i in \
	$(builddir)/ui_workspace.edj \
	$(builddir)/workspace_edit_object_part_state_del.edj \
	$(builddir)/workspace_background_image_set.edj \
	$(builddir)/workspace_highlight_unset.edj \
	$(builddir)/workspace_edit_object_part_state_add.edj \
	$(builddir)/workspace_edit_object_recalc.edj \
	$(builddir)/workspace_edit_object_get.edj \
	$(builddir)/workspace_highlight_set.edj \
	$(builddir)/workspace_separate_mode_set.edj \
	$(builddir)/workspace_separate_mode_get.edj \
	$(builddir)/workspace_edit_object_part_rename.edj \
	$(builddir)/workspace_edit_object_part_above.edj \
	$(builddir)/workspace_edit_object_part_restack.edj \
	$(builddir)/workspace_edit_object_set.edj \
	$(builddir)/workspace_edit_object_part_below.edj \
	$(builddir)/workspace_edit_object_unset.edj \
	$(builddir)/workspace_edit_object_part_add.edj \
	$(builddir)/workspace_edit_object_part_del.edj \
	$(builddir)/workspace_edit_object_part_state_set.edj \
	$(builddir)/workspace_edit_object_visible_set.edj \
	$(builddir)/workspace_edit_object_part_item_selected_set.edj \
	$(builddir)/workspace_edit_object_part_item_add.edj \
	$(builddir)/workspace_edit_object_part_item_del.edj \
	$(builddir)/workspace_highlight_align_visible_get.edj \
	$(builddir)/workspace_highlight_align_visible_set.edj \
	$(builddir)/workspace_object_area_visible_get.edj \
	$(builddir)/workspace_object_area_visible_set.edj \
	$(builddir)/workspace_zoom_factor_get.edj \
	$(builddir)/groupedit_edit_object_unset.edj \
	$(builddir)/groupedit_edit_object_set.edj \
	$(builddir)/mw_icon_set.edj \
	$(builddir)/program_editor_window_add_test.edj \
	$(builddir)/style_editor.edj \
	$(builddir)/test_project_manager.edj \
	$(builddir)/image_manager.edj \
	$(builddir)/ui_states_list_data_set.edj \
	$(builddir)/ui_states_list_data_unset.edj \
	$(builddir)/ui_states_list_part_get.edj \
	$(builddir)/ui_states_list_selected_state_get.edj \
	$(builddir)/ui_states_list_selected_states_del.edj \
	$(builddir)/ui_states_list_state_add.edj \
	$(builddir)/ui_signal_list_data_set.edj \
	$(builddir)/ui_signal_list_data_unset.edj \
	$(builddir)/wm_style_object_find.edj \
	$(builddir)/wm_styles_build_alias.edj \
	$(builddir)/wm_widget_list_objects_load.edj \
	$(builddir)/wm_widget_list_new.edj \
	$(builddir)/wm_style_copy.edj \
	$(builddir)/wm_program_signals_list_free.edj \
	$(builddir)/wm_layouts_list_new.edj \
	$(builddir)/wm_widget_list_free.edj \
	$(builddir)/wm_program_signals_list_get.edj \
	$(builddir)/wm_part_current_state_set.edj \
	$(builddir)/wm_layouts_list_objects_load.edj \
	$(builddir)/wm_style_free.edj \
	$(builddir)/wm_style_data_reload.edj \
	$(builddir)/wm_part_del.edj \
	$(builddir)/wm_part_add.edj \
	$(builddir)/widget_list.edj \
	$(builddir)/wm_part_by_name_find.edj \
	$(builddir)/ui_widget_list_part_add.edj \
	$(builddir)/ui_widget_list_title_set.edj \
	$(builddir)/part_dialog.edj \
	$(builddir)/main_window.edj \
	$(builddir)/ui_edj_load.edj \
	$(builddir)/ui_style_clicked.edj \
	$(builddir)/style_dialog.edj \
	$(builddir)/state_dialog.edj \
	$(builddir)/item_dialog.edj \
	$(builddir)/colorclass_manager.edj \
	$(builddir)/sound_editor.edj \
	$(builddir)/animator.edj \
	$(builddir)/history.edj \
	$(builddir)/history_diff_add.edj \
	$(builddir)/preference.edj \
	$(builddir)/project_manager.edj \
	$(builddir)/project_thread_cancel.edj \
	;do cp $(builddir)/radio.edj $$i; done
clean-local:
	rm -f *.edj
