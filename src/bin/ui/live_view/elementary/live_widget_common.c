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

Eina_Bool
standard_widget_name_parse(const char *full_group_name,
                           Eina_Stringshare **widget,
                           Eina_Stringshare **class,
                           Eina_Stringshare **style_name)
{
   char **c;
   unsigned int count;

   assert(full_group_name != NULL);

   c = eina_str_split_full(full_group_name, "/", 4, &count);

   if (count < 4)
     {
        if (widget) *widget = NULL;
        if (class) *class = NULL;
        if (style_name) *style_name = NULL;

        free(c[0]);
        free(c);
        return false;
     }

   if (widget) *widget = eina_stringshare_add(c[1]);
   if (class) *class = eina_stringshare_add(c[2]);
   if (style_name) *style_name = eina_stringshare_add(c[3]);

   free(c[0]);
   free(c);
   return true;
}

void
on_swallow_check(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, RECT_COLOR);
        elm_object_part_content_set(object, part_name, rect);
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
        rect = elm_object_part_content_unset(object, part_name);
        evas_object_del(rect);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

void
on_text_check(void *data,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Part_ *part = (Part_ *)ei;
   Evas_Object *object = (Evas_Object *) data;

   elm_object_part_text_set(object, part->name, part->content);
}

void
send_signal(void *data,
             Evas_Object *obj,
             void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;

   assert(data != NULL);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   elm_layout_signal_emit(object, name, source);
}
