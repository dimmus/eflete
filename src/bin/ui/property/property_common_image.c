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
#include "main_window.h"

Evas_Object *
property_image_normal_control_add(Evas_Object *parent)
{
   Evas_Object *content, *btn;

   assert(parent != NULL);

   ENTRY_ADD(parent, content, true);
   btn = elm_button_add(content);
   elm_object_style_set(btn, "elipsis");
   //         evas_object_smart_callback_add(btn, "clicked", btn_func_cb, pd);
   //         evas_object_smart_callback_add(content, "clicked", btn_func_cb, pd);
   elm_object_part_content_set(content, "elm.swallow.elipsis", btn);
   elm_entry_editable_set(content, false);
   evas_object_show(btn);
   evas_object_show(content);
   return content;
}
