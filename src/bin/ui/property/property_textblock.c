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

struct _Property_Textblock_Data {
   Property_Attribute items[PROPERTY_TEXTBLOCK_ITEM_LAST];
};
typedef struct _Property_Textblock_Data Property_Textblock_Data;
static Property_Textblock_Data textblock_pd;

struct _Property_Textblock_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Textblock_Update_Info Property_Textblock_Update_Info;

/* array to find item by Attribute */
static Property_Textblock_Update_Info attribute_map[ATTRIBUTE_TEXTBLOCK_ITEM_LAST];

/* local callbacks */
static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &textblock_pd.items[TYPE]);
   switch (pa->type.textblock_item)
     {
      case PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE);
         break;
      default:
         CRIT("items callback not found for %s", pa->name);
         abort();
     }

   return items;
#undef APPEND
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->control_type)
     {
      default:
         break;
     }

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         break;

      default:
         TODO("remove default case after all attributes will be added");
         CRIT("change callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("update callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("init callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type, Attribute_Textblock_Item attribute)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->control_type = control_type;
   action->type.attribute_textblock = attribute;
   action->init_cb = _init_cb;
   action->update_cb = _update_cb;
   action->change_cb = _change_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Textblock_Item attribute)
{
   _action_internal(&pa->action1, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Textblock_Item attribute)
{
   _action_internal(&pa->action2, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action2;
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   /* will be updated later on */
   return true;
}

static void
_init_items()
{
   Property_Textblock_Item it;
#define IT textblock_pd.items[it]
   for (it = 0 /* first element of enum */; it < PROPERTY_TEXTBLOCK_ITEM_LAST; it++)
     {
        IT.type.textblock_item = it;
        IT.filter_cb = _filter_cb;

        /* default value: show for all types */
        IT.filter_data.textblock_types = ~0u;
        switch(it)
          {
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE:
              IT.name = "text";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
              IT.name = "font name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
              IT.name = "font style (weight)";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
              IT.name = "font style (width)";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR:
              IT.name = "color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE:
              IT.name = "size";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE);
              break;

           case PROPERTY_TEXTBLOCK_ITEM_LAST:
              break;
          }
     }
#undef IT
}

/* public */
void
property_textblock_manager_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();
}

Eina_List *
property_textblock_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE]);

   return items;
}
