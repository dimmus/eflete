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

typedef struct _State_Params State_Params;

/**
 * @struct _State_Diff
 *
 * @brief Structure that store the diff data for states.
 * This structure "inherit" from @c _Diff. It's implement with use field Diff.
 * All fields of this structure provide save diff, that can be undo and
 * redo.
 *
 * @ingroup History
 */
struct _State_Diff
{
   Diff diff; /**<  Instance of main diff structure.*/
   Eina_Stringshare *style;  /**< Full group name */
   Eina_Stringshare *part;  /**< Part name */
   Edje_Part_Type type; /**< Part type. Helper that make search needed union faster */
   State_Params *state; /**< Here stored all params of state */
};

struct _State_Params
{
   Eina_Stringshare *name; /* the named state if any */
   double value; /* the value of the state (for ranges) */

   Eina_Bool visible;

   struct {
      int w, h;
   } min, max;

   struct {
        float x, y;
   } align; /* 0 <-> 1.0 alignment within allocated space */

   struct {
      Eina_Bool  w, h; /* width or height is fixed in side*/
   } fixed;

   struct {
      unsigned char param;
      float w, h;
   } aspect;

   Eina_Stringshare *color_class; /* how to modify the color */
   struct {
        int r, g, b, a;
   } color, color2;

   struct {
      float relative_x;
      float relative_y;
      int offset_x;
      int offset_y;
      Eina_Stringshare *to_x; /* -1 = whole part collection, or part ID */
      Eina_Stringshare *to_y; /* -1 = whole part collection, or part ID */
   } rel1, rel2;
};

/* argument 'type' will be used in future, for store special part types data */
static State_Params *
_state_params_save(Evas_Object *obj, const char *part, const char *state,
                   double value, Edje_Part_Type type __UNUSED__)
{
   State_Params *state_diff = NULL;
   int r, g, b, a;

   if ((!part) || (!state)) return NULL;
   if (!edje_edit_state_exist(obj, part, state, value)) return NULL;

   state_diff = (State_Params *)mem_calloc(1, sizeof(State_Params));

   state_diff->name = eina_stringshare_add(state);
   state_diff->value = value;
   state_diff->visible = edje_edit_state_visible_get(obj, part, state, value);
   state_diff->min.w = edje_edit_state_min_w_get(obj, part, state, value);
   state_diff->min.h = edje_edit_state_min_h_get(obj, part, state, value);
   state_diff->max.w = edje_edit_state_max_w_get(obj, part, state, value);
   state_diff->max.h = edje_edit_state_max_h_get(obj, part, state, value);
   state_diff->align.x = edje_edit_state_align_x_get(obj, part, state, value);
   state_diff->align.y = edje_edit_state_align_y_get(obj, part, state, value);
   state_diff->fixed.w = edje_edit_state_fixed_w_get(obj, part, state, value);
   state_diff->fixed.h = edje_edit_state_fixed_h_get(obj, part, state, value);

   state_diff->aspect.w = edje_edit_state_aspect_min_get(obj, part, state, value);
   state_diff->aspect.h = edje_edit_state_aspect_max_get(obj, part, state, value);
   state_diff->aspect.param =
      edje_edit_state_aspect_pref_get(obj, part, state, value);

   state_diff->color_class =
      edje_edit_state_color_class_get(obj, part, state, value);

   state_diff->rel1.to_x = edje_edit_state_rel1_to_x_get(obj, part, state, value);
   state_diff->rel1.to_y = edje_edit_state_rel1_to_y_get(obj, part, state, value);
   state_diff->rel1.offset_x =
      edje_edit_state_rel1_offset_x_get(obj, part, state, value);
   state_diff->rel1.offset_y =
      edje_edit_state_rel1_offset_y_get(obj, part, state, value);
   state_diff->rel1.relative_x =
      edje_edit_state_rel1_relative_x_get(obj, part, state, value);
   state_diff->rel1.relative_y =
      edje_edit_state_rel1_relative_y_get(obj, part, state, value);

   state_diff->rel2.to_x =  edje_edit_state_rel2_to_x_get(obj, part, state, value);
   state_diff->rel2.to_y =  edje_edit_state_rel2_to_y_get(obj, part, state, value);
   state_diff->rel2.offset_x =
      edje_edit_state_rel2_offset_x_get(obj, part, state, value);
   state_diff->rel2.offset_y =
      edje_edit_state_rel2_offset_y_get(obj, part, state, value);
   state_diff->rel2.relative_x =
      edje_edit_state_rel2_relative_x_get(obj, part, state, value);
   state_diff->rel2.relative_y =
      edje_edit_state_rel2_relative_y_get(obj, part, state, value);

   edje_edit_state_color_get(obj, part, state, value, &r, &g, &b, &a);
   state_diff->color.r = r; state_diff->color.g = g;
   state_diff->color.b = b; state_diff->color.a = a;
   edje_edit_state_color2_get(obj, part, state, value, &r, &g, &b, &a);
   state_diff->color2.r = r; state_diff->color2.g = g;
   state_diff->color2.b = b; state_diff->color2.a = a;

   return state_diff;
}

Eina_Bool
_state_redo(Evas_Object *source __UNUSED__, State_Diff *change __UNUSED__)
{
   return false;
}

Eina_Bool
_state_undo(Evas_Object *source __UNUSED__, State_Diff *change __UNUSED__)
{
   return false;
}

void
_state_change_free(State_Diff *change __UNUSED__)
{
   return;
}

Diff *
_state_change_new(va_list list, Evas_Object *source)
{
   State_Diff *change = (State_Diff *)mem_calloc(1, sizeof(State_Diff));

   change->diff.module_type = STATE_TARGET;
   change->diff.action_type = va_arg(list, Action);

   change->style = eina_stringshare_add((char *)va_arg(list, char *));
   if (!change->style) goto error;

   change->part = eina_stringshare_add((char *)va_arg(list, char *));
   if (!change->part) goto error;

   change->type = edje_edit_part_type_get(source, change->part);
   change->state = _state_params_save(source, change->part,
                                      (char *)va_arg(list, char *),
                                      (double)va_arg(list, double),
                                      change->type);
   if (!change->state) goto error;

   change->diff.description = eina_stringshare_add((char *)va_arg(list, char *));
   change->diff.source = eina_stringshare_add(change->part);
   change->diff.state = eina_stringshare_printf("%s %.2f", change->state->name,
                                                 change->state->value);

   return (Diff *)change;

error:
   _state_change_free(change);
   return NULL;
}
