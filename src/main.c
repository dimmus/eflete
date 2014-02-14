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

#include "eflete.h"
#include "ui_main_window.h"

EAPI_MAIN int
elm_main()
{

   if (!app_init()) return -1;

   #ifdef HAVE_CONFIG_H
      INFO("%s: %s - Started...", ETE_PACKAGE_NAME, VERSION);
   #else
      CRIT("Could not find 'config.h'");
   #endif

   App_Data *ap = app_create();
   if (!ui_main_window_add(ap))
     {
        app_shutdown();
        return -1;
     }
   evas_object_show(ap->win);

   elm_run();
   elm_shutdown();
   app_shutdown();
   return 0;
}
ELM_MAIN();
