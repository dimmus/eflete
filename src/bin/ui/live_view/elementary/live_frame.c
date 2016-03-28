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
#include "widget_macro.h"

static void
_on_frame_swallow_check(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *frame_obj = NULL;
   Evas_Object *content = NULL;

   assert(data != NULL);
   Eina_List *frame_list = elm_box_children_get(data);

   EINA_LIST_FREE(frame_list, frame_obj)
     {
        if (part->change)
          {
             if (part->object)
               {
                  content = elm_object_part_content_unset(frame_obj, part->name);
                  evas_object_del(content);
                  content = NULL;
                  part->object = NULL;
               }

             content = object_generate(part, frame_obj);
             part->change = false;
             part->object = content;
             elm_object_part_content_set(frame_obj, part->name, content);
          }

        if (part->object)
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
_on_frame_text_check(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *frame_obj = NULL;

   assert(data != NULL);
   Eina_List *frame_list = elm_box_children_get(data);

   EINA_LIST_FREE(frame_list, frame_obj)
     elm_object_part_text_set(frame_obj, part->name, part->text_content);
}

static void
_frame_send_signal(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Evas_Object *frame_obj = NULL;

   assert(data != NULL);

   Eina_List* frame_list = elm_box_children_get(data);
   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   EINA_LIST_FREE(frame_list, frame_obj)
     elm_layout_signal_emit(frame_obj, sig->sig_name, sig->source_name);
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

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_frame_swallow_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, _on_frame_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, _frame_send_signal, object);

   return object;
}
