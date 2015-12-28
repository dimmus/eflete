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
#include "live_elementary_widgets.h"

static void
_on_list_swallow_check(void *data,
                       Evas_Object *obj,
                       void *ei __UNUSED__)
{
   Evas_Object *rect, *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   Elm_Object_Item *item = NULL;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *lists_part_name = NULL;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");
   item = elm_list_first_item_get(object);

   if (!strcmp(part_name, "elm.swallow.icon"))
     lists_part_name = "start";
   else if (!strcmp(part_name, "elm.swallow.end"))
     lists_part_name = "end";

   while (item && lists_part_name)
     {
        if (elm_check_state_get(obj))
          {
             rect = elm_object_item_part_content_unset(item, lists_part_name);
             if (rect) evas_object_del(rect);
             rect = evas_object_rectangle_add(object);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             elm_object_item_part_content_set(item, lists_part_name, rect);
             item_list = elm_box_children_get(pd->prop_swallow.swallows);

             EINA_LIST_FOREACH(item_list, it, ch)
               {
                  if (elm_check_state_get(ch) == false)
                    all_checks = false;
               }
             if (all_checks)
               elm_check_state_set(check, true);
             eina_list_free(item_list);
          }
        else
          {
             rect = elm_object_item_part_content_unset(item, lists_part_name);
             evas_object_del(rect);
             if (elm_check_state_get(check)) elm_check_state_set(check, false);
          }

        item = elm_list_item_next(item);
     }
}

static void
_on_list_text_check(void *data,
                    Evas_Object *obj,
                    void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   Elm_Object_Item *item = NULL;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");
   item = elm_list_first_item_get(object);

   while (item)
     {
        if (elm_check_state_get(obj))
          {
             elm_object_item_part_text_set(item, part_name, _("Text Example"));
             item_list = elm_box_children_get(pd->prop_text.texts);

             EINA_LIST_FOREACH(item_list, it, ch)
               {
                  if (elm_check_state_get(ch) == false)
                    all_checks = false;
               }
             if (all_checks)
               elm_check_state_set(check, true);
             eina_list_free(item_list);
          }
        else
          {
             elm_object_item_part_text_set(item, part_name, "");
             if (elm_check_state_get(check)) elm_check_state_set(check, false);
          }

        item = elm_list_item_next(item);
     }
}

static void
_list_send_signal(void *data,
                  Evas_Object *obj,
                  void *ei __UNUSED__)
{
   Elm_Object_Item *item = NULL;

   assert(data != NULL);

   item = elm_list_first_item_get(data);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   while (item)
     {
        elm_object_item_signal_emit(item, name, source);
        item = elm_list_item_next(item);
     }
}

Evas_Object *
widget_list_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   int i = 0;
   Evas_Object *object = elm_list_add(parent);

   for (i = 0; i < ELEMENTS_COUNT; i++)
     elm_list_item_append(object, _("No icons"), NULL, NULL, NULL, NULL);

   if (strstr(group->class, "h_") == group->class)
     elm_list_horizontal_set(object, true);

   if (strstr(group->class, "compress"))
     elm_list_mode_set(object, ELM_LIST_COMPRESS);
   else
     elm_list_mode_set(object, ELM_LIST_SCROLL);

   evas_object_data_set(object, SWALLOW_FUNC, _on_list_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_list_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _list_send_signal);

   elm_object_style_set(object, group->style);

   return object;
}
