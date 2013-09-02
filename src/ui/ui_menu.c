/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
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
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui_main_window.h"
#include "open_file_dialog.h"
#include "save_file_dialog.h"
#include "style_editor.h"
#include "image_editor.h"
#include "program_editor.h"
#include "ui_highlight.h"
#include "about_window.h"

static void
_on_edc_open_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   open_edc_file(ap);
}

static void
_on_edj_open_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   open_edj_file(ap);
}

static void
_on_save_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if ((!ap) || (!ap->project)) return;

   if (pm_save_project_to_swap(ap->project))
     {
        if (pm_save_project_edj(ap->project))
          NOTIFY_INFO(3, "Theme saved: %s", ap->project->edj)
        else
          NOTIFY_ERROR("Theme can not be saved: %s", ap->project->edj);
     }
}

static void
_on_save_as_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   save_as_edj_file(ap);
}

static void
_on_edc_save_menu(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   NOTIFY_INFO(3, "Not implemented yet");
}

static void
_on_exit_menu(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   ui_main_window_del();
}

static void
_on_view_separate(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_groupspace_separate(ap->ws);
}

static void
_on_view_zoom_in(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_ws_zoom_in(ap->ws);
}

static void
_on_view_zoom_out(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_ws_zoom_in(ap->ws);
}

static void
_on_view_ruler_hor(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_visible_get(ap->ws->ruler_hor))
     ui_ruler_hide (ap->ws->ruler_hor);
   else
     ui_ruler_show (ap->ws->ruler_hor);
}

static void
_on_view_ruler_ver(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_visible_get(ap->ws->ruler_ver))
     ui_ruler_hide (ap->ws->ruler_ver);
   else
     ui_ruler_show (ap->ws->ruler_ver);
}

static void
_on_view_legend(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_ws_legend_visible_set(ap->ws, !ap->ws->legend.visible);
}

static void
_on_view_highlight(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (!ap->ws->highlight.part) return;
   hl_highlight_visible_set(ap->ws->highlight.space_hl,
                !hl_highlight_visible_get(ap->ws->highlight.space_hl));
}

static void
_on_view_ruler_rel(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_scale_relative_visible_get(ap->ws->ruler_hor))
     {
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_ver, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_ver, EINA_TRUE);
     }
}

static void
_on_view_ruler_abs(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_scale_absolute_visible_get(ap->ws->ruler_hor))
     {
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_ver, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_ver, EINA_TRUE);
     }
}

static void
_on_style_window_menu(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if(ap->project != NULL) style_editor_window_add(ap->win, ap->project);
   else NOTIFY_ERROR("EDC/EDJ file is not loaded. \n");
}

static void
_on_image_editor_menu(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   image_editor_init(image_editor_window_add(ap->win), ap->project);
}

static void
_on_ccl_viewer_menu(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   colorclass_viewer_init(colorclass_viewer_add(ap->win), ap->project);
}

static void
_on_prog_editor_menu(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   /*
   App_Data *ap = (App_Data *)data;
   if (!ap->project->current_group)
     NOTIFY_WARNING("Please open the widget style for editing style programs!")
   else
     program_editor_window_add(ap->win, ap->project->current_group);
   */
   NOTIFY_INFO(5, "Not Implemented Yet.")
}

/*
static void
_on_font_viewer_menu(void *data, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   font_viewer_init(font_viewer_add(ap->win), ap->project);
}
*/

static void
_on_about_window_menu(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   about_window_add(ap->win);
}

Eina_Bool
ui_menu_add(App_Data *ap)
{
   Evas_Object *tb, *menu;
   Elm_Object_Item *tb_it, *menu_sub;

   tb = elm_toolbar_add(ap->win_layout);
   if (tb == NULL) return EINA_FALSE;

   evas_object_size_hint_weight_set(tb, 0.0, 0.0);
   elm_toolbar_shrink_mode_set(tb, ELM_TOOLBAR_SHRINK_NONE);
   elm_toolbar_align_set(tb, 0.0);
   elm_toolbar_homogeneous_set(tb, EINA_TRUE);
   elm_object_part_content_set(ap->win_layout, "swallow/toolbar", tb);
   evas_object_show(tb);
   elm_toolbar_icon_size_set(tb,16);

   elm_toolbar_icon_order_lookup_set(tb, ELM_ICON_LOOKUP_FDO_THEME);
   tb_it = elm_toolbar_item_append(tb, NULL, "File", NULL, NULL);
   if (tb_it == NULL) return EINA_FALSE;

   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   if (menu == NULL) return EINA_FALSE;

   elm_menu_item_add(menu, NULL, "menu/folder", "Open edc-file",
                     _on_edc_open_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/folder", "Open edj-file",
                     _on_edj_open_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/file", "Save", _on_save_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/file", "Save as...", _on_save_as_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/file", "Save as EDC", _on_edc_save_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/close", "Exit", _on_exit_menu, ap);

   tb_it = elm_toolbar_item_append(tb, NULL, "View", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   menu_sub = elm_menu_item_add(menu, NULL, NULL, "Workspace", NULL, NULL);
   elm_menu_item_add(menu, menu_sub, NULL, "Zoom in", _on_view_zoom_in, ap);
   elm_menu_item_add(menu, menu_sub, NULL, "Zoom out", _on_view_zoom_out, ap);
   elm_menu_item_add(menu, menu_sub, NULL, "Separate", _on_view_separate, ap);
   elm_menu_item_add(menu, menu_sub, NULL, "Legend", _on_view_legend, ap);
   menu_sub = elm_menu_item_add(menu, NULL, NULL, "Rulers", NULL, NULL);
   elm_menu_item_add(menu, menu_sub, NULL, "Show/hide hor.", _on_view_ruler_hor, ap);
   elm_menu_item_add(menu, menu_sub, NULL, "Show/hide ver.", _on_view_ruler_ver, ap);
   elm_menu_item_add(menu, menu_sub, NULL, "Absolute scale", _on_view_ruler_abs, ap);
   elm_menu_item_add(menu, menu_sub, NULL, "Relative scale", _on_view_ruler_rel, ap);
   elm_menu_item_add(menu, NULL, NULL, "Highlight space", _on_view_highlight, ap);

   tb_it = elm_toolbar_item_append(tb, NULL, "Editors", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);

   elm_menu_item_add(menu, NULL, NULL, "Styles", _on_style_window_menu, ap);
   elm_menu_item_add(menu, NULL, NULL, "Images", _on_image_editor_menu, ap);
   elm_menu_item_add(menu, NULL, NULL, "Colorclasses", _on_ccl_viewer_menu, ap);
   elm_menu_item_add(menu, NULL, NULL, "Programs", _on_prog_editor_menu, ap);
   //elm_menu_item_add(menu, NULL, NULL, "Fonts", _on_font_viewer_menu, ap);

   elm_toolbar_menu_parent_set(tb, ap->win_layout);

   tb_it = elm_toolbar_item_append(tb, NULL, "Help", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   elm_menu_item_add(menu, NULL, "help-about", "About",
                     _on_about_window_menu, ap);

   tb_it = elm_toolbar_item_append(tb, NULL, "Separator", NULL, NULL);
   elm_toolbar_item_separator_set(tb_it, EINA_TRUE);
   elm_toolbar_icon_order_lookup_set(tb, ELM_ICON_LOOKUP_THEME);

   ap->main_menu = tb;
   return EINA_TRUE;
}
