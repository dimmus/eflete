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

#include "default.h"

#define EDITOR_STATE_ARGS_PROTO const char *part_name, const char *state_name, double state_val
#define EDITOR_STATE_ARGS part_name, state_name, state_val

#define EDITOR_DOUBLE_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   double val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   return !(fabs(val - DEF_VAL) > DBL_EPSILON); \
}

#define EDITOR_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_DOUBLE_RESET(FUNC, DEF_VAL) \
EDITOR_DOUBLE_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL)

EDITOR_STATE_DOUBLE_RESET(rel1_relative_x, 0.0)
EDITOR_STATE_DOUBLE_RESET(rel2_relative_x, 1.0)
EDITOR_STATE_DOUBLE_RESET(rel1_relative_y, 0.0)
EDITOR_STATE_DOUBLE_RESET(rel2_relative_y, 1.0)
EDITOR_STATE_DOUBLE_RESET(align_x, 0.5)
EDITOR_STATE_DOUBLE_RESET(align_y, 0.5)
EDITOR_STATE_DOUBLE_RESET(aspect_min, 0.0)
EDITOR_STATE_DOUBLE_RESET(aspect_max, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_origin_relative_x, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_origin_relative_y, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_size_relative_x, 1.0)
EDITOR_STATE_DOUBLE_RESET(fill_size_relative_y, 1.0)
EDITOR_STATE_DOUBLE_RESET(container_align_x, 0.5)
EDITOR_STATE_DOUBLE_RESET(container_align_y, 0.5)
EDITOR_STATE_DOUBLE_RESET(minmul_h, 0.0)
EDITOR_STATE_DOUBLE_RESET(minmul_w, 0.0)
EDITOR_STATE_DOUBLE_RESET(text_align_x, 0.5)
EDITOR_STATE_DOUBLE_RESET(text_align_y, 0.5)
EDITOR_STATE_DOUBLE_RESET(text_elipsis, 0.0)

#define EDITOR_SIMPLE_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   return (edje_edit_##FUNC##_get(edit_object, ARGS) == DEF_VAL); \
}

#define EDITOR_STATE_SIMPLE_RESET(FUNC, DEF_VAL) \
EDITOR_SIMPLE_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL)

EDITOR_STATE_SIMPLE_RESET(rel1_offset_x, 0)
EDITOR_STATE_SIMPLE_RESET(rel1_offset_y, 0)
EDITOR_STATE_SIMPLE_RESET(rel2_offset_x, -1)
EDITOR_STATE_SIMPLE_RESET(rel2_offset_y, -1)
EDITOR_STATE_SIMPLE_RESET(max_w, -1)
EDITOR_STATE_SIMPLE_RESET(max_h, -1)
EDITOR_STATE_SIMPLE_RESET(min_w, 0)
EDITOR_STATE_SIMPLE_RESET(min_h, 0)
EDITOR_STATE_SIMPLE_RESET(fill_origin_offset_x, 0)
EDITOR_STATE_SIMPLE_RESET(fill_origin_offset_y, 0)
EDITOR_STATE_SIMPLE_RESET(fill_size_offset_x, -1)
EDITOR_STATE_SIMPLE_RESET(fill_size_offset_y, -1)
EDITOR_STATE_SIMPLE_RESET(container_padding_x, 0)
EDITOR_STATE_SIMPLE_RESET(container_padding_y, 0)
EDITOR_STATE_SIMPLE_RESET(text_size, 0)

EDITOR_STATE_SIMPLE_RESET(fixed_h, false)
EDITOR_STATE_SIMPLE_RESET(fixed_w, false)
EDITOR_STATE_SIMPLE_RESET(fill_smooth, true)
EDITOR_STATE_SIMPLE_RESET(visible, true)
EDITOR_STATE_SIMPLE_RESET(container_min_h, false)
EDITOR_STATE_SIMPLE_RESET(container_min_v, false)
EDITOR_STATE_SIMPLE_RESET(text_fit_x, false)
EDITOR_STATE_SIMPLE_RESET(text_fit_y, false)
EDITOR_STATE_SIMPLE_RESET(text_max_x, false)
EDITOR_STATE_SIMPLE_RESET(text_max_y, false)
EDITOR_STATE_SIMPLE_RESET(text_min_x, false)
EDITOR_STATE_SIMPLE_RESET(text_min_y, false)

#define EDITOR_NULL_STRING_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   Eina_Bool res; \
   assert(edit_object != NULL); \
   Eina_Stringshare *val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   if (val != NULL) \
     { \
       res = val == NULL; \
       eina_stringshare_del(val); \
     } \
   else \
     res = true; \
   return res; \
}

#define EDITOR_STATE_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NULL_STRING_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS) \
EDITOR_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_NULL_STRING_RESET(rel1_to_x, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel1_to_y, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel2_to_x, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel2_to_y, NULL)
EDITOR_STATE_NULL_STRING_RESET(proxy_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(color_class, NULL)
EDITOR_STATE_NULL_STRING_RESET(image, "")
EDITOR_STATE_NULL_STRING_RESET(text_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(text_text_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(text, NULL)
EDITOR_STATE_NULL_STRING_RESET(font, NULL)
EDITOR_STATE_NULL_STRING_RESET(text_style, NULL)

#define EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   int val, val2, val3, val4; \
   edje_edit_##FUNC##_get(edit_object, ARGS, &val, &val2, &val3, &val4); \
   return (val == DEF_VAL) && (val2 == DEF_VAL_2) && (val3 == DEF_VAL_3) && (val4 == DEF_VAL_4); \
}

#define EDITOR_INT_INT_INT_INT_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4); \
}

#define EDITOR_STATE_INT_INT_INT_INT_RESET(FUNC, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4)

EDITOR_STATE_INT_INT_INT_INT_RESET(color, 255, 255, 255, 255)
EDITOR_STATE_INT_INT_INT_INT_RESET(color2, 0, 0, 0, 255)
EDITOR_STATE_INT_INT_INT_INT_RESET(color3, 0, 0, 0, 128)
EDITOR_STATE_INT_INT_INT_INT_RESET(image_border, 0, 0, 0, 0)

EDITOR_STATE_SIMPLE_RESET(image_border_fill, 0)
EDITOR_STATE_SIMPLE_RESET(fill_type, 0)
EDITOR_STATE_SIMPLE_RESET(aspect_pref, 0)
EDITOR_STATE_SIMPLE_RESET(table_homogeneous, 0)


#define EDITOR_PART_ITEM_ARGS_PROTO const char *part_name, const char *item_name
#define EDITOR_PART_ITEM_ARGS part_name, item_name

#define EDITOR_PART_ITEM_SIMPLE_RESET(FUNC, DEF_VAL) \
EDITOR_SIMPLE_DEFAULT_CHECK(part_item_##FUNC, EDITOR_PART_ITEM_ARGS_PROTO, EDITOR_PART_ITEM_ARGS, DEF_VAL) \
EDITOR_RESET(part_item_##FUNC, EDITOR_PART_ITEM_ARGS_PROTO, EDITOR_PART_ITEM_ARGS, DEF_VAL)

EDITOR_PART_ITEM_SIMPLE_RESET(aspect_mode, EDJE_ASPECT_CONTROL_NONE)
EDITOR_PART_ITEM_SIMPLE_RESET(aspect_h, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(aspect_w, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(max_h, -1)
EDITOR_PART_ITEM_SIMPLE_RESET(max_w, -1)
EDITOR_PART_ITEM_SIMPLE_RESET(min_h, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(min_w, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(prefer_h, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(prefer_w, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(spread_h, 1)
EDITOR_PART_ITEM_SIMPLE_RESET(spread_w, 1)
EDITOR_PART_ITEM_SIMPLE_RESET(span_col, 1)
EDITOR_PART_ITEM_SIMPLE_RESET(span_row, 1)
TODO("check item position calculation. there is a type missmatch")
EDITOR_PART_ITEM_SIMPLE_RESET(position_col, 0)
EDITOR_PART_ITEM_SIMPLE_RESET(position_row, 0)
#define EDITOR_PART_ITEM_DOUBLE_RESET(FUNC, DEF_VAL) \
EDITOR_DOUBLE_DEFAULT_CHECK(part_item_##FUNC, EDITOR_PART_ITEM_ARGS_PROTO, EDITOR_PART_ITEM_ARGS, DEF_VAL) \
EDITOR_RESET(part_item_##FUNC, EDITOR_PART_ITEM_ARGS_PROTO, EDITOR_PART_ITEM_ARGS, DEF_VAL)

EDITOR_PART_ITEM_DOUBLE_RESET(align_x, 0.5)
EDITOR_PART_ITEM_DOUBLE_RESET(align_y, 0.5)
EDITOR_PART_ITEM_DOUBLE_RESET(weight_x, 0.0)
EDITOR_PART_ITEM_DOUBLE_RESET(weight_y, 0.0)

#define EDITOR_PART_ITEM_INT_INT_INT_INT_RESET(FUNC, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(part_item_##FUNC, EDITOR_PART_ITEM_ARGS_PROTO, EDITOR_PART_ITEM_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_RESET(part_item_##FUNC, EDITOR_PART_ITEM_ARGS_PROTO, EDITOR_PART_ITEM_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4)
EDITOR_PART_ITEM_INT_INT_INT_INT_RESET(padding, 0, 0, 0, 0)

TODO("Add stub-group for part items and implement reseter")


#define EDITOR_PART_ARGS_PROTO const char *part_name
#define EDITOR_PART_ARGS part_name

#define EDITOR_PART_SIMPLE_RESET(FUNC, DEF_VAL) \
EDITOR_SIMPLE_DEFAULT_CHECK(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS, DEF_VAL) \
EDITOR_RESET(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS, DEF_VAL)

EDITOR_PART_SIMPLE_RESET(effect, EDJE_TEXT_EFFECT_NONE)
EDITOR_PART_SIMPLE_RESET(ignore_flags, EVAS_EVENT_FLAG_NONE)
EDITOR_PART_SIMPLE_RESET(mouse_events, true)
EDITOR_PART_SIMPLE_RESET(repeat_events, false)
EDITOR_PART_SIMPLE_RESET(scale, false)
EDITOR_PART_SIMPLE_RESET(multiline, false)

EDITOR_PART_SIMPLE_RESET(drag_count_x, 0)
EDITOR_PART_SIMPLE_RESET(drag_count_y, 0)
EDITOR_PART_SIMPLE_RESET(drag_x, 0)
EDITOR_PART_SIMPLE_RESET(drag_y, 0)
EDITOR_PART_SIMPLE_RESET(drag_step_x, 0)
EDITOR_PART_SIMPLE_RESET(drag_step_y, 0)

EDITOR_PART_SIMPLE_RESET(select_mode, 0)
EDITOR_PART_SIMPLE_RESET(entry_mode, 0)
EDITOR_PART_SIMPLE_RESET(cursor_mode, 0)

Eina_Bool
editor_part_pointer_mode_default_is(Evas_Object *edit_object, EDITOR_PART_ARGS_PROTO)
{
   assert(edit_object != NULL);
   Edje_Part_Type type = edje_edit_part_type_get(edit_object, EDITOR_PART_ARGS);
   return (((type == EDJE_PART_TYPE_SWALLOW ||
             type == EDJE_PART_TYPE_GROUP ||
             type == EDJE_PART_TYPE_EXTERNAL) &&
             edje_edit_part_pointer_mode_get(edit_object, EDITOR_PART_ARGS) == EVAS_OBJECT_POINTER_MODE_NOGRAB)
           ||(edje_edit_part_pointer_mode_get(edit_object, EDITOR_PART_ARGS) == EVAS_OBJECT_POINTER_MODE_AUTOGRAB));
}
Eina_Bool
editor_part_pointer_mode_reset(Evas_Object *edit_object, Change *change, EDITOR_PART_ARGS_PROTO)
{
   assert(edit_object != NULL);
   if (editor_part_pointer_mode_default_is(edit_object, EDITOR_PART_ARGS)) return true;
   Edje_Part_Type type = edje_edit_part_type_get(edit_object, EDITOR_PART_ARGS);
   Evas_Object_Pointer_Mode reset_value = (type == EDJE_PART_TYPE_SWALLOW ||
                                           type == EDJE_PART_TYPE_GROUP ||
                                           type == EDJE_PART_TYPE_EXTERNAL) ? EVAS_OBJECT_POINTER_MODE_NOGRAB : EVAS_OBJECT_POINTER_MODE_AUTOGRAB;
   return editor_part_pointer_mode_set(edit_object, change, false, EDITOR_PART_ARGS, reset_value);
}

#define EDITOR_PART_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NULL_STRING_DEFAULT_CHECK(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS) \
EDITOR_RESET(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS, RESET_VAL)

EDITOR_PART_NULL_STRING_RESET(clip_to, NULL)
EDITOR_PART_NULL_STRING_RESET(drag_confine, NULL)
EDITOR_PART_NULL_STRING_RESET(drag_threshold, NULL)
EDITOR_PART_NULL_STRING_RESET(drag_event, NULL)
EDITOR_PART_NULL_STRING_RESET(source, NULL)
EDITOR_PART_NULL_STRING_RESET(source2, NULL)
EDITOR_PART_NULL_STRING_RESET(source3, NULL)
EDITOR_PART_NULL_STRING_RESET(source4, NULL)
EDITOR_PART_NULL_STRING_RESET(source5, NULL)
EDITOR_PART_NULL_STRING_RESET(source6, NULL)
