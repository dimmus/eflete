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

static Elm_Bubble_Pos
_bubble_pos_get(const char *class)
{
   Elm_Bubble_Pos position = ELM_BUBBLE_POS_TOP_LEFT;

   assert(class != NULL);

   if (strcmp(class, "top_left") == 0)
     position = ELM_BUBBLE_POS_TOP_LEFT;
   else if (strcmp(class, "top_right") == 0)
     position = ELM_BUBBLE_POS_TOP_RIGHT;
   else if (strcmp(class, "bottom_left") == 0)
     position = ELM_BUBBLE_POS_BOTTOM_LEFT;
   else if (strcmp(class, "bottom_right") == 0)
     position = ELM_BUBBLE_POS_BOTTOM_RIGHT;

   return position;
}

Evas_Object *
widget_bubble_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->class != NULL);
   assert(group->style != NULL);

   Evas_Object *object = elm_bubble_add(parent);
   if (strcmp(group->class, "base") != 0)
     elm_bubble_pos_set(object, _bubble_pos_get(group->class));

   evas_object_smart_callback_add(object, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_TEXT_SET, on_text_check, NULL);
   evas_object_smart_callback_add(object, SIGNAL_DEMO_SIGNAL_SEND, send_signal, NULL);

   elm_object_style_set(object, group->style);
   evas_object_clip_set(object, evas_object_clip_get(parent));

   return object;
}
