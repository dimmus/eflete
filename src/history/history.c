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

/*
 * This function clear all changes, that was happens with module.
 */
static Eina_Bool
_module_changes_clear(Module *module __UNUSED__)
{
   return false;
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
