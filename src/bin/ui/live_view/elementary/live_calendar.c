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
widget_calendar_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_calendar_add(parent);

   /* Structure containing a calendar date and time
      broken down into its components (see "time.h").
      {sec, min, hour, day of the month, month, year since 1900,
      days since Sunday, days since January 1, Daylight Saving Time flag} */
   TODO("fix warnings on windows")
   struct tm saturday = {0, 0, 0, 0, 1, 114, 6, -1, 0, 0, NULL};
   elm_calendar_mark_add(object, "checked", &saturday,
                                ELM_CALENDAR_WEEKLY);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, send_signal, object);

   elm_object_style_set(object, group->style);

   return object;
}
