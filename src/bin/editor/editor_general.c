#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "editor.h"
#include "project_manager2.h"
#include "diff.h"
#include "change.h"

int _editor_signals_blocked = 0;
static inline Eina_Bool
_editor_save(Evas_Object *edit_object, Eina_Bool current_group)
{
   assert(edit_object != NULL);

   CRIT_ON_FAIL(edje_edit_without_source_save(edit_object, current_group));

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

   CRIT_ON_FAIL(edje_edit_group_add(edit_object, EFLETE_INTERNAL_GROUP_NAME));
   CRIT_ON_FAIL(edje_edit_without_source_save(edit_object, false));
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
