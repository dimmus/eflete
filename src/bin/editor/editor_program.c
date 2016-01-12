/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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
#include "editor_macro.h"
#include "change.h"
#include "diff.h"

extern int _editor_signals_blocked;

Edje_Tween_Mode
editor_program_transition_type_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   return edje_edit_program_transition_get(edit_object, program) & EDJE_TWEEN_MODE_MASK;
}

Eina_Bool
editor_program_transition_from_current_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   return edje_edit_program_transition_get(edit_object, program) & EDJE_TWEEN_MODE_OPT_FROM_CURRENT;
}

Eina_Bool
editor_program_transition_type_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                   const char *program, Edje_Tween_Mode new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_TRANSITION_TYPE;

   assert(edit_object != NULL);
   assert(program != NULL);
   assert(new_val < EDJE_TWEEN_MODE_LAST);

   if (change)
     {
        Edje_Tween_Mode old_value = editor_program_transition_type_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJETWEENMODE;
        diff->redo.function = editor_program_transition_type_set;
        diff->redo.args.type_setm.s1 = eina_stringshare_add(program);
        diff->redo.args.type_setm.etm2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJETWEENMODE;
        diff->undo.function = editor_program_transition_type_set;
        diff->undo.args.type_setm.s1 = eina_stringshare_add(program);
        diff->undo.args.type_setm.etm2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (editor_program_transition_from_current_get(edit_object, program))
      new_val = new_val | EDJE_TWEEN_MODE_OPT_FROM_CURRENT;
   if (!edje_edit_program_transition_set(edit_object, program, new_val))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_transition_from_current_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                                           const char *program, Eina_Bool new_val)
{
   Diff *diff;
   Edje_Tween_Mode mode;
   Attribute attribute = ATTRIBUTE_PROGRAM_TRANSITION_FROM_CURRENT;

   assert(edit_object != NULL);
   assert(program != NULL);

   if (change)
     {
        Eina_Bool old_value = editor_program_transition_from_current_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_BOOL;
        diff->redo.function = editor_program_transition_from_current_set;
        diff->redo.args.type_sb.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sb.b2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_BOOL;
        diff->undo.function = editor_program_transition_from_current_set;
        diff->undo.args.type_sb.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sb.b2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   mode = editor_program_transition_type_get(edit_object, program);
   if (new_val)
     mode = mode | EDJE_TWEEN_MODE_OPT_FROM_CURRENT;
   if (!edje_edit_program_transition_set(edit_object, program, mode))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_action_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program, Edje_Action_Type new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_ACTION;
   Eina_Bool clean_targets = false;
   Eina_List *targets, *l;
   Eina_Stringshare *target;

   assert(edit_object != NULL);
   assert(program != NULL);

   if (change)
     {
        Edje_Action_Type old_value = edje_edit_program_action_get(edit_object, program);
        switch (new_val)
          {
           case EDJE_ACTION_TYPE_STATE_SET:
           case EDJE_ACTION_TYPE_SIGNAL_EMIT:
           case EDJE_ACTION_TYPE_DRAG_VAL_SET:
           case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
           case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
           case EDJE_ACTION_TYPE_FOCUS_SET:
           case EDJE_ACTION_TYPE_FOCUS_OBJECT:
              /*These actions have part as a target so targets list can be left untouched
                if it was not list of programs (EDJE_ACTION_TYPE_ACTION_STOP) */
              if (old_value == EDJE_ACTION_TYPE_ACTION_STOP)
                clean_targets = true;
           case EDJE_ACTION_TYPE_ACTION_STOP:
              /*this action uses programs as targets*/
           default:
              /*other actions do not need targets so we need to delete them all */
              clean_targets = true;
          }
        if (clean_targets)
          {
             targets = edje_edit_program_targets_get(edit_object, program);
             EINA_LIST_FOREACH(targets, l, target)
                editor_program_target_del(edit_object, change, false, program, target);
             edje_edit_string_list_free(targets);
          }

        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEACTIONTYPE;
        diff->redo.function = editor_program_action_set;
        diff->redo.args.type_seat.s1 = eina_stringshare_add(program);
        diff->redo.args.type_seat.eat2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJEACTIONTYPE;
        diff->undo.function = editor_program_action_set;
        diff->undo.args.type_seat.s1 = eina_stringshare_add(program);
        diff->undo.args.type_seat.eat2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_action_set(edit_object, program, new_val))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_channel_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                           const char *program, Edje_Channel new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_CHANNEL;
   assert(edit_object != NULL);
   assert(program != NULL);
   if (change)
     {
        Edje_Channel old_value = edje_edit_program_channel_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJECHANNEL;
        diff->redo.function = editor_program_channel_set;
        diff->redo.args.type_sec.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sec.ec2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJECHANNEL;
        diff->undo.function = editor_program_channel_set;
        diff->undo.args.type_sec.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sec.ec2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_channel_set(edit_object, program, new_val))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

EDITOR_PROGRAM_DOUBLE(tone_duration, ATTRIBUTE_PROGRAM_TONE_DURATION);
EDITOR_PROGRAM_DOUBLE(in_from, ATTRIBUTE_PROGRAM_IN_FROM);
EDITOR_PROGRAM_DOUBLE(in_range, ATTRIBUTE_PROGRAM_IN_RANGE);
EDITOR_PROGRAM_DOUBLE(transition_time, ATTRIBUTE_PROGRAM_TRANSITION_TIME);
EDITOR_PROGRAM_DOUBLE(sample_speed, ATTRIBUTE_PROGRAM_SAMPLE_SPEED);
EDITOR_PROGRAM_DOUBLE(value2, ATTRIBUTE_PROGRAM_VALUE2);
EDITOR_PROGRAM_DOUBLE(value, ATTRIBUTE_PROGRAM_VALUE);
EDITOR_PROGRAM_DOUBLE(transition_value1, ATTRIBUTE_PROGRAM_TRANSITION_VALUE1);
EDITOR_PROGRAM_DOUBLE(transition_value2, ATTRIBUTE_PROGRAM_TRANSITION_VALUE2);
EDITOR_PROGRAM_DOUBLE(transition_value3, ATTRIBUTE_PROGRAM_TRANSITION_VALUE3);
EDITOR_PROGRAM_DOUBLE(transition_value4, ATTRIBUTE_PROGRAM_TRANSITION_VALUE4);

EDITOR_PROGRAM_STRING(filter_part, ATTRIBUTE_PROGRAM_FILTER_PART);
EDITOR_PROGRAM_STRING(filter_state, ATTRIBUTE_PROGRAM_FILTER_STATE);
EDITOR_PROGRAM_STRING(api_name, ATTRIBUTE_PROGRAM_API_NAME);
EDITOR_PROGRAM_STRING(api_description, ATTRIBUTE_PROGRAM_API_DESCRIPTION);
EDITOR_PROGRAM_STRING(sample_name, ATTRIBUTE_PROGRAM_SAMPLE_NAME);
EDITOR_PROGRAM_STRING(tone_name, ATTRIBUTE_PROGRAM_TONE_NAME);
EDITOR_PROGRAM_STRING(signal, ATTRIBUTE_PROGRAM_SIGNAL);
EDITOR_PROGRAM_STRING(source, ATTRIBUTE_PROGRAM_SOURCE);
EDITOR_PROGRAM_STRING(state, ATTRIBUTE_PROGRAM_STATE);
EDITOR_PROGRAM_STRING(state2, ATTRIBUTE_PROGRAM_STATE2);

Eina_Bool
editor_program_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                        const char *name, const char *new_val)
{
   Diff *diff;
   Rename ren;
   Attribute attribute = ATTRIBUTE_PROGRAM_NAME;
   assert(edit_object != NULL);
   assert(name != NULL);
   assert(new_val != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_program_name_set;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_program_name_set;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(new_val);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(name);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_name_set(edit_object, name, new_val))
     return false;
   _editor_project_changed();
   ren.old_name = name;
   ren.new_name = new_val;
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_PROGRAM_RENAMED, &ren);
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_target_add(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program_name, Eina_Stringshare *target)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_TARGET;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   assert(target != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_program_target_add;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(target);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_program_target_del;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(target);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_target_add(edit_object, program_name, target))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_target_del(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                          const char *program_name, Eina_Stringshare *target)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_TARGET;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_program_target_del;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(target);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_program_target_add;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(target);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_target_del(edit_object, program_name, target))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}
