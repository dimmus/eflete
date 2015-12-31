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
#include "project_manager.h"
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
   const char *path  = (const char *)data;

   assert(path != NULL);

   pm_project_develop_export(ap.project, path, progress_print, progress_end, NULL);
   return true;
}

static Eina_Bool
_export_dev(void *data __UNUSED__,
            Evas_Object *obj, /* this is fileselector from popup */
            void *event_info)
{
   Eina_List *selected = (Eina_List *)event_info;
   Eina_Strbuf *buf;

   assert(selected != NULL);

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf,
                             _("<font_size=16>A project file '%s' already exist."
                               "Do you want to replace it?</font_size>"),
                             (const char *)eina_list_data_get(selected));
   if (!exist_permission_check(elm_fileselector_path_get(obj),
                               elm_fileselector_current_name_get(obj),
                               _("Export to develop edj-file"),
                               eina_strbuf_string_get(buf)))
     return false;
   eina_strbuf_free(buf);

   ap.splash = splash_add(ap.win, _export_develop_setup, _export_teardown,
                           NULL, (void *)eina_stringshare_add(eina_list_data_get(selected)));
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);

   return true;
}

void
project_export_develop(void)
{
   popup_fileselector_edj_helper("Export to develop edj-file", NULL, NULL, _export_dev, NULL, false, true);
}

static Eina_Bool
_export_source_code_setup(void *data, Splash_Status status __UNUSED__)
{
   Eina_Stringshare *path = (Eina_Stringshare *)data;

   assert(path != NULL);

   pm_project_source_code_export(ap.project,
                                 path,
                                 progress_print,
                                 progress_end,
                                 NULL);
   return true;
}

static Eina_Bool
_export_source_code(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__, /* this is fileselector from popup */
                    void *event_info)
{
   Eina_List *selected = (Eina_List *)event_info;
   Eina_Stringshare *path;
   Eina_Strbuf *buf;

   assert(selected != NULL);

   path = eina_stringshare_add((const char *)eina_list_data_get(selected));
   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf,
                             _("<font_size=16>A project file '%s/%s' already exist."
                               "Do you want to replace it?</font_size>"),
                             path,
                             ap.project->name);
   if (!exist_permission_check(path,
                               ap.project->name,
                               _("Export to develop edj-file"),
                               eina_strbuf_string_get(buf)))
     return false;
   eina_strbuf_free(buf);

   ap.splash = splash_add(ap.win, _export_source_code_setup, _export_teardown, NULL, (void *)path);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);

   return true;
}

void
project_export_edc_project(void)
{
   popup_fileselector_folder_helper("Export source code", NULL, NULL, _export_source_code, NULL, false, false);
}
