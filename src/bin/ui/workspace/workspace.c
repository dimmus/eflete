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
#include "groupedit.h"
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
   MODE_DEMO,
   MODE_LAST
} Workspace_Mode;

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
         Evas_Object *tile;
         Evas_Object *black;
         Evas_Object *white;
      } bg_switcher;
   } toolbar;
   Evas_Object *panes_h; /* for set subobject like code, sequance etc */
   struct {
      Evas_Object *obj;
      Ewe_Ruler_Marker *pointer;
      Ewe_Ruler_Scale *scale_rel;
   } ruler_v, ruler_h;
   struct {
      Evas_Object *layout;  /* layout for rulers and scroller */
      Evas_Object *scroller;
      Evas_Object *container;
      Evas_Object *bg;
      Evas_Object *content; /* for normal mode - groupview, for demo - elm widget */
   } normal, demo;
   struct {
      Evas_Object *obj;
      double size;
   };
   Workspace_Mode mode;
   Group *group;
};

typedef struct _Workspace_Data Workspace_Data;

#define RULER_ADD(RUL, SCAL, POINT) \
   RUL = ewe_ruler_add(wd->normal.layout); \
   SCAL = ewe_ruler_scale_add(RUL, "relative"); \
   ewe_ruler_format_set(RUL, SCAL, "%.1f"); \
   ewe_ruler_scale_visible_set(RUL, SCAL, false); \
   ewe_ruler_value_step_set(RUL, SCAL, 0.5); \
   POINT = ewe_ruler_marker_add(RUL, "pointer");

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
   Workspace_Data *wd = data;
   Evas_Coord x, y, ruler_h_x, ruler_v_y;

   evas_pointer_output_xy_get(e, &x, &y);
   evas_object_geometry_get(wd->ruler_h.obj, &ruler_h_x, NULL, NULL, NULL);
   evas_object_geometry_get(wd->ruler_v.obj, NULL, &ruler_v_y, NULL, NULL);
   ewe_ruler_marker_absolute_set(wd->ruler_h.obj, wd->ruler_h.pointer, x - ruler_h_x);
   ewe_ruler_marker_absolute_set(wd->ruler_v.obj, wd->ruler_v.pointer, y - ruler_v_y);
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

static void
_mode_cb(void *data,
         Evas_Object *obj,
         void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;

   wd->mode = elm_radio_state_value_get(obj);
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



   wd->panes_h = elm_panes_add(wd->toolbar.layout);
   elm_panes_horizontal_set(wd->panes_h, true);
   elm_panes_content_right_size_set(wd->panes_h, 0); /* set the default min size */
   elm_layout_content_set(wd->toolbar.layout, NULL, wd->panes_h);

   wd->normal.layout = elm_layout_add(wd->panes_h);
   elm_layout_theme_set(wd->normal.layout, "layout", "workspace", "scroller");
   elm_object_part_content_set(wd->panes_h, "left", wd->normal.layout);

   RULER_ADD(wd->ruler_h.obj, wd->ruler_h.scale_rel, wd->ruler_h.pointer)
   elm_layout_content_set(wd->normal.layout, "elm.swallow.ruler_h", wd->ruler_h.obj);
   RULER_ADD(wd->ruler_v.obj, wd->ruler_v.scale_rel, wd->ruler_v.pointer)
   ewe_ruler_horizontal_set(wd->ruler_v.obj, false);
   elm_layout_content_set(wd->normal.layout, "elm.swallow.ruler_v", wd->ruler_v.obj);

   /* create scroller for normal mode and set bg */
   wd->normal.scroller = elm_scroller_add(wd->normal.layout);
   evas_object_event_callback_add(wd->normal.scroller, EVAS_CALLBACK_MOUSE_MOVE, _rulers_pointer_move, wd);
   elm_layout_content_set(wd->normal.layout, "elm.swallow.scroller", wd->normal.scroller);
   wd->normal.bg = elm_layout_add(wd->normal.layout);
   elm_layout_theme_set(wd->normal.bg, "layout", "workspace", "bg");
   elm_object_part_content_set(wd->normal.scroller, "elm.swallow.background", wd->normal.bg);

   wd->group = group;

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
