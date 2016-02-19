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
   Property_Attribute item3_2;
   Property_Attribute item4;
   Property_Attribute item5;
   Property_Attribute item6;
   Property_Attribute item7;
   Property_Attribute item8;
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

static void
_start_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("start of %s->%s", pa->name, action->name);
}

static void
_stop_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("stop of %s->%s", pa->name, action->name);
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("change of %s->%s", pa->name, action->name);
}

static void
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("update of %s->%s", pa->name, action->name);
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("init of %s->%s", pa->name, action->name);
}

static void
_combo_change_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("change of %s->%s", pa->name, action->name);
   DBG("selected_item is %s", ewe_combobox_select_item_get(action->control)->title);
}
static void
_combo_init_cb(Property_Attribute *pa, Property_Action *action)
{
   DBG("init of %s->%s", pa->name, action->name);
   ewe_combobox_item_add(action->control, "CItem1");
   ewe_combobox_item_add(action->control, "CItem2");
   ewe_combobox_item_add(action->control, "CItem3");
   ewe_combobox_select_item_set(action->control, 0);
}
void
property_dummy_init()
{
   dummy_data.item1.name = eina_stringshare_add(_("Item 1"));
   dummy_data.item1.icon_name = eina_stringshare_add(_("elm/image/icon/start-point"));

   dummy_data.item2.name = eina_stringshare_add(_("Item 2"));
   dummy_data.item2.expand_cb = _item2_subitems_get;
   dummy_data.item2.icon_name = eina_stringshare_add(_("elm/image/icon/end-point"));

   dummy_data.item2_1.name = eina_stringshare_add(_("Item 2_1"));
   dummy_data.item2_1.icon_name = eina_stringshare_add(_("elm/image/icon/start-point"));

   dummy_data.item2_2.name = eina_stringshare_add(_("Item 2_2"));
   dummy_data.item2_2.icon_name = eina_stringshare_add(_("elm/image/icon/end-point"));

   dummy_data.item3.name = eina_stringshare_add(_("Item 3"));
   dummy_data.item3.expand_cb = _item3_subitems_get;
   dummy_data.item3.expanded = true;

   dummy_data.item3_1.name = eina_stringshare_add(_("Item 3_1"));

   dummy_data.item3_2.name = eina_stringshare_add(_("Item 3_2"));

   dummy_data.item4.name = eina_stringshare_add(_("Item 4"));
   dummy_data.item4.action1.name = eina_stringshare_add(_("entry:"));
   dummy_data.item4.action1.control_type = PROPERTY_CONTROL_ENTRY;
   dummy_data.item4.action1.init_cb = _init_cb;
   dummy_data.item4.action1.start_cb = _start_cb;
   dummy_data.item4.action1.stop_cb = _stop_cb;
   dummy_data.item4.action1.change_cb = _change_cb;
   dummy_data.item4.action1.update_cb = _update_cb;

   dummy_data.item5.name = eina_stringshare_add(_("Item 5"));
   dummy_data.item5.action1.name = eina_stringshare_add(_("comb:"));
   dummy_data.item5.action1.control_type = PROPERTY_CONTROL_COMBOBOX;
   dummy_data.item5.action1.init_cb = _combo_init_cb;
   dummy_data.item5.action1.change_cb = _combo_change_cb;
   dummy_data.item5.action1.start_cb = _start_cb;
   dummy_data.item5.action1.stop_cb = _stop_cb;

   dummy_data.item6.name = eina_stringshare_add(_("Item 6"));
   dummy_data.item6.action1.name = eina_stringshare_add(_("min:"));
   dummy_data.item6.action1.control_type = PROPERTY_CONTROL_SPINNER;
   dummy_data.item6.action1.units = eina_stringshare_add(_("px"));
   dummy_data.item6.action2.name = eina_stringshare_add(_("max:"));
   dummy_data.item6.action2.control_type = PROPERTY_CONTROL_SPINNER;
   dummy_data.item6.action2.units = eina_stringshare_add(_("px"));

   dummy_data.item7.name = eina_stringshare_add(_("Item 7"));
   dummy_data.item7.icon_name = eina_stringshare_add(_("elm/image/icon/end-point"));
   dummy_data.item7.action1.name = eina_stringshare_add(_("min:"));
   dummy_data.item7.action1.control_type = PROPERTY_CONTROL_SPINNER;
   dummy_data.item7.action1.units = eina_stringshare_add(_("px"));

   dummy_data.item8.name = eina_stringshare_add(_("Item 8"));
   dummy_data.item8.action1.name = eina_stringshare_add(_("min:"));
   dummy_data.item8.action1.control_type = PROPERTY_CONTROL_CHECK;
   dummy_data.item8.action1.units = eina_stringshare_add(_("px"));
   dummy_data.item8.action2.name = eina_stringshare_add(_("max:"));
   dummy_data.item8.action2.control_type = PROPERTY_CONTROL_SPINNER;
   dummy_data.item8.action2.units = eina_stringshare_add(_("px"));
}

Eina_List *
property_dummy_items_get()
{
   Eina_List *items = NULL;
   items = eina_list_append(items, &dummy_data.item1);
   items = eina_list_append(items, &dummy_data.item2);
   items = eina_list_append(items, &dummy_data.item3);
   items = eina_list_append(items, &dummy_data.item4);
   items = eina_list_append(items, &dummy_data.item5);
   items = eina_list_append(items, &dummy_data.item6);
   items = eina_list_append(items, &dummy_data.item7);
   items = eina_list_append(items, &dummy_data.item8);
   return items;
}
