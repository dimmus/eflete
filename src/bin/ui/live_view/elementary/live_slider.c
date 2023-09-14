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

Evas_Object *
widget_slider_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   Eina_Stringshare *style_name = eina_stringshare_add(group->style);
   char **style_parsed = NULL;
   unsigned int count_split = 0;

   Evas_Object *object = elm_slider_add(parent);
   elm_slider_unit_format_set(object, "%1.2f units");
   elm_slider_indicator_format_set(object, "%1.2f");
   elm_slider_min_max_set(object, 0, 10);

   if (!strcmp(group->class, "range"))
     {
        elm_slider_range_enabled_set(object, true);
        if (!strncmp(style_name, "vertical", strlen_safe("vertical")))
          elm_slider_horizontal_set(object, false);
        elm_slider_range_set(object, 2, 8);
     }
   else if (!strcmp(group->class, "vertical"))
     elm_slider_horizontal_set(object, false);

   style_parsed = eina_str_split_full(style_name, "/", 2, &count_split);
   if (count_split == 2)
     {
        if (!strcmp(style_parsed[0], "popup"))
          eina_stringshare_replace(&style_name, style_parsed[1]);
     }
   free(style_parsed[0]);
   free(style_parsed);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, style_name);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   eina_stringshare_del(style_name);
   return object;
}
