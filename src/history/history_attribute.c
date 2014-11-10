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

Eina_Bool
_attribute_redo(Evas_Object *source __UNUSED__, Attribute_Diff *change __UNUSED__)
{
   return false;
}

Eina_Bool
_attribute_undo(Evas_Object *source, Attribute_Diff *change)
{
   switch(change->param_type)
    {
     case INT:
        change->state ?
           change->func(source, change->part, change->state,
                        change->state_value, change->integer.old) :
           change->func(source, change->part, change->integer.old);
     break;
     default:
       ERR("Unsupported module type, that store diff");
       return false;
     break;
    }
   return true;
}

void
_attribute_change_free(Attribute_Diff *change)
{
   if (!change) return;

   eina_stringshare_del(change->style);
   eina_stringshare_del(change->part);
   eina_stringshare_del(change->state);

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
      break;
      case DOUBLE:
         change->doubl.old = (double)va_arg(list, double);
         change->doubl.new = (double)va_arg(list, double);
      break;
      case RENAME:
      case STRING:
         string = (char *)va_arg(list, char *);
         if (!string) string = "";
         change->string.old = eina_stringshare_add(string);
         if (!string) string = "";
         change->string.new = eina_stringshare_add(string);
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
        change->state = eina_stringshare_add((char *)va_arg(list, char *));
        if (change->state)
          change->state_value = (double)va_arg(list, double);
     }

   return (Diff *)change;

error:
   _attribute_change_free(change);
   return NULL;
}
