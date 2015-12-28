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

int _eflete_lod_dom = -1;

Eina_Bool
logger_init(void)
{
   if (!eina_init()) return false;

#ifdef HAVE_EFLETE_DEBUG
   eina_log_level_set(EINA_LOG_LEVEL_DBG);
#else
   eina_log_level_set(EINA_LOG_LEVEL_ERR);
#endif

   if(_eflete_lod_dom < 0)
     {
        _eflete_lod_dom = eina_log_domain_register(PACKAGE, EINA_COLOR_LIGHTBLUE);
        if(_eflete_lod_dom < 0)
          {
             EINA_LOG_CRIT("Could not register log domain "PACKAGE);
             return false;
          }
     }
   return true;
}

int
logger_shutdown(void)
{
   if(_eflete_lod_dom >= 0)
     {
        eina_log_domain_unregister(_eflete_lod_dom);
        _eflete_lod_dom = -1;
     }
   return _eflete_lod_dom;
}
