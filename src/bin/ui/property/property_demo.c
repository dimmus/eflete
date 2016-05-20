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
#include "demo_group.h"

#define DEMO_MASK(TYPE) (1u << TYPE)
#define DEMO_SIGNAL     DEMO_MASK(EDJE_PART_TYPE_NONE) /* in case of SIGNAL things are different */
#define DEMO_TEXT       DEMO_MASK(EDJE_PART_TYPE_TEXT)
#define DEMO_TEXTBLOCK  DEMO_MASK(EDJE_PART_TYPE_TEXTBLOCK)
#define DEMO_SWALLOW    DEMO_MASK(EDJE_PART_TYPE_SWALLOW)

struct _Property_Demo_Data {
   Demo_Part *part;
   Demo_Signal *signal;
   Property_Attribute items[PROPERTY_DEMO_ITEM_LAST];
};
typedef struct _Property_Demo_Data Property_Demo_Data;
static Property_Demo_Data demo_pd;

struct _Property_Demo_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Demo_Update_Info Property_Demo_Update_Info;

static const char *program_actions[] = {
     "NONE",
     "STATE SET",
     "SIGNAL EMIT",
     "DRAG VALUE SET",
     "DRAG VALUE STEP",
     "DRAG VALUE PAGE",
     "PLAY SAMPLE",
     "PLAY TONE",
     "ACTION STOP",
     NULL
};

static const char *swallow_content_type[] = {
     "None",
     "Rectangle",
     "Picture",
     "Widget",
     NULL};

/* array to find item by Attribute */
static Property_Demo_Update_Info attribute_map[PROPERTY_DEMO_ITEM_LAST];

/* local callbacks */
static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &demo_pd.items[TYPE]);
   switch (pa->type.demo_item)
     {
      case PROPERTY_DEMO_ITEM_TEXT_TITLE:
         APPEND(PROPERTY_DEMO_ITEM_TEXT_NAME);
         APPEND(PROPERTY_DEMO_ITEM_TEXT_CONTENT);
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_TITLE:
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_NAME);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_CONTENT);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_PICTURE);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_WIDGET);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_STYLE);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_RECTANGLE);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_MIN);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_MAX);
         break;
      case PROPERTY_DEMO_ITEM_PROGRAM_TITLE:
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_SOURCE);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_ACTION);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_EMIT);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_EMITTER);
         break;
      default:
         CRIT("items callback not found for %s", pa->name);
         abort();
     }

   return items;
#undef APPEND
}

static void
_fill_combobox_with_enum(Evas_Object *control, const char **array)
{
   int i = 0;

   assert(control != NULL);
   assert(array != NULL);

   while (array[i] != NULL)
     {
        ewe_combobox_item_add(control, array[i]);
        ++i;
     }
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   Eina_Stringshare *str_val1 = NULL;
   Ewe_Combobox_Item *cb_item = NULL;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->control_type)
     {
      case PROPERTY_CONTROL_ENTRY:
         str_val1 = property_entry_get(action->control);
         break;
      case PROPERTY_CONTROL_COMBOBOX:
         TODO("change this after migrating to elm_combobox");
         cb_item = ewe_combobox_select_item_get(action->control);
      default:
         break;
     }

   switch (pa->type.demo_item)
     {
      case PROPERTY_DEMO_ITEM_TEXT_CONTENT:
         if (demo_pd.part->text_content)
           eina_stringshare_del(demo_pd.part->text_content);
         demo_pd.part->text_content = eina_stringshare_add(str_val1);
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_TEXT_SET, demo_pd.part);
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_CONTENT:
         demo_pd.part->swallow_content = cb_item->index;
         demo_pd.part->change = true;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
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

   switch (pa->type.demo_item)
     {
      case PROPERTY_DEMO_ITEM_TEXT_NAME:
         elm_layout_text_set(action->control, NULL, demo_pd.part->name);
         break;
      case PROPERTY_DEMO_ITEM_TEXT_CONTENT:
         property_entry_set(action->control, demo_pd.part->text_content);
         break;

      case PROPERTY_DEMO_ITEM_SWALLOW_NAME:
         elm_layout_text_set(action->control, NULL, demo_pd.part->name);
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_CONTENT:
         ewe_combobox_select_item_set(action->control, demo_pd.part->swallow_content);
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_PICTURE:
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_WIDGET:
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_STYLE:
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_RECTANGLE:
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_MIN:
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_MAX:
         break;

      case PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->sig_name);
         break;
      case PROPERTY_DEMO_ITEM_PROGRAM_SOURCE:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->source_name);
         break;
      case PROPERTY_DEMO_ITEM_PROGRAM_ACTION:
         elm_layout_text_set(action->control, NULL, program_actions[demo_pd.signal->action]);
         break;
      case PROPERTY_DEMO_ITEM_PROGRAM_EMIT:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->emit_signal);
         break;
      case PROPERTY_DEMO_ITEM_PROGRAM_EMITTER:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->emitter);
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

   switch (pa->type.demo_item)
     {
      case PROPERTY_DEMO_ITEM_TEXT_NAME:
      case PROPERTY_DEMO_ITEM_TEXT_CONTENT:
      case PROPERTY_DEMO_ITEM_SWALLOW_NAME:
      case PROPERTY_DEMO_ITEM_SWALLOW_PICTURE:
      case PROPERTY_DEMO_ITEM_SWALLOW_WIDGET:
      case PROPERTY_DEMO_ITEM_SWALLOW_STYLE:
      case PROPERTY_DEMO_ITEM_SWALLOW_RECTANGLE:
      case PROPERTY_DEMO_ITEM_SWALLOW_MIN:
      case PROPERTY_DEMO_ITEM_SWALLOW_MAX:
      case PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL:
      case PROPERTY_DEMO_ITEM_PROGRAM_SOURCE:
      case PROPERTY_DEMO_ITEM_PROGRAM_ACTION:
      case PROPERTY_DEMO_ITEM_PROGRAM_EMIT:
      case PROPERTY_DEMO_ITEM_PROGRAM_EMITTER:
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_CONTENT:
         _fill_combobox_with_enum(action->control, swallow_content_type);
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
                 Property_Control control_type)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->control_type = control_type;
   action->init_cb = _init_cb;
   action->update_cb = _update_cb;
   action->change_cb = _change_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type)
{
   _action_internal(&pa->action1, name, units, control_type);

   assert(attribute_map[pa->type.demo_item].pa == NULL);
   assert(attribute_map[pa->type.demo_item].action == NULL);

   attribute_map[pa->type.demo_item].pa = pa;
   attribute_map[pa->type.demo_item].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type)
{
   _action_internal(&pa->action2, name, units, control_type);

   assert(attribute_map[pa->type.demo_item].pa == NULL);
   assert(attribute_map[pa->type.demo_item].action == NULL);

   attribute_map[pa->type.demo_item].pa = pa;
   attribute_map[pa->type.demo_item].action = &pa->action2;
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   if (demo_pd.part)
     return !!(pa->filter_data.demo_types & DEMO_MASK(demo_pd.part->type));
   else if (demo_pd.signal)
     return !!(pa->filter_data.demo_types & DEMO_MASK(EDJE_PART_TYPE_NONE));
   else
     return false;
}

static void
_init_items()
{
   Property_Demo_Item it;
#define IT demo_pd.items[it]
   for (it = 0 /* first element of enum */; it < PROPERTY_DEMO_ITEM_LAST; it++)
     {
        IT.type.demo_item = it;
        IT.filter_cb = _filter_cb;

        /* default value: show for all types */
        IT.filter_data.demo_types = ~0u;
        switch(it)
          {
           case PROPERTY_DEMO_ITEM_TEXT_TITLE:
              IT.name = "text";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.demo_types = DEMO_TEXT | DEMO_TEXTBLOCK;
              break;
           case PROPERTY_DEMO_ITEM_TEXT_NAME:
              IT.name = "name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_DEMO_ITEM_TEXT_CONTENT:
              IT.name = "content";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY);
              break;

           case PROPERTY_DEMO_ITEM_SWALLOW_TITLE:
              IT.name = "swallow";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.demo_types = DEMO_SWALLOW;
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_NAME:
              IT.name = "name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_CONTENT:
              IT.name = "content";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_PICTURE:
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_WIDGET:
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_STYLE:
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_RECTANGLE:
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_MIN:
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_MAX:
              break;

           case PROPERTY_DEMO_ITEM_PROGRAM_TITLE:
              IT.name = "program";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.demo_types = DEMO_SIGNAL;
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL:
              IT.name = "signal";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_SOURCE:
              IT.name = "source";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_ACTION:
              IT.name = "action";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_EMIT:
              IT.name = "emit signal";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_EMITTER:
              IT.name = "emitter";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;

           case PROPERTY_DEMO_ITEM_LAST:
              break;
          }
     }
#undef IT
}

static void
_on_demo_part_clicked(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Property_Data *pd = data;
   demo_pd.part = event_info;
   demo_pd.signal = NULL;

   GENLIST_FILTER_APPLY(pd->genlist);

   if (demo_pd.part)
     {
        property_item_update_recursively(&demo_pd.items[PROPERTY_DEMO_ITEM_TEXT_TITLE]);
        property_item_update_recursively(&demo_pd.items[PROPERTY_DEMO_ITEM_SWALLOW_TITLE]);
     }
}
static void
_on_demo_program_clicked(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Property_Data *pd = data;
   demo_pd.part = NULL;
   demo_pd.signal = event_info;

   GENLIST_FILTER_APPLY(pd->genlist);

   if (demo_pd.signal)
     property_item_update_recursively(&demo_pd.items[PROPERTY_DEMO_ITEM_PROGRAM_TITLE]);
}
static void
_on_unclicked(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Property_Data *pd = data;
   demo_pd.part = NULL;
   demo_pd.signal = NULL;

   GENLIST_FILTER_APPLY(pd->genlist);
}

/* public */
void
property_demo_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_PART_CLICKED, _on_demo_part_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_PART_CLICKED, _on_demo_part_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_PROGRAM_PART_CLICKED, _on_demo_program_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, _on_unclicked, pd);
}

Eina_List *
property_demo_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &demo_pd.items[PROPERTY_DEMO_ITEM_TEXT_TITLE]);
   items = eina_list_append(items, &demo_pd.items[PROPERTY_DEMO_ITEM_SWALLOW_TITLE]);
   items = eina_list_append(items, &demo_pd.items[PROPERTY_DEMO_ITEM_PROGRAM_TITLE]);

   return items;
}
