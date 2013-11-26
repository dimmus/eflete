/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "ui_main_window.h"
#include "settings.h"
#include "part_dialog.h"
#include "state_dialog.h"
#include "style_dialog.h"

static void
_on_done(void *data,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_main_window_del(ap);
}

void
ui_main_window_del(App_Data *ap)
{
   eina_hash_free(ap->menu_hash);
   ui_panes_settings_save();
   INFO("%s: %s - Finished...", ETE_PACKAGE_NAME, VERSION);
   pm_free(ap->project);
   ws_free(ap->ws);
   demo_free(ap->demo);
   elm_exit();
}

static void
_on_window_resize(void *data __UNUSED__,
                  Evas *e __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int w, h;
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   ui_resize_pans(w,h);
}

static void
_add_part_dailog(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   part_dialog_add(ap->win_layout, ap->ws->groupspace);
}

static void
_add_state_dailog(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   state_dialog_add(ap);
}

static void
_del_style(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_style_delete(ap);
}

static void
_add_style_dailog(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   style_dialog_add(ap);
}

Eina_Bool
ui_main_window_add(App_Data *ap)
{
   Evas_Object *win, *bg, *layout;

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   win = elm_win_add(NULL, "panes", ELM_WIN_BASIC);
   if (win == NULL)
     {
        ERR("Failrue create main window.");
        return EINA_FALSE;
     }
   ap->win = win;

   elm_win_title_set(win, ETE_PACKAGE);
   evas_object_smart_callback_add(win, "delete,request", _on_done, ap);
   evas_object_event_callback_add(win, EVAS_CALLBACK_RESIZE, _on_window_resize,
                                  NULL);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(win, EINA_FALSE);
   evas_object_show(bg);

   layout = elm_layout_add(win);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, layout);
   elm_layout_file_set(layout, TET_EDJ, "ui/main_window");
   evas_object_show(layout);
   ap->win_layout = layout;
   ap->main_menu = ui_menu_add(ap);
   if (!ap->main_menu) ERR("Failrue add menu on main window.");
   if (!ui_panes_add(ap)) ERR("Failrue add panes on main window.");

   if (!ecore_file_mkpath(TET_SETT_PATH))
     {
        ERR("Couldn't create settings directory");
        NOTIFY_ERROR("Couldn't create settings directory");
     }

   ui_panes_settings_load(win);
   ap->ws = ws_add(ap->block.canvas);
   ap->demo = ui_demospace_add(ap->block.bottom_right);
   ui_block_demo_view_set(ap, ap->demo->layout);

   evas_object_smart_callback_add(ap->ws->groupspace, "gs,dialog,add",
                                  _add_part_dailog, ap);
   evas_object_smart_callback_add(ap->ws->groupspace, "gs,state,add",
                                  _add_state_dailog, ap);
   evas_object_smart_callback_add(ap->block.left_top, "gs,style,add",
                                  _add_style_dailog, ap);
   evas_object_smart_callback_add(ap->block.left_top, "gs,style,del",
                                  _del_style, ap);
   evas_object_show(win);

   return EINA_TRUE;
}
