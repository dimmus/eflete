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

Eina_Bool
progress_print(void *data __UNUSED__, Eina_Stringshare *progress_string)
{
   elm_object_part_text_set(ap.splash, "label.info", progress_string);
   fprintf(stdout,"%s\n", progress_string);
   return true;
}

void
progress_end(void *data __UNUSED__, PM_Project_Result result)
{

   switch (result)
     {
      case PM_PROJECT_ERROR:
        {
           NOTIFY_INFO(3, _("Can't open project."));
           break;
        }
      case PM_PROJECT_CANCEL:
        {
           NOTIFY_INFO(3, _("Project opening canceled."));
           break;
        }
      case PM_PROJECT_SUCCESS:
        {
           ap.project = pm_project_thread_project_get();
           assert(ap.project);

           ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_TEMPORARY, false);
           ui_menu_disable_set(ap.menu, MENU_FILE_CLOSE_PROJECT, false);
           navigator_project_set();
           tabs_tab_home_open(TAB_HOME_PROJECT_INFO);

           NOTIFY_INFO(3, _("Project '%s' is opened."), ap.project->name);
           STATUSBAR_PROJECT_PATH(ap.project->pro_path);
           STATUSBAR_PROJECT_SAVE_TIME_UPDATE();

           config_recent_add(ap.project->name, ap.project->pro_path);
           _tab_open_project_recents_update();
           break;
        }
      default:
        {
           ERR("Wrong result");
           abort();
        }
     }

   splash_del(ap.splash);
   ap.splash = NULL;
}
