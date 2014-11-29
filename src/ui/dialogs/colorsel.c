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

#include "colorsel.h"

static void
_on_changed(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info)
{
   Evas_Object *ctxpopup = (Evas_Object *)data;
   evas_object_smart_callback_call(ctxpopup, "color,changed", event_info);
}

static void
_on_item_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Evas_Object *ctxpopup = (Evas_Object *)data;
   evas_object_smart_callback_call(ctxpopup, "palette,item,selected", event_info);
}

static void
_on_item_longpressed(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Evas_Object *ctxpopup = (Evas_Object *)data;
   evas_object_smart_callback_call(ctxpopup, "palette,item,longpressed", event_info);
}

static void
_on_key_down(void *data __UNUSED__,
             Evas *e __UNUSED__,
             Evas_Object *obj,
             void *event_info)
{
   Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;
   if (strcmp(ev->keyname, "Escape") == 0)
     evas_object_smart_callback_call(obj, "dismissed", NULL);
}

Evas_Object *
colorselector_add(Evas_Object *parent)
{
   Evas_Object *ctxpopup, *colorsel;

   if (!parent) return NULL;

   ctxpopup = elm_ctxpopup_add(parent);
   elm_object_style_set(ctxpopup, "eflete/colorsel");
   colorsel = elm_colorselector_add(ctxpopup);
   elm_colorselector_mode_set(colorsel, ELM_COLORSELECTOR_ALL);
   elm_object_content_set(ctxpopup, colorsel);

   evas_object_event_callback_add(ctxpopup, EVAS_CALLBACK_KEY_DOWN, _on_key_down, NULL);
   evas_object_smart_callback_add(colorsel, "changed", _on_changed, ctxpopup);
   evas_object_smart_callback_add(colorsel, "color,item,selected", _on_item_selected, ctxpopup);
   evas_object_smart_callback_add(colorsel, "color,item,longpressed", _on_item_longpressed, ctxpopup);

   evas_object_hide(ctxpopup);
   return ctxpopup;
}

void
colorselector_color_set(Evas_Object *obj, int r, int g, int b, int a)
{
   Evas_Object *colorsel;

   if (!obj) return;
   colorsel = elm_object_content_get(obj);
   if (!colorsel) return;

   elm_colorselector_color_set(colorsel, r, g, b, a);
}

void
colorselector_color_get(Evas_Object *obj, int *r, int *g, int *b, int *a)
{
   Evas_Object *colorsel;
   int rc, gc, bc, ac;

   if (!obj) return;
   colorsel = elm_object_content_get(obj);
   if (!colorsel) return;

   elm_colorselector_color_get(colorsel, &rc, &gc, &bc, &ac);

   if (r) *r = rc;
   if (g) *g = gc;
   if (b) *b = bc;
   if (a) *a = ac;
}
