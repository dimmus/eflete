/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "property.h"
#include "property_private.h"

struct _Property_Group_Data {
   Group *group;
   Part *part;

   struct {
        Property_Attribute group;
        Property_Attribute part;
        Property_Attribute state;
        Property_Attribute item;
        Property_Attribute program;
   } items;
};
typedef struct _Property_Group_Data Property_Group_Data;
static Property_Group_Data group_pd;

void
property_group_init()
{
   group_pd.items.group.name = _("group");
   group_pd.items.group.expandable = true;

   group_pd.items.part.name = _("part");
   group_pd.items.part.expandable = true;

   group_pd.items.state.name = _("state");
   group_pd.items.state.expandable = true;

   group_pd.items.item.name = _("item");
   group_pd.items.item.expandable = true;

   group_pd.items.program.name = _("program");
   group_pd.items.program.expandable = true;
}

Eina_List *
property_group_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &group_pd.items.group);
   items = eina_list_append(items, &group_pd.items.part);
   items = eina_list_append(items, &group_pd.items.state);
   items = eina_list_append(items, &group_pd.items.item);
   items = eina_list_append(items, &group_pd.items.program);

   return items;
}
