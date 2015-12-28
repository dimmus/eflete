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

#include "live_view_prop.h"
#include "live_elementary_widgets.h"

static void
_on_popup_swallow_check(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   char *pointer = NULL;
   Evas_Object *btn;
   Eina_Stringshare *but_swallow = NULL, *title_swallow = NULL;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");
   pointer = strstr(part_name, "button");
   if (pointer) but_swallow = eina_stringshare_printf("button%c",
                                                      pointer[sizeof(pointer) - 2]);
   if (!strcmp(part_name, "elm.swallow.title.icon"))
     title_swallow = eina_stringshare_add("title,icon");

   if (elm_check_state_get(obj))
     {
        if (but_swallow)
          {
             btn = elm_button_add(object);
             elm_object_text_set(btn, "OK");
             elm_object_part_content_set(object, but_swallow, btn);
          }
        else if (title_swallow)
          {
             rect = evas_object_rectangle_add(object);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             evas_object_size_hint_min_set(rect, 25, 25);
             elm_object_part_content_set(object, title_swallow, rect);
          }
        else
          {
             rect = evas_object_rectangle_add(object);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             evas_object_size_hint_min_set(rect, 25, 25);
             elm_object_content_set(object, rect);
          }
        item_list = elm_box_children_get(pd->prop_swallow.swallows);

        EINA_LIST_FOREACH(item_list, it, ch)
          {
             if ((elm_check_state_get(ch) == false)
                 && !elm_object_disabled_get(ch))
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
     }
   else
     {
        if (but_swallow)
          {
             rect = elm_object_part_content_get(object, but_swallow);
             elm_object_part_content_unset(object, but_swallow);
          }
        else if (title_swallow)
          {
             rect = elm_object_part_content_get(object, part_name);
             elm_object_part_content_unset(object, part_name);
          }
        else
          {
             rect = elm_object_content_get(object);
             elm_object_content_unset(object);
          }
        evas_object_del(rect);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
        eina_stringshare_del(but_swallow);
        eina_stringshare_del(title_swallow);
     }
}

Evas_Object *
widget_popup_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Eina_Stringshare *style_name = eina_stringshare_add(group->style);
   char **style_parsed = NULL;
   unsigned int count_split = 0;

   Evas_Object *object = elm_popup_add(parent);
   elm_object_part_text_set(object, "title,text", "");

   style_parsed = eina_str_split_full(style_name, "/", 2, &count_split);
   if (count_split == 2)
     {
        if (strstr(style_parsed[0], "popup"))
          eina_stringshare_replace(&style_name, style_parsed[1]);
     }
   free(style_parsed[0]);
   free(style_parsed);

   evas_object_data_set(object, SWALLOW_FUNC, _on_popup_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   elm_object_style_set(object, style_name);
   eina_stringshare_del(style_name);
   return object;
}
