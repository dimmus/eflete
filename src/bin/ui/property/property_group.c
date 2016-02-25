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
#include "group_manager.h"

struct _Property_Group_Data {
   Group *group;
   Part *part;
   Eina_Stringshare *program;

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

static void
_on_part_selected(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   group_pd.part = event_info;

   assert(group_pd.part != NULL);

   DBG("selected part \"%s\"", group_pd.part->name);
   property_item_del(&group_pd.items.program);

   property_item_add(&group_pd.items.part, NULL);
   property_item_add(&group_pd.items.state, NULL);
   if (group_pd.part->current_item_name)
     property_item_add(&group_pd.items.item, NULL);
   else
     property_item_del(&group_pd.items.item);
}

static void
_on_group_navigator_unselected(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   DBG("unselected_cb\n");
   group_pd.part = NULL;
   group_pd.program = NULL;
   property_item_del(&group_pd.items.part);
   property_item_del(&group_pd.items.state);
   property_item_del(&group_pd.items.item);
   property_item_del(&group_pd.items.program);
}

static void
_on_part_state_selected(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Part *part = event_info;

   assert(group_pd.part == part);

   DBG("selected state \"%s\"", group_pd.part->current_state->name);
}

static void
_on_program_selected(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   group_pd.program = event_info;

   assert(group_pd.program != NULL);

   DBG("selected program \"%s\"", group_pd.program);
   property_item_del(&group_pd.items.part);
   property_item_del(&group_pd.items.state);
   property_item_del(&group_pd.items.item);

   property_item_add(&group_pd.items.program, NULL);
}

static void
_on_group_changed(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   group_pd.group = event_info;

   assert(group_pd.group != NULL);

   DBG("group changed to \"%s\"", group_pd.group->name);
   group_pd.part = group_pd.group->current_part;
   group_pd.program = group_pd.group->current_program;
}

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

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_CHANGED, _on_group_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_SELECTED, _on_part_selected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_NAVIGATOR_UNSELECTED, _on_group_navigator_unselected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_STATE_SELECTED, _on_part_state_selected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROGRAM_SELECTED, _on_program_selected, NULL);
   /* evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, _on_editor_attribute_changed, NULL); */
}

Eina_List *
property_group_items_get()
{
   Eina_List *items = NULL;

   assert(group_pd.group != NULL);

   items = eina_list_append(items, &group_pd.items.group);
   if (group_pd.part)
     {
        items = eina_list_append(items, &group_pd.items.part);
        items = eina_list_append(items, &group_pd.items.state);
        if (group_pd.part->current_item_name)
          items = eina_list_append(items, &group_pd.items.item);
     }
   else if (group_pd.program)
     items = eina_list_append(items, &group_pd.items.program);

   return items;
}
