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
   ui_main_window_del(ap);
}

Eina_Bool
ui_main_window_del(App_Data *ap)
{
   if ((ap->project) && (!project_close_request(ap,
                                 _("You want to close Eflete, but now you have<br/>"
                                   "opened project. If you dont save opened project<br/>"
                                   "all your changes will be lost!"))))
     return false;

   if (!history_term(ap->history))
     WARN("Failed terminate history module");
#ifdef HAVE_ENVENTOR
   code_edit_mode_switch(ap, false);
#endif

   if (ap->project)
     pm_project_close(ap->project);

   eina_hash_free(ap->menu_hash);
   ap->menu_hash = NULL;
   config_save(ap);
   INFO("%s %s - Finished...", PACKAGE_NAME, VERSION);
   /* FIXME: remove it from here */
   live_view_free(ap->live_view);
   /* FIXME: when be implemented multi workspace feature, remove this line */
   evas_object_del(ap->workspace);
   elm_exit();

   return true;
}

Evas_Object *
_statusbar_init(Evas_Object *obj)
{
   Evas_Object *statusbar = NULL;
   Evas_Object *label = NULL;
   Ewe_Statusbar_Item *item = NULL;

   statusbar = ewe_statusbar_add(obj);
   elm_object_part_content_set(obj, "eflete.swallow.statusbar",
                               statusbar);
   evas_object_show(statusbar);
   LABEL_ADD(statusbar, label, _("No project opened"));

   item = ewe_statusbar_item_append(statusbar, label,
                                    EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_label_set(item, _("Last saved: "));
   /* MAGIC number 500 is width of item, which display path to currently open
      project. It will be fixed in ewe_statusbar module from ewe library. Currently
      width param "-1"(unlimited width) work incorrect. */
   ewe_statusbar_item_width_set(item, 500);

   LABEL_ADD(statusbar, label, _("the project is not opened"));
   item = ewe_statusbar_item_append(statusbar, label,
                                    EWE_STATUSBAR_ITEM_TYPE_OBJECT, NULL, NULL);
   ewe_statusbar_item_label_set(item, _("Project path: "));
   /* MAGIC number 500 is width of item, which display path to currently open
      project. It will be fixed in ewe_statusbar module from ewe library. Currently
      width param "-1"(unlimited width) work incorrect. */
   ewe_statusbar_item_width_set(item, 500);

   return statusbar;
}

#define MARK_TO_SHUTDOWN(fmt, ...) \
   { \
      ERR(fmt, ## __VA_ARGS__); \
      return false; \
   } \

Eina_Bool
ui_main_window_add(App_Data *ap)
{
   Config *config;
   Evas_Object *bg;

   if (!ap)
     {
        ERR("Can't create the window. App_Data is NULL");
        return EINA_FALSE;
     }
   config_load(ap);
   config = config_get();

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   ap->win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);

   if (ap->win == NULL)
     {
        ERR("Failed to create main window.");
        return false;
     }
   evas_object_resize(ap->win, config->window.w, config->window.h);
   evas_object_move(ap->win, config->window.x, config->window.y);

   elm_win_title_set(ap->win, "EFL Edje Theme Editor");

   /* FIXME: Magic that solves font issue with property item */
   Evas_Object *N = elm_layout_add(ap->win);
   elm_layout_file_set(N, EFLETE_EDJ, "eflete/property/item/default");
   evas_object_resize(N, 1, 1);
   /* End of magic */

   evas_object_smart_callback_add(ap->win, "delete,request", _on_done, ap);
   if (!cursor_main_set(ap->win, CURSOR_ARROW))
     ERR("Main cursor not setted.");

   bg = elm_bg_add(ap->win);
   elm_win_resize_object_add(ap->win, bg);
   evas_object_size_hint_min_set(bg, 1366, 768);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(ap->win, false);
   evas_object_show(bg);

   ap->win_layout = elm_layout_add(ap->win);
   if (!ap->win_layout) MARK_TO_SHUTDOWN("Failed to create layout main window.")
   evas_object_size_hint_weight_set(ap->win_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   elm_win_resize_object_add(ap->win, ap->win_layout);
   elm_layout_file_set(ap->win_layout, EFLETE_EDJ, "eflete/main_window/base/default");
   evas_object_show(ap->win_layout);

   ap->main_menu = ui_menu_add(ap);
   if (!ap->main_menu)
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

   if (!register_callbacks(ap))
     MARK_TO_SHUTDOWN("Failed to register callbacks");

   ap->statusbar = _statusbar_init(ap->win_layout);
   if (!ap->statusbar)
     MARK_TO_SHUTDOWN("Can't create a statusbar.")

   ap->history = history_init();
   if (!ap->history)
     MARK_TO_SHUTDOWN("Failed initialize history module.")

   return true;
}
#undef MARK_TO_SHUTDOWN
