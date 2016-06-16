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
#include "editor_macro.h"
#include "change.h"
#include "diff.h"

extern int _editor_signals_blocked;
Eina_Bool
editor_part_text_effect_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *part_name, Edje_Text_Effect new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_TEXT_EFFECT;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(new_val < EDJE_TEXT_EFFECT_LAST);

   if (change)
     {
        Edje_Text_Effect old_value = edje_edit_part_text_effect_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJETEXTEFFECT;
        diff->redo.function = editor_part_text_effect_set;
        diff->redo.args.type_sete.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sete.ete2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJETEXTEFFECT;
        diff->undo.function = editor_part_text_effect_set;
        diff->undo.args.type_sete.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sete.ete2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_effect_set(edit_object, part_name, (new_val | edje_edit_part_text_shadow_direction_get(edit_object, part_name))))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_text_shadow_direction_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                      const char *part_name, Edje_Text_Effect new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_TEXT_SHADOW_DIRECTION;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert((new_val & EDJE_TEXT_EFFECT_MASK_BASIC) == 0);

   if (change)
     {
        Edje_Text_Effect old_value = edje_edit_part_text_shadow_direction_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJETEXTEFFECT;
        diff->redo.function = editor_part_text_shadow_direction_set;
        diff->redo.args.type_sete.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sete.ete2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJETEXTEFFECT;
        diff->undo.function = editor_part_text_shadow_direction_set;
        diff->undo.args.type_sete.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sete.ete2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_effect_set(edit_object, part_name, (new_val | edje_edit_part_text_effect_get(edit_object, part_name))))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_ignore_flags_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                             const char *part_name, Evas_Event_Flags new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_IGNORE_FLAGS;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   if (change)
     {
        Evas_Event_Flags old_value = edje_edit_part_ignore_flags_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EVASEVENTFLAGS;
        diff->redo.function = editor_part_ignore_flags_set;
        diff->redo.args.type_seef.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_seef.eef2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EVASEVENTFLAGS;
        diff->undo.function = editor_part_ignore_flags_set;
        diff->undo.args.type_seef.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_seef.eef2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_ignore_flags_set(edit_object, part_name, new_val))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

EDITOR_PART_BOOL(mouse_events, ATTRIBUTE_PART_MOUSE_EVENTS)
EDITOR_PART_BOOL(repeat_events, ATTRIBUTE_PART_REPEAT_EVENTS)
EDITOR_PART_BOOL(scale, ATTRIBUTE_PART_SCALE)
EDITOR_PART_BOOL(multiline, ATTRIBUTE_PART_MULTILINE)

EDITOR_PART_INT(drag_count_x, ATTRIBUTE_PART_DRAG_COUNT_X)
EDITOR_PART_INT(drag_count_y, ATTRIBUTE_PART_DRAG_COUNT_Y)
EDITOR_PART_INT(drag_x, ATTRIBUTE_PART_DRAG_X)
EDITOR_PART_INT(drag_y, ATTRIBUTE_PART_DRAG_Y)
EDITOR_PART_INT(drag_step_x, ATTRIBUTE_PART_DRAG_STEP_X)
EDITOR_PART_INT(drag_step_y, ATTRIBUTE_PART_DRAG_STEP_Y)

EDITOR_STRING_STRING(part_clip_to, part_clip_to, ATTRIBUTE_PART_CLIP_TO)
EDITOR_STRING_STRING(part_drag_confine, part_drag_confine, ATTRIBUTE_PART_DRAG_CONFINE)
EDITOR_STRING_STRING(part_drag_threshold, part_drag_threshold, ATTRIBUTE_PART_DRAG_THRESHOLD)
EDITOR_STRING_STRING(part_drag_event, part_drag_event, ATTRIBUTE_PART_DRAG_EVENT)
EDITOR_STRING_STRING(part_group_source, part_source, ATTRIBUTE_PART_GROUP_SOURCE)
EDITOR_STRING_STRING(part_textblock_selection_under, part_source, ATTRIBUTE_PART_TEXTBLOCK_SELECTION_UNDER)
EDITOR_STRING_STRING(part_textblock_selection_over, part_source2, ATTRIBUTE_PART_TEXTBLOCK_SELECTION_OVER)
EDITOR_STRING_STRING(part_textblock_cursor_under, part_source3, ATTRIBUTE_PART_TEXTBLOCK_CURSOR_UNDER)
EDITOR_STRING_STRING(part_textblock_cursor_over, part_source4, ATTRIBUTE_PART_TEXTBLOCK_CURSOR_OVER)
EDITOR_STRING_STRING(part_textblock_anchors_under, part_source5, ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_UNDER)
EDITOR_STRING_STRING(part_textblock_anchors_over, part_source6, ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_OVER)

Eina_Bool
editor_part_name_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                     const char *name, const char *new_val)
{
   Diff *diff;
   Rename ren;
   Attribute attribute = ATTRIBUTE_PART_NAME;
   assert(edit_object != NULL);
   assert(name != NULL);
   assert(new_val != NULL);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_RENAME;
        diff->redo.function = editor_part_name_set;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_RENAME;
        diff->undo.function = editor_part_name_set;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(new_val);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(name);
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_name_set(edit_object, name, new_val))
          return false;
        _editor_project_changed();
        ren.old_name = name;
        ren.new_name = new_val;
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_PART_RENAMED, &ren);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_item_aspect_mode_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *part_name, const char *item_name, Edje_Aspect_Control new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_ITEM_ASPECT_MODE;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);
   if (change)
     {
        Edje_Aspect_Control old_value = edje_edit_part_item_aspect_mode_get(edit_object, part_name, item_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_EDJEASPECTCONTROL;
        diff->redo.function = editor_part_item_aspect_mode_set;
        diff->redo.args.type_sseac.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sseac.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_sseac.eac3 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_EDJEASPECTCONTROL;
        diff->undo.function = editor_part_item_aspect_mode_set;
        diff->undo.args.type_sseac.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sseac.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_sseac.eac3 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_aspect_mode_set(edit_object, part_name, item_name, new_val))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

EDITOR_PART_ITEM_DOUBLE(align_x, ATTRIBUTE_PART_ITEM_ALIGN_X);
EDITOR_PART_ITEM_DOUBLE(align_y, ATTRIBUTE_PART_ITEM_ALIGN_Y);
EDITOR_PART_ITEM_DOUBLE(weight_x, ATTRIBUTE_PART_ITEM_WEIGHT_X);
EDITOR_PART_ITEM_DOUBLE(weight_y, ATTRIBUTE_PART_ITEM_WEIGHT_Y);

EDITOR_PART_ITEM_INT(aspect_h, ATTRIBUTE_PART_ITEM_ASPECT_H, false);
EDITOR_PART_ITEM_INT(aspect_w, ATTRIBUTE_PART_ITEM_ASPECT_W, false);
EDITOR_PART_ITEM_INT(max_h, ATTRIBUTE_PART_ITEM_MAX_H, false);
EDITOR_PART_ITEM_INT(max_w, ATTRIBUTE_PART_ITEM_MAX_W, false);
EDITOR_PART_ITEM_INT(min_h, ATTRIBUTE_PART_ITEM_MIN_H, false);
EDITOR_PART_ITEM_INT(min_w, ATTRIBUTE_PART_ITEM_MIN_W, false);
EDITOR_PART_ITEM_INT(prefer_h, ATTRIBUTE_PART_ITEM_PREFER_H, false);
EDITOR_PART_ITEM_INT(prefer_w, ATTRIBUTE_PART_ITEM_PREFER_W, false);
EDITOR_PART_ITEM_INT(spread_h, ATTRIBUTE_PART_ITEM_SPREAD_H, true);
EDITOR_PART_ITEM_INT(spread_w, ATTRIBUTE_PART_ITEM_SPREAD_W, true);

EDITOR_PART_ITEM_USHORT(span_col, ATTRIBUTE_PART_ITEM_SPAN_COL, true);
EDITOR_PART_ITEM_USHORT(span_row, ATTRIBUTE_PART_ITEM_SPAN_ROW, true);
EDITOR_PART_ITEM_USHORT(position_col, ATTRIBUTE_PART_ITEM_POSITION_COL, true);
EDITOR_PART_ITEM_USHORT(position_row, ATTRIBUTE_PART_ITEM_POSITION_ROW, true);

Eina_Bool
editor_part_item_source_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *part_name, const char *item_name, const char *new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_ITEM_SOURCE;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);
   if (change)
     {
        Eina_Stringshare *old_value = edje_edit_part_item_source_get(edit_object, part_name, item_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_STRING;
        diff->redo.function = editor_part_item_source_set;
        diff->redo.args.type_sss.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sss.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_sss.s3 = eina_stringshare_add(new_val);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_STRING;
        diff->undo.function = editor_part_item_source_set;
        diff->undo.args.type_sss.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sss.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_sss.s3 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_source_set(edit_object, part_name, item_name, new_val))
          return false;
        _editor_project_changed();
        CRIT_ON_FAIL(editor_save(edit_object));
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

/* PADDINGS */

Eina_Bool
editor_part_item_padding_left_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                  const char *part_name, const char *item_name, int n3)
{
   Diff *diff;
   int o3, o4, o5, o6;
   Attribute attribute = ATTRIBUTE_PART_ITEM_PADDING_LEFT;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);
   edje_edit_part_item_padding_get(edit_object, part_name, item_name, &o3, &o4, &o5, &o6);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->redo.function = editor_part_item_padding_left_set;
        diff->redo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_ssi.i3 = n3;
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->undo.function = editor_part_item_padding_left_set;
        diff->undo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_ssi.i3 = o3;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_padding_set(edit_object, part_name, item_name, n3, o4, o5, o6))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_item_padding_right_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                   const char *part_name, const char *item_name, int n4)
{
   Diff *diff;
   int o3, o4, o5, o6;
   Attribute attribute = ATTRIBUTE_PART_ITEM_PADDING_RIGHT;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);
   edje_edit_part_item_padding_get(edit_object, part_name, item_name, &o3, &o4, &o5, &o6);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->redo.function = editor_part_item_padding_right_set;
        diff->redo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_ssi.i3 = n4;
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->undo.function = editor_part_item_padding_right_set;
        diff->undo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_ssi.i3 = o4;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_padding_set(edit_object, part_name, item_name, o3, n4, o5, o6))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_item_padding_top_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                 const char *part_name, const char *item_name, int n5)
{
   Diff *diff;
   int o3, o4, o5, o6;
   Attribute attribute = ATTRIBUTE_PART_ITEM_PADDING_TOP;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);
   edje_edit_part_item_padding_get(edit_object, part_name, item_name, &o3, &o4, &o5, &o6);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->redo.function = editor_part_item_padding_top_set;
        diff->redo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_ssi.i3 = n5;
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->undo.function = editor_part_item_padding_top_set;
        diff->undo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_ssi.i3 = o5;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_padding_set(edit_object, part_name, item_name, o3, o4, n5, o6))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_item_padding_bottom_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                                    const char *part_name, const char *item_name, int n6)
{
   Diff *diff;
   int o3, o4, o5, o6;
   Attribute attribute = ATTRIBUTE_PART_ITEM_PADDING_BOTTOM;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);
   edje_edit_part_item_padding_get(edit_object, part_name, item_name, &o3, &o4, &o5, &o6);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->redo.function = editor_part_item_padding_bottom_set;
        diff->redo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_ssi.i3 = n6;
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_INT;
        diff->undo.function = editor_part_item_padding_bottom_set;
        diff->undo.args.type_ssi.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ssi.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_ssi.i3 = o6;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_padding_set(edit_object, part_name, item_name, o3, o4, o5, n6))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

/**********************/

Eina_Bool
editor_part_select_mode_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *part_name, Edje_Edit_Select_Mode new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_SELECT_MODE;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   if (change)
     {
        Edje_Edit_Select_Mode old_value = edje_edit_part_select_mode_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEEDITSELECTMODE;
        diff->redo.function = editor_part_select_mode_set;
        diff->redo.args.type_seesm.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_seesm.eesm2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJEEDITSELECTMODE;
        diff->undo.function = editor_part_select_mode_set;
        diff->undo.args.type_seesm.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_seesm.eesm2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_select_mode_set(edit_object, part_name, new_val))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_entry_mode_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *part_name, Edje_Edit_Entry_Mode new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_ENTRY_MODE;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   if (change)
     {
        Edje_Edit_Entry_Mode old_value = edje_edit_part_entry_mode_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE;
        diff->redo.function = editor_part_entry_mode_set;
        diff->redo.args.type_seeem.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_seeem.eeem2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE;
        diff->undo.function = editor_part_entry_mode_set;
        diff->undo.args.type_seeem.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_seeem.eeem2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_entry_mode_set(edit_object, part_name, new_val))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_pointer_mode_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                           const char *part_name, Evas_Object_Pointer_Mode new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_POINTER_MODE;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   if (change)
     {
        Evas_Object_Pointer_Mode old_value = edje_edit_part_pointer_mode_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE;
        diff->redo.function = editor_part_pointer_mode_set;
        diff->redo.args.type_seopm.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_seopm.eopm2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE;
        diff->undo.function = editor_part_pointer_mode_set;
        diff->undo.args.type_seopm.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_seopm.eopm2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_pointer_mode_set(edit_object, part_name, new_val))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

Eina_Bool
editor_part_cursor_mode_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                            const char *part_name, unsigned char new_val)
{
   Diff *diff;
   Attribute attribute = ATTRIBUTE_PART_CURSOR_MODE;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   if (change)
     {
        unsigned char old_value = edje_edit_part_cursor_mode_get(edit_object, part_name);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_UCHAR;
        diff->redo.function = editor_part_cursor_mode_set;
        diff->redo.args.type_suc.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_suc.uc2 = new_val;
        diff->undo.type = FUNCTION_TYPE_STRING_UCHAR;
        diff->undo.function = editor_part_cursor_mode_set;
        diff->undo.args.type_suc.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_suc.uc2 = old_value;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_cursor_mode_set(edit_object, part_name, new_val))
          return false;
        _editor_project_changed();
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &attribute);
     }
   return true;
}

static Eina_Bool _part_item_restacking;
Eina_Bool
editor_part_item_reset(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                       const char *part_name, const char *item_name)
{
   Eina_Bool res = true;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(item_name != NULL);

   Edje_Part_Type type = edje_edit_part_type_get(edit_object, part_name);

   assert((type == EDJE_PART_TYPE_TABLE) || (type == EDJE_PART_TYPE_BOX));

   you_shall_not_pass_editor_signals(change);

   if (type == EDJE_PART_TYPE_TABLE)
     {
        res = res && editor_part_item_span_col_reset(edit_object, change, apply, part_name, item_name);
        res = res && editor_part_item_span_row_reset(edit_object, change, apply, part_name, item_name);
        res = res && editor_part_item_position_col_reset(edit_object, change, apply, part_name, item_name);
        res = res && editor_part_item_position_row_reset(edit_object, change, apply, part_name, item_name);
        res = res && editor_part_item_spread_h_reset(edit_object, change, apply, part_name, item_name);
        res = res && editor_part_item_spread_w_reset(edit_object, change, apply, part_name, item_name);
     }

   res = res && editor_part_item_aspect_mode_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_aspect_h_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_aspect_w_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_max_h_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_max_w_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_min_h_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_min_w_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_prefer_h_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_prefer_w_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_align_x_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_align_y_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_weight_x_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_weight_y_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_padding_top_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_padding_bottom_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_padding_left_reset(edit_object, change, apply, part_name, item_name);
   res = res && editor_part_item_padding_right_reset(edit_object, change, apply, part_name, item_name);
   if (!_part_item_restacking)
     res = res && editor_part_item_restack(edit_object, change, apply, false, part_name, item_name, NULL);

   you_shall_pass_editor_signals(change);

   return res;
}

Eina_Bool
editor_part_item_append(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                        const char *part_name, const char *item_name, const char *source_group)
{
   Diff *diff;
   Editor_Item event_info;

   assert(edit_object != NULL);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_STRING;
        diff->redo.function = editor_part_item_append;
        diff->redo.args.type_sss.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sss.s2 = eina_stringshare_add(item_name);
        diff->redo.args.type_sss.s3 = eina_stringshare_add(source_group);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_part_item_del;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ss.s2 = eina_stringshare_add(item_name);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_append(edit_object, part_name, item_name, source_group))
          return false;

        /* fixing incorrect default item position */
        edje_edit_part_item_position_row_set(edit_object, part_name, item_name, 0);
        edje_edit_part_item_position_col_set(edit_object, part_name, item_name, 0);

        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();

        event_info.part_name = eina_stringshare_add(part_name);
        event_info.item_name = eina_stringshare_add(item_name);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_ITEM_ADDED, (void *)&event_info);
        eina_stringshare_del(event_info.part_name);
        eina_stringshare_del(event_info.item_name);
     }
   return true;
}

Eina_Bool
editor_part_item_del(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                     const char *part_name, const char *item_name)
{
   Diff *diff;
   Editor_Item event_info;
   Eina_Stringshare *source_group;

   assert(edit_object != NULL);

   event_info.part_name = eina_stringshare_add(part_name);
   event_info.item_name = eina_stringshare_add(item_name);
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_ITEM_DELETED, (void *)&event_info);
   if (change)
     {
        source_group = edje_edit_part_item_source_get(edit_object, part_name, item_name);
        if (!editor_part_item_reset(edit_object, change, false, apply, part_name, item_name))
          return false;
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_part_item_del;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(item_name);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_STRING;
        diff->undo.function = editor_part_item_append;
        diff->undo.args.type_sss.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sss.s2 = eina_stringshare_add(item_name);
        diff->undo.args.type_sss.s3 = eina_stringshare_add(source_group);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_item_del(edit_object, part_name, item_name))
          {
             eina_stringshare_del(event_info.part_name);
             eina_stringshare_del(event_info.item_name);
             return false;
          }
        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();
        eina_stringshare_del(event_info.part_name);
        eina_stringshare_del(event_info.item_name);
     }
   return true;
}

Eina_Bool
editor_part_reset(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                  const char *part_name)
{
   Eina_Bool res = true;
   Eina_List *items, *states, *parts, *l, *l_s;
   Eina_Stringshare *part, *state, *name, *ref;
   double state_val;

   assert(edit_object != NULL);
   assert(part_name != NULL);

   you_shall_not_pass_editor_signals(change);

   Edje_Part_Type type = edje_edit_part_type_get(edit_object, part_name);

   if ((type == EDJE_PART_TYPE_TEXTBLOCK) || (type == EDJE_PART_TYPE_TEXT))
     {
        res = res && editor_part_text_effect_reset(edit_object, change, apply, part_name);
        res = res && editor_part_text_shadow_direction_reset(edit_object, change, apply, part_name);
        res = res && editor_part_multiline_reset(edit_object, change, apply, part_name);
        res = res && editor_part_select_mode_reset(edit_object, change, apply, part_name);
        res = res && editor_part_entry_mode_reset(edit_object, change, apply, part_name);
        res = res && editor_part_cursor_mode_reset(edit_object, change, apply, part_name);
        res = res && editor_part_textblock_selection_under_reset(edit_object, change, apply, part_name);
        res = res && editor_part_textblock_selection_over_reset(edit_object, change, apply, part_name);
        res = res && editor_part_textblock_cursor_under_reset(edit_object, change, apply, part_name);
        res = res && editor_part_textblock_cursor_over_reset(edit_object, change, apply, part_name);
        res = res && editor_part_textblock_anchors_under_reset(edit_object, change, apply, part_name);
        res = res && editor_part_textblock_anchors_over_reset(edit_object, change, apply, part_name);
     }
   else if (type == EDJE_PART_TYPE_GROUP)
     res = res && editor_part_group_source_reset(edit_object, change, apply, part_name);

   res = res && editor_part_ignore_flags_reset(edit_object, change, apply, part_name);
   res = res && editor_part_mouse_events_reset(edit_object, change, apply, part_name);
   res = res && editor_part_repeat_events_reset(edit_object, change, apply, part_name);
   res = res && editor_part_scale_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_count_x_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_count_y_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_x_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_y_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_step_x_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_step_y_reset(edit_object, change, apply, part_name);
   res = res && editor_part_clip_to_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_confine_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_threshold_reset(edit_object, change, apply, part_name);
   res = res && editor_part_drag_event_reset(edit_object, change, apply, part_name);
   res = res && editor_part_pointer_mode_reset(edit_object, change, apply, part_name);
   res = res && editor_part_restack(edit_object, change, apply, false, part_name, NULL);

   states = edje_edit_part_states_list_get(edit_object, part_name);
   assert(states != NULL);

   /* Removing states. Default state can only be reseted */
   res = res && editor_part_selected_state_set(edit_object, change, false, apply, part_name, "default", 0.0);
   res = res && editor_state_reset(edit_object, change, false, apply, part_name, "default", 0.0);
   EINA_LIST_FOREACH(eina_list_next(states), l, state)
     {
        state_name_split(state, &name, &state_val);
        res = res && editor_state_del(edit_object, change, false, apply, part_name, name, state_val);
        eina_stringshare_del(name);
     }
   edje_edit_string_list_free(states);

   /* removing part items */
   items = edje_edit_part_items_list_get(edit_object, part_name);
   EINA_LIST_REVERSE_FOREACH(items, l, name)
      res = res && editor_part_item_del(edit_object, change, false, apply, part_name, name);
   edje_edit_string_list_free(items);

   /* remove external references */
   parts = edje_edit_parts_list_get(edit_object);
   EINA_LIST_FOREACH(parts, l, part)
     {
        if (part == part_name) continue;
        states = edje_edit_part_states_list_get(edit_object, part);
        type = edje_edit_part_type_get(edit_object, part);
        EINA_LIST_FOREACH(states, l_s, state)
          {
             state_name_split(state, &name, &state_val);

             #define RESET_STATE_REF(ATT) \
             ref = edje_edit_state_ ## ATT ## _get(edit_object, part, name, state_val); \
             if (ref == part_name) \
               res = res && editor_state_ ## ATT ## _reset(edit_object, change, apply, part, name, state_val);

             RESET_STATE_REF(rel1_to_x);
             RESET_STATE_REF(rel2_to_x);
             RESET_STATE_REF(rel1_to_y);
             RESET_STATE_REF(rel2_to_y);
             if (type == EDJE_PART_TYPE_PROXY)
               {
                  RESET_STATE_REF(proxy_source);
               }
             else if (type == EDJE_PART_TYPE_TEXT)
               {
                  RESET_STATE_REF(text_source);
                  RESET_STATE_REF(text_text_source);
               }

             eina_stringshare_del(name);
          }
        #define RESET_PART_REF(ATT, REAL_ATT) \
        ref = edje_edit_part_ ## REAL_ATT ## _get(edit_object, part); \
        if (ref == part_name) \
          res = res && editor_part_ ## ATT ## _reset(edit_object, change, apply, part);

        RESET_PART_REF(clip_to, clip_to);
        RESET_PART_REF(drag_confine, drag_confine);
        RESET_PART_REF(drag_threshold, drag_threshold);
        RESET_PART_REF(drag_event, drag_event);
        if (type == EDJE_PART_TYPE_TEXTBLOCK)
          {
             RESET_PART_REF(textblock_selection_under, source);
             RESET_PART_REF(textblock_selection_over, source2);
             RESET_PART_REF(textblock_cursor_under, source3);
             RESET_PART_REF(textblock_cursor_over, source4);
             RESET_PART_REF(textblock_anchors_under, source5);
             RESET_PART_REF(textblock_anchors_over, source6);
          }
        if (type == EDJE_PART_TYPE_GROUP)
          RESET_PART_REF(group_source, source);

     }
   edje_edit_string_list_free(parts);
   you_shall_pass_editor_signals(change);

   return res;
}

Eina_Bool
editor_part_add(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                const char *part_name, Edje_Part_Type type)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEPARTTYPE;
        diff->redo.function = editor_part_add;
        diff->redo.args.type_sept.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sept.ept2 = type;
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_part_del;
        diff->undo.args.type_s.s1 = eina_stringshare_add(part_name);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_add(edit_object, part_name, type))
          return false;

        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();

        /* fix incorrect default values */
        TODO("Fix edje_edit")
           if (type == EDJE_PART_TYPE_BOX)
             edje_edit_state_box_layout_set(edit_object, part_name, "default", 0.0, "horizontal");

        /* apply our default values */
        if (!editor_part_reset(edit_object, NULL, false, apply, part_name))
          return false;

        event_info = eina_stringshare_add(part_name);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_ADDED, (void *)event_info);
        eina_stringshare_del(event_info);
     }
   return true;
}

Eina_Bool
editor_part_copy(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                 const char *part_name, const char *part_name_copy)
{
   Diff *diff;
   Eina_Stringshare *event_info;

   assert(edit_object != NULL);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_EDJEPARTCOPY;
        diff->redo.function = editor_part_copy;
        diff->redo.args.type_sepc.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sepc.s2 = eina_stringshare_add(part_name_copy);
        diff->undo.type = FUNCTION_TYPE_STRING;
        diff->undo.function = editor_part_del;
        diff->undo.args.type_s.s1 = eina_stringshare_add(part_name_copy);

        change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_copy(edit_object, part_name, part_name_copy))
          return false;

        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();

        event_info = eina_stringshare_add(part_name_copy);
        if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_ADDED, (void *)event_info);
        eina_stringshare_del(event_info);
     }
   return true;
}

Eina_Bool
editor_part_del(Evas_Object *edit_object, Change *change, Eina_Bool merge __UNUSED__, Eina_Bool apply,
                const char *part_name)
{
   Diff *diff;
   Eina_Stringshare *event_info;
   Edje_Part_Type type;

   assert(edit_object != NULL);

   event_info = eina_stringshare_add(part_name);
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_DELETED, (void *)event_info);

   if (change)
     {
        type = edje_edit_part_type_get(edit_object, part_name);
        if (!editor_part_reset(edit_object, change, false, apply, part_name))
          return false;
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING;
        diff->redo.function = editor_part_del;
        diff->redo.args.type_s.s1 = eina_stringshare_add(part_name);
        diff->undo.type = FUNCTION_TYPE_STRING_EDJEPARTTYPE;
        diff->undo.function = editor_part_add;
        diff->undo.args.type_sept.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sept.ept2 = type;

        change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_del(edit_object, part_name))
          {
             eina_stringshare_del(event_info);
             return false;
          }
        eina_stringshare_del(event_info);
        CRIT_ON_FAIL(editor_save(edit_object));
        _editor_project_changed();
     }
   return true;
}

Eina_Bool
editor_part_restack(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                    const char *part_name, const char *relative_part)
{
   Diff *diff;
   Eina_Stringshare *old_relative_part;
   Editor_Part_Restack event_info;

   assert(edit_object != NULL);
   assert(part_name != NULL);

   old_relative_part = edje_edit_part_above_get(edit_object, part_name);
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING;
        diff->redo.function = editor_part_restack;
        diff->redo.args.type_ss.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ss.s2 = eina_stringshare_add(relative_part);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING;
        diff->undo.function = editor_part_restack;
        diff->undo.args.type_ss.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ss.s2 = old_relative_part;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (relative_part)
          {
             if (!edje_edit_part_restack_part_below(edit_object, part_name, relative_part))
               return false;
          }
        else
          {
             /* edje_edit don't allows to restack_above part that is already on top,
                but it is needed to simplify adding part deletion to history */
             if (old_relative_part != NULL)
               if (!edje_edit_part_restack_above(edit_object, part_name))
                 return false;
          }

        _editor_project_changed();
        if (!_editor_signals_blocked)
          {
             event_info.part_name = eina_stringshare_add(part_name);
             event_info.relative_part_name = eina_stringshare_add(relative_part);
             evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_RESTACKED, &event_info);
             eina_stringshare_del(event_info.part_name);
             eina_stringshare_del(event_info.relative_part_name);
          }
     }
   return true;
}

Eina_Bool
editor_part_item_restack(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                         const char *part_name, const char *part_item, const char *relative_part_item)
{
   Diff *diff;
   Eina_Stringshare *old_relative_part_item;
   Editor_Part_Item_Restack event_info;
   Eina_List *items, *part_item_l, *l;
   Eina_Stringshare *part_item_name, *relative_part_item_name, *item_name;
   Change *virtual_change;
   Eina_Bool res;

   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(part_item != NULL);

   items = edje_edit_part_items_list_get(edit_object, part_name);
   assert(items != NULL);
   part_item_name = eina_stringshare_add(part_item);
   part_item_l = eina_list_data_find_list(items, part_item_name);

   old_relative_part_item = eina_stringshare_add(eina_list_data_get(eina_list_next(part_item_l)));
   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_STRING;
        diff->redo.function = editor_part_item_restack;
        diff->redo.args.type_sss.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_sss.s2 = eina_stringshare_add(part_item);
        diff->redo.args.type_sss.s3 = eina_stringshare_add(relative_part_item);
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_STRING;
        diff->undo.function = editor_part_item_restack;
        diff->undo.args.type_sss.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_sss.s2 = eina_stringshare_add(part_item);
        diff->undo.args.type_sss.s3 = old_relative_part_item;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }

   virtual_change = change_add(NULL);
   you_shall_not_pass_editor_signals(NULL);

   _part_item_restacking = true;
   if (relative_part_item)
     {
        relative_part_item_name = eina_stringshare_add(relative_part_item);
        if (old_relative_part_item != relative_part_item_name)
          {
             EINA_LIST_REVERSE_FOREACH(items, l, item_name)
               {
                  if (item_name == part_item_name) continue;

                  res = editor_part_item_del(edit_object, virtual_change, false, apply, part_name, item_name);
                  assert(res);

                  if (item_name == relative_part_item_name) break;
               }
             res = editor_part_item_del(edit_object, virtual_change, false, apply, part_name, part_item);
             assert(res);
             res = change_undo(edit_object, virtual_change);
             assert(res);
          }
     }
   else
     {
        if (eina_list_next(part_item_l))
          {
             res = editor_part_item_del(edit_object, virtual_change, false, apply, part_name, part_item);
             assert(res);
             res = change_undo(edit_object, virtual_change);
             assert(res);
          }
     }

   _part_item_restacking = false;
   you_shall_pass_editor_signals(NULL);
   change_free(virtual_change);

   edje_edit_string_list_free(items);

   _editor_project_changed();
   if (!_editor_signals_blocked)
     {
        event_info.part_name = eina_stringshare_add(part_name);
        event_info.part_item = part_item_name;
        event_info.relative_part_item = eina_stringshare_add(relative_part_item);
        evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_ITEM_RESTACKED, &event_info);
        eina_stringshare_del(event_info.part_name);
        eina_stringshare_del(event_info.part_item);
        eina_stringshare_del(event_info.relative_part_item);
     }
   return true;
}

Eina_Bool
editor_part_selected_state_set(Evas_Object *edit_object, Change *change, Eina_Bool merge, Eina_Bool apply,
                               const char *part_name, const char *state_name, double state_val)
{
   Diff *diff;
   Editor_State event_info;
   assert(edit_object != NULL);
   assert(part_name != NULL);
   assert(state_name != NULL);
   if (change)
     {
        double old_val = 0.0;
        Eina_Stringshare *old_state = edje_edit_part_selected_state_get(edit_object, part_name, &old_val);
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE;
        diff->redo.function = editor_part_selected_state_set;
        diff->redo.args.type_ssd.s1 = eina_stringshare_add(part_name);
        diff->redo.args.type_ssd.s2 = eina_stringshare_add(state_name);
        diff->redo.args.type_ssd.d3 = state_val;
        diff->undo.type = FUNCTION_TYPE_STRING_STRING_DOUBLE;
        diff->undo.function = editor_part_selected_state_set;
        diff->undo.args.type_ssd.s1 = eina_stringshare_add(part_name);
        diff->undo.args.type_ssd.s2 = old_state;
        diff->undo.args.type_ssd.d3 = old_val;
        if (merge)
          change_diff_merge_add(change, diff);
        else
          change_diff_add(change, diff);
     }
   if (apply)
     {
        if (!edje_edit_part_selected_state_set(edit_object, part_name, state_name, state_val))
          return false;
        if (!_editor_signals_blocked)
          {
             event_info.part_name = eina_stringshare_add(part_name);
             event_info.state_name = eina_stringshare_add(state_name);
             event_info.state_value = state_val;
             evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_PART_STATE_SELECTED, &event_info);
             eina_stringshare_del(event_info.part_name);
             eina_stringshare_del(event_info.state_name);
          }
     }
   return true;
}
