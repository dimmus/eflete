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

#include "live_view_prop.h"
#include "live_elementary_widgets.h"
#include "widget_macro.h"

static void
_on_frame_swallow_check(void *data,
                        Evas_Object *obj,
                        void *ei __UNUSED__)
{
   Evas_Object *rect = NULL, *frame_obj = NULL, *check = NULL, *ch, *box __UNUSED__;
   Eina_List* frame_list = NULL, *item_list = NULL, *it;
   Eina_List *l = NULL;
   Eina_Bool all_checks = true;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   frame_list = elm_box_children_get(object);
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        EINA_LIST_FOREACH(frame_list, l, frame_obj)
          {
             rect = evas_object_rectangle_add(frame_obj);
             if (!strcmp(part_name, "elm.swallow.content"))
               evas_object_size_hint_min_set(rect, 120, 70);
             evas_object_color_set(rect, HIGHLIGHT_COLOR);
             elm_object_part_content_set(frame_obj, part_name, rect);
          }
        item_list = elm_box_children_get(pd->prop_swallow.swallows);

        EINA_LIST_FOREACH(item_list, it, ch)
          {
             if (elm_check_state_get(ch) == false)
               all_checks = false;
          }
        if (all_checks)
          elm_check_state_set(check, true);
        eina_list_free(item_list);
     }
   else
     {
        EINA_LIST_FOREACH(frame_list, l, frame_obj)
          {
             rect = elm_object_part_content_unset(frame_obj, part_name);
             evas_object_del(rect);
          }
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
   eina_list_free(frame_list);
}

static void
_on_frame_text_check(void *data,
                     Evas_Object *obj,
                     void *ei __UNUSED__)
{
   Evas_Object *frame_obj = NULL, *check = NULL, *ch;
   Eina_List* frame_list = NULL, *item_list = NULL, *it;
   Eina_List *l = NULL;
   Eina_Bool all_checks = true;
   const char *default_text;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   frame_list = elm_box_children_get(object);
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   if (elm_check_state_get(obj))
     {
        frame_obj = eina_list_data_get(frame_list);
        default_text = elm_object_part_text_get(frame_obj, part_name);
        if (default_text)
          eina_hash_add(pd->prop_text.default_text, part_name, eina_stringshare_add(default_text));

        EINA_LIST_FOREACH(frame_list, l, frame_obj)
          elm_object_part_text_set(frame_obj, part_name, _("Text Example"));

        item_list = elm_box_children_get(pd->prop_text.texts);
        EINA_LIST_FOREACH(item_list, it, ch)
          {
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
        EINA_LIST_FOREACH(frame_list, l, frame_obj)
          elm_object_part_text_set(frame_obj, part_name, default_text);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
   eina_list_free(frame_list);
}

static void
_frame_send_signal(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *frame_obj = NULL;

   assert(data != NULL);

   Eina_List* frame_list = elm_box_children_get(data);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   EINA_LIST_FREE(frame_list, frame_obj)
     elm_layout_signal_emit(frame_obj, name, source);
}

Evas_Object *
widget_frame_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object, *frame;

   object = elm_box_add(parent);
   evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   FRAME_ADD(object, frame, true, "");
   elm_box_pack_end(object, frame);
   elm_object_style_set(frame, group->style);
   FRAME_ADD(object, frame, true, "");
   elm_box_pack_end(object, frame);
   elm_object_style_set(frame, group->style);

   evas_object_data_set(object, SWALLOW_FUNC, _on_frame_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, _on_frame_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, _frame_send_signal);

   return object;
}
