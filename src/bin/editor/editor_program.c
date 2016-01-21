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

   return edje_edit_program_transition_get(edit_object, program) & ~(EDJE_TWEEN_MODE_OPT_FROM_CURRENT);
}

Eina_Bool
editor_program_transition_from_current_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   if (edje_edit_program_transition_get(edit_object, program) & EDJE_TWEEN_MODE_OPT_FROM_CURRENT)
     return true;
   else
     return false;
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

Eina_Bool
editor_program_after_add(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                         const char *program_name, Eina_Stringshare *after)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_AFTER;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   assert(after != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_program_after_add;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(after);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_program_after_del;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(after);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_after_add(edit_object, program_name, after))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_after_del(Evas_Object *edit_object, Change *change, Eina_Bool merge,
                         const char *program_name, Eina_Stringshare *after)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PROGRAM_AFTER;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_program_after_del;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(after);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_program_after_add;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(program_name);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(after);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (!edje_edit_program_after_del(edit_object, program_name, after))
     return false;
   _editor_project_changed();
   if (!_editor_signals_blocked)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
   return true;
}

Eina_Bool
editor_program_reset(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__,
                     const char *program_name)
{
   Eina_Bool res = true;
   Eina_List *list, *l;
   Eina_Stringshare *name;
   assert(edit_object != NULL);
   assert(program_name != NULL);

   you_shall_not_pass_editor_signals(change);

   Edje_Action_Type type = edje_edit_program_action_get(edit_object, program_name);

   list = edje_edit_program_targets_get(edit_object, program_name);
   EINA_LIST_FOREACH(list, l, name)
      res = res && editor_program_target_del(edit_object, change, false, program_name, name);
   edje_edit_string_list_free(list);

   list = edje_edit_program_afters_get(edit_object, program_name);
   EINA_LIST_FOREACH(list, l, name)
      res = res && editor_program_after_del(edit_object, change, false, program_name, name);
   edje_edit_string_list_free(list);

   res = res && editor_program_filter_state_reset(edit_object, change, program_name);
   res = res && editor_program_filter_part_reset(edit_object, change, program_name);
   res = res && editor_program_in_from_reset(edit_object, change, program_name);
   res = res && editor_program_in_range_reset(edit_object, change, program_name);
   res = res && editor_program_api_name_reset(edit_object, change, program_name);
   res = res && editor_program_api_description_reset(edit_object, change, program_name);
   res = res && editor_program_signal_reset(edit_object, change, program_name);
   res = res && editor_program_source_reset(edit_object, change, program_name);
   switch (type)
     {
        case EDJE_ACTION_TYPE_STATE_SET:
           res = res && editor_program_state_reset(edit_object, change, program_name);
           res = res && editor_program_value_reset(edit_object, change, program_name);
           res = res && editor_program_transition_time_reset(edit_object, change, program_name);
           res = res && editor_program_transition_value1_reset(edit_object, change, program_name);
           res = res && editor_program_transition_value2_reset(edit_object, change, program_name);
           res = res && editor_program_transition_value3_reset(edit_object, change, program_name);
           res = res && editor_program_transition_value4_reset(edit_object, change, program_name);
           res = res && editor_program_transition_from_current_reset(edit_object, change, program_name);
           res = res && editor_program_transition_type_reset(edit_object, change, program_name);
           break;
        case EDJE_ACTION_TYPE_SIGNAL_EMIT:
           res = res && editor_program_state_reset(edit_object, change, program_name);
           res = res && editor_program_state2_reset(edit_object, change, program_name);
           break;
        case EDJE_ACTION_TYPE_DRAG_VAL_SET:
        case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
        case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
           res = res && editor_program_value_reset(edit_object, change, program_name);
           res = res && editor_program_value2_reset(edit_object, change, program_name);
           break;
        case EDJE_ACTION_TYPE_SOUND_SAMPLE:
           res = res && editor_program_sample_name_reset(edit_object, change, program_name);
           res = res && editor_program_sample_speed_reset(edit_object, change, program_name);
           res = res && editor_program_channel_reset(edit_object, change, program_name);
           break;
        case EDJE_ACTION_TYPE_SOUND_TONE:
           res = res && editor_program_tone_name_reset(edit_object, change, program_name);
           res = res && editor_program_tone_duration_reset(edit_object, change, program_name);
           break;
        case EDJE_ACTION_TYPE_ACTION_STOP:
        default:
           TODO("Add other action types when they will be supported");
           break;
     }
   res = res && editor_program_action_reset(edit_object, change, program_name);

   you_shall_pass_editor_signals(change);

   return res;
}

Eina_Bool
editor_program_add(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__,
                const char *program_name)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_program_add;
        diff->redo.args.type_s.s1 = eina_stringshare_add(program_name);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_program_del;
        diff->undo.args.type_s.s1 = eina_stringshare_add(program_name);

        change_diff_add(change, diff);
     }
   if (!edje_edit_program_add(edit_object, program_name))
     return false;

   editor_save(edit_object);
   _editor_project_changed();

   event_info = eina_stringshare_add(program_name);
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PROGRAM_ADDED, (void *)event_info);
   eina_stringshare_del(event_info);
   return true;
}

Eina_Bool
editor_program_del(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__,
                const char *program_name)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);

   event_info = eina_stringshare_add(program_name);
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PROGRAM_DELETED, (void *)event_info);

   if (change)
     {
        if (!editor_program_reset(edit_object, change, false, program_name))
          return false;
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_program_del;
        diff->redo.args.type_s.s1 = eina_stringshare_add(program_name);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_program_add;
        diff->undo.args.type_s.s1 = eina_stringshare_add(program_name);

        change_diff_add(change, diff);
     }
   if (!edje_edit_program_del(edit_object, program_name))
     {
        eina_stringshare_del(event_info);
        return false;
     }
   eina_stringshare_del(event_info);
   editor_save(edit_object);
   _editor_project_changed();
   return true;
}
