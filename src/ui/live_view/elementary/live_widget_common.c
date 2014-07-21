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

void
on_swallow_check(void *data,
                 Evas_Object *obj,
                 void *ei __UNUSED__)
{
   Evas_Object *rect = NULL;

   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     {
        rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, RECT_COLOR);
        elm_object_part_content_set(object, part_name, rect);
     }
   else
     {
        rect = elm_object_part_content_unset(object, part_name);
        evas_object_del(rect);
     }
}

void
on_text_check(void *data,
              Evas_Object *obj,
              void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     elm_object_part_text_set(object, part_name, _("Look at it! This is absolutely and totally text"));
   else
     elm_object_part_text_set(object, part_name, NULL);
}
