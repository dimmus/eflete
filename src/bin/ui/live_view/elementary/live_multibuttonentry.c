/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "live_view_prop.h"

static void
_on_multibutton_swallow_check(void *data,
                              Evas_Object *obj,
                              void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *check = NULL, *item, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   Elm_Object_Item *multi_item = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *object = pd->live_object;
   const char *part_name = evas_object_data_get(obj, PART_NAME);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        multi_item = elm_multibuttonentry_first_item_get(object);
        while (multi_item)
          {
             rect = evas_object_rectangle_add(object);
             evas_object_color_set(rect, RECT_COLOR);
             elm_object_part_content_set(multi_item, part_name, rect);
             multi_item = elm_multibuttonentry_item_next_get(multi_item);
          }
        item_list = elm_box_children_get(pd->prop_swallow.swallows);

        EINA_LIST_FOREACH(item_list, it, item)
          {
             ch = elm_object_part_content_get(item, "info");
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
        eina_list_free(item_list);
     }
   else
     {
        multi_item = elm_multibuttonentry_first_item_get(object);
        while (multi_item)
          {
             rect = elm_object_part_content_unset(multi_item, part_name);
             evas_object_del(rect);
             multi_item = elm_multibuttonentry_item_next_get(multi_item);
          }
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

static void
_on_multibutton_text_check(void *data,
                           Evas_Object *obj,
                           void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *item, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   Elm_Object_Item *multi_item = NULL;
   const char *default_text;

   Prop_Data *pd = (Prop_Data *)data;
   Evas_Object *object = pd->live_object;
   const char *part_name = evas_object_data_get(obj, PART_NAME);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        multi_item = elm_multibuttonentry_first_item_get(object);
        default_text = elm_object_item_part_text_get(multi_item, part_name);
        if (default_text)
          eina_hash_add(pd->prop_text.default_text, part_name, eina_stringshare_add(default_text));
        while (multi_item)
          {
             elm_object_item_part_text_set(multi_item, part_name,
                                 _("Look at it! This is absolutely and totally text"));
             multi_item = elm_multibuttonentry_item_next_get(multi_item);
          }
        item_list = elm_box_children_get(pd->prop_text.texts);

        EINA_LIST_FOREACH(item_list, it, item)
          {
             ch = elm_object_part_content_get(item, "info");
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
        eina_list_free(item_list);
     }
   else
     {
        default_text = eina_hash_find(pd->prop_text.default_text, part_name);
        eina_hash_del(pd->prop_text.default_text, part_name, NULL);
        multi_item = elm_multibuttonentry_first_item_get(object);
        while (multi_item)
          {
             elm_object_item_part_text_set(multi_item, part_name, default_text);
             multi_item = elm_multibuttonentry_item_next_get(multi_item);
          }
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

static void
_multibutton_send_signal(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Elm_Object_Item *item = NULL;
   item = elm_multibuttonentry_first_item_get(data);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   while (item)
     {
        elm_object_item_signal_emit(item, name, source);
        item = elm_multibuttonentry_item_next_get(item);
     }
}

Evas_Object *
widget_multibuttonentry_create(Evas_Object *parent, const Style *style)
{
   Eina_Stringshare *class;
   Eina_Stringshare *style_name;
   standard_widget_name_parse(style->full_group_name, NULL, &class, &style_name);

   Evas_Object *object = elm_multibuttonentry_add(parent);

   /** guide text is text that being shown in entry when there are no items */
   if (strcmp(class, "guidetext") != 0)
     {
        elm_multibuttonentry_item_append(object, _("Eflete"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("Multi-"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("-button-"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("-entry"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("Check"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("this"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("out"), NULL, NULL);
        elm_multibuttonentry_item_append(object, _("!"), NULL, NULL);
     }

   /** in case when we edit button gorup, we need to apply changes to items */
   if (strcmp(class, "btn") == 0)
     {
        evas_object_data_set(object, SWALLOW_FUNC, _on_multibutton_swallow_check);
        evas_object_data_set(object, TEXT_FUNC, _on_multibutton_text_check);
        evas_object_data_set(object, SIGNAL_FUNC, _multibutton_send_signal);
     }
   else /** on all other cases we need to apply changes to whole object */
     {
        evas_object_data_set(object, SWALLOW_FUNC, on_swallow_check);
        evas_object_data_set(object, TEXT_FUNC, on_text_check);
        evas_object_data_set(object, SIGNAL_FUNC, send_signal);
     }

   /** closed button is group used for showing collapsed multibuttonentry only */
   if (strcmp(class, "closedbutton") == 0)
     elm_multibuttonentry_expanded_set(object, false);

   elm_object_style_set(object, style_name);
   elm_object_text_set(object, _("Text1"));
   elm_object_part_text_set(object, "guide", _("Tap and enter for adding buttons"));

   /** TODO: 1.) disable SWALLOWs for any group,
             2.) disable live view prop for groups like label, guidetext etc */

   eina_stringshare_del(class);
   eina_stringshare_del(style_name);

   evas_object_resize(object, 100, 100);
   evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(object, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_calculate(object);

   return object;
}
