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

static void
_change_bg_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info)
{
   Evas_Object *live_layout = (Evas_Object *)data;
   Evas_Object *bg = NULL;

   assert(live_layout != NULL);

   int state = elm_radio_state_value_get((Evas_Object *)event_info);
   switch (state)
     {
      case 0:
        {
           IMAGE_ADD_NEW(live_layout, bg, "bg", "tile");
        }
      break;
      case 1:
        {
           bg = evas_object_rectangle_add(live_layout);
           evas_object_color_set(bg, 0, 0, 0, 255);
           evas_object_show(bg);
        }
      break;
      case 2:
        {
           bg = evas_object_rectangle_add(live_layout);
           evas_object_color_set(bg, 255, 255, 255, 255);
           evas_object_show(bg);
        }
      break;
      default:
        {
           ERR("Wrong state");
           abort();
        }
      break;
     }
   elm_object_part_content_set(live_layout, SWALLOW_BG, bg);
}

Live_View *
live_view_add(Evas_Object *parent, Eina_Bool in_prog_edit)
{
   Live_View *live;
   Evas_Object *bg;

   assert(parent != NULL);

   live = mem_calloc(1, sizeof(Live_View));

   live->in_prog_edit = in_prog_edit;

   live->parent = parent;
   live->layout = elm_layout_add(parent);
   elm_layout_theme_set(live->layout, "layout", "live_view", "toolbar");
   bg = elm_bg_add(live->layout);
   IMAGE_ADD_NEW(live->layout, bg, "bg", "tile");
   evas_object_show(bg);

   live->live_view = container_add(parent);
   live->panel = elm_panel_add(parent);
   live->property = live_view_property_add(live->panel, in_prog_edit);
   elm_object_content_set(live->panel, live->property);
   evas_object_smart_callback_add(live->property, "bg,changed", _change_bg_cb,
                                  live->layout);
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
   Eina_Stringshare *widget;
   Eina_Bool ret = true;
   Eina_Bool first_load;
   Eina_Bool using_layout = false;
   int x, y;

   assert(live != NULL);
   assert(project != NULL);
   assert(style != NULL);

   first_load = live->object == NULL; /* fallback notifications should pop-up only on style load*/

   Eina_Stringshare *version = edje_edit_data_value_get(style->obj, "version");
   if ((!version) || (strcmp(version, "110")))
     {
        if (first_load)
          NOTIFY_INFO(3, _("Outdated version of file. Using fallback to layout"));
        using_layout = true;
     }
   eina_stringshare_del(version);

   using_layout = using_layout || (style->__type == LAYOUT) || live->in_prog_edit;

   /* sadly, but we should delete live object to reapply style */
   live_view_widget_style_unset(live);

   if (!using_layout)
     {
        standard_widget_name_parse(style->full_group_name, &widget, NULL, NULL);

        live->object = live_widget_create(widget, style, live->layout);

        if (!live->object)
          {
             if (first_load)
               NOTIFY_INFO(3, _("Widget live view isn't implemented yet. Using fallback to layout"));
             using_layout = true;
          }
        else
          {
             live_view_theme_update(live, project);
             live_view_property_style_set(live->property, live->object, style, widget, live->parent);
          }

        eina_stringshare_del(widget);
     }
   if (using_layout)
     {
        if (!live->in_prog_edit)
          {
             live->object = layout_custom_create(live->layout);
          }
        else
          {
             live->object = layout_prog_edit_create(live->layout);

             evas_object_freeze_events_set(live->object, true);
          }
        if (!edje_object_mmap_set(live->object, project->mmap_file,
                                  style->full_group_name))
          {
             evas_object_del(live->object);
             live->object = elm_label_add(live->layout);
             elm_object_text_set(live->object, _("Failed to load live view object"));
             ret = false;
          }
        live_view_theme_update(live, project);
        live_view_property_style_set(live->property, live->object, style, "edje", live->parent);
     }
   TODO("reapply swallows/texts")
   evas_object_show(live->live_view);
   evas_object_show(live->object);
   TODO("reapply comtainer size and position")
   container_content_set(live->live_view, live->object);

   elm_layout_signal_emit(live->layout, "live_view,show", "eflete");

   evas_object_geometry_get(live->live_view, NULL, NULL, &x, &y);
   edje_object_part_drag_value_set(elm_layout_edje_get(live->layout),
                                   "bottom_pad", x, y);

   return ret;
}

Eina_Bool
live_view_widget_style_unset(Live_View *live)
{
   assert(live != NULL);

   if (!live->object) return false;

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
   Eina_Stringshare *path;

   assert(live != NULL);
   assert(project != NULL);
   assert(live->object != NULL);

#ifdef HAVE_ENVENTOR
   if ((app_data_get())->enventor_mode)
     path = eina_stringshare_printf("%s/tmp.edj", project->develop_path);
   else
#endif /* HAVE_ENVENTOR */
     path = eina_stringshare_add(project->dev);

   if (project->current_style->__type == LAYOUT)
     {
        eina_file_map_free(project->mmap_file, live->object);
        edje_object_mmap_set(live->object, project->mmap_file,
                             project->current_style->full_group_name);
        edje_object_calc_force(live->object);
        eina_stringshare_del(path);
        return true;
     }

   Elm_Theme *theme = elm_theme_new();
   elm_theme_set(theme, path);
   if (!live->in_prog_edit)
     elm_object_theme_set(live->object, theme);
   elm_theme_free(theme);

   eina_stringshare_del(path);
   return true;
}

Eina_Bool
live_view_free(Live_View *live)
{
   assert(live != NULL);

   live_view_widget_style_unset(live);
   live_view_property_free(live->property);

   free(live);
   live = NULL;
   return true;
}

TODO("We need implementation here!~~ ")
Eina_Bool
live_view_part_add(Live_View *live, Part *part)
{
   assert(live != NULL);
   assert(part != NULL);
   return live_view_property_part_add(live->property, part);
}

Eina_Bool
live_view_part_del(Live_View *live, Part *part)
{
   assert(live != NULL);
   assert(part != NULL);
   return live_view_property_part_del(live->property, part);
}

Eina_Bool
live_view_part_rename(Live_View *live, Part *part, Eina_Stringshare *new_name)
{
   assert(live != NULL);
   assert(part != NULL);
   assert(new_name != NULL);
   return live_view_property_part_rename(live->property, part, new_name);
}

Eina_Bool
live_view_part_restack_above(Live_View *live, Part *part_move, Part *part_above)
{
   assert(live != NULL);
   assert(part_move != NULL);
   assert(part_above != NULL);
   return live_view_property_part_restack_above(live->property, part_move, part_above);
}

Eina_Bool
live_view_part_restack_below(Live_View *live, Part *part_move)
{
   assert(live != NULL);
   assert(part_move != NULL);
   return live_view_property_part_restack_below(live->property, part_move);
}
