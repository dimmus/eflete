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
#include "history_ui.h"
#include "project_navigator.h"
#include "tabs.h"
#include "shortcuts.h"
#include "cursor.h"
#include "config.h"
#include "property.h"

static void
_project_navigator_group_open(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   tabs_tab_add((Group *)event_info);
}

static void
_close_request(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   ui_main_window_del();
}

static void
_help(void *data __UNUSED__,
      Evas_Object *obj __UNUSED__,
      void *event_info __UNUSED__)
{
   shortcuts_window_add();
}

Eina_Bool
ui_main_window_del(void)
{
   ap.exit_in_progress = true;
   if (ap.project)
     if (!project_close())
       return false;

#ifdef HAVE_ENVENTOR
   code_edit_mode_switch(false);
#endif

   free(ap.menu);
   ap.menu = NULL;
   cursor_main_free();

   config_save();
   evas_object_del(ap.property.group);
   INFO("%s %s - Finished...", PACKAGE_NAME, VERSION);
   elm_exit();

   return true;
}

Eina_Bool
ui_main_window_add(void)
{
   Config *config;
   Evas_Object *bg, *project_navigator, *tabs;

   config = config_get();

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   ap.win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);

   assert(ap.win != NULL);

   evas_object_resize(ap.win, config->window.w, config->window.h);
   evas_object_move(ap.win, config->window.x, config->window.y);

#ifdef HAVE_TIZEN
   elm_win_title_set(ap.win, "Component Designer");
#else
   elm_win_title_set(ap.win, "EFL Edje Theme Editor");
#endif

   evas_object_smart_callback_add(ap.win, "delete,request", _close_request, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_QUIT, _close_request, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_HELP, _help, NULL);

#if 0 // turn off the eflete main cursor, while not used elementary combobox, and not fixed bug with double cursors
   if (!cursor_main_set(ap.win, CURSOR_ARROW))
     {
        ERR("Main cursor not setted.");
        abort();
     }
#endif /* if 0 */

   elm_object_theme_set(ap.win, ap.theme);

   bg = elm_bg_add(ap.win);
   elm_win_resize_object_add(ap.win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(ap.win, false);
   evas_object_show(bg);

   ap.win_layout = elm_layout_add(ap.win);
   elm_layout_theme_set(ap.win_layout, "layout", "window", "main");
   evas_object_size_hint_weight_set(ap.win_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(ap.win, ap.win_layout);
   elm_layout_text_set(ap.win_layout, "eflete.project.time", _("Last saved: none"));
   elm_layout_text_set(ap.win_layout, "eflete.project.part", _("Project path: none"));
   evas_object_show(ap.win_layout);

   /* add panes to main window */
   ap.panes.left = elm_panes_add(ap.win);
   elm_object_part_content_set(ap.win_layout, "eflete.swallow.panes", ap.panes.left);

   ap.panes.right = elm_panes_add(ap.win);
   elm_object_part_content_set(ap.panes.left, "right", ap.panes.right);

   ap.panes.left_ver = elm_panes_add(ap.win);
   elm_panes_horizontal_set(ap.panes.left_ver, true);
   elm_object_part_content_set(ap.panes.left, "left", ap.panes.left_ver);

   /* apply the panes size from config */
   elm_panes_content_left_size_set(ap.panes.left, config->panes.left);
   elm_panes_content_left_size_set(ap.panes.right, config->panes.right);
   elm_panes_content_left_size_set(ap.panes.left_ver, config->panes.left_ver);

   project_navigator = project_navigator_add();
   evas_object_smart_callback_add(project_navigator, SIGNAL_GROUP_OPEN, _project_navigator_group_open, NULL);
   elm_object_part_content_set(ap.panes.left_ver, "left", project_navigator);

   tabs = tabs_add();
   elm_object_part_content_set(ap.panes.right, "left", tabs);

   ap.property.group = property_add(ap.win, PROPERTY_MODE_GROUP);
   ap.property.image_manager = property_add(ap.win, PROPERTY_MODE_IMAGE);
   ap.property.sound_manager = property_add(ap.win, PROPERTY_MODE_SOUND);
   ap.property.style_manager = property_add(ap.win, PROPERTY_MODE_STYLE);
   ap.property.color_manager = property_add(ap.win, PROPERTY_MODE_COLOR_CLASS);
   ap.property.demo = property_add(ap.win, PROPERTY_MODE_DEMO);
   elm_object_part_content_set(ap.panes.right, "right", ap.property.group);

   ap.menu = ui_menu_add();

   #ifdef HAVE_ENVENTOR
     ap.enventor= enventor_object_init(ap.win);
   #endif /* HAVE_ENVENTOR */

   if (!shortcuts_init())
     {
        CRIT("Can't initialize the shortcut module");
        return false;
     }

   elm_config_window_auto_focus_enable_set(false);
   return true;
}

#if !HAVE_TIZEN
Evas_Object *
_about_window_content_get(void *data, Evas_Object *popup __UNUSED__, Evas_Object **to_focus __UNUSED__)
{
   Evas_Object *label = (Evas_Object *) data;
   elm_object_text_set(label,
     "<color=#b6b6b6>"
     "<b><align=center>"PACKAGE_NAME" v."VERSION"</align></b><br>"
     "This application was written for Enlightenment project.<br>"
     "It is designed to create and modify styles of Elementary widgets.<br>"
     "<br>"
     "Copyright (C) 2013 - 2015 Samsung Electronics.<br>"
     "<br>"
     "<align=center><b>Authors:</b><br>"
     "Vyacheslav \"rimmed\" Reutskiy (v.reutskiy@samsung.com)<br>"
     "Mykyta Biliavskyi (m.biliavskyi@samsung.com)<br>"
     "Vitalii Vorobiov (vi.vorobiov@samsung.com)<br>"
     "Andrii Kroitor (an.kroitor@samsung.com)<br>"
     "Kateryna Fesyna (fesyna1@gmail.com)<br>"
     "Maksym Volodin (mac9.ua@gmail.com)<br>"
     "Igor Gala (igor.gala89@gmail.com)<br>"
     "<br>"
     "Olga Kolesnik (o.kolesnik@samsung.com)<br>"
     "<br>"
     "Oleg Dotsenko (o.dotsenko@samsung.com)<br>"
     "Yurii Tsivun (y.tsivun@samsung.com)<br>"
     "Dmitriy Samoylov (dm.samoylov@samsung.com)<br>"
     "</align>");

   elm_object_style_set(label, "slide_about");
   elm_layout_signal_emit(label, "elm,state,slide,start", "elm");

   return label;
}

Evas_Object *
about_window_add(void)
{
   Evas_Object *content = elm_label_add(ap.win);
   popup_add(_("About"), NULL, BTN_CANCEL, _about_window_content_get, content);
   return NULL;
}

#else
Evas_Object *
_about_window_content_get(void *data, Evas_Object *popup __UNUSED__, Evas_Object **to_focus __UNUSED__)
{
  Evas_Object *layout = (Evas_Object *)data;
  elm_layout_theme_set(layout, "layout", "about", "default");
  elm_object_part_text_set(layout, "ver.text", VERSION);
  evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
  return layout;
}

Evas_Object *
about_window_add(void)
{
   Evas_Object *content = elm_layout_add(ap.win);
   popup_add(_("About Component Designer"), NULL, BTN_OK, _about_window_content_get, content);
   return NULL;
}

#endif
static Evas_Object *
_shortcuts_window_content_get(void *data, Evas_Object *popup __UNUSED__, Evas_Object **to_focus __UNUSED__)
{
   Evas_Object *box = data;
   Evas_Object *scroller = elm_scroller_add(ap.win);
   Evas_Object *label = elm_label_add(scroller);
   elm_object_text_set(label, _(
                       "Global:<br>"
                       "<b>F1</b> - show shortcuts list (this dialog)<br>"
                       "<b>F2</b> - normal mode<br>"
                       "<b>F3</b> - code mode<br>"
                       "<b>F4</b> - demo mode<br>"
                       "<b>F7</b> - open image manager<br>"
                       "<b>F8</b> - open sound manager<br>"
                       "<b>F9</b> - open style manager<br>"
                       "<b>F10</b> - open colorclass manager<br>"
                       "<b>ctrl + q</b> - quit<br>"
                       "<br>"
                       "Workspace:<br>"
                       "<b>ctrl + s</b> - save<br>"
                       "<b>ctrl + z</b> - undo<br>"
                       "<b>ctrl + y</b> - redo<br>"
                       "<b>ctrl + n</b> - add new group<br>"
                       "<b>q</b> - add new part<br>"
                       "<b>w</b> - add new state<br>"
                       "<b>e</b> - add new item<br>"
                       "<b>r</b> - add new program<br>"
                       "<b>t</b> - add new data item<br>"
                       "<b>del</b> - delete selected part/state/item/program/data item<br>"
                       "<b>s</b> - select next state of active part<br>"
                       "<b>z</b> - select prev part<br>"
                       "<b>x</b> - select next part<br>"
                       "<b>ESC</b> - unselect<br>"
                       "<b>o</b> - show/hide object area<br>"
                       "<b>ctrl + wheel_up</b> / <b>KP_ADD</b> - zoom in<br>"
                       "<b>ctrl + wheel_down</b> / <b>KP_SUB</b> - zoom out<br>"
                       "<b>KP_DIV</b> - set zoom to 100%<br>"
                       "<br>"
                       "Tabs:<br>"
                       "<b>ctrl + w</b> - close tab<br>"
                       "<b>ctrl + num</b> - switch to tab 1-10<br>"
                       "<b>tab</b> / <b>ctrl + pg_down</b> - switch to next tab<br>"
                       "<b>shift + tab</b> / <b>ctrl + pg_up</b> - switch to prev tab<br>"
                       "<br>"
                       "Popups:<br>"
                       "<b>Enter</b> - OK<br>"
                       "<b>ESC</b> - cancel<br>"
                       ));

   elm_object_style_set(label, "help");
   elm_object_content_set(scroller, label);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(scroller);
   elm_box_pack_end(box, scroller);

   return box;
}

Evas_Object *
shortcuts_window_add(void)
{
   Evas_Object *content = elm_box_add(ap.win);

   evas_object_size_hint_min_set(content, 0, 300);

   popup_add(_("Help: shortcuts"), NULL, BTN_OK, _shortcuts_window_content_get, content);
   return NULL;
}
