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
_on_notify_swallow_check(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *) data;

   if (part->change)
     {
        /* if NONE - delete object */
        if (part->object)
          {
             elm_box_unpack_all(object);
             evas_object_del(part->object);
             part->object = NULL;
          }

        elm_box_unpack(object, part->object);
        part->object = object_generate(part, object);
        evas_object_show(part->object);
        part->change = false;
        elm_box_pack_end(object, part->object);
     }

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

static void
_notify_send_signal(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Evas_Object *object = (Evas_Object *)data;

   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   elm_layout_signal_emit(object, sig->sig_name, sig->source_name);
}

static void
_notify_orient_get(const char *class, double *horizontal, double *vertical)
{
   assert(class != NULL);
   assert(horizontal != NULL);
   assert(vertical != NULL);

   if (strcmp(class, "top") == 0)
     {
        *horizontal = 0.5;
        *vertical = 0.0;
     }
   else if (strcmp(class, "center") == 0)
     {
        *horizontal = 0.5;
        *vertical = 0.5;
     }
   else if (strcmp(class, "bottom") == 0)
     {
        *horizontal = 0.5;
        *vertical = 1.0;
     }
   else if (strcmp(class, "left") == 0)
     {
        *horizontal = 0.0;
        *vertical = 0.5;
     }
   else if (strcmp(class, "right") == 0)
     {
        *horizontal = 1.0;
        *vertical = 0.5;
     }
   else if (strcmp(class, "top_left") == 0)
     {
        *horizontal = 0.0;
        *vertical = 0.0;
     }
   else if (strcmp(class, "top_right") == 0)
     {
        *horizontal = 1.0;
        *vertical = 0.0;
     }
   else if (strcmp(class, "bottom_left") == 0)
     {
        *horizontal = 0.0;
        *vertical = 1.0;
     }
   else if (strcmp(class, "bottom_right") == 0)
     {
        *horizontal = 1.0;
        *vertical = 1.0;
     }
   else
     {
        *horizontal = 0.5;
        *vertical = 0.5;
     }
}

static void
_on_click(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *notify = data;

   assert(notify != NULL);

   evas_object_show(notify);
}

void
_demo_noti_del(void *data,
               Evas *evas __UNUSED__,
               Evas_Object *object,
               void *event_info __UNUSED__)
{
   Evas_Object *bx = (Evas_Object *)data;

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_notify_swallow_check, bx);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _notify_send_signal, object);
}

Evas_Object *
widget_notify_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *noti, *bx, *btn, *object = NULL;
   double horizontal, vertical;

   assert(group->class != NULL);

   BUTTON_ADD(parent, btn, _("Press to show Notify"));

   noti = elm_notify_add(btn);
   _notify_orient_get(group->class, &horizontal, &vertical);
   elm_notify_align_set(noti, horizontal, vertical);
   elm_notify_allow_events_set(noti, false);
   elm_notify_timeout_set(noti, 3);
   BOX_ADD(parent, bx, false, false);
   elm_object_content_set(noti, bx);

   evas_object_smart_callback_add(btn, "clicked", _on_click, noti);

   object = elm_box_add(parent);
   elm_box_pack_end(object, btn);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_notify_swallow_check, bx);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, noti);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _notify_send_signal, noti);
   evas_object_event_callback_add(noti, EVAS_CALLBACK_DEL, _demo_noti_del, bx);

   elm_object_style_set(noti, group->style);
   return object;
}
