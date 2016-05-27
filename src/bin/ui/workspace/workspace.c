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
#include "groupview.h"
#include "container.h"
#include "eflete.h"
#include "group_navigator.h"
#include "history.h"
#include "history_ui.h"
#include "demo.h"
#include "project_manager.h"
#include "change.h"
#include "syntax_color.h"

#define WORKSPACE_DATA "workspace_data"

#define PANES_RIGHT_SIZE_MIN 225

#define WS_DATA_GET(OBJ) \
   assert(OBJ != NULL); \
   Workspace_Data *wd = evas_object_data_get(OBJ, WORKSPACE_DATA); \
   assert(wd != NULL);

static int zoom_values[] = { 20, 50, 100, 200, 500, 0 };

/* As the workspace can manage the one drag in one time, and only one workspace
 * viewed, we can use the static Change for all workspace */
static Change *change;
static Evas_Coord part_w, part_h;
static double part_align_x, part_align_y;

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
   Eina_Bool rulers_visible : 1;
};
typedef struct _Scroll_Area Scroll_Area;

struct _Workspace_Data
{
   Evas_Object *layout; /* equal to whole workspace, this object returned in workspace_add */
   Evas_Object *group_navi;
   Evas_Object *demo_navi;
   struct {
      Evas_Object *obj;
      Evas_Object *history;
      struct {
         Evas_Object *fit;
         Evas_Object *cmb_zoom;
         Evas_Object *slider;
      } zoom;
#if !HAVE_TIZEN
      struct {
         Evas_Object *normal;
         Evas_Object *code;
         Evas_Object *demo;
      } mode_switcher;
#else
      Evas_Object *mode_switcher;
#endif
      struct {
         Evas_Object *spinner_w;
         Evas_Object *check_chain;
         Evas_Object *spinner_h;
         Evas_Object *check_lock;
      } container_sizer;
      struct {
         Evas_Object *black;
         Evas_Object *tile;
         Evas_Object *white;
      } bg_switcher;
#if HAVE_TIZEN
      Evas_Object *libraries_switcher;
#endif
   } toolbar;
   Evas_Object *panes_h; /* for set subobject like code, sequance etc */

   struct {
      Evas_Object *obj;
      Evas_Object *scale_abs;
      Evas_Object *scale_rel;
      Evas_Object *scale_both;
   } menu;
   Scroll_Area normal;
   Scroll_Area demo;
   struct {
      Evas_Object *obj;
      color_data *color_data;
      double size;
   } code;
   Workspace_Mode mode;
   Group *group;
   double zoom_factor;
};

typedef struct _Workspace_Data Workspace_Data;

Eina_Bool
workspace_highlight_unset(Evas_Object *obj __UNUSED__)
{
   return false;
}

TODO("remove after property refactor!!! HIGH LEVEL");
Eina_Bool
workspace_active_demo_mode_get(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode == MODE_DEMO)
     return true;
   return false;
}

const char *
_group_code_get(Workspace_Data *wd)
{
   Eina_Stringshare *code;
   char *str;
   const char *colored;

   code = edje_edit_source_generate(wd->group->edit_object);
   str = elm_entry_utf8_to_markup(code);
   colored = color_apply(wd->code.color_data, str, strlen(str), NULL, NULL);

   free(str);
   eina_stringshare_del(code);

   return colored;
}

static void
_container_hints_set(Workspace_Data *wd)
{
   Evas_Coord min_w, max_w, min_h, max_h;

   min_w = edje_edit_group_min_w_get(wd->group->edit_object);
   min_h = edje_edit_group_min_h_get(wd->group->edit_object);
   max_w = edje_edit_group_max_w_get(wd->group->edit_object);
   max_h = edje_edit_group_max_h_get(wd->group->edit_object);

   container_min_size_set(wd->normal.container, min_w, min_h);
   container_max_size_set(wd->normal.container, (max_w == 0) ? -1 : max_w, (max_h == 0) ? -1 : max_h);
}

/* return the current scroll area, accourdingly to selected mode */
static Scroll_Area *
_scroll_area_get(Workspace_Data *wd)
{
   switch (wd->mode)
     {
      case MODE_NORMAL:
      case MODE_CODE:
         return &wd->normal;
      case MODE_DEMO:
         return &wd->demo;
     }
   return NULL;
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
_workspace_del(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   assert(data != NULL);

   Workspace_Data *wd = data;
   gm_group_edit_object_unload(wd->group);
   color_term(wd->code.color_data);

   TODO("remove after moving from smart object");
   evas_object_del(wd->normal.layout);
   evas_object_del(wd->normal.scroller);
   evas_object_del(wd->normal.content);
   evas_object_del(wd->normal.container);
   evas_object_hide(wd->normal.container);
   evas_object_del(wd->demo.layout);
   evas_object_del(wd->demo.scroller);
   evas_object_del(wd->demo.content);
   evas_object_del(wd->demo.container);
   evas_object_hide(wd->demo.container);

   free(wd);
}

/******************************************************************************/
/*                              CODE RELOAD                                   */
/******************************************************************************/
static void
_object_delete_job(void *data)
{
   evas_object_del(data);
}

static void
_code_reload(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   assert(data != NULL);

   Workspace_Data *wd = data;
   Evas_Object *layout;

   if (MODE_CODE != wd->mode) return;
   elm_entry_entry_set(wd->code.obj, _group_code_get(wd));
   layout = elm_object_part_content_unset(wd->code.obj, "elm.swallow.overlay");
   ecore_job_add(_object_delete_job, layout);
}

/******************************************************************************/

static void
_members_zoom_set(Workspace_Data *wd)
{
   Scroll_Area *area;

   DBG("Set the zoom factor %f in tab '%s'", wd->zoom_factor * 100, wd->group->name);
   area = _scroll_area_get(wd);

   container_zoom_factor_set(area->container, wd->zoom_factor);
   groupview_zoom_factor_set(area->content, wd->zoom_factor);

   ewe_ruler_step_set(area->ruler_h.obj, NULL, (int)(50 * wd->zoom_factor));
   ewe_ruler_step_set(area->ruler_v.obj, NULL, (int)(50 * wd->zoom_factor));
}

static void
_fit_cb(void *data,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;

   workspace_container_fit(wd->layout);
}

static void
_slider_zoom_cb(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Eina_Stringshare *text;
   Workspace_Data *wd = data;

   wd->zoom_factor = elm_slider_value_get(wd->toolbar.zoom.slider) / 100;
   text = eina_stringshare_printf("%d%%", (int)(wd->zoom_factor * 100));
   ewe_combobox_text_set(wd->toolbar.zoom.cmb_zoom, text);
   eina_stringshare_del(text);
   _members_zoom_set(wd);
}

static void
_zoom_controls_disabled_set(Workspace_Data *wd, Eina_Bool disabled)
{
   elm_object_disabled_set(wd->toolbar.zoom.fit, disabled);
   elm_object_disabled_set(wd->toolbar.zoom.cmb_zoom, disabled);
   elm_object_disabled_set(wd->toolbar.zoom.slider, disabled);
}

static void
_zoom_selected_cb(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Workspace_Data *wd = data;
   Ewe_Combobox_Item *item = event_info;

   wd->zoom_factor = zoom_values[item->index] / 100.0;
   elm_slider_value_set(wd->toolbar.zoom.slider, zoom_values[item->index]);

   _members_zoom_set(wd);
}

static void
_zoom_controls_add(Workspace_Data *wd)
{
   int i = 0;
   Elm_Object_Item *tb_it;
   Evas_Object *img;
   Eina_Stringshare *text;

   wd->toolbar.zoom.fit = elm_button_add(wd->toolbar.obj);
   evas_object_smart_callback_add(wd->toolbar.zoom.fit, "clicked", _fit_cb, wd);
   IMAGE_ADD_NEW(wd->toolbar.zoom.fit, img, "icon", "fit")
   elm_object_part_content_set(wd->toolbar.zoom.fit, NULL, img);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.zoom.fit);

   EWE_COMBOBOX_ADD(wd->toolbar.obj, wd->toolbar.zoom.cmb_zoom);
   evas_object_size_hint_min_set(wd->toolbar.zoom.cmb_zoom, 70, 0);
   ewe_combobox_text_set(wd->toolbar.zoom.cmb_zoom, _("100%"));
   evas_object_smart_callback_add(wd->toolbar.zoom.cmb_zoom, "selected", _zoom_selected_cb, wd);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.zoom.cmb_zoom);
   while (zoom_values[i])
   {
      text = eina_stringshare_printf("%d%%", zoom_values[i]);
      ewe_combobox_item_add(wd->toolbar.zoom.cmb_zoom, text);
      eina_stringshare_del(text);
      i++;
   }

   wd->toolbar.zoom.slider = elm_slider_add(wd->toolbar.obj);
   elm_slider_min_max_set(wd->toolbar.zoom.slider, 10.0, 1000.0);
   elm_slider_value_set(wd->toolbar.zoom.slider, 100);
   evas_object_smart_callback_add(wd->toolbar.zoom.slider, "changed", _slider_zoom_cb, wd);
   IMAGE_ADD_NEW(wd->toolbar.zoom.slider, img, "icon", "scale_smaller")
   elm_object_part_content_set(wd->toolbar.zoom.slider, "elm.swallow.icon", img);
   IMAGE_ADD_NEW(wd->toolbar.zoom.slider, img, "icon", "scale_larger")
   elm_object_part_content_set(wd->toolbar.zoom.slider, "elm.swallow.end", img);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.zoom.slider);

   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_toolbar_item_separator_set(tb_it, true);
}

static void
_spinner_container_change(void *data,
                          Evas_Object *obj,
                          void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;
   int w ,h;

   area = _scroll_area_get(wd);
   assert(area != NULL);

   w = elm_spinner_value_get(wd->toolbar.container_sizer.spinner_w);
   h = elm_spinner_value_get(wd->toolbar.container_sizer.spinner_h);

   if (elm_check_state_get(wd->toolbar.container_sizer.check_chain) &&
       obj == wd->toolbar.container_sizer.spinner_h)
     w = h * container_aspect_get(area->container);

   container_container_size_set(area->container, w, h);
}

static void
_container_aspect_change(void *data,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;
   int w ,h;
   double aspect = -1.0;

   area = _scroll_area_get(wd);
   assert(area != NULL);

   if (elm_check_state_get(obj))
     {
        w = elm_spinner_value_get(wd->toolbar.container_sizer.spinner_w);
        h = elm_spinner_value_get(wd->toolbar.container_sizer.spinner_h);
        if (w == 0) w = 1;
        if (h == 0) h = 1;
        aspect = (double)w / (double)h;
     }
   container_aspect_set(area->container, aspect);
}

static void
_container_lock(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;
   Eina_Bool lock;

   area = _scroll_area_get(wd);
   assert(area != NULL);

   lock = elm_check_state_get(obj);
   container_lock_set(area->container, lock);
}

static void
_container_size_controls_add(Workspace_Data *wd)
{
   Elm_Object_Item *tb_it;

   wd->toolbar.container_sizer.spinner_w = elm_spinner_add(wd->toolbar.obj);
   elm_spinner_min_max_set(wd->toolbar.container_sizer.spinner_w, 0, 9999);
   elm_spinner_editable_set(wd->toolbar.container_sizer.spinner_w, true);
   evas_object_smart_callback_add(wd->toolbar.container_sizer.spinner_w, "changed", _spinner_container_change, wd);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.container_sizer.spinner_w);

   wd->toolbar.container_sizer.check_chain = elm_check_add(wd->toolbar.obj);
   evas_object_smart_callback_add(wd->toolbar.container_sizer.check_chain, "changed", _container_aspect_change, wd);
   elm_object_style_set(wd->toolbar.container_sizer.check_chain, "chain");
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.container_sizer.check_chain);

   wd->toolbar.container_sizer.spinner_h = elm_spinner_add(wd->toolbar.obj);
   elm_spinner_min_max_set(wd->toolbar.container_sizer.spinner_h, 0, 9999);
   elm_spinner_editable_set(wd->toolbar.container_sizer.spinner_h, true);
   evas_object_smart_callback_add(wd->toolbar.container_sizer.spinner_h, "changed", _spinner_container_change, wd);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.container_sizer.spinner_h);

   wd->toolbar.container_sizer.check_lock = elm_check_add(wd->toolbar.obj);
   elm_object_style_set(wd->toolbar.container_sizer.check_lock, "locker");
   evas_object_smart_callback_add(wd->toolbar.container_sizer.check_lock, "changed", _container_lock, wd);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.container_sizer.check_lock);

   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_toolbar_item_separator_set(tb_it, true);
}

static Evas_Object *
_radio_switcher_add(Workspace_Data *wd,
                    const char *style,
                    Evas_Smart_Cb func,
                    int state_value,
                    Evas_Object *group)
{
   Evas_Object *radio;

   radio = elm_radio_add(wd->layout);
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
   Workspace_Data *wd = data;
   Container_Geom *geom = event_info;
   const Groupview_Geom *part_geom = NULL;
   Evas_Coord x, y;
   Evas_Coord scale_x, scale_y, step_w, step_h;
   Scroll_Area *area;

   area = _scroll_area_get(wd);
   assert(area != NULL);

   evas_object_geometry_get(area->ruler_h.obj, &x, NULL, NULL, NULL);
   evas_object_geometry_get(area->ruler_v.obj, NULL, &y, NULL, NULL);

   if (((MODE_NORMAL == wd->mode) || (MODE_CODE == wd->mode)) && area->content)
     if (groupview_part_object_area_visible_get(area->content))
       part_geom = groupview_part_selected_object_area_geom_get(area->content);
   if (part_geom)
     {
        scale_x = part_geom->x - x;
        scale_y = part_geom->y - y;
        step_w = part_geom->w;
        step_h = part_geom->h;
     }
   else
     {
        scale_x = geom->x - x;
        scale_y = geom->y - y;
        step_w = geom->w;
        step_h = geom->h;
     }


   /* shift the abs scale zero mark */
   ewe_ruler_zero_offset_set(area->ruler_h.obj, NULL, geom->x - x);
   ewe_ruler_zero_offset_set(area->ruler_v.obj, NULL, geom->y - y);

   /* shift the rel scale zero mark */
   if (area->ruler_h.scale_rel)
     {
        ewe_ruler_zero_offset_set(area->ruler_h.obj, area->ruler_h.scale_rel, scale_x);
        ewe_ruler_step_set(area->ruler_h.obj, area->ruler_h.scale_rel, ((step_w * wd->zoom_factor) / 2));
     }
   if (area->ruler_v.scale_rel)
     {
        ewe_ruler_zero_offset_set(area->ruler_v.obj, area->ruler_v.scale_rel, scale_y);
        ewe_ruler_step_set(area->ruler_v.obj, area->ruler_v.scale_rel, ((step_h * wd->zoom_factor) / 2));
     }

   elm_spinner_value_set(wd->toolbar.container_sizer.spinner_w, geom->w);
   elm_spinner_value_set(wd->toolbar.container_sizer.spinner_h, geom->h);
}

static void
_menu_undo(void *data __UNUSED__,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_UNDO, NULL);
}

static void
_menu_redo(void *data __UNUSED__,
           Evas_Object *obj __UNUSED__,
           void *event_info __UNUSED__)
{
   evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_REDO, NULL);
}

static void
_menu_rulers_visible(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_RULERS_VISIBLED, NULL);
}

static void
_menu_ruler_abs(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;

   area = _scroll_area_get(wd);
   ewe_ruler_scale_visible_set(area->ruler_h.obj, NULL, true);
   ewe_ruler_scale_visible_set(area->ruler_h.obj, area->ruler_h.scale_rel, false);
   ewe_ruler_scale_visible_set(area->ruler_v.obj, NULL, true);
   ewe_ruler_scale_visible_set(area->ruler_v.obj, area->ruler_v.scale_rel, false);
}

static void
_menu_ruler_rel(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;

   area = _scroll_area_get(wd);
   ewe_ruler_scale_visible_set(area->ruler_h.obj, NULL, false);
   ewe_ruler_scale_visible_set(area->ruler_h.obj, area->ruler_h.scale_rel, true);
   ewe_ruler_scale_visible_set(area->ruler_v.obj, NULL, false);
   ewe_ruler_scale_visible_set(area->ruler_v.obj, area->ruler_v.scale_rel, true);
}

static void
_menu_rulers_both(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;

   area = _scroll_area_get(wd);
   ewe_ruler_scale_visible_set(area->ruler_h.obj, NULL, true);
   ewe_ruler_scale_visible_set(area->ruler_h.obj, area->ruler_h.scale_rel, true);
   ewe_ruler_scale_visible_set(area->ruler_v.obj, NULL, true);
   ewe_ruler_scale_visible_set(area->ruler_v.obj, area->ruler_v.scale_rel, true);
}

#define MENU_ITEM_ADD(MENU, PARENT, ICON, LABEL, CALLBACK, SHORTCUT, WIDGET, DATA) \
   do \
     { \
        Elm_Object_Item *item; \
        item = elm_menu_item_add(MENU, PARENT, ICON, LABEL, CALLBACK, DATA); \
        if (SHORTCUT || WIDGET) \
          { \
             Evas_Object *item_obj = elm_menu_item_object_get(item);\
             if (SHORTCUT) elm_object_part_text_set(item_obj, "elm.shortcut", SHORTCUT); \
             if (WIDGET) elm_object_part_content_set(item_obj, "elm.swallow.content", WIDGET); \
          } \
     } \
   while (0);

static void
_menu_cb(void *data,
         Evas *e __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info)
{
   Workspace_Data *wd = data;
   Scroll_Area *area;
   Evas_Event_Mouse_Down *ev = event_info;
   Eina_Bool sa, sr;

   if (ev->button != 3) return;
   area = _scroll_area_get(wd);

   sa = ewe_ruler_scale_visible_get(area->ruler_h.obj, NULL);
   sr = ewe_ruler_scale_visible_get(area->ruler_h.obj, area->ruler_h.scale_rel);

   if (sa) elm_radio_value_set(wd->menu.scale_abs, 0);
   if (sr) elm_radio_value_set(wd->menu.scale_abs, 1);
   if (sa && sr) elm_radio_value_set(wd->menu.scale_abs, 2);

   elm_menu_move(wd->menu.obj, ev->canvas.x, ev->canvas.y);
   evas_object_show(wd->menu.obj);
}

static void
_menu_add(Workspace_Data *wd)
{
   wd->menu.obj = elm_menu_add(ap.win);
   MENU_ITEM_ADD(wd->menu.obj, NULL, NULL, _("Undo"), _menu_undo, "Ctrl-Z", NULL, NULL);
   MENU_ITEM_ADD(wd->menu.obj, NULL, NULL, _("Redo"), _menu_redo, "Ctrl-Y", NULL, NULL);
   elm_menu_item_separator_add(wd->menu.obj, NULL);
   MENU_ITEM_ADD(wd->menu.obj, NULL, NULL, _("Show rulers"), _menu_rulers_visible, NULL, NULL, NULL);

   elm_menu_item_separator_add(wd->menu.obj, NULL);
   wd->menu.scale_abs = _radio_switcher_add(wd, NULL, NULL, 0, NULL);
   MENU_ITEM_ADD(wd->menu.obj, NULL, NULL, _("Absolute scale"), _menu_ruler_abs, NULL, wd->menu.scale_abs, wd);
   wd->menu.scale_rel = _radio_switcher_add(wd, NULL, NULL, 1, wd->menu.scale_abs);
   MENU_ITEM_ADD(wd->menu.obj, NULL, NULL, _("Relative scale"), _menu_ruler_rel, NULL, wd->menu.scale_rel, wd);
   wd->menu.scale_both = _radio_switcher_add(wd, NULL, NULL, 2, wd->menu.scale_abs);
   MENU_ITEM_ADD(wd->menu.obj, NULL, NULL, _("Both scales"), _menu_rulers_both, NULL, wd->menu.scale_both, wd);
}

static void
_scroll_area_add(Workspace_Data *wd, Scroll_Area *area, Eina_Bool scale_rel)
{
   area->bg_preview = BG_PREVIEW_TILE;

   area->layout = elm_layout_add(wd->layout);
   elm_layout_theme_set(area->layout, "layout", "workspace", "scroller");

   _ruler_add(area->layout, &area->ruler_h, scale_rel);
   elm_layout_content_set(area->layout, "elm.swallow.ruler_h", area->ruler_h.obj);
   _ruler_add(area->layout, &area->ruler_v, scale_rel);
   ewe_ruler_horizontal_set(area->ruler_v.obj, false);
   elm_layout_content_set(area->layout, "elm.swallow.ruler_v", area->ruler_v.obj);

   /* create scroller for normal mode and set bg */
   area->scroller = elm_scroller_add(area->layout);
   elm_scroller_policy_set(area->scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
   evas_object_event_callback_add(area->scroller, EVAS_CALLBACK_MOUSE_MOVE, _rulers_pointer_move, area);
   elm_layout_content_set(area->layout, "elm.swallow.scroller", area->scroller);
   area->bg = elm_layout_add(area->layout);
   elm_layout_theme_set(area->bg, "layout", "workspace", "bg");
   elm_object_part_content_set(area->scroller, "elm.swallow.background", area->bg);

   area->container = container_add(area->scroller);
   container_handler_size_set(area->container, 8, 8);
   evas_object_smart_callback_add(area->container, "container,changed", _container_changed, wd);
   elm_object_content_set(area->scroller, area->container);
   container_container_size_set(area->container, 350, 350);

   if (scale_rel) /* this bool like marker, if scale_rel is true we have the area for normal mode */
     evas_object_event_callback_add(area->scroller, EVAS_CALLBACK_MOUSE_DOWN, _menu_cb, wd);
}

#if HAVE_TIZEN
static void
_library_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Workspace_Data *wd = data;
   Ewe_Combobox_Item *item = event_info;

   switch (item->index)
     {
        case 0: /* Image library */
         image_manager_add();
        break;
        case 1: /* Sound library */
         sound_manager_add();
        break;
        case 2: /* TextBlock library */
         style_manager_add();
        break;
        case 3: /* Colorclass library */
         colorclass_manager_add();
        break;
     }

   ewe_combobox_text_set(wd->toolbar.libraries_switcher, _("Library"));
}
#endif

static void
_mode_cb(void *data,
         Evas_Object *obj,
         void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;
   Workspace_Mode mode;
   Evas_Object *content;
   Scroll_Area *area = NULL;
   const Container_Geom *geom;

#if !HAVE_TIZEN
   mode = elm_radio_value_get(obj);
#else
   // Tizen spceific code. Here uses check instead radio button.
   mode = elm_check_state_get(obj) == EINA_TRUE ? MODE_DEMO : MODE_NORMAL;
#endif

   if (mode == wd->mode) return;

   wd->mode = mode;
   content = elm_object_part_content_unset(wd->panes_h, "left");
   evas_object_hide(content);
   //content = elm_object_part_content_unset(wd->panes, "right");
   evas_object_hide(content);
   if (wd->demo.content)
     {
        evas_object_del(wd->demo.content);
        wd->demo.content = NULL;
     }

   elm_panes_fixed_set(wd->panes_h, true);
   elm_panes_content_right_size_set(wd->panes_h, 0);
   switch (wd->mode)
     {
      case MODE_CODE:
         elm_panes_fixed_set(wd->panes_h, false);
         if (wd->code.size == -1) wd->code.size = 0.5;
         elm_panes_content_right_size_set(wd->panes_h, wd->code.size);
         elm_entry_entry_set(wd->code.obj, _group_code_get(wd));
      case MODE_NORMAL:
         elm_object_part_content_set(wd->panes_h, "left", wd->normal.layout);
         evas_object_show(wd->normal.layout);
         elm_radio_value_set(wd->toolbar.bg_switcher.white, wd->normal.bg_preview);

         //elm_object_part_content_set(wd->panes, "right", wd->group_navi);
         evas_object_show(wd->group_navi);
         _zoom_controls_disabled_set(wd, false);
         evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_CHANGED, wd->group);

         area = &wd->normal;
         ui_menu_disable_set(ap.menu, MENU_VIEW_WORKSPACE_FIT, false);
         break;
      case MODE_DEMO:
         if (!wd->demo.layout) _scroll_area_add(wd, &wd->demo, false);
         elm_object_part_content_set(wd->panes_h, "left", wd->demo.layout);
         evas_object_show(wd->demo.layout);
         elm_radio_value_set(wd->toolbar.bg_switcher.white, wd->demo.bg_preview);

         wd->demo.content = demo_add(wd->demo.scroller, wd->group);
         container_content_set(wd->demo.container, wd->demo.content);

         //elm_object_part_content_set(wd->panes, "right", wd->demo_navi);
         evas_object_show(wd->demo_navi);
         demo_group_demo_update(wd->demo_navi);

         _zoom_controls_disabled_set(wd, true);
         evas_object_smart_callback_call(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, NULL);

         area = &wd->demo;
         ui_menu_disable_set(ap.menu, MENU_VIEW_WORKSPACE_FIT, true);
         demo_group_property_update(wd->demo_navi);
         break;
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_WORKSPACE_MODE_CHANGED, &wd->mode);
   assert (area != NULL);

   if (container_aspect_get(area->container) < 0)
     elm_check_state_set(wd->toolbar.container_sizer.check_chain, false);
   else
     elm_check_state_set(wd->toolbar.container_sizer.check_chain, true);

   elm_check_state_set(wd->toolbar.container_sizer.check_lock, container_lock_get(area->container));

   geom = container_geom_get(area->container);
   elm_spinner_value_set(wd->toolbar.container_sizer.spinner_w, geom->w);
   elm_spinner_value_set(wd->toolbar.container_sizer.spinner_h, geom->h);
}

#if !HAVE_TIZEN
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
   if ((MODE_NORMAL == wd->mode) || (MODE_CODE == wd->mode))
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
#endif

static void
_part_select(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Workspace_Data *wd = data;
   Part *part = event_info;

   assert((MODE_NORMAL == wd->mode) || (MODE_CODE == wd->mode));
   assert(part != NULL);

   groupview_part_select(wd->normal.content, part->name);
   evas_object_smart_callback_call(ap.win, SIGNAL_PART_SELECTED, part);
   groupview_part_item_selected_set(wd->normal.content, part->current_item_name, part->current_item_name ? true : false);
}

static void
_part_visible(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info)
{
   Workspace_Data *wd = data;
   Part *part = event_info;

   assert((MODE_NORMAL == wd->mode) || (MODE_CODE == wd->mode));

   groupview_part_visible_set(wd->normal.content, part);
}

/******************************************************************************/
/*                           GROUPVIEW CALLBACKS                              */
/******************************************************************************/
static void
_groupview_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Workspace_Data *wd = data;
   Part *part = event_info;

   assert((MODE_NORMAL == wd->mode) || (MODE_CODE == wd->mode));

   group_navigator_part_select(wd->group_navi, part ? part : NULL);
   groupview_part_select(wd->normal.content, part ? part->name : NULL);
}

static void
_groupview_hl_part_drag_start(void *data,
                              Evas_Object *obj __UNUSED__,
                              void *event_info)
{
   Workspace_Data *wd = data;
   Groupview_HL_Event *event = event_info;

   change = change_add(NULL);
   if (MIDDLE != event->hl_type)
     {
        part_w = edje_edit_state_max_w_get(wd->group->edit_object, event->part->name,
                                           event->part->current_state->parsed_name,
                                           event->part->current_state->parsed_val);
        part_h = edje_edit_state_max_h_get(wd->group->edit_object, event->part->name,
                                           event->part->current_state->parsed_name,
                                           event->part->current_state->parsed_val);
     }
   else
     {
        part_align_x = edje_edit_state_align_x_get(wd->group->edit_object, event->part->name,
                                                   event->part->current_state->parsed_name,
                                                   event->part->current_state->parsed_val);
        part_align_y = edje_edit_state_align_y_get(wd->group->edit_object, event->part->name,
                                                   event->part->current_state->parsed_name,
                                                   event->part->current_state->parsed_val);
     }
}

static void
_groupview_hl_part_changed(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Workspace_Data *wd = data;
   Groupview_HL_Event *event = event_info;

   assert((wd->mode == MODE_NORMAL) || (wd->mode == MODE_CODE));

   if (MIDDLE != event->hl_type)
     {
        CRIT_ON_FAIL(editor_state_max_w_set(wd->group->edit_object, change, true, event->part->name,
                                            event->part->current_state->parsed_name,
                                            event->part->current_state->parsed_val,
                                            event->w));
        CRIT_ON_FAIL(editor_state_max_h_set(wd->group->edit_object, change, true, event->part->name,
                                            event->part->current_state->parsed_name,
                                            event->part->current_state->parsed_val,
                                            event->h));
     }
   else
     {
        const Groupview_Geom *geom;
        geom = groupview_part_selected_object_area_geom_get(wd->normal.content);
        double align_x = (double)(event->x - geom->x) / (double)(geom->w - event->w);
        double align_y = (double)(event->y - geom->y) / (double)(geom->h - event->h);


        if ((geom->w == event->w) || (align_x < 0)) align_x = 0;
        if ((geom->h == event->h) || (align_y < 0)) align_y = 0;
        if (align_x > 1.0) align_x = 1.0;
        if (align_y > 1.0) align_y = 1.0;
        CRIT_ON_FAIL(editor_state_align_x_set(wd->group->edit_object, change, true, event->part->name,
                                              event->part->current_state->parsed_name,
                                              event->part->current_state->parsed_val, align_x));
        CRIT_ON_FAIL(editor_state_align_y_set(wd->group->edit_object, change, true, event->part->name,
                                              event->part->current_state->parsed_name,
                                              event->part->current_state->parsed_val, align_y));

     }
}

static void
_groupview_hl_part_drag_stop(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info)
{
   Workspace_Data *wd = data;
   Groupview_HL_Event *event = event_info;
   Eina_Stringshare *msg;

   if (MIDDLE != event->hl_type)
     {
        if ((part_w == event->w) && (part_h == event->h))
          change_free(change);
        else
          {
             msg = eina_stringshare_printf(_("max size changed from [%dx%d] to [%dx%d]"), part_w, part_h, event->w, event->h);
             change_description_set(change, msg);
             eina_stringshare_del(msg);
             history_change_add(wd->group->history, change);
          }
     }
   else
     {
        double align_x = edje_edit_state_align_x_get(wd->group->edit_object, event->part->name,
                                                     event->part->current_state->parsed_name,
                                                     event->part->current_state->parsed_val);
        double align_y = edje_edit_state_align_y_get(wd->group->edit_object, event->part->name,
                                                     event->part->current_state->parsed_name,
                                                     event->part->current_state->parsed_val);
        if ((align_x == part_align_x) && (align_y == part_align_y))
          change_free(change);
        else
          {
             msg = eina_stringshare_printf(_("align changed from [%fx%f] to [%fx%f]"),
                                           part_align_x, part_align_y, align_x, align_y);
             change_description_set(change, msg);
             eina_stringshare_del(msg);
             history_change_add(wd->group->history, change);
          }
     }
   change = NULL;
}
/******************************************************************************/

static void
_panes_h_unpress(void *data,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   Workspace_Data *wd = data;

   wd->code.size = elm_panes_content_right_size_get(obj);
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
#if 0
   wd->panes = elm_panes_add(parent);
   elm_panes_content_right_min_size_set(wd->panes, PANES_RIGHT_SIZE_MIN);
   elm_panes_content_right_size_set(wd->panes, 0); /* set the default min size */
#endif /* if 0 */

   wd->code.size = -1;
   wd->group = group;
   wd->mode = MODE_NORMAL;
   wd->normal.rulers_visible = true;
   wd->demo.rulers_visible = true;

   wd->layout = elm_layout_add(parent);
   elm_layout_theme_set(wd->layout, "layout", "workspace", "toolbar");

   /* add toolbar */
   wd->toolbar.obj = elm_toolbar_add(wd->layout);
   elm_toolbar_align_set(wd->toolbar.obj, 0.0);
   elm_toolbar_shrink_mode_set(wd->toolbar.obj, ELM_TOOLBAR_SHRINK_SCROLL);
   elm_toolbar_select_mode_set(wd->toolbar.obj, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_layout_content_set(wd->layout, "elm.swallow.toolbar", wd->toolbar.obj);

   /* add to toolbar the zoom controls */
   wd->zoom_factor = 1.0;
   _zoom_controls_add(wd);

#if !HAVE_TIZEN
   /* add to toolbar modes switcher */
   wd->toolbar.mode_switcher.normal = _radio_switcher_add(wd, "radio_normal", _mode_cb, MODE_NORMAL, NULL);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.mode_switcher.normal);
   wd->toolbar.mode_switcher.code = _radio_switcher_add(wd, "radio_code", _mode_cb, MODE_CODE, wd->toolbar.mode_switcher.normal);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.mode_switcher.code);
   wd->toolbar.mode_switcher.demo = _radio_switcher_add(wd, "radio_demo", _mode_cb, MODE_DEMO, wd->toolbar.mode_switcher.normal);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.mode_switcher.demo);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_toolbar_item_separator_set(tb_it, true);
   elm_radio_value_set(wd->toolbar.mode_switcher.normal, 1);
#else
   CHECK_ADD(wd->layout, wd->toolbar.mode_switcher);
   elm_object_style_set(wd->toolbar.mode_switcher, "demo");
   evas_object_smart_callback_add(wd->toolbar.mode_switcher, "changed", _mode_cb, wd);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.mode_switcher);
#endif
   /* add the container size controls */
   _container_size_controls_add(wd);

#if !HAVE_TIZEN
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
#endif


#if HAVE_TIZEN
   /* Combobox for a choose libraries. */
   EWE_COMBOBOX_ADD(wd->toolbar.obj, wd->toolbar.libraries_switcher);
   evas_object_size_hint_min_set(wd->toolbar.libraries_switcher, 92, 0);
   ewe_combobox_text_set(wd->toolbar.libraries_switcher, _("Library"));
   evas_object_smart_callback_add(wd->toolbar.libraries_switcher, "selected", _library_select, wd);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.libraries_switcher);

   ewe_combobox_item_add(wd->toolbar.libraries_switcher, "Image");
   ewe_combobox_item_add(wd->toolbar.libraries_switcher, "Sound");
   ewe_combobox_item_add(wd->toolbar.libraries_switcher, "Textblock styles");
   ewe_combobox_item_add(wd->toolbar.libraries_switcher, "Color classes");

#endif


   /*Add to toolbar history controls */
   wd->toolbar.history =  history_ui_add(wd->toolbar.obj, wd->group->history);
   evas_object_show(wd->toolbar.history);
   tb_it = elm_toolbar_item_append(wd->toolbar.obj, NULL, NULL, NULL, NULL);
   elm_object_item_part_content_set(tb_it, NULL, wd->toolbar.history);

   wd->panes_h = elm_panes_add(wd->layout);
   elm_object_style_set(wd->panes_h, "pan_hide");
   elm_panes_horizontal_set(wd->panes_h, true);
   elm_panes_content_right_size_set(wd->panes_h, 0); /* set the default min size */
   evas_object_smart_callback_add(wd->panes_h, "unpress", _panes_h_unpress, wd);
   elm_layout_content_set(wd->layout, NULL, wd->panes_h);

   ENTRY_ADD(wd->panes_h, wd->code.obj, false)
   elm_entry_editable_set(wd->code.obj, false);
   elm_object_part_content_set(wd->panes_h, "right", wd->code.obj);
   wd->code.color_data = color_init(eina_strbuf_new());

   _scroll_area_add(wd, &wd->normal, true);
   elm_object_part_content_set(wd->panes_h, "left", wd->normal.layout);
   wd->normal.content = groupview_add(wd->normal.scroller, group);
   container_content_set(wd->normal.container, wd->normal.content);
   _container_hints_set(wd);

   container_protrusion_func_set(wd->normal.container, groupview_protrusion_get);
   evas_object_smart_callback_add(wd->normal.content, SIGNAL_GROUPVIEW_CLICKED, _groupview_clicked, wd);
   evas_object_smart_callback_add(wd->normal.content, SIGNAL_GROUPVIEW_HL_PART_DRAG_START, _groupview_hl_part_drag_start, wd);
   evas_object_smart_callback_add(wd->normal.content, SIGNAL_GROUPVIEW_HL_PART_CHANGED, _groupview_hl_part_changed, wd);
   evas_object_smart_callback_add(wd->normal.content, SIGNAL_GROUPVIEW_HL_PART_DRAG_STOP, _groupview_hl_part_drag_stop, wd);

   wd->group_navi = group_navigator_add(parent, group);
   //elm_object_part_content_set(wd->panes, "right", wd->group_navi);
   evas_object_smart_callback_add(wd->group_navi, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED, _part_select, wd);
   evas_object_smart_callback_add(wd->group_navi, SIGNAL_GROUP_NAVIGATOR_PART_VISIBLE_CHANGED, _part_visible, wd);

   wd->demo_navi = demo_group_add(group);
   evas_object_size_hint_weight_set(wd->demo_navi, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(wd->demo_navi, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_hide(wd->demo_navi);

   _menu_add(wd);

   evas_object_data_set(wd->layout, WORKSPACE_DATA, wd);
   evas_object_event_callback_add(wd->layout, EVAS_CALLBACK_DEL, _workspace_del, wd);

   return wd->layout;
}

void
workspace_mode_set(Evas_Object *obj, Workspace_Mode mode)
{
   WS_DATA_GET(obj);

#if !HAVE_TIZEN
   elm_radio_value_set(wd->toolbar.mode_switcher.normal, mode);
   _mode_cb(wd, wd->toolbar.mode_switcher.normal, NULL);
#else
   assert(mode != MODE_CODE);

   if (mode == MODE_NORMAL)
     elm_check_state_set(wd->toolbar.mode_switcher, EINA_FALSE);
   else if (mode == MODE_DEMO)
     elm_check_state_set(wd->toolbar.mode_switcher, EINA_TRUE);
   _mode_cb(wd, wd->toolbar.mode_switcher, NULL);
#endif
}

void
workspace_group_navigator_update_part(Evas_Object *obj, Part *part)
{
   WS_DATA_GET(obj);

   assert(part != NULL);

   group_navigator_part_update(wd->group_navi, part);
}

void
workspace_group_navigator_update_group_data(Evas_Object *obj, Resource *group_data)
{
   WS_DATA_GET(obj);

   assert(group_data != NULL);

   group_navigator_group_data_update(wd->group_navi, group_data);
}

Eina_Bool
workspace_edit_object_recalc(Evas_Object *obj __UNUSED__)
{
   return false;
}

void
workspace_groupview_hard_update(Evas_Object *obj)
{
   WS_DATA_GET(obj);
   assert(wd->normal.content != NULL);

   _container_hints_set(wd);
   groupview_hard_update(wd->normal.content);
}

void
workspace_groupview_soft_update(Evas_Object *obj)
{
   WS_DATA_GET(obj);
   assert(wd->normal.content != NULL);
   groupview_soft_update(wd->normal.content);
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

void
workspace_object_area_visible_set(Evas_Object *obj, Eina_Bool visible)
{
   Scroll_Area *area;

   WS_DATA_GET(obj);

   if ((MODE_NORMAL != wd->mode) && (MODE_CODE != wd->mode)) return;
   area = _scroll_area_get(wd);

   groupview_part_object_area_visible_set(area->content, visible);
   _container_changed(wd, NULL, container_geom_get(area->container));
}

Eina_Bool
workspace_object_area_visible_get(Evas_Object *obj)
{
   Scroll_Area *area;

   WS_DATA_GET(obj);

   if ((MODE_NORMAL != wd->mode) && (MODE_CODE != wd->mode)) return false;
   area = _scroll_area_get(wd);

   return groupview_part_object_area_visible_get(area->content);
}

void
workspace_part_add(Evas_Object *obj, Eina_Stringshare *part_name)
{
   Part *part;
   WS_DATA_GET(obj);
   assert(part_name != NULL);

   part = gm_part_add(ap.project, wd->group, part_name);
   groupview_part_add(wd->normal.content, part);
   group_navigator_part_add(wd->group_navi, part);
   demo_group_part_add(wd->demo_navi, part);
}

void
workspace_part_del(Evas_Object *obj, Eina_Stringshare *part_name)
{
   Part *part;
   WS_DATA_GET(obj);
   assert(part_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);
   group_navigator_part_del(wd->group_navi, part);
   demo_group_part_del(wd->demo_navi, part);
   groupview_part_del(wd->normal.content, part);
   gm_part_del(ap.project, part);
}

void
workspace_part_item_add(Evas_Object *obj,
                        Eina_Stringshare *part_name,
                        Eina_Stringshare *item_name)
{
   Part *part;
   WS_DATA_GET(obj);
   assert(part_name != NULL);
   assert(item_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);

   assert((part->type == EDJE_PART_TYPE_TABLE) ||
          (part->type == EDJE_PART_TYPE_BOX));

   group_navigator_part_select(wd->group_navi, part);
   gm_part_item_add(ap.project, part, item_name);
   groupview_hard_update(wd->normal.content);
   group_navigator_part_item_add(wd->group_navi, part, item_name);
}

void
workspace_part_item_del(Evas_Object *obj,
                        Eina_Stringshare *part_name,
                        Eina_Stringshare *item_name)
{
   Part *part;
   WS_DATA_GET(obj);
   assert(part_name != NULL);
   assert(item_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);

   assert((part->type == EDJE_PART_TYPE_TABLE) ||
          (part->type == EDJE_PART_TYPE_BOX));

   group_navigator_part_select(wd->group_navi, part);
   gm_part_item_del(ap.project, part, item_name);
   group_navigator_part_item_del(wd->group_navi, part, item_name);
}

void
workspace_part_state_add(Evas_Object *obj,
                         Eina_Stringshare *part_name,
                         Eina_Stringshare *state_name)
{
   Part *part;
   State *state;
   WS_DATA_GET(obj);
   assert(part_name != NULL);
   assert(state_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);

   group_navigator_part_select(wd->group_navi, part);
   state = gm_state_add(ap.project, part, state_name);
   group_navigator_part_state_add(wd->group_navi, part, state);
}

void
workspace_part_state_select(Evas_Object *obj,
                            Eina_Stringshare *part_name,
                            Eina_Stringshare *state_name)
{
   Part *part;
   State *state;
   WS_DATA_GET(obj);
   assert(part_name != NULL);
   assert(state_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);
   state = pm_resource_get(part->states, state_name);

   groupview_hard_update(wd->normal.content);
   group_navigator_part_state_select(wd->group_navi, state);
}

void
workspace_part_state_del(Evas_Object *obj,
                         Eina_Stringshare *part_name,
                         Eina_Stringshare *state_name)
{
   Part *part;
   State *state;
   WS_DATA_GET(obj);
   assert(part_name != NULL);
   assert(state_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);
   state = pm_resource_get(part->states, state_name);

   group_navigator_part_select(wd->group_navi, part);
   group_navigator_part_state_del(wd->group_navi, part, state);
   gm_state_del(ap.project, state);
}

void
workspace_part_restack(Evas_Object *obj,
                       Eina_Stringshare *part_name,
                       Eina_Stringshare *relative_part_name)
{
   Part *part, *rel_part = NULL;
   WS_DATA_GET(obj);
   assert(part_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);
   if (relative_part_name)
     rel_part = pm_resource_unsorted_get(wd->group->parts, relative_part_name);

   group_navigator_part_select(wd->group_navi, part);
   group_navigator_part_restack(wd->group_navi, part, rel_part);
   gm_part_restack(part, rel_part);

   groupview_part_restack(wd->normal.content, part_name, relative_part_name);
}

void
workspace_part_item_restack(Evas_Object *obj,
                            Eina_Stringshare *part_name,
                            Eina_Stringshare *part_item_name,
                            Eina_Stringshare *relative_part_item_name)
{
   Part *part;
   WS_DATA_GET(obj);
   assert(part_item_name != NULL);

   part = pm_resource_unsorted_get(wd->group->parts, part_name);

   group_navigator_part_select(wd->group_navi, part);
   gm_part_item_restack(part, part_item_name, relative_part_item_name);
   group_navigator_part_item_restack(wd->group_navi, part, part_item_name, relative_part_item_name);

   groupview_hard_update(wd->normal.content);
}

void
workspace_program_add(Evas_Object *obj, Eina_Stringshare *program_name)
{
   WS_DATA_GET(obj);
   assert(program_name != NULL);

   gm_program_add(ap.project, wd->group, program_name);
   group_navigator_program_add(wd->group_navi, program_name);
}

void
workspace_program_del(Evas_Object *obj, Eina_Stringshare *program_name)
{
   WS_DATA_GET(obj);
   assert(program_name != NULL);

   gm_program_del(ap.project, wd->group, program_name);
   group_navigator_program_del(wd->group_navi, program_name);
   demo_group_program_del(wd->demo_navi, program_name);
}

void
workspace_rulers_visible_set(Evas_Object *obj, Eina_Bool visible)
{
   Scroll_Area *area;
   WS_DATA_GET(obj);

   area = _scroll_area_get(wd);
   area->rulers_visible = visible;
   if (visible)
     elm_layout_signal_emit(area->layout, "elm,state,rulers,show", "eflete");
   else
     elm_layout_signal_emit(area->layout, "elm,state,rulers,hide", "eflete");
}

void
workspace_demo_group_property_update(Evas_Object *obj)
{
   WS_DATA_GET(obj);
   if (wd->mode == MODE_DEMO)
     demo_group_property_update(wd->demo_navi);
}

Eina_Bool
workspace_rulers_visible_get(Evas_Object *obj)
{
   Scroll_Area *area;
   WS_DATA_GET(obj);

   area = _scroll_area_get(wd);
   return area->rulers_visible;
}

void
workspace_add_part_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_add_part_request(wd->group_navi);
}

void
workspace_add_part_item_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_add_part_item_request(wd->group_navi);
}

void
workspace_add_state_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_add_state_request(wd->group_navi);
}

void
workspace_add_program_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_add_program_request(wd->group_navi);
}

void
workspace_delete_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_delete_request(wd->group_navi);
}

void
workspace_zoom_factor_set(Evas_Object *obj, double factor)
{
   Eina_Stringshare *text;
   WS_DATA_GET(obj);

   if (!elm_object_disabled_get(wd->toolbar.zoom.slider))
     {
        wd->zoom_factor = factor;
        elm_slider_value_set(wd->toolbar.zoom.slider, factor * 100);
        text = eina_stringshare_printf("%d%%", (int)(wd->zoom_factor * 100));
        ewe_combobox_text_set(wd->toolbar.zoom.cmb_zoom, text);
        eina_stringshare_del(text);
        TODO("Fix elementary callbacks on changing value from code");
        _slider_zoom_cb(wd, wd->toolbar.zoom.slider, NULL);
     }
}

double
workspace_zoom_factor_get(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   return wd->zoom_factor;
}

void
workspace_container_fill(Evas_Object *obj)
{
   int w, h;
   int r, t, l, b;
   Scroll_Area *area;

   WS_DATA_GET(obj);

   area = _scroll_area_get(wd);
   /* get the bg size, because bg size not included the scrollbar size */
   evas_object_geometry_get(area->bg, NULL, NULL, &w, &h);
   container_padding_size_get(area->container, &r, &t, &l, &b);
   w = (w - l - r) / wd->zoom_factor;
   h = (h - t - b) / wd->zoom_factor;
   container_container_size_set(area->container, w, h);
}

void
workspace_container_fit(Evas_Object *obj)
{
   int w, h;
   double zoom;
   int r, t, l, b;
   Scroll_Area *area;
   const Container_Geom *geom;

   WS_DATA_GET(obj);

   if ((MODE_NORMAL != wd->mode) && (MODE_CODE != wd->mode)) return;

   area = _scroll_area_get(wd);
   /* get the bg size, because bg size not included the scrollbar size */
   evas_object_geometry_get(area->bg, NULL, NULL, &w, &h);
   geom = container_geom_get(area->container);
   container_padding_size_get(area->container, &r, &t, &l, &b);

   if (geom->w >= geom->h)
     zoom = (w - l - r) / (double)geom->w;
   else
     zoom = (h - t - b) / (double)geom->h;

   workspace_zoom_factor_set(obj, zoom);
}

void
workspace_state_next_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_state_next_request(wd->group_navi);
}

void
workspace_part_next_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_part_next_request(wd->group_navi);
}

void
workspace_part_prev_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_part_prev_request(wd->group_navi);
}

void
workspace_part_showhide_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_part_showhide_request(wd->group_navi);
}

void
workspace_all_parts_showhide_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_all_parts_showhide_request(wd->group_navi);
}

void
workspace_part_unselect_request(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   if (wd->mode != MODE_DEMO)
     group_navigator_part_select(wd->group_navi, NULL);
}

void
workspace_demo_swallow_set(Evas_Object *obj, Demo_Part *part)
{
   WS_DATA_GET(obj);

   if ((wd->demo.content) && (wd->mode == MODE_DEMO))
     evas_object_smart_callback_call(wd->demo.content, SIGNAL_DEMO_SWALLOW_SET, part);
}

void
workspace_demo_text_set(Evas_Object *obj, Demo_Part *part)
{
   WS_DATA_GET(obj);

   if ((wd->demo.content) && (wd->mode == MODE_DEMO))
     evas_object_smart_callback_call(wd->demo.content, SIGNAL_DEMO_TEXT_SET, part);
}

void
workspace_demo_signal_set(Evas_Object *obj, Demo_Signal *sig)
{
   WS_DATA_GET(obj);

   if ((wd->demo.content) && (wd->mode == MODE_DEMO))
     evas_object_smart_callback_call(wd->demo.content, SIGNAL_DEMO_SIGNAL_SEND, sig);
}

void
workspace_code_reload(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   _code_reload(wd, NULL, NULL);
}

void
workspace_code_changed(Evas_Object *obj)
{
   WS_DATA_GET(obj);
   Evas_Object *layout, *btn;

   layout = elm_object_part_content_get(wd->code.obj, "elm.swallow.overlay");
   if (layout) return;

   layout = elm_layout_add(wd->code.obj);
   elm_layout_theme_set(layout, "layout", "entry", "overlay");
   elm_object_text_set(layout, _("Project is changed"));
   btn = elm_button_add(layout);
   elm_object_text_set(btn, _("Reload"));
   evas_object_smart_callback_add(btn, "clicked", _code_reload, wd);
   elm_object_content_set(layout, btn);

   elm_object_part_content_set(wd->code.obj, "elm.swallow.overlay", layout);
}

Evas_Object *
workspace_group_navigator_get(Evas_Object *obj)
{
   if (!obj) return group_navigator_dummy_get();

   WS_DATA_GET(obj);
   if (MODE_DEMO == wd->mode)
     return wd->demo_navi;
   return wd->group_navi;
}

void
workspace_history_undo(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   history_ui_undo(wd->toolbar.history);
}

void
workspace_history_redo(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   history_ui_redo(wd->toolbar.history);
}

void
workspace_history_update(Evas_Object *obj)
{
   WS_DATA_GET(obj);

   history_ui_update(wd->toolbar.history);
}
