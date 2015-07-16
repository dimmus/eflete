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


Evas_Object *
widget_slider_create(Evas_Object *parent, const Style *style)
{
   assert(parent != NULL);
   assert(style != NULL);

   Eina_Stringshare *class;
   Eina_Stringshare *style_name;
   char **style_parsed = NULL;
   unsigned int count_split = 0;
   standard_widget_name_parse(style->full_group_name, NULL, &class, &style_name);

   Evas_Object *object = elm_slider_add(parent);
   if (!strcmp(class, "vertical"))
     elm_slider_horizontal_set(object, false);

   style_parsed = eina_str_split_full(style_name, "/", 2, &count_split);
   if (count_split == 2)
     {
        if (!strcmp(style_parsed[0], "popup"))
          eina_stringshare_replace(&style_name, style_parsed[1]);
     }
   free(style_parsed[0]);
   free(style_parsed);

   elm_slider_unit_format_set(object, "%1.2f units");
   elm_slider_indicator_format_set(object, "%1.2f");
   elm_slider_min_max_set(object, 0, 10);

   evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   elm_object_style_set(object, style_name);

   eina_stringshare_del(class);
   eina_stringshare_del(style_name);
   return object;
}
