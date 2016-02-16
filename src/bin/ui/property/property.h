/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#ifndef PROPERTY_H
#define PROPERTY_H

#include "eflete.h"

typedef enum {
   PROPERTY_MODE_NONE,
   PROPERTY_MODE_GROUP,
   PROPERTY_MODE_COLOR_CLASS,
   PROPERTY_MODE_STYLE,
   PROPERTY_MODE_SOUND,
   PROPERTY_MODE_IMAGE,
   PROPERTY_MODE_DEMO
} Property_Mode;

Evas_Object *
property_add(Evas_Object *parent);

void
property_mode_set(Property_Mode mode);

#endif /* PROPERTY_H */
