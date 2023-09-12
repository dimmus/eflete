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
#include "project_common.h"
#include "config.h"

struct _Tab_Home_Open {
   Evas_Object *layout;
   Evas_Object *btn_clear;
   Evas_Object *fs;
};

typedef struct _Tab_Home_Open Tab_Home_Open;
static Tab_Home_Open tab;
static Eina_Bool recover = false;

static Eina_Bool
_eflete_filter(const char *path,
               Eina_Bool dir,
               void *data EINA_UNUSED)
{
   if (dir) return true;
   if (eina_str_has_extension(path, ".pro")) return true;
   return false;
}

static Eina_Bool
_setup_open_splash(void *data, Splash_Status status EINA_UNUSED)
{
   Eina_Bool ret = true;
   Eina_Stringshare *path = data;
   PM_Project_Result result;
   char buf[PATH_MAX];

   assert(path != NULL);

   result = pm_project_open(path, progress_print, _tabs_progress_end, NULL, recover);
   recover = false;
   if (PM_PROJECT_SUCCESS != result)
     {
        snprintf(buf, sizeof(buf), "Warning: %s", pm_project_result_string_get(result));
        popup_add(_("Project open"), NULL, BTN_CANCEL, NULL, NULL);
        ret = false;
     }
   eina_stringshare_del(path);

   return ret;
}

static Eina_Bool
_teardown_open_splash(void *data EINA_UNUSED, Splash_Status status EINA_UNUSED)
{
   return true;
}

EINA_UNUSED static Eina_Bool
_cancel_open_splash(void *data EINA_UNUSED, Splash_Status status EINA_UNUSED)
{
   //pm_project_thread_cancel();
   return true;
}

static void
_popup_recover_cb(void *data,
                  Evas_Object *obj EINA_UNUSED,
                  void *event_info)
{
   Eina_Stringshare *file = data;
   Popup_Button btn_res = (Popup_Button)event_info;

   recover = true;
   if (BTN_OK == btn_res)
     {
        ap.splash = splash_add(ap.win,
                               _setup_open_splash,
                               _teardown_open_splash,
                               NULL,
                               (void *)eina_stringshare_add(file));
        evas_object_show(ap.splash);
     }
}

static void
_open_after_popup_close(void *data EINA_UNUSED,
                        Evas_Object *obj EINA_UNUSED,
                        void *event_info EINA_UNUSED)
{
   const char *selected;
   PM_Project_Result result;
   Evas_Object *popup;

   selected = elm_fileselector_selected_get(tab.fs);
   if ((!selected) || !eina_str_has_suffix(selected, ".pro")) return;

   result = pm_lock_check(selected);
   if (PM_PROJECT_LOCKED_PROC_MISS == result)
     {
        popup = popup_add(_("Open project"), _("Given project is dameged. Do you want to recover project?"),
                          BTN_OK | BTN_CANCEL, NULL, NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _popup_recover_cb, selected);
        return;
     }
   else if (PM_PROJECT_SUCCESS != result)
     {
        popup_add(_("Open project"), _("The given file is locked by another application"), BTN_OK, NULL, NULL);
        return;
     }

   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          NULL,
                          (void *)eina_stringshare_add(selected));
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

static void
_open_done(void *data EINA_UNUSED,
           Evas_Object *obj EINA_UNUSED,
           void *event_info EINA_UNUSED)
{
   if (ap.project)
     if (!project_close(_open_after_popup_close, NULL))
       return;

   _open_after_popup_close(NULL, NULL, (void *)BTN_OK);
}

static void
_selected(void *data EINA_UNUSED,
          Evas_Object *obj EINA_UNUSED,
          void *event_info)
{
   const char *selected = event_info;
   char *p = strrchr(selected, '.');

   if ((!p) || (strcmp(p, ".pro")))
     elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), true);
   else
     elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), false);
}
/* because selected doesn't work with folders */
static void
_unselected(void *data EINA_UNUSED,
          Evas_Object *obj EINA_UNUSED,
          void *event_info EINA_UNUSED)
{
   elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), true);
}

static void
_recent_clear_popup_close_cb(void *data EINA_UNUSED,
                             Evas_Object *obj EINA_UNUSED,
                             void *event_info)
{
   Popup_Button btn_res = (Popup_Button) event_info;
   if (BTN_CANCEL == btn_res) return;

   config_recent_list_clear();
   _tab_open_project_recents_update();
}

static void
_recent_clear(void *data EINA_UNUSED,
              Evas_Object *obj EINA_UNUSED,
              void *event_info EINA_UNUSED)
{
   Evas_Object *popup = popup_add(_("Confirm clear recent list"),
                                  _("Are you sure you want to clear list of "
                                    "recently opened projects?<br>"),
                                  BTN_OK|BTN_CANCEL, NULL, NULL);
   evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _recent_clear_popup_close_cb, NULL);
}

static void
_fs_mode_cb(void *data EINA_UNUSED,
            Evas_Object *obj,
            void *event_info EINA_UNUSED)
{
   int mode = elm_radio_value_get(obj);

   if (mode == 0)
     {
        elm_fileselector_mode_set(tab.fs, ELM_FILESELECTOR_LIST);
        elm_fileselector_thumbnail_size_set(tab.fs, 14, 14);
     }
   else if (mode == 1)
     {
        elm_fileselector_mode_set(tab.fs, ELM_FILESELECTOR_GRID);
        elm_fileselector_thumbnail_size_set(tab.fs, 64, 64);
     }
}

Evas_Object *
_tab_open_project_add(void)
{
   Evas_Object *r_list, *r_grid;
   tab.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab.layout, "layout", "tab_home", "open_project");
   elm_layout_text_set(tab.layout, "elm.text.recent", _("Open recent"));

   tab.btn_clear = elm_button_add(ap.win);
   elm_object_text_set(tab.btn_clear, _("Clear list"));
   elm_layout_content_set(tab.layout, "elm.swallow.btn_clear", tab.btn_clear);
   evas_object_smart_callback_add(tab.btn_clear, signals.elm.button.clicked, _recent_clear, NULL);

   tab.fs = elm_fileselector_add(ap.win);
   elm_object_style_set(tab.fs, "extended");
   elm_fileselector_expandable_set(tab.fs, false);
   elm_fileselector_path_set(tab.fs, profile_get()->general.projects_folder);
   elm_fileselector_custom_filter_append(tab.fs, _eflete_filter, NULL, "Eflete Files");
   evas_object_smart_callback_add(tab.fs, signals.elm.fileselector.done, _open_done, NULL);
   evas_object_smart_callback_add(tab.fs, signals.elm.fileselector.selected, _selected, NULL);
   /* small hack for disabling 'Open' button when .pro is not selected */
   evas_object_smart_callback_add(elm_layout_content_get(tab.fs, "elm.swallow.files"),
                                  signals.elm.genlist.unselected, _unselected, NULL);
   evas_object_smart_callback_add(tab.fs, signals.elm.fileselector.activated, _open_done, NULL);
   /* small hack, hide not necessary button */
   evas_object_hide(elm_layout_content_unset(tab.fs, "elm.swallow.filters"));
   evas_object_hide(elm_layout_content_unset(tab.fs, "elm.swallow.cancel"));
   /* one more hack, set text our text to button 'ok' */
   elm_object_text_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), _("Open"));
   elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), true);

   r_list = elm_radio_add(tab.fs);
   elm_object_style_set(r_list, "fs_list");
   elm_radio_state_value_set(r_list, 0);
   elm_layout_content_set(tab.fs, "elm.swallow.btn_list", r_list);
   evas_object_smart_callback_add(r_list, signals.elm.radio.changed, _fs_mode_cb, NULL);

   r_grid = elm_radio_add(tab.fs);
   elm_object_style_set(r_grid, "fs_grid");
   elm_radio_state_value_set(r_grid, 1);
   elm_layout_content_set(tab.fs, "elm.swallow.btn_grid", r_grid);
   elm_radio_group_add(r_list, r_grid);
   evas_object_smart_callback_add(r_grid, signals.elm.radio.changed, _fs_mode_cb, NULL);

   elm_layout_content_set(tab.layout, "elm.swallow.fileselector", tab.fs);

   _tab_open_project_recents_update();
   return tab.layout;
}

static void
_recent_after_popup_close(void *data,
                          Evas_Object *obj EINA_UNUSED,
                          void *event_info)
{
   Recent *r = (Recent *)data;
   Popup_Button pbtn = (Popup_Button) event_info;
   PM_Project_Result result;
   Evas_Object *popup;

   if (BTN_CANCEL == pbtn) return;

   result = pm_lock_check(r->path);
   if (PM_PROJECT_LOCKED_PROC_MISS == result)
     {
        popup = popup_add(_("Open project"), _("Given project is dameged. Do you want to recover project?"),
                          BTN_OK | BTN_CANCEL, NULL, NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _popup_recover_cb, r->path);
        return;
     }
   else if (PM_PROJECT_SUCCESS != result)
     {
        popup_add(_("Open project"), _("The given file is locked by another application"), BTN_OK, NULL, NULL);
        return;
     }

   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          NULL,
                          (void *)eina_stringshare_add(r->path));
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

static void
_open_recent(void *data,
             Evas_Object *obj EINA_UNUSED,
             void *event_info EINA_UNUSED)
{

   if (ap.project)
     if (!project_close(_recent_after_popup_close, data))
       return;

   _recent_after_popup_close(data, NULL, (void *)BTN_OK);
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
   elm_object_disabled_set(tab.btn_clear, true);

   if (!config->recents) return;
   elm_object_disabled_set(tab.btn_clear, false);
   BOX_ADD(tab.layout, box, false, false)
   elm_box_align_set(box, 0.5, 0.0);
   elm_box_padding_set(box, 0, 6);
   elm_layout_content_set(tab.layout, "elm.swallow.recents", box);
   EINA_LIST_FOREACH(config->recents, l, r)
     {
        btn = elm_button_add(ap.win);
        elm_object_style_set(btn, "recent");
        elm_object_part_text_set(btn, "elm.text", r->name);
        elm_object_part_text_set(btn, "elm.subtext", r->path);
        evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
        evas_object_smart_callback_add(btn, signals.elm.button.clicked, _open_recent, r);
        elm_box_pack_end(box, btn);
        evas_object_show(btn);
     } 
   evas_object_show(box);
   
   /* free 'r' created in config_recent_add (see _tabs_progress_end as well) */
   free(r);
}
