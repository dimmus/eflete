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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "tabs_private.h"
#include "tabs.h"
#include "main_window.h"

struct _Tab_Home_Edj
{
   Evas_Object *layout;
   Evas_Object *btn_create;
   Elm_Validator_Regexp *name_validator;

   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edj;
   Evas_Object *meta_version;
   Evas_Object *meta_authors;
   Evas_Object *meta_licenses;
   Evas_Object *meta_comment;
   Evas_Object *popup_fs;
   Evas_Object *fs;
};

typedef struct _Tab_Home_Edj Tab_Home_Edj;
Tab_Home_Edj tab_edj;

static Eina_Bool
_setup_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_import_edj(elm_entry_entry_get(tab_edj.name),
                         elm_entry_entry_get(tab_edj.path),
                         elm_entry_entry_get(tab_edj.edj),
                         progress_print,
                         progress_end,
                         NULL);

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
_import(void *data __UNUSED__,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   if (ap.project)
     if (!project_close())
       return;

   exist_permission_check(elm_entry_entry_get(tab_edj.path),
                          elm_entry_entry_get(tab_edj.name),
                          _("Import edj-file"));

   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          NULL);
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

static void
_validate(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   if ((elm_validator_regexp_status_get(tab_edj.name_validator) != ELM_REG_NOERROR) ||
       !eina_str_has_extension(elm_entry_entry_get(tab_edj.edj), ".edj"))
     elm_object_disabled_set(tab_edj.btn_create, true);
   else
     elm_object_disabled_set(tab_edj.btn_create, false);
}

static void
_elipsis(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   popup_fileselector_folder_helper(tab_edj.path, elm_entry_entry_get(tab_edj.path));
}

static void
_elipsis_edj(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   popup_fileselector_edj_helper(tab_edj.edj, NULL);
}

Evas_Object *
_tab_import_edj_add(void)
{
   tab_edj.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   tab_edj.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab_edj.layout, "layout", "tab_home", "import_edj");

   BUTTON_ADD(tab_edj.layout, tab_edj.btn_create, _("Create"))
   elm_object_part_content_set(tab_edj.layout, "elm.swallow.btn_create", tab_edj.btn_create);
   evas_object_smart_callback_add(tab_edj.btn_create, "clicked", _import, NULL);
   elm_object_disabled_set(tab_edj.btn_create, true);

   /* label.name */
   elm_object_part_text_set(tab_edj.layout, "label.name", _("Project name:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.name, true)
   eo_do(tab_edj.name, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, tab_edj.name_validator));
   evas_object_smart_callback_add(tab_edj.name, "changed", _validate, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.name", tab_edj.name);
   /* label.path */
   elm_object_part_text_set(tab_edj.layout, "label.path", _("Path to project:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.path, true)
   elm_object_part_content_set(tab_edj.layout, "swallow.path", tab_edj.path);
   elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);
   elipsis_btn_add(tab_edj.path, _elipsis, NULL);

   /* label.path */
   elm_object_part_text_set(tab_edj.layout, "label.edj", _("Path to edj-file:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.edj, true)
   evas_object_smart_callback_add(tab_edj.edj, "changed", _validate, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.edj", tab_edj.edj);
   elipsis_btn_add(tab_edj.edj, _elipsis_edj, NULL);

   /* label.meta_version */
   elm_object_part_text_set(tab_edj.layout, "label.meta_version", _("Version of file:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.meta_version, true)
   elm_object_part_content_set(tab_edj.layout, "swallow.meta_version", tab_edj.meta_version);
   /* label.meta_authors */
   elm_object_part_text_set(tab_edj.layout, "label.meta_authors", _("Authors:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.meta_authors, false)
   elm_object_part_content_set(tab_edj.layout, "swallow.meta_authors", tab_edj.meta_authors);
   /* label.meta_licenses */
   elm_object_part_text_set(tab_edj.layout, "label.meta_licenses", _("Licenses:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.meta_licenses, false)
   elm_object_part_content_set(tab_edj.layout, "swallow.meta_licenses", tab_edj.meta_licenses);
   /* label.meta_comment */
   elm_object_part_text_set(tab_edj.layout, "label.meta_comment", _("Comment:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.meta_comment, false)
   elm_object_part_content_set(tab_edj.layout, "swallow.meta_comment", tab_edj.meta_comment);
   elm_entry_entry_set(tab_edj.meta_comment, N_("Created with Eflete!"));

   return tab_edj.layout;
}
