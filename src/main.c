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

#include <Ecore_Getopt.h>
#include "eflete.h"
#include "config.h"
#include "ui_main_window.h"

static const Ecore_Getopt options = {
   PACKAGE_NAME,
   "%prog [options]",
   VERSION,
   "(C) 2013-2014 Samsung Electronics.",
   "GNU Library General Public License version 2",
   N_("This application was written for Enlightenment, to use EFL\n"
   "and design to a create and modify a Elementary widgets style.\n"),
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_STR('o', "open", N_("open EDJ file")),
      ECORE_GETOPT_VERSION  ('v', "version"),
      ECORE_GETOPT_COPYRIGHT('c', "copyright"),
      ECORE_GETOPT_LICENSE  ('l', "license"),
      ECORE_GETOPT_HELP     ('h', "help"),
      ECORE_GETOPT_SENTINEL
   }
};

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   char *edj = NULL;
   Eina_Bool info_only = false;
   int args;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR(edj),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_NONE
   };

   if (!app_init()) return -1;

   args = ecore_getopt_parse(&options, values, argc, argv);
   if (args < 0)
     {
        ERR("Could not parse command line options.");
        info_only = true;
     }

   if (!info_only)
     {
#ifdef HAVE_CONFIG_H
        INFO("%s: %s - Started...", PACKAGE_NAME, VERSION);
#else
        CRIT("Could not find 'eflete_config.h'");
#endif

        App_Data *ap = app_create();
        if (!ui_main_window_add(ap))
          {
             app_shutdown();
             return -1;
          }
        evas_object_show(ap->win);

        if (edj)
          {
             if (eina_str_has_suffix(edj, ".edj"))
               {
                  Evas_Object *wd_list = ui_edj_load_done(ap, edj);
                  if (wd_list)
                    add_callbacks_wd(wd_list, ap);
               }
          }
        elm_run();
        elm_shutdown();
        app_shutdown();
     }

   return 0;
}
ELM_MAIN();
