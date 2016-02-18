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

static char *
_caption_text_get(void *data,
                  Evas_Object *obj __UNUSED__,
                  const char *part)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert(!strcmp(part, "text.item.name"));

   if (pa->name != NULL)
     return strdup(pa->name);

   return NULL;
}

static char *
_1swallow_text_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char *part)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert((!strcmp(part, "text.item.name"))    ||
          (!strcmp(part, "text.action1.name")) ||
          (!strcmp(part, "text.action1.units")));

   if ((pa->name != NULL) && (!strcmp(part, "text.item.name")))
     return strdup(pa->name);

   if ((pa->action1.name != NULL) && (!strcmp(part, "text.action1.name")))
     return strdup(pa->action1.name);

   if ((pa->action1.units != NULL) && (!strcmp(part, "text.action1.units")))
     return strdup(pa->action1.units);

   return NULL;
}

static char *
_2swallow_text_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char *part)
{
   Property_Attribute *pa = data;

   assert(pa != NULL);
   /* Check that item style is correct. Don't need this check in release build */
   assert((!strcmp(part, "text.item.name"))     ||
          (!strcmp(part, "text.action1.name"))  ||
          (!strcmp(part, "text.action1.units")) ||
          (!strcmp(part, "text.action2.name"))  ||
          (!strcmp(part, "text.action2.units")));

   if ((pa->name != NULL) && (!strcmp(part, "text.item.name")))
     return strdup(pa->name);

   if ((pa->action1.name != NULL) && (!strcmp(part, "text.action1.name")))
     return strdup(pa->action1.name);

   if ((pa->action1.units != NULL) && (!strcmp(part, "text.action1.units")))
     return strdup(pa->action1.units);

   if ((pa->action2.name != NULL) && (!strcmp(part, "text.action2.name")))
     return strdup(pa->action1.name);

   if ((pa->action2.units != NULL) && (!strcmp(part, "text.action2.units")))
     return strdup(pa->action1.units);

   return NULL;
}

void
property_common_itc_init()
{
   /* init item classes */
   pd.itc_caption = elm_genlist_item_class_new();
   pd.itc_caption->func.text_get = _caption_text_get;

   pd.itc_1swallow = elm_genlist_item_class_new();
   pd.itc_1swallow->func.text_get = _1swallow_text_get;

   pd.itc_2swallow = elm_genlist_item_class_new();
   pd.itc_2swallow->func.text_get = _2swallow_text_get;

   /* map control pairs to item classes */
   pd.item_classes[PROPERTY_CONTROL_NONE][PROPERTY_CONTROL_NONE] = pd.itc_caption;
}
