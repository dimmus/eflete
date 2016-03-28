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

#include "live_elementary_widgets.h"

static const char *item_style_name = NULL;

void
_pop_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = data;

   assert(nf != NULL);

   elm_naviframe_item_pop(nf);
}

void
_next_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data, *bt;
   Eina_Stringshare *find_part = eina_stringshare_add("elm.swallow.prev_btn");

   bt = elm_button_add(nf);
   elm_object_text_set(bt, _("Back"));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt, "clicked", _pop_page_cb, nf);
   evas_object_show(bt);

   elm_naviframe_item_push(nf, _("Page Next"), bt, NULL, NULL, item_style_name);
   eina_stringshare_del(find_part);
}

void
_prev_page_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Evas_Object *nf = (Evas_Object *)data, *bt;
   Eina_Stringshare *find_part = eina_stringshare_add("elm.swallow.next_btn");

   bt = elm_button_add(nf);
   elm_object_text_set(bt, _("Back"));
   evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt, "clicked", _pop_page_cb, nf);
   evas_object_show(bt);

   elm_naviframe_item_push(nf, _("Page Prev"), NULL, bt, NULL, item_style_name);
   eina_stringshare_del(find_part);
}

static void
_on_naviframe_swallow_check(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *ei __UNUSED__)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *) data;
   Elm_Object_Item *item_main, *item_current;

   item_current = evas_object_data_get(object, "main_page");
   item_main = elm_naviframe_top_item_get(object);
   if (item_current != item_main)
     elm_naviframe_item_pop(object);

   if (part->change)
     {
        if (part->object)
          {
             elm_object_part_content_unset(object, part->name);
             evas_object_del(part->object);
             part->object = NULL;
          }

        part->object = object_generate(part, object);
        evas_object_show(part->object);
        if ((part->swallow_content == CONTENT_WIDGET) && (part->widget == WIDGET_BUTTON))
          {
             if (!strcmp(part->name, "elm.swallow.prev_btn"))
               {
                  elm_object_text_set(part->object, _("Prev page"));
                  evas_object_smart_callback_add(part->object, "clicked", _prev_page_cb, object);
               }
             else if (!strcmp(part->name, "elm.swallow.next_btn"))
               {
                  elm_object_text_set(part->object, _("Next page"));
                  evas_object_smart_callback_add(part->object, "clicked", _next_page_cb, object);
               }
          }
        part->change = false;
        elm_object_part_content_set(object, part->name, part->object);
     }

   if (part->object)
     {
        evas_object_color_set(part->object,
                              part->r,
                              part->g,
                              part->b,
                              part->a);

        evas_object_size_hint_min_set(part->object,
                                      part->min_w,
                                      part->min_h);
        evas_object_size_hint_max_set(part->object,
                                      part->max_w,
                                      part->max_h);
     }
}

static void
_on_naviframe_text_check(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *object = (Evas_Object *) data;
   Elm_Object_Item *item_main, *item_current;

   item_current = evas_object_data_get(object, "main_page");
   item_main = elm_naviframe_top_item_get(object);
   if (item_current != item_main)
     elm_naviframe_item_pop(object);

   elm_object_part_text_set(object, part->name, part->text_content);
}

static void
_naviframe_send_signal(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei __UNUSED__)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Evas_Object *object = (Evas_Object *)data;

   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   Elm_Object_Item *item_main, *item_current;

   item_current = evas_object_data_get(object, "main_page");
   item_main = elm_naviframe_top_item_get(object);
   if (item_current != item_main)
     item_current = elm_naviframe_item_pop(object);

   elm_object_item_signal_emit(item_current, sig->sig_name, sig->source_name);
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

   elm_naviframe_prev_btn_auto_pushed_set(nf, false);

   it = elm_naviframe_item_push(nf, _("Page 1"), NULL, NULL, NULL, item_style_name);
   elm_object_item_part_text_set(it, "subtitle", _("Subtitle 1"));
   elm_object_item_part_text_set(it, "title", _("Main Page"));
   evas_object_data_set(nf, "main_page", it);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_naviframe_swallow_check, nf);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, _on_naviframe_text_check, nf);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _naviframe_send_signal, nf);

   elm_object_style_set(nf, style_name);

   return nf;
}
