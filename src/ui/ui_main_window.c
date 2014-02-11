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
   /*TODO: add mesasge about save the project */
   App_Data *ap = (App_Data *)data;
   /* TODO: add unsaved project check here*/
   ui_main_window_del(ap);

   ecore_main_loop_quit();
}

Eina_Bool
ui_main_window_del(App_Data *ap)
{
   if (!ap)
     {
        ERR("ap is NULL");
        return false;
     }
   eina_hash_free(ap->menu_hash);
   ui_panes_settings_save();
   INFO("%s: %s - Finished...", ETE_PACKAGE_NAME, VERSION);
   if (ap->project)
     pm_free(ap->project);
   /* FIXME: remove it from here */
   demo_free(ap->demo);
   /* FIXME: when be implemented multi workspace feature, remove this line */
   evas_object_del(ap->workspace);
   elm_exit();
   return true;
}

static void
_on_window_resize(void *data __UNUSED__,
                  Evas *e __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int w, h;
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   ui_resize_panes(w,h);
}

#define MARK_TO_SHUTDOWN(fmt, ...) \
   { \
      ERR(fmt, ## __VA_ARGS__); \
      return false; \
   } \

Eina_Bool
ui_main_window_add(App_Data *ap)
{
   Evas_Object *win, *bg, *layout;

   if (!ap)
     {
        ERR("Can't create the window. App_Data is NULL");
        return EINA_FALSE;
     }

   if (!ecore_file_mkpath(EFLETE_SETT_PATH))
     ERR("Can't create settings directory");

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);

   if (win == NULL)
     {
        ERR("Failrue create main window.");
        return false;
     }
   ap->win = win;

   elm_win_title_set(win, ETE_PACKAGE);
   evas_object_smart_callback_add(win, "delete,request", _on_done, ap);
   evas_object_event_callback_add(win, EVAS_CALLBACK_RESIZE, _on_window_resize,
                                  NULL);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(win, false);
   evas_object_show(bg);

   layout = elm_layout_add(win);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   elm_win_resize_object_add(win, layout);
   elm_layout_file_set(layout, EFLETE_EDJ, "ui/main_window");
   evas_object_show(layout);
   ap->win_layout = layout;
   if (!ap->win_layout)
     MARK_TO_SHUTDOWN("Failrue create layout main window.")

   ap->main_menu = ui_menu_add(ap);
   if (!ap->main_menu)
     MARK_TO_SHUTDOWN("Failrue add menu on main window.")

   if (!ui_panes_add(ap))
     MARK_TO_SHUTDOWN("Failrue add panes on main window.")

   ui_panes_settings_load(win);
   ap->workspace = workspace_add(ap->block.canvas);
   if (!ap->workspace)
     MARK_TO_SHUTDOWN("Failrue create workspace in main window.")

   ui_block_ws_set(ap, ap->workspace);
   evas_object_show(ap->workspace);
   ap->demo = ui_demospace_add(ap->block.bottom_right);
   if (!ap->demo)
     MARK_TO_SHUTDOWN("Failed create live view")
   else
     ui_block_demo_view_set(ap, ap->demo->layout);

   ap->colorsel = colorselector_add(ap->win);
   if (!ap->colorsel)
     MARK_TO_SHUTDOWN("Can't create a colorselector.")

   if (!register_callbacks(ap))
     MARK_TO_SHUTDOWN("Failed register callbacks");

   evas_object_show(win);
   ecore_main_loop_begin();
   return true;
}
#undef MARK_TO_SHUTDOWN
