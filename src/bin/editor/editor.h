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
   RM_ATTRIBUTE_NONE,

   RM_ATTRIBUTE_GROUP_NAME,
   RM_ATTRIBUTE_GROUP_MIN_W,
   RM_ATTRIBUTE_GROUP_MIN_H,
   RM_ATTRIBUTE_GROUP_MAX_W,
   RM_ATTRIBUTE_GROUP_MAX_H,
   RM_ATTRIBUTE_GROUP_DATA_NAME,
   RM_ATTRIBUTE_GROUP_DATA_VALUE,
   RM_ATTRIBUTE_STATE_NAME,
   RM_ATTRIBUTE_STATE_MIN_W,
   RM_ATTRIBUTE_STATE_MIN_H,
   RM_ATTRIBUTE_STATE_MAX_W,
   RM_ATTRIBUTE_STATE_MAX_H,
   RM_ATTRIBUTE_STATE_ALIGN_X,
   RM_ATTRIBUTE_STATE_ALIGN_Y,
   RM_ATTRIBUTE_STATE_REL1_RELATIVE_X,
   RM_ATTRIBUTE_STATE_REL1_RELATIVE_Y,
   RM_ATTRIBUTE_STATE_REL2_RELATIVE_X,
   RM_ATTRIBUTE_STATE_REL2_RELATIVE_Y,
   RM_ATTRIBUTE_STATE_REL1_OFFSET_X,
   RM_ATTRIBUTE_STATE_REL1_OFFSET_Y,
   RM_ATTRIBUTE_STATE_REL2_OFFSET_X,
   RM_ATTRIBUTE_STATE_REL2_OFFSET_Y,
   RM_ATTRIBUTE_STATE_ASPECT_MIN,
   RM_ATTRIBUTE_STATE_ASPECT_MAX,
   RM_ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_X,
   RM_ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_Y,
   RM_ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_X,
   RM_ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_Y,
   RM_ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_X,
   RM_ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_Y,
   RM_ATTRIBUTE_STATE_FILL_SIZE_OFFSET_X,
   RM_ATTRIBUTE_STATE_FILL_SIZE_OFFSET_Y,
   RM_ATTRIBUTE_STATE_TEXT_ALIGN_X,
   RM_ATTRIBUTE_STATE_TEXT_ALIGN_Y,
   RM_ATTRIBUTE_STATE_TEXT_ELIPSIS,
   RM_ATTRIBUTE_STATE_TEXT_ELIPSIS_ENABLE /* virtual attribute */,
   RM_ATTRIBUTE_STATE_TEXT_SIZE,
   RM_ATTRIBUTE_STATE_TEXT_FIT_X,
   RM_ATTRIBUTE_STATE_TEXT_FIT_Y,
   RM_ATTRIBUTE_STATE_TEXT_MAX_X,
   RM_ATTRIBUTE_STATE_TEXT_MAX_Y,
   RM_ATTRIBUTE_STATE_TEXT_MIN_X,
   RM_ATTRIBUTE_STATE_TEXT_MIN_Y,
   RM_ATTRIBUTE_STATE_FIXED_H,
   RM_ATTRIBUTE_STATE_FIXED_W,
   RM_ATTRIBUTE_STATE_FILL_SMOOTH,
   RM_ATTRIBUTE_STATE_VISIBLE,
   RM_ATTRIBUTE_STATE_IMAGE,
   RM_ATTRIBUTE_STATE_IMAGE_TWEEN,
   RM_ATTRIBUTE_STATE_COLOR_CLASS,
   RM_ATTRIBUTE_STATE_REL1_TO_X,
   RM_ATTRIBUTE_STATE_REL1_TO_Y,
   RM_ATTRIBUTE_STATE_REL2_TO_X,
   RM_ATTRIBUTE_STATE_REL2_TO_Y,
   RM_ATTRIBUTE_STATE_TEXT_SOURCE,
   RM_ATTRIBUTE_STATE_TEXT_TEXT_SOURCE,
   RM_ATTRIBUTE_STATE_TEXT,
   RM_ATTRIBUTE_STATE_FONT,
   RM_ATTRIBUTE_STATE_TEXT_STYLE,
   RM_ATTRIBUTE_STATE_COLOR,
   RM_ATTRIBUTE_STATE_OUTLINE_COLOR,
   RM_ATTRIBUTE_STATE_SHADOW_COLOR,
   RM_ATTRIBUTE_STATE_IMAGE_BORDER_TOP,
   RM_ATTRIBUTE_STATE_IMAGE_BORDER_BOTTOM,
   RM_ATTRIBUTE_STATE_IMAGE_BORDER_LEFT,
   RM_ATTRIBUTE_STATE_IMAGE_BORDER_RIGHT,
   RM_ATTRIBUTE_STATE_IMAGE_BORDER_FILL,
   RM_ATTRIBUTE_STATE_ASPECT_PREF,
   RM_ATTRIBUTE_PART_TEXT_EFFECT,
   RM_ATTRIBUTE_PART_TEXT_SHADOW_DIRECTION,
   RM_ATTRIBUTE_PART_IGNORE_FLAGS,
   RM_ATTRIBUTE_PART_MOUSE_EVENTS,
   RM_ATTRIBUTE_PART_REPEAT_EVENTS,
   RM_ATTRIBUTE_PART_SCALE,
   RM_ATTRIBUTE_PART_DRAG_COUNT_X,
   RM_ATTRIBUTE_PART_DRAG_COUNT_Y,
   RM_ATTRIBUTE_PART_DRAG_X,
   RM_ATTRIBUTE_PART_DRAG_Y,
   RM_ATTRIBUTE_PART_DRAG_STEP_X,
   RM_ATTRIBUTE_PART_DRAG_STEP_Y,
   RM_ATTRIBUTE_PART_CLIP_TO,
   RM_ATTRIBUTE_PART_DRAG_CONFINE,
   RM_ATTRIBUTE_PART_DRAG_THRESHOLD,
   RM_ATTRIBUTE_PART_DRAG_EVENT,
   RM_ATTRIBUTE_PART_NAME,
   RM_ATTRIBUTE_PART_TYPE,
   RM_ATTRIBUTE_PART_GROUP_SOURCE,
   RM_ATTRIBUTE_PART_TEXTBLOCK_SELECTION_UNDER,
   RM_ATTRIBUTE_PART_TEXTBLOCK_SELECTION_OVER,
   RM_ATTRIBUTE_PART_TEXTBLOCK_CURSOR_UNDER,
   RM_ATTRIBUTE_PART_TEXTBLOCK_CURSOR_OVER,
   RM_ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_UNDER,
   RM_ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_OVER,
   RM_ATTRIBUTE_PART_ITEM_ASPECT_MODE,
   RM_ATTRIBUTE_PART_ITEM_ALIGN_X,
   RM_ATTRIBUTE_PART_ITEM_ALIGN_Y,
   RM_ATTRIBUTE_PART_ITEM_WEIGHT_X,
   RM_ATTRIBUTE_PART_ITEM_WEIGHT_Y,
   RM_ATTRIBUTE_PART_ITEM_ASPECT_H,
   RM_ATTRIBUTE_PART_ITEM_ASPECT_W,
   RM_ATTRIBUTE_PART_ITEM_MAX_H,
   RM_ATTRIBUTE_PART_ITEM_MAX_W,
   RM_ATTRIBUTE_PART_ITEM_MIN_H,
   RM_ATTRIBUTE_PART_ITEM_MIN_W,
   RM_ATTRIBUTE_PART_ITEM_NAME,
   RM_ATTRIBUTE_PART_ITEM_PREFER_H,
   RM_ATTRIBUTE_PART_ITEM_PREFER_W,
   RM_ATTRIBUTE_PART_ITEM_SPREAD_H,
   RM_ATTRIBUTE_PART_ITEM_SPREAD_W,
   RM_ATTRIBUTE_PART_ITEM_SPAN_COL,
   RM_ATTRIBUTE_PART_ITEM_SPAN_ROW,
   RM_ATTRIBUTE_PART_ITEM_POSITION_COL,
   RM_ATTRIBUTE_PART_ITEM_POSITION_ROW,
   RM_ATTRIBUTE_PART_ITEM_SOURCE,
   RM_ATTRIBUTE_PART_ITEM_PADDING_TOP,
   RM_ATTRIBUTE_PART_ITEM_PADDING_BOTTOM,
   RM_ATTRIBUTE_PART_ITEM_PADDING_LEFT,
   RM_ATTRIBUTE_PART_ITEM_PADDING_RIGHT,
   RM_ATTRIBUTE_STATE_CONTAINER_ALIGN_X,
   RM_ATTRIBUTE_STATE_CONTAINER_ALIGN_Y,
   RM_ATTRIBUTE_STATE_CONTAINER_MIN_H,
   RM_ATTRIBUTE_STATE_CONTAINER_MIN_V,
   RM_ATTRIBUTE_STATE_TABLE_HOMOGENEOUS,
   RM_ATTRIBUTE_STATE_CONTAINER_PADING_X,
   RM_ATTRIBUTE_STATE_CONTAINER_PADING_Y,
   RM_ATTRIBUTE_STATE_MINMUL_H,
   RM_ATTRIBUTE_STATE_MINMUL_W,
   RM_ATTRIBUTE_PART_MULTILINE,
   RM_ATTRIBUTE_STATE_PROXY_SOURCE,
   RM_ATTRIBUTE_PART_SELECT_MODE,
   RM_ATTRIBUTE_PART_ENTRY_MODE,
   RM_ATTRIBUTE_PART_POINTER_MODE,
   RM_ATTRIBUTE_PART_CURSOR_MODE,
   RM_ATTRIBUTE_STATE_FILL_TYPE,
   RM_ATTRIBUTE_STATE_MAP_ON,
   RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE_ON,
   RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE,
   RM_ATTRIBUTE_STATE_MAP_LIGHT,
   RM_ATTRIBUTE_STATE_MAP_SMOOTH,
   RM_ATTRIBUTE_STATE_MAP_ALPHA,
   RM_ATTRIBUTE_STATE_MAP_BACKFACE_CULL,
   RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE_FOCAL,
   RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE_ZPLANE,
   RM_ATTRIBUTE_STATE_MAP_ROTATION_CENTER,
   RM_ATTRIBUTE_STATE_MAP_ROTATION_X,
   RM_ATTRIBUTE_STATE_MAP_ROTATION_Y,
   RM_ATTRIBUTE_STATE_MAP_ROTATION_Z,
   RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_1,
   RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_2,
   RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_3,
   RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_4,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_TYPE,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_FROM_CURRENT,
   RM_ATTRIBUTE_PROGRAM_ACTION,
   RM_ATTRIBUTE_PROGRAM_CHANNEL,
   RM_ATTRIBUTE_PROGRAM_TONE_DURATION,
   RM_ATTRIBUTE_PROGRAM_IN_FROM,
   RM_ATTRIBUTE_PROGRAM_IN_RANGE,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_TIME,
   RM_ATTRIBUTE_PROGRAM_SAMPLE_SPEED,
   RM_ATTRIBUTE_PROGRAM_VALUE,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_FACTOR,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_GRADIENT,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_DECAY,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_BOUNCES,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_SWINGS,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_X1,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_Y1,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_X2,
   RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_Y2,
   RM_ATTRIBUTE_PROGRAM_FILTER_PART,
   RM_ATTRIBUTE_PROGRAM_FILTER_STATE,
   RM_ATTRIBUTE_PROGRAM_API_NAME,
   RM_ATTRIBUTE_PROGRAM_API_DESCRIPTION,
   RM_ATTRIBUTE_PROGRAM_SAMPLE_NAME,
   RM_ATTRIBUTE_PROGRAM_TONE_NAME,
   RM_ATTRIBUTE_PROGRAM_SIGNAL,
   RM_ATTRIBUTE_PROGRAM_SOURCE,
   RM_ATTRIBUTE_PROGRAM_STATE,
   RM_ATTRIBUTE_PROGRAM_EMIT_SIGNAL,
   RM_ATTRIBUTE_PROGRAM_EMIT_SOURCE,
   RM_ATTRIBUTE_PROGRAM_DRAG_VALUE_X,
   RM_ATTRIBUTE_PROGRAM_DRAG_VALUE_Y,
   RM_ATTRIBUTE_PROGRAM_NAME,
   RM_ATTRIBUTE_PROGRAM_TARGET,
   RM_ATTRIBUTE_PROGRAM_AFTER,
   RM_ATTRIBUTE_PROGRAM_SCRIPT,

   RM_ATTRIBUTE_LAST
} RM_Attribute;

typedef enum {
   RM_ATTRIBUTE_RESOURCES_NONE,

   RM_ATTRIBUTE_RESOURCES_COLORCLASS_DESCRIPTION,
   RM_ATTRIBUTE_RESOURCES_COLORCLASS_COLORS,
   RM_ATTRIBUTE_RESOURCES_STYLE_TAG_ADDED,
   RM_ATTRIBUTE_RESOURCES_STYLE_TAG_DELETED,

   RM_ATTRIBUTE_RESOURCES_LAST
} RM_Attribute_Resources;

typedef struct {
   RM_Attribute attribute;
   Evas_Object *edit_object;
   Eina_Stringshare *group_name;
   Eina_Stringshare *part_name;
   Eina_Stringshare *item_name;
   Eina_Stringshare *state_name;
   Eina_Stringshare *program_name;
   double state_value;
   Eina_Stringshare *value;
   Eina_Stringshare *old_value;
} Editor_Attribute_Change;

typedef struct {
   RM_Attribute_Resources attribute;

   Eina_Stringshare *color_class_name;
   Eina_Stringshare *color_class_description;

   Eina_Stringshare *style_name;
   Eina_Stringshare *tag_value;
   Eina_Stringshare *tag_name;

   int r, g, b, a;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
} Editor_Attribute_Resource_Change;


void
_editor_project_changed();

/* Top Level Blocks */
Eina_Bool
editor_image_add(Evas_Object *obj, const char *selected, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_image_del(Evas_Object *obj, const char *selected, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_color_class_add(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_color_class_del(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_color_class_description_set(Evas_Object *obj, const char *name, const char *description) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_color_class_colors_set(Evas_Object *obj, const char *name,
                              int r, int g, int b, int a,
                              int r2, int g2, int b2, int a2,
                              int r3, int g3, int b3, int a3) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_sound_sample_add(Evas_Object *obj, const char *name, const char *source, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_sound_sample_del(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_sound_tone_add(Evas_Object *obj, const char *name, int frq, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_sound_tone_del(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_style_add(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_style_del(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_style_tag_add(Evas_Object *obj, const char *name, const char *tag) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_style_tag_del(Evas_Object *obj, const char *name, const char *tag) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_style_tag_value_set(Evas_Object *obj, const char *name, const char *tag, const char *value) EINA_WARN_UNUSED_RESULT;

/* General */
Eina_Bool
editor_save(Evas_Object *edit_object) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_save_all(Evas_Object *edit_object) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_internal_group_add(Evas_Object *edit_object) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_add(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_copy(Evas_Object *obj, const char *group_src, const char *group_dest, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_alias_add(Evas_Object *obj, const char *group_src, const char *group_alias, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_del(Evas_Object *obj, const char *name, Eina_Bool notify) EINA_WARN_UNUSED_RESULT;

Eina_Bool
you_shall_not_pass_editor_signals(Change *change);

Eina_Bool
you_shall_pass_editor_signals(Change *change);

/* Group */
Eina_Bool
editor_group_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply,
                       int new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply,
                       int new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply,
                       int new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply,
                       int new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_group_data_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                      const char *item_name) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_group_data_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                      const char *item_name) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_group_data_value_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *item_name, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_group_data_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *item_name, const char *new_val) EINA_WARN_UNUSED_RESULT;
/* State */

Eina_Bool
editor_state_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                 const char *part_name, const char *state_name, double state_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_copy(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                  const char *part_name, const char *from_name, double from_val,
                                         const char *state_name, double state_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                 const char *part_name, const char *state_name, double state_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_aspect_min_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_aspect_max_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_minmul_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_minmul_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel1_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel1_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel1_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel1_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel2_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel2_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel2_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel2_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_size_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_elipsis_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_origin_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_origin_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_origin_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_origin_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_size_relative_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_size_relative_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_size_offset_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_size_offset_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_container_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_container_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_container_padding_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_container_padding_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_visible_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fixed_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fixed_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_min_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_min_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_max_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_max_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_fit_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_fit_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_smooth_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_container_min_v_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_container_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_state_proxy_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel1_to_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel1_to_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel2_to_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_rel2_to_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_text_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_text_style_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_state_aspect_pref_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_table_homogeneous_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_image_border_fill_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_image_border_left_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_image_border_right_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_image_border_top_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_image_border_bottom_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_fill_type_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      unsigned char new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_color_class_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_state_color_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int r, int g, int b, int a) EINA_WARN_UNUSED_RESULT;

#define edje_edit_state_outline_color_get edje_edit_state_color2_get
#pragma GCC poison edje_edit_state_color2_get
Eina_Bool
editor_state_outline_color_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int r, int g, int b, int a) EINA_WARN_UNUSED_RESULT;

#define edje_edit_state_shadow_color_get edje_edit_state_color3_get
#pragma GCC poison edje_edit_state_color3_get
Eina_Bool
editor_state_shadow_color_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int r, int g, int b, int a) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_state_text_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_font_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_image_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;


/* Part */

Eina_Bool
editor_part_selected_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                               const char *part_name, const char *state_name, double state_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                const char *part_name, Edje_Part_Type type) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_copy(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                 const char *part_name, const char *part_name_copy) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                const char *part_name) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_restack(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                    const char *part_name, const char *relative_part) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_restack(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *part_name, const char *part_item, const char *relative_part_item) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_append(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                        const char *part_name, const char *item_name, const char *source_group) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                     const char *part_name, const char *item_name) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_prefer_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_prefer_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_align_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_align_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_weight_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_weight_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_aspect_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_aspect_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_spread_w_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_spread_h_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_span_col_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      unsigned short new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_span_row_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      unsigned short new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_position_col_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      unsigned short new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_position_row_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      unsigned short new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_aspect_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      Edje_Aspect_Control new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_index_aspect_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, unsigned int index,
      Edje_Aspect_Control new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_padding_bottom_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val1) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_padding_top_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val1) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_padding_left_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val1) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_item_padding_right_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *item_name,
      int new_val1) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_scale_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_mouse_events_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_repeat_events_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_multiline_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_ignore_flags_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Evas_Event_Flags new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_select_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Edje_Edit_Select_Mode new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_entry_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Edje_Edit_Entry_Mode new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_pointer_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Evas_Object_Pointer_Mode new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_cursor_mode_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      unsigned char new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_part_drag_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_step_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_step_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_count_x_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_count_y_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_name_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char *new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_state_tween_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                       const char *part_name, const char *state_name, double state_val,
                       Eina_Stringshare *name) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_tween_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                       const char *part_name, const char *state_name, double state_val,
                       Eina_Stringshare *name) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_part_clip_to_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_group_source_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_confine_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_threshold_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_drag_event_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_textblock_selection_under_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_textblock_selection_over_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_textblock_cursor_under_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_textblock_cursor_over_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_textblock_anchors_under_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_textblock_anchors_over_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
#define edje_edit_part_textblock_selection_under_get(...) (edje_edit_part_source_get(__VA_ARGS__))
#define edje_edit_part_textblock_selection_over_get(...) (edje_edit_part_source2_get(__VA_ARGS__))
#define edje_edit_part_textblock_cursor_under_get(...) (edje_edit_part_source3_get(__VA_ARGS__))
#define edje_edit_part_textblock_cursor_over_get(...) (edje_edit_part_source4_get(__VA_ARGS__))
#define edje_edit_part_textblock_anchors_under_get(...) (edje_edit_part_source5_get(__VA_ARGS__))
#define edje_edit_part_textblock_anchors_over_get(...) (edje_edit_part_source6_get(__VA_ARGS__))

#define edje_edit_part_text_effect_get(...) (edje_edit_part_effect_get(__VA_ARGS__) & EDJE_TEXT_EFFECT_MASK_BASIC)
#define edje_edit_part_text_shadow_direction_get(...) (edje_edit_part_effect_get(__VA_ARGS__) & EDJE_TEXT_EFFECT_MASK_SHADOW_DIRECTION)
#pragma GCC poison edje_edit_part_effect_get
Eina_Bool
editor_part_text_effect_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Edje_Text_Effect new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_part_text_shadow_direction_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name,
      Edje_Text_Effect new_val) EINA_WARN_UNUSED_RESULT;

/* programs */
Edje_Tween_Mode
editor_program_transition_type_get(Evas_Object *edit_object, const char *program);
#define edje_edit_program_transition_type_get editor_program_transition_type_get
Eina_Bool
editor_program_transition_from_current_get(Evas_Object *edit_object, const char *program);
#define edje_edit_program_transition_from_current_get editor_program_transition_from_current_get

Eina_Bool
editor_program_transition_type_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                   const char *program, Edje_Tween_Mode new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_from_current_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                           const char *program, Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_channel_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *program, Edje_Channel new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_tone_duration_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_in_from_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_in_range_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_time_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                   const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_sample_speed_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_value_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
#define edje_edit_program_drag_value_x_get edje_edit_program_value_get
Eina_Bool
editor_program_drag_value_x_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
#define edje_edit_program_drag_value_y_get edje_edit_program_value2_get
#pragma GCC poison edje_edit_program_value2_get
Eina_Bool
editor_program_drag_value_y_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *program, double new_val) EINA_WARN_UNUSED_RESULT;

#define edje_edit_program_transition_factor_get editor_program_transition_factor_get
#define edje_edit_program_transition_gradient_get editor_program_transition_gradient_get
#define edje_edit_program_transition_decay_get editor_program_transition_decay_get
#define edje_edit_program_transition_bounces_get editor_program_transition_bounces_get
#define edje_edit_program_transition_swings_get editor_program_transition_swings_get
#define edje_edit_program_transition_bezier_x1_get editor_program_transition_bezier_x1_get
#define edje_edit_program_transition_bezier_x2_get editor_program_transition_bezier_x2_get
#define edje_edit_program_transition_bezier_y1_get editor_program_transition_bezier_y1_get
#define edje_edit_program_transition_bezier_y2_get editor_program_transition_bezier_y2_get
double
editor_program_transition_factor_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_factor_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_gradient_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_gradient_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_decay_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_decay_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_bounces_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_bounces_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_swings_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_swings_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_bezier_x1_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_bezier_x1_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_bezier_x2_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_bezier_x2_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_bezier_y1_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_bezier_y1_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;
double
editor_program_transition_bezier_y2_get(Evas_Object *edit_object, const char *program) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_transition_bezier_y2_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val) EINA_WARN_UNUSED_RESULT;

Eina_Bool
editor_program_filter_part_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                               const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_filter_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_api_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_api_description_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                   const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_sample_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                               const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_tone_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                             const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_signal_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                          const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_source_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                          const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
#define edje_edit_program_emit_signal_get edje_edit_program_state_get
Eina_Bool
editor_program_emit_signal_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                               const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
#define edje_edit_program_emit_source_get edje_edit_program_state2_get
#pragma GCC poison edje_edit_program_state2_get
Eina_Bool
editor_program_emit_source_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *program, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                        const char *name, const char *new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_target_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                          const char *program_name, Eina_Stringshare *target) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_target_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                          const char *program_name, Eina_Stringshare *target) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_after_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *program_name, Eina_Stringshare *after) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_after_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *program_name, Eina_Stringshare *after) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                   const char *program_name, Edje_Action_Type type) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_program_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                   const char *program_name) EINA_WARN_UNUSED_RESULT;

/* map */
Eina_Bool
editor_state_map_on_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_perspective_on_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_smooth_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_alpha_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_backface_cull_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      Eina_Bool new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_perspective_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_light_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_rotation_x_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *part_name, const char *state_name, double state_val, double n4) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_rotation_y_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *part_name, const char *state_name, double state_val, double n4) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_rotation_z_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                const char *part_name, const char *state_name, double state_val, double n4) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_point_color_1_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *part_name, const char *state_name, double state_val, int n4, int n5, int n6, int n7) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_point_color_2_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *part_name, const char *state_name, double state_val, int n4, int n5, int n6, int n7) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_point_color_3_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *part_name, const char *state_name, double state_val, int n4, int n5, int n6, int n7) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_point_color_4_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *part_name, const char *state_name, double state_val, int n4, int n5, int n6, int n7) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_rotation_center_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      const char * new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_perspective_focal_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;
Eina_Bool
editor_state_map_perspective_zplane_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, const char *part_name, const char *state_name, double state_val,
      int new_val) EINA_WARN_UNUSED_RESULT;

#endif /* EDITOR_H */
