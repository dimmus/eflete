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

#include "notify.h"
#include "widget_macro.h"

static void
_notify_close (void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Evas_Object *notify = (Evas_Object *)data;

   assert(notify != NULL);

   evas_object_del(notify);
}


TODO("it all need to remake, use widget macro. notify style make as popup,"
     " with a blink block area")

Eina_Bool
noti_error_show(Evas_Object *obj, const char *message)
{
   Evas_Object *notify, *layout, *btn;

   assert(obj != NULL);
   assert(message != NULL);

   //1. setting notify up
   notify = elm_notify_add(obj);
   elm_notify_allow_events_set(notify, EINA_FALSE);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_notify_align_set(notify, 0.5, 0.5);

   //2. start forming layout
   layout = elm_layout_add(obj);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "notify", "default");
   elm_layout_signal_emit(layout, "notify,error", "eflete");

   //2.2 add not editable entry for information viewable
   elm_object_part_text_set(layout, "information", message);

   //2.3. add button for closing the error message
   BUTTON_ADD(obj, btn, _("Close"));
   evas_object_smart_callback_add(btn, "clicked", _notify_close, notify);
   elm_object_part_content_set (layout, "close_button", btn);

   //2.4. show this layout with everything
   evas_object_show (layout);
   elm_object_content_set(notify, layout);
   evas_object_show (notify);
   return true;
}

Eina_Bool
noti_warning_show(Evas_Object *obj, const char *message)
{
   Evas_Object *notify, *layout, *btn;

   assert(obj != NULL);
   assert(message != NULL);

   //1. setting notify up
   notify = elm_notify_add(obj);
   elm_notify_allow_events_set(notify, EINA_FALSE);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_notify_align_set(notify, 0.5, 0.5);

   //2. start forming layout
   layout = elm_layout_add(obj);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "notify", "default");
   elm_layout_signal_emit(layout, "notify,warning", "eflete");

   //2.2 add not editable entry for information viewable
   elm_object_part_text_set(layout, "information", message);

   //2.3. add button for closing the error message
   BUTTON_ADD(obj, btn, _("Close"));
   evas_object_smart_callback_add(btn, "clicked", _notify_close, notify);
   elm_object_part_content_set (layout, "close_button", btn);

   //2.4. show this layout with everything
   evas_object_show (layout);
   elm_object_content_set(notify, layout);
   elm_object_focus_set(btn, EINA_TRUE);
   evas_object_show (notify);
   return true;
}

Eina_Bool
noti_info_show(Evas_Object *obj, const char *message, double time)
{
   Evas_Object *notify, *layout;

   assert(obj != NULL);
   assert(message != NULL);
   assert(time >= 0);

   //1. setting notify up
   notify = elm_notify_add(obj);
   evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(notify, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_notify_align_set(notify, 0.5, 1.0);
   elm_notify_timeout_set(notify, time);
   evas_object_smart_callback_add(notify, "timeout", _notify_close, notify);

   //2. start forming layout
   layout = elm_layout_add(notify);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_theme_set(layout, "layout", "notify", "info");

   //2.2 add not editable entry for information viewable
   elm_object_part_text_set(layout, "information", message);

   //2.4. show this layout with everything
   evas_object_show (layout);
   elm_object_content_set(notify, layout);
   evas_object_show (notify);
   return true;
}
