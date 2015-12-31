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

#include "live_widget.h"
#include "project_manager.h"
#include "live_elementary_widgets.h"

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
   if (!obj)
     {
        ERR(N_("Widget live view isn't implemented yet. Using fallback to layout"))
        obj = layout_custom_create(parent, group);
     }

   assert(obj != NULL);

   return obj;
}
