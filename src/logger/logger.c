/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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
* along with this program; If not, see .
*/

#include "logger.h"

int _tet_log_dom = -1;

Eina_Bool
logger_init(void)
{
   if (!eina_init())
     {
        return EINA_FALSE;
     }

   /*  */
   eina_log_level_set(EINA_LOG_LEVEL_DBG);

   if(_tet_log_dom < 0)
     {
        _tet_log_dom = eina_log_domain_register("Theme Editor Tool",
                                                EINA_COLOR_LIGHTBLUE);
        if(_tet_log_dom < 0)
          {
             EINA_LOG_CRIT("Could not register log domain 'TET'");
             return EINA_FALSE;
          }
     }

   return EINA_TRUE;
}

void
logger_shutdown(void)
{
   if(_tet_log_dom >= 0)
     {
        eina_log_domain_unregister(_tet_log_dom);
        _tet_log_dom = -1;
     }
}
