/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "string_common.h"
#include "common_macro.h"
#include "new_history.h"
#include "editor.h"
#include "validator.h"

#include "signals.h"

#define DEMO_SWALLOW_PROP_DATA "image_prop_data"

#define DEMO_SWALLOW_PROP_DATA_GET() \
   assert(property != NULL); \
   Demo_Swallow_Prop_Data *pd = evas_object_data_get(property, DEMO_SWALLOW_PROP_DATA); \
   assert(pd != NULL);

struct _Demo_Swallow_Prop_Data
{
   Evas_Object *name;
   Evas_Object *visibility;

   Evas_Object *swallow_content;

   Evas_Object *color;
   Evas_Object *picture;
   Evas_Object *widget;

   Evas_Object *size_h, *size_w;
   Evas_Object *hint_h, *hint_w;
   Evas_Object *align_x, *align_y;
};
typedef struct _Demo_Swallow_Prop_Data Demo_Swallow_Prop_Data;


