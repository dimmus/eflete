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

#include "live_view.h"
#include "live_view_prop.h"

#include "signals.h"

static void
_set_text(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   Part_ *part = (Part_ *) event_info;
   Evas_Object *object = (Evas_Object *) data;

   elm_object_part_text_set(object, part->name, part->content);
}

Evas_Object *
demo_add(Evas_Object *parent, Group *group)
{
   Evas_Object *obj;

   if (group->widget)
     {
        obj = live_widget_create(group, parent);
        if (!obj)
           ERR(N_("Widget live view isn't implemented yet. Using fallback to layout"))
        else
        {
           Elm_Theme *theme = elm_theme_new();
           elm_theme_set(theme, ap.project->dev);
           elm_object_theme_set(obj, theme);
           elm_theme_free(theme);
           return obj;
        }
     }
   /* if widget is not created, need use the layout */
   obj = elm_layout_add(parent);
   if (!elm_layout_file_set(obj, ap.project->dev, group->name))
     {
        ERR(N_("Could not load group '%s' from mapped file '%s'."), group->name, ap.project->dev)
        evas_object_del(obj);
        obj = NULL;
        TODO("Add frame to container with info that need this state is unstable"
             "and need to restart Eflete.");
     }


   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, _set_text, obj);

   return obj;
}
