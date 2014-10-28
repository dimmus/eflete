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
   switch (diff->module_type)
     {
      case PROPERTY:
         _attribute_change_free((Attribute_Diff *)diff);
      break;
      default:
         ERR("Unsupported module type in diff attributes. This should never happens");
         return false;
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

/*
 * This function manage adding new diff into the history of module.
 */
static Eina_Bool
_change_save(Module *module, Diff *change)
{
   if (!change) return false;

   module->changes = _clear_untracked_changes(module->changes,
                                              module->current_change);

   module->changes = eina_list_append(module->changes, change);
   module->current_change = change;

   return true;
}

/*
 * This function clear all changes, that was happens with module.
 */
static Eina_Bool
_module_changes_clear(Module *module)
{
   Eina_List *l, *l_next;
   Diff *diff = NULL;
   Eina_Bool result = true;

   EINA_LIST_FOREACH_SAFE(module->changes, l, l_next, diff)
     {
        result &= _diff_free(diff);
     }

   return result;
}

Eina_Bool
history_redo(Evas_Object *source __UNUSED__, int count __UNUSED__)
{
   return false;
}

Eina_Bool
history_undo(Evas_Object *source, int count)
{
   if ((count <= 0 ) || (!source)) return false;

   Diff *diff = NULL;
   Eina_List *prev = NULL;
   Eina_Bool result = false;
   Module *module = NULL;

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);

   if (module) diff = module->current_change;
   else return false;

   if (!diff) return false;

   switch (diff->module_type)
     {
      case PROPERTY:
         result = _attribute_undo(module->target, (Attribute_Diff *)diff);
      break;
      default:
         ERR("Unsupported module type: %d", diff->module_type);
         return false;
     }

   prev = eina_list_prev(eina_list_data_find_list(module->changes, diff));
   if (!prev) module->current_change = NULL;
   else module->current_change = eina_list_data_get(prev);

   if (count > 1) history_undo(source, --count);

   return result;
}

Eina_Bool
history_clear(History *history)
{
   Eina_List *l, *l_next;
   Module *module;

   if (!history) return false;

   EINA_LIST_FOREACH_SAFE(history->modules, l, l_next, module)
     {
        if (!_module_changes_clear(module))
          {
             ERR("Didn't cleared history for module %p", module->target);
             return false;
          }
       free(module);
     }

   return true;
}

Eina_Bool
history_module_add(Evas_Object *source)
{
   History *history = NULL;
   Module *module = NULL;

   if (!source) return false;
   history = history_get();
   if (!history) return false;

   module = evas_object_data_get(source, HISTORY_MODULE_KEY);
   if (module) return true;

   module = (Module *)mem_calloc(1, sizeof(Module));
   module->target = source;
   evas_object_data_set(module->target, HISTORY_MODULE_KEY, module);
   history->modules = eina_list_append(history->modules, module);

   return true;
}

Eina_Bool
history_module_del(Evas_Object *source)
{
   History *history = NULL;
   Module *module = NULL;
   Eina_List *module_list_node = NULL;

   if (!source) return false;
   history = history_get();
   if (!history) return false;

   module = evas_object_data_del(source, HISTORY_MODULE_KEY);
   if (!module) return false;

   if (!_module_changes_clear(module))
     {
        ERR("Didn't cleared history for module %p", module->target);
        return false;
     }

   module_list_node = eina_list_data_find_list(history->modules, module);
   history->modules = eina_list_remove_list(history->modules, module_list_node);
   free(module);

   return true;
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
   if (!history) return false;

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

   if (!source) return false;
   history = history_get();
   if (!history) return false;
   module = evas_object_data_get(source, HISTORY_MODULE_KEY);
   if (!module) return false;

   va_start(list, target);

   switch (target)
     {
      case PROPERTY:
         change = _attribute_change_new(list);
      break;
      default:
         ERR("Unsupported target");
         va_end(list);
         return false;
     }
   va_end(list);

   return _change_save(module, change);
}


