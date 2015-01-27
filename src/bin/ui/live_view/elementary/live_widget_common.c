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

void
on_swallow_check(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *check = NULL, *item, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *object = pd->live_object;
   const char *part_name = evas_object_data_get(obj, PART_NAME);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, RECT_COLOR);
        elm_object_part_content_set(object, part_name, rect);
        item_list = elm_box_children_get(pd->prop_swallow.swallows);

        EINA_LIST_FOREACH(item_list, it, item)
          {
             ch = elm_object_part_content_get(item, "info");
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
        eina_list_free(item_list);
     }
   else
     {
        rect = elm_object_part_content_unset(object, part_name);
        evas_object_del(rect);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

void
on_text_check(void *data,
              Evas_Object *obj,
              void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *item, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *object = pd->live_object;
   const char *part_name = evas_object_data_get(obj, PART_NAME);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        elm_object_part_text_set(object, part_name,
                                 _("Look at it! This is absolutely and totally text"));
        item_list = elm_box_children_get(pd->prop_text.texts);

        EINA_LIST_FOREACH(item_list, it, item)
          {
             ch = elm_object_part_content_get(item, "info");
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
        eina_list_free(item_list);
     }
   else
     {
        elm_object_part_text_set(object, part_name, "");
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

void
send_signal(void *data,
             Evas_Object *obj,
             void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;
   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   elm_layout_signal_emit(object, name, source);
}
