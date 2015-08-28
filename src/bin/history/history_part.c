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
   Part_Params *params;
};

static Part_Params *
_part_params_save(Evas_Object *obj, Eina_Stringshare *part)
{
   Part_Params *params_diff = NULL;

   assert(obj != NULL);
   assert(part != NULL);

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
   assert(obj != NULL);
   assert(part != NULL);
   assert(params_diff != NULL);

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
_part_redo(Evas_Object *source, Part_Diff *change)
{
   Eina_Bool last = false;
   Eina_Stringshare *above = NULL;
   Eina_List *l = NULL, *l_next = NULL;
   State_Params *state = NULL;

   assert(source != NULL);
   assert(change != NULL);

   Style *style = ap.project->current_style;

   assert(style != NULL);
   assert(style->obj != NULL);

   Evas_Object *prop = ui_block_property_get();
   Evas_Object *widget_list = ui_block_widget_list_get();

   switch (change->diff.action_type)
     {
      case ADD:
         if (!workspace_edit_object_part_add(ap.workspace, change->part,
                                             change->params->type, NULL))
           {
              ERR("Can't add part to workspace");
              abort();
           }
         if (change->params->type == EDJE_PART_TYPE_IMAGE)
           {
              EINA_LIST_FOREACH_SAFE(change->states, l, l_next, state)
                 {
                    _state_param_restore(source, change->part, state,
                                         change->params->type);
                 }
           }
         ui_widget_list_part_add(widget_list, style, change->part);

         ui_widget_list_part_selected_set(widget_list, change->part, true);
      break;
      case DEL:
         ui_widget_list_part_selected_set(widget_list, change->part, true);
         ui_widget_list_selected_part_del(widget_list, style);
         evas_object_smart_callback_call(ap.workspace, "ws,part,unselected",
                                         (void *)change->part);
         //workspace_highlight_unset(ap.workspace);
         ui_property_part_unset(prop);
         //ui_property_style_set(prop, style, ap.workspace);

         workspace_edit_object_part_del(ap.workspace, change->part);
         ui_widget_list_style_parts_reload(widget_list, style);
      break;
      case RESTACK:

         if (!change->params->last)
           wm_style_parts_restack(style, change->part, change->params->above);
         else
           wm_style_parts_restack(style, change->part, NULL);

         above = edje_edit_part_above_get(style->obj, change->part);
         if (!above)
           {
             above = edje_edit_part_below_get(style->obj, change->part);
             last = true;
           }
         workspace_edit_object_part_restack(ap.workspace, change->part,
                                            change->params->above,
                                            change->params->last);
         change->params->last = last;
         change->params->above = above;

         ui_widget_list_style_parts_reload(widget_list, style);
         ui_widget_list_part_selected_set(widget_list, change->part, true);
      break;
      default:
        {
           ERR("Wrong action type");
           abort();
        }
      break;
     }
   return true;
}

Eina_Bool
_part_undo(Evas_Object *source, Part_Diff *change)
{
   Eina_List *l = NULL, *l_next = NULL;
   State_Params *state = NULL;
   Eina_Bool last = false;
   Eina_Stringshare *above = NULL;

   assert(source != NULL);
   assert(change != NULL);

   Style *style = ap.project->current_style;

   assert(style != NULL);
   assert(style->obj != NULL);

   //Evas_Object *prop = ui_block_property_get();
   Evas_Object *widget_list = ui_block_widget_list_get();

   switch (change->diff.action_type)
     {
      case ADD:
         workspace_edit_object_part_del(ap.workspace, change->part);
         //workspace_highlight_unset(ap.workspace);
         ui_widget_list_part_selected_set(widget_list, change->part, true);
         ui_widget_list_selected_part_del(widget_list, style);

         //ui_property_style_set(prop, style, ap.workspace);
         ui_states_list_data_unset();
         ui_widget_list_style_parts_reload(widget_list, style);
      break;
      case DEL:
         if (!workspace_edit_object_part_add(ap.workspace, change->part,
                                             change->params->type, NULL))
           {
              ERR("Can't add part to workspace");
              abort();
           }
         _part_params_restore(source, change->part, change->params);
         EINA_LIST_FOREACH_SAFE(change->states, l, l_next, state)
           {
             _state_param_restore(source, change->part, state,
                                  change->params->type);
           }
         ui_widget_list_part_add(widget_list, style, change->part);
         wm_style_parts_restack(style, change->part, change->params->above);
         workspace_edit_object_part_restack(ap.workspace, change->part,
                                            change->params->above,
                                            change->params->last);

         ui_widget_list_style_parts_reload(widget_list, style);
         ui_widget_list_part_selected_set(widget_list, change->part, true);
      break;
      case RESTACK:
         /*Save current position in parts stack*/
         above = edje_edit_part_above_get(source, change->part);
         if (!above)
           {
             above = edje_edit_part_below_get(source, change->part);
             last = true;
           }

         if (!change->params->last)
           wm_style_parts_restack(style, change->part, change->params->above);
         else
           wm_style_parts_restack(style, change->part, NULL);

         workspace_edit_object_part_restack(ap.workspace, change->part,
                                            change->params->above,
                                            change->params->last);
         change->params->last = last;
         change->params->above = above;

         ui_widget_list_style_parts_reload(widget_list, style);
         ui_widget_list_part_selected_set(widget_list, change->part, true);
      break;
      default:
        {
           ERR("Wrong action type");
           abort();
        }
      break;
     }
   return true;
}

void
_part_change_free(Part_Diff *change)
{
   State_Params *state = NULL;

   assert(change != NULL);

   eina_stringshare_del(change->part);

   EINA_LIST_FREE(change->states, state)
     free(state);
   if (change->diff.ui_item)
     elm_object_item_del(change->diff.ui_item);

   if (change->params)
     {
        eina_stringshare_del(change->params->dragable.confine);
        eina_stringshare_del(change->params->dragable.threshold);
        eina_stringshare_del(change->params->above);
        free(change->params);
     }
   free(change);
   return;
}

Diff *
_part_change_new(va_list list, Evas_Object *source)
{
   Eina_List *states, *l, *l_next;
   Eina_Stringshare *state_name;
   char **split = NULL;
   State_Params *state = NULL;

   Part_Diff *change = (Part_Diff *)mem_calloc(1, sizeof(Part_Diff));

   change->diff.module_type = PART_TARGET;
   change->diff.action_type = va_arg(list, Action);

   assert ((change->diff.action_type == ADD) ||
           (change->diff.action_type == DEL) ||
           (change->diff.action_type == RESTACK));

   change->part = eina_stringshare_add((char *)va_arg(list, char *));

   assert (change->part != NULL);

   /*This strings needed for UI elements. Will change later. */
   change->diff.new = eina_stringshare_add(change->part);

   change->params = _part_params_save(source, change->part);

   assert(change->params != NULL);

   if (change->diff.action_type == RESTACK)
     return (Diff *)change;

   states = edje_edit_part_states_list_get(source, change->part);
   EINA_LIST_FOREACH_SAFE(states, l, l_next, state_name)
       {
          split = eina_str_split(state_name, " ", 2);
          state = _state_params_save(source, change->part, split[0],
                                     atof(split[1]), change->params->type);
          change->states = eina_list_append(change->states, state);
          free(split[0]);
          free(split);
       }
   edje_edit_string_list_free(states);

   assert(change->states != NULL);

   return (Diff *)change;
}
