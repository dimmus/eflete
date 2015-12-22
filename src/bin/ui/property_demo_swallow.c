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
   Evas_Object *box;

   Evas_Object *name;
   Evas_Object *swallow_content;

   Evas_Object *color;
   Evas_Object *picture;
   Evas_Object *widget;

   Evas_Object *size_h, *size_w;
   Evas_Object *hint_h, *hint_w;
   Evas_Object *align_x, *align_y;

   Part_ *part;
};
typedef struct _Demo_Swallow_Prop_Data Demo_Swallow_Prop_Data;

static const char *swallow_content_type[] = { N_("None"),
                                         N_("Rectangle"),
                                         N_("Picture"),
                                         N_("Widget"),
                                         NULL};

TODO("move from here, property_demo_text and property_group all common stuff")
static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   assert(parent != NULL);
   assert(label != NULL);

   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

static void
_on_content_set(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Demo_Swallow_Prop_Data *pd __UNUSED__ = (Demo_Swallow_Prop_Data *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;
   if (item->index == 0)
     return;
}

static Evas_Object *
prop_swallow_content_add(Evas_Object *parent, Demo_Swallow_Prop_Data *pd)
{
   int i;
   PROPERTY_ITEM_ADD(parent, _("Content Set"), "1swallow")
   EWE_COMBOBOX_ADD(parent, pd->swallow_content)
   for (i = 0; swallow_content_type[i]; ewe_combobox_item_add(pd->swallow_content, swallow_content_type[i]), i++) ;
   evas_object_smart_callback_add(pd->swallow_content, "selected", _on_content_set, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->swallow_content);
   ewe_combobox_select_item_set(pd->swallow_content, 0);
   return item;
}

void
ui_property_demo_swallow_part_set(Evas_Object *property, Part_ *part)
{
   DEMO_SWALLOW_PROP_DATA_GET()

   /* setting */
   if (part)
     {
        elm_object_text_set(pd->name, part->name);
     }

   pd->part = part;

   /* showing */
   evas_object_show(property);
}

Evas_Object *
ui_property_demo_swallow_add(Evas_Object *parent)
{
   Demo_Swallow_Prop_Data *pd;
   Evas_Object *item;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Demo_Swallow_Prop_Data));

   /* editors */
   BOX_ADD(parent, pd->box, false, false);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);
   evas_object_data_set(pd->box, DEMO_SWALLOW_PROP_DATA, pd);

   /* Frame with info */
   elm_box_align_set(pd->box, 0.5, 0.0);
   item = prop_item_label_add(pd->box, &pd->name, _("name"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_swallow_content_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);

   return pd->box;
}

