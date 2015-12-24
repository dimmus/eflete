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

#include "live_view_prop.h"

static void
_on_gengrid_swallow_check(void *data,
                          Evas_Object *obj,
                          void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *ch;
   Eina_List *item_list = NULL;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   Eina_List *part_list = evas_object_data_get(object, SWALLOW_LIST);
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   if (elm_check_state_get(obj) && (!eina_list_data_find(part_list, part_name)))
     {
        part_list =  eina_list_append(part_list, part_name);
        item_list = elm_box_children_get(pd->prop_swallow.swallows);
        EINA_LIST_FREE(item_list, ch)
          {
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
     }
   else if (!elm_check_state_get(obj) && (eina_list_data_find(part_list, part_name)))
     {
        part_list = eina_list_remove(part_list, part_name);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }

   evas_object_data_set(object, SWALLOW_LIST, part_list);

   Elm_Object_Item *item = NULL;
   item = elm_gengrid_first_item_get(object);
   while (item)
     {
        elm_gengrid_item_update(item);
        item = elm_gengrid_item_next_get(item);
     }
}

static void
_on_gengrid_text_check(void *data,
                       Evas_Object *obj,
                       void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *ch;
   Eina_List *item_list = NULL;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   Eina_List *part_list = evas_object_data_get(object, TEXT_LIST);
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   if (elm_check_state_get(obj) && (!eina_list_data_find(part_list, part_name)))
     {
        part_list =  eina_list_append(part_list, part_name);
        item_list = elm_box_children_get(pd->prop_text.texts);
        EINA_LIST_FREE(item_list, ch)
          {
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
     }
   else if (!elm_check_state_get(obj) && (eina_list_data_find(part_list, part_name)))
     {
        part_list = eina_list_remove(part_list, part_name);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }

   evas_object_data_set(object, TEXT_LIST, part_list);

   Elm_Object_Item *item = NULL;
   item = elm_gengrid_first_item_get(object);
   while (item)
     {
        elm_gengrid_item_update(item);
        item = elm_gengrid_item_next_get(item);
     }
}

static void
_gengrid_send_signal(void *data,
                     Evas_Object *obj,
                     void *ei __UNUSED__)
{
   Elm_Object_Item *item = NULL;

   assert(data != NULL);

   item = elm_gengrid_first_item_get(data);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   while (item)
     {
        elm_object_item_signal_emit(item, name, source);
        item = elm_gengrid_item_next_get(item);
     }
}


/*********** GEN GRID CREATING FUNCTIONS ****************/
static char *
_grid_text_get(void        *data,
               Evas_Object *obj,
               const char  *part)
{
   Eina_List *part_list = evas_object_data_get(obj, TEXT_LIST);
   Eina_List *l = NULL;
   const char *part_name = NULL;
   const char *text = (char *) data;

   EINA_LIST_FOREACH(part_list, l, part_name)
     {
        if (!strcmp(part_name, part))
          return strdup(text);
     }

   return NULL;
}

static Evas_Object *
_grid_content_get(void *data __UNUSED__,
                  Evas_Object *obj,
                  const char  *part)
{
   Eina_List *part_list = evas_object_data_get(obj, SWALLOW_LIST);
   Eina_List *l = NULL;
   const char *part_name = NULL;

   EINA_LIST_FOREACH(part_list, l, part_name)
     {
        if (!strcmp(part_name, part))
          {
             Evas_Object *rect = evas_object_rectangle_add(obj);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             return rect;
          }
     }

   return NULL;
}

static Evas_Object *
_create_gengrid(Evas_Object *obj, Eina_Bool item_style, const char *style)
{
   Elm_Gengrid_Item_Class *ic = NULL;
   int i;
   Evas_Object *grid = NULL;
   double scale = elm_config_scale_get();

   assert(obj != NULL);
   assert(style != NULL);

   grid = elm_gengrid_add(obj);
   elm_gengrid_item_size_set(grid, scale * 100, scale * 100);
   evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   ic = elm_gengrid_item_class_new();
   ic->func.text_get = _grid_text_get;
   ic->func.content_get = _grid_content_get;
   ic->func.state_get = NULL;
   ic->func.del = NULL;

   if (item_style)
     {
        char **c;

        c = eina_str_split(style, "/", 2);
        ic->item_style = strdup(c[0]);
        elm_object_style_set(grid, c[1]);

        free(c[0]);
        free(c);
     }
   else
     {
        ic->item_style = strdup(style);
        elm_object_style_set(grid, style);
     }

   for (i = 0; i < 40; i++)
     elm_gengrid_item_append(grid, ic, eina_stringshare_printf("Item #%d", i),
                             NULL, NULL);

   elm_gengrid_item_class_free(ic);
   return grid;
}
/*********** GEN GRID CREATING FUNCTIONS ****END*********/

Evas_Object *
widget_gengrid_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *object = NULL;
   Eina_Bool item_style;
   Eina_List *swallow_list = NULL, *text_list = NULL;

   item_style = strcmp(group->class, "item") == 0;
   object = _create_gengrid(parent, item_style, group->style);

   evas_object_data_set(object, SWALLOW_FUNC, _on_gengrid_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_gengrid_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _gengrid_send_signal);

   evas_object_data_set(object, SWALLOW_LIST, swallow_list);
   evas_object_data_set(object, TEXT_LIST, text_list);

   return object;
}
