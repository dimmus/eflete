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
#include "resource_manager2.h"

struct _History {
   Group2 *group;
   Eina_List *changes;
   Eina_List *current_change;
};
typedef struct _History History;

History *
history_add(Group2 *group);

void
history_del(History *history);

void
history_change_add(History *history, Change *change);

Eina_Bool
history_undo(History *history);

Eina_Bool
history_undo_all(History *history);

Eina_Bool
history_redo(History *history);

#endif /* NEW_HISTORY_H */
