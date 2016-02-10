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
#include <regex.h>
#include "main_window.h"
#include "project_navigator.h"
#include "project_manager.h"
#include "project_common.h"
#include "tabs.h"
/* it's really bad idia, but need haven't time to make it correctly */
#include "tabs_private.h"
#include "config.h"

static char *open = NULL;
static char *import_edj = NULL;
static char *pro_name = NULL;
static char *pro_path = NULL;
static Eina_Bool pro_replace = false;

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
      ECORE_GETOPT_STORE_STR(0, "import-edj", N_("Import the edj file as new project")),
      ECORE_GETOPT_STORE_STR(0, "name", N_("Name for new project that would be created in import process")),
      ECORE_GETOPT_STORE_STR(0, "path", N_("Path for project")),
      ECORE_GETOPT_STORE_TRUE(0, "replace", N_("Replace existing project")),
      ECORE_GETOPT_STORE_TRUE('r', "reopen", "reopen last project"),
      ECORE_GETOPT_VERSION  ('v', "version"),
      ECORE_GETOPT_COPYRIGHT('c', "copyright"),
      ECORE_GETOPT_LICENSE  ('l', "license"),
      ECORE_GETOPT_HELP     ('h', "help"),
      ECORE_GETOPT_SENTINEL
   }
};

static Eina_Bool
_setup_open_splash(void *data, Splash_Status status __UNUSED__)
{
   Eina_Stringshare *path = data;

   assert(path != NULL);

   pm_project_open(path, progress_print, _tabs_progress_end, NULL);
   eina_stringshare_del(path);

   return true;
}

static Eina_Bool
_teardown_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_free();
   return true;
}

static Eina_Bool
_cancel_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_cancel();
   return true;
}

static void
_open_project(void *data __UNUSED__)
{
   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          (void *)eina_stringshare_add(open));
   evas_object_show(ap.splash);
}

static void
_import_edj(void *data __UNUSED__)
{
   tabs_menu_import_edj_data_set(pro_name, pro_path, import_edj);
   tabs_menu_tab_open(TAB_HOME_IMPORT_EDJ);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eina_Bool info_only = false, reopen = false;
   Config *config;
   Recent *r;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR(open),
     ECORE_GETOPT_VALUE_STR(import_edj),
     ECORE_GETOPT_VALUE_STR(pro_name),
     ECORE_GETOPT_VALUE_STR(pro_path),
     ECORE_GETOPT_VALUE_BOOL(pro_replace),
     ECORE_GETOPT_VALUE_BOOL(reopen),
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

   ecore_getopt_parse(&options, values, argc, argv);
   if (!info_only)
     {
#ifdef HAVE_CONFIG_H
        INFO("%s: %s - Started...", PACKAGE_NAME, VERSION);
#else
        CRIT("Could not find 'eflete_config.h'");
#endif

        if (!ui_main_window_add())
          {
             app_shutdown();
             return -1;
          }

        if (reopen)
          {
             config = config_get();
             if (!config->recents)
               {
                  ERR(_("There are no previously opened projects yet."));
                  return 1;
               }
             r = eina_list_data_get(config->recents);
             open = r->path;
             ecore_job_add(_open_project, NULL);
             goto run;
          }
        if (open)
          {
             if ((eina_str_has_suffix(open, ".pro")) &&
                 (ecore_file_exists(open)))
               {
                  ecore_job_add(_open_project, NULL);
                  goto run;
               }
             else
               {
                  ERR(_("Can not open file '%s'. Wrong path or file format."), open);
                  return 1;
               }
          }
        if (import_edj)
          {
            ecore_job_add(_import_edj, NULL);
            goto run;
          }

run:
        evas_object_show(ap.win);
        elm_run();
#ifdef HAVE_ENVENTOR
        enventor_shutdown();
#endif
        app_shutdown();
     }

   return 0;
}
ELM_MAIN();
