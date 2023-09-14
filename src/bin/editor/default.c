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
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   if (editor_##FUNC##_set(edit_object, change, false, apply, ARGS, RESET_VAL)) \
     return true; \
   CRIT("reset failed"); \
   abort(); \
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

#define EDITOR_THREE_DOUBLE_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL1, DEF_VAL2, DEF_VAL3) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   Eina_Bool res = true; \
   double val1, val2, val3; \
   edje_edit_##FUNC##_get(edit_object, ARGS, &val1, &val2, &val3); \
   res = (res) & (!(fabs(val1 - DEF_VAL1) > DBL_EPSILON)); \
   res = (res) & (!(fabs(val2 - DEF_VAL2) > DBL_EPSILON)); \
   res = (res) & (!(fabs(val3 - DEF_VAL3) > DBL_EPSILON)); \
   return res; \
}
#define EDITOR_THREE_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL1, RESET_VAL2, RESET_VAL3) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   Eina_Bool res = true; \
   if (editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   res = (res) & (editor_##FUNC##_x_set(edit_object, change, false, apply, ARGS, RESET_VAL1)); \
   res = (res) & (editor_##FUNC##_y_set(edit_object, change, false, apply, ARGS, RESET_VAL2)); \
   res = (res) & (editor_##FUNC##_z_set(edit_object, change, false, apply, ARGS, RESET_VAL3)); \
   if (res) return true; \
   CRIT("reset failed"); \
   abort(); \
}

#define EDITOR_STATE_THREE_DOUBLE_RESET(FUNC, DEF_VAL1, DEF_VAL2, DEF_VAL3) \
EDITOR_THREE_DOUBLE_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL1, DEF_VAL2, DEF_VAL3) \
EDITOR_THREE_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL1, DEF_VAL2, DEF_VAL3)

EDITOR_STATE_THREE_DOUBLE_RESET(map_rotation, 0.0, 0.0, 0.0)

#define EDITOR_SIMPLE_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   return EINA_DBL_EQ(edje_edit_##FUNC##_get(edit_object, ARGS), DEF_VAL); \
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
EDITOR_STATE_SIMPLE_RESET(fill_size_offset_x, 0)
EDITOR_STATE_SIMPLE_RESET(fill_size_offset_y, 0)
EDITOR_STATE_SIMPLE_RESET(container_padding_x, 0)
EDITOR_STATE_SIMPLE_RESET(container_padding_y, 0)
EDITOR_STATE_SIMPLE_RESET(text_size, 12)
EDITOR_STATE_SIMPLE_RESET(map_perspective_focal, 1000)
EDITOR_STATE_SIMPLE_RESET(map_perspective_zplane, 0)

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
EDITOR_STATE_SIMPLE_RESET(map_on, false)
EDITOR_STATE_SIMPLE_RESET(map_perspective_on, false)
EDITOR_STATE_SIMPLE_RESET(map_smooth, true)
EDITOR_STATE_SIMPLE_RESET(map_alpha, true)
EDITOR_STATE_SIMPLE_RESET(map_backface_cull, false)

#define EDITOR_NULL_STRING_DEFAULT_CHECK(FUNC, REAL_FUNC, PROTO_ARGS, ARGS) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   Eina_Bool res; \
   assert(edit_object != NULL); \
   Eina_Stringshare *val = edje_edit_##REAL_FUNC##_get(edit_object, ARGS); \
   if (val != NULL) \
     { \
       res = false; \
       eina_stringshare_del(val); \
     } \
   else \
     res = true; \
   return res; \
}

#define EDITOR_NOT_NULL_STRING_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   Eina_Bool res; \
   assert(edit_object != NULL); \
   Eina_Stringshare *val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   if (val != NULL) \
     { \
       res = !strcmp(val, RESET_VAL); \
       eina_stringshare_del(val); \
     } \
   else \
     res = false; \
   return res; \
}

#define EDITOR_STATE_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NULL_STRING_DEFAULT_CHECK(state_##FUNC, state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS) \
EDITOR_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_NULL_STRING_RESET(rel1_to_x, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel1_to_y, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel2_to_x, NULL)
EDITOR_STATE_NULL_STRING_RESET(rel2_to_y, NULL)
EDITOR_STATE_NULL_STRING_RESET(proxy_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(color_class, NULL)
EDITOR_STATE_NULL_STRING_RESET(text_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(text_text_source, NULL)
EDITOR_STATE_NULL_STRING_RESET(text_style, NULL)
EDITOR_STATE_NULL_STRING_RESET(font, NULL)
EDITOR_STATE_NULL_STRING_RESET(map_perspective, NULL)
EDITOR_STATE_NULL_STRING_RESET(map_light, NULL)
EDITOR_STATE_NULL_STRING_RESET(map_rotation_center, NULL)

#define EDITOR_STATE_NOT_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NOT_NULL_STRING_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL) \
EDITOR_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_NOT_NULL_STRING_RESET(image, EFLETE_DUMMY_IMAGE_NAME)
EDITOR_STATE_NULL_STRING_RESET(vector, NULL)
EDITOR_STATE_NOT_NULL_STRING_RESET(text, "")

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
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   if(editor_##FUNC##_set(edit_object, change, false, apply, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4)) \
     return true; \
   CRIT("reset failed"); \
   abort(); \
}

#define EDITOR_STATE_INT_INT_INT_INT_RESET(FUNC, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4)

EDITOR_STATE_INT_INT_INT_INT_RESET(color, 255, 255, 255, 255)
EDITOR_STATE_INT_INT_INT_INT_RESET(outline_color, 0, 0, 0, 255)
EDITOR_STATE_INT_INT_INT_INT_RESET(shadow_color, 0, 0, 0, 128)

#define EDITOR_INT_INT_INT_INT_INT_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   int val, val2, val3, val4; \
   Eina_Bool res = true; \
   edje_edit_##FUNC##_get(edit_object, ARGS, 0, &val, &val2, &val3, &val4); \
   res = res & ((val == DEF_VAL) && (val2 == DEF_VAL_2) && (val3 == DEF_VAL_3) && (val4 == DEF_VAL_4)); \
   edje_edit_##FUNC##_get(edit_object, ARGS, 1, &val, &val2, &val3, &val4); \
   res = res & ((val == DEF_VAL) && (val2 == DEF_VAL_2) && (val3 == DEF_VAL_3) && (val4 == DEF_VAL_4)); \
   edje_edit_##FUNC##_get(edit_object, ARGS, 2, &val, &val2, &val3, &val4); \
   res = res & ((val == DEF_VAL) && (val2 == DEF_VAL_2) && (val3 == DEF_VAL_3) && (val4 == DEF_VAL_4)); \
   edje_edit_##FUNC##_get(edit_object, ARGS, 3, &val, &val2, &val3, &val4); \
   res = res & ((val == DEF_VAL) && (val2 == DEF_VAL_2) && (val3 == DEF_VAL_3) && (val4 == DEF_VAL_4)); \
   return res; \
}
#define EDITOR_INT_INT_INT_INT_INT_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   Eina_Bool res = true; \
   res = res & editor_##FUNC##_1_set(edit_object, change, false, apply, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4); \
   res = res & editor_##FUNC##_2_set(edit_object, change, false, apply, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4); \
   res = res & editor_##FUNC##_3_set(edit_object, change, false, apply, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4); \
   res = res & editor_##FUNC##_4_set(edit_object, change, false, apply, ARGS, RESET_VAL, RESET_VAL_2, RESET_VAL_3, RESET_VAL_4); \
   if(res) return true; \
   CRIT("reset failed"); \
   abort(); \
}

#define EDITOR_STATE_INT_INT_INT_INT_INT_RESET(FUNC, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_INT_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4) \
EDITOR_INT_INT_INT_INT_INT_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL, DEF_VAL_2, DEF_VAL_3, DEF_VAL_4)

EDITOR_STATE_INT_INT_INT_INT_INT_RESET(map_point_color, 255, 255, 255, 255)

#define EDITOR_FOUR_INT_RESET(FUNC, TYPE, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_##TYPE##_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if(editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   if(editor_##FUNC##_##TYPE##_set(edit_object, change, false, apply, ARGS, RESET_VAL)) \
     return true; \
   CRIT("reset failed"); \
   abort(); \
}

EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(state_image_border, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, 0, 0, 0, 0)
EDITOR_FOUR_INT_RESET(state_image_border, top, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, 0)
EDITOR_FOUR_INT_RESET(state_image_border, bottom, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, 0)
EDITOR_FOUR_INT_RESET(state_image_border, left, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, 0)
EDITOR_FOUR_INT_RESET(state_image_border, right, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, 0)

EDITOR_STATE_SIMPLE_RESET(image_border_fill, 1)
EDITOR_STATE_SIMPLE_RESET(fill_type, 0)
EDITOR_STATE_SIMPLE_RESET(aspect_pref, 0)
EDITOR_STATE_SIMPLE_RESET(table_homogeneous, 0)


#define EDITOR_PART_ITEM_INDEX_ARGS_PROTO const char *part_name, unsigned int index
#define EDITOR_PART_ITEM_INDEX_ARGS part_name, index

#define EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(FUNC, DEF_VAL) \
EDITOR_SIMPLE_DEFAULT_CHECK(part_item_index_##FUNC, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, DEF_VAL) \
EDITOR_RESET(part_item_index_##FUNC, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, DEF_VAL)

EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(aspect_mode, EDJE_ASPECT_CONTROL_NONE)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(aspect_h, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(aspect_w, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(max_h, -1)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(max_w, -1)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(min_h, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(min_w, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(prefer_h, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(prefer_w, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(spread_h, 1)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(spread_w, 1)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(span_col, 1)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(span_row, 1)
TODO("check item position calculation. there is a type missmatch")
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(position_col, 0)
EDITOR_PART_ITEM_INDEX_SIMPLE_RESET(position_row, 0)

#define EDITOR_PART_ITEM_INDEX_DOUBLE_RESET(FUNC, DEF_VAL) \
EDITOR_DOUBLE_DEFAULT_CHECK(part_item_index_##FUNC, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, DEF_VAL) \
EDITOR_RESET(part_item_index_##FUNC, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, DEF_VAL)

EDITOR_PART_ITEM_INDEX_DOUBLE_RESET(align_x, 0.5)
EDITOR_PART_ITEM_INDEX_DOUBLE_RESET(align_y, 0.5)
EDITOR_PART_ITEM_INDEX_DOUBLE_RESET(weight_x, 0.0)
EDITOR_PART_ITEM_INDEX_DOUBLE_RESET(weight_y, 0.0)

EDITOR_INT_INT_INT_INT_DEFAULT_CHECK(part_item_index_padding, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, 0, 0, 0, 0)
EDITOR_FOUR_INT_RESET(part_item_index_padding, top, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, 0)
EDITOR_FOUR_INT_RESET(part_item_index_padding, bottom, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, 0)
EDITOR_FOUR_INT_RESET(part_item_index_padding, left, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, 0)
EDITOR_FOUR_INT_RESET(part_item_index_padding, right, EDITOR_PART_ITEM_INDEX_ARGS_PROTO, EDITOR_PART_ITEM_INDEX_ARGS, 0)
TODO("Add stub-group for part items and implement reseter")


#define EDITOR_PART_ARGS_PROTO const char *part_name
#define EDITOR_PART_ARGS part_name

#define EDITOR_PART_SIMPLE_RESET(FUNC, DEF_VAL) \
EDITOR_SIMPLE_DEFAULT_CHECK(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS, DEF_VAL) \
EDITOR_RESET(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS, DEF_VAL)

EDITOR_PART_SIMPLE_RESET(text_effect, EDJE_TEXT_EFFECT_NONE)
EDITOR_PART_SIMPLE_RESET(text_shadow_direction, EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM_RIGHT)
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
editor_part_pointer_mode_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply, EDITOR_PART_ARGS_PROTO)
{
   assert(edit_object != NULL);
   if (editor_part_pointer_mode_default_is(edit_object, EDITOR_PART_ARGS)) return true;
   Edje_Part_Type type = edje_edit_part_type_get(edit_object, EDITOR_PART_ARGS);
   Evas_Object_Pointer_Mode reset_value = (type == EDJE_PART_TYPE_SWALLOW ||
                                           type == EDJE_PART_TYPE_GROUP ||
                                           type == EDJE_PART_TYPE_EXTERNAL) ? EVAS_OBJECT_POINTER_MODE_NOGRAB : EVAS_OBJECT_POINTER_MODE_AUTOGRAB;
   if (editor_part_pointer_mode_set(edit_object, change, false, apply, EDITOR_PART_ARGS, reset_value))
     return true;
   CRIT("reset failed");
   abort();
}

#define EDITOR_PART_NULL_STRING_RESET(FUNC, REAL_FUNC, RESET_VAL) \
EDITOR_NULL_STRING_DEFAULT_CHECK(part_##FUNC, part_##REAL_FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS) \
EDITOR_RESET(part_##FUNC, EDITOR_PART_ARGS_PROTO, EDITOR_PART_ARGS, RESET_VAL)

EDITOR_PART_NULL_STRING_RESET(clip_to, clip_to, NULL)
EDITOR_PART_NULL_STRING_RESET(drag_confine, drag_confine, NULL)
EDITOR_PART_NULL_STRING_RESET(drag_threshold, drag_threshold, NULL)
EDITOR_PART_NULL_STRING_RESET(drag_event, drag_event, NULL)
EDITOR_PART_NULL_STRING_RESET(group_source, source, NULL)
EDITOR_PART_NULL_STRING_RESET(textblock_selection_under, source, NULL)
EDITOR_PART_NULL_STRING_RESET(textblock_selection_over, source2, NULL)
EDITOR_PART_NULL_STRING_RESET(textblock_cursor_under, source3, NULL)
EDITOR_PART_NULL_STRING_RESET(textblock_cursor_over, source4, NULL)
EDITOR_PART_NULL_STRING_RESET(textblock_anchors_under, source5, NULL)
EDITOR_PART_NULL_STRING_RESET(textblock_anchors_over, source6, NULL)

#define EDITOR_PROGRAM_ARGS_PROTO const char *program_name
#define EDITOR_PROGRAM_ARGS program_name

#define EDITOR_PROGRAM_SIMPLE_RESET(FUNC, DEF_VAL) \
EDITOR_SIMPLE_DEFAULT_CHECK(program_##FUNC, EDITOR_PROGRAM_ARGS_PROTO, EDITOR_PROGRAM_ARGS, DEF_VAL) \
EDITOR_RESET(program_##FUNC, EDITOR_PROGRAM_ARGS_PROTO, EDITOR_PROGRAM_ARGS, DEF_VAL)

EDITOR_PROGRAM_SIMPLE_RESET(transition_type, EDJE_TWEEN_MODE_LINEAR)
EDITOR_PROGRAM_SIMPLE_RESET(transition_from_current, false)
EDITOR_PROGRAM_SIMPLE_RESET(channel, EDJE_CHANNEL_EFFECT)
EDITOR_PROGRAM_SIMPLE_RESET(tone_duration, 0.1)
EDITOR_PROGRAM_SIMPLE_RESET(in_from, 0)
EDITOR_PROGRAM_SIMPLE_RESET(in_range, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_time, 0)
EDITOR_PROGRAM_SIMPLE_RESET(sample_speed, 1.0)
EDITOR_PROGRAM_SIMPLE_RESET(value, 0)
EDITOR_PROGRAM_SIMPLE_RESET(drag_value_x, 0)
EDITOR_PROGRAM_SIMPLE_RESET(drag_value_y, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_factor, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_gradient, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_decay, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_bounces, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_swings, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_bezier_x1, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_bezier_x2, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_bezier_y1, 0)
EDITOR_PROGRAM_SIMPLE_RESET(transition_bezier_y2, 0)

#define EDITOR_PROGRAM_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NULL_STRING_DEFAULT_CHECK(program_##FUNC, program_##FUNC, EDITOR_PROGRAM_ARGS_PROTO, EDITOR_PROGRAM_ARGS) \
EDITOR_RESET(program_##FUNC, EDITOR_PROGRAM_ARGS_PROTO, EDITOR_PROGRAM_ARGS, RESET_VAL)
TODO("fix edje_edit API to accept NULL")
EDITOR_PROGRAM_NULL_STRING_RESET(filter_part, "")
EDITOR_PROGRAM_NULL_STRING_RESET(filter_state, "")
EDITOR_PROGRAM_NULL_STRING_RESET(api_name, NULL)
EDITOR_PROGRAM_NULL_STRING_RESET(api_description, NULL)
EDITOR_PROGRAM_NULL_STRING_RESET(tone_name, "")
EDITOR_PROGRAM_NULL_STRING_RESET(signal, "")
EDITOR_PROGRAM_NULL_STRING_RESET(source, "")
EDITOR_PROGRAM_NULL_STRING_RESET(state, "")
EDITOR_PROGRAM_NULL_STRING_RESET(emit_signal, "")
EDITOR_PROGRAM_NULL_STRING_RESET(emit_source, "")

#define EDITOR_PROGRAM_NOT_NULL_STRING_RESET(FUNC, RESET_VAL) \
EDITOR_NOT_NULL_STRING_DEFAULT_CHECK(program_##FUNC, EDITOR_PROGRAM_ARGS_PROTO, EDITOR_PROGRAM_ARGS, RESET_VAL) \
EDITOR_RESET(program_##FUNC, EDITOR_PROGRAM_ARGS_PROTO, EDITOR_PROGRAM_ARGS, RESET_VAL)
EDITOR_PROGRAM_NOT_NULL_STRING_RESET(sample_name, EFLETE_DUMMY_SAMPLE_NAME)
