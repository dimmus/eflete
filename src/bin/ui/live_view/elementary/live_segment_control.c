/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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
#include "live_view_prop.h"

static void
_on_sc_swallow_check(void *data,
                       Evas_Object *obj,
                       void *ei __UNUSED__)
{
   Evas_Object *rect, *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   const char *text = NULL;
   int i = 0, index = -1;
   Elm_Object_Item *item = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *object = pd->live_object;
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");
   item = elm_segment_control_item_selected_get(object);
   if (item) index = elm_segment_control_item_index_get(item);
   for (i = 0; i < 3; i++)
     {
        text = elm_segment_control_item_label_get(object, i);
        if (elm_check_state_get(obj))
          {
             rect = elm_segment_control_item_icon_get(object, i);
             if (rect) evas_object_del(rect);
             rect = evas_object_rectangle_add(object);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             elm_segment_control_item_del_at(object, i);
             elm_segment_control_item_insert_at(object, rect, text, 0);

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
             rect = elm_segment_control_item_icon_get(object, i);
             if (rect) evas_object_del(rect);
             elm_segment_control_item_del_at(object, i);
             elm_segment_control_item_insert_at(object, NULL, text, 0);
             if (elm_check_state_get(check)) elm_check_state_set(check, false);
          }
     }
   if (index >= 0)
     {
        item = elm_segment_control_item_get(object, index);
        elm_segment_control_item_selected_set(item, true);
     }
}

static void
_on_sc_text_check(void *data,
                  Evas_Object *obj,
                  void *ei __UNUSED__)
{
   Evas_Object *rect, *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   int i = 0, index = -1;
   Elm_Object_Item *item = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *object = pd->live_object;
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   item = elm_segment_control_item_selected_get(object);
   if (item) index = elm_segment_control_item_index_get(item);
   for (i = 0; i < 3; i++)
     {
        rect = elm_segment_control_item_icon_get(object, i);
        if (rect)
          {
             evas_object_del(rect);
             rect = evas_object_rectangle_add(object);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
          }
        if (elm_check_state_get(obj))
          {
             elm_segment_control_item_del_at(object, i);
             elm_segment_control_item_insert_at(object, rect, _("Text Example"), 0);

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
             elm_segment_control_item_del_at(object, i);
             elm_segment_control_item_insert_at(object, rect, NULL, 0);
             if (elm_check_state_get(check)) elm_check_state_set(check, false);
          }
     }
   if (index >= 0)
     {
        item = elm_segment_control_item_get(object, index);
        elm_segment_control_item_selected_set(item, true);
     }
}

static void
_sc_send_signal(void *data,
                Evas_Object *obj,
                void *ei __UNUSED__)
{
   Elm_Object_Item *item = NULL;
   int i = 0;

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   for (i = 0; i < 3; i++)
     {
        item = elm_segment_control_item_get(data, i);
        elm_object_item_signal_emit(item, name, source);
     }
}

Evas_Object *
widget_segment_control_create(Evas_Object *parent, const Style *style)
{
   Eina_Stringshare *style_name;
   standard_widget_name_parse(style->full_group_name, NULL, NULL, &style_name);

   Evas_Object *object = elm_segment_control_add(parent);
   elm_segment_control_item_insert_at(object, NULL, NULL, 1);
   elm_segment_control_item_insert_at(object, NULL, NULL, 2);
   elm_segment_control_item_insert_at(object, NULL, NULL, 3);

   evas_object_data_set(object, SWALLOW_FUNC, _on_sc_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_sc_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _sc_send_signal);

   elm_object_style_set(object, style_name);

   eina_stringshare_del(style_name);
   return object;
}
