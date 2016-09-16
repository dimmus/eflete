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
_on_radio_swallow_check(void *data __UNUSED__,
                        Evas_Object *obj,
                        void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Eina_List* radio_list = elm_box_children_get(obj);
   Evas_Object *content, *radio_obj;

   int content_type = part->swallow_content;

   EINA_LIST_FREE(radio_list, radio_obj)
     {
        content = elm_object_part_content_unset(radio_obj, part->name);
        if (part->change)
          {
             part->objects = eina_list_remove(part->objects, content);
             evas_object_del(content);
             content = NULL;

             /* if NONE - delete object */
             if (content_type != CONTENT_NONE)
               {
                  content = object_generate(part, obj);
                  part->objects = eina_list_append(part->objects, content);
                  elm_object_part_content_set(radio_obj, part->name, content);
               }
          }

        if (content)
          {
             evas_object_color_set(content,
                                   part->r,
                                   part->g,
                                   part->b,
                                   part->a);

             evas_object_size_hint_min_set(content,
                                           part->min_w,
                                           part->min_h);
             evas_object_size_hint_max_set(content,
                                           part->max_w,
                                           part->max_h);
          }
     }
}

static void
_on_radio_text_check(void *data __UNUSED__,
                     Evas_Object *obj,
                     void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Eina_List* radio_list = elm_box_children_get(obj);
   Evas_Object *radio_obj;

   EINA_LIST_FREE(radio_list, radio_obj)
     elm_object_part_text_set(radio_obj, part->name, part->text_content);
}

static void
_radio_send_signal(void *data __UNUSED__,
                   Evas_Object *obj,
                   void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Eina_List* radio_list = elm_box_children_get(obj);
   Evas_Object *radio_obj = NULL;

   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   EINA_LIST_FREE(radio_list, radio_obj)
     elm_layout_signal_emit(radio_obj, sig->sig_name, sig->source_name);
}

Evas_Object *
widget_radio_create(Evas_Object *parent, const Group2 *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *object, *rd, *rdg;

   object = elm_box_add(parent);
   evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   RADIO_ADD(parent, rd, 1, NULL);
   elm_box_pack_end(object, rd);
   rdg = rd;
   elm_object_style_set(rd, group->style);
   RADIO_ADD(parent, rd, 2, NULL);
   elm_radio_group_add(rd, rdg);
   elm_box_pack_end(object, rd);
   elm_object_style_set(rd, group->style);
   RADIO_ADD(parent, rd, 3, NULL);
   elm_radio_group_add(rd, rdg);
   elm_box_pack_end(object, rd);
   elm_object_style_set(rd, group->style);

   elm_radio_value_set(rdg, 2);

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, _on_radio_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, _on_radio_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, _radio_send_signal, NULL);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
