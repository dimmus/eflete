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

#include "history.h"
#include "change.h"
#include "project_manager.h"

History *
history_add(Group2 *group)
{
   History *history;

   assert(group != NULL);

   history = mem_calloc(1, sizeof(History));
   history->group = group;

   return history;
}

void
history_del(History *history)
{
   Change *change;

   assert(history != NULL);

   EINA_LIST_FREE(history->changes, change)
      change_free(change);
   free(history);
}

void
history_change_add(History *history, Change *change)
{
   Eina_List *undone, *l, *l_n;
   Change *undone_change;

   assert(history != NULL);
   assert(change != NULL);

   undone = (history->current_change != NULL) ?
      eina_list_next(history->current_change) :
      history->changes;
   EINA_LIST_FOREACH_SAFE(undone, l, l_n, undone_change)
     {
        change_free(undone_change);
        history->changes = eina_list_remove_list(history->changes, l);
     }

   history->changes = eina_list_append(history->changes, change);
   history->current_change = eina_list_last(history->changes);

   evas_object_smart_callback_call(ap.win, SIGNAL_HISTORY_CHANGE_ADDED, change);
}

Eina_Bool
history_undo(History *history)
{
   assert(history != NULL);
   assert(history->current_change != NULL);

   if (!change_undo(history->group->edit_object, eina_list_data_get(history->current_change)))
     return false;
   history->current_change = eina_list_prev(history->current_change);

   return true;
}

Eina_Bool
history_undo_all(History *history)
{
   assert(history != NULL);
   assert(history->current_change != NULL);

   while (history->current_change != NULL)
     if (!history_undo(history))
       return false;

   return true;
}

Eina_Bool
history_redo(History *history)
{
   assert(history != NULL);

   history->current_change = (history->current_change != NULL) ?
      eina_list_next(history->current_change) :
      history->changes;

  if (!history->current_change) return true;

   if (!change_redo(history->group->edit_object, eina_list_data_get(history->current_change)))
     return false;

   return true;
}
