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

#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "editor.h"
#include "project_manager.h"
#include "diff.h"
#include "change.h"

int _editor_signals_blocked = 0;
static inline Eina_Bool
_editor_save(Evas_Object *edit_object, Eina_Bool current_group)
{
   assert(edit_object != NULL);

   if (!edje_edit_without_source_save(edit_object, current_group))
     return false;

   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_SAVED, NULL);
   return true;
}
Eina_Bool
editor_save(Evas_Object *edit_object)
{
   return _editor_save(edit_object, true);
}

Eina_Bool
editor_save_all(Evas_Object *edit_object)
{
   return _editor_save(edit_object, false);
}

Eina_Bool
editor_internal_group_add(Evas_Object *edit_object)
{
   assert(edit_object != NULL);

   if (edje_edit_group_exist(edit_object, EFLETE_INTERNAL_GROUP_NAME))
     return true;

   if (!edje_edit_group_add(edit_object, EFLETE_INTERNAL_GROUP_NAME))
     return false;
   if (!edje_edit_without_source_save(edit_object, false))
     return false;
   return true;
}

Eina_Bool
you_shall_not_pass_editor_signals(Change *change)
{
   Diff *diff;

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_SIGNALSBLOCK;
        diff->redo.function = you_shall_not_pass_editor_signals;
        diff->undo.type = FUNCTION_TYPE_NONE;

        change_diff_add(change, diff);
     }
   _editor_signals_blocked++;
   return true;
}

Eina_Bool
you_shall_pass_editor_signals(Change *change)
{
   Diff *diff;

   assert(_editor_signals_blocked > 0);

   if (change)
     {
        diff = mem_calloc(1, sizeof(Diff));
        diff->redo.type = FUNCTION_TYPE_SIGNALSBLOCK;
        diff->redo.function = you_shall_pass_editor_signals;
        diff->undo.type = FUNCTION_TYPE_NONE;

        change_diff_add(change, diff);
     }
   _editor_signals_blocked--;
   return true;
}

void
_editor_project_changed()
{
   /* avoid changes not opened project while we create fake diffs for import
    * groups from custom groups */
   if (ap.project)
     ap.project->changed = true;
   if (!_editor_signals_blocked) evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_CHANGED, NULL);
}
