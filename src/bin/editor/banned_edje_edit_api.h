#ifndef BANNED_EDJE_EDIT_API_H
#define BANNED_EDJE_EDIT_API_H

#define EDJE_EDIT_IS_UNSTABLE_AND_I_KNOW_ABOUT_IT
#include <Edje_Edit.h>

#ifndef ALLOW_DIRECT_EDJE_EDIT_CALLS

/***********************************************************************************
 * NOTE: you should modify this file only by finding and uncommenting needed lines.
 * Adding new lines are allowed only for newly implemented API
 ***********************************************************************************/

/* General API */
#pragma GCC poison edje_edit_save
#pragma GCC poison edje_edit_save_all
#pragma GCC poison edje_edit_without_source_save

/* groups API */
//#pragma GCC poison edje_edit_group_add
//#pragma GCC poison edje_edit_group_copy
//#pragma GCC poison edje_edit_group_del
//#pragma GCC poison edje_edit_group_name_set
#pragma GCC poison edje_edit_group_min_w_set
#pragma GCC poison edje_edit_group_min_h_set
#pragma GCC poison edje_edit_group_max_w_set
#pragma GCC poison edje_edit_group_max_h_set

/* Alias API */
//#pragma GCC poison edje_edit_group_alias_add

/* Data API */
//#pragma GCC poison edje_edit_data_add
//#pragma GCC poison edje_edit_data_del
//#pragma GCC poison edje_edit_data_value_set
//#pragma GCC poison edje_edit_data_name_set
#pragma GCC poison edje_edit_group_data_add
#pragma GCC poison edje_edit_group_data_del
#pragma GCC poison edje_edit_group_data_value_set
#pragma GCC poison edje_edit_group_data_name_set

/* Color Classes API */
#pragma GCC poison edje_edit_color_class_add
#pragma GCC poison edje_edit_color_class_del
#pragma GCC poison edje_edit_color_class_colors_set
//#pragma GCC poison edje_edit_color_class_name_set
#pragma GCC poison edje_edit_color_class_description_set

/* Sound and Tone API */
#pragma GCC poison edje_edit_sound_sample_add
#pragma GCC poison edje_edit_sound_sample_del
#pragma GCC poison edje_edit_sound_tone_add
#pragma GCC poison edje_edit_sound_tone_del

/* Text styles API */
#pragma GCC poison edje_edit_style_add
#pragma GCC poison edje_edit_style_del
#pragma GCC poison edje_edit_style_tag_value_set
//#pragma GCC poison edje_edit_style_tag_name_set
#pragma GCC poison edje_edit_style_tag_add
#pragma GCC poison edje_edit_style_tag_del

/* Externals API */
//#pragma GCC poison edje_edit_external_add
//#pragma GCC poison edje_edit_external_del

/* Parts API */
//#pragma GCC poison edje_edit_part_add
//#pragma GCC poison edje_edit_part_external_add
//#pragma GCC poison edje_edit_part_del
//#pragma GCC poison edje_edit_part_restack_below
//#pragma GCC poison edje_edit_part_restack_part_below
//#pragma GCC poison edje_edit_part_restack_above
//#pragma GCC poison edje_edit_part_restack_part_above
#pragma GCC poison edje_edit_part_name_set
//#pragma GCC poison edje_edit_part_api_name_set
//#pragma GCC poison edje_edit_part_api_description_set
#pragma GCC poison edje_edit_part_clip_to_set
#pragma GCC poison edje_edit_part_source_set
#pragma GCC poison edje_edit_part_source2_set
#pragma GCC poison edje_edit_part_source3_set
#pragma GCC poison edje_edit_part_source4_set
#pragma GCC poison edje_edit_part_source5_set
#pragma GCC poison edje_edit_part_source6_set
#pragma GCC poison edje_edit_part_effect_set
//#pragma GCC poison edje_edit_part_selected_state_set
#pragma GCC poison edje_edit_part_mouse_events_set
#pragma GCC poison edje_edit_part_repeat_events_set
#pragma GCC poison edje_edit_part_multiline_set
#pragma GCC poison edje_edit_part_ignore_flags_set
#pragma GCC poison edje_edit_part_scale_set
#pragma GCC poison edje_edit_part_drag_x_set
#pragma GCC poison edje_edit_part_drag_y_set
#pragma GCC poison edje_edit_part_drag_step_x_set
#pragma GCC poison edje_edit_part_drag_step_y_set
//#pragma GCC poison edje_edit_part_drag_count_x_set
//#pragma GCC poison edje_edit_part_drag_count_y_set
#pragma GCC poison edje_edit_part_drag_confine_set
#pragma GCC poison edje_edit_part_drag_event_set
#pragma GCC poison edje_edit_part_drag_threshold_set

#pragma GCC poison edje_edit_part_item_move_below_index
#pragma GCC poison edje_edit_part_item_move_below_above
#pragma GCC poison edje_edit_part_item_index_append
#pragma GCC poison edje_edit_part_item_index_del
#pragma GCC poison edje_edit_part_item_index_source_set
#pragma GCC poison edje_edit_part_item_index_min_w_set
#pragma GCC poison edje_edit_part_item_index_min_h_set
#pragma GCC poison edje_edit_part_item_index_max_w_set
#pragma GCC poison edje_edit_part_item_index_max_h_set
#pragma GCC poison edje_edit_part_item_index_aspect_w_set
#pragma GCC poison edje_edit_part_item_index_aspect_h_set
#pragma GCC poison edje_edit_part_item_index_aspect_mode_set
#pragma GCC poison edje_edit_part_item_index_prefer_w_set
#pragma GCC poison edje_edit_part_item_index_prefer_h_set
#pragma GCC poison edje_edit_part_item_index_spread_w_set
#pragma GCC poison edje_edit_part_item_index_spread_h_set
#pragma GCC poison edje_edit_part_item_index_padding_set
#pragma GCC poison edje_edit_part_item_index_align_x_set
#pragma GCC poison edje_edit_part_item_index_align_y_set
#pragma GCC poison edje_edit_part_item_index_weight_x_set
#pragma GCC poison edje_edit_part_item_index_weight_y_set
#pragma GCC poison edje_edit_part_item_index_position_set
#pragma GCC poison edje_edit_part_item_index_position_col_set
#pragma GCC poison edje_edit_part_item_index_position_row_set
#pragma GCC poison edje_edit_part_item_index_span_set
#pragma GCC poison edje_edit_part_item_index_span_col_set
#pragma GCC poison edje_edit_part_item_index_span_row_set

/* OLD PART ITEM API WE SHOULDN'T USE */
#pragma GCC poison edje_edit_part_item_append
#pragma GCC poison edje_edit_part_item_del
#pragma GCC poison edje_edit_part_item_source_set
#pragma GCC poison edje_edit_part_item_min_w_set
#pragma GCC poison edje_edit_part_item_min_h_set
#pragma GCC poison edje_edit_part_item_max_w_set
#pragma GCC poison edje_edit_part_item_max_h_set
#pragma GCC poison edje_edit_part_item_aspect_w_set
#pragma GCC poison edje_edit_part_item_aspect_h_set
#pragma GCC poison edje_edit_part_item_aspect_mode_set
#pragma GCC poison edje_edit_part_item_prefer_w_set
#pragma GCC poison edje_edit_part_item_prefer_h_set
#pragma GCC poison edje_edit_part_item_spread_w_set
#pragma GCC poison edje_edit_part_item_spread_h_set
#pragma GCC poison edje_edit_part_item_padding_set
#pragma GCC poison edje_edit_part_item_align_x_set
#pragma GCC poison edje_edit_part_item_align_y_set
#pragma GCC poison edje_edit_part_item_weight_x_set
#pragma GCC poison edje_edit_part_item_weight_y_set
#pragma GCC poison edje_edit_part_item_position_set
#pragma GCC poison edje_edit_part_item_position_col_set
#pragma GCC poison edje_edit_part_item_position_row_set
#pragma GCC poison edje_edit_part_item_span_set
#pragma GCC poison edje_edit_part_item_span_col_set
#pragma GCC poison edje_edit_part_item_span_row_set
#pragma GCC poison edje_edit_part_select_mode_set
#pragma GCC poison edje_edit_part_entry_mode_set
#pragma GCC poison edje_edit_part_pointer_mode_set
#pragma GCC poison edje_edit_part_cursor_mode_set

/* States API */
//#pragma GCC poison edje_edit_state_name_set
//#pragma GCC poison edje_edit_state_add
//#pragma GCC poison edje_edit_state_del
//#pragma GCC poison edje_edit_state_copy
#pragma GCC poison edje_edit_state_rel1_relative_x_set
#pragma GCC poison edje_edit_state_rel1_relative_y_set
#pragma GCC poison edje_edit_state_rel2_relative_x_set
#pragma GCC poison edje_edit_state_rel2_relative_y_set
#pragma GCC poison edje_edit_state_rel1_offset_x_set
#pragma GCC poison edje_edit_state_rel1_offset_y_set
#pragma GCC poison edje_edit_state_rel2_offset_x_set
#pragma GCC poison edje_edit_state_rel2_offset_y_set
#pragma GCC poison edje_edit_state_rel1_to_x_set
#pragma GCC poison edje_edit_state_rel1_to_y_set
#pragma GCC poison edje_edit_state_rel2_to_x_set
#pragma GCC poison edje_edit_state_rel2_to_y_set
#pragma GCC poison edje_edit_state_color_set
#pragma GCC poison edje_edit_state_color2_set
#pragma GCC poison edje_edit_state_color3_set
#pragma GCC poison edje_edit_state_align_x_set
#pragma GCC poison edje_edit_state_align_y_set
#pragma GCC poison edje_edit_state_min_w_set
#pragma GCC poison edje_edit_state_min_h_set
#pragma GCC poison edje_edit_state_max_w_set
#pragma GCC poison edje_edit_state_max_h_set
#pragma GCC poison edje_edit_state_fixed_w_set
#pragma GCC poison edje_edit_state_fixed_h_set
#pragma GCC poison edje_edit_state_aspect_min_set
#pragma GCC poison edje_edit_state_aspect_max_set
#pragma GCC poison edje_edit_state_aspect_pref_set
#pragma GCC poison edje_edit_state_fill_smooth_set
#pragma GCC poison edje_edit_state_fill_origin_relative_x_set
#pragma GCC poison edje_edit_state_fill_origin_relative_y_set
#pragma GCC poison edje_edit_state_fill_origin_offset_x_set
#pragma GCC poison edje_edit_state_fill_origin_offset_y_set
#pragma GCC poison edje_edit_state_fill_size_relative_x_set
#pragma GCC poison edje_edit_state_fill_size_relative_y_set
#pragma GCC poison edje_edit_state_fill_size_offset_x_set
#pragma GCC poison edje_edit_state_fill_size_offset_y_set
#pragma GCC poison edje_edit_state_visible_set
#pragma GCC poison edje_edit_state_color_class_set
//#pragma GCC poison edje_edit_state_external_param_set
//#pragma GCC poison edje_edit_state_external_param_int_set
//#pragma GCC poison edje_edit_state_external_param_bool_set
//#pragma GCC poison edje_edit_state_external_param_double_set
//#pragma GCC poison edje_edit_state_external_param_string_set
//#pragma GCC poison edje_edit_state_external_param_choice_set
#pragma GCC poison edje_edit_state_minmul_w_set
#pragma GCC poison edje_edit_state_minmul_h_set
#pragma GCC poison edje_edit_state_container_align_x_set
#pragma GCC poison edje_edit_state_container_align_y_set
#pragma GCC poison edje_edit_state_container_padding_x_set
#pragma GCC poison edje_edit_state_container_padding_y_set
#pragma GCC poison edje_edit_state_container_min_v_set
#pragma GCC poison edje_edit_state_container_min_h_set
#pragma GCC poison edje_edit_state_proxy_source_set
#pragma GCC poison edje_edit_state_table_homogeneous_set
#pragma GCC poison edje_edit_state_fill_type_set

/* Text API */
#pragma GCC poison edje_edit_state_text_set
#pragma GCC poison edje_edit_state_font_set
#pragma GCC poison edje_edit_state_text_size_set
#pragma GCC poison edje_edit_state_text_align_x_set
#pragma GCC poison edje_edit_state_text_align_y_set
#pragma GCC poison edje_edit_state_text_elipsis_set
#pragma GCC poison edje_edit_state_text_fit_x_set
#pragma GCC poison edje_edit_state_text_fit_y_set
#pragma GCC poison edje_edit_state_text_min_x_set
#pragma GCC poison edje_edit_state_text_max_x_set
#pragma GCC poison edje_edit_state_text_min_y_set
#pragma GCC poison edje_edit_state_text_max_y_set
#pragma GCC poison edje_edit_state_text_style_set
#pragma GCC poison edje_edit_state_text_source_set
#pragma GCC poison edje_edit_state_text_text_source_set
//#pragma GCC poison edje_edit_font_add
//#pragma GCC poison edje_edit_font_del

/* Images API */
#pragma GCC poison edje_edit_image_add
#pragma GCC poison edje_edit_image_del
//#pragma GCC poison edje_edit_image_replace
//#pragma GCC poison edje_edit_image_data_add
#pragma GCC poison edje_edit_state_image_set
#pragma GCC poison edje_edit_state_image_border_set
#pragma GCC poison edje_edit_state_image_border_fill_set
//#pragma GCC poison edje_edit_state_tween_add
//#pragma GCC poison edje_edit_state_tween_del

/* Programs API */
#pragma GCC poison edje_edit_program_add
#pragma GCC poison edje_edit_program_del
#pragma GCC poison edje_edit_program_name_set
#pragma GCC poison edje_edit_program_source_set
#pragma GCC poison edje_edit_program_signal_set
#pragma GCC poison edje_edit_program_in_from_set
#pragma GCC poison edje_edit_program_in_range_set
#pragma GCC poison edje_edit_program_action_set
#pragma GCC poison edje_edit_program_target_add
#pragma GCC poison edje_edit_program_target_del
#pragma GCC poison edje_edit_program_targets_clear
#pragma GCC poison edje_edit_program_after_add
#pragma GCC poison edje_edit_program_after_del
#pragma GCC poison edje_edit_program_afters_clear
#pragma GCC poison edje_edit_program_api_name_set
#pragma GCC poison edje_edit_program_api_description_set
#pragma GCC poison edje_edit_program_state_set
#pragma GCC poison edje_edit_program_value_set
#pragma GCC poison edje_edit_program_state2_set
#pragma GCC poison edje_edit_program_value2_set
#pragma GCC poison edje_edit_program_transition_set
#pragma GCC poison edje_edit_program_transition_get
#pragma GCC poison edje_edit_program_transition_value1_set
#pragma GCC poison edje_edit_program_transition_value1_get
#pragma GCC poison edje_edit_program_transition_value2_set
#pragma GCC poison edje_edit_program_transition_value2_get
#pragma GCC poison edje_edit_program_transition_value3_set
#pragma GCC poison edje_edit_program_transition_value3_get
#pragma GCC poison edje_edit_program_transition_value4_set
#pragma GCC poison edje_edit_program_transition_value4_get
#pragma GCC poison edje_edit_program_transition_time_set
#pragma GCC poison edje_edit_program_filter_part_set
#pragma GCC poison edje_edit_program_channel_set

/* Scripts API */
#pragma GCC poison edje_edit_script_set
#pragma GCC poison edje_edit_script_program_set
//#pragma GCC poison edje_edit_script_compile

#endif /* ALLOW_DIRECT_EDJE_EDIT_CALLS */
#endif /* BANNED_EDJE_EDIT_API_H */
