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
#include "project_manager.h"

static Elm_Gengrid_Item_Class *_itc_tween = NULL;
static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   return strdup(data);
}

static Evas_Object *
_item_content_get(void *data, Evas_Object *obj, const char *part)
{
   const char* buf = NULL;
   const char *file = NULL, *group = NULL;
   Evas_Object *image;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        edje_object_file_get(ap.project->global_object, &file, &group);
        image = elm_thumb_add(obj);
        elm_object_style_set(image, "noframe");
        buf = eina_stringshare_printf("edje/images/%i",
                                      edje_edit_image_id_get(ap.project->global_object,
                                                             (const char*)data));
        elm_thumb_file_set(image, file, buf);
        return image;
     }
   return NULL;
}

static void
_item_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   assert(data != NULL);

   eina_stringshare_del(data);
}

Evas_Object *
property_image_tween_control_add(Evas_Object *parent)
{
   Evas_Object *item, *tween_list, *button, *icon;

   assert(parent != NULL);

   item = elm_layout_add(parent);
   elm_layout_theme_set(item, "layout", "property", "tween");
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(item);

   tween_list = elm_gengrid_add(item);
   elm_gengrid_item_size_set(tween_list, 96, 111);
   elm_gengrid_align_set(tween_list, 0.0, 0.0);
   elm_gengrid_horizontal_set(tween_list, true);
   elm_gengrid_multi_select_set(tween_list, true);

   if (!_itc_tween)
     {
        _itc_tween = elm_gengrid_item_class_new();
        _itc_tween->item_style = "default";
        _itc_tween->func.text_get = _item_label_get;
        _itc_tween->func.content_get = _item_content_get;
        _itc_tween->func.state_get = NULL;
        _itc_tween->func.del = _item_del;
     }

   elm_object_content_set(item, tween_list);

   BUTTON_ADD(item, button, NULL)
   ICON_STANDARD_ADD(button, icon, true, "plus");
   elm_object_part_content_set(button, NULL, icon);
//   evas_object_smart_callback_add(button, "clicked", _add_tween_image,
//                                  tween_list);
   elm_layout_content_set(item, "elm.swallow.add", button);

   BUTTON_ADD(item, button, NULL)
   ICON_STANDARD_ADD(button, icon, true, "minus");
   elm_object_part_content_set(button, NULL, icon);
//   evas_object_smart_callback_add(button, "clicked", _del_tween_image,
//                                  tween_list);
   elm_layout_content_set(item, "elm.swallow.del", button);

   evas_object_show(tween_list);

   return item;
}
