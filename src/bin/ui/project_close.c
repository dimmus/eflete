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
#include "tabs.h"
#include "project_manager2.h"
#include "project_navigator.h"
#include "project_common.h"

static Eina_Bool
_progress_print(void *data __UNUSED__, Eina_Stringshare *progress_string)
{
   elm_object_part_text_set(ap.splash, "label.info", progress_string);

   return true;
}

static void
_progress_export_edj_end(void *data __UNUSED__, PM_Project_Result result, Project *project __UNUSED__)
{
   char buf[BUFF_MAX];

   if (ap.path.export_edc)
     {
        Eina_List *groups = tabs_open_groups_get();
        result = pm_group_source_code_export(ap.project, groups, ap.path.export_edc,
                                             _progress_print, progress_end, NULL);
        eina_list_free(groups);
        if (PM_PROJECT_SUCCESS != result)
          {
             snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
             popup_add(_("Export source code"), NULL, BTN_CANCEL, NULL, NULL);
          }
        return;
     }
   progress_end(NULL, result, NULL);
}

static void
_progress_save_end(void *data __UNUSED__, PM_Project_Result result, Project *project __UNUSED__)
{
   char buf[BUFF_MAX];

   if (PM_PROJECT_SUCCESS == result)
     {
        ap.project->changed = false;
        if (ap.path.export_edj)
          {
             Eina_List *groups = tabs_open_groups_get();
             result = pm_project_develop_export(ap.project, ap.path.export_edj, groups,
                                                _progress_print, _progress_export_edj_end, NULL);
             eina_list_free(groups);
             if (PM_PROJECT_SUCCESS != result)
               {
                  snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
                  popup_add(_("Export develop edj"), NULL, BTN_CANCEL, NULL, NULL);
               }
            return;
          }

        if (ap.path.export_edc)
          {
             Eina_List *groups = tabs_open_groups_get();
             result = pm_group_source_code_export(ap.project, groups, ap.path.export_edc,
                                                  _progress_print, progress_end, NULL);
             eina_list_free(groups);
             if (PM_PROJECT_SUCCESS != result)
               {
                  snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
                  popup_add(_("Export source code"), NULL, BTN_CANCEL, NULL, NULL);
                  return;
               }
             return;
          }
     }
   progress_end(NULL, result, NULL);
}

static Eina_Bool
_setup_save_splash(void *data, Splash_Status status __UNUSED__)
{
   char buf[PATH_MAX];
   PM_Project_Result result;

   result = pm_project_save(ap.project,
                            _progress_print,
                            _progress_save_end,
                            data);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Project save"), NULL, BTN_CANCEL, NULL, NULL);
        return false;
     }
   else
     {
        /* HOTFIX: now save - it's only file copy, and if file is small spash
         * animation start after file copy and animation will player in loop.
         * For avoid this situation I was add this hack.
         *
         * Need to update it, and rewrite correctly.
         */
        return false;
     }

   return true;
}

static Eina_Bool
_teardown_save_splash(void *data __UNUSED__, Splash_Status status)
{

   if (status == SPLASH_SUCCESS)
     STATUSBAR_PROJECT_SAVE_TIME_UPDATE();

   ap.project->changed = false;

   return true;
}

void
project_save(void)
{
   assert(ap.project != NULL);

   if (!ap.project->changed) return;
   if (ap.splash) return;

   ap.splash = splash_add(ap.win, _setup_save_splash, _teardown_save_splash, NULL, NULL);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);

   ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, true);
}

#ifndef HAVE_TIZEN
static void
_popup_close_cb(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *ei)
{
   Popup_Button btn_res = (Popup_Button) ei;

   switch (btn_res)
     {
      case BTN_OK:
         project_save();
         break;
      case BTN_DONT_SAVE:
         ap.project->changed = false;
         break;
      case BTN_CANCEL:
         ap.exit_in_progress = false;
         return;
      default:
         ERR("Popup return wrong value. Go fix it!");
         abort(); /* it's wrong value need to fix popup code or popup call */
     }
   if (ap.exit_in_progress)
     ui_main_window_del();
   else
     project_close(NULL, NULL);
}
#endif

Eina_Bool
project_close(Evas_Smart_Cb func, void *data)
{
   char buf[PATH_MAX];
   PM_Project_Result result;
   Project *project_to_close;
   assert(ap.project != NULL);

#ifndef HAVE_TIZEN
   Evas_Object *popup;
   Eina_Stringshare *title;

   if (ap.project->changed)
     {
        title = eina_stringshare_printf(_("Close project %s"), ap.project->name);
        popup = popup_add(title,
                          _("Do you want to save changes?"),
                          BTN_OK|BTN_DONT_SAVE|BTN_CANCEL,
                          NULL,
                          NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _popup_close_cb, NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, func, data);
        eina_stringshare_del(title);
        return false;
     }
#endif
   project_to_close = ap.project;
   tabs_clean();
   result = pm_project_close(project_to_close);
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Project close"), NULL, BTN_CANCEL, NULL, NULL);
        return false;
     }

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
   ui_menu_disable_set(ap.menu, MENU_WINDOW_MANAGER_SCRIPT, true);
   ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, true);
   project_navigator_project_unset();

   /* some code in close project callback checks ap.project for NULL, so we need to
      change it before closing project */
   ap.project = NULL;
   elm_layout_text_set(ap.win_layout, "eflete.project.time", _("Last saved: none"));
   elm_layout_text_set(ap.win_layout, "eflete.project.part", _("Project path: none"));

   return true;
}
