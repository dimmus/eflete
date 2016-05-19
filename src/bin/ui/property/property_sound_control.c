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
property_player_control_add(Evas_Object *parent)
{
   Evas_Object *control;

   assert(parent != NULL);

   control = elm_layout_add(parent);
   elm_layout_theme_set(control, "layout", "sound_editor", "preview");
   evas_object_size_hint_weight_set(control,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(control, EVAS_HINT_FILL, EVAS_HINT_FILL);

   return control;
}
