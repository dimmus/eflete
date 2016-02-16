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

Evas_Object *
property_add(Evas_Object *parent)
{
   assert(parent != NULL);

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
