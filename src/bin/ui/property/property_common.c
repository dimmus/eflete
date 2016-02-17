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

char *
property_common_caption_text_get(void *data,
                                 Evas_Object *obj __UNUSED__,
                                 const char *part __UNUSED__)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   assert(pa->name != NULL);

   return strdup(pa->name);
}

void
property_common_itc_init()
{
   pd.itc_caption = elm_genlist_item_class_new();
   pd.itc_caption->func.text_get = property_common_caption_text_get;
}
