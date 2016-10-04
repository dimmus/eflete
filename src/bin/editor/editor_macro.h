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

#define EDITOR_STATE_DOUBLE(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, double new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        double old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssdd.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdd.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdd.d3 = state_val; \
        diff->redo.args.type_ssdd.d4 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssdd.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdd.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdd.d3 = state_val; \
        diff->undo.args.type_ssdd.d4 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       _editor_project_changed(); \
        CRIT_ON_FAIL(editor_save(edit_object)); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STATE_BOOL(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, Eina_Bool new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        Eina_Bool old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_BOOL; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssdb.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdb.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdb.d3 = state_val; \
        diff->redo.args.type_ssdb.b4 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_BOOL; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssdb.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdb.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdb.d3 = state_val; \
        diff->undo.args.type_ssdb.b4 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       _editor_project_changed(); \
       CRIT_ON_FAIL(editor_save(edit_object)); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STATE_STRING(FUNC, RM_ATTRIBUTE, SAVE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, const char *new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   Eina_Stringshare *old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
   send.attribute = RM_ATTRIBUTE; \
   send.part_name = eina_stringshare_add(part_name); \
   send.state_name = eina_stringshare_add(state_name); \
   send.state_value = state_val; \
   send.old_value = eina_stringshare_add(old_value); \
   send.value = eina_stringshare_add(new_val); \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssds.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssds.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssds.d3 = state_val; \
        diff->redo.args.type_ssds.s4 = eina_stringshare_add(new_val); \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssds.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssds.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssds.d3 = state_val; \
        diff->undo.args.type_ssds.s4 = old_value; /* assuming that getter returned stringshare */\
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       if (SAVE) CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   eina_stringshare_del(send.part_name); \
   eina_stringshare_del(send.state_name); \
   eina_stringshare_del(send.old_value); \
   eina_stringshare_del(send.value); \
   return true; \
}

#define EDITOR_STATE_STRING_WITH_RESET(FUNC, RM_ATTRIBUTE, SAVE, RESET) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, const char *new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   Eina_Stringshare *old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
   send.attribute = RM_ATTRIBUTE; \
   send.part_name = eina_stringshare_add(part_name); \
   send.state_name = eina_stringshare_add(state_name); \
   send.state_value = state_val; \
   send.old_value = eina_stringshare_add(old_value); \
   send.value = eina_stringshare_add(new_val); \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssds.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssds.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssds.d3 = state_val; \
        diff->redo.args.type_ssds.s4 = eina_stringshare_add(new_val); \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssds.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssds.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssds.d3 = state_val; \
        diff->undo.args.type_ssds.s4 = old_value; /* assuming that getter returned stringshare */\
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
   { \
      CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
      if (SAVE) CRIT_ON_FAIL(editor_save(edit_object)); \
      _editor_project_changed(); \
      if (!new_val) editor_state_## RESET ##_reset(edit_object, change, apply, part_name, state_name, state_val); \
      if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
   } \
   eina_stringshare_del(send.part_name); \
   eina_stringshare_del(send.state_name); \
   eina_stringshare_del(send.old_value); \
   eina_stringshare_del(send.value); \
   return true; \
}

#define EDITOR_STATE_STRING_WITH_FALLBACK(FUNC, RM_ATTRIBUTE, FALLBACK_VAL, SAVE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, const char *new_val) \
{ \
   Diff *diff = NULL; \
   Eina_Stringshare *old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   send.part_name = eina_stringshare_add(part_name); \
   send.state_name = eina_stringshare_add(state_name); \
   send.state_value = state_val; \
   send.old_value = eina_stringshare_add(old_value); \
   send.value = eina_stringshare_add(new_val); \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssds.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssds.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssds.d3 = state_val; \
        diff->redo.args.type_ssds.s4 = eina_stringshare_add(new_val); \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssds.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssds.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssds.d3 = state_val; \
        diff->undo.args.type_ssds.s4 = old_value; /* assuming that getter returned stringshare */\
     } \
   if (apply) \
     { \
       if (!edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)) \
         { \
            TODO("i'm not sure that fallback is setted correctly, need to check") \
            CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, FALLBACK_VAL)); \
            if (diff) \
              { \
                eina_stringshare_del(diff->redo.args.type_ssds.s4); \
                diff->redo.args.type_ssds.s4 = eina_stringshare_add(FALLBACK_VAL); \
              } \
         } \
       if (change) /* we should add diff only after all changes to it */\
         { \
            if (merge) \
              change_diff_merge_add(change, diff); \
            else \
              change_diff_add(change, diff); \
         } \
       if (SAVE) CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   else \
     { \
       if (change) /* we should add diff only after all changes to it */\
         { \
            if (merge) \
              change_diff_merge_add(change, diff); \
            else \
              change_diff_add(change, diff); \
         } \
     } \
   eina_stringshare_del(send.part_name); \
   eina_stringshare_del(send.state_name); \
   eina_stringshare_del(send.value); \
   eina_stringshare_del(send.old_value); \
   return true; \
}

#define EDITOR_STATE_INT_INT_INT_INT(FUNC, REAL_FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, \
                            int n4, int n5, int n6, int n7) \
{ \
   Diff *diff; \
   int o4, o5, o6, o7; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val, &o4, &o5, &o6, &o7); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssdiiii.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdiiii.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdiiii.d3 = state_val; \
        diff->redo.args.type_ssdiiii.i4 = n4; \
        diff->redo.args.type_ssdiiii.i5 = n5; \
        diff->redo.args.type_ssdiiii.i6 = n6; \
        diff->redo.args.type_ssdiiii.i7 = n7; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssdiiii.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdiiii.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdiiii.d3 = state_val; \
        diff->undo.args.type_ssdiiii.i4 = o4; \
        diff->undo.args.type_ssdiiii.i5 = o5; \
        diff->undo.args.type_ssdiiii.i6 = o6; \
        diff->undo.args.type_ssdiiii.i7 = o7; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## REAL_FUNC ##_set(edit_object, part_name, state_name, state_val, n4, n5, n6, n7)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STATE_INT_INT_INT_INT_INT(FUNC, REAL_FUNC, RM_ATTRIBUTE, NUMBER) \
Eina_Bool \
editor_state_## FUNC ##_## NUMBER ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                                 const char *part_name, const char *state_name, double state_val, int n4, int n5, int n6, int n7) \
{ \
   Diff *diff; \
   int o4, o5, o6, o7; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE ##_## NUMBER; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        edje_edit_state_## REAL_FUNC ##_get(edit_object, part_name, state_name, state_val, NUMBER - 1, &o4, &o5, &o6, &o7); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT; \
        diff->redo.function = editor_state_## FUNC ##_## NUMBER ##_set; \
        diff->redo.args.type_ssdiiii.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdiiii.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdiiii.d3 = state_val; \
        diff->redo.args.type_ssdiiii.i4 = n4; \
        diff->redo.args.type_ssdiiii.i5 = n5; \
        diff->redo.args.type_ssdiiii.i6 = n6; \
        diff->redo.args.type_ssdiiii.i7 = n7; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT; \
        diff->undo.function = editor_state_## FUNC ##_## NUMBER ##_set; \
        diff->undo.args.type_ssdiiii.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdiiii.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdiiii.d3 = state_val; \
        diff->undo.args.type_ssdiiii.i4 = o4; \
        diff->undo.args.type_ssdiiii.i5 = o5; \
        diff->undo.args.type_ssdiiii.i6 = o6; \
        diff->undo.args.type_ssdiiii.i7 = o7; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
        CRIT_ON_FAIL(edje_edit_state_## REAL_FUNC ##_set(edit_object, part_name, state_name, state_val, NUMBER - 1, n4, n5, n6, n7)); \
        _editor_project_changed(); \
        CRIT_ON_FAIL(editor_save(edit_object)); \
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STATE_UCHAR(FUNC, RM_ATTRIBUTE, SAVE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, unsigned char new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        unsigned char old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_UCHAR; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssduc.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssduc.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssduc.d3 = state_val; \
        diff->redo.args.type_ssduc.uc4 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_UCHAR; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssduc.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssduc.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssduc.d3 = state_val; \
        diff->undo.args.type_ssduc.uc4 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       if (SAVE) CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_PART_BOOL(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_part_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                           const char *part_name, Eina_Bool new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   if (change) \
     { \
        Eina_Bool old_value = edje_edit_part_## FUNC ##_get(edit_object, part_name); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_BOOL; \
        diff->redo.function = editor_part_## FUNC ##_set; \
        diff->redo.args.type_sb.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_sb.b2 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_BOOL; \
        diff->undo.function = editor_part_## FUNC ##_set; \
        diff->undo.args.type_sb.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_sb.b2 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_part_## FUNC ##_set(edit_object, part_name, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_PART_INT(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_part_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                           const char *part_name, int new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   if (change) \
     { \
        int old_value = edje_edit_part_## FUNC ##_get(edit_object, part_name); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_INT; \
        diff->redo.function = editor_part_## FUNC ##_set; \
        diff->redo.args.type_si.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_si.i2 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_INT; \
        diff->undo.function = editor_part_## FUNC ##_set; \
        diff->undo.args.type_si.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_si.i2 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_part_## FUNC ##_set(edit_object, part_name, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STRING_STRING(FUNC, REAL_FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                      const char *name, const char *new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(name != NULL); \
   Eina_Stringshare *old_value = edje_edit_## REAL_FUNC ##_get(edit_object, name); \
   send.part_name = eina_stringshare_add(name); \
   send.value = eina_stringshare_add(new_val); \
   send.old_value = eina_stringshare_add(old_value); \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING; \
        diff->redo.function = editor_## FUNC ##_set; \
        diff->redo.args.type_ssds.s1 = eina_stringshare_add(name); \
        diff->redo.args.type_ssds.s2 = eina_stringshare_add(new_val); \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING; \
        diff->undo.function = editor_## FUNC ##_set; \
        diff->undo.args.type_ssds.s1 = eina_stringshare_add(name); \
        diff->undo.args.type_ssds.s2 = old_value; /* assuming that getter returned stringshare */\
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_## REAL_FUNC ##_set(edit_object, name, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   eina_stringshare_del(send.part_name); \
   eina_stringshare_del(send.value); \
   eina_stringshare_del(send.old_value); \
   return true; \
}

#define EDITOR_PART_ITEM_INDEX_DOUBLE(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_part_item_index_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                                     const char *part_name, unsigned int index, double new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   if (change) \
     { \
        double old_value = edje_edit_part_item_index_## FUNC ##_get(edit_object, part_name, index); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_UINT_DOUBLE; \
        diff->redo.function = editor_part_item_index_## FUNC ##_set; \
        diff->redo.args.type_suid.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_suid.ui2 = index; \
        diff->redo.args.type_suid.d3 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_UINT_DOUBLE; \
        diff->undo.function = editor_part_item_index_## FUNC ##_set; \
        diff->undo.args.type_suid.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_suid.ui2 = index; \
        diff->undo.args.type_suid.d3 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_part_item_index_## FUNC ##_set(edit_object, part_name, index, new_val)); \
       CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_PART_ITEM_INDEX_INT(FUNC, RM_ATTRIBUTE, SAVE) \
Eina_Bool \
editor_part_item_index_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                                      const char *part_name, unsigned int index, int new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   if (change) \
     { \
        int old_value = edje_edit_part_item_index_## FUNC ##_get(edit_object, part_name, index); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_UINT_INT; \
        diff->redo.function = editor_part_item_index_## FUNC ##_set; \
        diff->redo.args.type_suii.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_suii.ui2 = index; \
        diff->redo.args.type_suii.i3 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_UINT_INT; \
        diff->undo.function = editor_part_item_index_## FUNC ##_set; \
        diff->undo.args.type_suii.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_suii.ui2 = index; \
        diff->undo.args.type_suii.i3 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_part_item_index_## FUNC ##_set(edit_object, part_name, index, new_val)); \
       if (SAVE) CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
     } \
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
   return true; \
}

#define EDITOR_PART_ITEM_INDEX_USHORT(FUNC, RM_ATTRIBUTE, SAVE) \
Eina_Bool \
editor_part_item_index_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                                      const char *part_name, unsigned int index, unsigned short new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   if (change) \
     { \
        unsigned short old_value = edje_edit_part_item_index_## FUNC ##_get(edit_object, part_name, index); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_UINT_USHORT; \
        diff->redo.function = editor_part_item_index_## FUNC ##_set; \
        diff->redo.args.type_suius.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_suius.ui2 = index; \
        diff->redo.args.type_suius.us3 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_UINT_USHORT; \
        diff->undo.function = editor_part_item_index_## FUNC ##_set; \
        diff->undo.args.type_suius.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_suius.ui2 = index; \
        diff->undo.args.type_suius.us3 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_part_item_index_## FUNC ##_set(edit_object, part_name, index, new_val)); \
       if (SAVE) CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STATE_INT(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, int new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        int old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssdi.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdi.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdi.d3 = state_val; \
        diff->redo.args.type_ssdi.i4 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssdi.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdi.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdi.d3 = state_val; \
        diff->undo.args.type_ssdi.i4 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

TODO("delete macro _SAVE after fix issue in the edje_edit")
#define EDITOR_STATE_INT_SAVE(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, int new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        int old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssdi.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdi.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdi.d3 = state_val; \
        diff->redo.args.type_ssdi.i4 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_INT; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssdi.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdi.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdi.d3 = state_val; \
        diff->undo.args.type_ssdi.i4 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_STATE_DOUBLE_SAVE(FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_state_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                            const char *part_name, const char *state_name, double state_val, double new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(part_name != NULL); \
   assert(state_name != NULL); \
   if (change) \
     { \
        double old_value = edje_edit_state_## FUNC ##_get(edit_object, part_name, state_name, state_val); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE; \
        diff->redo.function = editor_state_## FUNC ##_set; \
        diff->redo.args.type_ssdd.s1 = eina_stringshare_add(part_name); \
        diff->redo.args.type_ssdd.s2 = eina_stringshare_add(state_name); \
        diff->redo.args.type_ssdd.d3 = state_val; \
        diff->redo.args.type_ssdd.d4 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE; \
        diff->undo.function = editor_state_## FUNC ##_set; \
        diff->undo.args.type_ssdd.s1 = eina_stringshare_add(part_name); \
        diff->undo.args.type_ssdd.s2 = eina_stringshare_add(state_name); \
        diff->undo.args.type_ssdd.d3 = state_val; \
        diff->undo.args.type_ssdd.d4 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_state_## FUNC ##_set(edit_object, part_name, state_name, state_val, new_val)); \
       CRIT_ON_FAIL(editor_save(edit_object)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_PROGRAM_DOUBLE(FUNC, REAL_FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_program_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                           const char *program, double new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   send.attribute = RM_ATTRIBUTE; \
   assert(edit_object != NULL); \
   assert(program != NULL); \
   if (change) \
     { \
        double old_value = edje_edit_program_## FUNC ##_get(edit_object, program); \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_DOUBLE; \
        diff->redo.function = editor_program_## FUNC ##_set; \
        diff->redo.args.type_sd.s1 = eina_stringshare_add(program); \
        diff->redo.args.type_sd.d2 = new_val; \
        diff->undo.type = FUNCTION_TYPE_STRING_DOUBLE; \
        diff->undo.function = editor_program_## FUNC ##_set; \
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program); \
        diff->undo.args.type_sd.d2 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_program_## REAL_FUNC ##_set(edit_object, program, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
     } \
   return true; \
}

#define EDITOR_PROGRAM_STRING(FUNC, REAL_FUNC, RM_ATTRIBUTE) \
Eina_Bool \
editor_program_## FUNC ##_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply, \
                           const char *program, const char *new_val) \
{ \
   Diff *diff; \
   Editor_Attribute_Change send; \
   send.edit_object = edit_object; \
 \
   assert(edit_object != NULL); \
   assert(program != NULL); \
   Eina_Stringshare *old_value = edje_edit_program_## FUNC ##_get(edit_object, program); \
   send.attribute = RM_ATTRIBUTE; \
   send.program_name = eina_stringshare_add(program); \
   send.old_value = eina_stringshare_add(old_value); \
   send.value = eina_stringshare_add(new_val); \
   if (change) \
     { \
        diff = mem_calloc(1, sizeof(Diff)); \
        diff->redo.type = FUNCTION_TYPE_STRING_STRING; \
        diff->redo.function = editor_program_## FUNC ##_set; \
        diff->redo.args.type_ss.s1 = eina_stringshare_add(program); \
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val); \
        diff->undo.type = FUNCTION_TYPE_STRING_STRING; \
        diff->undo.function = editor_program_## FUNC ##_set; \
        diff->undo.args.type_sd.s1 = eina_stringshare_add(program); \
        diff->undo.args.type_ss.s2 = old_value; \
        if (merge) \
          change_diff_merge_add(change, diff); \
        else \
          change_diff_add(change, diff); \
     } \
   if (apply) \
     { \
       CRIT_ON_FAIL(edje_edit_program_## REAL_FUNC ##_set(edit_object, program, new_val)); \
       _editor_project_changed(); \
       if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, &send); \
       evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PROGRAM_UPDATE, (void *)program); \
     } \
   eina_stringshare_del(send.program_name); \
   eina_stringshare_del(send.old_value); \
   eina_stringshare_del(send.value); \
   return true; \
}
