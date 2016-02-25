/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "property.h"
#include "property_private.h"
#include "colorsel.h"

static void
_on_color_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *control = data;

   assert(control != NULL);

   evas_object_smart_callback_call(control, "changed", NULL);
}

static void
_on_dismissed(void *data,
                    Evas_Object *obj,
                    void *event_info __UNUSED__)
{
   Evas_Object *control = data;

   assert(control != NULL);

   evas_object_smart_callback_del_full(obj, "color,changed", _on_color_change, control);
   evas_object_smart_callback_del_full(obj, "dismissed", _on_dismissed, control);

   elm_object_scroll_freeze_pop(control);
   evas_object_smart_callback_call(control, "dismissed", NULL);

   evas_object_hide(obj);
}

static void
_on_color_clicked(void *data __UNUSED__,
                  Evas_Object *control,
                  const char *emission __UNUSED__,
                  const char *source __UNUSED__)
{
   int x, y;
   int r, g, b, a;
   Evas_Object *colorsel;

   assert(control != NULL);

   colorsel = colorselector_get();
   property_color_control_color_get(control, &r, &g, &b, &a);
   colorselector_color_set(colorsel, r, g, b, a);
   elm_object_scroll_freeze_push(control);
   evas_object_smart_callback_add(colorsel, "color,changed", _on_color_change, control);
   evas_object_smart_callback_add(colorsel, "dismissed", _on_dismissed, control);
   evas_object_geometry_get(control, &x, &y, NULL, NULL);
   evas_object_move(colorsel, x, y);
   evas_object_show(colorsel);
}

Evas_Object *
property_color_control_add(Evas_Object *parent)
{
   Evas_Object *control, *color;

   assert(parent != NULL);

   control = elm_layout_add(parent);
   elm_layout_theme_set(control, "layout", "property", "color");
   evas_object_size_hint_weight_set(control,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(control, EVAS_HINT_FILL, EVAS_HINT_FILL);
   color = elm_layout_add(parent);
   elm_layout_theme_set(color, "image", "color", "color_set");
   elm_layout_signal_callback_add(control, "clicked", "eflete", _on_color_clicked, NULL);
   elm_layout_content_set(control, NULL, color);
   return control;
}

void
property_color_control_color_set(Evas_Object *control, int r, int g, int b, int a)
{
   Evas_Object *color;

   assert(control != NULL);
   assert((r >= 0) && (r <= 255));
   assert((g >= 0) && (g <= 255));
   assert((b >= 0) && (b <= 255));
   assert((a >= 0) && (a <= 255));

   color = elm_layout_content_get(control, NULL);
   evas_color_argb_premul(a, &r, &g, &b);
   evas_object_color_set(color, r, g, b, a);
}

void
property_color_control_color_get(Evas_Object *control, int *r, int *g, int *b, int *a)
{
   Evas_Object *color;

   assert(control != NULL);

   color = elm_layout_content_get(control, NULL);
   evas_object_color_get(color, r, g, b, a);
}
