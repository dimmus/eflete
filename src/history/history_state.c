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
typedef struct _Description_Fill Description_Fill;
typedef struct _Description_Image Description_Image;
typedef struct _Description_Text Description_Text;
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

struct _Description_Fill
{
   double         pos_rel_x; /* fill offset x relative to area */
   double         rel_x; /* relative size compared to area */
   double         pos_rel_y; /* fill offset y relative to area */
   double         rel_y; /* relative size compared to area */
   int            pos_abs_x; /* fill offset x added to fill offset */
   int            abs_x; /* size of fill added to relative fill */
   int            pos_abs_y; /* fill offset y added to fill offset */
   int            abs_y; /* size of fill added to relative fill */
   char           smooth; /* fill with smooth scaling or not */
   unsigned char  type; /* fill coordinate from container (SCALE) or from source image (TILE) */
};

struct _Description_Image
{
   Description_Fill fill;
   struct {
      int l, r, t, b; /* border scaling on image fill */
      unsigned char  no_fill; /* do we fill the center of the image if bordered? 1 == NO!!!! */
   } border;

   Eina_List *tweens;
   Eina_Stringshare *normal;
};

struct _Description_Text
{
   Eina_Stringshare *text; /* if "" or NULL, then leave text unchanged */
   Eina_Stringshare *text_class; /* how to apply/modify the font */
   Eina_Stringshare *font; /* if a specific font is asked for */
   Eina_Stringshare *repch;

   struct {
        float x, y;
   } align; /* 0 <-> 1.0 alignment within allocated space */

   struct {
        int r, g, b, a;
   } color3;

   double         ellipsis; /* 0.0 - 1.0 defining where the ellipsis align */
   int            size; /* 0 = use user set size */
   Eina_Stringshare *source_part;
   Eina_Stringshare *text_source_part;

   unsigned char  fit_x; /* resize font size down to fit in x dir */
   unsigned char  fit_y; /* resize font size down to fit in y dir */
   unsigned char  min_x; /* if text size should be part min size */
   unsigned char  min_y; /* if text size should be part min size */
   unsigned char  max_x; /* if text size should be part max size */
   unsigned char  max_y; /* if text size should be part max size */
   int            size_range_min;
   int            size_range_max; /* -1 means, no bound. */
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
   union {
      Description_Text text;
      Description_Image image;
   };
};

static void
_history_ui_state_update(Evas_Object *source, State_Diff *change)
{
   Style *style = NULL;
   Part *part = NULL;

   App_Data *app = app_data_get();
   if (!app->project) return;
   style = app->project->current_style;
   if ((!style) || (style->obj != source)) return;

   Evas_Object *prop_view = ui_block_property_get(app);
   part = wm_part_by_name_find(style, change->part);
   ui_property_state_unset(prop_view);
   ui_widget_list_part_selected_set(ui_block_widget_list_get(app),
                                    change->part, false);
   if (part)
     {
        if (edje_edit_state_exist(style->obj, change->part, change->state->name,
                                  change->state->value))
          {
             eina_stringshare_replace(&part->curr_state, change->state->name);
             part->curr_state_value = change->state->value;
          }
        else
          {
             eina_stringshare_replace(&part->curr_state, "default");
             part->curr_state_value = 0.0;
          }
        workspace_edit_object_part_state_set(app->workspace, part);
        ui_property_state_set(prop_view, part);
     }
   ui_widget_list_part_selected_set(ui_block_widget_list_get(app), change->part,
                                    true);
}

/* argument 'type' will be used in future, for store special part types data */
static State_Params *
_state_params_save(Evas_Object *obj, const char *part, const char *state,
                   double value, Edje_Part_Type type)
{
   State_Params *state_diff = NULL;
   int r, g, b, a;
   int l, t;

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

   switch (type)
     {
      case EDJE_PART_TYPE_IMAGE:
         state_diff->image.fill.smooth =
            edje_edit_state_fill_smooth_get(obj, part, state, value);
         state_diff->image.fill.type =
            edje_edit_state_fill_type_get(obj, part, state, value);
         state_diff->image.fill.pos_rel_x =
            edje_edit_state_fill_origin_relative_x_get(obj, part, state, value);
         state_diff->image.fill.pos_rel_y =
            edje_edit_state_fill_origin_relative_y_get(obj, part, state, value);
         state_diff->image.fill.pos_abs_x =
            edje_edit_state_fill_origin_offset_x_get(obj, part, state, value);
         state_diff->image.fill.pos_abs_y =
            edje_edit_state_fill_origin_offset_y_get(obj, part, state, value);
         state_diff->image.fill.rel_x =
            edje_edit_state_fill_size_relative_x_get(obj, part, state, value);
         state_diff->image.fill.rel_y =
            edje_edit_state_fill_size_relative_y_get(obj, part, state, value);
         state_diff->image.fill.abs_x =
            edje_edit_state_fill_size_offset_x_get(obj, part, state, value);
         state_diff->image.fill.abs_y =
            edje_edit_state_fill_size_offset_y_get(obj, part, state, value);

         state_diff->image.border.no_fill =
            edje_edit_state_image_border_fill_get(obj, part, state, value);
         edje_edit_state_image_border_get(obj, part, state, value, &l, &r, &t, &b);
         state_diff->image.border.l = l; state_diff->image.border.r = r;
         state_diff->image.border.t = t; state_diff->image.border.b = b;
         state_diff->image.border.t = t; state_diff->image.border.b = b;

         state_diff->image.tweens =
            edje_edit_state_tweens_list_get(obj, part, state, value);
         state_diff->image.normal =
            edje_edit_state_image_get(obj, part, state, value);
      break;
      case EDJE_PART_TYPE_TEXT:
         state_diff->text.text =
            eina_stringshare_add(edje_edit_state_text_get(obj, part, state, value));
         state_diff->text.font =
            eina_stringshare_add(edje_edit_state_font_get(obj, part, state, value));
         state_diff->text.text_source_part =
            eina_stringshare_add(edje_edit_state_text_text_source_get(obj, part,
                                                                  state, value));
         state_diff->text.source_part =
            eina_stringshare_add(edje_edit_state_text_source_get(obj, part,
                                                                 state, value));
         state_diff->text.text_class =
            eina_stringshare_add(edje_edit_state_text_class_get(obj, part,
                                                                state, value));
         state_diff->text.repch =
            eina_stringshare_add(edje_edit_state_text_repch_get(obj, part,
                                                                state, value));
         edje_edit_state_text_size_range_min_max_get(obj, part, state, value,
                                                     &state_diff->text.size_range_min,
                                                     &state_diff->text.size_range_max);

         state_diff->text.size =
            edje_edit_state_text_size_get(obj, part, state, value);
         state_diff->text.align.x =
            edje_edit_state_text_align_x_get(obj, part, state, value);
         state_diff->text.align.y =
            edje_edit_state_text_align_y_get(obj, part, state, value);
         state_diff->text.ellipsis =
            edje_edit_state_text_elipsis_get(obj, part, state, value);
         state_diff->text.fit_x =
            edje_edit_state_text_fit_x_get(obj, part, state, value);
         state_diff->text.fit_y =
            edje_edit_state_text_fit_y_get(obj, part, state, value);
         state_diff->text.min_x =
            edje_edit_state_text_min_x_get(obj, part, state, value);
         state_diff->text.min_y =
            edje_edit_state_text_min_y_get(obj, part, state, value);
         state_diff->text.max_x =
            edje_edit_state_text_max_x_get(obj, part, state, value);
         state_diff->text.max_y =
            edje_edit_state_text_max_y_get(obj, part, state, value);

      break;
      default:
      break;
     }

   return state_diff;
}

/* argument 'type' will be used in future, for restore special part types data */
static Eina_Bool
_state_param_restore(Evas_Object *obj, Eina_Stringshare *part,
                     State_Params *state_diff, Edje_Part_Type type)
{
   Eina_List *l, *l_next;
   Eina_Stringshare *tween = NULL;

   if ((!part) || (!state_diff)) return false;

   if (!edje_edit_state_exist(obj, part, state_diff->name, state_diff->value))
     edje_edit_state_add(obj, part, state_diff->name, state_diff->value);


   edje_edit_state_visible_set(obj, part, state_diff->name,
                               state_diff->value, state_diff->visible);
   edje_edit_state_min_w_set(obj, part, state_diff->name,
                             state_diff->value, state_diff->min.w);
   edje_edit_state_min_h_set(obj, part, state_diff->name,
                             state_diff->value, state_diff->min.h);
   edje_edit_state_max_w_set(obj, part,state_diff->name,
                             state_diff->value, state_diff->max.w);
   edje_edit_state_max_h_set(obj, part,state_diff->name,
                             state_diff->value, state_diff->max.h);
   edje_edit_state_align_x_set(obj, part, state_diff->name,
                               state_diff->value, state_diff->align.x);
   edje_edit_state_align_y_set(obj, part, state_diff->name,
                               state_diff->value, state_diff->align.y);
   edje_edit_state_fixed_w_set(obj, part, state_diff->name,
                               state_diff->value, state_diff->fixed.w);
   edje_edit_state_fixed_h_set(obj, part, state_diff->name,
                               state_diff->value, state_diff->fixed.h);

   edje_edit_state_aspect_min_set(obj, part, state_diff->name,
                                  state_diff->value, state_diff->aspect.w);
   edje_edit_state_aspect_max_set(obj, part, state_diff->name,
                                  state_diff->value,  state_diff->aspect.h);
   edje_edit_state_aspect_pref_set(obj, part, state_diff->name,
                                   state_diff->value, state_diff->aspect.param);

   edje_edit_state_color_class_set(obj, part, state_diff->name,
                                   state_diff->value, state_diff->color_class);
   edje_edit_state_color_set(obj, part, state_diff->name, state_diff->value,
                             state_diff->color.r, state_diff->color.g,
                             state_diff->color.b, state_diff->color.a);
   edje_edit_state_color2_set(obj, part, state_diff->name, state_diff->value,
                             state_diff->color2.r, state_diff->color2.g,
                             state_diff->color2.b, state_diff->color2.a);

   edje_edit_state_rel1_to_x_set(obj, part, state_diff->name,
                                 state_diff->value, state_diff->rel1.to_x);
   edje_edit_state_rel1_to_y_set(obj, part, state_diff->name,
                                 state_diff->value, state_diff->rel1.to_y);
   edje_edit_state_rel1_offset_x_set(obj, part, state_diff->name,
                                     state_diff->value,
                                     state_diff->rel1.offset_x);
   edje_edit_state_rel1_offset_y_set(obj, part, state_diff->name,
                                     state_diff->value,
                                     state_diff->rel1.offset_y);
   edje_edit_state_rel1_relative_x_set(obj, part, state_diff->name,
                                       state_diff->value,
                                       state_diff->rel1.relative_x);
   edje_edit_state_rel1_relative_y_set(obj, part, state_diff->name,
                                       state_diff->value,
                                       state_diff->rel1.relative_y);

   edje_edit_state_rel2_to_x_set(obj, part, state_diff->name,
                                 state_diff->value, state_diff->rel2.to_x );
   edje_edit_state_rel2_to_y_set(obj, part, state_diff->name,
                                 state_diff->value, state_diff->rel2.to_y);
   edje_edit_state_rel2_offset_x_set(obj, part, state_diff->name,
                                     state_diff->value,
                                     state_diff->rel2.offset_x );
   edje_edit_state_rel2_offset_y_set(obj, part, state_diff->name,
                                     state_diff->value,
                                     state_diff->rel2.offset_y);
   edje_edit_state_rel2_relative_x_set(obj, part, state_diff->name,
                                       state_diff->value,
                                       state_diff->rel2.relative_x);
   edje_edit_state_rel2_relative_y_set(obj, part, state_diff->name,
                                       state_diff->value,
                                       state_diff->rel2.relative_y);
   switch (type)
     {
      case EDJE_PART_TYPE_IMAGE:
         edje_edit_state_fill_smooth_set(obj, part, state_diff->name,
                                         state_diff->value,
                                         state_diff->image.fill.smooth);
         edje_edit_state_fill_type_set(obj, part, state_diff->name,
                                       state_diff->value,
                                       state_diff->image.fill.type);
         edje_edit_state_fill_origin_relative_x_set(obj, part, state_diff->name,
                                                    state_diff->value,
                                                    state_diff->image.fill.pos_rel_x);
         edje_edit_state_fill_origin_relative_y_set(obj, part, state_diff->name,
                                                    state_diff->value,
                                                    state_diff->image.fill.pos_rel_y);
         edje_edit_state_fill_origin_offset_x_set(obj, part, state_diff->name,
                                                  state_diff->value,
                                                  state_diff->image.fill.pos_abs_x);
         edje_edit_state_fill_origin_offset_y_set(obj, part, state_diff->name,
                                                  state_diff->value,
                                                  state_diff->image.fill.pos_abs_y);
         edje_edit_state_fill_size_relative_x_set(obj, part, state_diff->name,
                                                  state_diff->value,
                                                  state_diff->image.fill.rel_x);
         edje_edit_state_fill_size_relative_y_set(obj, part, state_diff->name,
                                                  state_diff->value,
                                                  state_diff->image.fill.rel_y);
         edje_edit_state_fill_size_offset_x_set(obj, part, state_diff->name,
                                                state_diff->value,
                                                state_diff->image.fill.abs_x);
         edje_edit_state_fill_size_offset_y_set(obj, part, state_diff->name,
                                                state_diff->value,
                                                state_diff->image.fill.abs_y);

         edje_edit_state_image_border_fill_set(obj, part, state_diff->name,
                                               state_diff->value,
                                               state_diff->image.border.no_fill);
         edje_edit_state_image_border_set(obj, part, state_diff->name,
                                          state_diff->value,
                                          state_diff->image.border.l,
                                          state_diff->image.border.r,
                                          state_diff->image.border.t,
                                          state_diff->image.border.b);

         EINA_LIST_FOREACH_SAFE(state_diff->image.tweens, l, l_next, tween)
           {
              edje_edit_state_tween_add(obj, part, state_diff->name,
                                        state_diff->value, tween);
           }

         edje_edit_state_image_set(obj, part, state_diff->name,
                                   state_diff->value, state_diff->image.normal);
      break;
      case EDJE_PART_TYPE_TEXT:
         edje_edit_state_text_set(obj, part, state_diff->name,
                                  state_diff->value, state_diff->text.text);
         edje_edit_state_font_set(obj, part, state_diff->name,
                                  state_diff->value, state_diff->text.font);
         edje_edit_state_text_text_source_set(obj, part, state_diff->name,
                                              state_diff->value,
                                              state_diff->text.text_source_part);
         edje_edit_state_text_source_set(obj, part, state_diff->name,
                                         state_diff->value,
                                         state_diff->text.source_part);
         edje_edit_state_text_class_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.text_class);
         edje_edit_state_text_repch_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.repch);
         edje_edit_state_text_size_range_min_max_set(obj, part, state_diff->name,
                                                     state_diff->value,
                                                     state_diff->text.size_range_min,
                                                     state_diff->text.size_range_max);

         edje_edit_state_text_size_set(obj, part, state_diff->name,
                                       state_diff->value, state_diff->text.size);
         edje_edit_state_text_align_x_set(obj, part, state_diff->name,
                                          state_diff->value,
                                          state_diff->text.align.x);
         edje_edit_state_text_align_y_set(obj, part, state_diff->name,
                                          state_diff->value,
                                          state_diff->text.align.y);
         edje_edit_state_text_elipsis_set(obj, part, state_diff->name,
                                          state_diff->value,
                                          state_diff->text.ellipsis);
         edje_edit_state_text_fit_x_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.fit_x);
         edje_edit_state_text_fit_y_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.fit_y);
         edje_edit_state_text_min_x_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.min_x);
         edje_edit_state_text_min_y_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.min_y);
         edje_edit_state_text_max_x_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.max_x);
         edje_edit_state_text_max_y_set(obj, part, state_diff->name,
                                        state_diff->value,
                                        state_diff->text.max_y);
      break;
      default:
      break;
     }
   return true;
}

Eina_Bool
_state_redo(Evas_Object *source, State_Diff *change)
{
   Eina_Bool result = false;

   switch(change->diff.action_type)
     {
      case ADD:
         result = _state_param_restore(source, change->part, change->state,
                                       change->type);
      break;
      case DEL:
         result = edje_edit_state_del(source, change->part, change->state->name,
                                      change->state->value);
      break;
      default:
        return false;
      break;
     }
   if (result) _history_ui_state_update(source, change);
   return result;
}

Eina_Bool
_state_undo(Evas_Object *source, State_Diff *change)
{
   Eina_Bool result = false;

   switch(change->diff.action_type)
     {
      case ADD:
         result = edje_edit_state_del(source, change->part, change->state->name,
                                      change->state->value);
      break;
      case DEL:
         result = _state_param_restore(source, change->part, change->state,
                                       change->type);
      break;
      default:
         return false;
      break;
     }

   if (result) _history_ui_state_update(source, change);
   return result;
}

void
_state_change_free(State_Diff *change)
{
   eina_stringshare_del(change->style);
   eina_stringshare_del(change->part);

   eina_stringshare_del(change->diff.description);
   eina_stringshare_del(change->diff.source);
   eina_stringshare_del(change->diff.state);

   if (change->diff.ui_item)
     elm_object_item_del(change->diff.ui_item);

   if (change->state)
     eina_stringshare_del(change->state->name);

   free(change->state);
   free(change);
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
   change->diff.new = eina_stringshare_add(" ");
   change->diff.old = eina_stringshare_add(" ");

   return (Diff *)change;

error:
   _state_change_free(change);
   return NULL;
}
