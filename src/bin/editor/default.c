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

#define EDITOR_DOUBLE_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_DOUBLE_RESET(FUNC, DEF_VAL, RESET_VAL) \
EDITOR_DOUBLE_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_DOUBLE_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_DOUBLE_RESET(rel1_relative_x, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(rel2_relative_x, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(rel1_relative_y, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(rel2_relative_y, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(align_x, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(align_y, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(aspect_min, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(aspect_max, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_origin_relative_x, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_origin_relative_y, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_size_relative_x, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(fill_size_relative_y, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(container_align_x, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(container_align_y, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(minmul_h, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(minmul_w, 0.0, 0.0)

#define EDITOR_INT_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   int val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   return val == DEF_VAL; \
}

#define EDITOR_INT_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_INT_RESET(FUNC, DEF_VAL, RESET_VAL) \
EDITOR_INT_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_INT_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_INT_RESET(rel1_offset_x, 0, 0)
EDITOR_STATE_INT_RESET(rel1_offset_y, 0, 0)
EDITOR_STATE_INT_RESET(rel2_offset_x, -1, -1)
EDITOR_STATE_INT_RESET(rel2_offset_y, -1, -1)
EDITOR_STATE_INT_RESET(max_w, -1, -1)
EDITOR_STATE_INT_RESET(max_h, -1, -1)
EDITOR_STATE_INT_RESET(min_w, 0, 0)
EDITOR_STATE_INT_RESET(min_h, 0, 0)
EDITOR_STATE_INT_RESET(fill_origin_offset_x, 0, 0)
EDITOR_STATE_INT_RESET(fill_origin_offset_y, 0, 0)
EDITOR_STATE_INT_RESET(fill_size_offset_x, -1, -1)
EDITOR_STATE_INT_RESET(fill_size_offset_y, -1, -1)
EDITOR_STATE_INT_RESET(container_padding_x, 0, 0)
EDITOR_STATE_INT_RESET(container_padding_y, 0, 0)

#define EDITOR_BOOL_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   Eina_Bool val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   return val == DEF_VAL; \
}

#define EDITOR_BOOL_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_BOOL_RESET(FUNC, DEF_VAL, RESET_VAL) \
EDITOR_BOOL_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_BOOL_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_BOOL_RESET(fixed_h, false, false)
EDITOR_STATE_BOOL_RESET(fixed_w, false, false)
EDITOR_STATE_BOOL_RESET(fill_smooth, true, true)
EDITOR_STATE_BOOL_RESET(visible, true, true)
EDITOR_STATE_BOOL_RESET(container_min_h, false, false)
EDITOR_STATE_BOOL_RESET(container_min_v, false, false)

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

#define EDITOR_NULL_STRING_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NULL_STRING_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS) \
EDITOR_NULL_STRING_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_NULL_STRING_RESET(rel1_to_x, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel1_to_y, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel2_to_x, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel2_to_y, NULL)
EDITOR_STATE_NULL_STRING_RESET(proxy_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(color_class, NULL)
EDITOR_STATE_NULL_STRING_RESET(image, "")

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
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4); \
}

#define EDITOR_STATE_INT_INT_INT_INT_RESET(FUNC, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4)

EDITOR_STATE_INT_INT_INT_INT_RESET(color, 255, 255, 255, 255)
EDITOR_STATE_INT_INT_INT_INT_RESET(color2, 0, 0, 0, 255)
EDITOR_STATE_INT_INT_INT_INT_RESET(color3, 0, 0, 0, 128)
EDITOR_STATE_INT_INT_INT_INT_RESET(image_border, 0, 0, 0, 0)


#define EDITOR_UCHAR_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   unsigned char val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   return val == DEF_VAL; \
}

#define EDITOR_UCHAR_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_UCHAR_RESET(FUNC, DEF_VAL, RESET_VAL) \
EDITOR_UCHAR_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_UCHAR_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_UCHAR_RESET(image_border_fill, 0, 0)
EDITOR_STATE_UCHAR_RESET(fill_type, 0, 0)
EDITOR_STATE_UCHAR_RESET(aspect_pref, 0, 0)
EDITOR_STATE_UCHAR_RESET(table_homogeneous, 0, 0)
