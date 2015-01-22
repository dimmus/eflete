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
void
on_layout_swallow_check(void *data,
                        Evas_Object *obj,
                        void *ei __UNUSED__)
{
   Evas_Object *rect = NULL;

   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     {
        if (edje_object_part_swallow_get(object, part_name))
          return;
        rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, RECT_COLOR);
        edje_object_part_swallow(object, part_name, rect);
     }
   else
     {
        rect = edje_object_part_swallow_get(object, part_name);
        if (!rect) return;
        edje_object_part_unswallow(object, rect);
        evas_object_del(rect);
     }
}

void
on_layout_text_check(void *data,
                     Evas_Object *obj,
                     void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;
   const char *part_name = evas_object_data_get(obj, PART_NAME);

   if (elm_check_state_get(obj))
     edje_object_part_text_set(object, part_name,
                               _("Look at it! This is absolutely and totally text"));
   else
     edje_object_part_text_set(object, part_name, "");
}

void
send_layout_signal(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;
   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   edje_object_signal_emit(object, name, source);
}

Evas_Object *
layout_custom_create(Evas_Object *parent)
{
   Evas_Object *object = edje_object_add(evas_object_evas_get(parent));

   evas_object_data_set(object, SWALLOW_FUNC, on_layout_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_layout_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_layout_signal);

   return object;
}
