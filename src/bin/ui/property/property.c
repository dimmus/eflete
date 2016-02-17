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


Evas_Object *
property_add(Evas_Object *parent)
{
   assert(parent != NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, _none_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_IMAGE_EDITOR_TAB_CLICKED, _image_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_EDITOR_TAB_CLICKED, _sound_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_STYLE_EDITOR_TAB_CLICKED, _style_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_COLOR_EDITOR_TAB_CLICKED, _color_class_mode, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _group_mode, NULL);

   pd.genlist = elm_genlist_add(parent);
   return pd.genlist;
}

void
property_mode_set(Property_Mode mode)
{
   assert (pd.genlist != NULL);

   if (mode == pd.mode) return;

   pd.mode = mode;
   DBG("changing property mode to %d", mode);

   elm_genlist_clear(pd.genlist);
   switch (mode)
     {
      case PROPERTY_MODE_NONE:
      case PROPERTY_MODE_GROUP:
      case PROPERTY_MODE_COLOR_CLASS:
      case PROPERTY_MODE_STYLE:
      case PROPERTY_MODE_SOUND:
      case PROPERTY_MODE_IMAGE:
      case PROPERTY_MODE_DEMO:
         break;
     }
}
