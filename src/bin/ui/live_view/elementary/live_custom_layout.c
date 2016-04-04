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
layout_custom_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);

   Evas_Object *obj = elm_layout_add(parent);
   if (!elm_layout_file_set(obj, ap.project->dev, group->name))
     {
        ERR(N_("Could not load group '%s' from mapped file '%s'."), group->name, ap.project->dev)
        evas_object_del(obj);
        obj = NULL;
        TODO("Add frame to container with info that need this state is unstable"
             "and need to restart Eflete.");
        return NULL;
     }

   evas_object_smart_callback_add(obj, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(obj, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(obj, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   return obj;
}
