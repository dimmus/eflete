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
                     Evas_Object *obj,
                     void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Elm_Object_Item *item;
   int i;

   for (i = 0; i < 3; i++)
     {
        TODO("Three items at the same time (store it somehow and free later)")
        item = elm_segment_control_item_get(obj, i);
        if (part->object)
          {
             evas_object_del(part->object);
             part->object = NULL;
          }

        part->object = object_generate(part, obj);
        part->change = false;
        elm_object_item_part_content_set(item, part->name, part->object);

        if (part->object)
          {
             evas_object_color_set(part->object,
                                   part->r,
                                   part->g,
                                   part->b,
                                   part->a);

             evas_object_size_hint_min_set(part->object,
                                           part->min_w,
                                           part->min_h);
             evas_object_size_hint_max_set(part->object,
                                           part->max_w,
                                           part->max_h);
          }
     }
}

static void
_on_sc_text_check(void *data __UNUSED__,
                  Evas_Object *obj,
                  void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Elm_Object_Item *item;
   int i;

   for (i = 0; i < 3; i++)
     {
        item = elm_segment_control_item_get(obj, i);
        elm_object_item_part_text_set(item, part->name, part->text_content);
     }
}

static void
_sc_send_signal(void *data __UNUSED__,
                Evas_Object *obj,
                void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   int i;

   Elm_Object_Item *item = NULL;

   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   for (i = 0; i < 3; i++)
     {
        item = elm_segment_control_item_get(obj, i);
        elm_object_item_signal_emit(item, sig->sig_name, sig->source_name);
     }
}

Evas_Object *
widget_segment_control_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_segment_control_add(parent);
   elm_segment_control_item_insert_at(object, NULL, NULL, 1);
   elm_segment_control_item_insert_at(object, NULL, NULL, 2);
   elm_segment_control_item_insert_at(object, NULL, NULL, 3);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, _on_sc_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, _on_sc_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, _sc_send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
