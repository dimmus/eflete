/**
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

Evas_Object *
widget_button_create(Evas_Object *parent, const char *class __UNUSED__, const char *style __UNUSED__)
{
   Evas_Object *object = elm_button_add(parent);

   evas_object_data_set(object, SWALLOW_FUNC, &on_swallow_check);
   evas_object_data_set(object, TEXT_FUNC, &on_text_check);
   evas_object_data_set(object, SIGNAL_FUNC, &send_signal);

   return object;
}