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
widget_clock_create(Evas_Object *parent, const char *class, const char *style __UNUSED__)
{
   Evas_Object *object = elm_clock_add(parent);

   unsigned int digit_edit;
   if (strcmp(class, "flipdigit") == 0)
     elm_clock_edit_set(object, true);
   /****************************************************************************
    * Enum  Elm_Clock_Edit_Mode are identifiers for which clock digits should
    * be editable, when a clock widget is in edition mode. For "flipampm"
    * mode variable ELM_CLOCK_EDIT_HOUR_DECIMAL is responsible for user editing
    * time. In this case time can be increased or decreased with step 12 hours.
    ***************************************************************************/
   else if (strcmp(class, "flipampm") == 0)
     {
        elm_clock_show_am_pm_set(object, true);
        elm_clock_edit_set(object, true);
        digit_edit = ELM_CLOCK_EDIT_HOUR_DECIMAL;
        elm_clock_edit_mode_set(object, digit_edit);
     }
   else if (strcmp(class, "base-seconds") == 0)
     elm_clock_show_seconds_set(object, true);
   else if (strcmp(class, "base-am_pm") == 0)
     elm_clock_show_am_pm_set(object, true);
   else if (strcmp(class, "base-all") == 0)
     {
        elm_clock_show_seconds_set(object, true);
        elm_clock_show_am_pm_set(object, true);
     }

   evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   return object;
}
