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

Property_Data pd;

#define MODE_CB(NAME, MODE) \
static void \
NAME(void *data __UNUSED__, \
     Evas_Object *obj __UNUSED__, \
     void *event_info __UNUSED__) \
{ \
   property_mode_set(MODE); \
}

MODE_CB(_none_mode, PROPERTY_MODE_NONE)
MODE_CB(_image_mode, PROPERTY_MODE_IMAGE)
MODE_CB(_sound_mode, PROPERTY_MODE_SOUND)
MODE_CB(_style_mode, PROPERTY_MODE_STYLE)
MODE_CB(_color_class_mode, PROPERTY_MODE_COLOR_CLASS)
MODE_CB(_group_mode, PROPERTY_MODE_GROUP)

static void
_items_add(Eina_List **items, Elm_Object_Item *parent)
{
   Property_Attribute *pa;

   EINA_LIST_FREE(*items, pa)
     {
        assert(pd.item_classes[pa->action1.control_type][pa->action2.control_type] != NULL);

        pa->glit = elm_genlist_item_append(pd.genlist,
                                           pd.item_classes[pa->action1.control_type][pa->action2.control_type],
                                           pa,
                                           parent,
                                           (pa->expand_cb != NULL) ? ELM_GENLIST_ITEM_TREE : ELM_GENLIST_ITEM_NONE,
                                           NULL,
                                           NULL);
        if (pa->expanded)
          elm_genlist_item_expanded_set(pa->glit, true);

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
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Property_Attribute *pa = elm_object_item_data_get(glit);
   Eina_List *items;

   assert(pa != NULL);
   assert(pa->expand_cb != NULL);

   items = pa->expand_cb(pa);
   _items_add(&items, pa->glit);
}

static void
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

Evas_Object *
property_add(Evas_Object *parent)
{
   assert(parent != NULL);

   property_common_itc_init();
   property_dummy_init();

   evas_object_smart_callback_add(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, _none_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_EDITOR_TAB_CLICKED, _image_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_EDITOR_TAB_CLICKED, _sound_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_STYLE_EDITOR_TAB_CLICKED, _style_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_COLOR_EDITOR_TAB_CLICKED, _color_class_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _group_mode, NULL);

   pd.genlist = elm_genlist_add(parent);
   elm_object_style_set(pd.genlist, "property");

   evas_object_smart_callback_add(pd.genlist, "expand,request", _expand_request_cb, NULL);
   evas_object_smart_callback_add(pd.genlist, "contract,request", _contract_request_cb, NULL);
   evas_object_smart_callback_add(pd.genlist, "expanded", _expanded_cb, NULL);
   evas_object_smart_callback_add(pd.genlist, "contracted", _contracted_cb, NULL);

   return pd.genlist;
}

void
property_mode_set(Property_Mode mode)
{
   Eina_List *items = NULL;

   assert (pd.genlist != NULL);

   if (mode == pd.mode) return;

   pd.mode = mode;
   DBG("changing property mode to %d", mode);

   elm_genlist_clear(pd.genlist);
   switch (mode)
     {
      case PROPERTY_MODE_NONE:
         items = property_dummy_items_get();
         break;
      case PROPERTY_MODE_GROUP:
      case PROPERTY_MODE_COLOR_CLASS:
      case PROPERTY_MODE_STYLE:
      case PROPERTY_MODE_SOUND:
      case PROPERTY_MODE_IMAGE:
      case PROPERTY_MODE_DEMO:
         break;
     }
   _items_add(&items, NULL);
}
