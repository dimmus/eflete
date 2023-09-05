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

#include "tabs_private.h"
#include "tabs.h"
#include "main_window.h"
#include "project_navigator.h"
#include "config.h"
#include "project_common.h"
#include "widget_list.h"

void
meta_controls_add(Evas_Object *layout, Meta_Data_Controls *meta)
{
   assert(layout != NULL);
   assert(meta != NULL);

   /* meta version */
   elm_object_part_text_set(layout, "label.meta_version", _("Version of file:"));
   ENTRY_ADD(layout, meta->version, true)
   elm_object_part_content_set(layout, "swallow.meta_version", meta->version);
   /* meta authors */
   elm_object_part_text_set(layout, "label.meta_authors", _("Authors:"));
   ENTRY_ADD(layout, meta->authors, false)
   elm_object_part_content_set(layout, "swallow.meta_authors", meta->authors);
   /* meta licenses */
   elm_object_part_text_set(layout, "label.meta_licenses", _("Licenses:"));
   ENTRY_ADD(layout, meta->licenses, false)
   elm_object_part_content_set(layout, "swallow.meta_licenses", meta->licenses);
   /* meta comment */
   elm_object_part_text_set(layout, "label.meta_comment", _("Comment:"));
   ENTRY_ADD(layout, meta->comment, false)
   elm_object_part_content_set(layout, "swallow.meta_comment", meta->comment);
   elm_entry_entry_set(meta->comment, N_("Created with Eflete!"));

}

void
meta_controls_data_save(Meta_Data_Controls *meta)
{
   char buf[PATH_MAX];
   PM_Project_Result result;

   if (meta == NULL) return;
   result = pm_project_meta_data_set(ap.project,
                                     ap.project->name,
                                     elm_entry_entry_get(meta->version),
                                     elm_entry_entry_get(meta->authors),
                                     elm_entry_entry_get(meta->licenses),
                                     elm_entry_entry_get(meta->comment));

   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Meta data save"), NULL, BTN_CANCEL, NULL, NULL);
     }
}

void
elipsis_btn_add(Evas_Object *entry, Evas_Smart_Cb cb_func, void *data)
{
   Evas_Object *bt;

   assert(entry != NULL);

   bt = elm_button_add(entry);
   elm_object_style_set(bt, "elipsis");
   elm_object_focus_allow_set(bt, false);
   evas_object_show(bt);
   evas_object_smart_callback_add(bt, signals.elm.button.clicked, cb_func, data);
   elm_object_part_content_set(entry, "elm.swallow.elipsis", bt);
}

Eina_Bool
entry_path_set(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Evas_Object *entry = (Evas_Object *)data;
   Eina_List *selected = (Eina_List *)event_info;

   elm_entry_entry_set(entry, eina_list_data_get(selected));
   return true;
}

void
_tabs_progress_end(void *data, PM_Project_Result result, Project *project)
{
   Meta_Data_Controls *meta = (Meta_Data_Controls *)data;

   if (PM_PROJECT_SUCCESS != result)
     {
        progress_end(data, result, project);
        return;
     }

   ap.project = project;
   assert(ap.project);
   meta_controls_data_save(meta);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_BASE, false);
   ui_menu_disable_set(ap.menu, MENU_FILE_CLOSE_PROJECT, false);
   ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, true);
   project_navigator_project_set();
   //tabs_menu_tab_open(TAB_HOME_PROJECT_INFO);

   //_group_copy(ap.project, widgets);

   config_recent_add(ap.project->name, ap.project->pro_path);
   _tab_open_project_recents_update();
   evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_OPENED, NULL);

   progress_end(data, result, project);
}

void
_tab_default_focus(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *target = data;

   assert(target != NULL);

   elm_object_focus_set(target, true);
}
