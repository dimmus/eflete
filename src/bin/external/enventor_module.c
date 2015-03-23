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

#include "enventor_module.h"

#ifdef HAVE_ENVENTOR

#define MAX_SCALE 3.0
#define MIN_SCALE 0.5
#define STEP_SCALE 0.1f

static void
_on_enventor_mouse_wheel(void *data __UNUSED__,
                         Evas *evas __UNUSED__,
                         Evas_Object *enventor,
                         void *event_info)
{
   Evas_Event_Mouse_Wheel *event = (Evas_Event_Mouse_Wheel *)event_info ;

   if (!evas_key_modifier_is_set(event->modifiers, "Control"))  return;

   double scale = enventor_object_font_scale_get(enventor);
   scale += event->z >= 0 ? -STEP_SCALE: +STEP_SCALE;

   if ((scale <= MIN_SCALE) || (scale >= MAX_SCALE)) return;

   enventor_object_font_scale_set(enventor, scale);
}


Evas_Object *
enventor_object_init(Evas_Object *parent)
{
  Evas_Object *enventor = NULL;
  enventor = enventor_object_add(parent);
  evas_object_event_callback_add(enventor, EVAS_CALLBACK_MOUSE_WHEEL,
                                 _on_enventor_mouse_wheel, NULL);
  return enventor;
}

#endif /* HAVE_ENVENTOR */
