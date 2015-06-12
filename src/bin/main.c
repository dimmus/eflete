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

#include <Ecore_Getopt.h>
#include "main_window.h"

static const Ecore_Getopt options = {
   PACKAGE_NAME,
   "%prog [options]",
   VERSION,
   "(C) 2013-2014 Samsung Electronics.",
   "GNU Library General Public License version 2",
   N_("This application was written for Enlightenment, to use EFL\n"
   "and design to create and modify Elementary widgets styles.\n"),
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_STR('o', "open", N_("Eflete project file")),
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
   char *open = NULL;
   Eina_Bool info_only = false;
   int args;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR(open),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_NONE
   };

   if (!app_init()) return -1;

#ifdef HAVE_ENVENTOR
   elm_app_compile_data_dir_set(EFLETE_EDJ_PATH);
   elm_app_info_set(NULL, EFLETE_EDJ_PATH, NULL);
   enventor_init(argc, argv);
#endif

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

        App_Data *ap = app_data_get();
        if (!ui_main_window_add(ap))
          {
             app_shutdown();
             return -1;
          }
        evas_object_show(ap->win);

        if (open)
          {
             if ((eina_str_has_suffix(open, ".pro")) &&
                 (ecore_file_exists(open)))
               {
                  ap->project = pm_project_open(open);
                  blocks_show(ap);
                  wm_widgets_list_objects_load(ap->project->widgets,
                                               evas_object_evas_get(ap->win),
                                               ap->project->mmap_file);
                  wm_layouts_list_objects_load(ap->project->layouts,
                                               evas_object_evas_get(ap->win),
                                               ap->project->mmap_file);
                  wm_styles_build_alias(ap->project->widgets,
                                        ap->project->layouts);

                  if (!eina_inlist_count(ap->project->widgets))
                    ui_widget_list_tab_activate(ui_block_widget_list_get(ap), 1);
               }
             else
               {
                  ERR(_("Can not open file '%s'. Wrong path or file format."), open);
                  return 1;
               }
          }
        elm_run();
#ifdef HAVE_ENVENTOR
        enventor_shutdown();
#endif
        app_shutdown();
     }

   return 0;
}
ELM_MAIN();
