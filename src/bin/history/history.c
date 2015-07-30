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

#include "eflete.h"
#include "main_window.h"
#include "history.h"
#include "history_private.h"

static Eina_Bool
_diff_free(Diff *diff)
{
   assert(diff != NULL);

   switch (diff->module_type)
     {
      case PROPERTY:
         _attribute_change_free((Attribute_Diff *)diff);
      break;
      case STATE_TARGET:
         _state_change_free((State_Diff *)diff);
      break;
      case PART_TARGET:
         _part_change_free((Part_Diff *)diff);
      break;
      default:
         ERR("Unsupported module type in diff attributes. This should never happens");
         abort();
     }
   return true;
}

/*
 * All changes that stored below rel_change in the list of changes will be freed.
 */
static Eina_List *
_clear_untracked_changes(Eina_List *changes,
                         Diff *rel_change)
{
   Eina_List *l, *l_prev;
   Diff *diff;

   if (!changes) return NULL;
   if (!rel_change)
     {
        EINA_LIST_FREE(changes, diff)
           {
              _diff_free(diff);
           }
        return NULL;
     }

   EINA_LIST_REVERSE_FOREACH_SAFE(changes, l, l_prev, diff)
     {
        if (rel_change != diff)
          {
             _diff_free(diff);
             changes = eina_list_remove_list(changes, l);
          }
        else break;
     }
   return changes;
}

static void
_changes_index_update(Eina_List *changes)
{
   Eina_List *l = NULL;
   Diff *data = NULL;
   int index = 0;

   EINA_LIST_FOREACH(changes, l, data)
     {
        data->index = index;
        index++;
     }
}

/*
 * This function manage adding new diff into the history of module.
 */
static Eina_Bool
_change_save(Module *module, Diff *change)
{
   Diff *data = NULL;

   assert(module != NULL);

   module->changes = _clear_untracked_changes(module->changes,
                                              module->current_change);

   TODO("Check return logic here")
   if (!change) return false;

   module->changes = eina_list_append(module->changes, change);
   change->index = eina_list_count(module->changes);

   module->current_change = change;

   if ((module->depth > 0) && (eina_list_count(module->changes) > module->depth))
     {
        data = eina_list_data_get(module->changes);
        _diff_free(data);
        module->changes = eina_list_remove_list(module->changes, module->changes);
        _changes_index_update(module->changes);
     }
   _history_ui_item_add(change, module);

   return true;
}

/*
 * This function clear all changes, that was happens with module.
 */
static Eina_Bool
_module_changes_clear(Module *module)
{
   Diff *diff = NULL;
   Eina_Bool result = true;

   assert(module != NULL);

   EINA_LIST_FREE(module->changes, diff)
     {
        result &= _diff_free(diff);
     }

   return result;
}

Eina_Bool
history_redo(Evas_Object *source, int count)
{
   Diff *diff = NULL;
   Eina_List *next = NULL;
   Module *module = NULL;
   Eina_Bool result = false;

   assert(source != NULL);
   assert(count > 0);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);

   assert(module != NULL);

   if (module->changes)
     {
        diff = module->current_change;
        if (!diff) diff = eina_list_data_get(module->changes);
        else
          {
             next = eina_list_next(eina_list_data_find_list(module->changes, diff));
             if (next) diff = eina_list_data_get(next);
             else
               return false;
          }
        if (!diff)
          return false;
     }
   else
     return false;

   switch (diff->module_type)
     {
      case PROPERTY:
         result = _attribute_redo(module->target, (Attribute_Diff *)diff);
      break;
      case STATE_TARGET:
         result = _state_redo(module->target, (State_Diff *)diff);
      break;
      case PART_TARGET:
         result = _part_redo(module->target, (Part_Diff *)diff);
      break;
      default:
         ERR("Unsupported module type, that store diff");
         abort();
     }

   module->current_change = diff;
   _history_ui_item_update(module->current_change, true, true);

   if (count > 1) result = history_redo(source, --count);

   project_changed(true);

   return result;
}

Eina_Bool
history_undo(Evas_Object *source, int count)
{
   Diff *diff = NULL;
   Eina_List *prev = NULL;
   Eina_Bool result = false;
   Module *module = NULL;

   assert(source != NULL);
   assert(count > 0);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);

   assert(module != NULL);

   diff = module->current_change;

   if (!diff) return false;

   switch (diff->module_type)
     {
      case PROPERTY:
         result = _attribute_undo(module->target, (Attribute_Diff *)diff);
      break;
      case STATE_TARGET:
         result = _state_undo(module->target, (State_Diff *)diff);
      break;
      case PART_TARGET:
         result = _part_undo(module->target, (Part_Diff *)diff);
      break;
      default:
         ERR("Unsupported module type: %d", diff->module_type);
         abort();
     }

   prev = eina_list_prev(eina_list_data_find_list(module->changes, diff));
   if (!prev)
     module->current_change = NULL;
   else
     module->current_change = eina_list_data_get(prev);

   _history_ui_item_update(diff, false, false);
   elm_genlist_item_selected_set(diff->ui_item, false);
   if (count > 1) result = history_undo(source, --count);

   if (module->current_change)
     _history_ui_item_update(module->current_change, true, true);
   else
     elm_genlist_item_selected_set(module->ui_item, true);

   project_changed(true);

   return result;
}

int
history_diff_count_get(Evas_Object *source)
{
   int count = 0;
   Module *module;

   assert(source != NULL);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);
   if (module) count = eina_list_count(module->changes);

   return count;
}

Eina_Bool
history_clear(History *history)
{
   Module *module;

   assert(history != NULL);

   EINA_LIST_FREE(history->modules, module)
     {
        if (!_module_changes_clear(module))
          {
             ERR("Didn't cleared history for module %p", module->target);
             abort();
          }
       elm_object_item_del(module->ui_item);
       free(module);
     }
   elm_genlist_clear(history->genlist);
   history->modules = NULL;

   return true;
}

Eina_Bool
history_module_add(Evas_Object *source)
{
   History *history = NULL;
   Module *module = NULL;

   assert(source != NULL);

   history = history_get();

   assert(history != NULL);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);
   if (module)
     {
        _history_ui_list_reload(history, module);
        evas_object_show(history->genlist);
        return true;
     }

   elm_genlist_clear(history->genlist);
   module = (Module *)mem_calloc(1, sizeof(Module));
   module->target = source;
   evas_object_data_set(module->target, HISTORY_MODULE_KEY, module);
   history->modules = eina_list_append(history->modules, module);
   evas_object_show(history->genlist);
   return true;
}

Eina_Bool
history_module_del(Evas_Object *source)
{
   History *history = NULL;
   Module *module = NULL;
   Eina_List *module_list_node = NULL;

   assert(source != NULL);

   history = history_get();

   assert(history != NULL);

   module = evas_object_data_del(source, HISTORY_MODULE_KEY);

   assert(module != NULL);

   if (!_module_changes_clear(module))
     {
        ERR("Didn't cleared history for module %p", module->target);
        abort();
     }
   elm_object_item_del(module->ui_item);
   module_list_node = eina_list_data_find_list(history->modules, module);
   history->modules = eina_list_remove_list(history->modules, module_list_node);
   free(module);

   return true;
}

Evas_Object *
history_genlist_get(History *history, Evas_Object *parent)
{
   assert(history != NULL);

   if (history->genlist) return history->genlist;

   assert(parent != NULL);

   history->genlist = _history_ui_add(parent);

   return history->genlist;
}

History *
history_init(void)
{
   History *history;

   history = (History*)mem_calloc(1, sizeof(History));
   return history;
}

Eina_Bool
history_term(History *history __UNUSED__)
{
   assert(history != NULL);

   history_clear(history);
   free(history);
   return true;
}

Eina_Bool
history_diff_add(Evas_Object *source, Target target, ...)
{
   History *history = NULL;
   Module *module = NULL;
   Diff *change = NULL;
   va_list list;

   assert(source != NULL);

   history = history_get();

   assert(history != NULL);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);

   assert(module != NULL);

   va_start(list, target);

   switch (target)
     {
      case PROPERTY:
         change = _attribute_change_new(list);
         change = _attribute_change_merge((Attribute_Diff *)change, module);
      break;
      case STATE_TARGET:
         change = _state_change_new(list, module->target);
      break;
      case PART_TARGET:
         change = _part_change_new(list, module->target);
      break;
      default:
         ERR("Unsupported target");
         abort();
     }
   va_end(list);
   return _change_save(module, change);
}

Eina_Bool
history_module_depth_set(Evas_Object *source, unsigned int depth)
{
   Module *module = NULL;

   assert(source != NULL);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);

   assert(module != NULL);

   module->depth = depth;
   return true;
}

int
history_module_depth_get(Evas_Object *source)
{
   Module *module = NULL;

   assert(source != NULL);

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);

   assert(module != NULL);

   return (int)module->depth;
}

