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

#include "wizard_common.h"

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

void
_fs_close(void *data __UNUSED__,
          Evas_Object *obj,
          void *event_info __UNUSED__)
{
   evas_object_del(obj);
}

FILESELCTOR_WINDOW(_on_path_bt, _("Select path for new project"), true, false, NULL, path)

/************************** Project import functions **************************/

Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string)
{
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)data;
   elm_object_text_set(wiew->splash, progress_string);

   return true;
}

void
_progress_end(void *data, PM_Project_Result result)
{
   Wizard_Import_Edj_Win *wiew;
   Project *pro;
   App_Data *ap;

   wiew = (Wizard_Import_Edj_Win *)data;
   ap = app_data_get();

   if (result == PM_PROJECT_SUCCESS)
     {
        pro = pm_project_thread_project_get(wiew->thread);
        ap->project = pro;

        wm_widgets_list_objects_load(pro->widgets,
                                     evas_object_evas_get(ap->win),
                                     pro->dev);
        wm_layouts_list_objects_load(pro->layouts,
                                     evas_object_evas_get(ap->win),
                                     pro->dev);

        blocks_show(ap);
        ui_menu_disable_set(ap->menu_hash, _("Close project"), false);

        if (!eina_inlist_count(ap->project->widgets))
          ui_widget_list_tab_activate(ui_block_widget_list_get(ap), 1);
     }
   splash_del(wiew->splash);
}

static Eina_Bool
_teardown_splash(void *data)
{
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)data;
   evas_object_del(wiew->win);

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

   if (!wiew->splash_setup_func) return;

   wiew->splash = splash_add(ap->win, wiew->splash_setup_func, _teardown_splash, wiew);
   evas_object_focus_set(wiew->splash, true);
   evas_object_show(wiew->splash);
}

void
project_path_item_add(Wizard_Import_Edj_Win *wiew,
                      const char *label,
                      Evas_Smart_Cb cb_func)
{
   Evas_Object *bt;

   elm_object_part_text_set(wiew->layout, "label.edj", label);

   ENTRY_ADD(wiew->layout, wiew->edj, true, DEFAULT_STYLE)
   elm_object_part_content_set(wiew->layout, "swallow.edj", wiew->edj);

   bt = elm_button_add(wiew->layout);
   elm_object_style_set(bt, "eflete/elipsis");
   evas_object_show(bt);
   evas_object_smart_callback_add(bt, "clicked", cb_func, wiew);
   elm_object_part_content_set(wiew->edj, "elm.swallow.end", bt);
}

Wizard_Import_Edj_Win *
wizard_import_common_add(const char *layout_name)
{
   Evas_Object *mwin, *layout;
   Evas_Object *bt;
   Wizard_Import_Edj_Win *wiew;

   wiew = (Wizard_Import_Edj_Win *)mem_malloc(sizeof(Wizard_Import_Edj_Win));

   mwin = mw_add(_on_cancel, wiew);
   if (!mwin) return NULL;

   wiew->win = mwin;

   layout = elm_layout_add(mwin);
   elm_layout_file_set(layout, EFLETE_EDJ, layout_name);

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

   wiew->layout = layout;

   evas_object_show(mwin);

   return wiew;
}

