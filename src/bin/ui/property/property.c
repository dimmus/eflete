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
#include "property_common.h"

#define MODE_CB(NAME, MODE) \
static void \
NAME(void *data, \
     Evas_Object *obj __UNUSED__, \
     void *event_info __UNUSED__) \
{ \
   Property_Data *pd = data; \
   property_mode_set(pd->genlist, MODE); \
}

MODE_CB(_none_mode, PROPERTY_MODE_NONE)
MODE_CB(_image_mode, PROPERTY_MODE_IMAGE)
MODE_CB(_sound_mode, PROPERTY_MODE_SOUND)
MODE_CB(_style_mode, PROPERTY_MODE_STYLE)
MODE_CB(_color_class_mode, PROPERTY_MODE_COLOR_CLASS)
MODE_CB(_group_mode, PROPERTY_MODE_GROUP)
MODE_CB(_demo_mode, PROPERTY_MODE_DEMO)

static void
_items_add(Evas_Object *property, Eina_List **items, Elm_Object_Item *parent)
{
   Property_Attribute *pa;

   EINA_LIST_FREE(*items, pa)
      property_item_add(property, pa, parent);
}

void
property_item_add(Evas_Object *property, Property_Attribute *pa, Elm_Object_Item *parent)
{
   PROPERTY_DATA_GET(property);
   Eina_List *subitems;

   assert(pa != NULL);
   assert(pd->item_classes[pa->action1.control_type][pa->action2.control_type] != NULL);

   /* item is already added */
   if (pa->glit) return;

   pa->glit = elm_genlist_item_append(pd->genlist,
                                      pd->item_classes[pa->action1.control_type][pa->action2.control_type],
                                      pa,
                                      parent,
                                      (pa->expandable) ? ELM_GENLIST_ITEM_TREE : ELM_GENLIST_ITEM_NONE,
                                      NULL,
                                      NULL);
   if (pa->expanded)
     {
        assert(pa->expandable);
        elm_genlist_item_expanded_set(pa->glit, true);
     }
   else if ((!pa->expandable) && (pa->expand_cb != NULL))
     {
        subitems = pa->expand_cb(pa);
        _items_add(property, &subitems, pa->glit);
     }
}

static void
_expand_request_cb(void *data __UNUSED__,
                   Evas_Object *o __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, true);
}

static void
_contract_request_cb(void *data __UNUSED__,
                     Evas_Object *o __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, false);
}

static void
_expanded_cb(void *data __UNUSED__,
             Evas_Object *obj,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Property_Attribute *pa = elm_object_item_data_get(glit);
   Eina_List *items;

   assert(pa != NULL);
   assert(pa->expand_cb != NULL);

   pa->expanded = true;
   items = pa->expand_cb(pa);
   _items_add(obj, &items, pa->glit);
}

static void
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Property_Attribute *pa = elm_object_item_data_get(glit);

   assert(pa != NULL);

   pa->expanded = false;
   elm_genlist_item_subitems_clear(glit);
}

static void
_realized_cb(void *data __UNUSED__,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Property_Attribute *pa = elm_object_item_data_get(glit);

   assert(pa != NULL);

   pa->realized = true;

   property_item_update(pa);
}

static void
_unrealized_cb(void *data,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Property_Data *pd = data;
   Elm_Object_Item *glit = event_info;
   Property_Attribute *pa = elm_object_item_data_get(glit);

   assert(pd != NULL);
   assert(pa != NULL);

   pa->realized = false;
   /* we need to manually remove content because genlist can later realize
      new item with old content. o_O */
   evas_object_del(elm_object_item_part_content_get(glit, "swallow.icon"));
   if (pa->action1.control)
     {
        /* forcing unfocus callback to stop entry change before setting control to NULL */
        elm_object_focus_set(pa->action1.control, false);
        evas_object_del(pa->action1.control);
        pa->action1.control = NULL;
     }
   if (pa->action2.control)
     {
        elm_object_focus_set(pa->action2.control, false);
        evas_object_del(pa->action2.control);
        pa->action2.control = NULL;
     }
   elm_object_focus_set(pd->genlist, true);
}

Evas_Object *
property_add(Evas_Object *parent)
{
   Property_Data *pd;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Property_Data));

   property_common_itc_init(pd);
   property_dummy_init();
   property_color_class_manager_init();
   property_image_manager_init();
   property_group_init(pd);

   evas_object_smart_callback_add(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, _none_mode, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_EDITOR_TAB_CLICKED, _image_mode, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_EDITOR_TAB_CLICKED, _sound_mode, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_STYLE_EDITOR_TAB_CLICKED, _style_mode, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_COLOR_EDITOR_TAB_CLICKED, _color_class_mode, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROPERTY_MODE_GROUP, _group_mode, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_PROPERTY_MODE_DEMO, _demo_mode, pd);

   pd->genlist = elm_genlist_add(parent);
   elm_genlist_block_count_set(pd->genlist, 64);
   elm_scroller_policy_set(pd->genlist, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_ON);
   elm_object_style_set(pd->genlist, "property");

   evas_object_data_set(pd->genlist, PROPERTY_DATA, pd);

   evas_object_smart_callback_add(pd->genlist, "expand,request", _expand_request_cb, pd);
   evas_object_smart_callback_add(pd->genlist, "contract,request", _contract_request_cb, pd);
   evas_object_smart_callback_add(pd->genlist, "expanded", _expanded_cb, pd);
   evas_object_smart_callback_add(pd->genlist, "contracted", _contracted_cb, pd);
   evas_object_smart_callback_add(pd->genlist, "realized", _realized_cb, pd);
   evas_object_smart_callback_add(pd->genlist, "unrealized", _unrealized_cb, pd);

   return pd->genlist;
}

void
property_mode_set(Evas_Object *property, Property_Mode mode)
{
   PROPERTY_DATA_GET(property);
   Eina_List *items = NULL;

   assert (pd->genlist != NULL);

   if (mode == pd->mode) return;

   pd->mode = mode;
   DBG("changing property mode to %d", mode);

   elm_genlist_clear(pd->genlist);
   switch (mode)
     {
      case PROPERTY_MODE_NONE:
         items = property_dummy_items_get();
         break;
      case PROPERTY_MODE_GROUP:
         items = property_group_items_get();
         break;
      case PROPERTY_MODE_STYLE:
      case PROPERTY_MODE_SOUND:
      case PROPERTY_MODE_DEMO:
         break;
      case PROPERTY_MODE_COLOR_CLASS:
         items = property_color_class_manager_items_get();
         break;
      case PROPERTY_MODE_IMAGE:
         items = property_image_manager_items_get();
         break;
     }
   _items_add(property, &items, NULL);
   GENLIST_FILTER_APPLY(pd->genlist);
}

void
property_item_update(Property_Attribute *pa)
{
   assert(pa != NULL);

   if (!pa->realized) return;

   if (pa->action1.update_cb != NULL)
     {
        DBG("calling update_cb of %s (%s)", pa->name, (pa->action1.name) ? pa->action1.name : "unnamed");
        pa->action1.update_cb(pa, &pa->action1);
     }
   if (pa->action2.update_cb != NULL)
     {
        DBG("calling update_cb of %s (%s)", pa->name, (pa->action2.name) ? pa->action2.name : "unnamed");
        pa->action2.update_cb(pa, &pa->action2);
     }
}

void
property_item_update_recursively(Property_Attribute *pa)
{
   const Eina_List *subitems, *l;
   Elm_Object_Item *glit;

   assert(pa != NULL);

   property_item_update(pa);

   subitems = elm_genlist_item_subitems_get(pa->glit);
   EINA_LIST_FOREACH(subitems, l, glit)
      property_item_update_recursively(elm_object_item_data_get(glit));
}
