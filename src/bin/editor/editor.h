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

#ifndef EDITOR_H
#define EDITOR_H

#include "eflete.h"
#include "banned_edje_edit_api.h"
#include "default.h"

typedef enum {
   ATTRIBUTE_GROUP_MIN_W,
   ATTRIBUTE_GROUP_MIN_H,
   ATTRIBUTE_GROUP_MAX_W,
   ATTRIBUTE_GROUP_MAX_H,
   ATTRIBUTE_STATE_MIN_W,
   ATTRIBUTE_STATE_MIN_H,
   ATTRIBUTE_STATE_MAX_W,
   ATTRIBUTE_STATE_MAX_H,
   ATTRIBUTE_STATE_ALIGN_X,
   ATTRIBUTE_STATE_ALIGN_Y,
   ATTRIBUTE_STATE_REL1_RELATIVE_X,
   ATTRIBUTE_STATE_REL1_RELATIVE_Y,
   ATTRIBUTE_STATE_REL2_RELATIVE_X,
   ATTRIBUTE_STATE_REL2_RELATIVE_Y,
   ATTRIBUTE_STATE_REL1_OFFSET_X,
   ATTRIBUTE_STATE_REL1_OFFSET_Y,
   ATTRIBUTE_STATE_REL2_OFFSET_X,
   ATTRIBUTE_STATE_REL2_OFFSET_Y,
   ATTRIBUTE_STATE_ASPECT_MIN,
   ATTRIBUTE_STATE_ASPECT_MAX,
   ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_X,
   ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_Y,
   ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_X,
   ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_Y,
   ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_X,
   ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_Y,
   ATTRIBUTE_STATE_FILL_SIZE_OFFSET_X,
   ATTRIBUTE_STATE_FILL_SIZE_OFFSET_Y,
   ATTRIBUTE_STATE_TEXT_ALIGN_X,
   ATTRIBUTE_STATE_TEXT_ALIGN_Y,
   ATTRIBUTE_STATE_TEXT_ELIPSIS,
   ATTRIBUTE_STATE_TEXT_SIZE,
   ATTRIBUTE_STATE_TEXT_FIT_X,
   ATTRIBUTE_STATE_TEXT_FIT_Y,
   ATTRIBUTE_STATE_TEXT_MAX_X,
   ATTRIBUTE_STATE_TEXT_MAX_Y,
   ATTRIBUTE_STATE_TEXT_MIN_X,
   ATTRIBUTE_STATE_TEXT_MIN_Y,
   ATTRIBUTE_STATE_FIXED_H,
   ATTRIBUTE_STATE_FIXED_W,
   ATTRIBUTE_STATE_FILL_SMOOTH,
   ATTRIBUTE_STATE_VISIBLE,
   ATTRIBUTE_STATE_IMAGE,
   ATTRIBUTE_STATE_IMAGE_TWEEN,
   ATTRIBUTE_STATE_COLOR_CLASS,
   ATTRIBUTE_STATE_REL1_TO_X,
   ATTRIBUTE_STATE_REL1_TO_Y,
   ATTRIBUTE_STATE_REL2_TO_X,
   ATTRIBUTE_STATE_REL2_TO_Y,
   ATTRIBUTE_STATE_TEXT_SOURCE,
   ATTRIBUTE_STATE_TEXT_TEXT_SOURCE,
   ATTRIBUTE_STATE_TEXT,
   ATTRIBUTE_STATE_FONT,
   ATTRIBUTE_STATE_TEXT_STYLE,
   ATTRIBUTE_STATE_COLOR,
   ATTRIBUTE_STATE_COLOR2,
   ATTRIBUTE_STATE_COLOR3,
   ATTRIBUTE_STATE_IMAGE_BORDER,
   ATTRIBUTE_STATE_IMAGE_BORDER_FILL,
   ATTRIBUTE_STATE_ASPECT_PREF,
   ATTRIBUTE_PART_EFFECT,
   ATTRIBUTE_PART_IGNORE_FLAGS,
   ATTRIBUTE_PART_MOUSE_EVENTS,
   ATTRIBUTE_PART_REPEAT_EVENTS,
   ATTRIBUTE_PART_SCALE,
   ATTRIBUTE_PART_DRAG_COUNT_X,
   ATTRIBUTE_PART_DRAG_COUNT_Y,
   ATTRIBUTE_PART_DRAG_X,
   ATTRIBUTE_PART_DRAG_Y,
   ATTRIBUTE_PART_DRAG_STEP_X,
   ATTRIBUTE_PART_DRAG_STEP_Y,
   ATTRIBUTE_PART_CLIP_TO,
   ATTRIBUTE_PART_DRAG_CONFINE,
   ATTRIBUTE_PART_DRAG_THRESHOLD,
   ATTRIBUTE_PART_DRAG_EVENT,
   ATTRIBUTE_PART_NAME,
   ATTRIBUTE_PART_SOURCE,
   ATTRIBUTE_PART_SOURCE2,
   ATTRIBUTE_PART_SOURCE3,
   ATTRIBUTE_PART_SOURCE4,
   ATTRIBUTE_PART_SOURCE5,
   ATTRIBUTE_PART_SOURCE6,
   ATTRIBUTE_GROUP_NAME,
   ATTRIBUTE_PART_ITEM_ASPECT_MODE,
   ATTRIBUTE_PART_ITEM_ALIGN_X,
   ATTRIBUTE_PART_ITEM_ALIGN_Y,
   ATTRIBUTE_PART_ITEM_WEIGHT_X,
   ATTRIBUTE_PART_ITEM_WEIGHT_Y,
   ATTRIBUTE_PART_ITEM_ASPECT_H,
   ATTRIBUTE_PART_ITEM_ASPECT_W,
   ATTRIBUTE_PART_ITEM_MAX_H,
   ATTRIBUTE_PART_ITEM_MAX_W,
   ATTRIBUTE_PART_ITEM_MIN_H,
   ATTRIBUTE_PART_ITEM_MIN_W,
   ATTRIBUTE_PART_ITEM_PREFER_H,
   ATTRIBUTE_PART_ITEM_PREFER_W,
   ATTRIBUTE_PART_ITEM_SPREAD_H,
   ATTRIBUTE_PART_ITEM_SPREAD_W,
   ATTRIBUTE_PART_ITEM_SPAN_COL,
   ATTRIBUTE_PART_ITEM_SPAN_ROW,
   ATTRIBUTE_PART_ITEM_POSITION_COL,
   ATTRIBUTE_PART_ITEM_POSITION_ROW,
   ATTRIBUTE_PART_ITEM_SOURCE,
   ATTRIBUTE_PART_ITEM_PADDING,
   ATTRIBUTE_STATE_CONTAINER_ALIGN_X,
   ATTRIBUTE_STATE_CONTAINER_ALIGN_Y,
   ATTRIBUTE_STATE_CONTAINER_MIN_H,
   ATTRIBUTE_STATE_CONTAINER_MIN_V,
   ATTRIBUTE_STATE_TABLE_HOMOGENEOUS,
   ATTRIBUTE_STATE_CONTAINER_PADING_X,
   ATTRIBUTE_STATE_CONTAINER_PADING_Y,
   ATTRIBUTE_STATE_MINMUL_H,
   ATTRIBUTE_STATE_MINMUL_W,
   ATTRIBUTE_PART_MULTILINE,
   ATTRIBUTE_STATE_PROXY_SOURCE,
   ATTRIBUTE_PART_SELECT_MODE,
   ATTRIBUTE_PART_ENTRY_MODE,
   ATTRIBUTE_PART_POINTER_MODE,
   ATTRIBUTE_PART_CURSOR_MODE,
   ATTRIBUTE_STATE_FILL_TYPE,
   ATTRIBUTE_PROGRAM_TRANSITION_TYPE,
   ATTRIBUTE_PROGRAM_TRANSITION_FROM_CURRENT,
   ATTRIBUTE_PROGRAM_ACTION,
   ATTRIBUTE_PROGRAM_CHANNEL,
   ATTRIBUTE_PROGRAM_TONE_DURATION,
   ATTRIBUTE_PROGRAM_IN_FROM,
   ATTRIBUTE_PROGRAM_IN_RANGE,
   ATTRIBUTE_PROGRAM_TRANSITION_TIME,
   ATTRIBUTE_PROGRAM_SAMPLE_SPEED,
   ATTRIBUTE_PROGRAM_VALUE2,
   ATTRIBUTE_PROGRAM_VALUE,
   ATTRIBUTE_PROGRAM_TRANSITION_VALUE1,
   ATTRIBUTE_PROGRAM_TRANSITION_VALUE2,
   ATTRIBUTE_PROGRAM_TRANSITION_VALUE3,
   ATTRIBUTE_PROGRAM_TRANSITION_VALUE4,
   ATTRIBUTE_PROGRAM_FILTER_PART,
   ATTRIBUTE_PROGRAM_FILTER_STATE,
   ATTRIBUTE_PROGRAM_API_NAME,
   ATTRIBUTE_PROGRAM_API_DESCRIPTION,
   ATTRIBUTE_PROGRAM_SAMPLE_NAME,
   ATTRIBUTE_PROGRAM_TONE_NAME,
   ATTRIBUTE_PROGRAM_SIGNAL,
   ATTRIBUTE_PROGRAM_SOURCE,
   ATTRIBUTE_PROGRAM_STATE,
   ATTRIBUTE_PROGRAM_STATE2,
} Attribute;

void
_editor_project_changed();

/* General */
Eina_Bool
editor_save(Evas_Object *edit_object);

Eina_Bool
editor_save_all(Evas_Object *edit_object);

Eina_Bool
editor_internal_group_add(Evas_Object *edit_object);

Eina_Bool
editor_group_add(Evas_Object *obj, const char *name);

Eina_Bool
editor_group_copy(Evas_Object *obj, const char *group_src, const char *group_dest);

Eina_Bool
editor_group_alias_add(Evas_Object *obj, const char *group_src, const char *group_alias);

Eina_Bool
editor_group_del(Evas_Object *obj, const char *name);

Eina_Bool
you_shall_not_pass_editor_signals(Change *change);

Eina_Bool
you_shall_pass_editor_signals(Change *change);

/* Group */
Eina_Bool
editor_group_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

Eina_Bool
editor_group_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

Eina_Bool
editor_group_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

Eina_Bool
editor_group_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

/* State */

Eina_Bool
editor_state_add(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                 const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_copy(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                  const char *part_name, const char *from_name, double from_val,
                                         const char *state_name, double state_val);
Eina_Bool
editor_state_del(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                 const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_state_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_aspect_min_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_aspect_max_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_minmul_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_minmul_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_rel1_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_rel1_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_rel1_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_rel1_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_rel2_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_rel2_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_rel2_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_rel2_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_text_size_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_text_elipsis_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_text_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_text_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_fill_origin_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_fill_origin_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_fill_origin_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_fill_origin_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_fill_size_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_fill_size_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_fill_size_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_fill_size_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_container_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_container_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      double new_val);
Eina_Bool
editor_state_container_padding_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_container_padding_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val);
Eina_Bool
editor_state_visible_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_fixed_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_fixed_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_text_min_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_text_min_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_text_max_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_text_max_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_text_fit_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_text_fit_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_fill_smooth_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_container_min_v_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);
Eina_Bool
editor_state_container_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val);

Eina_Bool
editor_state_proxy_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_rel1_to_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_rel1_to_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_rel2_to_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_rel2_to_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_text_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_text_text_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_text_style_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);

Eina_Bool
editor_state_aspect_pref_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val);
Eina_Bool
editor_state_table_homogeneous_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val);
Eina_Bool
editor_state_image_border_fill_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val);
Eina_Bool
editor_state_image_border_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int new_val1, int new_val2, int new_val3, int new_val4);
Eina_Bool
editor_state_fill_type_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val);
Eina_Bool
editor_state_color_class_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);

Eina_Bool
editor_state_color_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int r, int g, int b, int a);

Eina_Bool
editor_state_color2_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int r, int g, int b, int a);

Eina_Bool
editor_state_color3_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      int r, int g, int b, int a);

Eina_Bool
editor_state_text_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_font_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);
Eina_Bool
editor_state_image_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *state_name, double state_val,
      const char * new_val);


/* Part */

Eina_Bool
editor_part_selected_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                               const char *part_name, const char *state_name, double state_val);
Eina_Bool
editor_part_add(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                const char *part_name, Edje_Part_Type type);
Eina_Bool
editor_part_del(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                const char *part_name);
Eina_Bool
editor_part_restack(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                    const char *part_name, const char *relative_part);
Eina_Bool
editor_part_item_restack(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                         const char *part_name, const char *part_item, const char *relative_part_item);
Eina_Bool
editor_part_item_append(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                        const char *part_name, const char *item_name, const char *source_group);
Eina_Bool
editor_part_item_del(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                     const char *part_name, const char *item_name);
Eina_Bool
editor_part_item_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_prefer_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_prefer_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      double new_val);
Eina_Bool
editor_part_item_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      double new_val);
Eina_Bool
editor_part_item_weight_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      double new_val);
Eina_Bool
editor_part_item_weight_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      double new_val);
Eina_Bool
editor_part_item_aspect_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_aspect_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_spread_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_spread_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val);
Eina_Bool
editor_part_item_span_col_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      unsigned short new_val);
Eina_Bool
editor_part_item_span_row_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      unsigned short new_val);
Eina_Bool
editor_part_item_position_col_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      unsigned short new_val);
Eina_Bool
editor_part_item_position_row_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      unsigned short new_val);
Eina_Bool
editor_part_item_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      const char * new_val);
Eina_Bool
editor_part_item_aspect_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      Edje_Aspect_Control new_val);
Eina_Bool
editor_part_item_padding_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name, const char *item_name,
      int new_val1, int new_val2, int new_val3, int new_val4);
Eina_Bool
editor_part_scale_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Eina_Bool new_val);
Eina_Bool
editor_part_mouse_events_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Eina_Bool new_val);
Eina_Bool
editor_part_repeat_events_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Eina_Bool new_val);
Eina_Bool
editor_part_multiline_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Eina_Bool new_val);
Eina_Bool
editor_part_ignore_flags_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Evas_Event_Flags new_val);
Eina_Bool
editor_part_select_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Edje_Edit_Select_Mode new_val);
Eina_Bool
editor_part_entry_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Edje_Edit_Entry_Mode new_val);
Eina_Bool
editor_part_pointer_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Evas_Object_Pointer_Mode new_val);
Eina_Bool
editor_part_cursor_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      unsigned char new_val);

Eina_Bool
editor_part_drag_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      int new_val);
Eina_Bool
editor_part_drag_step_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      int new_val);
Eina_Bool
editor_part_drag_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      int new_val);
Eina_Bool
editor_part_drag_step_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      int new_val);
Eina_Bool
editor_part_drag_count_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      int new_val);
Eina_Bool
editor_part_drag_count_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      int new_val);
Eina_Bool
editor_part_name_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char *new_val);

Eina_Bool
editor_state_tween_del(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                       const char *part_name, const char *state_name, double state_val,
                       Eina_Stringshare *name);
Eina_Bool
editor_state_tween_add(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                       const char *part_name, const char *state_name, double state_val,
                       Eina_Stringshare *name);

Eina_Bool
editor_part_clip_to_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_drag_confine_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_drag_threshold_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_drag_event_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_source2_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_source3_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_source4_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_source5_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_source6_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      const char * new_val);
Eina_Bool
editor_part_effect_set(Evas_Object *obj, Change *change, Eina_Bool merge, const char *part_name,
      Edje_Text_Effect new_val);

/* programs */
Edje_Tween_Mode
editor_program_transition_type_get(Evas_Object *edit_object, const char *program);
#define edje_edit_program_transition_type_get editor_program_transition_type_get
Eina_Bool
editor_program_transition_from_current_get(Evas_Object *edit_object, const char *program);
#define edje_edit_program_transition_from_current_get editor_program_transition_from_current_get

Eina_Bool
editor_program_transition_type_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                   const char *program, Edje_Tween_Mode new_val);
Eina_Bool
editor_program_transition_from_current_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                           const char *program, Eina_Bool new_val);
Eina_Bool
editor_program_action_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program, Edje_Action_Type new_val);
Eina_Bool
editor_program_channel_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                           const char *program, Edje_Channel new_val);
Eina_Bool
editor_program_tone_duration_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                 const char *program, double new_val);
Eina_Bool
editor_program_in_from_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                           const char *program, double new_val);
Eina_Bool
editor_program_in_range_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                            const char *program, double new_val);
Eina_Bool
editor_program_transition_time_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                   const char *program, double new_val);
Eina_Bool
editor_program_sample_speed_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                const char *program, double new_val);
Eina_Bool
editor_program_value2_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program, double new_val);
Eina_Bool
editor_program_value_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                         const char *program, double new_val);
Eina_Bool
editor_program_transition_value1_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                     const char *program, double new_val);
Eina_Bool
editor_program_transition_value2_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                     const char *program, double new_val);
Eina_Bool
editor_program_transition_value3_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                     const char *program, double new_val);
Eina_Bool
editor_program_transition_value4_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                     const char *program, double new_val);
Eina_Bool
editor_program_filter_part_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                               const char *program, const char *new_val);
Eina_Bool
editor_program_filter_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                const char *program, const char *new_val);
Eina_Bool
editor_program_api_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                            const char *program, const char *new_val);
Eina_Bool
editor_program_api_description_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                   const char *program, const char *new_val);
Eina_Bool
editor_program_sample_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                               const char *program, const char *new_val);
Eina_Bool
editor_program_tone_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                             const char *program, const char *new_val);
Eina_Bool
editor_program_signal_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program, const char *new_val);
Eina_Bool
editor_program_source_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program, const char *new_val);
Eina_Bool
editor_program_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                         const char *program, const char *new_val);
Eina_Bool
editor_program_state2_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program, const char *new_val);

#endif /* EDITOR_H */
