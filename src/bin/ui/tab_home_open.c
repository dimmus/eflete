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

struct _Tab_Home_Open {
   Evas_Object *layout;
   Evas_Object *btn_clear;
   Evas_Object *fs;
};

typedef struct _Tab_Home_Open Tab_Home_Open;
Tab_Home_Open tab;

static Eina_Bool
_eflete_filter(const char *path,
               Eina_Bool dir,
               void *data __UNUSED__)
{
   if (dir) return true;
   if (eina_str_has_extension(path, ".pro")) return true;
   return false;
}

static Eina_Bool
_progress_print(void *data __UNUSED__, Eina_Stringshare *progress_string)
{
   elm_object_part_text_set(ap.splash, "label.info", progress_string);
   return true;
}

static void
_progress_pm_open_end(void *data __UNUSED__, PM_Project_Result result)
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

           ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
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

static Eina_Bool
_setup_open_splash(void *data, Splash_Status status __UNUSED__)
{
   Eina_Stringshare *path = data;

   assert(path != NULL);

   pm_project_open(path, _progress_print, _progress_pm_open_end, NULL);
   eina_stringshare_del(path);

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
_open(void *data __UNUSED__,
      Evas_Object *obj __UNUSED__,
      void *event_info __UNUSED__)
{
   const char *selected;

   selected = elm_fileselector_selected_get(tab.fs);
   if ((!selected) || !eina_str_has_suffix(selected, ".pro")) return;

   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          (void *)eina_stringshare_add(selected));
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

Evas_Object *
_tab_open_project_add(void)
{
   assert(tab.layout == NULL);

   tab.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab.layout, "layout", "tab_home", "open_project");
   elm_layout_text_set(tab.layout, "elm.text.recent", _("Open recent"));

   tab.btn_clear = elm_button_add(ap.win);
   elm_object_text_set(tab.btn_clear, _("Clear list"));
   elm_layout_content_set(tab.layout, "elm.swallow.btn_clear", tab.btn_clear);
   /* while recent list not implemented this button will be disabled */
   elm_object_disabled_set(tab.btn_clear, true);

   tab.fs = elm_fileselector_add(ap.win);
   elm_fileselector_expandable_set(tab.fs, false);
   elm_fileselector_path_set(tab.fs, getenv("HOME"));
   elm_fileselector_custom_filter_append(tab.fs, _eflete_filter, NULL, "Eflete Files");
   evas_object_smart_callback_add(tab.fs, "done", _open, NULL);
   evas_object_smart_callback_add(tab.fs, "activated", _open, NULL);
   /* small hack, hide not necessary button */
   evas_object_hide(elm_layout_content_unset(tab.fs, "elm.swallow.filters"));
   evas_object_hide(elm_layout_content_unset(tab.fs, "elm.swallow.cancel"));
   /* one more hack, set text our text to button 'ok' */
   elm_object_text_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), _("Open"));

   elm_layout_content_set(tab.layout, "elm.swallow.fileselector", tab.fs);

   _tab_open_project_recents_update();
   return tab.layout;
}

static void
_open_recent(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Recent *r = (Recent *)data;

   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          (void *)eina_stringshare_add(r->path));
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

void
_tab_open_project_recents_update(void)
{
   Config *config;
   Evas_Object *box, *btn;
   Eina_List *l;
   Recent *r;

   config = config_get();
   evas_object_del(elm_layout_content_unset(tab.layout, "elm.swallow.recents"));

   if (!config->recents) return;
   BOX_ADD(tab.layout, box, false, false)
   elm_box_align_set(box, 0.5, 0.0);
   elm_layout_content_set(tab.layout, "elm.swallow.recents", box);
   EINA_LIST_FOREACH(config->recents, l, r)
     {
        btn = elm_button_add(ap.win);
        elm_object_text_set(btn, r->path);
        evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
        evas_object_smart_callback_add(btn, "clicked", _open_recent, r);
        elm_box_pack_end(box, btn);
        evas_object_show(btn);
     }
   evas_object_show(box);
}
