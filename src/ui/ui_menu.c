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
_on_exit_menu(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_main_window_del(ap);
}

static void
_on_view_separate(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_groupspace_separate(ap->ws);
   ui_menu_disable_set(ap, "Highlight space", !ap->ws->separated);
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

static void
_on_about_window_menu(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   about_window_add(ap->win);
}

Evas_Object *
ui_menu_add(App_Data *ap)
{
   Evas_Object *menu, *toolbar;
   Elm_Object_Item *it, *menu_it, *sub_menu;
   Eina_Hash *menu_elms_hash = NULL;

   menu = elm_win_main_menu_get(ap->win);
   menu_elms_hash = eina_hash_string_small_new(NULL);

#define ITEM_MENU_ADD(menu_obj, parent_menu, icon, label, callback, data, ret) \
   ret = elm_menu_item_add(menu_obj, parent_menu, icon, label, callback, data); \
   eina_hash_add(menu_elms_hash, label, ret);

   ITEM_MENU_ADD(menu, NULL, NULL, "File", NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "New theme", _on_new_theme_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Open edc-file", _on_edc_open_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Open edj-file", _on_edj_open_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Save", _on_save_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Save as...", _on_save_as_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Save to edc", _on_edc_save_menu, ap, it);
   elm_menu_item_separator_add(menu, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Exit", _on_exit_menu, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, "View", NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Workspace", NULL, NULL, sub_menu);
   ITEM_MENU_ADD(menu, sub_menu, NULL, "Zoom in", _on_view_zoom_in, ap, it);
   elm_object_item_disabled_set(it, true);
   ITEM_MENU_ADD(menu, sub_menu, NULL, "Zoom out", _on_view_zoom_out, ap, it);
   elm_object_item_disabled_set(it, true);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Separate", _on_view_separate, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Legend", _on_view_legend, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Rulers", NULL, NULL, sub_menu);
   ITEM_MENU_ADD(menu, sub_menu, NULL, "Show/hide hor.", _on_view_ruler_hor, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, "Show/hide ver.", _on_view_ruler_ver, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, "Absolute scale", _on_view_ruler_abs, ap, it);
   ITEM_MENU_ADD(menu, sub_menu, NULL, "Relative scale", _on_view_ruler_rel, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Highlight space", _on_view_highlight, ap, it);

   ITEM_MENU_ADD(menu, NULL, NULL, "Editors", NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Styles", _on_style_window_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Images", _on_image_editor_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Colorclasses", _on_ccl_viewer_menu, ap, it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "Programs", _on_prog_editor_menu, ap, it);
   elm_object_item_disabled_set(it, true);

   ITEM_MENU_ADD(menu, NULL, NULL, "Help", NULL, NULL, menu_it);
   ITEM_MENU_ADD(menu, menu_it, NULL, "About", _on_about_window_menu , ap, it);

#undef ITEM_MENU_ADD

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

   elm_toolbar_item_append(toolbar, TET_IMG_PATH"icon-new_project.png", "New project", _on_new_theme_menu, ap);
   elm_toolbar_item_append(toolbar, TET_IMG_PATH"icon-open_project.png", "Open project", _on_edj_open_menu, ap);
   elm_toolbar_item_append(toolbar, TET_IMG_PATH"icon_save.png", "Save project", _on_save_menu, ap);

   ap->menu_hash = menu_elms_hash;
   return menu;
}
