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
#include "history.h"
#include "change.h"

struct _Property_Group_Data {
   Group *group;
   Part *part;
   Eina_Stringshare *program;

   /* data needed to correctly handle changes that will be passed to history module */
   struct {
      const char *format;
      Change *change;
      struct {
         int int_val1;
         Eina_Stringshare *str_val1;
      } old, new;
   } history;

   struct {
        struct {
             Property_Attribute title;
             Property_Attribute name;
             Property_Attribute min;
             Property_Attribute max;
        } group;
        struct {
             Property_Attribute title;
             Property_Attribute name;
             Property_Attribute type;
        } part;
        Property_Attribute state;
        Property_Attribute item;
        Property_Attribute program;
   } items;
};
typedef struct _Property_Group_Data Property_Group_Data;
static Property_Group_Data group_pd;

struct _Property_Group_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Group_Update_Info Property_Group_Update_Info;

/* array to find item by Attribute */
static Property_Group_Update_Info attribute_map[ATTRIBUTE_LAST];

/* global callbacks */
static void
_on_part_selected(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   group_pd.part = event_info;

   assert(group_pd.part != NULL);

   DBG("selected part \"%s\"", group_pd.part->name);
   property_item_del(&group_pd.items.program);

   property_item_add(&group_pd.items.part.title, NULL);
   property_item_add(&group_pd.items.state, NULL);
   if (group_pd.part->current_item_name)
     property_item_add(&group_pd.items.item, NULL);
   else
     property_item_del(&group_pd.items.item);

   property_item_update_recursively(&group_pd.items.part.title);
}

static void
_on_group_navigator_unselected(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   DBG("unselected_cb\n");
   group_pd.part = NULL;
   group_pd.program = NULL;
   property_item_del(&group_pd.items.part.title);
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
   property_item_del(&group_pd.items.part.title);
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
   if (group_pd.part)
     {
        property_item_add(&group_pd.items.part.title, NULL);
        property_item_update_recursively(&group_pd.items.part.title);
     }
   else
     property_item_del(&group_pd.items.part.title);
   group_pd.program = group_pd.group->current_program;
   property_item_update_recursively(&group_pd.items.group.title);
}

static void
_on_editor_attribute_changed(void *data __UNUSED__,
                             Evas_Object *obj __UNUSED__,
                             void *event_info)
{
   Attribute *attribute = event_info;
   Property_Attribute *pa = attribute_map[*attribute].pa;
   Property_Action *action = attribute_map[*attribute].action;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->update_cb != NULL);

   if (pa->realized)
     action->update_cb(pa, action);
}

/* local callbacks */
static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);

   if (pa == &group_pd.items.group.title)
     {
         items = eina_list_append(items, &group_pd.items.group.name);
         items = eina_list_append(items, &group_pd.items.group.min);
         items = eina_list_append(items, &group_pd.items.group.max);
     }
   else if (pa == &group_pd.items.part.title)
     {
         items = eina_list_append(items, &group_pd.items.part.name);
         items = eina_list_append(items, &group_pd.items.part.type);
     }
   else
     {
        CRIT("items callback not found for %s", pa->name);
        abort();
     }
   return items;
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute)
     {
      case ATTRIBUTE_GROUP_NAME:
         elm_object_disabled_set(action->control, true);
         break;
      case ATTRIBUTE_GROUP_MIN_W:
      case ATTRIBUTE_GROUP_MIN_H:
      case ATTRIBUTE_GROUP_MAX_W:
      case ATTRIBUTE_GROUP_MAX_H:
      case ATTRIBUTE_PART_NAME:
      case ATTRIBUTE_PART_TYPE:
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("init callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   int int_val1;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute)
     {
      case ATTRIBUTE_GROUP_NAME:
         property_entry_set(action->control, group_pd.group->name);
         break;
      case ATTRIBUTE_GROUP_MIN_W:
         int_val1 = edje_edit_group_min_w_get(group_pd.group->edit_object);
         elm_spinner_value_set(action->control, int_val1);
         break;
      case ATTRIBUTE_GROUP_MIN_H:
         int_val1 = edje_edit_group_min_h_get(group_pd.group->edit_object);
         elm_spinner_value_set(action->control, int_val1);
         break;
      case ATTRIBUTE_GROUP_MAX_W:
         int_val1 = edje_edit_group_max_w_get(group_pd.group->edit_object);
         elm_spinner_value_set(action->control, int_val1);
         break;
      case ATTRIBUTE_GROUP_MAX_H:
         int_val1 = edje_edit_group_max_h_get(group_pd.group->edit_object);
         elm_spinner_value_set(action->control, int_val1);
         break;
      case ATTRIBUTE_PART_NAME:
         property_entry_set(action->control, group_pd.part->name);
         break;
      case ATTRIBUTE_PART_TYPE:
         elm_layout_text_set(action->control, NULL, gm_part_type_text_get(group_pd.part->type));
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("update callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_start_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);
   assert(group_pd.history.change == NULL);

   group_pd.history.change = change_add(NULL);

   /* setting new and old to current value. this will be checked on stop to ignore
      empty changes (i.e. from 0 to 0) */
#define VAL(NAME) \
 group_pd.history.new.NAME = group_pd.history.old.NAME

   switch (action->type.attribute)
     {
      case ATTRIBUTE_GROUP_NAME:
         group_pd.history.format = _("group name changed from %s to %s");
         VAL(str_val1) = eina_stringshare_add(eina_stringshare_add(group_pd.group->name));
         break;
      case ATTRIBUTE_GROUP_MIN_W:
         group_pd.history.format = _("group.min_w changed from %d to %d");
         VAL(int_val1) = edje_edit_group_min_w_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_GROUP_MIN_H:
         group_pd.history.format = _("group.min_h changed from %d to %d");
         VAL(int_val1) = edje_edit_group_min_h_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_GROUP_MAX_W:
         group_pd.history.format = _("group.max_w changed from %d to %d");
         VAL(int_val1) = edje_edit_group_max_w_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_GROUP_MAX_H:
         group_pd.history.format = _("group.max_h changed from %d to %d");
         VAL(int_val1) = edje_edit_group_max_h_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_PART_NAME:
         group_pd.history.format = _("part name changed from %s to %s");
         VAL(str_val1) = eina_stringshare_add(eina_stringshare_add(group_pd.part->name));
         break;
      case ATTRIBUTE_PART_TYPE:
         /* part type can't be changed */
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("start callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
#undef VAL
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   double double_val1 = 0.0;
   Eina_Stringshare *str_val1 = NULL;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);
   assert(group_pd.history.change != NULL);

   switch (action->control_type)
     {
      case PROPERTY_CONTROL_SPINNER:
         double_val1 = elm_spinner_value_get(action->control);
         break;
      case PROPERTY_CONTROL_ENTRY:
         str_val1 = property_entry_get(action->control);
         break;
      default:
         break;
     }

   switch (action->type.attribute)
     {
      case ATTRIBUTE_GROUP_NAME:
         TODO("implement group rename");
         break;
      case ATTRIBUTE_GROUP_MIN_W:
         editor_group_min_w_set(group_pd.group->edit_object, group_pd.history.change, true, double_val1);
         group_pd.history.new.int_val1 = edje_edit_group_min_w_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_GROUP_MIN_H:
         editor_group_min_h_set(group_pd.group->edit_object, group_pd.history.change, true, double_val1);
         group_pd.history.new.int_val1 = edje_edit_group_min_h_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_GROUP_MAX_W:
         editor_group_max_w_set(group_pd.group->edit_object, group_pd.history.change, true, double_val1);
         group_pd.history.new.int_val1 = edje_edit_group_max_w_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_GROUP_MAX_H:
         editor_group_max_h_set(group_pd.group->edit_object, group_pd.history.change, true, double_val1);
         group_pd.history.new.int_val1 = edje_edit_group_max_h_get(group_pd.group->edit_object);
         break;
      case ATTRIBUTE_PART_NAME:
         editor_part_name_set(group_pd.group->edit_object, group_pd.history.change, false, group_pd.part->name, str_val1);
         eina_stringshare_del(group_pd.history.new.str_val1);
         group_pd.history.new.str_val1 = str_val1;
         break;
      case ATTRIBUTE_PART_TYPE:
         /* part type can't be changed */
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("change callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_stop_cb(Property_Attribute *pa, Property_Action *action)
{
   Eina_Stringshare *msg = NULL;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);
   assert(group_pd.history.change != NULL);

#define CHECK_VAL(NAME) \
 if (group_pd.history.new.NAME == group_pd.history.old.NAME) \
   { \
     change_free(group_pd.history.change); \
     goto clean; \
   }

   switch (action->type.attribute)
     {
      case ATTRIBUTE_GROUP_NAME:
      case ATTRIBUTE_PART_NAME:
         CHECK_VAL(str_val1);
         msg = eina_stringshare_printf(group_pd.history.format,
                                       group_pd.history.old.str_val1,
                                       group_pd.history.new.str_val1);
         break;
      case ATTRIBUTE_GROUP_MIN_W:
      case ATTRIBUTE_GROUP_MIN_H:
      case ATTRIBUTE_GROUP_MAX_W:
      case ATTRIBUTE_GROUP_MAX_H:
         CHECK_VAL(int_val1);
         msg = eina_stringshare_printf(group_pd.history.format,
                                       group_pd.history.old.int_val1,
                                       group_pd.history.new.int_val1);
         break;
      case ATTRIBUTE_PART_TYPE:
         /* part type can't be changed */
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("stop callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
   change_description_set(group_pd.history.change, msg);
   history_change_add(group_pd.group->history, group_pd.history.change);
   eina_stringshare_del(msg);
#undef CHECK_VAL
clean:
   eina_stringshare_del(group_pd.history.new.str_val1);
   eina_stringshare_del(group_pd.history.old.str_val1);
   group_pd.history.change = NULL;
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type, Attribute attribute)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->control_type = control_type;
   action->type.attribute = attribute;
   action->update_cb = _update_cb;
   action->init_cb = _init_cb;
   action->start_cb = _start_cb;
   action->stop_cb = _stop_cb;
   action->change_cb = _change_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute attribute)
{
   _action_internal(&pa->action1, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute attribute)
{
   _action_internal(&pa->action2, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action2;
}

static void
_init_group_block()
{
   group_pd.items.group.title.name = "group";
   group_pd.items.group.title.expandable = true;
   group_pd.items.group.title.expanded = true;
   group_pd.items.group.title.expand_cb = _subitems_get;

   group_pd.items.group.name.name = "name";
   _action1(&group_pd.items.group.name, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_GROUP_NAME);

   group_pd.items.group.min.name = "min";
   _action1(&group_pd.items.group.min, "w", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_GROUP_MIN_W);
   _action2(&group_pd.items.group.min, "h", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_GROUP_MIN_H);

   group_pd.items.group.max.name = "max";
   _action1(&group_pd.items.group.max, "w", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_GROUP_MAX_W);
   _action2(&group_pd.items.group.max, "h", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_GROUP_MAX_H);
}

static void
_init_part_block()
{
   group_pd.items.part.title.name = "part";
   group_pd.items.part.title.expandable = true;
   group_pd.items.part.title.expanded = true;
   group_pd.items.part.title.expand_cb = _subitems_get;

   group_pd.items.part.name.name = "name";
   _action1(&group_pd.items.part.name, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_PART_NAME);

   group_pd.items.part.type.name = "type";
   _action1(&group_pd.items.part.type, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_PART_TYPE);
}

/* public */
void
property_group_init()
{
   _init_group_block();
   _init_part_block();

   group_pd.items.state.name = "state";
   group_pd.items.state.expandable = true;

   group_pd.items.item.name = "item";
   group_pd.items.item.expandable = true;

   group_pd.items.program.name = "program";
   group_pd.items.program.expandable = true;

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_CHANGED, _on_group_changed, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_SELECTED, _on_part_selected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_NAVIGATOR_UNSELECTED, _on_group_navigator_unselected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_STATE_SELECTED, _on_part_state_selected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROGRAM_SELECTED, _on_program_selected, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, _on_editor_attribute_changed, NULL);
}

Eina_List *
property_group_items_get()
{
   Eina_List *items = NULL;

   assert(group_pd.group != NULL);

   items = eina_list_append(items, &group_pd.items.group.title);
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
