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
#include "demo_group.h"
#include "signals.h"
#include "string_common.h"
#include "main_window.h"
#include "editor.h"
#include "new_history.h"

#define DEMO_GROUP_DATA "demo_group_data"

typedef struct
{
   Group *group;

   Evas_Object *layout;
   Evas_Object *genlist;

   Elm_Genlist_Item_Class *itc_group;
   Elm_Genlist_Item_Class *itc_text;
   Elm_Genlist_Item_Class *itc_swallow;
   Elm_Genlist_Item_Class *itc_signals;

   Elm_Object_Item *selected_part_item;
} Part_Demo_List;

Evas_Object *
demo_group_add(Group *group)
{
   assert(group != NULL);
   assert(ap.win != NULL);

   Part_Demo_List *pl = mem_calloc(1, sizeof(Part_Demo_List));
   pl->layout = elm_layout_add(ap.win);
   elm_layout_theme_set(pl->layout, "layout", "navigator", "default");
   evas_object_show(pl->layout);

   evas_object_data_set(pl->layout, DEMO_GROUP_DATA, pl);

   pl->group = group;

   return pl->layout;
}

void
demo_group_part_select(Evas_Object *obj, Part_ *part __UNUSED__)
{
   Part_Demo_List *pl = evas_object_data_get(obj, DEMO_GROUP_DATA);

   assert(pl != NULL);
}
