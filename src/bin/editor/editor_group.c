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

#define MAX_SET(VAL) \
Eina_Bool \
editor_group_max_## VAL ##_set(Evas_Object *obj, Change *change, int new_value) \
{ \
   Eina_Bool res; \
   int old_value; \
   int min_value; \
   Diff_ *diff; \
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
        diff = mem_calloc(1, sizeof(Diff_)); \
        diff->redo.type = FUNCTION_TYPE_INT; \
        diff->redo.function = editor_group_max_## VAL ##_set; \
        diff->redo.args.type_int.ival = new_value; \
        diff->undo.type = FUNCTION_TYPE_INT; \
        diff->undo.function = editor_group_max_## VAL ##_set; \
        diff->undo.args.type_int.ival = old_value; \
        change_diff_merge_add(change, diff); \
     } \
   res = edje_edit_group_max_## VAL ##_set(obj, new_value); \
   TODO("Add signal to property here") \
   return res; \
}

MAX_SET(w)
MAX_SET(h)

#define MIN_SET(VAL) \
Eina_Bool \
editor_group_min_## VAL ##_set(Evas_Object *obj, Change *change, int new_value) \
{ \
   Eina_Bool res; \
   int old_value; \
   int max_value; \
   Diff_ *diff; \
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
        diff = mem_calloc(1, sizeof(Diff_)); \
        diff->redo.type = FUNCTION_TYPE_INT; \
        diff->redo.function = editor_group_min_## VAL ##_set; \
        diff->redo.args.type_int.ival = new_value; \
        diff->undo.type = FUNCTION_TYPE_INT; \
        diff->undo.function = editor_group_min_## VAL ##_set; \
        diff->undo.args.type_int.ival = old_value; \
        change_diff_merge_add(change, diff); \
     } \
   res = edje_edit_group_min_## VAL ##_set(obj, new_value); \
   TODO("Add signal to property here") \
   return res; \
}

MIN_SET(w)
MIN_SET(h)
