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

#include "main_window.h"
#include "project_manager.h"
#include "property.h"
#include "property_private.h"

typedef struct {
   Property_Attribute item_preview;

   Property_Attribute item_name;
   Property_Attribute item_location;
   Property_Attribute item_type;
   Property_Attribute item_image_compression;
   Property_Attribute item_image_quality;
   Property_Attribute item_width;
   Property_Attribute item_height;
} Property_Image_Data;

static Property_Image_Data image_data;

void
property_image_manager_init()
{
   image_data.item_name.name = eina_stringshare_add(_("name"));
   image_data.item_name.action1.control_type = PROPERTY_CONTROL_LABEL;

   image_data.item_location.name = eina_stringshare_add(_("location"));
   image_data.item_location.action1.control_type = PROPERTY_CONTROL_LABEL;

   image_data.item_type.name = eina_stringshare_add(_("type"));
   image_data.item_type.action1.control_type = PROPERTY_CONTROL_LABEL;

   image_data.item_image_compression.name = eina_stringshare_add(_("image compression"));
   image_data.item_image_compression.action1.control_type = PROPERTY_CONTROL_COMBOBOX;

   image_data.item_image_quality.name = eina_stringshare_add(_("image quality"));
   image_data.item_image_quality.action1.control_type = PROPERTY_CONTROL_SPINNER;

   image_data.item_width.name = eina_stringshare_add(_("width"));
   image_data.item_width.action1.control_type = PROPERTY_CONTROL_LABEL;
   image_data.item_height.name = eina_stringshare_add(_("height"));
   image_data.item_height.action1.control_type = PROPERTY_CONTROL_LABEL;
}

Eina_List *
property_image_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &image_data.item_name);
   items = eina_list_append(items, &image_data.item_location);
   items = eina_list_append(items, &image_data.item_type);
   items = eina_list_append(items, &image_data.item_image_compression);
   items = eina_list_append(items, &image_data.item_image_quality);
   items = eina_list_append(items, &image_data.item_width);
   items = eina_list_append(items, &image_data.item_height);

   return items;
}
