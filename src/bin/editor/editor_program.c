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
editor_program_transition_type_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                   const char *program, Edje_Tween_Mode new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_TYPE;

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
   if (apply)
     {
        if (editor_program_transition_from_current_get(edit_object, program))
          new_val = new_val | EDJE_TWEEN_MODE_OPT_FROM_CURRENT;
        CRIT_ON_FAIL(edje_edit_program_transition_set(edit_object, program, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

Eina_Bool
editor_program_transition_from_current_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                           const char *program, Eina_Bool new_val)
{
   Diff *diff;
   Edje_Tween_Mode mode;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_FROM_CURRENT;

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
   if (apply)
     {
        mode = editor_program_transition_type_get(edit_object, program);
        if (new_val)
          mode = mode | EDJE_TWEEN_MODE_OPT_FROM_CURRENT;
        CRIT_ON_FAIL(edje_edit_program_transition_set(edit_object, program, mode));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

Eina_Bool
editor_program_channel_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *program, Edje_Channel new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_CHANNEL;
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
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_channel_set(edit_object, program, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

EDITOR_PROGRAM_DOUBLE(tone_duration, tone_duration, RM_ATTRIBUTE_PROGRAM_TONE_DURATION);
EDITOR_PROGRAM_DOUBLE(in_from, in_from, RM_ATTRIBUTE_PROGRAM_IN_FROM);
EDITOR_PROGRAM_DOUBLE(in_range, in_range, RM_ATTRIBUTE_PROGRAM_IN_RANGE);
EDITOR_PROGRAM_DOUBLE(transition_time, transition_time, RM_ATTRIBUTE_PROGRAM_TRANSITION_TIME);
EDITOR_PROGRAM_DOUBLE(sample_speed, sample_speed, RM_ATTRIBUTE_PROGRAM_SAMPLE_SPEED);

double
editor_program_transition_factor_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   TODO("Revert this commit after fix genlist filter in properties being not called before update callback");
   /*
   assert((type == EDJE_TWEEN_MODE_ACCELERATE_FACTOR) ||
          (type == EDJE_TWEEN_MODE_DECELERATE_FACTOR) ||
          (type == EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR) ||
          (type == EDJE_TWEEN_MODE_DIVISOR_INTERP));
          */

   if (type == EDJE_TWEEN_MODE_DIVISOR_INTERP)
     return edje_edit_program_transition_value2_get(edit_object, program);
   else
     return edje_edit_program_transition_value1_get(edit_object, program);
}
Eina_Bool
editor_program_transition_factor_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_FACTOR;
   assert(edit_object != NULL);
   assert(program != NULL);

   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert((type == EDJE_TWEEN_MODE_ACCELERATE_FACTOR) ||
          (type == EDJE_TWEEN_MODE_DECELERATE_FACTOR) ||
          (type == EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR) ||
          (type == EDJE_TWEEN_MODE_DIVISOR_INTERP));
   if (change)
     {
        double old_value = editor_program_transition_factor_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->redo.function = editor_program_transition_factor_set;
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sd.d2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->undo.function = editor_program_transition_factor_set;
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sd.d2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (type == EDJE_TWEEN_MODE_DIVISOR_INTERP)
          {
             CRIT_ON_FAIL(edje_edit_program_transition_value2_set(edit_object, program, new_val));
          }
        else
          {
             CRIT_ON_FAIL(edje_edit_program_transition_value1_set(edit_object, program, new_val));
          }

        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

double
editor_program_transition_gradient_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   /*
   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert(type == EDJE_TWEEN_MODE_DIVISOR_INTERP);
   */

   return edje_edit_program_transition_value1_get(edit_object, program);
}
Eina_Bool
editor_program_transition_gradient_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                       const char *program, double new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_GRADIENT;
   assert(edit_object != NULL);
   assert(program != NULL);

   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert(type == EDJE_TWEEN_MODE_DIVISOR_INTERP);
   if (change)
     {
        double old_value = editor_program_transition_gradient_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->redo.function = editor_program_transition_gradient_set;
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sd.d2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->undo.function = editor_program_transition_gradient_set;
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sd.d2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_transition_value1_set(edit_object, program, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

double
editor_program_transition_decay_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   /*
   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert((type == EDJE_TWEEN_MODE_BOUNCE) ||
          (type == EDJE_TWEEN_MODE_SPRING));
   */

   return edje_edit_program_transition_value1_get(edit_object, program);
}
Eina_Bool
editor_program_transition_decay_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                    const char *program, double new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_DECAY;
   assert(edit_object != NULL);
   assert(program != NULL);

   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert((type == EDJE_TWEEN_MODE_BOUNCE) ||
          (type == EDJE_TWEEN_MODE_SPRING));
   if (change)
     {
        double old_value = editor_program_transition_decay_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->redo.function = editor_program_transition_decay_set;
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sd.d2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->undo.function = editor_program_transition_decay_set;
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sd.d2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_transition_value1_set(edit_object, program, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

double
editor_program_transition_bounces_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   /*
   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert(type == EDJE_TWEEN_MODE_BOUNCE);
   */

   return edje_edit_program_transition_value2_get(edit_object, program);
}
Eina_Bool
editor_program_transition_bounces_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                      const char *program, double new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_BOUNCES;
   assert(edit_object != NULL);
   assert(program != NULL);

   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert(type == EDJE_TWEEN_MODE_BOUNCE);
   if (change)
     {
        double old_value = editor_program_transition_bounces_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->redo.function = editor_program_transition_bounces_set;
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sd.d2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->undo.function = editor_program_transition_bounces_set;
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sd.d2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_transition_value2_set(edit_object, program, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

double
editor_program_transition_swings_get(Evas_Object *edit_object, const char *program)
{
   assert(edit_object != NULL);
   assert(program != NULL);

   /*
   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert(type == EDJE_TWEEN_MODE_SPRING);
   */

   return edje_edit_program_transition_value2_get(edit_object, program);
}
Eina_Bool
editor_program_transition_swings_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                     const char *program, double new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TRANSITION_SWINGS;
   assert(edit_object != NULL);
   assert(program != NULL);

   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program);
   assert(type == EDJE_TWEEN_MODE_SPRING);
   if (change)
     {
        double old_value = editor_program_transition_swings_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->redo.function = editor_program_transition_swings_set;
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->redo.args.type_sd.d2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE;
        diff->undo.function = editor_program_transition_swings_set;
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->undo.args.type_sd.d2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_transition_value2_set(edit_object, program, new_val));
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

#define EDITOR_PROGRAM_TRANSITION_BEZIER(VAL, REAL_VAL, RM_ATTRIBUTE) \
double \
editor_program_transition_bezier_## VAL ##_get(Evas_Object *edit_object, const char *program) \
{ \
   assert(edit_object != NULL); \
   assert(program != NULL); \
 \
   /*
   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program); \
   assert(type == EDJE_TWEEN_MODE_CUBIC_BEZIER); \
   */ \
 \
   return edje_edit_program_transition_value## REAL_VAL ##_get(edit_object, program); \
} \
Eina_Bool \
editor_program_transition_bezier_## VAL ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                                               const char *program, double new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(program != NULL); \
 \
   Edje_Tween_Mode type = editor_program_transition_type_get(edit_object, program); \
   assert(type == EDJE_TWEEN_MODE_CUBIC_BEZIER); \
   if (change) \
     { \
        double old_value = editor_program_transition_bezier_## VAL ##_get(edit_object, program); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE; \
        diff->redo.function = editor_program_transition_bezier_## VAL ##_set; \
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program); \
        diff->redo.args.type_sd.d2 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE; \
        diff->undo.function = editor_program_transition_bezier_## VAL ##_set; \
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program); \
        diff->undo.args.type_sd.d2 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_program_transition_value## REAL_VAL##_set(edit_object, program, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

EDITOR_PROGRAM_TRANSITION_BEZIER(x1, 1, RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_X1);
EDITOR_PROGRAM_TRANSITION_BEZIER(y1, 2, RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_Y1);
EDITOR_PROGRAM_TRANSITION_BEZIER(x2, 3, RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_X2);
EDITOR_PROGRAM_TRANSITION_BEZIER(y2, 4, RM_ATTRIBUTE_PROGRAM_TRANSITION_BEZIER_Y2);

EDITOR_PROGRAM_STRING(filter_part, filter_part, RM_ATTRIBUTE_PROGRAM_FILTER_PART);
EDITOR_PROGRAM_STRING(filter_state, filter_state, RM_ATTRIBUTE_PROGRAM_FILTER_STATE);
EDITOR_PROGRAM_STRING(api_name, api_name, RM_ATTRIBUTE_PROGRAM_API_NAME);
EDITOR_PROGRAM_STRING(api_description, api_description, RM_ATTRIBUTE_PROGRAM_API_DESCRIPTION);
EDITOR_PROGRAM_STRING(sample_name, sample_name, RM_ATTRIBUTE_PROGRAM_SAMPLE_NAME);
EDITOR_PROGRAM_STRING(tone_name, tone_name, RM_ATTRIBUTE_PROGRAM_TONE_NAME);
EDITOR_PROGRAM_STRING(signal, signal, RM_ATTRIBUTE_PROGRAM_SIGNAL);
EDITOR_PROGRAM_STRING(source, source, RM_ATTRIBUTE_PROGRAM_SOURCE);

EDITOR_PROGRAM_STRING(state, state, RM_ATTRIBUTE_PROGRAM_STATE);
EDITOR_PROGRAM_DOUBLE(value, value, RM_ATTRIBUTE_PROGRAM_VALUE);

EDITOR_PROGRAM_STRING(emit_source, state2, RM_ATTRIBUTE_PROGRAM_EMIT_SOURCE);

EDITOR_PROGRAM_DOUBLE(drag_value_x, value, RM_ATTRIBUTE_PROGRAM_DRAG_VALUE_X);
EDITOR_PROGRAM_DOUBLE(drag_value_y, value2, RM_ATTRIBUTE_PROGRAM_DRAG_VALUE_Y);

Eina_Bool
editor_program_emit_signal_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                               const char *program, const char *new_val)
{
   Diff *diff;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_EMIT_SIGNAL;
   assert(edit_object != NULL);
   assert(program != NULL);
   if (change)
     {
        Eina_Stringshare *old_value = edje_edit_program_emit_signal_get(edit_object, program);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_program_emit_signal_set;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(program);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_program_emit_signal_set;
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program);
        diff->undo.args.type_ss.s2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
       CRIT_ON_FAIL(edje_edit_program_state_set(edit_object, program, new_val));
       if (!edje_edit_program_emit_source_get(edit_object, program))
         CRIT_ON_FAIL(edje_edit_program_state2_set(edit_object, program, ""));
       _editor_project_changed();
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
       evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PROGRAM_UPDATE, (void *)program);
     }
   return true;
}

Eina_Bool
editor_program_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                        const char *name, const char *new_val)
{
   Diff *diff;
   Rename ren;
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_NAME;
   assert(edit_object != NULL);
   assert(name != NULL);
   assert(new_val != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_RENAME;
        diff->redo.function = editor_program_name_set;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_RENAME;
        diff->undo.function = editor_program_name_set;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(new_val);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(name);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_name_set(edit_object, name, new_val));
        _editor_project_changed();
        ren.old_name = name;
        ren.new_name = new_val;
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_PROGRAM_RENAMED, &ren);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   return true;
}

Eina_Bool
editor_program_target_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                          const char *program_name, Eina_Stringshare *target)
{
   Diff *diff;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   assert(target != NULL);
   Editor_Attribute_Change send;
   send.attribute = RM_ATTRIBUTE_PROGRAM_TARGET;
   send.program_name = eina_stringshare_add(program_name);
   send.value = eina_stringshare_add(target);
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
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_target_add(edit_object, program_name, target));
        _editor_project_changed();
        if (!_editor_signals_blocked)
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   eina_stringshare_del(send.program_name);
   eina_stringshare_del(send.value);
   return true;
}

Eina_Bool
editor_program_target_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                          const char *program_name, Eina_Stringshare *target)
{
   Diff *diff;
   Editor_Attribute_Change send;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   send.attribute = RM_ATTRIBUTE_PROGRAM_TARGET;
   send.program_name = eina_stringshare_add(program_name);
   send.old_value = eina_stringshare_add(target);
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
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_target_del(edit_object, program_name, target));
        _editor_project_changed();
        if (!_editor_signals_blocked)
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   eina_stringshare_del(send.program_name);
   eina_stringshare_del(send.old_value);
   return true;
}

Eina_Bool
editor_program_after_add(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *program_name, Eina_Stringshare *after)
{
   Diff *diff;
   Editor_Attribute_Change send;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   assert(after != NULL);
   send.attribute = RM_ATTRIBUTE_PROGRAM_AFTER;
   send.program_name = eina_stringshare_add(program_name);
   send.value = eina_stringshare_add(after);
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
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_after_add(edit_object, program_name, after));
        _editor_project_changed();
        if (!_editor_signals_blocked)
          evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   eina_stringshare_del(send.program_name);
   eina_stringshare_del(send.value);
   return true;
}

Eina_Bool
editor_program_after_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *program_name, Eina_Stringshare *after)
{
   Diff *diff;
   Editor_Attribute_Change send;
   assert(edit_object != NULL);
   assert(program_name != NULL);
   send.attribute = RM_ATTRIBUTE_PROGRAM_AFTER;
   send.program_name = eina_stringshare_add(program_name);
   send.old_value = eina_stringshare_add(after);
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
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_after_del(edit_object, program_name, after));
        _editor_project_changed();
        if (!_editor_signals_blocked)
          evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send);
     }
   eina_stringshare_del(send.program_name);
   eina_stringshare_del(send.old_value);
   return true;
}

Eina_Bool
editor_program_reset(Evas_Object *edit_object, Change *change, Eina_Bool apply,
                     const char *program_name)
{
   Eina_Bool res = true;
   Eina_List *list, *l;
   Eina_Stringshare *name;
   Edje_Tween_Mode transition_type;
   assert(edit_object != NULL);
   assert(program_name != NULL);

   you_shall_not_pass_editor_signals(change);

   Edje_Action_Type type = edje_edit_program_action_get(edit_object, program_name);

   list = edje_edit_program_targets_get(edit_object, program_name);
   EINA_LIST_REVERSE_FOREACH(list, l, name)
      res = res && editor_program_target_del(edit_object, change, false, apply, program_name, name);
   edje_edit_string_list_free(list);

   list = edje_edit_program_afters_get(edit_object, program_name);
   EINA_LIST_REVERSE_FOREACH(list, l, name)
      res = res && editor_program_after_del(edit_object, change, false, apply, program_name, name);
   edje_edit_string_list_free(list);

   res = res && editor_program_filter_state_reset(edit_object, change, apply, program_name);
   res = res && editor_program_filter_part_reset(edit_object, change, apply, program_name);
   res = res && editor_program_in_from_reset(edit_object, change, apply, program_name);
   res = res && editor_program_in_range_reset(edit_object, change, apply, program_name);
   res = res && editor_program_api_name_reset(edit_object, change, apply, program_name);
   res = res && editor_program_api_description_reset(edit_object, change, apply, program_name);
   res = res && editor_program_signal_reset(edit_object, change, apply, program_name);
   res = res && editor_program_source_reset(edit_object, change, apply, program_name);
   switch (type)
     {
        case EDJE_ACTION_TYPE_STATE_SET:
           transition_type = editor_program_transition_type_get(edit_object, program_name);

           res = res && editor_program_state_reset(edit_object, change, apply, program_name);
           res = res && editor_program_value_reset(edit_object, change, apply, program_name);
           res = res && editor_program_transition_time_reset(edit_object, change, apply, program_name);
           switch(transition_type)
             {
              case EDJE_TWEEN_MODE_ACCELERATE_FACTOR:
              case EDJE_TWEEN_MODE_DECELERATE_FACTOR:
              case EDJE_TWEEN_MODE_SINUSOIDAL_FACTOR:
                 res = res && editor_program_transition_factor_reset(edit_object, change, apply, program_name);
                 break;
              case EDJE_TWEEN_MODE_DIVISOR_INTERP:
                 res = res && editor_program_transition_gradient_reset(edit_object, change, apply, program_name);
                 res = res && editor_program_transition_factor_reset(edit_object, change, apply, program_name);
                 break;
              case EDJE_TWEEN_MODE_BOUNCE:
                 res = res && editor_program_transition_decay_reset(edit_object, change, apply, program_name);
                 res = res && editor_program_transition_bounces_reset(edit_object, change, apply, program_name);
                 break;
              case EDJE_TWEEN_MODE_SPRING:
                 res = res && editor_program_transition_decay_reset(edit_object, change, apply, program_name);
                 res = res && editor_program_transition_swings_reset(edit_object, change, apply, program_name);
                 break;
              case EDJE_TWEEN_MODE_CUBIC_BEZIER:
                 res = res && editor_program_transition_bezier_x1_reset(edit_object, change, apply, program_name);
                 res = res && editor_program_transition_bezier_x2_reset(edit_object, change, apply, program_name);
                 res = res && editor_program_transition_bezier_y1_reset(edit_object, change, apply, program_name);
                 res = res && editor_program_transition_bezier_y2_reset(edit_object, change, apply, program_name);
              default:
                 break;
             }

           res = res && editor_program_transition_from_current_reset(edit_object, change, apply, program_name);
           res = res && editor_program_transition_type_reset(edit_object, change, apply, program_name);
           break;
        case EDJE_ACTION_TYPE_SIGNAL_EMIT:
           res = res && editor_program_emit_signal_reset(edit_object, change, apply, program_name);
           res = res && editor_program_emit_source_reset(edit_object, change, apply, program_name);
           break;
        case EDJE_ACTION_TYPE_DRAG_VAL_SET:
        case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
        case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
           res = res && editor_program_drag_value_x_reset(edit_object, change, apply, program_name);
           res = res && editor_program_drag_value_y_reset(edit_object, change, apply, program_name);
           break;
        case EDJE_ACTION_TYPE_SOUND_SAMPLE:
           res = res && editor_program_sample_name_reset(edit_object, change, apply, program_name);
           res = res && editor_program_sample_speed_reset(edit_object, change, apply, program_name);
           res = res && editor_program_channel_reset(edit_object, change, apply, program_name);
           break;
        case EDJE_ACTION_TYPE_SOUND_TONE:
           res = res && editor_program_tone_name_reset(edit_object, change, apply, program_name);
           res = res && editor_program_tone_duration_reset(edit_object, change, apply, program_name);
           break;
        case EDJE_ACTION_TYPE_ACTION_STOP:
        default:
           TODO("Add other action types when they will be supported");
           break;
     }

   you_shall_pass_editor_signals(change);

   return res;
}

Eina_Bool
editor_program_add(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                   const char *program_name, Edje_Action_Type type)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEACTIONTYPE;
        diff->redo.function = editor_program_add;
        diff->redo.args.type_seat.s1 = eina_stringshare_add(program_name);
        diff->redo.args.type_seat.eat2 = type;
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_program_del;
        diff->undo.args.type_s.s1 = eina_stringshare_add(program_name);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_add(edit_object, program_name));
        CRIT_ON_FAIL(edje_edit_program_action_set(edit_object, program_name, type));

        if (type == EDJE_ACTION_TYPE_SOUND_SAMPLE)
          {
             edje_edit_program_sample_name_set(edit_object, program_name, EFLETE_DUMMY_SAMPLE_NAME);
          }

        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();

        event_info = eina_stringshare_add(program_name);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PROGRAM_ADDED, (void *)event_info);
        eina_stringshare_del(event_info);
     }
   return true;
}

Eina_Bool
_editor_program_del(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply, Eina_Bool reset,
                    const char *program_name)
{
   Diff *diff;
   Editor_Program event_info;

   assert(edit_object != NULL);

   event_info.program_name = eina_stringshare_add(program_name);
   event_info.change = change;
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PROGRAM_DELETED, (void *)&event_info);

   if (change)
     {
        Edje_Action_Type type = edje_edit_program_action_get(edit_object, program_name);
        if (reset)
          if (!editor_program_reset(edit_object, change, apply, program_name))
            return false;
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_program_del;
        diff->redo.args.type_s.s1 = eina_stringshare_add(program_name);
        diff->undo.type = FUNCTION_TYPE_STRING_EDJEACTIONTYPE;
        diff->undo.function = editor_program_add;
        diff->undo.args.type_seat.s1 = eina_stringshare_add(program_name);
        diff->undo.args.type_seat.eat2 = type;

        change_diff_add(change, diff);
     }
   if (apply)
     {
        CRIT_ON_FAIL(edje_edit_program_del(edit_object, program_name));
        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();
     }
   eina_stringshare_del(event_info.program_name);
   return true;
}

Eina_Bool
editor_program_del(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                   const char *program_name)
{
   return _editor_program_del(edit_object, change, merge, apply, true, program_name);
}
