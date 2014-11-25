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

/**
 * @struct _State_Diff
 *
 * @brief Structure that store the diff data for states.
 * This structure "inherit" from @c _Diff. It's implement with use field Diff.
 * All fields of this structure provide save diff, that can be undo and
 * redo.
 *
 * @ingroup History
 */
struct _State_Diff
{
   Diff diff; /**<  Instance of main diff structure.*/
   Eina_Stringshare *style;  /**< Full group name */
   Eina_Stringshare *part;  /**< Part name */
   Edje_Part_Type type; /**< Part type. Helper that make search needed union faster */
};

Eina_Bool
_state_redo(Evas_Object *source __UNUSED__, State_Diff *change __UNUSED__)
{
   return false;
}

Eina_Bool
_state_undo(Evas_Object *source __UNUSED__, State_Diff *change __UNUSED__)
{
   return false;
}

void
_state_change_free(State_Diff *change __UNUSED__)
{
   return;
}

Diff *
_state_change_new(va_list list __UNUSED__)
{
   return NULL;
}
