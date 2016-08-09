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
#include "project_manager.h"
#include "project_navigator.h"

static Eina_Bool
_progress_print(void *data __UNUSED__, Eina_Stringshare *progress_string)
{
   elm_object_part_text_set(ap.splash, "label.info", progress_string);

   return true;
}

static void
_progress_end(void *data __UNUSED__, PM_Project_Result result, Project *project __UNUSED__)
{

   switch (result)
     {
      case PM_PROJECT_ERROR:
        {
           ERR(_("Error: project not saved."));
           break;
        }
      case PM_PROJECT_CANCEL:
        {
           INFO(_("Saving canceled."));
           break;
        }
      case PM_PROJECT_SUCCESS:
        {
           ap.project->changed = false;
           break;
        }
      default:
        {
           ERR("Wrong result");
           abort();
        }
     }

#ifdef HAVE_ENVENTOR
   if (ap.enventor_mode)
     {
        wm_widgets_list_objects_load(ap.project->widgets,
                                     evas_object_evas_get(ap.win),
                                     ap.project->mmap_file);
        wm_layouts_list_objects_load(ap.project->layouts,
                                     evas_object_evas_get(ap.win),
                                     ap.project->mmap_file);
        wm_styles_build_alias(ap.project->widgets,
                              ap.project->layouts);
        enventor_object_focus_set(ap.enventor, true);
        //pm_save_to_dev(ap.project, ap.project->current_style, true);
     }
#endif /* HAVE_ENVENTOR */

   splash_del(ap.splash);
   ap.splash = NULL;
}

static Eina_Bool
_setup_save_splash(void *data, Splash_Status status __UNUSED__)
{

#ifdef HAVE_ENVENTOR
   if (ap.enventor_mode)
     {
        enventor_object_file_version_update(ap.enventor, ap.project, "110");

        pm_project_enventor_save(ap.project,
                                 _progress_print,
                                 _progress_end,
                                 data);
        pm_save_to_dev(ap.project, ap.project->current_style, true);
     }
   else
     {
#endif /* HAVE_ENVENTOR */
        pm_project_save(ap.project,
                        _progress_print,
                        _progress_end,
                        data);
#ifdef HAVE_ENVENTOR
     }
#endif /* HAVE_ENVENTOR */

   return true;
}

static Eina_Bool
_teardown_save_splash(void *data __UNUSED__, Splash_Status status)
{

   if (status == SPLASH_SUCCESS)
     STATUSBAR_PROJECT_SAVE_TIME_UPDATE();

   ap.project->changed = false;
   pm_project_thread_free();

   return true;
}

void
project_save(void)
{
   assert(ap.project != NULL);

#ifdef HAVE_ENVENTOR
   if (!ap.enventor_mode)
#endif /* HAVE_ENVENTOR */
     if (!ap.project->changed) return;
   if (ap.splash) return;

   ap.splash = splash_add(ap.win, _setup_save_splash, _teardown_save_splash, NULL, NULL);
   evas_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);

#ifdef HAVE_ENVENTOR
   if (!ap.enventor_mode)
#endif /* HAVE_ENVENTOR */
     ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, true);
}


Eina_Bool
project_close(void)
{
   Popup_Button btn_res;
   Eina_Stringshare *title;

   assert(ap.project != NULL);

   if (ap.project->changed)
     {
        title = eina_stringshare_printf(_("Close project %s"), ap.project->name);
        btn_res = popup_want_action(title, _("Do you want to save changes?"), NULL,
                                    BTN_OK|BTN_DONT_SAVE|BTN_CANCEL,
                                    NULL, NULL);
        switch (btn_res)
          {
           case BTN_OK:
              project_save();
              break;
           case BTN_DONT_SAVE:
              break;
           case BTN_CANCEL:
              return false;
           default:
              ERR("Popup return wrong value. Go to fix it!");
              abort(); /* it's wrong value need to fix popup code or popup call */
          }
        eina_stringshare_del(title);
     }

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
   project_navigator_project_unset();
   tabs_clean();

   pm_project_close(ap.project);
   ap.project = NULL;
   elm_layout_text_set(ap.win_layout, "eflete.project.time", _("Last saved: none"));
   elm_layout_text_set(ap.win_layout, "eflete.project.part", _("Project path: none"));

   return true;
}
