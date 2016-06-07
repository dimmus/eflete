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

Eina_Bool
editor_state_reset(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apple,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_part_item_reset(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_reset(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
      const char *part_name);

Eina_Bool
editor_state_rel1_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_relative_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_relative_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_relative_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_align_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_min_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_min_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_max_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_max_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_relative_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_relative_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_align_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_minmul_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_align_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_elipsis_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_elipsis_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_rel1_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_offset_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_offset_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_offset_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_max_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_origin_offset_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_size_offset_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_padding_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_size_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_size_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_fixed_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fixed_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fixed_w_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fixed_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_smooth_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_smooth_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_visible_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_visible_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_h_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_v_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_container_min_v_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_fit_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_max_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_min_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_rel1_to_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_to_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_to_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel1_to_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_x_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_y_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_rel2_to_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_proxy_source_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_proxy_source_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color_class_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color_class_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_source_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_source_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_text_source_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_text_source_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_font_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_font_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_style_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_text_style_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_color_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_color_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_outline_color_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_outline_color_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_shadow_color_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_shadow_color_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_top_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_bottom_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_left_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_right_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);


Eina_Bool
editor_state_image_border_fill_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_image_border_fill_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_type_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_fill_type_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_pref_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_aspect_pref_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_table_homogeneous_default_is(Evas_Object *edit_object, const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_table_homogeneous_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *state_name, double state_val);


/* part items */
Eina_Bool
editor_part_item_aspect_mode_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_aspect_mode_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_aspect_h_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_aspect_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_aspect_w_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_aspect_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_max_h_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_max_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_max_w_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_max_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_min_h_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_min_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_min_w_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_min_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_prefer_h_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_prefer_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_prefer_w_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_prefer_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_spread_h_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_spread_h_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_spread_w_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_spread_w_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_span_col_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_span_col_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_span_row_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_span_row_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_position_col_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_position_col_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_position_row_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_position_row_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_align_x_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_align_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_align_y_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_align_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_weight_x_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_weight_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_weight_y_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_weight_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_padding_default_is(Evas_Object *edit_object, const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_padding_top_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_padding_bottom_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_padding_left_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_padding_right_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name, const char *item_name);

Eina_Bool
editor_part_text_effect_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_text_effect_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_text_shadow_direction_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_text_shadow_direction_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_ignore_flags_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_ignore_flags_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_mouse_events_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_mouse_events_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_repeat_events_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_repeat_events_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_scale_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_scale_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_multiline_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_multiline_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_count_x_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_count_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_count_y_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_count_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_x_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_y_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_step_x_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_step_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_step_y_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_step_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_select_mode_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_select_mode_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_entry_mode_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_entry_mode_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_cursor_mode_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_cursor_mode_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_clip_to_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_clip_to_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_confine_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_confine_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_threshold_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_threshold_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_drag_event_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_drag_event_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_group_source_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_group_source_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_textblock_selection_under_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_textblock_selection_under_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_textblock_selection_over_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_textblock_selection_over_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_textblock_cursor_under_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_textblock_cursor_under_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_textblock_cursor_over_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_textblock_cursor_over_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_textblock_anchors_under_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_textblock_anchors_under_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_textblock_anchors_over_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_textblock_anchors_over_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);
Eina_Bool
editor_part_pointer_mode_default_is(Evas_Object *edit_object, const char *part_name);
Eina_Bool
editor_part_pointer_mode_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *part_name);

Eina_Bool
editor_program_transition_type_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_from_current_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_channel_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_tone_duration_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_in_from_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_in_range_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_time_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_sample_speed_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_value_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_drag_value_x_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_drag_value_y_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_factor_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_gradient_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_decay_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_bounces_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_swings_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_bezier_x1_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_bezier_x2_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_bezier_y1_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_transition_bezier_y2_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_filter_part_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_filter_state_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_api_name_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_api_description_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_sample_name_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_tone_name_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_signal_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_source_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_state_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_emit_signal_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);
Eina_Bool
editor_program_emit_source_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
      const char *program_name);

#endif /* DEFAULT_H */
