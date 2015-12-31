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

#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "editor.h"
#include "diff.h"
#include "change.h"

extern int _editor_signals_blocked;
Eina_Bool
editor_group_add(Evas_Object *obj, const char *name)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_group_add(obj, name))
     return false;
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   return true;
}

Eina_Bool
editor_group_copy(Evas_Object *obj, const char *group_src, const char *group_dest)
{
   assert(obj != NULL);
   assert(group_src != NULL);
   assert(group_dest != NULL);

   if (!edje_edit_group_copy(obj, group_src, group_dest))
     return false;
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   return true;
}

Eina_Bool
editor_group_alias_add(Evas_Object *obj, const char *group_src, const char *group_alias)
{
   assert(obj != NULL);
   assert(group_src != NULL);
   assert(group_alias != NULL);

   if (!edje_edit_group_alias_add(obj, group_src, group_alias))
     return false;
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   return true;
}

Eina_Bool
editor_group_del(Evas_Object *obj, const char *name)
{
   assert(obj != NULL);
   assert(name != NULL);

   if (!edje_edit_group_del(obj, name))
     return false;
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   return true;
}

#define MAX_SET(VAL, VAL_CAPS) \
Eina_Bool \
editor_group_max_## VAL ##_set(Evas_Object *obj, Change *change, Eina_Bool merge, \
                               int new_value) \
{ \
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
        diff->redo.args.type_i.i1 = new_value; \
        diff->undo.type = FUNCTION_TYPE_INT; \
        diff->undo.function = editor_group_max_## VAL ##_set; \
        diff->undo.args.type_i.i1 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (!edje_edit_group_max_## VAL ##_set(obj, new_value)) \
     return false; \
   _editor_project_changed(); \
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute); \
   return true; \
}

MAX_SET(w, W)
MAX_SET(h, H)

#define MIN_SET(VAL, VAL_CAPS) \
Eina_Bool \
editor_group_min_## VAL ##_set(Evas_Object *obj, Change *change, Eina_Bool merge, \
                               int new_value) \
{ \
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
   if ((max_value < new_value) && (max_value != 0)) \
     new_value = max_value; \
 \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_INT; \
        diff->redo.function = editor_group_min_## VAL ##_set; \
        diff->redo.args.type_i.i1 = new_value; \
        diff->undo.type = FUNCTION_TYPE_INT; \
        diff->undo.function = editor_group_min_## VAL ##_set; \
        diff->undo.args.type_i.i1 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (!edje_edit_group_min_## VAL ##_set(obj, new_value)) \
     return false; \
   _editor_project_changed(); \
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute); \
   return true; \
}

MIN_SET(w, W)
MIN_SET(h, H)

Eina_Bool
editor_group_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                      const char *new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_GROUP_NAME;
   assert(edit_object != NULL);
   assert(new_val != NULL);
   if (change)
     {
        const char *old_value;
        edje_object_file_get(edit_object, NULL, &old_value);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_group_name_set;
        diff->redo.args.type_ssds.s1 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_group_name_set;
        diff->undo.args.type_ssds.s1 = eina_stringshare_add(old_value);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_group_name_set(edit_object, new_val))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}
