/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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
#include "tabs.h"
#include "project_manager2.h"
#include "project_common.h"

static Eina_Bool
_export_teardown(void *data, Splash_Status status __UNUSED__)
{
   eina_stringshare_del(data);
   return true;
}

static Eina_Bool
_export_develop_setup(void *data, Splash_Status status __UNUSED__)
{
   char buf[PATH_MAX];
   PM_Project_Result result;
   const char *path  = (const char *)data;
   Eina_List *groups = NULL;

   assert(path != NULL);

#ifdef HAVE_TIZEN
   groups = tabs_open_groups_get();
#endif /* HAVE_TIZEN */
   result = pm_project_develop_export(ap.project, path, groups,
                                  progress_print, progress_end, NULL);
   eina_list_free(groups);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Export develop edj"), NULL, BTN_CANCEL, NULL, NULL);
     return false;
     }
   return true;
}

static void
_after_export_dev_check(void *data)
{
   ap.splash = splash_add(ap.win, _export_develop_setup, _export_teardown, NULL, data);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
   if (!ap.path.export_edj)
     popup_fileselector_helper_dismiss();
}

static Eina_Bool
_export_dev(void *data __UNUSED__,
            Evas_Object *obj, /* this is fileselector from popup */
            void *event_info)
{
   Eina_List *selected = (Eina_List *)event_info;
   Eina_Strbuf *buf;
   const char *path = (const char *)eina_list_data_get(selected);

   assert(selected != NULL);

   if (ecore_file_is_dir(path) || (!eina_str_has_extension(path, ".edj")))
     {
        buf = eina_strbuf_new();
        eina_strbuf_append_printf(buf, _("Please type in actual .edj file"));
        popup_add(_("Export to develop edj-file"), eina_strbuf_string_get(buf), BTN_OK, NULL, NULL);
        eina_strbuf_free(buf);
        return false;
     }

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf,
                             _("<font_size=16>A project file '%s' already exist."
                               "Do you want to replace it?</font_size>"), path);
   exist_permission_check(elm_fileselector_path_get(obj),
                          elm_fileselector_current_name_get(obj),
                          _("Export to develop edj-file"),
                          eina_strbuf_string_get(buf), EINA_TRUE,
                          _after_export_dev_check, (void *)eina_stringshare_add(path));
   eina_strbuf_free(buf);

   return false;
}

void
project_export_develop(void)
{
   Eina_Strbuf *buf;

   if (!ap.path.export_edj)
     popup_fileselector_edj_helper("Export to develop edj-file", NULL, NULL, _export_dev, NULL, false, true);
   else
     {
        Eina_List *l = NULL;
        l = eina_list_append(l, ap.path.export_edj);
        _export_dev(NULL, NULL, l);
        eina_list_free(l);
     }

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s-develop.edj", ap.project->name);
   popup_fileselector_file_set(eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);
}

static Eina_Bool
_export_release_setup(void *data, Splash_Status status __UNUSED__)
{
   char buf[PATH_MAX];
   PM_Project_Result result;
   const char *path  = (const char *)data;

   assert(path != NULL);

   result = pm_project_release_export(ap.project, path,
                                      progress_print, progress_end, NULL);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Export release edj"), NULL, BTN_CANCEL, NULL, NULL);
        return false;
     }
   return true;
}

static void
_after_export_release_check(void *data)
{
   ap.splash = splash_add(ap.win, _export_release_setup, _export_teardown,
                           NULL, data);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
   if (!ap.path.export_edj)
     popup_fileselector_helper_dismiss();
}

static Eina_Bool
_export_release(void *data __UNUSED__,
                Evas_Object *obj, /* this is fileselector from popup */
                void *event_info)
{
   Eina_List *selected = (Eina_List *)event_info;
   Eina_Strbuf *buf;

   const char *path = (const char *)eina_list_data_get(selected);

   assert(selected != NULL);

   if (ecore_file_is_dir(path) || (!eina_str_has_extension(path, ".edj")))
     {
        buf = eina_strbuf_new();
        eina_strbuf_append_printf(buf, _("Please type in actual .edj file"));
        popup_add(_("Export to develop edj-file"), eina_strbuf_string_get(buf), BTN_OK, NULL, NULL);
        eina_strbuf_free(buf);
        return false;
     }

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf,
                             _("<font_size=16>A project file '%s' already exist."
                               "Do you want to replace it?</font_size>"), path);
   exist_permission_check(elm_fileselector_path_get(obj),
                          elm_fileselector_current_name_get(obj),
                          _("Export to release edj-file"),
                          eina_strbuf_string_get(buf), EINA_FALSE,
                          _after_export_release_check, (void *)eina_stringshare_add(path));
   eina_strbuf_free(buf);

   return false;
}

void
project_export_release(void)
{
   Eina_Strbuf *buf;

   if (!ap.path.export_edj)
     popup_fileselector_edj_helper("Export to release edj-file", NULL, NULL, _export_release, NULL, false, true);
   else
     {
        Eina_List *l = NULL;
        l = eina_list_append(l, ap.path.export_edj);
        _export_dev(NULL, NULL, l);
        eina_list_free(l);
     }

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s-release.edj", ap.project->name);
   popup_fileselector_file_set(eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);
}

static Eina_Bool
_export_source_code_setup(void *data, Splash_Status status __UNUSED__)
{
   char buf[PATH_MAX];
   PM_Project_Result result;
   Eina_Stringshare *path = (Eina_Stringshare *)data;

   assert(path != NULL);

   result = pm_project_source_code_export(ap.project,
                                          path,
                                          progress_print,
                                          progress_end,
                                          NULL);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Export surce code"), NULL, BTN_CANCEL, NULL, NULL);
        return false;
     }
   return true;
}

static void
_after_export_source_code_check(void *data)
{
   ap.splash = splash_add(ap.win, _export_source_code_setup, _export_teardown, NULL, data);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
   if (!ap.path.export_edc.path)
     popup_fileselector_helper_dismiss();
}

static Eina_Bool
_export_source_code(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__, /* this is fileselector from popup */
                    void *event_info)
{
   Eina_List *selected = (Eina_List *)event_info;
   Eina_Stringshare *path, *folder;
   Eina_Strbuf *buf;

   assert(selected != NULL);

   path = eina_stringshare_add((const char *)eina_list_data_get(selected));
   buf = eina_strbuf_new();
   if (!ap.path.export_edc.folder)
     folder = ap.project->name;
   else
     folder = ap.path.export_edc.folder;

   eina_strbuf_append_printf(buf,
                             _("<font_size=16>A project file '%s/%s' already exist."
                               "Do you want to replace it?</font_size>"),
                             path,
                             folder);
   exist_permission_check(path,
                          folder,
                          _("Export to develop edj-file"),
                          eina_strbuf_string_get(buf), EINA_FALSE,
                          _after_export_source_code_check, (void *)path);
   eina_strbuf_free(buf);

   return false;
}

void
project_export_edc_project(void)
{
   if (!ap.path.export_edc.path)
     popup_fileselector_folder_helper("Export source code", NULL, NULL, _export_source_code, NULL, false, false);
   else
     {
        Eina_List *l = NULL;
        l = eina_list_append(l, ap.path.export_edc.path);
        _export_source_code(NULL, NULL, l);
        eina_list_free(l);
     }
}

static Eina_Bool
_export_group_source_code_setup(void *data, Splash_Status status __UNUSED__)
{
   Eina_Stringshare *path = (Eina_Stringshare *)data;
   PM_Project_Result result;
   char buf[BUFF_MAX];

   assert(path != NULL);

   result = pm_group_source_code_export(ap.project,
                                    tabs_current_group_get(),
                                    path,
                                    progress_print,
                                    progress_end,
                                    NULL);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Export source edc"), NULL, BTN_CANCEL, NULL, NULL);
     return false;
     }
   return true;
}

static void
_after_group_source_check(void *data)
{
   ap.splash = splash_add(ap.win, _export_group_source_code_setup, _export_teardown, NULL, data);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
   popup_fileselector_helper_dismiss();
}

static Eina_Bool
_export_group_source_code(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__, /* this is fileselector from popup */
                          void *event_info)
{
   Eina_List *selected = (Eina_List *)event_info;
   Eina_Stringshare *path;
   Eina_Strbuf *buf;
   char *name;
   Group2 *group;

   assert(selected != NULL);

   group = tabs_current_group_get();
   name = strdup(group->common.name);
   string_char_replace(name, '/', '_');

   path = eina_stringshare_add((const char *)eina_list_data_get(selected));
   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf,
                             _("<font_size=16>A project file '%s/%s' already exist."
                               "Do you want to replace it?</font_size>"),
                             path, name);
   exist_permission_check(path, name,
                          _("Export group source code"),
                          eina_strbuf_string_get(buf), EINA_FALSE, _after_group_source_check, path);

   eina_strbuf_free(buf);
   free(name);

   return false;
}

void
project_export_edc_group(void)
{
   popup_fileselector_folder_helper("Export group source code", NULL, NULL, _export_group_source_code, NULL, false, false);
}
