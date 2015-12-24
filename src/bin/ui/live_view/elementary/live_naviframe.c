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

static const char *item_style_name = NULL;

static void
_pop_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = data;

   assert(nf != NULL);

   elm_naviframe_item_pop(nf);
}

static void
_next_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *nf = pd->live_object, *bt = NULL;
   Eina_Stringshare *find_part = eina_stringshare_add("elm.swallow.prev_btn");

//   if (wm_part_by_name_find(pd->style, find_part))
     {
        bt = elm_button_add(nf);
        elm_object_text_set(bt, _("Back"));
        evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_smart_callback_add(bt, "clicked", _pop_page_cb, nf);
        evas_object_show(bt);
     }

   elm_naviframe_item_push(nf, _("Page Next"), bt, NULL, NULL, item_style_name);
   eina_stringshare_del(find_part);
}

static void
_prev_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *nf = pd->live_object, *bt = NULL;
   Eina_Stringshare *find_part = eina_stringshare_add("elm.swallow.next_btn");

//   if (wm_part_by_name_find(pd->style, find_part))
     {
        bt = elm_button_add(nf);
        elm_object_text_set(bt, _("Back"));
        evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_smart_callback_add(bt, "clicked", _pop_page_cb, nf);
        evas_object_show(bt);
     }

   elm_naviframe_item_push(nf, _("Page Prev"), NULL, bt, NULL, item_style_name);
   eina_stringshare_del(find_part);
}

static void
_on_naviframe_swallow_check(void *data,
                            Evas_Object *obj,
                            void *ei __UNUSED__)
{
   Evas_Object *content = NULL, *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;

   Evas_Object *bt;
   Elm_Object_Item *item_main, *item_current;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");

   item_current = evas_object_data_get(object, "main_page");
   item_main = elm_naviframe_top_item_get(object);
   if (item_current != item_main)
     elm_naviframe_item_pop(object);

   if (elm_check_state_get(obj))
     {
        if (!strcmp(part_name, "elm.swallow.prev_btn"))
          {
             bt = elm_button_add(object);
             elm_object_text_set(bt, _("Prev page"));
             evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_smart_callback_add(bt, "clicked", _prev_page_cb, pd);
             evas_object_show(bt);

             elm_object_part_content_set(object, part_name, bt);
          }
        else if (!strcmp(part_name, "elm.swallow.next_btn"))
          {
             bt = elm_button_add(object);
             elm_object_text_set(bt, _("Next page"));
             evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_smart_callback_add(bt, "clicked", _next_page_cb, pd);
             evas_object_show(bt);

             elm_object_part_content_set(object, part_name, bt);
          }
        else
          {
             content = evas_object_rectangle_add(object);
             evas_object_color_set(content, HIGHLIGHT_COLOR);
             elm_object_part_content_set(object, part_name, content);
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
        content = elm_object_part_content_unset(object, part_name);
        evas_object_del(content);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

static void
_on_naviframe_text_check(void *data,
                         Evas_Object *obj,
                         void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *ch;
   Eina_List *item_list = NULL, *it;
   Eina_Bool all_checks = true;
   const char *default_text;

   Elm_Object_Item *item_main, *item_current;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   Evas_Object *object = pd->live_object;
   const char *part_name = elm_object_part_text_get(obj, NULL);
   check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");

   item_current = evas_object_data_get(object, "main_page");
   item_main = elm_naviframe_top_item_get(object);
   if (item_current != item_main)
     elm_naviframe_item_pop(object);

   if (elm_check_state_get(obj))
     {
        default_text = elm_object_part_text_get(object, part_name);
        if (default_text)
          eina_hash_add(pd->prop_text.default_text, part_name,
                        eina_stringshare_add(default_text));
        elm_object_part_text_set(object, part_name,
                                 _("Look at it! This is absolutely and totally text"));
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
        elm_object_part_text_set(object, part_name, default_text);
        if (elm_check_state_get(check)) elm_check_state_set(check, false);
     }
}

static void
_naviframe_send_signal(void *data,
                       Evas_Object *obj,
                       void *ei __UNUSED__)
{
   Evas_Object *object = (Evas_Object *)data;

   assert(object != NULL);

   const char *name = evas_object_data_get(obj, SIGNAL_NAME);
   const char *source = evas_object_data_get(obj, SIGNAL_SOURCE);

   assert(name != NULL);
   assert(source != NULL);

   Elm_Object_Item *item_main, *item_current;

   item_current = evas_object_data_get(object, "main_page");
   item_main = elm_naviframe_top_item_get(object);
   if (item_current != item_main)
     item_current = elm_naviframe_item_pop(object);

   elm_object_item_signal_emit(item_current, name, source);
}


Evas_Object *
widget_naviframe_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Eina_Stringshare *style_name = group->style;
   char **style_parsed = NULL;
   unsigned int count_split = 0;
   Elm_Object_Item *it = NULL;

   style_parsed = eina_str_split_full(style_name, "/", 2, &count_split);
   if (count_split == 2)
     {
        item_style_name = eina_stringshare_add(style_parsed[0]);
        eina_stringshare_replace(&style_name, style_parsed[1]);
     }
   free(style_parsed[0]);
   free(style_parsed);

   Evas_Object *nf = elm_naviframe_add(parent);
   evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(parent, nf);

   elm_naviframe_prev_btn_auto_pushed_set(nf, false);

   it = elm_naviframe_item_push(nf, _("Page 1"), NULL, NULL, NULL, item_style_name);
   elm_object_item_part_text_set(it, "subtitle", _("Subtitle 1"));
   elm_object_item_part_text_set(it, "title", _("Main Page"));
   evas_object_data_set(nf, "main_page", it);

   evas_object_data_set(nf, SWALLOW_FUNC, _on_naviframe_swallow_check);
   evas_object_data_set(nf, TEXT_FUNC, _on_naviframe_text_check);
   evas_object_data_set(nf, SIGNAL_FUNC, _naviframe_send_signal);

   elm_object_style_set(nf, style_name);

   return nf;
}
