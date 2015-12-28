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

#ifndef CHANGE_H
#define CHANGE_H
#include "eflete.h"

struct _Change {
   Eina_Stringshare *description;
   Eina_List *diffs;
   Eina_Bool reverted : 1;
};

Change *
change_add(const char *description);

void
change_description_set(Change *change, const char *description);

void
change_free(Change *change);

void
change_diff_add(Change *change, Diff *diff);

void
change_diff_merge_add(Change *change, Diff *diff);

Eina_Bool
change_undo(Evas_Object *edit_object, Change *change);

Eina_Bool
change_redo(Evas_Object *edit_object, Change *change);
#endif /* CHANGE_H */
