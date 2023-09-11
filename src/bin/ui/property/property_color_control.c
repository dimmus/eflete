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
#include "main_window.h"

typedef struct {
   int r, g, b, a;
} Color;
#define COLOR_DATA "COLOR_DATA"

static void
_on_color_change(void *data,
                 Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{
   Color *c;
   Evas_Object *control = data;

   assert(control != NULL);

   c = evas_object_data_get(control, COLOR_DATA);
   elm_colorselector_color_get(obj, &c->r, &c->g, &c->b, &c->a);
   property_color_control_color_set(control, c->r, c->g, c->b, c->a);
   evas_object_smart_callback_call(control, signals.eflete.property.color_control.changed, NULL);
}

static Eina_Bool
_on_dismissed(void *data,
              Evas_Object *obj,
              void *event_info EINA_UNUSED)
{
   Evas_Object *control = data;

   assert(control != NULL);

   elm_object_scroll_freeze_pop(control);
   evas_object_smart_callback_call(control, signals.eflete.property.color_control.dismissed, NULL);

   evas_object_hide(obj);
   return true;
}

static void
_on_color_clicked(void *data EINA_UNUSED,
                  Evas_Object *control,
                  const char *emission EINA_UNUSED,
                  const char *source EINA_UNUSED)
{
   int r, g, b, a;

   assert(control != NULL);

   property_color_control_color_get(control, &r, &g, &b, &a);
   popup_colorselector_helper(control,
                              _on_dismissed,
                              _on_color_change,
                              control, r, g, b, a);
   elm_object_scroll_freeze_push(control);
}

static void
_color_free(void *data,
            Evas *e EINA_UNUSED,
            Evas_Object *obj EINA_UNUSED,
            void *event_info EINA_UNUSED)
{
   Color *c = data;

   assert(c != NULL);

   free(c);
}

Evas_Object *
property_color_control_add(Evas_Object *parent)
{
   Evas_Object *control, *color;
   Color *c;

   assert(parent != NULL);

   control = elm_layout_add(parent);
   elm_layout_theme_set(control, "layout", "property", "color");
   evas_object_size_hint_weight_set(control,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(control, EVAS_HINT_FILL, EVAS_HINT_FILL);
   color = elm_layout_add(parent);
   elm_layout_theme_set(color, "image", "color", "color_set");
   elm_layout_signal_callback_add(control, "clicked", "eflete", _on_color_clicked, NULL);
   elm_layout_content_set(control, NULL, color);

   evas_object_event_callback_add(control,
                                  EVAS_CALLBACK_FREE,
                                  popup_active_helper_close,
                                  (void*)(uintptr_t)POPUP_COLORSELECTOR_HELPER);

   c = mem_calloc(1, sizeof(Color));
   evas_object_data_set(control, COLOR_DATA, c);
   evas_object_event_callback_add(control, EVAS_CALLBACK_FREE, _color_free, c);

   return control;
}

void
property_color_control_color_set(Evas_Object *control, int r, int g, int b, int a)
{
   Evas_Object *color;
   Color *c;

   assert(control != NULL);
   assert((r >= 0) && (r <= 255));
   assert((g >= 0) && (g <= 255));
   assert((b >= 0) && (b <= 255));
   assert((a >= 0) && (a <= 255));

   c = evas_object_data_get(control, COLOR_DATA);

   assert(c != NULL);

   c->r = r;
   c->g = g;
   c->b = b;
   c->a = a;

   color = elm_layout_content_get(control, NULL);
   evas_color_argb_premul(a, &r, &g, &b);
   evas_object_color_set(color, r, g, b, a);
}

void
property_color_control_color_get(Evas_Object *control, int *r, int *g, int *b, int *a)
{
   assert(control != NULL);

   Color *color = evas_object_data_get(control, COLOR_DATA);

   assert(color != NULL);

   *r = color->r;
   *g = color->g;
   *b = color->b;
   *a = color->a;
}
