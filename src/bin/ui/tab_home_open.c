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
_setup_open_splash(void *data, Splash_Status status __UNUSED__)
{
   Eina_Stringshare *path = data;

   assert(path != NULL);

   pm_project_open(path, progress_print, _tabs_progress_end, NULL);
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

   if (ap.project)
     if (!project_close())
       return;
   selected = elm_fileselector_selected_get(tab.fs);
   if ((!selected) || !eina_str_has_suffix(selected, ".pro")) return;

   if (!pm_lock_check(selected))
     {
       popup_want_action(_("Open project"), _("The given file is locked by another application"),
                         NULL, NULL, BTN_OK, NULL, NULL);
       return;
     }

   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          (void *)eina_stringshare_add(selected));
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

static void
_selected(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   const char *selected = event_info;
   char *p = strrchr(selected, '.');

   if ((!p) && (strcmp(p, "pro")))
     elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), true);
   else
     elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), false);
}
/* because selected doesn't work with folders */
static void
_unselected(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), true);
}

static void
_recent_clear(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Popup_Button btn_res;
   btn_res = popup_want_action(_("Confirm clear recent list"),
                               _("Are you sure you want to clear list of "
                                 "recently opened projects?<br>"),
                               NULL, NULL, BTN_OK|BTN_CANCEL, NULL, NULL);
   if (BTN_CANCEL == btn_res) return;

   config_recent_list_clear();
   _tab_open_project_recents_update();
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
   evas_object_smart_callback_add(tab.btn_clear, "clicked", _recent_clear, NULL);

   tab.fs = elm_fileselector_add(ap.win);
   elm_fileselector_expandable_set(tab.fs, false);
   elm_fileselector_path_set(tab.fs, profile_get()->general.projects_folder);
   elm_fileselector_custom_filter_append(tab.fs, _eflete_filter, NULL, "Eflete Files");
   evas_object_smart_callback_add(tab.fs, "done", _open, NULL);
   evas_object_smart_callback_add(tab.fs, "selected", _selected, NULL);
   /* small hack for disabling 'Open' button when .pro is not selected */
   evas_object_smart_callback_add(elm_layout_content_get(tab.fs, "elm.swallow.files"),
                                  "unselected", _unselected, NULL);
   evas_object_smart_callback_add(tab.fs, "activated", _open, NULL);
   /* small hack, hide not necessary button */
   evas_object_hide(elm_layout_content_unset(tab.fs, "elm.swallow.filters"));
   evas_object_hide(elm_layout_content_unset(tab.fs, "elm.swallow.cancel"));
   /* one more hack, set text our text to button 'ok' */
   elm_object_text_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), _("Open"));
   elm_object_disabled_set(elm_layout_content_get(tab.fs, "elm.swallow.ok"), true);

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

   if (ap.project)
     if (!project_close())
       return;

   if (!pm_lock_check(r->path))
     {
       popup_want_action(_("Open project"), _("The given file is locked by another application"),
                         NULL, NULL, BTN_OK, NULL, NULL);
       return;
     }

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
        evas_object_smart_callback_add(btn, "clicked", _open_recent, r);
        elm_box_pack_end(box, btn);
        evas_object_show(btn);
     }
   evas_object_show(box);
}
