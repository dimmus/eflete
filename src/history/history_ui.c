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

#include "history.h"
#include "history_private.h"

static  Elm_Genlist_Item_Class *_itc_module = NULL;
static  Elm_Genlist_Item_Class *_itc_change = NULL;

static char *
_module_item_label_get(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   return strdup("Discard all changes");
}

static char *
_item_label_get(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   return strdup(" ");
}

Evas_Object *
_history_ui_add(Evas_Object *parent)
{
   Evas_Object *genlist = NULL;

   if (!_itc_module)
     {
        _itc_module = elm_genlist_item_class_new();
        _itc_module->item_style = "eflete/level1";
        _itc_module->func.text_get = _module_item_label_get;
        _itc_module->func.content_get = NULL;
        _itc_module->func.state_get = NULL;
        _itc_module->func.del = NULL;
     }

   if (!_itc_change)
     {
        _itc_change = elm_genlist_item_class_new();
        _itc_change->item_style = "eflete/history";
        _itc_change->func.text_get = _item_label_get;
        _itc_change->func.content_get = NULL;
        _itc_change->func.state_get = NULL;
        _itc_change->func.del = NULL;
     }

   genlist = elm_genlist_add(parent);

   elm_object_style_set(genlist, "eflete/default");
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   return genlist;
}

