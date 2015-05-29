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

Evas_Object *
widget_separator_create(Evas_Object *parent, const Style *style)
{
   Eina_Stringshare *class;
   Eina_Stringshare *style_name;
   standard_widget_name_parse(style->full_group_name, NULL, &class, &style_name);

   Evas_Object *rect, *object, *separator;
   Eina_Bool is_horizontal;

   object = elm_box_add(parent);
   separator = elm_separator_add(parent);

   elm_object_style_set(separator, style_name);

   is_horizontal = !strcmp(class, "horizontal");
   elm_box_horizontal_set(object, !is_horizontal);
   elm_separator_horizontal_set(separator, is_horizontal);

#define _ADD_RECT(COLOR_BLUE) \
        rect = evas_object_rectangle_add(evas_object_evas_get(parent)); \
        evas_object_color_set(rect, COLOR_BLUE); \
        evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
        evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL); \
        elm_box_pack_end(object, rect); \
        evas_object_show(rect);

   _ADD_RECT(COLOR_BLUE_LIGHT);
   elm_box_pack_end(object, separator);
   evas_object_show(separator);
   _ADD_RECT(COLOR_BLUE_DARK);

#undef _ADD_RECT

   evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   eina_stringshare_del(class);
   eina_stringshare_del(style_name);
   return object;
}
