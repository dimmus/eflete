/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#include "cursor.h"

#define CURSOR_KEY "eflete_cursor"

Eina_Bool
cursor_main_set(Evas_Object *win, Cursor_Type type __UNUSED__)
{
   if (!win) return false;

   return false;
}

Cursor_Type
cursor_main_get(Evas_Object *win)
{
   if (!win) return CURSOR_UNKNOWN;

   return CURSOR_UNKNOWN;
}

Eina_Bool
cursor_style_set(Evas_Object *obj __UNUSED__, Cursor_Type type __UNUSED__)
{
   return false;
}

Cursor_Type
cursor_style_get(Evas_Object *obj __UNUSED__)
{
   return CURSOR_SYSTEM;
}
