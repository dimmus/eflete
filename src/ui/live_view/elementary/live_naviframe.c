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
_next_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = data;
   Elm_Object_Item *it;

   it = elm_naviframe_item_push(nf, _("Page 2"), NULL, NULL, NULL, NULL);
   elm_object_item_part_text_set(it, "subtitle", _("Subtitle 2"));
}


Evas_Object *
widget_naviframe_create(Evas_Object *parent,
                        const char  *class __UNUSED__,
                        const char  *style __UNUSED__)
{
   Evas_Object *bt;
   Elm_Object_Item *it;

   if (!parent) return NULL;
   Evas_Object *nf = elm_naviframe_add(parent);
   evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(parent, nf);

   bt = elm_button_add(nf);
   elm_object_text_set(bt, _("Page 2"));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt, "clicked", _next_page_cb, nf);
   evas_object_show(bt);

   it = elm_naviframe_item_push(nf, _("Page 1"), NULL, bt, NULL, NULL);
   elm_object_item_part_text_set(it, "subtitle", _("Subtitle 1"));

   evas_object_data_set(nf, SWALLOW_FUNC, on_swallow_check);
   evas_object_data_set(nf, TEXT_FUNC, on_text_check);
   evas_object_data_set(nf, SIGNAL_FUNC, send_signal);

   return nf;
}
