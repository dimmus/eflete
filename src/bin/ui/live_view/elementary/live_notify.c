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
_on_notify_swallow_check(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   TODO("Remake on_swallow_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

static void
_on_notify_text_check(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   TODO("Remake on_text_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

static void
_notify_send_signal(void *data,
                    Evas_Object *obj,
                    void *ei __UNUSED__)
{
   Evas_Object *notify_obj = NULL;

   assert(data != NULL);

   Eina_List *notify_list = elm_box_children_get(data);
   notify_obj = eina_list_nth(notify_list, 1);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   elm_layout_signal_emit(notify_obj, name, source);

   eina_list_free(notify_list);
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

Evas_Object *
widget_notify_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *content, *noti, *bx, *btn, *object = NULL;
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
   LABEL_ADD(parent, content, _("Text example. Timeout 3 sec"));
   elm_box_pack_end(bx, content);

   evas_object_smart_callback_add(btn, "clicked", _on_click, noti);

   object = elm_box_add(parent);
   elm_box_pack_end(object, btn);

   evas_object_data_set(object, SWALLOW_FUNC, _on_notify_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_notify_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _notify_send_signal);

   elm_object_style_set(noti, group->style);
   return object;
}
