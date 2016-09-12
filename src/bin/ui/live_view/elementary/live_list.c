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

static void
_on_list_swallow_check(void *data __UNUSED__,
                       Evas_Object *obj,
                       void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;

   Elm_Object_Item *item = elm_list_first_item_get(obj);

   const char *part_name = part->name;

   /* because elm_list is weird */
   if (!strcmp(part_name, "elm.swallow.icon"))
     part_name = "start";
   else if (!strcmp(part_name, "elm.swallow.end"))
     part_name = "end";

   while (item)
     {
        part->object = elm_object_item_part_content_unset(item, part_name);
        if (part->object)
          {
             evas_object_del(part->object);
             part->object = NULL;
          }

        if (part->swallow_content != CONTENT_NONE)
          part->object = object_generate(part, obj);

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

        elm_object_item_part_content_set(item, part_name, part->object);
        item = elm_list_item_next(item);
     }
}

static void
_on_list_text_check(void *data __UNUSED__,
                    Evas_Object *obj,
                    void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;

   Elm_Object_Item *item = elm_list_first_item_get(obj);

   while (item)
     {
        elm_object_item_part_text_set(item, part->name, part->text_content);
        item = elm_list_item_next(item);
     }
}

static void
_list_send_signal(void *data __UNUSED__,
                  Evas_Object *obj,
                  void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Elm_Object_Item *item = NULL;

   item = elm_list_first_item_get(obj);
   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   while (item)
     {
        elm_object_item_signal_emit(item, sig->sig_name, sig->source_name);
        item = elm_list_item_next(item);
     }
}

Evas_Object *
widget_list_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   int i = 0;
   Evas_Object *object = elm_list_add(parent);

   for (i = 0; i < ELEMENTS_BIG_COUNT; i++)
     elm_list_item_append(object, _("No icons"), NULL, NULL, NULL, NULL);

   if (strstr(group->class, "h_") == group->class)
     elm_list_horizontal_set(object, true);

   if (strstr(group->class, "compress"))
     elm_list_mode_set(object, ELM_LIST_COMPRESS);
   else
     elm_list_mode_set(object, ELM_LIST_SCROLL);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, _on_list_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, _on_list_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, _list_send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
