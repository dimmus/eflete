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

#include "history.h"
#include "history_private.h"

struct _Part_Diff
{
   Diff diff;
   Eina_Stringshare *part;
   Eina_List *states;
};

Eina_Bool
_part_redo(Evas_Object *source __UNUSED__, Part_Diff *change __UNUSED__)
{
   return false;
}

Eina_Bool
_part_undo(Evas_Object *source __UNUSED__, Part_Diff *change __UNUSED__)
{
   return false;
}

void
_part_change_free(Part_Diff *change __UNUSED__)
{
   return;
}

Diff *
_part_change_new(va_list list __UNUSED__, Evas_Object *source __UNUSED__)
{
   return NULL;
}
