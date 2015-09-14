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

#include "editor.h"

#define MAX_SET(VAL, VAL_CAPS) \
Eina_Bool \
editor_group_max_## VAL ##_set(Evas_Object *obj, Change *change, Eina_Bool merge, \
                               int new_value) \
{ \
   Eina_Bool res; \
   int old_value; \
   int min_value; \
   Diff *diff; \
   Attribute attribute = ATTRIBUTE_GROUP_MAX_##VAL_CAPS; \
 \
   assert(obj != NULL); \
   assert(new_value >= 0); \
 \
   old_value = edje_edit_group_max_## VAL ##_get(obj); \
   min_value = edje_edit_group_min_## VAL ##_get(obj); \
 \
   if ((min_value > new_value) && (new_value != 0)) \
     new_value = min_value; \
 \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_INT; \
        diff->redo.function = editor_group_max_## VAL ##_set; \
        diff->redo.args.type_int.ival = new_value; \
        diff->undo.type = FUNCTION_TYPE_INT; \
        diff->undo.function = editor_group_max_## VAL ##_set; \
        diff->undo.args.type_int.ival = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   res = edje_edit_group_max_## VAL ##_set(obj, new_value); \
   TODO("Add signal to property here") \
   evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute); \
   _editor_project_changed(); \
   return res; \
}

MAX_SET(w, W)
MAX_SET(h, H)

#define MIN_SET(VAL, VAL_CAPS) \
Eina_Bool \
editor_group_min_## VAL ##_set(Evas_Object *obj, Change *change, Eina_Bool merge, \
                               int new_value) \
{ \
   Eina_Bool res; \
   int old_value; \
   int max_value; \
   Diff *diff; \
   Attribute attribute = ATTRIBUTE_GROUP_MIN_##VAL_CAPS; \
 \
   assert(obj != NULL); \
   assert(new_value >= 0); \
 \
   old_value = edje_edit_group_min_## VAL ##_get(obj); \
   max_value = edje_edit_group_max_## VAL ##_get(obj); \
 \
   if ((max_value < new_value)) \
     new_value = max_value; \
 \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_INT; \
        diff->redo.function = editor_group_min_## VAL ##_set; \
        diff->redo.args.type_int.ival = new_value; \
        diff->undo.type = FUNCTION_TYPE_INT; \
        diff->undo.function = editor_group_min_## VAL ##_set; \
        diff->undo.args.type_int.ival = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   res = edje_edit_group_min_## VAL ##_set(obj, new_value); \
   TODO("Add signal to property here") \
   evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute); \
   _editor_project_changed(); \
   return res; \
}

MIN_SET(w, W)
MIN_SET(h, H)
