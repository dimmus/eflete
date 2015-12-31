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

static void
_on_naviframe_swallow_check(void *data __UNUSED__,
                            Evas_Object *obj __UNUSED__,
                            void *ei __UNUSED__)
{
   TODO("Remake on_swallow_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
}

static void
_on_naviframe_text_check(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   TODO("Remake on_text_check, so that would be used everywhere.")
   ERR(N_("Complex widgets are not implemented yet."))
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
