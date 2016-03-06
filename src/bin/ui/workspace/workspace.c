/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "workspace.h"
#include "main_window.h"
#include "highlight.h"
#include "groupview.h"
#include "container.h"
#include "eflete.h"
#include "group_navigator.h"
#include "history.h"
#include "demo.h"
#include "demo_group.h"
#include "project_manager.h"
#include "change.h"

#define WORKSPACE_DATA "workspace_data"

#define PANES_RIGHT_SIZE_MIN 225

typedef enum
{
   MODE_NORMAL = 1,
   /* MODE_SEPARATE, */
   /* MODE_ANIMATOR, */
   /* MODE_CODE, */
   MODE_DEMO
} Workspace_Mode;

struct _Ruler {
   Evas_Object *obj;
   Ewe_Ruler_Marker *pointer;
   Ewe_Ruler_Scale *scale_rel;
};
typedef struct _Ruler Ruler;

struct _Scroll_Area {
   Evas_Object *layout;  /* layout for rulers and scroller */
   Evas_Object *scroller;
   Evas_Object *bg;
   Bg_Preview bg_preview;
   Evas_Object *container;
   Evas_Object *content; /* for normal mode - groupview, for demo - elm widget */
   Ruler ruler_v;
   Ruler ruler_h;
   Evas_Object *hilight;
   Evas_Object *clipper;
};
typedef struct _Scroll_Area Scroll_Area;

struct _Workspace_Data
{
   Evas_Object *panes; /* equal to all workspace, this object returned in workspace_add */
   Evas_Object *group_navi;
   Evas_Object *demo_navi;
   struct {
      Evas_Object *layout;
      Evas_Object *obj;
      struct {
         Evas_Object *normal;
         Evas_Object *demo;
      } mode_switcher;
      struct {
         Evas_Object *black;
         Evas_Object *tile;
         Evas_Object *white;
      } bg_switcher;
   } toolbar;
   Evas_Object *panes_h; /* for set subobject like code, sequance etc */

   Scroll_Area normal;
   Scroll_Area demo;
   struct {
      Evas_Object *obj;
      double size;
   } code;
   Workspace_Mode mode;
   Group *group;
};

typedef struct _Workspace_Data Workspace_Data;

Eina_Bool
workspace_zoom_factor_set(Evas_Object *obj __UNUSED__, double factor __UNUSED__)
{
   return false;
}

double
workspace_zoom_factor_get(Evas_Object *obj __UNUSED__)
{
   return -1.0;
}

Eina_Bool
workspace_highlight_unset(Evas_Object *obj __UNUSED__)
{
   return false;
}

TODO("remove after property refactor!!! HIGH LEVEL");
Eina_Bool
workspace_active_demo_mode_get(Evas_Object *obj __UNUSED__)
{
   return false;
}

static void
_rulers_pointer_move(void *data,
                     Evas *e,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Scroll_Area *area = data;
   Evas_Coord x, y, ruler_h_x, ruler_v_y;

   evas_pointer_output_xy_get(e, &x, &y);
   evas_object_geometry_get(area->ruler_h.obj, &ruler_h_x, NULL, NULL, NULL);
   evas_object_geometry_get(area->ruler_v.obj, NULL, &ruler_v_y, NULL, NULL);
   ewe_ruler_marker_absolute_set(area->ruler_h.obj, area->ruler_h.pointer, x - ruler_h_x);
   ewe_ruler_marker_absolute_set(area->ruler_v.obj, area->ruler_v.pointer, y - ruler_v_y);
}

static void
_job_workspace_del(void *data)
{
   assert(data != NULL);

   Workspace_Data *wd = data;
   gm_group_edit_object_unload(wd->group);

   free(wd);
}

static void
_workspace_del(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   ecore_job_add(_job_workspace_del, data);
}

static Evas_Object *
_radio_switcher_add(Workspace_Data *wd,
                    const char *style,
                    Evas_Smart_Cb func,
                    int state_value,
                    Evas_Object *group)
{
   Evas_Object *radio;

   radio = elm_radio_add(wd->panes);
   elm_object_style_set(radio, style);
   elm_radio_state_value_set(radio, state_value);
   evas_object_smart_callback_add(radio, "changed", func, wd);
   elm_radio_group_add(radio, group);

   return radio;
}

static void
_ruler_add(Evas_Object *parent, Ruler *ruler, Eina_Bool scale_rel)
{
   ruler->obj = ewe_ruler_add(parent);
   ruler->pointer = ewe_ruler_marker_add(ruler->obj, "pointer");

   if (scale_rel)
     {
      ruler->scale_rel = ewe_ruler_scale_add(ruler->obj, "relative");
      ewe_ruler_format_set(ruler->obj, ruler->scale_rel, "%.1f");
      //ewe_ruler_scale_visible_set(ruler->obj, ruler->scale_rel, false);
      ewe_ruler_value_step_set(ruler->obj, ruler->scale_rel, 0.5);
     }
}

static void
_container_changed(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Scroll_Area *area = data;
   Container_Geom *geom = event_info;
   Evas_Coord x, y;

   evas_object_geometry_get(area->ruler_h.obj, &x, NULL, NULL, NULL);
   evas_object_geometry_get(area->ruler_v.obj, NULL, &y, NULL, NULL);

   /* shift the abs scale zero mark */
   ewe_ruler_zero_offset_set(area->ruler_h.obj, NULL, geom->x - x);
   ewe_ruler_zero_offset_set(area->ruler_v.obj, NULL, geom->y - y);

   /* shift the rel scale zero mark */
   if (area->ruler_h.scale_rel)
     {
        ewe_ruler_zero_offset_set(area->ruler_h.obj, area->ruler_h.scale_rel, geom->x - x);
        ewe_ruler_step_set(area->ruler_h.obj, area->ruler_h.scale_rel, geom->w);
     }
   if (area->ruler_v.scale_rel)
     {
        ewe_ruler_zero_offset_set(area->ruler_v.obj, area->ruler_v.scale_rel, geom->y - y);
        ewe_ruler_step_set(area->ruler_v.obj, area->ruler_v.scale_rel, geom->h);
     }
}

static void
_scroll_area_add(Workspace_Data *wd, Scroll_Area *area, Eina_Bool scale_rel)
{
   area->bg_preview = BG_PREVIEW_TILE;

   area->layout = elm_layout_add(wd->panes);
   elm_layout_theme_set(area->layout, "layout", "workspace", "scroller");

   _ruler_add(area->layout, &area->ruler_h, scale_rel);
   elm_layout_content_set(area->layout, "elm.swallow.ruler_h", area->ruler_h.obj);
   _ruler_add(area->layout, &area->ruler_v, scale_rel);
   ewe_ruler_horizontal_set(area->ruler_v.obj, false);
   elm_layout_content_set(area->layout, "elm.swallow.ruler_v", area->ruler_v.obj);

   /* create scroller for normal mode and set bg */
   area->scroller = elm_scroller_add(area->layout);
   evas_object_event_callback_add(area->scroller, EVAS_CALLBACK_MOUSE_MOVE, _rulers_pointer_move, area);
   elm_layout_content_set(area->layout, "elm.swallow.scroller", area->scroller);
   area->bg = elm_layout_add(area->layout);
   elm_layout_theme_set(area->bg, "layout", "workspace", "bg");
   elm_object_part_content_set(area->scroller, "elm.swallow.background", area->bg);

   area->container = container_add(area->scroller);
   container_handler_size_set(area->container, 8, 8);
   evas_object_smart_callback_add(area->container, "container,changed", _container_changed, area);
   elm_object_content_set(area->scroller, area->container);

   wd->normal.clipper = evas_object_rectangle_add(wd->normal.layout);
   elm_object_part_content_set(area->scroller, "elm.swallow.overlay", wd->normal.clipper);
   wd->normal.hilight = highlight_add(wd->normal.layout);
   evas_object_color_set(wd->normal.hilight, HIGHLIGHT_COLOR);
   evas_object_clip_set(wd->normal.hilight, wd->normal.clipper);
}

static void
_mode_cb(void *data,
         Evas_Object *obj,
         void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Workspace_Mode mode;
   Evas_Object *content;

   mode = elm_radio_state_value_get(obj);
   if (mode == wd->mode) return;

   wd->mode = mode;
   content = elm_object_part_content_unset(wd->panes_h, "left");
   evas_object_hide(content);
   switch (wd->mode)
     {
      case MODE_NORMAL:
         elm_object_part_content_set(wd->panes_h, "left", wd->normal.layout);
         evas_object_show(wd->normal.layout);
         elm_radio_value_set(wd->toolbar.bg_switcher.white, wd->normal.bg_preview);
         break;
      case MODE_DEMO:
         if (!wd->demo.layout) _scroll_area_add(wd, &wd->demo, false);
         elm_object_part_content_set(wd->panes_h, "left", wd->demo.layout);
         evas_object_show(wd->demo.layout);
         elm_radio_value_set(wd->toolbar.bg_switcher.white, wd->demo.bg_preview);
         break;
     }
}

static void
_bg_cb(void *data,
       Evas_Object *obj,
       void *event_info __UNUSED__)
{
   Bg_Preview bg_mode;
   Workspace_Data *wd = data;
   Scroll_Area *area;
   const char *signal = NULL;

   bg_mode = elm_radio_value_get(obj);
   if (MODE_NORMAL == wd->mode)
     area = &wd->normal;
   else
     area = &wd->demo;
   switch (bg_mode)
     {
      case BG_PREVIEW_WHITE:
         signal = "elm,bg,white";
         container_style_set(area->container, "default");
         break;
      case BG_PREVIEW_TILE:
         signal = "elm,bg,tile";
         container_style_set(area->container, "default");
         break;
      case BG_PREVIEW_BLACK:
         signal = "elm,bg,black";
         container_style_set(area->container, "white");
         break;
     }
   area->bg_preview = bg_mode;
   elm_layout_signal_emit(area->bg, signal, "eflete");

}

static void
_part_select(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Workspace_Data *wd = data;
   Part *part = event_info;
   Evas_Object *part_obj;

   assert(MODE_NORMAL == wd->mode);

   if (part)
     {
        part_obj = groupview_edit_object_part_draw_get(wd->normal.content, part->name);
        highlight_object_follow(wd->normal.hilight, part_obj);
        evas_object_show(wd->normal.hilight);
     }
   else
     {
        highlight_object_unfollow(wd->normal.hilight);
        evas_object_hide(wd->normal.hilight);
     }
}

static void
_groupview_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Workspace_Data *wd = data;
   Part *part = event_info;

   assert(MODE_NORMAL == wd->mode);

   group_navigator_part_select(wd->group_navi, part ? part : NULL);
}

Evas_Object *
workspace_add(Evas_Object *parent, Group *group)
{
   Workspace_Data *wd;
   Elm_Object_Item *tb_it;

   assert(parent != NULL);
   assert(group != NULL);

   /* load the editable object */
   gm_group_edit_object_load(ap.project, group, evas_object_evas_get(ap.win));
   edje_object_animation_set(group->edit_object, false);

   wd = mem_calloc(1, sizeof(Workspace_Data));
   wd->panes = elm_panes_add(parent);
   elm_panes_content_right_min_size_set(wd->panes, PANES_RIGHT_SIZE_MIN);
   elm_panes_content_right_size_set(wd->panes, 0); /* set the default min size */

   wd->toolbar.layout = elm_layout_add(wd->panes);
   elm_layout_theme_set(wd->toolbar.layout, "layout", "workspace", "toolbar");
   elm_object_part_content_set(wd->panes, "left", wd->toolbar.layout);

   /* add toolbar */
   wd->toolbar.obj = elm_toolbar_add(wd->panes);
   elm_toolbar_align_set(wd->toolbar.obj, 0.0);
   elm_toolbar_shrink_mode_set(wd->toolbar.obj, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(wd->toolbar.obj, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_layout_content_set(wd->toolbar.layout, "elm.swallow.toolbar", wd->toolbar.obj);

   /* add to toolbar modes switcher */
   wd->toolbar.mode_switcher.normal = _radio_switcher_add(wd, "radio_normal", _mode_cb, MODE_NORMAL, NULL);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.mode_switcher.normal);
   wd->toolbar.mode_switcher.demo = _radio_switcher_add(wd, "radio_demo", _mode_cb, MODE_DEMO, wd->toolbar.mode_switcher.normal);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.mode_switcher.demo);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_toolbar_item_separator_set(tb_it, true);
   elm_radio_value_set(wd->toolbar.mode_switcher.normal, 1);

   /* add to toolbar bg switcher */
   wd->toolbar.bg_switcher.white = _radio_switcher_add(wd, "bg_white", _bg_cb, BG_PREVIEW_WHITE, NULL);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.bg_switcher.white);
   wd->toolbar.bg_switcher.tile = _radio_switcher_add(wd, "bg_tile", _bg_cb, BG_PREVIEW_TILE, wd->toolbar.bg_switcher.white);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.bg_switcher.tile);
   wd->toolbar.bg_switcher.black = _radio_switcher_add(wd, "bg_black", _bg_cb, BG_PREVIEW_BLACK, wd->toolbar.bg_switcher.white);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.bg_switcher.black);
   elm_radio_value_set(wd->toolbar.bg_switcher.white, BG_PREVIEW_TILE);

   wd->panes_h = elm_panes_add(wd->toolbar.layout);
   elm_panes_horizontal_set(wd->panes_h, true);
   elm_panes_content_right_size_set(wd->panes_h, 0); /* set the default min size */
   elm_layout_content_set(wd->toolbar.layout, NULL, wd->panes_h);

   _scroll_area_add(wd, &wd->normal, true);
   elm_object_part_content_set(wd->panes_h, "left", wd->normal.layout);
   wd->normal.content = groupview_add(wd->normal.scroller, group);
   container_content_set(wd->normal.container, wd->normal.content);
   container_protrusion_func_set(wd->normal.container, groupview_protrusion_get);
   evas_object_smart_callback_add(wd->normal.content, SIGNAL_GROUPVIEW_CLICKED, _groupview_clicked, wd);

   wd->group_navi = group_navigator_add(wd->panes, group);
   elm_object_part_content_set(wd->panes, "right", wd->group_navi);
   evas_object_smart_callback_add(wd->group_navi, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, _part_select, wd);

   wd->group = group;
   wd->mode = MODE_NORMAL;

   evas_object_data_set(wd->panes, WORKSPACE_DATA, wd);
   evas_object_event_callback_add(wd->panes, EVAS_CALLBACK_DEL, _workspace_del, wd);

   return wd->panes;
}

void
workspace_group_navigator_update_part(Evas_Object *obj __UNUSED__, Part *part __UNUSED__)
{
}

Eina_Bool
workspace_edit_object_recalc(Evas_Object *obj __UNUSED__)
{
   return false;
}

void
workspace_groupview_hard_update(Evas_Object *obj __UNUSED__)
{
}

void
workspace_groupview_soft_update(Evas_Object *obj __UNUSED__)
{
}

Eina_Bool
workspace_highlight_align_visible_set(Evas_Object *obj __UNUSED__, Eina_Bool flag __UNUSED__)
{
   return false;
}

Eina_Bool
workspace_highlight_align_visible_get(Evas_Object *obj __UNUSED__)
{
   return false;
}

Eina_Bool
workspace_object_area_visible_set(Evas_Object *obj __UNUSED__, Eina_Bool flag __UNUSED__)
{
   return false;
}

Eina_Bool
workspace_object_area_visible_get(Evas_Object *obj __UNUSED__)
{
   return false;
}

void
workspace_part_add(Evas_Object *obj __UNUSED__, Eina_Stringshare *part_name __UNUSED__)
{
}

void
workspace_part_del(Evas_Object *obj __UNUSED__, Eina_Stringshare *part_name __UNUSED__)
{
}

void
workspace_part_item_add(Evas_Object *obj __UNUSED__,
                        Eina_Stringshare *part_name __UNUSED__,
                        Eina_Stringshare *item_name __UNUSED__)
{
}

void
workspace_part_item_del(Evas_Object *obj __UNUSED__,
                        Eina_Stringshare *part_name __UNUSED__,
                        Eina_Stringshare *item_name __UNUSED__)
{
}

void
workspace_part_state_add(Evas_Object *obj __UNUSED__,
                         Eina_Stringshare *part_name __UNUSED__,
                         Eina_Stringshare *state_name __UNUSED__)
{
}

void
workspace_part_state_select(Evas_Object *obj __UNUSED__,
                            Eina_Stringshare *part_name __UNUSED__,
                            Eina_Stringshare *state_name __UNUSED__)
{
}

void
workspace_part_state_del(Evas_Object *obj __UNUSED__,
                         Eina_Stringshare *part_name __UNUSED__,
                         Eina_Stringshare *state_name __UNUSED__)
{
}

void
workspace_part_restack(Evas_Object *obj __UNUSED__,
                       Eina_Stringshare *part_name __UNUSED__,
                       Eina_Stringshare *relative_part_name __UNUSED__)
{
}

void
workspace_part_item_restack(Evas_Object *obj __UNUSED__,
                            Eina_Stringshare *part_name __UNUSED__,
                            Eina_Stringshare *part_item_name __UNUSED__,
                            Eina_Stringshare *relative_part_item_name __UNUSED__)
{
}

void
workspace_program_add(Evas_Object *obj __UNUSED__, Eina_Stringshare *program_name __UNUSED__)
{
}

void
workspace_program_del(Evas_Object *obj __UNUSED__, Eina_Stringshare *program_name __UNUSED__)
{
}
