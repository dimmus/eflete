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

#include "change.h"
#include "diff.h"

Change *
change_add(const char *description)
{
   Change *change;

   change = mem_calloc(1, sizeof(Change));
   change->description = eina_stringshare_add(description);
   change->diffs = NULL;

   return change;
}

void
change_description_set(Change *change, const char *description)
{
   assert(change != NULL);

   eina_stringshare_del(change->description);
   change->description = eina_stringshare_add(description);
}

void
change_free(Change *change)
{
   Diff *diff;

   assert(change != NULL);

   eina_stringshare_del(change->description);

   EINA_LIST_FREE(change->diffs, diff)
      diff_free(diff);
   free(change);
}
void
change_diff_add(Change *change, Diff *diff)
{
   assert(change != NULL);
   assert(diff != NULL);

   change->diffs = eina_list_append(change->diffs, diff);
}

void
change_diff_merge_add(Change *change, Diff *diff)
{
   Diff *existing_diff;
   Eina_List *l;

   assert(change != NULL);
   assert(diff != NULL);

   EINA_LIST_FOREACH(change->diffs, l, existing_diff)
     {
        if (diff->undo.function == existing_diff->undo.function)
          {
             diff_update(existing_diff, diff);
             return;
          }
     }
   change->diffs = eina_list_append(change->diffs, diff);
}

Eina_Bool
change_undo(Evas_Object *edit_object, Change *change)
{
   Eina_List *l;
   Diff *diff;

   assert(edit_object != NULL);
   assert(change != NULL);
   assert(!change->reverted);

   EINA_LIST_REVERSE_FOREACH(change->diffs, l, diff)
      if (!diff_undo(edit_object, diff))
        return false;
   change->reverted = true;
   return true;
}

Eina_Bool
change_redo(Evas_Object *edit_object, Change *change)
{
   Eina_List *l;
   Diff *diff;

   assert(edit_object != NULL);
   assert(change != NULL);
   assert(change->reverted);

   EINA_LIST_FOREACH(change->diffs, l, diff)
      if (!diff_redo(edit_object, diff))
        return false;
   change->reverted = false;
   return true;
}
