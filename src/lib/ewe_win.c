/**
 * Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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

#include "ewe_private.h"

EAPI Evas_Object *
ewe_win_add(Evas_Object *parent, const char *name, Elm_Win_Type type)
{
   return elm_win_add(parent, name, type);
}

EAPI void
ewe_win_floating_mode_set(Evas_Object *obj, Eina_Bool floating)
{
   elm_win_floating_mode_set(obj, floating);
}

EAPI Eina_Bool
ewe_win_floating_mode_get(const Evas_Object *obj)
{
   return elm_win_floating_mode_get(obj);
}

EAPI void
ewe_win_render(Evas_Object *obj)
{
   elm_win_render(obj);
}

typedef struct _Ewe_Win_Data Ewe_Win_Data;
struct _Ewe_Win_Data {
};

#include "ewe_win.eo.c"
