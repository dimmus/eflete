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

typedef struct {
   Property_Attribute item1;
   Property_Attribute item2;
   Property_Attribute item2_1;
   Property_Attribute item2_2;
   Property_Attribute item3;
   Property_Attribute item3_1;
} Property_Dummy_Data;

static Property_Dummy_Data dummy_data;

static Eina_List *
_item2_subitems_get(Property_Attribute *pa __UNUSED__)
{
   Eina_List *items = NULL;
   items = eina_list_append(items, &dummy_data.item2_1);
   items = eina_list_append(items, &dummy_data.item2_2);
   return items;
}

static Eina_List *
_item3_subitems_get(Property_Attribute *pa __UNUSED__)
{
   Eina_List *items = NULL;
   items = eina_list_append(items, &dummy_data.item3_1);
   return items;
}

void
property_dummy_init()
{
   dummy_data.item1.name = eina_stringshare_add(_("Item 1"));

   dummy_data.item2.name = eina_stringshare_add(_("Item 2"));
   dummy_data.item2.expand_cb = _item2_subitems_get;

   dummy_data.item2_1.name = eina_stringshare_add(_("Item 2_1"));

   dummy_data.item2_2.name = eina_stringshare_add(_("Item 2_2"));

   dummy_data.item3.name = eina_stringshare_add(_("Item 3"));
   dummy_data.item3.expand_cb = _item3_subitems_get;
   dummy_data.item3.expanded = true;

   dummy_data.item3_1.name = eina_stringshare_add(_("Item 3_1"));
}

Eina_List *
property_dummy_items_get()
{
   Eina_List *items = NULL;
   items = eina_list_append(items, &dummy_data.item1);
   items = eina_list_append(items, &dummy_data.item2);
   items = eina_list_append(items, &dummy_data.item3);
   return items;
}
