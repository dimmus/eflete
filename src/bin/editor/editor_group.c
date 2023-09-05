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

#include "editor_private.h"

extern int _editor_signals_blocked;

Eina_Bool
editor_group_add(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_group_add(obj, name));
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_group_copy(Evas_Object *obj, const char *group_src, const char *group_dest, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(group_src != NULL);
   assert(group_dest != NULL);

   CRIT_ON_FAIL(edje_edit_group_copy(obj, group_src, group_dest));
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_ADDED, (void *)group_dest);
   return true;
}

Eina_Bool
editor_group_alias_add(Evas_Object *obj, const char *group_src, const char *group_alias, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(group_src != NULL);
   assert(group_alias != NULL);

   CRIT_ON_FAIL(edje_edit_group_alias_add(obj, group_src, group_alias));
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_ADDED, (void *)group_alias);
   return true;
}

Eina_Bool
editor_group_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   /* for deleting and cleaning up dependencies, for groups it is important to
      delete after cleaning up */
   if (notify)
     {
        evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_DELETED, (void *)name);
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_DELETED, (void *)name);
     }

   if (!edje_edit_group_del(obj, name))
      return false;
   if (!editor_save_all(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   return true;
}

Eina_Bool
editor_group_reset(Evas_Object *obj, Change *change, Eina_Bool apply)
{
   Eina_List *programs, *parts, *l;
   Eina_Stringshare *data;

   assert(obj != NULL);

   programs = edje_edit_programs_list_get(obj);
   parts = edje_edit_parts_list_get(obj);

   EINA_LIST_FOREACH(programs, l, data)
      CRIT_ON_FAIL(editor_program_reset(obj, change, apply, data));
   EINA_LIST_FOREACH(parts, l, data)
      CRIT_ON_FAIL(editor_part_reset(obj, change, apply, data));
   EINA_LIST_FOREACH(programs, l, data)
      CRIT_ON_FAIL(_editor_program_del(obj, change, false, apply, false, data));
   EINA_LIST_FOREACH(parts, l, data)
      CRIT_ON_FAIL(_editor_part_del(obj, change, false, apply, false, data));

   edje_edit_string_list_free(programs);
   edje_edit_string_list_free(parts);

   return true;
}

#define MAX_SET(VAL, VAL_CAPS) \
Eina_Bool \
editor_group_max_## VAL ##_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, \
                               int new_value) \
{ \
   int old_value; \
   int min_value; \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = obj; \
 \
   send.attribute = RM_ATTRIBUTE_GROUP_MAX_##VAL_CAPS; \
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
   if (apply) \
     { \
        CRIT_ON_FAIL(edje_edit_group_max_## VAL ##_set(obj, new_value)); \
        _editor_project_changed(); \
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

MAX_SET(w, W)
MAX_SET(h, H)

#define MIN_SET(VAL, VAL_CAPS) \
Eina_Bool \
editor_group_min_## VAL ##_set(Evas_Object *obj, Change *change, Eina_Bool merge, Eina_Bool apply, \
                               int new_value) \
{ \
   int old_value; \
   int max_value; \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = obj; \
 \
   send.attribute = RM_ATTRIBUTE_GROUP_MIN_##VAL_CAPS; \
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
   if (apply) \
     { \
        CRIT_ON_FAIL(edje_edit_group_min_## VAL ##_set(obj, new_value)); \
        _editor_project_changed(); \
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

MIN_SET(w, W)
MIN_SET(h, H)

Eina_Bool
editor_group_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                      const char *new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.edit_object = edit_object;

   send.attribute = RM_ATTRIBUTE_GROUP_NAME;
   assert(edit_object != NULL);
   assert(new_val != NULL);
   if (change)
     {
        const char *old_value;
        edje_object_file_get(edit_object, NULL, &old_value);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_group_name_set;
        diff->redo.args.type_s.s1 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_group_name_set;
        diff->undo.args.type_s.s1 = eina_stringshare_add(old_value);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_group_name_set(edit_object, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

Eina_Bool
editor_group_script_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                        const char *new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.edit_object = edit_object;

   send.attribute = RM_ATTRIBUTE_GROUP_SCRIPT;
   assert(edit_object != NULL);
   assert(new_val != NULL);
   if (change)
     {
        char *old_value = edje_edit_script_get(edit_object);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_group_script_set;
        diff->redo.args.type_s.s1 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_group_script_set;
        diff->undo.args.type_s.s1 = eina_stringshare_add(old_value);
        free(old_value);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
       CRIT_ON_FAIL(edje_edit_script_set(edit_object, new_val));
       _editor_project_changed();
       if (!_editor_signals_blocked)
         {
            evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
         }
     }
   return true;
}

Eina_Bool
editor_group_data_value_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *item_name, const char *new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.edit_object = edit_object;

   send.attribute = RM_ATTRIBUTE_GROUP_DATA_VALUE;
   assert(edit_object != NULL);
   assert(item_name != NULL);
   assert(new_val != NULL);
   if (change)
     {
        Eina_Stringshare *old_value = edje_edit_group_data_value_get(edit_object, item_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_group_data_value_set;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(item_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_group_data_value_set;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(item_name);
        diff->undo.args.type_ss.s2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_group_data_value_set(edit_object, item_name, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

Eina_Bool
editor_group_data_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *item_name, const char *new_val)
{
   Diff *diff;
   Rename ren;
   Editor_Attribute_Change send;
   send.edit_object = edit_object;

   assert(edit_object != NULL);
   assert(item_name != NULL);
   assert(new_val != NULL);

   send.attribute = RM_ATTRIBUTE_GROUP_DATA_NAME;
   send.old_value = eina_stringshare_add(item_name);
   send.value = eina_stringshare_add(new_val);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_group_data_name_set;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(item_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_group_data_name_set;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(new_val);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(item_name);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_group_data_name_set(edit_object, item_name, new_val));
        _editor_project_changed();
        ren.old_name = item_name;
        ren.new_name = new_val;
        if (!_editor_signals_blocked)
          {
             evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_DATA_RENAMED, &ren);
             evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
          }
     }
   eina_stringshare_del(send.old_value);
   eina_stringshare_del(send.value);
   return true;
}

Eina_Bool
editor_group_data_add(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                      const char *item_name)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);
   assert(item_name != NULL);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_group_data_add;
        diff->redo.args.type_s.s1 = eina_stringshare_add(item_name);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_group_data_del;
        diff->undo.args.type_s.s1 = eina_stringshare_add(item_name);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_group_data_add(edit_object, item_name, ""));

        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();

        event_info = eina_stringshare_add(item_name);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_DATA_ADDED, (void *)event_info);
        eina_stringshare_del(event_info);
     }
   return true;
}

Eina_Bool
editor_group_data_del(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                      const char *item_name)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);
   assert(item_name != NULL);

   event_info = eina_stringshare_add(item_name);
   if (!_editor_signals_blocked)
     {
        /* so in here we need to delete part from workspace, groupedit,
           all kind of UI part lists since they use original Resource structure,
           and only after that we can finally delete it,
           so keep those signals in this order please */
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_DATA_PREDELETED, (void *)event_info);
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_GROUP_DATA_DELETED, (void *)event_info);
     }

   if (change)
     {
        if (!editor_group_data_value_set(edit_object, change, false, apply, item_name, ""))
          return false;
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_group_data_del;
        diff->redo.args.type_s.s1 = eina_stringshare_add(item_name);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_group_data_add;
        diff->undo.args.type_s.s1 = eina_stringshare_add(item_name);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_group_data_del(edit_object, item_name));
        eina_stringshare_del(event_info);
        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();
     }
   return true;
}
