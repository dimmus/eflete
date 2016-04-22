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

#include "live_elementary_widgets.h"

/* TODO: We can't set swallows or texts or even send signal into map...
   need some additional work on it */
Evas_Object *
widget_map_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Elm_Map_Overlay *ovl1, *ovl2, *grp1;

   Evas_Object *object = elm_map_add(parent);

   if (strcmp(group->class, "marker_bubble") == 0)
     {
        /* "marker_bubble" group */
        elm_map_overlay_bubble_add(object);
     }
   else
     {
        /* "circle" group */
        elm_map_overlay_circle_add(object, -45, -45, 10);
        /* "marker/radio" group */
        ovl1 = elm_map_overlay_add(object, 30, 30);
        ovl2 = elm_map_overlay_add(object, 35, 35);

        grp1 = elm_map_overlay_class_add(object);
        elm_map_overlay_class_zoom_max_set(grp1, 2);
        elm_map_overlay_class_append(grp1, ovl1);
        elm_map_overlay_class_append(grp1, ovl2);

        /* "marker/empty" group */
        ovl1 = elm_map_overlay_add(object, 130, 30);
        ovl2 = elm_map_overlay_add(object, 135, 35);
        Evas_Object *rect = evas_object_rectangle_add(object);
        evas_object_color_set(rect, 255, 0, 0, 145);
        grp1 = elm_map_overlay_class_add(object);
        elm_map_overlay_icon_set(grp1, rect);
        elm_map_overlay_class_zoom_max_set(grp1, 2);
        elm_map_overlay_class_append(grp1, ovl1);
        elm_map_overlay_class_append(grp1, ovl2);

        /* "scale" group */
        elm_map_overlay_scale_add(object, 550, 720);
     }
   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
