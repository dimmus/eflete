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
#include "navigator.h"

static Eina_Bool want_close;
static Evas_Object *popup;

static void
_cancel_cb(void *data __UNUSED__,
           Evas_Object *obj __UNUSED__,
           void *ei __UNUSED__)
{
   want_close = false;
   ecore_main_loop_quit();
}

static void
_save_cb(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *ei __UNUSED__)
{
   evas_object_del(popup);
   popup = NULL;
   project_save();
   want_close = true;
   ecore_main_loop_quit();
}

static void
_discard_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *ei __UNUSED__)
{
   want_close = true;
   ecore_main_loop_quit();
}

static void
_popup_want_save(void)
{
   Evas_Object *btn, *label;
   Eina_Stringshare *title;

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);

   title = eina_stringshare_printf(_("Close project %s"), ap.project->name);
   popup = elm_popup_add(ap.win_layout);
   elm_object_part_text_set(popup, "title,text", title);

   BUTTON_ADD(popup, btn, _("Save"));
   evas_object_smart_callback_add(btn, "clicked", _save_cb, NULL);
   elm_object_part_content_set(popup, "button1", btn);
   BUTTON_ADD(popup, btn, _("Don't save"));
   evas_object_smart_callback_add(btn, "clicked", _discard_cb, NULL);
   elm_object_part_content_set(popup, "button2", btn);
   BUTTON_ADD(popup, btn, _("Cancel"));
   evas_object_smart_callback_add(btn, "clicked", _cancel_cb, NULL);
   elm_object_part_content_set(popup, "button3", btn);
   LABEL_ADD(popup, label, _("Do you want to save changes?"));
   elm_object_content_set(popup, label);

   evas_object_show(popup);

   ecore_main_loop_begin();
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

Eina_Bool
project_close(void)
{
   assert(ap.project != NULL);

   if (ap.project->changed)
     {
        _popup_want_save();
        if (!want_close) return false;
     }

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
   navigator_project_unset();
   tabs_clean();
   //ui_property_style_unset(Evas_Object *property);
   //ui_signal_list_data_unset(Evas_Object *object);
   //history_clear(History *history);
   tabs_tab_home_open(TAB_HOME_OPEN_PROJECT);

   pm_project_close(ap.project);
   elm_layout_text_set(ap.win_layout, "eflete.project.time", _("Last saved: none"));
   elm_layout_text_set(ap.win_layout, "eflete.project.part", _("Project path: none"));

   return true;
}
