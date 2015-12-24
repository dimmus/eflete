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

static void
_live_view_load_object(Live_View *live, Group *group)
{
   Eina_Bool using_layout = false;

   assert(ap.project != NULL);
   assert(live != NULL);
   assert(group != NULL);

   /* check version */
   Eina_Stringshare *version = edje_edit_data_value_get(ap.project->global_object, "version");
   if ((!version) || (strcmp(version, "110")))
     {
        NOTIFY_INFO(3, _("Outdated version of file. Using fallback to layout"));
        using_layout = true;
     }
   eina_stringshare_del(version);

   /* If loaded gorup is widget, then fields widget/class/style would be filled */
   if ((group->widget) && (!live->in_prog_edit))
     {
        live->object = live_widget_create(group, live->layout);

        if (!live->object)
          {
             NOTIFY_INFO(3, _("Widget live view isn't implemented yet. Using fallback to layout"));
             using_layout = true;
          }
        else
          {
             live_view_theme_update(live->block);
          }
     }
   else
     {
        using_layout = true;
     }

   if (using_layout)
     {
        if (!live->in_prog_edit)
          {
             live->object = layout_custom_create(live->layout, NULL);
          }
        else
          {
             live->object = layout_prog_edit_create(live->layout);
             evas_object_freeze_events_set(live->object, true);
          }
        if (!edje_object_mmap_set(live->object, ap.project->mmap_file,
                                  group->name))
          {
             evas_object_del(live->object);
             live->object = elm_label_add(live->layout);
             elm_object_text_set(live->object, _("Failed to load live view object"));
          }
        live_view_theme_update(live->block);
     }
}

static void
_live_view_delete(void *data __UNUSED__,
                  Evas *e __UNUSED__,
                  Evas_Object *obj,
                  void *event_info __UNUSED__)
{
   assert(obj != NULL);
   Live_View *live = evas_object_data_get(obj, "live_view_structure");
   assert(live != NULL);

   if (!live_widget_del(live->object))
     evas_object_del(live->object);

   live_view_property_style_unset(live->property);
   live_view_property_free(live->property);

   free(live);
   live = NULL;
}

Evas_Object *
live_view_add(Evas_Object *parent, Eina_Bool in_prog_edit, Group *group)
{
   Live_View *live;
   Evas_Object *bg;
   Evas_Object *block;

   assert(parent != NULL);

   /* Create the very top object of live view */
   block = elm_layout_add(parent);
   elm_layout_theme_set(block, "layout", "block", "default");
   elm_layout_text_set(block, "elm.text", _("Live View"));
   evas_object_show(block);

   /* Fill up live view structure */
   live = mem_calloc(1, sizeof(Live_View));
   live->in_prog_edit = in_prog_edit;
   live->parent = parent;
   live->group = group;
   live->block = block;

   live->panel = elm_panes_add(live->block);
   evas_object_size_hint_weight_set(live->panel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(live->panel, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_right_min_size_set(live->panel, 225);
   elm_panes_content_right_size_set(live->panel, 0); /* default is min size */
   elm_object_part_content_set(live->block, "elm.swallow.content", live->panel);
   evas_object_show(live->panel);
   /* Create main layout of entire live view */
   live->layout = elm_layout_add(live->block);
   elm_layout_theme_set(live->layout, "layout", "live_view", "toolbar");
   elm_object_part_content_set(live->panel, "left", live->layout);
   bg = elm_bg_add(live->layout);
   IMAGE_ADD_NEW(live->layout, bg, "bg", "tile");
   evas_object_show(bg);

   /* Create container,  will hold live object inside (resizable container).
      And create panes for having both live view property and live object. */
   live->live_view = container_add(parent);
   elm_object_part_content_set(live->layout, SWALLOW_CONTENT, live->live_view);
   elm_object_part_content_set(live->layout, SWALLOW_BG, bg);

   /* save structure inside of an object */
   evas_object_data_set(live->block, "live_view_structure", live);
   _live_view_load_object(live, group);

   TODO("reapply swallows/texts")
   evas_object_show(live->live_view);
   evas_object_show(live->object);
   TODO("reapply comtainer size and position")
   container_content_set(live->live_view, live->object);

   live->property = live_view_property_add(live->block, group, in_prog_edit);
   elm_object_part_content_set(live->panel, "right", live->property);

   evas_object_smart_callback_add(live->property, "bg,changed", _change_bg_cb,
                                  live->layout);
   evas_object_event_callback_add(live->block, EVAS_CALLBACK_DEL,
                                  _live_view_delete, NULL);

   return live->block;
}

Eina_Bool
live_view_theme_update(Evas_Object *object)
{
   Eina_Stringshare *path;

   assert(object != NULL);
   assert(ap.project != NULL);

   Live_View *live = evas_object_data_get(object, "live_view_structure");
   assert(live != NULL);
   assert(live->object != NULL);

#ifdef HAVE_ENVENTOR
   if ((ap.enventor_mode)
     path = eina_stringshare_printf("%s/tmp.edj", ap.project->develop_path);
   else
#endif /* HAVE_ENVENTOR */
     path = eina_stringshare_add(ap.project->dev);

   if (!live->group->widget)
     {
        edje_object_mmap_set(live->object, ap.project->mmap_file,
                             live->group->name);
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
live_view_part_add(Evas_Object *object, Part_ *part)
{
   assert(object != NULL);
   assert(ap.project != NULL);

   Live_View *live = evas_object_data_get(object, "live_view_structure");

   assert(live != NULL);
   assert(live->object != NULL);
   assert(part != NULL);
   return live_view_property_part_add(live->property, part);
}

Eina_Bool
live_view_part_del(Evas_Object *object, Part_ *part)
{
   assert(object != NULL);
   assert(ap.project != NULL);

   Live_View *live = evas_object_data_get(object, "live_view_structure");

   assert(live != NULL);
   assert(live->object != NULL);
   assert(part != NULL);
   assert(part != NULL);
   return live_view_property_part_del(live->property, part);
}

/*
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
*/
Evas_Object *
live_view_live_object_get(Evas_Object *object)
{
   assert(object != NULL);
   assert(ap.project != NULL);

   Live_View *live = evas_object_data_get(object, "live_view_structure");

   return live->object;
}
