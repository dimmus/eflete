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

#ifndef NEW_HISTORY_H
#define NEW_HISTORY_H
#include "eflete.h"

struct _History_ {
   Group *group;
   Eina_List *changes;
   Eina_List *current_change;
};
typedef struct _History_ History_;

History_ *
history_add(Group *group);

void
history_del(History_ *history);

void
history_change_add(History_ *history, Change *change);

Eina_Bool
history_undo_(History_ *history);

Eina_Bool
history_undo_all(History_ *history);

Eina_Bool
history_redo_(History_ *history);

#endif /* NEW_HISTORY_H */
