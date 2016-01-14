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

static void
_on_popup_swallow_check(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *) data;
   Evas_Object *content;
   char *pointer = NULL;
   Eina_Stringshare *but_swallow = NULL, *title_swallow = NULL;

   pointer = strstr(part->name, "button");
   if (pointer) but_swallow = eina_stringshare_printf("button%c",
                                                      pointer[sizeof(pointer) - 2]);
   if (!strcmp(part->name, "elm.swallow.title.icon"))
     title_swallow = eina_stringshare_add("title,icon");

   int content_type = part->swallow_content;

   if (part->change)
     {
        if ((content_type == CONTENT_NONE) && (part->object))
          {
             if (but_swallow)
               {
                  content = elm_object_part_content_get(object, but_swallow);
                  elm_object_part_content_unset(object, but_swallow);
               }
             else if (title_swallow)
               {
                  content = elm_object_part_content_get(object, part->name);
                  elm_object_part_content_unset(object, part->name);
               }
             else
               {
                  content = elm_object_content_get(object);
                  elm_object_content_unset(object);
               }
             evas_object_del(content);
             content = NULL;
             part->object = NULL;
          }

        if (but_swallow)
          {
             part->object = elm_button_add(object);
             elm_object_text_set(part->object, "OK");
             elm_object_part_content_set(object, but_swallow, part->object);
          }
        else if (title_swallow)
          {
             part->object = object_generate(part, object);
             elm_object_part_content_set(object, title_swallow, part->object);
          }
        else
          {
             part->object = object_generate(part, object);
             elm_object_content_set(object, part->object);
          }
        part->change = false;
     }

   if (part->object)
     {
        evas_object_color_set(part->object,
                              part->r,
                              part->g,
                              part->b,
                              part->a);

        evas_object_size_hint_min_set(part->object,
                                      part->min_w,
                                      part->min_h);
        evas_object_size_hint_max_set(part->object,
                                      part->max_w,
                                      part->max_h);
     }

   eina_stringshare_del(but_swallow);
   eina_stringshare_del(title_swallow);
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

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_popup_swallow_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, send_signal, object);

   elm_object_style_set(object, style_name);
   eina_stringshare_del(style_name);
   return object;
}
