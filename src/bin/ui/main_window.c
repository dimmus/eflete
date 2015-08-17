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
#include "navigator.h"
#include "shortcuts.h"
#include "cursor.h"

static void
_on_done(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   assert(ap != NULL);
   ui_main_window_del();
}

Eina_Bool
ui_main_window_del(void)
{
   if (!project_close())
     return false;

   /*
   if (!history_term(ap->history))
     {
        ERR("Failed terminate history module");
        abort();
     }
   */
#ifdef HAVE_ENVENTOR
   code_edit_mode_switch(false);
#endif

   /* FIXME: remove it from here */
   //live_view_free(ap->live_view);

   free(ap->menu);
   ap->menu = NULL;
   cursor_main_free();

   config_save();
   INFO("%s %s - Finished...", PACKAGE_NAME, VERSION);
   /* FIXME: when be implemented multi workspace feature, remove this line */
   evas_object_del(ap->workspace);
   elm_exit();

   return true;
}

Eina_Bool
ui_main_window_add(void)
{
   Config *config;
   Evas_Object *bg, *navigator;

   assert(ap != NULL);

   config_load();
   config = config_get();

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   ap->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);

   assert(ap->win != NULL);

   evas_object_resize(ap->win, config->window.w, config->window.h);
   evas_object_move(ap->win, config->window.x, config->window.y);

   elm_win_title_set(ap->win, "EFL Edje Theme Editor");

   evas_object_smart_callback_add(ap->win, "delete,request", _on_done, NULL);
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

   /* add panes to main window */
   ap->panes.left = elm_panes_add(ap->win_layout);
   evas_object_size_hint_weight_set(ap->panes.left, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ap->panes.left, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(ap->win_layout, "eflete.swallow.panes", ap->panes.left);

   ap->panes.right = elm_panes_add(ap->win_layout);
   evas_object_size_hint_weight_set(ap->panes.right, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ap->panes.right, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(ap->panes.left, "right", ap->panes.right);

   ap->panes.right_hor = elm_panes_add(ap->win_layout);
   elm_panes_horizontal_set(ap->panes.right_hor, true);
   evas_object_size_hint_weight_set(ap->panes.right_hor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ap->panes.right_hor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(ap->panes.right, "right", ap->panes.right_hor);

   /* apply the panes size from config */
   elm_panes_content_left_size_set(ap->panes.left, config->panes.left);
   elm_panes_content_left_size_set(ap->panes.right, config->panes.right);
   elm_panes_content_left_size_set(ap->panes.right_hor, config->panes.right_hor);

   navigator = navigator_add();
   elm_object_part_content_set(ap->panes.left, "left", navigator);

   ap->menu = ui_menu_add();
   //ui_panes_add();
   //ap->workspace = workspace_add(ap->block.canvas);
   //ui_block_ws_set(ap->workspace);
   //evas_object_show(ap->workspace);
   //ap->live_view = live_view_add(ap->block.bottom_right, false);
   //ui_block_live_view_set(ap->live_view->layout);
   //ap->colorsel = colorselector_add(ap->win);
   #ifdef HAVE_ENVENTOR
     ap->enventor= enventor_object_init(ap->win);
   #endif /* HAVE_ENVENTOR */
   //register_callbacks();
   //ap->history = history_init();

   return true;
}
