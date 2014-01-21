/* Edje Theme Editor
* Copyright (C) 2014 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/
#include "loop_manager.h"

struct _Callback {
   Loop_Exit_Cb func;
   void *data;
};
typedef struct _Callback Callback;

static Eina_List *callbacks = NULL;

void
loop_begin(Loop_Exit_Cb cb_func, void *cb_data)
{
   Callback *cb = mem_malloc(sizeof(Callback));
   cb->func = cb_func;
   cb->data = cb_data;
   callbacks = eina_list_prepend(callbacks, cb);
   DBG("Starting loop");
   ecore_main_loop_begin();
}

Eina_Bool
loop_quit(Eina_Bool force)
{
   Callback *cb = eina_list_data_get(callbacks);
   DBG("Trying to quit loop");
   if ((!cb->func) || (cb->func(force, cb->data)) || (force))
     {
        DBG("Loop quited");
        free(cb);
        callbacks = eina_list_remove_list(callbacks, callbacks);
        ecore_main_loop_quit();
        return true;
     }
   DBG("Loop quit request ignored");
   return false;
}

Eina_Bool
loop_quit_all(Eina_Bool force)
{
   Callback *cb;
   for (cb = eina_list_data_get(callbacks);
       callbacks && ((!cb->func) || (cb->func(force, cb->data)) || (force));
       cb = eina_list_data_get(callbacks))
     {
        free(cb);
        callbacks = eina_list_remove_list(callbacks, callbacks);
        ecore_main_loop_quit();
     }
   return callbacks == NULL;
}
