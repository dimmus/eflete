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
widget_panes_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

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

   elm_panes_horizontal_set(object, _panes_orient_get(group->class));
   elm_object_part_content_set(object, "right", test_right);
   elm_object_part_content_set(object, "left", test_left);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
