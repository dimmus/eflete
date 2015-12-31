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

static void
_on_radio_swallow_check(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   TODO("Remake on_swallow_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

static void
_on_radio_text_check(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   TODO("Remake on_text_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

static void
_radio_send_signal(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *radio_obj = NULL;

   assert(data != NULL);

   Eina_List* radio_list = elm_box_children_get(data);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   EINA_LIST_FREE(radio_list, radio_obj)
     elm_layout_signal_emit(radio_obj, name, source);
}

Evas_Object *
widget_radio_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object, *rd, *rdg;

   object = elm_box_add(parent);
   evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   RADIO_ADD(parent, rd, 1, NULL);
   elm_box_pack_end(object, rd);
   rdg = rd;
   elm_object_style_set(rd, group->style);
   RADIO_ADD(parent, rd, 2, NULL);
   elm_radio_group_add(rd, rdg);
   elm_box_pack_end(object, rd);
   elm_object_style_set(rd, group->style);
   RADIO_ADD(parent, rd, 3, NULL);
   elm_radio_group_add(rd, rdg);
   elm_box_pack_end(object, rd);
   elm_object_style_set(rd, group->style);

   elm_radio_value_set(rdg, 2);

   evas_object_data_set(object, SWALLOW_FUNC, _on_radio_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_radio_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _radio_send_signal);

   return object;
}
