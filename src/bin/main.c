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

#include "main_window.h"
#include <Ecore_Getopt.h>
#include <regex.h>
#include "project_navigator.h"
#include "project_manager2.h"
#include "project_common.h"
#include "tabs.h"
/* it's really bad idia, but need haven't time to make it correctly */
#include "tabs_private.h"
#include "config.h"

static char *file = NULL;
static char *pro_name = NULL;
static char *pro_path = NULL;
static char *export_edj = NULL;
static char *export_edc = NULL;
static Eina_List *img_dirs = NULL;
static Eina_List *snd_dirs = NULL;
static Eina_List *fnt_dirs = NULL;
static Eina_List *data_dirs = NULL;
static Eina_List *widgets = NULL;

#define _ERR_EXIT(MSG, ...) \
do { \
   printf(_("ERROR: ")); \
   printf(MSG, ## __VA_ARGS__); \
   printf(_("\nERROR: invalid options found. See --help.\n")); \
   return 1; \
} while (0);

static const Ecore_Getopt options = {
   PACKAGE_NAME,
   "%prog [OPTION]... [FILE]\n"
   "  if FILE is *.pro: open project\n"
   "  if FILE is *.edj: import edj\n"
   "  if FILE is not specified but --name given: new project\n"
   ,
   VERSION,
   "(C) 2013-2016 Samsung Electronics.",
   "GNU Library General Public License version 2",
   "This application was written for Enlightenment, to use EFL\n"
   "and design to create and modify Elementary widgets styles.\n",
   EINA_TRUE,
   {
      ECORE_GETOPT_STORE_STR(0, "name", N_("Name for new project")),
      ECORE_GETOPT_STORE_STR(0, "path", N_("Path to project directory")),
      ECORE_GETOPT_STORE_STR(0, "export-edj", N_("Export edj file path")),
      ECORE_GETOPT_STORE_STR(0, "export-edc", N_("Export edc file path. This should include the path, folder and edc file name.(PATH/FOLDER/FILE)")),
      ECORE_GETOPT_APPEND_METAVAR('i', "id", "Add image directory for edc compilation", "DIR_NAME", ECORE_GETOPT_TYPE_STR),
      ECORE_GETOPT_APPEND_METAVAR('s', "sd", "Add sound directory for edc compilation", "DIR_NAME", ECORE_GETOPT_TYPE_STR),
      ECORE_GETOPT_APPEND_METAVAR('f', "fd", "Add font directory for edc compilation", "DIR_NAME", ECORE_GETOPT_TYPE_STR),
      ECORE_GETOPT_APPEND_METAVAR('d', "dd", "Add data directory for edc compilation", "DIR_NAME", ECORE_GETOPT_TYPE_STR),
      ECORE_GETOPT_APPEND_METAVAR('w', "widget", "Add widget to new project or import edj-file. Add its styles if the style names are added. Copy its styles if the copy style names are added. **rule = WIDGET:STYLE[COPY_STYLE,..]{ITEM_STYLE,..[COPY_ITEM_STYLE,..]},..", "(Follow the below rule)", ECORE_GETOPT_TYPE_STR),
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
   Eina_Bool ret = true;
   Eina_Stringshare *path = data;
   PM_Project_Result result;
   char buf[BUFF_MAX];

   assert(path != NULL);

   result = pm_project_open(path, progress_print, _tabs_progress_end, NULL);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Open project"), NULL, BTN_CANCEL, NULL, NULL);
        ret = false;
     }
   eina_stringshare_del(path);

   return ret;
}

static Eina_Bool
_teardown_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   return true;
}

__UNUSED__ static Eina_Bool
_cancel_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   //pm_project_thread_cancel();
   return true;
}

static void
_open_project(void *data __UNUSED__)
{
   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          NULL,
                          (void *)eina_stringshare_add(file));
   evas_object_show(ap.splash);
}

static void
_import_edj(void *data __UNUSED__)
{
   const char *name;
   Eina_Tmpstr *proj_name;
   if (pro_name)
     {
        tabs_menu_import_edj_data_set(pro_name, pro_path, file, widgets);
     }
   else
     {
        name = ecore_file_file_get(file);
        proj_name = eina_tmpstr_add_length(name, strlen(name) - 4);
        tabs_menu_import_edj_data_set(proj_name, pro_path, file, widgets);
        eina_tmpstr_del(proj_name);
     }
   tabs_home_tab_add(TAB_HOME_IMPORT_EDJ);
}

static void
_import_edc(void *data __UNUSED__)
{
   const char *name;
   Eina_Tmpstr *proj_name;
   if (pro_name)
     {
        tabs_menu_import_edc_data_set(pro_name, pro_path, file, img_dirs, snd_dirs, fnt_dirs, data_dirs);
     }
   else
     {
        name = ecore_file_file_get(file);
        proj_name = eina_tmpstr_add_length(name, strlen(name) - 4);
        tabs_menu_import_edc_data_set(proj_name, pro_path, file, img_dirs, snd_dirs, fnt_dirs, data_dirs);
        eina_tmpstr_del(proj_name);
     }
   tabs_home_tab_add(TAB_HOME_IMPORT_EDC);
}

static void
_new_project(void *data __UNUSED__)
{
   const char *name;
   Eina_Tmpstr *proj_name;
   if (pro_name)
     {
        tabs_menu_new_data_set(pro_name, pro_path, widgets);
     }
   else
     {
        name = ecore_file_file_get(file);
        proj_name = eina_tmpstr_add_length(name, strlen(name) - 4);
        tabs_menu_new_data_set(proj_name, pro_path, widgets);
        eina_tmpstr_del(proj_name);
     }
   tabs_home_tab_add(TAB_HOME_NEW_PROJECT);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eina_Bool info_only = false, reopen = false;
   Config *config;
   Recent *r;
   int pos;

   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR(pro_name),
     ECORE_GETOPT_VALUE_STR(pro_path),
     ECORE_GETOPT_VALUE_STR(export_edj),
     ECORE_GETOPT_VALUE_STR(export_edc),
     ECORE_GETOPT_VALUE_LIST(img_dirs),
     ECORE_GETOPT_VALUE_LIST(snd_dirs),
     ECORE_GETOPT_VALUE_LIST(fnt_dirs),
     ECORE_GETOPT_VALUE_LIST(data_dirs),
     ECORE_GETOPT_VALUE_LIST(widgets),
     ECORE_GETOPT_VALUE_BOOL(reopen),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_BOOL(info_only),
     ECORE_GETOPT_VALUE_NONE
   };

   if (!app_init()) return -1;

#ifdef HAVE_ENVENTOR
   enventor_init(argc, argv);
#endif

   pos = ecore_getopt_parse(&options, values, argc, argv);
   if (pos < 0)
     return 1;
   if (pos < argc - 1)
     _ERR_EXIT(_("Only one file should be specified."));

   if (pos == argc -1)
     file = argv[pos];

   if (!info_only)
     {
#ifdef HAVE_CONFIG_H
        INFO("%s: %s - Started...", PACKAGE_NAME, VERSION);
#else
        CRIT("Could not find 'eflete_config.h'");
#endif

        config_load();
        if (!ui_main_window_add())
          {
             app_shutdown();
             return -1;
          }

        if (reopen)
          {
             if (file)
               _ERR_EXIT(_("--reopen is given but file specified."));
             if (pro_name)
               _ERR_EXIT(_("--reopen is given but --name specified."));
             if (pro_path)
               _ERR_EXIT(_("--repoen is given but --path specified."));
             if (img_dirs)
               _ERR_EXIT(_("--reopen is given but --id specified."));
             if (snd_dirs)
               _ERR_EXIT(_("--reopen is given but --sd specified."));
             if (fnt_dirs)
               _ERR_EXIT(_("--reopen is given but --fd specified."));
             if (data_dirs)
               _ERR_EXIT(_("--reopen is given but --dd specified."));
             if (widgets)
               _ERR_EXIT(_("widgets can be added only to new project."));

             config = config_get();
             if (!config->recents)
               _ERR_EXIT(_("There are no previously opened projects yet."));

             r = eina_list_data_get(config->recents);
             file = r->path;
             ecore_job_add(_open_project, NULL);
             goto run;
          }
        if (file)
          {
             if (!ecore_file_exists(file))
               _ERR_EXIT(_("File '%s' doesn't exists."), file);
             if (ecore_file_is_dir(file))
               _ERR_EXIT(_("'%s' is a directory."), file);

             if (eina_str_has_suffix(file, ".pro"))
               {
                  if (pro_name)
                    _ERR_EXIT(_("*.pro file is given but --name specified."));
                  if (pro_path)
                    _ERR_EXIT(_("*.pro file is given but --path specified."));
                  if (img_dirs)
                    _ERR_EXIT(_("*.pro file is given but --id specified."));
                  if (snd_dirs)
                    _ERR_EXIT(_("*.pro file is given but --sd specified."));
                  if (fnt_dirs)
                    _ERR_EXIT(_("*.pro file is given but --fd specified."));
                  if (data_dirs)
                    _ERR_EXIT(_("*.pro file is given but --dd specified."));
                  if (widgets)
                    _ERR_EXIT(_("widgets can be added only to new project."));

                  ecore_job_add(_open_project, NULL);
                  goto run;
               }
             else if (eina_str_has_suffix(file, ".edj"))
               {
                  if (img_dirs)
                    _ERR_EXIT(_("*.edj file is given but --id specified."));
                  if (snd_dirs)
                    _ERR_EXIT(_("*.edj file is given but --sd specified."));
                  if (fnt_dirs)
                    _ERR_EXIT(_("*.edj file is given but --fd specified."));
                  if (data_dirs)
                    _ERR_EXIT(_("*.edj file is given but --dd specified."));

                  ecore_job_add(_import_edj, NULL);
                  goto run;
               }
             else if (eina_str_has_suffix(file, ".edc"))
               {
                  ecore_job_add(_import_edc, NULL);
                  goto run;
               }
             else
               _ERR_EXIT(_("Wrong file extension."));
          }
        else
          {
             if (!pro_name && pro_path)
               _ERR_EXIT(_("no file or --name are given but --path specified."));
             if (img_dirs)
               _ERR_EXIT(_("no file is given but --id specified."));
             if (snd_dirs)
               _ERR_EXIT(_("no file is given but --sd specified."));
             if (fnt_dirs)
               _ERR_EXIT(_("no file is given but --fd specified."));
             if (data_dirs)
               _ERR_EXIT(_("no file is given but --dd specified."));

             if (pro_name)
               {
                  ecore_job_add(_new_project, NULL);
                  goto run;
               }
             else if (widgets)
               _ERR_EXIT(_("widgets can be added only to new project."));
          }

run:
        ap.path.export_edj = export_edj;
        ap.path.export_edc = export_edc;
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
