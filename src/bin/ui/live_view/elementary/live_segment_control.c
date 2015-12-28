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
_on_sc_swallow_check(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *ei __UNUSED__)
{
   TODO("Remake on_swallow_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

static void
_on_sc_text_check(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *ei __UNUSED__)
{
   TODO("Remake on_text_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
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

   assert(name != NULL);
   assert(source != NULL);

   for (i = 0; i < 3; i++)
     {
        item = elm_segment_control_item_get(data, i);
        elm_object_item_signal_emit(item, name, source);
     }
}

Evas_Object *
widget_segment_control_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_segment_control_add(parent);
   elm_segment_control_item_insert_at(object, NULL, NULL, 1);
   elm_segment_control_item_insert_at(object, NULL, NULL, 2);
   elm_segment_control_item_insert_at(object, NULL, NULL, 3);

   evas_object_data_set(object, SWALLOW_FUNC, _on_sc_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_sc_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _sc_send_signal);

   elm_object_style_set(object, group->style);

   return object;
}
