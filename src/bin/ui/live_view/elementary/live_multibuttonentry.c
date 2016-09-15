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

#include "live_elementary_widgets.h"

static void
_on_multibutton_swallow_check(void *data __UNUSED__,
                              Evas_Object *obj,
                              void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Elm_Object_Item *multi_item = elm_multibuttonentry_first_item_get(obj);
   Evas_Object *content;

   while (multi_item)
     {
        if (part->change)
          {
             if (part->object)
               {
                  content = elm_object_part_content_unset(multi_item, part->name);
                  evas_object_del(content);
                  part->object = NULL;
               }

             part->object = object_generate(part, multi_item);
             part->change = false;
             elm_object_part_content_set(multi_item, part->name, part->object);
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
        multi_item = elm_multibuttonentry_item_next_get(multi_item);
     }
}

static void
_on_multibutton_text_check(void *data __UNUSED__,
                           Evas_Object *obj,
                           void *ei)
{
   Demo_Part *part = (Demo_Part *)ei;
   Elm_Object_Item *multi_item = elm_multibuttonentry_first_item_get(obj);

   while (multi_item)
     {
        elm_object_item_part_text_set(multi_item, part->name, part->text_content);
        multi_item = elm_multibuttonentry_item_next_get(multi_item);
     }
}

static void
_multibutton_send_signal(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *ei)
{
   Demo_Signal *sig = (Demo_Signal *)ei;
   Elm_Object_Item *item = NULL;

   item = elm_multibuttonentry_first_item_get(obj);
   assert(sig != NULL);
   assert(sig->sig_name != NULL);
   assert(sig->source_name != NULL);

   while (item)
     {
        elm_object_item_signal_emit(item, sig->sig_name, sig->source_name);
        item = elm_multibuttonentry_item_next_get(item);
     }
}

Evas_Object *
widget_multibuttonentry_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_multibuttonentry_add(parent);

   /** guide text is text that being shown in entry when there are no items */
   if (strcmp(group->class, "guidetext") != 0)
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
   if (strcmp(group->class, "btn") == 0)
     {
        evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, _on_multibutton_swallow_check, NULL);
        evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET,    _on_multibutton_text_check, NULL);
        evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, _multibutton_send_signal, NULL);
     }
   else /** on all other cases we need to apply changes to whole object */
     {
        evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
        evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
        evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);
     }

   /** closed button is group used for showing collapsed multibuttonentry only */
   if (strcmp(group->class, "closedbutton") == 0)
     elm_multibuttonentry_expanded_set(object, false);

   elm_object_style_set(object, group->style);
   elm_object_text_set(object, _("Text1"));
   elm_object_part_text_set(object, "guide", _("Tap and enter for adding buttons"));

   /** TODO: 1.) disable SWALLOWs for any group,
             2.) disable live view prop for groups like label, guidetext etc */

   evas_object_resize(object, 100, 100);
   evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(object, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_calculate(object);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
