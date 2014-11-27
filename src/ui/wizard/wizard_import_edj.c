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

#include "wizard.h"
#include "main_window.h"

struct _Wizard_Import_Edj_Win
{
   Evas_Object *win;
   Evas_Object *splash;
   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edj;
   Evas_Object *meta_version;
   Evas_Object *meta_authors;
   Evas_Object *meta_licenses;
   Evas_Object *meta_comment;
   Evas_Object *fs;
   Project_Thread *thread;
};

typedef struct _Wizard_Import_Edj_Win Wizard_Import_Edj_Win;


static void
_on_cancel(void *data,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;
   wiew = (Wizard_Import_Edj_Win *)data;

   evas_object_del(wiew->win);
   free(wiew);
}

static void
_fs_close(void *data __UNUSED__,
          Evas_Object *obj,
          void *event_info __UNUSED__)
{
   evas_object_del(obj);
}

static Eina_Bool
_edje_filter(const char *path,
             Eina_Bool dir,
             void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".edj"))
     return true;
   return false;
}

#define FILESELCTOR_WINDOW(FUNC, TITLE, FOLDER_ONLY, FILTER, FIELD) \
static void \
FUNC##_done(void *data, \
            Evas_Object *obj __UNUSED__, \
            void *event_info) \
{ \
   Wizard_Import_Edj_Win *wiew; \
   const char *selected; \
\
   selected = event_info; \
   wiew = (Wizard_Import_Edj_Win *)data; \
   if ((!selected) || (!strcmp(selected, ""))) goto close; \
   elm_entry_entry_set(wiew->FIELD, selected); \
close: \
   _fs_close(NULL, wiew->fs, NULL); \
} \
\
static void \
FUNC(void *data, \
     Evas_Object *obj __UNUSED__, \
     void *event_info __UNUSED__) \
{ \
   Evas_Object *bg, *fs; \
   Wizard_Import_Edj_Win *wiew; \
   const char *path; \
\
   wiew = (Wizard_Import_Edj_Win *)data; \
\
   MODAL_WINDOW_ADD(wiew->fs, main_window_get(), TITLE, _fs_close, NULL); \
   bg = elm_bg_add(wiew->fs); \
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(bg); \
   elm_win_resize_object_add(wiew->fs, bg); \
   FILESELECTOR_ADD(fs, wiew->fs, FUNC##_done, data); \
   elm_fileselector_folder_only_set(fs, FOLDER_ONLY); \
   path = elm_entry_entry_get(wiew->FIELD); \
   if ((path) && (ecore_file_is_dir(path))) elm_fileselector_path_set(fs, path); \
   if (FILTER) elm_fileselector_custom_filter_append(fs, _edje_filter, NULL, "Edje Files"); \
   elm_win_resize_object_add(wiew->fs, fs); \
} \

FILESELCTOR_WINDOW(_on_path_bt, _("Select path for new project"), true, false, path)
FILESELCTOR_WINDOW(_on_edj_bt, _("Select edj file for import"), false, true, edj)

/************************** Project import functions **************************/

static Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string)
{
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)data;
   elm_object_text_set(wiew->splash, progress_string);

   return true;
}

static void
_progress_end(void *data, PM_Project_Result result)
{
   Wizard_Import_Edj_Win *wiew;
   Project *pro;
   App_Data *ap;

   wiew = (Wizard_Import_Edj_Win *)data;
   ap = app_data_get();

   if (result != PM_PROJECT_SUCCESS) goto close;

   pro = pm_project_thread_project_get(wiew->thread);
   ap->project = pro;
   blocks_data_unset(ap);
   blocks_show(ap);
   ui_menu_disable_set(ap->menu_hash, _("Close project"), false);

close:
   splash_del(wiew->splash);
}

static Eina_Bool
_setup_splash(void *data __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)data;
   wiew->thread = pm_project_import_edj(elm_entry_entry_get(wiew->name),
                                        elm_entry_entry_get(wiew->path),
                                        elm_entry_entry_get(wiew->edj),
                                        _progress_print,
                                        _progress_end,
                                        wiew);

   return true;
}

static Eina_Bool
_teardown_splash(void *data __UNUSED__)
{
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)data;
   evas_object_del(wiew->win);

   NOTIFY_INFO(3, _("File '%s' imported as project '%s'"),
               elm_entry_entry_get(wiew->edj),
               elm_entry_entry_get(wiew->name));

   return true;
}

/******************************************************************************/

static void
_on_apply(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   App_Data *ap;
   Wizard_Import_Edj_Win *wiew;

   ap = app_data_get();
   wiew = (Wizard_Import_Edj_Win *)data;
   wiew->splash = splash_add(ap->win, _setup_splash, _teardown_splash, wiew);
   evas_object_focus_set(wiew->splash, true);
   evas_object_show(wiew->splash);
}

Evas_Object *
wizard_import_edj_add(App_Data *ap)
{
   Evas_Object *mwin, *layout;
   Evas_Object *bt;
   Wizard_Import_Edj_Win *wiew;

   if ((ap->project) && (!project_close_request(ap,
                              _("You want to import edj as project, but now you have<br/>"
                                "opened project. If you dont save opened project<br/>"
                                "all your changes will be lost!"))))
     return NULL;


   wiew = (Wizard_Import_Edj_Win *)mem_malloc(sizeof(Wizard_Import_Edj_Win));

   mwin = mw_add(_on_cancel, wiew);
   if (!mwin) return NULL;
   mw_title_set(mwin, _("Wizard: import edj"));
   wiew->win = mwin;

   layout = elm_layout_add(mwin);
   elm_layout_file_set(layout, EFLETE_EDJ, "elm/layout/wizard/import");
   elm_object_content_set(mwin, layout);
   evas_object_show(layout);

   BUTTON_ADD(layout, bt, _("Apply"))
   elm_object_part_content_set(layout, "swallow.button1", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_apply, wiew);
   BUTTON_ADD(layout, bt, _("Cancel"))
   elm_object_part_content_set(layout, "swallow.button2", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_cancel, wiew);

   //label.name
   elm_object_part_text_set(layout, "label.name", _("Project name:"));
   ENTRY_ADD(layout, wiew->name, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.name", wiew->name);
   //label.path
   elm_object_part_text_set(layout, "label.path", _("Path to project:"));
   ENTRY_ADD(layout, wiew->path, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.path", wiew->path);
   elm_entry_entry_set(wiew->path, getenv("HOME"));

   bt = elm_button_add(layout);
   elm_object_style_set(bt, "eflete/elipsis");
   evas_object_show(bt);
   evas_object_smart_callback_add(bt, "clicked", _on_path_bt, wiew);
   elm_object_part_content_set(wiew->path, "elm.swallow.end", bt);

   //label.edj
   elm_object_part_text_set(layout, "label.edj", _("Path to edj file:"));
   ENTRY_ADD(layout, wiew->edj, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.edj", wiew->edj);

   bt = elm_button_add(layout);
   elm_object_style_set(bt, "eflete/elipsis");
   evas_object_show(bt);
   evas_object_smart_callback_add(bt, "clicked", _on_edj_bt, wiew);
   elm_object_part_content_set(wiew->edj, "elm.swallow.end", bt);

   //label.meta_version
   elm_object_part_text_set(layout, "label.meta_version", _("Version of file:"));
   ENTRY_ADD(layout, wiew->meta_version, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_version", wiew->meta_version);
   //label.meta_authors
   elm_object_part_text_set(layout, "label.meta_authors", _("Authors:"));
   ENTRY_ADD(layout, wiew->meta_authors, false, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_authors", wiew->meta_authors);
   //label.meta_licenses
   elm_object_part_text_set(layout, "label.meta_licenses", _("Licenses:"));
   ENTRY_ADD(layout, wiew->meta_licenses, false, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_licenses", wiew->meta_licenses);
   //label.meta_comment
   elm_object_part_text_set(layout, "label.meta_comment", _("Comment:"));
   ENTRY_ADD(layout, wiew->meta_comment, false, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_comment", wiew->meta_comment);
   elm_entry_entry_set(wiew->meta_comment, _("Created with Eflete!"));

   evas_object_show(mwin);
   return mwin;
}

#undef FILESELCTOR_WINDOW
