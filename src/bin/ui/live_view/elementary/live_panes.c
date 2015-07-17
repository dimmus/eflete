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

#include "live_elementary_widgets.h"

static Eina_Bool
_panes_orient_get(const char *class)
{
   assert(class != NULL);

   Eina_Bool horizontal = true;

   if (strcmp(class, "horizontal") == 0)
     horizontal = true;
   else if (strcmp(class, "vertical") == 0)
     horizontal = false;

   return horizontal;
}

Evas_Object *
widget_panes_create(Evas_Object *parent, const Style *style)
{
   assert(parent != NULL);
   assert(style != NULL);

   Eina_Stringshare *class;
   Eina_Stringshare *style_name;
   standard_widget_name_parse(style->full_group_name, NULL, &class, &style_name);

   Evas *e = evas_object_evas_get(parent);
   Evas_Object *test_right = evas_object_rectangle_add(e);
   Evas_Object *test_left = evas_object_rectangle_add(e);

   evas_object_color_set(test_right,
                         254 * 255 / 255,
                         212 * 255 / 255,
                         3 * 255 / 255,
                         255);

   evas_object_color_set(test_left,
                         2 * 255 / 255,
                         90 * 255 / 255,
                         188 * 255 / 255,
                         255);

   Evas_Object *object = elm_panes_add(parent);

   elm_panes_horizontal_set(object, _panes_orient_get(class));
   elm_object_part_content_set(object, "right", test_right);
   elm_object_part_content_set(object, "left", test_left);

   evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   elm_object_style_set(object, style_name);

   eina_stringshare_del(class);
   eina_stringshare_del(style_name);
   return object;
}
