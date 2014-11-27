/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "history.h"
#include "history_private.h"

typedef struct _Part_Params Part_Params;

struct _Part_Params
{
   Eina_Stringshare *clip_to;
   struct
     {
        int step_x; /* drag jumps n pixels (0 = no limit) */
        int step_y; /* drag jumps n pixels (0 = no limit) */
        signed char x; /* can u click & drag this bit in x dir */
        signed char y; /* can u click & drag this bit in y dir */
        Eina_Stringshare *confine; /* dragging within this bit, NULL = no */
        Eina_Stringshare *threshold; /* dragging outside this bit, NULL = no */
     } dragable;
   Edje_Part_Type type; /* what type (image, rect, text) */
   Eina_Bool mouse_events; /* it will affect/respond to mouse events */
   Eina_Bool repeat_events; /* it will repeat events to objects below */
   Eina_Bool multiline;
   Eina_Bool scale; /* should certain properties scale with edje scale factor? */
   Eina_Bool last;
   Evas_Object_Pointer_Mode pointer_mode;
   Evas_Event_Flags ignore_flags;
   Edje_Edit_Entry_Mode entry_mode;
   Edje_Edit_Select_Mode select_mode;
   unsigned char effect; /* 0 = plain... */
   unsigned char cursor_mode;
   Eina_Stringshare *above;
};

struct _Part_Diff
{
   Diff diff;
   Eina_Stringshare *part;
   Eina_List *states;
};

Part_Params *
_part_params_save(Evas_Object *obj, Eina_Stringshare *part)
{
   Part_Params *params_diff = NULL;

   if (!edje_edit_part_exist(obj, part)) return NULL;

   params_diff = (Part_Params *)mem_calloc(1, sizeof(Part_Params));

   params_diff->clip_to = edje_edit_part_clip_to_get(obj, part);
   params_diff->type = edje_edit_part_type_get(obj, part);
   params_diff->effect = edje_edit_part_effect_get(obj, part);
   params_diff->mouse_events = edje_edit_part_mouse_events_get(obj, part);
   params_diff->repeat_events = edje_edit_part_repeat_events_get(obj, part);
   params_diff->multiline = edje_edit_part_multiline_get(obj, part);
   params_diff->scale = edje_edit_part_scale_get(obj, part);
   params_diff->ignore_flags = edje_edit_part_ignore_flags_get(obj, part);
   params_diff->pointer_mode = edje_edit_part_pointer_mode_get(obj, part);
   params_diff->entry_mode = edje_edit_part_entry_mode_get(obj, part);
   params_diff->select_mode = edje_edit_part_select_mode_get(obj, part);
   params_diff->cursor_mode = edje_edit_part_cursor_mode_get(obj, part);

   params_diff->dragable.x = edje_edit_part_drag_x_get(obj, part);
   params_diff->dragable.y = edje_edit_part_drag_y_get(obj, part);
   params_diff->dragable.step_x = edje_edit_part_drag_step_x_get(obj, part);
   params_diff->dragable.step_y = edje_edit_part_drag_step_y_get(obj, part);
   params_diff->dragable.confine = edje_edit_part_drag_confine_get(obj, part);
   params_diff->dragable.threshold = edje_edit_part_drag_threshold_get(obj, part);

   params_diff->above = edje_edit_part_above_get(obj, part);
   if (!params_diff->above)
     {
        params_diff->above = edje_edit_part_below_get(obj, part);
        params_diff->last = true;
     }

   return params_diff;
}

Eina_Bool
_part_params_restore(Evas_Object *obj, Eina_Stringshare *part,
                     Part_Params *params_diff)
{
   if (!edje_edit_part_exist(obj, part)) return false;

   edje_edit_part_clip_to_set(obj, part, params_diff->clip_to);
   edje_edit_part_effect_set(obj, part, params_diff->effect);
   edje_edit_part_mouse_events_set(obj, part, params_diff->mouse_events);
   edje_edit_part_repeat_events_set(obj, part, params_diff->repeat_events);
   edje_edit_part_multiline_set(obj, part, params_diff->multiline);
   edje_edit_part_scale_set(obj, part, params_diff->scale);
   edje_edit_part_ignore_flags_set(obj, part, params_diff->ignore_flags);
   edje_edit_part_pointer_mode_set(obj, part, params_diff->pointer_mode);
   edje_edit_part_entry_mode_set(obj, part, params_diff->entry_mode);
   edje_edit_part_select_mode_set(obj, part, params_diff->select_mode);
   edje_edit_part_cursor_mode_set(obj, part, params_diff->cursor_mode);

   edje_edit_part_drag_x_set(obj, part, params_diff->dragable.x);
   edje_edit_part_drag_y_set(obj, part, params_diff->dragable.y);
   edje_edit_part_drag_step_x_set(obj, part, params_diff->dragable.step_x);
   edje_edit_part_drag_step_y_set(obj, part, params_diff->dragable.step_y);
   edje_edit_part_drag_confine_set(obj, part, params_diff->dragable.confine);
   edje_edit_part_drag_threshold_set(obj, part, params_diff->dragable.threshold);

   return true;
}

Eina_Bool
_part_redo(Evas_Object *source __UNUSED__, Part_Diff *change __UNUSED__)
{
   return false;
}

Eina_Bool
_part_undo(Evas_Object *source __UNUSED__, Part_Diff *change __UNUSED__)
{
   return false;
}

void
_part_change_free(Part_Diff *change __UNUSED__)
{
   return;
}

Diff *
_part_change_new(va_list list __UNUSED__, Evas_Object *source __UNUSED__)
{
   return NULL;
}
