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

/**
 * @struct _Attribute_Diff
 *
 * @brief Structure that store the diff data for attributes. (like part params).
 * This structure "inherit" from @c _Diff. It's implement with use field Diff.
 * All fields of this structure provide save diff, that can be undo and
 * redo.
 *
 * @ingroup History
 */
struct _Attribute_Diff
{
   Diff diff; /**<  Instance of main diff structure.*/
   Eina_Stringshare *style; /**< Full group name */
   Eina_Stringshare *part; /**< Part name, can be NULL */
   Eina_Stringshare *state; /**< State name, can present only if present part.
                                 Can be NULL */
   double state_value; /**< State value, uses only with field state*/
   void (*func)(Evas_Object *obj, ...); /**< Function that provide change
                                          attribute in edit object*/
   Value_Type param_type; /**< Helper, that make faster search needed
                               struct in union */
   union {
      struct {
         Eina_Stringshare *old;
         Eina_Stringshare *new;
      } string; /**< Save diffs for string values. */
      struct {
         int old;
         int new;
      } integer; /**< Save diffs for integer values. */
      struct {
         double old;
         double new;
      } doubl; /**< Save diffs for double values. */
      struct {
         int old_1;
         int old_2;
         int old_3;
         int old_4;
         int new_1;
         int new_2;
         int new_3;
         int new_4;
      } four; /**< Save diffs for four int values. Usually being used for store colors.*/
   };
};

/*
 * This function updates EFLETE ui accordingly to the given change.
 * It's placed here, because needed access to fields of the Attribute_Diff
 * structure.
 */
static void
_history_ui_attribute_update(Evas_Object *source, Attribute_Diff *change)
{
   Style *style = NULL;
   Part *part = NULL;

   App_Data *app = app_data_get();
   if (!app->project) return;
   style = app->project->current_style;
   if ((!style) || (style->obj != source)) return;

   Evas_Object *prop = ui_block_property_get(app);
   Evas_Object *state_list = ui_block_state_list_get(app);

   if (change->part)
     {
        /* this case mean, that change have action with part attributes */
        part = wm_part_by_name_find(style, change->part);
        if (!part) return;
        if (change->state)
          {
             part->curr_state = change->state;
             part->curr_state_value = change->state_value;
             ui_states_list_data_set(state_list, style, part);
          }
        else if (change->param_type == RENAME)
          evas_object_smart_callback_call(app->workspace, "part,name,changed", part);
        evas_object_smart_callback_call(app->workspace, "part,changed", part);
        evas_object_smart_callback_call(app->workspace, "ws,part,selected",
                                        (void *)change->part);
        workspace_edit_object_part_state_set(app->workspace, part);
        ui_property_part_set(prop, part);
     }
   else
     {
        /* next lines of code update UI of EFLETE for group params change */
        part = ui_widget_list_selected_part_get(
                                 ui_block_widget_list_get(app));
        if (part)
          {
             evas_object_smart_callback_call(app->workspace, "ws,part,unselected",
                                             (void *)part->name);
             workspace_highlight_unset(app->workspace);
          }
        ui_property_part_unset(prop);
        ui_property_style_unset(prop);
        ui_property_style_set(prop, style, app->workspace);
     }
}

Eina_Bool
_attribute_redo(Evas_Object *source, Attribute_Diff *change)
{
   Style *style = NULL;
   Part *part = NULL;

   App_Data *app = app_data_get();
   if (!app->project) return false;
   style = app->project->current_style;
   if ((!style) || (style->obj != source)) return false;

   switch(change->param_type)
    {
     case INT:
        change->state ?
           change->func(source, change->part, change->state,
                        change->state_value, change->integer.new) :
           change->func(source, change->part, change->integer.new);
     break;
     case DOUBLE:
        if (change->state)
           change->func(source, change->part, change->state,
                        change->state_value, change->doubl.new);
        else
          return false;
     break;
     case STRING:
        change->state ?
           change->func(source, change->part, change->state,
                        change->state_value, change->string.new) :
           change->func(source, change->part, change->string.new);
     break;
     case FOUR:
        if (change->state)
           change->func(source, change->part, change->state,
                        change->state_value, change->four.new_1,
                        change->four.new_2, change->four.new_3,
                        change->four.new_4);
        else
          return false;
     break;
     case ONE:
        if (!change->state)
           change->func(source, change->integer.new);
        else
          return false;
     break;
     case RENAME:
        if (!change->state)
          {
             part = wm_part_by_name_find(style, change->part);
             if (!part) return false;

             change->func(source, change->part, change->string.new);
             change->part = change->string.new;
             part->name = change->string.new;
          }
        else
          return false;
     break;
     default:
       ERR("Unsupported param type");
       return false;
     break;
    }

   _history_ui_attribute_update(source, change);
   return true;
}

Eina_Bool
_attribute_undo(Evas_Object *source, Attribute_Diff *change)
{
   Style *style = NULL;
   Part *part = NULL;

   App_Data *app = app_data_get();
   if (!app->project) return false;
   style = app->project->current_style;
   if ((!style) || (style->obj != source)) return false;

   switch(change->param_type)
    {
     case INT:
        change->state ?
           change->func(source, change->part, change->state,
                        change->state_value, change->integer.old) :
           change->func(source, change->part, change->integer.old);
     break;
     case DOUBLE:
        if (change->state)
          change->func(source, change->part, change->state,
                       change->state_value, change->doubl.old);
        else
           return false;
     break;
     case STRING:
        change->state ?
           change->func(source, change->part, change->state,
                        change->state_value, change->string.old) :
           change->func(source, change->part, change->string.old);
     break;
     case FOUR:
        if (change->state)
          change->func(source, change->part, change->state,
                       change->state_value, change->four.old_1,
                       change->four.old_2, change->four.old_3,
                       change->four.old_4);
        else
           return false;
     break;
     case ONE:
        if (!change->state)
          change->func(source, change->integer.old);
        else
          return false;
     break;
     case RENAME:
        if (!change->state)
          {
             part =   wm_part_by_name_find(style, change->part);
             if (!part) return false;

             change->func(source, change->part, change->string.old);
             change->part = change->string.old;
             part->name = change->string.old;
          }
        else return false;
     break;
     default:
       ERR("Unsupported module type, that store diff");
       return false;
     break;
    }

   _history_ui_attribute_update(source, change);
   return true;
}

void
_attribute_change_free(Attribute_Diff *change)
{
   if (!change) return;

   eina_stringshare_del(change->style);
   eina_stringshare_del(change->part);
   eina_stringshare_del(change->state);

   eina_stringshare_del(change->diff.description);
   eina_stringshare_del(change->diff.source);
   eina_stringshare_del(change->diff.state);
   eina_stringshare_del(change->diff.new);
   eina_stringshare_del(change->diff.old);

   if (change->diff.ui_item)
     elm_object_item_del(change->diff.ui_item);

   if (change->param_type == STRING)
     {
        eina_stringshare_del(change->string.old);
        eina_stringshare_del(change->string.new);
     }
   free(change);
   change = NULL;
}

Diff *
_attribute_change_new(va_list list)
{
   Attribute_Diff *change = NULL;
   char *string = NULL;

   change = (Attribute_Diff *)mem_calloc(1, sizeof(Attribute_Diff));
   change->diff.module_type = PROPERTY;
   change->diff.action_type = va_arg(list, Action);
   change->param_type = (Value_Type)va_arg(list, Value_Type);
   switch(change->param_type)
     {
      case ONE:
      case INT:
         change->integer.old = (int)va_arg(list, int);
         change->integer.new = (int)va_arg(list, int);
         change->diff.new = eina_stringshare_printf("%d", change->integer.new);
         change->diff.old = eina_stringshare_printf("%d", change->integer.old);
      break;
      case DOUBLE:
         change->doubl.old = (double)va_arg(list, double);
         change->doubl.new = (double)va_arg(list, double);
         change->diff.new = eina_stringshare_printf("%.3f", change->doubl.new);
         change->diff.old = eina_stringshare_printf("%.3f", change->doubl.old);
      break;
      case RENAME:
      case STRING:
         string = (char *)va_arg(list, char *);
         change->string.old = eina_stringshare_add(string);
         string = (char *)va_arg(list, char *);
         change->string.new = eina_stringshare_add(string);
         if (change->string.new)
           change->diff.new = eina_stringshare_add(change->string.new);
         else
           change->diff.new = eina_stringshare_add("None");
         if (change->string.old)
           change->diff.old = eina_stringshare_add(change->string.old);
         else
           change->diff.old = eina_stringshare_add("None");
      break;
      case FOUR:
         change->four.old_1 = (int)va_arg(list, int);
         change->four.old_2 = (int)va_arg(list, int);
         change->four.old_3 = (int)va_arg(list, int);
         change->four.old_4 = (int)va_arg(list, int);
         change->four.new_1 = (int)va_arg(list, int);
         change->four.new_2 = (int)va_arg(list, int);
         change->four.new_3 = (int)va_arg(list, int);
         change->four.new_4 = (int)va_arg(list, int);

         change->diff.new = eina_stringshare_printf("%d %d %d %d",
                              change->four.new_1, change->four.new_2,
                              change->four.new_3, change->four.new_4);

         change->diff.old = eina_stringshare_printf("%d %d %d %d",
                              change->four.old_1, change->four.old_2,
                              change->four.old_3, change->four.old_4);

      break;
      default:
          ERR("Unsupported value type.");
          goto error;
     }
   change->style = eina_stringshare_add((char *)va_arg(list, char *));
   if (!change->style) goto error;
   change->func = (void *)va_arg(list, void *);
   if (!change->func) goto error;
   change->diff.description = eina_stringshare_add((char *)va_arg(list, char *));

   change->part = eina_stringshare_add((char *)va_arg(list, char *));
   if (change->part)
     {
        change->diff.source = eina_stringshare_add(change->part);
        change->state = eina_stringshare_add((char *)va_arg(list, char *));
        if (change->state)
          {
             change->state_value = (double)va_arg(list, double);
             change->diff.state = eina_stringshare_printf("%s %.2f",
                                                          change->state,
                                                          change->state_value);
          }
     }
   else
     change->diff.source = eina_stringshare_add("Group");

   return (Diff *)change;

error:
   _attribute_change_free(change);
   return NULL;
}
