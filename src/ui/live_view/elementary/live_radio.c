/**
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

#include "live_elementary_widgets.h"

static void
_on_radio_swallow_check(void *data,
                        Evas_Object *obj,
                        void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *radio_obj = NULL;
   Eina_List* radio_list = elm_box_children_get(data);
   Eina_List *l = NULL;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     {
        EINA_LIST_FOREACH(radio_list, l, radio_obj)
          {
             rect = evas_object_rectangle_add(radio_obj);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             elm_object_part_content_set(radio_obj, part_name, rect);
          }
     }
   else
     {
        EINA_LIST_FOREACH(radio_list, l, radio_obj)
          {
             rect = elm_object_part_content_unset(radio_obj, part_name);
             evas_object_del(rect);
          }
     }
   eina_list_free(radio_list);
}

static void
_on_radio_text_check(void *data,
                     Evas_Object *obj,
                     void *ei __UNUSED__)
{
   Evas_Object *radio_obj = NULL;
   Eina_List* radio_list = elm_box_children_get(data);
   Eina_List *l = NULL;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     {
        EINA_LIST_FOREACH(radio_list, l, radio_obj)
          elm_object_part_text_set(radio_obj, part_name, _("Text Example"));
     }
   else
     {
        EINA_LIST_FOREACH(radio_list, l, radio_obj)
          elm_object_part_text_set(radio_obj, part_name, "");
     }
   eina_list_free(radio_list);
}

static void
_radio_send_signal(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *radio_obj = NULL;
   Eina_List* radio_list = elm_box_children_get(data);
   Eina_List *l = NULL;

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   EINA_LIST_FOREACH(radio_list, l, radio_obj)
     elm_layout_signal_emit(radio_obj, name, source);
}

Evas_Object *
widget_radio_create(Evas_Object *parent, const char *class __UNUSED__, const char *style __UNUSED__)
{
   Evas_Object *object, *rd, *rdg;

   object = elm_box_add(parent);
   evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   RADIO_ADD(parent, rd, 1, NULL);
   elm_box_pack_end(object, rd);
   rdg = rd;
   RADIO_ADD(parent, rd, 2, NULL);
   elm_radio_group_add(rd, rdg);
   elm_box_pack_end(object, rd);
   RADIO_ADD(parent, rd, 3, NULL);
   elm_radio_group_add(rd, rdg);
   elm_box_pack_end(object, rd);

   elm_radio_value_set(rdg, 2);

   evas_object_data_set(object, SWALLOW_FUNC, _on_radio_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_radio_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _radio_send_signal);

   return object;
}
