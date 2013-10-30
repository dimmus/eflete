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
* along with this program; If not, see .
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
_on_new_theme_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   new_theme_create(ap);
}

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
   if ((!ap) || (!ap->project))
     {
        ERR("Project coud'nt be save");
        return;
     }

   if (pm_save_project_to_swap(ap->project))
     {
        if (!ap->project->edj)
          {
             save_as_edj_file(ap);
          }
        else
          {
             if (pm_save_project_edj(ap->project))
               {
                  NOTIFY_INFO(3, "Theme saved: %s", ap->project->edj)
                  ui_demospace_set(ap->demo, ap->project, ap->project->current_group);
               }
             else
               NOTIFY_ERROR("Theme can not be saved: %s", ap->project->edj);
          }
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
   App_Data *ap = (App_Data *)data;
   save_as_edc_file(ap);
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
   ui_ws_zoom_out(ap->ws);
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
   if ((!ap->ws->highlight.part) || (!ap->ws->highlight.space_hl)) return;
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
   if(ap->project != null) style_editor_window_add(ap->win, ap->project);
   else NOTIFY_ERROR("EDC/EDJ file is not loaded. \n");
}

static void
_on_image_editor_menu(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *imed = image_editor_window_add(ap->win, MULTIPLE);
   image_editor_init(imed, ap->project);
}

static void
_on_ccl_viewer_menu(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *cced = colorclass_viewer_add(ap->win);
   colorclass_viewer_init(cced, ap->project);
}

static void
_on_prog_editor_menu(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (!ap->project->current_group)
     NOTIFY_WARNING("Please open the widget style for editing style programs!")
   else
     program_editor_window_add(ap->win, ap->project->current_group);
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
   Evas_Object *menu, *toolbar;
   Elm_Object_Item *it, *menu_it, *sub_menu;

   menu = elm_win_main_menu_get(ap->win);

   menu_it = elm_menu_item_add(menu, null, null, "File", null, null);
   elm_menu_item_add(menu, menu_it, null, "New theme", _on_new_theme_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Open edc-file", _on_edc_open_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Open edj-file", _on_edj_open_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Save", _on_save_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Save as...", _on_save_as_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Save to edc", _on_edc_save_menu, ap);
   elm_menu_item_separator_add(menu, menu_it);
   elm_menu_item_add(menu, menu_it, null, "Exit", _on_exit_menu, ap);

   menu_it = elm_menu_item_add(menu, null, null, "View", null, null);
   sub_menu = elm_menu_item_add(menu, menu_it, null, "Workspace", null, null);
   it = elm_menu_item_add(menu, sub_menu, null, "Zoom in", _on_view_zoom_in, ap);
   elm_object_item_disabled_set(it, false); /* FIXME: */
   it = elm_menu_item_add(menu, sub_menu, null, "Zoom out", _on_view_zoom_out, ap);
   elm_object_item_disabled_set(it, false); /* FIXME:  */
   elm_menu_item_add(menu, menu_it, null, "Separete", _on_view_separate, ap);
   elm_menu_item_add(menu, menu_it, null, "Legend", _on_view_legend, ap);
   sub_menu = elm_menu_item_add(menu, menu_it, null, "Rulers", null, null);
   elm_menu_item_add(menu, sub_menu, null, "Show/hide hor.", _on_view_ruler_hor, ap);
   elm_menu_item_add(menu, sub_menu, null, "Show/hide ver.", _on_view_ruler_ver, ap);
   elm_menu_item_add(menu, sub_menu, null, "Absolute scale", _on_view_ruler_abs, ap);
   elm_menu_item_add(menu, sub_menu, null, "Relative scale", _on_view_ruler_rel, ap);
   elm_menu_item_add(menu, menu_it, null, "Highlight space", _on_view_highlight, ap);

   menu_it = elm_menu_item_add(menu, null, null, "Editors", null, null);
   elm_menu_item_add(menu, menu_it, null, "Styles", _on_style_window_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Images", _on_image_editor_menu, ap);
   elm_menu_item_add(menu, menu_it, null, "Colorclasses", _on_ccl_viewer_menu, ap);
   it = elm_menu_item_add(menu, menu_it, null, "Programs", _on_prog_editor_menu, ap);
   elm_object_item_disabled_set(it, false);
   //elm_menu_item_add(menu, null, null, "Fonts", _on_font_viewer_menu, ap);

   menu_it = elm_menu_item_add(menu, null, null, "Help", null, null);
   elm_menu_item_add(menu, menu_it, null, "About", _on_about_window_menu, ap);

   toolbar = elm_toolbar_add(ap->win);
   elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_MENU);
   /*TODO: for this select mode need new style */
   elm_toolbar_select_mode_set(toolbar, ELM_OBJECT_SELECT_MODE_NONE);
   elm_object_style_set(toolbar, DEFAULT_STYLE);
   elm_toolbar_align_set(toolbar, 0.0);
   evas_object_size_hint_weight_set(toolbar, 0.0, 0.0);
   evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, 0.0);
   elm_object_part_content_set(ap->win_layout, "swallow/button_toolbar", toolbar);
   evas_object_show(toolbar);

   elm_toolbar_item_append(toolbar, null, "New project", _on_new_theme_menu, ap);
   elm_toolbar_item_append(toolbar, null, "Open project", _on_edj_open_menu, ap);
   elm_toolbar_item_append(toolbar, null, "Save project", _on_save_menu, ap);

/*
   (TET_IMG_PATH"icon-new_project.png", "New project")
   (TET_IMG_PATH"icon-open_project.png", "Open project")
   (TET_IMG_PATH"icon_save.png", "Save project")
*/
   return true;
}
