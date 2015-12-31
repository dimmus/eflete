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
_on_scroller_swallow_check(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *ei __UNUSED__)
{
   TODO("Remake on_swallow_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

Evas_Object *
widget_scroller_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   Evas_Object *object;

   if ((strcmp(group->class, "entry") == 0) || (strcmp(group->class, "entry_single") == 0))
     {
        object = elm_entry_add(parent);
        evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
        elm_entry_scrollable_set(object, true);
        if (strcmp(group->class, "entry_single") == 0)
          elm_entry_single_line_set(object, true);
     }
   else
     {
        object = elm_scroller_add(parent);
        evas_object_data_set(object, SWALLOW_FUNC, _on_scroller_swallow_check);
     }
   elm_object_style_set(object, group->style);

   elm_scroller_policy_set(object, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);

   evas_object_data_set(object, TEXT_FUNC, on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, send_signal);

   return object;
}
