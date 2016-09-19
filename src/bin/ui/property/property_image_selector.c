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

static Eina_Bool
_on_image_done(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   const char *value;
   Eina_List *list_selected = (Eina_List *)event_info;
   const char *selected;

   if (!list_selected) return false;
   value = property_entry_get(data);

   selected = eina_list_data_get(list_selected);

   if (strcmp(value, selected) == 0) return true;
   property_entry_set(data, selected);

   evas_object_smart_callback_call(data, signals.eflete.property.image_selector.changed, NULL);

   return true;
}

static void
_on_image_choose(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   popup_fileselector_image_helper(NULL,
                                   data,
                                   NULL,
                                   _on_image_done,
                                   data,
                                   false,
                                   false);
}

Evas_Object *
property_image_selector_get(Evas_Object *parent)
{
   Evas_Object *content, *btn;

   assert(parent != NULL);

   ENTRY_ADD(parent, content, true);
   btn = elm_button_add(content);
   elm_object_style_set(btn, "elipsis");
   evas_object_smart_callback_add(content, signals.elm.entry.clicked, _on_image_choose, content);
   elm_object_part_content_set(content, "elm.swallow.elipsis", btn);
   evas_object_smart_callback_add(btn, signals.elm.button.clicked, _on_image_choose, content);
   elm_entry_editable_set(content, false);
   evas_object_show(btn);
   evas_object_show(content);

   evas_object_event_callback_add(content,
                                  EVAS_CALLBACK_FREE,
                                  popup_active_helper_close,
                                  (void*)(uintptr_t)POPUP_FILESELECTOR_IMAGE_HELPER);

   return content;
}
