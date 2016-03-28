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
_on_scroller_swallow_check(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Evas_Object *content, *bt;
   Evas_Object *object = (Evas_Object *)data;

   if (!strcmp(part->name, "elm.swallow.content"))
     {
        content = elm_table_add(object);
        int i, j;

        for (j = 0; j < ELEMENTS_COUNT; j++)
          {
             for (i = 0; i < ELEMENTS_COUNT; i++)
               {
                  bt = object_generate(part, content);
                  if (bt)
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
                  else
                    return;
                  elm_table_pack(content, bt, i, j, 1, 1);
                  evas_object_show(bt);
               }
          }
        elm_object_content_set(object, content);
        part->object = content;
        evas_object_show(content);
     }
   else
     {
        bt = object_generate(part, object);
        if (bt)
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
        elm_object_part_content_set(object, part->name, part->object);
     }
}

void
_demo_scroller_del(void *data __UNUSED__,
                   Evas *evas __UNUSED__,
                   Evas_Object *object,
                   void *event_info __UNUSED__)
{
   evas_object_smart_callback_del_full(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_scroller_swallow_check, object);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, object);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_DEMO_SIGNAL_SEND, send_signal, object);
}

Evas_Object *
widget_scroller_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);
   assert(group->class != NULL);

   Evas_Object *object;

   if ((strcmp(group->class, "entry") == 0) || (strcmp(group->class, "entry_single") == 0))
     {
        object = elm_entry_add(parent);
        evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, object);
        elm_entry_scrollable_set(object, true);
        if (strcmp(group->class, "entry_single") == 0)
          elm_entry_single_line_set(object, true);
        evas_object_event_callback_add(object, EVAS_CALLBACK_DEL, demo_object_del, NULL);
     }
   else
     {
        object = elm_scroller_add(parent);
        evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, _on_scroller_swallow_check, object);
        evas_object_event_callback_add(object, EVAS_CALLBACK_DEL, _demo_scroller_del, NULL);
     }
   elm_object_style_set(object, group->style);

   elm_scroller_policy_set(object, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);

   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, object);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, send_signal, object);

   return object;
}
