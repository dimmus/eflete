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
widget_clock_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_clock_add(parent);

   unsigned int digit_edit;
   if (strcmp(group->class, "flipdigit") == 0)
     elm_clock_edit_set(object, true);
   /****************************************************************************
    * Enum  Elm_Clock_Edit_Mode are identifiers for which clock digits should
    * be editable, when a clock widget is in edition mode. For "flipampm"
    * mode variable ELM_CLOCK_EDIT_HOUR_DECIMAL is responsible for user editing
    * time. In this case time can be increased or decreased with step 12 hours.
    ***************************************************************************/
   else if (strcmp(group->class, "flipampm") == 0)
     {
        elm_clock_show_am_pm_set(object, true);
        elm_clock_edit_set(object, true);
        digit_edit = ELM_CLOCK_EDIT_HOUR_DECIMAL;
        elm_clock_edit_mode_set(object, digit_edit);
     }
   else if (strcmp(group->class, "base-seconds") == 0)
     elm_clock_show_seconds_set(object, true);
   else if (strcmp(group->class, "base-am_pm") == 0)
     elm_clock_show_am_pm_set(object, true);
   else if (strcmp(group->class, "base-all") == 0)
     {
        elm_clock_show_seconds_set(object, true);
        elm_clock_show_am_pm_set(object, true);
     }

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
