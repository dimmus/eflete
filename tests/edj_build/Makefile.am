AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

AM_V_EDJ = $(am__v_EDJ)
am__v_EDJ = @echo "  EDJ   " $@;

EDJE_CC = @edje_cc@
EDJE_FLAGS = -id $(builddir) -fd $(builddir)

check: Makefile
	$(AM_V_EDJ)$(EDJE_CC) $(EDJE_FLAGS) \
	$(builddir)/radio.edc \
	$(builddir)/radio.edj
	for i in \
	$(builddir)/workspace_edit_object_part_state_del.edj \
	$(builddir)/workspace_background_image_set.edj \
	$(builddir)/workspace_highlight_unset.edj \
	$(builddir)/workspace_edit_object_part_state_add.edj \
	$(builddir)/workspace_edit_object_recalc.edj \
	$(builddir)/workspace_edit_object_get.edj \
	$(builddir)/workspace_highlight_set.edj \
	$(builddir)/workspace_edit_object_part_above.edj \
	$(builddir)/workspace_edit_object_set.edj \
	$(builddir)/workspace_edit_object_part_below.edj \
	$(builddir)/workspace_edit_object_unset.edj \
	$(builddir)/workspace_edit_object_part_add.edj \
	$(builddir)/workspace_edit_object_part_del.edj \
	$(builddir)/workspace_edit_object_part_state_set.edj \
	$(builddir)/workspace_edit_object_visible_set.edj \
	$(builddir)/groupedit_edit_object_unset.edj \
	$(builddir)/groupedit_edit_object_set.edj \
	$(builddir)/live_view_widget_style_set.edj \
	$(builddir)/live_view_widget_style_unset.edj \
	$(builddir)/live_view_theme_update.edj \
	$(builddir)/mw_icon_set.edj \
	$(builddir)/program_editor_window_add_test.edj \
	$(builddir)/style_editor_window_add.edj \
	$(builddir)/test_project_manager.edj \
	$(builddir)/compile.edj \
	$(builddir)/decompile.edj \
	$(builddir)/image_editor_window_add.edj \
	$(builddir)/image_editor_callback_add.edj \
	$(builddir)/image_editor_file_choose.edj \
	$(builddir)/ui_states_list_data_set.edj \
	$(builddir)/ui_states_list_part_get.edj \
	$(builddir)/ui_states_list_selected_state_get.edj \
	$(builddir)/ui_states_list_selected_states_del.edj \
	$(builddir)/ui_states_list_state_add.edj \
	$(builddir)/ui_signal_list_data_set.edj \
	$(builddir)/ui_signal_list_data_unset.edj \
	$(builddir)/wm_style_object_find.edj \
	$(builddir)/wm_widget_list_objects_load.edj \
	$(builddir)/wm_widget_list_new.edj \
	$(builddir)/wm_style_copy.edj \
	$(builddir)/wm_program_signals_list_free.edj \
	$(builddir)/wm_widget_list_layouts_load.edj \
	$(builddir)/wm_widget_list_free.edj \
	$(builddir)/wm_program_signals_list_get.edj \
	$(builddir)/wm_part_current_state_set.edj \
	$(builddir)/wm_layouts_list_objects_load.edj \
	$(builddir)/wm_style_free.edj \
	$(builddir)/wm_part_del.edj \
	$(builddir)/wm_part_add.edj \
	$(builddir)/ui_widget_list_data_set.edj \
	$(builddir)/ui_widget_list_data_reload.edj \
	$(builddir)/ui_widget_list_layout_reload.edj \
	$(builddir)/ui_widget_list_part_add.edj \
	$(builddir)/ui_widget_list_selected_part_above.edj \
	$(builddir)/ui_widget_list_selected_part_below.edj \
	$(builddir)/ui_widget_list_selected_part_del.edj \
	$(builddir)/ui_widget_list_selected_part_get.edj \
	$(builddir)/ui_widget_list_part_selected_set.edj \
	$(builddir)/ui_widget_list_title_set.edj \
	$(builddir)/ui_widget_list_selected_parts_get.edj \
	$(builddir)/part_dialog_add.edj \
	$(builddir)/pm_open_project_edj.edj \
	$(builddir)/pm_project_close.edj \
	$(builddir)/pm_save_project_edj.edj \
	$(builddir)/pm_save_project_to_swap.edj \
	$(builddir)/pm_export_to_edc.edj \
	$(builddir)/pm_save_as_project_edj.edj \
	$(builddir)/register_callbacks.edj \
	$(builddir)/ui_edj_load_done.edj \
	$(builddir)/save_as_edj_file.edj \
	$(builddir)/save_as_edc_file.edj \
	;do cp $(builddir)/radio.edj $$i; done
clean-local:
	rm -f *.edj
