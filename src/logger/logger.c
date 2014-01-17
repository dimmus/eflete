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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "logger.h"

int _eflete_lod_dom = -1;

Eina_Bool
logger_init(void)
{
   if (!eina_init()) return EINA_FALSE;

   if (HAVE_EFLETE_DEBUG) eina_log_level_set(EINA_LOG_LEVEL_DBG);

   if(_eflete_lod_dom < 0)
     {
        _eflete_lod_dom = eina_log_domain_register(SH_NAME,
                                                   EINA_COLOR_LIGHTBLUE);
        if(_eflete_lod_dom < 0)
          {
             EINA_LOG_CRIT("Could not register log domain "SH_NAME);
             return EINA_FALSE;
          }
     }

   return EINA_TRUE;
}

void
logger_shutdown(void)
{
   if(_eflete_lod_dom >= 0)
     {
        eina_log_domain_unregister(_eflete_lod_dom);
        _eflete_lod_dom = -1;
     }
}

void
logger_message_print(Eina_Log_Level level, const char *fmt, ...)
{
   va_list args;
   const char *prefix;

   eina_log_console_color_set(stderr, eina_log_level_color_get(level));
   switch (level)
     {
      case EINA_LOG_LEVEL_CRITICAL:
         prefix = "Critical. ";
         break;
      case EINA_LOG_LEVEL_ERR:
         prefix = "Error. ";
         break;
      case EINA_LOG_LEVEL_WARN:
         prefix = "Warning. ";
         break;
      case EINA_LOG_LEVEL_INFO:
      case EINA_LOG_LEVEL_DBG:
         return;
      default:
         prefix = "";
     }
   fprintf(stderr, "%s: %s", SH_NAME, prefix);
   eina_log_console_color_set(stderr, EINA_COLOR_RESET);

   va_start(args, fmt);
   vfprintf(stderr, fmt, args);
   va_end(args);
   putc('\n', stderr);
}
