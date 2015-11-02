/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#ifndef DEFAULT_H
#define DEFAULT_H

#include "eflete.h"
#include "editor.h"

Eina_Bool
editor_state_rel1_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_relative_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_relative_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_min_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_min_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_max_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_max_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_h_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_w_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_elipsis_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_elipsis_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_rel1_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_offset_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_offset_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_w_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_h_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_w_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_h_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_size_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_size_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_fixed_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fixed_h_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fixed_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fixed_w_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_smooth_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_smooth_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_visible_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_visible_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_h_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_v_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_v_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_rel1_to_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_to_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_to_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_to_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_x_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_y_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_proxy_source_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_proxy_source_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color_class_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color_class_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_source_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_source_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_text_source_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_text_source_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_font_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_font_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_style_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_style_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_color_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color2_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color2_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color3_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color3_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_image_border_fill_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_fill_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_type_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_type_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_pref_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_pref_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_table_homogeneous_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_table_homogeneous_reset(Evas_Object *edit_object, Change *change, const char *part_name, const char *state_name, double state_val);
#endif /* DEFAULT_H */
