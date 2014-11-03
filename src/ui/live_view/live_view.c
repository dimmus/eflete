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

#include "live_view.h"
#include "live_view_prop.h"
#include "notify.h"
#include "container.h"

#define SWALLOW_BG "eflete.swallow.bg"
#define SWALLOW_CONTENT "eflete.swallow.content"
#define SWALLOW_MENU "eflete.swallow.menu"

Live_View *
live_view_add(Evas_Object *parent, Eina_Bool in_prog_edit)
{
   Live_View *live;
   Evas_Object *bg;

   if (!parent) return NULL;

   live = mem_calloc(1, sizeof(Live_View));

   live->in_prog_edit = in_prog_edit;

   live->layout = elm_layout_add(parent);
   elm_layout_file_set(live->layout, EFLETE_EDJ, "eflete/live_view/toolbar/default");
   bg = elm_bg_add(live->layout);
   elm_bg_color_set(bg, 203, 207, 209);
   evas_object_show(bg);

   live->live_view = container_add(parent);
   live->panel = elm_panel_add(parent);
   live->property = live_view_property_add(live->panel, in_prog_edit);
   elm_object_content_set(live->panel, live->property);
   elm_panel_orient_set(live->panel, ELM_PANEL_ORIENT_RIGHT);
   evas_object_size_hint_weight_set(live->panel, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(live->panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(live->panel);

   elm_object_part_content_set(live->layout, SWALLOW_CONTENT, live->live_view);
   elm_object_part_content_set(live->layout, SWALLOW_MENU, live->panel);
   elm_object_part_content_set(live->layout, SWALLOW_BG, bg);
   container_confine_set(live->live_view, bg);
   evas_object_hide(live->live_view);
   elm_layout_signal_emit(live->layout, "live_view,hide", "eflete");

   return live;
}

Eina_Bool
live_view_widget_style_set(Live_View *live, Project *project, Style *style)
{
   char **c;
   const char *widget = NULL, *type, *style_name;
   const char *custom_name = NULL;
   const char *fail_message = NULL;
   Eina_Bool ret = true;
   int x, y;

   if ((!live) || (!project)) return false;

   if (!style)
     {
        WARN("Could'nt apply the style to live view. The style is missing!");
        return false;
     }

   live_view_widget_style_unset(live);

   if ((style->__type != LAYOUT) && (!live->in_prog_edit))
     {
        if (strstr(style->full_group_name, "gengrid"))
          {
             c = eina_str_split(style->full_group_name, "/", 5);
             custom_name = c[4];
          }
        else
          c = eina_str_split(style->full_group_name, "/", 4);

        widget = c[1];
        type = c[2];
        style_name = c[3];

        if (!live->object)
          {
             live->object = live_widget_create(widget, type, style_name, live->layout);
             container_content_set(live->live_view, live->object);
          }

        if (!live->object)
          {
             fail_message = _("Widget isn't implemented yet!");
             ret = false;
          }
        if (ret)
          {
             const char *version = edje_edit_data_value_get(style->obj, "version");
             if (!version || strcmp(version, "110"))
               {
                  fail_message = _("Outdated version of EDJ file! Should be 110");
                  ret = false;
               }
          }
        if (!ret)
          {
             if (live->object)
               live_widget_del(live->object);
             live->object = elm_label_add(live->layout);
             elm_object_text_set(live->object, fail_message);
             container_content_set(live->live_view, live->object);
          }
        else
          live_view_property_style_set(live->property, live->object, style, widget);

        live_view_theme_update(live, project);
        if ((!strcmp(type, "item")) && (custom_name))
          elm_object_style_set(live->object, custom_name);
        else
          elm_object_style_set(live->object, style_name);

        free(c[0]);
        free(c);
     }
   else
     {
        if (!live->in_prog_edit)
          {
             live->object = layout_custom_create(live->layout);
             elm_layout_file_set(live->object, project->dev, style->full_group_name);
             elm_object_style_set(live->object, style->full_group_name);
          }
        else
          {
             live->object = layout_prog_edit_create(live->layout);
             edje_object_file_set(live->object, project->dev, style->full_group_name);
             evas_object_freeze_events_set(live->object, true);
          }
        container_content_set(live->live_view, live->object);
        live_view_theme_update(live, project);
        live_view_property_style_set(live->property, live->object, style, "layout");
     }

   evas_object_show(live->live_view);
   evas_object_show(live->object);

   elm_layout_signal_emit(live->layout, "live_view,show", "eflete");

   evas_object_geometry_get(live->live_view, NULL, NULL, &x, &y);
   edje_object_part_drag_value_set(elm_layout_edje_get(live->layout),
                                   "bottom_pad", x, y);

   return ret;
}

Eina_Bool
live_view_widget_style_unset(Live_View *live)
{
   if ((!live) || (!live->object)) return false;
   evas_object_hide(live->live_view);
   elm_layout_signal_emit(live->layout, "live_view,hide", "eflete");
   container_content_unset(live->live_view);
   live_view_property_style_unset(live->property);
   if (!live_widget_del(live->object))
     evas_object_del(live->object);
   live->object = NULL;
   return true;
}

Eina_Bool
live_view_theme_update(Live_View *live, Project *project)
{
   if ((!live) || (!project) || (!live->object)) return false;
   if ((project->current_style) && (project->current_style->__type == LAYOUT))
     {
        elm_layout_file_set(live->object, project->dev,
                            project->current_style->full_group_name);
        return true;
     }

   if ((!project->current_style) || (!project->current_style->parts))
     {
        WARN("Could'nt apply the empty style to live view.");
        live_view_widget_style_unset(live);
        return false;
     }
   Elm_Theme *theme = elm_theme_new();
   elm_theme_set(theme, project->dev);
   elm_object_theme_set(live->object, theme);
   elm_theme_free(theme);

   return true;
}

Eina_Bool
live_view_free(Live_View *live)
{
   if (live) live_view_widget_style_unset(live);
   else return false;

   free(live);
   live = NULL;
   return true;
}
