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
#include "project_manager2.h"

/* gengrid base functions */

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
        /* use image instead of thumbs (for now) because currently thumbs
           doesn't show image from edje file correctly. */
        TODO("use image resources from project folder instead of edje")
        image = elm_image_add(obj);
        elm_object_style_set(image, "noframe");
        buf = eina_stringshare_printf("edje/images/%i",
                                      edje_edit_image_id_get(ap.project->global_object,
                                                             (const char*)data));
        elm_image_file_set(image, file, buf);
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

/* tweens functions */

static Eina_List *deleted_tweens = NULL;
static Eina_List *added_tweens = NULL;;

static void
_del_tween_image(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *control = (Evas_Object *)data;
   Evas_Object *tween_list = elm_layout_content_get(control, NULL);
   const Eina_List *sel_list = elm_gengrid_selected_items_get(tween_list), *l;
   Eina_List *selected = NULL;
   Elm_Object_Item *it;
   const char *name;

   EINA_LIST_FOREACH(sel_list, l, it)
     {
        name = elm_object_item_data_get(it);
        deleted_tweens = eina_list_append(deleted_tweens, eina_stringshare_add(name));
     }

   eina_list_free(selected);
   evas_object_smart_callback_call(control, signals.eflete.property.image_tween_control.changed, NULL);
}

static Eina_Bool
_on_image_editor_tween_done(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Evas_Object *control = (Evas_Object *)data;
   Evas_Object *tween_list __UNUSED__ = elm_layout_content_get(control, NULL);
   Eina_List *selected = (Eina_List *)event_info;
   Eina_List *l = NULL;
   const char *name = NULL;

   if (!selected) return false;
   EINA_LIST_FOREACH(selected, l, name)
     {
        if (strcmp(name, EFLETE_DUMMY_IMAGE_NAME) != 0)
          added_tweens = eina_list_append(added_tweens, name);
     }

   if (added_tweens)
     evas_object_smart_callback_call(control, signals.eflete.property.image_tween_control.changed, NULL);
TODO("apply when popup will be fixed");
//   elm_object_scroll_freeze_pop(tween_list);

   return true;
}

static void
_add_tween_image(void *data,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   Evas_Object *control = (Evas_Object *)data;

   popup_gengrid_image_helper(NULL,
                              obj,
                              _on_image_editor_tween_done,
                              control,
                              true,
                              false);
TODO("apply when popup will be fixed");
//   elm_object_scroll_freeze_push(data);
}

Eina_List *
property_image_tween_added_list_get()
{
   return added_tweens;
}

Eina_List *
property_image_tween_deleted_list_get()
{
   return deleted_tweens;
}

void
property_image_tween_lists_free()
{
   /* code taken from edje_edit_string_list_free */
   while (added_tweens)
     {
        if (eina_list_data_get(added_tweens))
          eina_stringshare_del(eina_list_data_get(added_tweens));
        added_tweens = eina_list_remove(added_tweens, eina_list_data_get(added_tweens));
     }
   while (deleted_tweens)
     {
        if (eina_list_data_get(deleted_tweens))
          eina_stringshare_del(eina_list_data_get(deleted_tweens));
        deleted_tweens = eina_list_remove(deleted_tweens, eina_list_data_get(deleted_tweens));
     }
   added_tweens = NULL;
   deleted_tweens = NULL;
}

void
property_image_tween_cleanup(Evas_Object *control)
{
   Evas_Object *tween_list;

   assert(control != NULL);
   tween_list = elm_layout_content_get(control, NULL);
   elm_gengrid_clear(tween_list);
}

void
property_image_tween_append(Evas_Object *control, Eina_Stringshare *image)
{
   Evas_Object *tween_list;

   assert(control != NULL);
   tween_list = elm_layout_content_get(control, NULL);
   elm_gengrid_item_append(tween_list, _itc_tween,
                           eina_stringshare_add(image), NULL, NULL);
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
   evas_object_smart_callback_add(button, signals.elm.button.clicked, _add_tween_image,
                                  item);
   elm_layout_content_set(item, "elm.swallow.add", button);

   BUTTON_ADD(item, button, NULL)
   ICON_STANDARD_ADD(button, icon, true, "minus");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, signals.elm.button.clicked, _del_tween_image,
                                  item);
   elm_layout_content_set(item, "elm.swallow.del", button);

   evas_object_event_callback_add(item,
                                  EVAS_CALLBACK_FREE,
                                  popup_active_helper_close,
                                  (void*)(uintptr_t)POPUP_GENGRID_IMAGE_HELPER);

   evas_object_show(tween_list);

   return item;
}
