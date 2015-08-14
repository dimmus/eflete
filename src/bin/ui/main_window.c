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

#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif /* include eflete_config.h */

#include "main_window.h"
#include "shortcuts.h"
#include "cursor.h"

static void
_on_done(void *data,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   assert(ap != NULL);

   ui_main_window_del(ap);
}

Eina_Bool
ui_main_window_del(App_Data *ap)
{
   if (!project_close(ap))
     return false;

   if (!history_term(ap->history))
     {
        ERR("Failed terminate history module");
        abort();
     }
#ifdef HAVE_ENVENTOR
   code_edit_mode_switch(ap, false);
#endif

   /* FIXME: remove it from here */
   live_view_free(ap->live_view);

   free(ap->menu);
   ap->menu = NULL;
   cursor_main_free();

   config_save(ap);
   INFO("%s %s - Finished...", PACKAGE_NAME, VERSION);
   /* FIXME: when be implemented multi workspace feature, remove this line */
   evas_object_del(ap->workspace);
   elm_exit();

   return true;
}

#define MARK_TO_SHUTDOWN(fmt, ...) \
   { \
      ERR(fmt, ## __VA_ARGS__); \
      abort(); \
   } \

Eina_Bool
ui_main_window_add(App_Data *ap)
{
   Config *config;
   Evas_Object *bg;

   assert(ap != NULL);

   config_load(ap);
   config = config_get();

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   ap->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);

   assert(ap->win != NULL);

   evas_object_resize(ap->win, config->window.w, config->window.h);
   evas_object_move(ap->win, config->window.x, config->window.y);

   elm_win_title_set(ap->win, "EFL Edje Theme Editor");

   evas_object_smart_callback_add(ap->win, "delete,request", _on_done, ap);
   if (!cursor_main_set(ap->win, CURSOR_ARROW))
     {
        ERR("Main cursor not setted.");
        abort();
     }

   elm_object_theme_set(ap->win, ap->theme);

   bg = elm_bg_add(ap->win);
   elm_win_resize_object_add(ap->win, bg);
   evas_object_size_hint_min_set(bg, 1024, 600);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(ap->win, false);
   evas_object_show(bg);

   ap->win_layout = elm_layout_add(ap->win);
   elm_layout_theme_set(ap->win_layout, "layout", "window", "main");
   evas_object_size_hint_weight_set(ap->win_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(ap->win, ap->win_layout);
   elm_layout_text_set(ap->win_layout, "eflete.project.time", _("Last saved: none"));
   elm_layout_text_set(ap->win_layout, "eflete.project.part", _("Project path: none"));
   evas_object_show(ap->win_layout);

   ap->menu = ui_menu_add(ap);
   if (!ap->menu)
     MARK_TO_SHUTDOWN("Failed to add menu on main window.")

   if (!ui_panes_add(ap))
     MARK_TO_SHUTDOWN("Failed to add panes on main window.")

   ap->workspace = workspace_add(ap->block.canvas);
   if (!ap->workspace)
     MARK_TO_SHUTDOWN("Failed to create workspace in main window.")

   ui_block_ws_set(ap, ap->workspace);
   evas_object_show(ap->workspace);
   ap->live_view = live_view_add(ap->block.bottom_right, false);
   if (!ap->live_view)
     MARK_TO_SHUTDOWN("Failed to create live view")
   else
     ui_block_live_view_set(ap, ap->live_view->layout);

   ap->colorsel = colorselector_add(ap->win);
   if (!ap->colorsel)
     MARK_TO_SHUTDOWN("Can't create a colorselector.")

   #ifdef HAVE_ENVENTOR
     ap->enventor= enventor_object_init(ap->win);
     if (!ap->enventor)
       MARK_TO_SHUTDOWN("Can't create a enventor object.")
   #endif /* HAVE_ENVENTOR */

   if (!register_callbacks(ap))
     MARK_TO_SHUTDOWN("Failed to register callbacks");

   ap->history = history_init();
   if (!ap->history)
     MARK_TO_SHUTDOWN("Failed initialize history module.")

   return true;
}
#undef MARK_TO_SHUTDOWN
