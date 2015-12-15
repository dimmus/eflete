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

#include "workspace.h"
#include "main_window.h"
#include "highlight.h"
#include "groupedit.h"
#include "container.h"
#include "eflete.h"
#include "group_navigator.h"
#include "signals.h"
#include "new_history.h"
#include "editor.h"
#include "demo.h"

struct _Ws_Menu
{
   Elm_Object_Item *undo;
   Elm_Object_Item *redo;

   Elm_Object_Item *rulers;
   Elm_Object_Item *rulers_enable;
   Elm_Object_Item *rulers_abs;
   Elm_Object_Item *rulers_rel;
   Elm_Object_Item *rulers_both;

   Elm_Object_Item *zoom;
   Elm_Object_Item *zoom_in;
   Elm_Object_Item *zoom_out;
   Elm_Object_Item *zoom_fit;
   Elm_Object_Item *zoom_far;
   Elm_Object_Item *zoom_normal;
   Elm_Object_Item *zoom_near;

   Elm_Object_Item *mode_normal;
   Elm_Object_Item *mode_separate;

   Elm_Object_Item *settings;
};
typedef struct _Ws_Menu Ws_Menu;

typedef enum
{
   MODE_NORMAL = 1,
   MODE_SEPARATE,
   /* MODE_ANIMATOR, */
   /* MODE_CODE, */
   MODE_DEMO,
   MODE_LAST
} Workspace_Mode;

/**
 * @typedef Ws_Smart_Data
 *
 * @brief The struct _Ws_Smart_Data uses, as smart data for Workspace object.
 * All UI elements of workspace stored here, only provate API of a workspace
 * module can modify pointers and values in this struct.
 *
 * @ingroup Workspace
 */
struct _Ws_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;    /**< Needed for correct create and \
                                             managment of Smart Object. \
                                             In future clip rulers, for \
                                             increase efficiency of EFLETE */
   Evas_Object *obj;             /**< Self-reference to workspace */
   Evas_Object *events;          /**< Needed for processing common events,\
                                   like mouse move or mouse click. */
   Evas_Object *clipper;         /**< Needed for hiding all additional object on
                                      scroller.*/
   Evas_Object *background;      /**< A backround image, \
                                   which swallowed into scroller.*/
   Evas_Object *ruler_hor;       /**< A ruler object, Horizontal.*/
   Ewe_Ruler_Scale *scale_rel_hor;/** <Pointer to relative scale of horizontal \
                                    ruler. */
   Ewe_Ruler_Marker *pointer_hor;/** <Pointer to marker on horizontal ruler */
   Evas_Object *ruler_ver;       /**< A ruler object, Vertical.*/
   Ewe_Ruler_Scale *scale_rel_ver;/** <Pointer to relative scale of vertical \
                                    ruler. */
   Ewe_Ruler_Marker *pointer_ver;/** <Pointer to marker on vertical ruler */
   Evas_Object *scroller;        /**< A scroler with 'eflete/workspace' style. \
                                   Implement scrollable interface.*/
   char scroll_flag;             /**< Needed for control drag bar's in scroller*/
   Evas_Object *bottom_box;
   Evas_Object *groupedit;       /**< A groupedit smart object, \
                                   needed for view and edit style.*/
   Workspace_Mode active_mode;
   Evas_Object *active_mode_object;

   struct {
        Evas_Object *obj;        /**< Container that contains groupedit.*/
        Evas_Coord prev_w;       /**< Container's width before separated mode.*/
        Evas_Coord prev_h;       /**< Container's height before separated mode.*/
        Evas_Coord dx;           /**< Groupedit's horizontal shift inside of \
                                   the container (for separate mode purpose). */
        Evas_Coord dy;           /**< Groupedit's vertical shift inside of the
                                   container (for separate mode purpose). */
   } container;
   struct {
        Evas_Object *obj;        /**< elm_menu object */
        Ws_Menu items;           /**< Context menu items structure*/
   } menu;
   Evas_Object *button_separate; /**< A button object, which switch (on/off)\
                                   separate mode of groupedit.*/
   Eina_List *guides;            /**< A guides list. Not implemented yet*/
   struct {
        double factor;           /**< zoom factor, coefficient for view \
                                   groupedit primitives with zoom.*/
        Evas_Object *control;    /**< ui element, which provide change zoom \
                                   factor. Markup in zoom_control.edc*/
   } zoom;
   Group *group;

   Evas_Object *panes;
   Evas_Object *group_navigator;

   struct {
        Evas_Object *highlight; /**< A highlight object */
        Evas_Object *space_hl; /**< A object area highlight*/
        Part_ *part; /**< Contain part name and it's state. need for callbacks of highlight. */
   } highlight;

   Change *change;
   int old_max_w, old_max_h;
   double old_align_x, old_align_y;
};
typedef struct _Ws_Smart_Data Ws_Smart_Data;

#define _evas_smart_ws "Evas_Smart_Ws"

#define WS_DATA_GET(o, ptr) \
   assert(o != NULL); \
   Ws_Smart_Data *ptr = evas_object_smart_data_get(o); \
   assert(ptr != NULL);

EVAS_SMART_SUBCLASS_NEW(_evas_smart_ws, _workspace,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, NULL);

static void
_obj_area_visible_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
   Eina_Bool visible;

   WS_DATA_GET(obj, sd);
   if (!groupedit_edit_object_parts_separated_is(sd->groupedit))
     {
        visible = groupedit_part_object_area_visible_get(sd->groupedit);
        groupedit_part_object_area_visible_set(sd->groupedit, !visible);
     }
}

TODO("need to remake it. create the public functions for workspace for ruler manipulation.")
static void
_menu_rulers_enabled_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("rulers"));
}

TODO("need to remake it. create the public functions for workspace for ruler manipulation.")
static void
_menu_rulers_abs_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("abs"));
}

TODO("need to remake it. create the public functions for workspace for ruler manipulation.")
static void
_menu_rulers_rel_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("rel"));
}

TODO("need to remake it. create the public functions for workspace for ruler manipulation.")
static void
_menu_rulers_both_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("abs&rel"));
}

static void
_separate_mode_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;

   assert(sd != NULL);

   workspace_separate_mode_set(sd->obj, true);
}

static void
_normal_mode_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;

   assert(sd != NULL);

   workspace_separate_mode_set(sd->obj, false);
}

static void
_zoom_factor_50(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   workspace_zoom_factor_set(ws, 0.5);
}

static void
_zoom_factor_100(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   workspace_zoom_factor_set(ws, 1.0);
}

static void
_zoom_factor_200(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;

   assert(ws != NULL);

   workspace_zoom_factor_set(ws, 2.0);
}

static void
_zoom_factor_in(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Ws_Smart_Data *ws = (Ws_Smart_Data *)data;

   assert(ws != NULL);

   workspace_zoom_factor_set(ws->obj, ws->zoom.factor + 0.1);
}

static void
_zoom_factor_out(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Ws_Smart_Data *ws = (Ws_Smart_Data *)data;

   assert(ws != NULL);

   workspace_zoom_factor_set(ws->obj, ws->zoom.factor - 0.1);
}

static void
_menu_undo_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Ws_Smart_Data *ws = (Ws_Smart_Data *)data;

   assert(ws != NULL);

   if (!ws->group) return;
   TODO("Implement undo")
   /*history_undo(ws->group->edit_object, 1);*/
}

static void
_menu_redo_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Ws_Smart_Data *ws = (Ws_Smart_Data *)data;

   assert(ws != NULL);

   if (!ws->group) return;

   TODO("Implement redo")
   /* history_redo(ws->group->edit_object, 1); */
}

static void
_init_ctx_menu(Ws_Smart_Data *ws, Evas_Object *parent)
{
   Evas_Object *menu;
   Ws_Menu *items = &ws->menu.items;

   assert(ws != NULL);
   assert(parent != NULL);

   ws->menu.obj = menu = elm_menu_add(elm_object_top_widget_get (parent));

   items->undo = elm_menu_item_add(menu, NULL, NULL, _("Undo"), _menu_undo_cb, ws);
   items->redo = elm_menu_item_add(menu, NULL, NULL, _("Redo"), _menu_redo_cb, ws);
   elm_menu_item_separator_add(menu, NULL);

   items->rulers = elm_menu_item_add(menu, NULL, NULL, _("Rulers"), NULL, NULL);
   items->rulers_enable = elm_menu_item_add(menu, items->rulers, NULL, _("Rulers enabled"), _menu_rulers_enabled_cb, ws->obj);
   elm_menu_item_icon_name_set(items->rulers_enable, EFLETE_IMG_PATH"context_menu-check.png");
   elm_menu_item_separator_add(menu, items->rulers);
   items->rulers_abs = elm_menu_item_add(menu, items->rulers, NULL, _("Absolute scale"), _menu_rulers_abs_cb, ws->obj);
   elm_menu_item_icon_name_set(items->rulers_abs, EFLETE_IMG_PATH"context_menu-bullet.png");
   items->rulers_rel = elm_menu_item_add(menu, items->rulers, NULL, _("Relative scale"), _menu_rulers_rel_cb, ws->obj);
   items->rulers_both = elm_menu_item_add(menu, items->rulers, NULL, _("Both"), _menu_rulers_both_cb, ws->obj);

   items->zoom = elm_menu_item_add(menu, NULL, NULL, _("Zoom"), NULL, NULL);
   items->zoom_in = elm_menu_item_add(menu, items->zoom, NULL, _("Zoom in"), _zoom_factor_in, ws);
   items->zoom_out = elm_menu_item_add(menu, items->zoom, NULL, _("Zoom out"), _zoom_factor_out, ws);
   elm_menu_item_separator_add(menu, items->zoom);
   items->zoom_fit = elm_menu_item_add(menu, items->zoom, NULL, _("Fit"), NULL, NULL);
   elm_menu_item_separator_add(menu, items->zoom);
   items->zoom_far = elm_menu_item_add(menu, items->zoom, NULL, _("50%"), _zoom_factor_50, ws->obj);
   items->zoom_normal = elm_menu_item_add(menu, items->zoom, NULL, _("100%"), _zoom_factor_100, ws->obj);
   items->zoom_near = elm_menu_item_add(menu, items->zoom, NULL, _("200%"), _zoom_factor_200, ws->obj);
   elm_menu_item_separator_add(menu, NULL);

   items->mode_normal = elm_menu_item_add(menu, NULL, NULL, _("Normal mode"), _normal_mode_click, ws);
   elm_menu_item_icon_name_set(items->mode_normal, EFLETE_IMG_PATH"context_menu-bullet.png");
   elm_object_item_disabled_set(items->mode_normal, true);
   items->mode_separate = elm_menu_item_add(menu, NULL, NULL, _("Separate mode"), _separate_mode_click, ws);
   elm_object_item_disabled_set(items->mode_separate, true);
   elm_menu_item_separator_add(menu, NULL);
   items->settings = elm_menu_item_add(menu, NULL, NULL, _("Settings..."), NULL, NULL);
   elm_object_item_disabled_set(items->settings, true);

   elm_object_item_disabled_set(items->zoom, true);
}

static void
_ws_ruler_toggle_cb(void *data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info)
{
   WS_DATA_GET(obj, sd)
   char *data_info = (char *)event_info;

   if (!strcmp(data_info, "rulers"))
     {
        if (evas_object_visible_get(sd->ruler_hor))
          {
             elm_menu_item_icon_name_set(sd->menu.items.rulers_enable, "");
             elm_layout_signal_emit(sd->scroller, "rulers,hide", "eflete");
          }
        else
          {
             elm_menu_item_icon_name_set(sd->menu.items.rulers_enable, EFLETE_IMG_PATH"context_menu-check.png");
             elm_layout_signal_emit(sd->scroller, "rulers,show", "eflete");
          }
     }
   else if (!strcmp(data_info, "abs"))
     {
        ewe_ruler_scale_visible_set(sd->ruler_hor, NULL, true);
        ewe_ruler_scale_visible_set(sd->ruler_ver, NULL, true);
        ewe_ruler_scale_visible_set(sd->ruler_hor, sd->scale_rel_hor, false);
        ewe_ruler_scale_visible_set(sd->ruler_ver, sd->scale_rel_ver, false);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, EFLETE_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_both, "");
     }
   else if (!strcmp(data_info, "rel"))
     {
        ewe_ruler_scale_visible_set(sd->ruler_hor, NULL, false);
        ewe_ruler_scale_visible_set(sd->ruler_ver, NULL, false);
        ewe_ruler_scale_visible_set(sd->ruler_hor, sd->scale_rel_hor, true);
        ewe_ruler_scale_visible_set(sd->ruler_ver, sd->scale_rel_ver, true);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, EFLETE_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_both, "");
     }
   else if (!strcmp(data_info, "abs&rel"))
     {
        ewe_ruler_scale_visible_set(sd->ruler_hor, NULL, true);
        ewe_ruler_scale_visible_set(sd->ruler_ver, NULL, true);
        ewe_ruler_scale_visible_set(sd->ruler_hor, sd->scale_rel_hor, true);
        ewe_ruler_scale_visible_set(sd->ruler_ver, sd->scale_rel_ver, true);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_both, EFLETE_IMG_PATH"context_menu-bullet.png");
     }

   free(data_info);
}

static void
_ws_ruler_abs_zero_move_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Groupedit_Geom *ge_geom = (Groupedit_Geom *)event_info;
   WS_DATA_GET(workspace, sd)

   Evas_Coord cross_size = 0, bg_x = 0, bg_y = 0;
   evas_object_geometry_get(sd->ruler_hor, NULL, NULL, NULL, &cross_size);
   evas_object_geometry_get(sd->background, &bg_x, &bg_y, NULL, NULL);

   ewe_ruler_zero_offset_set(sd->ruler_hor, NULL, ge_geom->x - cross_size - bg_x);
   ewe_ruler_zero_offset_set(sd->ruler_ver, NULL, ge_geom->y - cross_size - bg_y);
}

static void
_ws_ruler_rel_zero_move_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Groupedit_Geom *ge_geom = (Groupedit_Geom *)event_info;
   WS_DATA_GET(workspace, sd)

   Evas_Coord cross_size = 0, bg_x = 0, bg_y = 0;
   evas_object_geometry_get(sd->ruler_hor, NULL, NULL, NULL, &cross_size);
   evas_object_geometry_get(sd->background, &bg_x, &bg_y, NULL, NULL);

   ewe_ruler_zero_offset_set(sd->ruler_hor, sd->scale_rel_hor, ge_geom->x - cross_size - bg_x);
   /* placing 3 marks on relative scale along object's side: 0.0, 0.5, 1.0 */
   ewe_ruler_step_set(sd->ruler_hor, sd->scale_rel_hor, (ge_geom->w / 2) * sd->zoom.factor);

   ewe_ruler_zero_offset_set(sd->ruler_ver, sd->scale_rel_ver, ge_geom->y - cross_size - bg_y);
   /* placing 3 marks on relative scale along object's side: 0.0, 0.5, 1.0 */
   ewe_ruler_step_set(sd->ruler_ver, sd->scale_rel_ver, (ge_geom->h / 2) * sd->zoom.factor);
}

static void
_ws_smart_mouse_click_cb(void *data ,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   WS_DATA_GET(data, sd);

   if (ev->button == 3)
     {
        elm_menu_move(sd->menu.obj, ev->canvas.x, ev->canvas.y);
        evas_object_show(sd->menu.obj);
      }
}

static void
_ws_smart_mouse_move_cb(void *data, Evas *e,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int x, y;
   int dx, dy;
   Evas_Object *ws = (Evas_Object *)data;
   WS_DATA_GET(ws, sd)
   evas_pointer_output_xy_get(e, &x, &y);
   evas_object_geometry_get(sd->ruler_hor, &dx, NULL, NULL, NULL);
   evas_object_geometry_get(sd->ruler_ver, NULL, &dy, NULL, NULL);
   ewe_ruler_marker_absolute_set(sd->ruler_hor, sd->pointer_hor, x - dx);
   ewe_ruler_marker_absolute_set(sd->ruler_ver, sd->pointer_ver, y - dy);
}

static void __UNUSED__
_separate_smart_on_click(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Evas_Object *o = (Evas_Object *)data;
   WS_DATA_GET(o, sd)

   if (!sd->group) return;
   Eina_Bool sep = groupedit_edit_object_parts_separated_is(sd->groupedit);

   /* FIXME: stub until set mode api remake */
   if (!sep)
     _separate_mode_click(sd, NULL, NULL);
   else
     _normal_mode_click(sd, NULL, NULL);
}

static void
_sc_smart_drag_y_cb(void *data ,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *o = (Evas_Object *)data;
   WS_DATA_GET(o, sd)

   int x, y, w, h, w_ge, h_ge, w_cont;

   if ((groupedit_edit_object_parts_separated_is(sd->groupedit))
       && (!sd->scroll_flag))
     {
        elm_scroller_region_get(sd->scroller, NULL, &y, &w, &h);
        elm_scroller_child_size_get(sd->scroller, &w_ge, &h_ge);
        container_container_size_get(sd->container.obj, &w_cont, NULL);

        x = ceil((y * ((double)w_ge / h_ge)));
        if (y >=  (h_ge - h)) x += w_cont;
        elm_scroller_region_show(sd->scroller, x, y, w, h);
        sd->scroll_flag = 1;
     }
}

static void
_sc_smart_drag_x_cb(void *data ,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *o = (Evas_Object *)data;
   WS_DATA_GET(o, sd)

   int x, y, w, h, w_ge, h_ge, h_cont;
   if ((groupedit_edit_object_parts_separated_is(sd->groupedit))
       && (!sd->scroll_flag))
     {
        elm_scroller_region_get(sd->scroller, &x, NULL, &w, &h);
        elm_scroller_child_size_get(sd->scroller, &w_ge, &h_ge);
        container_container_size_get(sd->container.obj, NULL, &h_cont);

        y = ceil((x * ((double)h_ge / w_ge)));
        if (x >=  (w_ge - w)) y += h_cont;
        elm_scroller_region_show(sd->scroller, x, y, w, h);
        sd->scroll_flag = 2;
     }
}

void
_sc_smart_move_cb(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int bg_x, bg_y, gs_x, gs_y, gs_w, gs_h;
   int cross_size;
   Evas_Object *o = (Evas_Object *)data;
   WS_DATA_GET(o, sd)

   if (!sd->group) return;

   Evas_Object *object_area = groupedit_part_object_area_get(sd->groupedit);

   evas_object_geometry_get(sd->ruler_hor, &cross_size, NULL, NULL, NULL);
   evas_object_geometry_get(sd->background, &bg_x, &bg_y, NULL, NULL);
   evas_object_geometry_get(sd->group->edit_object, &gs_x, &gs_y, &gs_w, &gs_h);
   cross_size -= bg_x;
   ewe_ruler_zero_offset_set(sd->ruler_hor, NULL, gs_x - bg_x - cross_size);
   ewe_ruler_zero_offset_set(sd->ruler_ver, NULL, gs_y - bg_y - cross_size);

   if ((groupedit_edit_object_parts_separated_is(sd->groupedit))
       && (sd->scroll_flag < 2))
     evas_object_smart_callback_call(sd->scroller, "vbar,drag", NULL);
   sd->scroll_flag = 0;

   evas_object_geometry_get(object_area, &gs_x, &gs_y, &gs_w, &gs_h);

   ewe_ruler_zero_offset_set(sd->ruler_hor, sd->scale_rel_hor, gs_x - bg_x - cross_size);
   /* placing 3 marks on relative scale along object's side: 0.0, 0.5, 1.0 */
   ewe_ruler_step_set(sd->ruler_hor, sd->scale_rel_hor, (gs_w / 2) * sd->zoom.factor);

   ewe_ruler_zero_offset_set(sd->ruler_ver, sd->scale_rel_ver, gs_y - bg_y - cross_size);
   /* placing 3 marks on relative scale along object's side: 0.0, 0.5, 1.0 */
   ewe_ruler_step_set(sd->ruler_ver, sd->scale_rel_ver, (gs_h / 2) * sd->zoom.factor);
}

#define DEFAULT_STEP 50

Eina_Bool
workspace_zoom_factor_set(Evas_Object *obj, double factor)
{
   if ((factor < 0.01) || (factor > 20)) return false;
   WS_DATA_GET(obj, sd)

   if ((groupedit_edit_object_parts_separated_is(sd->groupedit)) ||
       (fabs(sd->zoom.factor - factor) <= 0.001) || (!sd->group))
     return false;

   sd->zoom.factor = factor;
   groupedit_zoom_factor_set(sd->groupedit, factor);

   /* calculate step for VERTICAL RELATIVE */
   ewe_ruler_step_set(sd->ruler_ver, sd->scale_rel_ver, DEFAULT_STEP * sd->zoom.factor);

   /* calculate step for VERTICAL ABSOLUTE */
   ewe_ruler_step_set(sd->ruler_ver, NULL, DEFAULT_STEP * sd->zoom.factor);

   /* calculate step for HORIZONTAL RELATIVE */
   ewe_ruler_step_set(sd->ruler_hor, sd->scale_rel_hor, DEFAULT_STEP * sd->zoom.factor);

   /* calculate step for HORIZONTAL ABSOLUTE */
   ewe_ruler_step_set(sd->ruler_hor, NULL, DEFAULT_STEP * sd->zoom.factor);

   if (fabs(factor - 1.0) > 0.001)
     {
        container_border_hide(sd->container.obj);

        ui_menu_disable_set(ap.menu, MENU_VIEW_WORKSPACE_SEPARATE, true);
        Ws_Menu *items = &sd->menu.items;
        elm_object_item_disabled_set(items->mode_normal, true);
        elm_object_item_disabled_set(items->mode_separate, true);
        elm_object_disabled_set(sd->button_separate, true);
     }
   else
     {
        container_border_show(sd->container.obj);

        ui_menu_disable_set(ap.menu, MENU_VIEW_WORKSPACE_SEPARATE, false);

        Ws_Menu *items = &sd->menu.items;
        elm_object_item_disabled_set(items->mode_normal, false);
        elm_object_item_disabled_set(items->mode_separate, false);
        elm_object_disabled_set(sd->button_separate, false);
     }

   workspace_edit_object_recalc(obj);
   evas_object_smart_changed(sd->groupedit);

   return true;
}

#undef DEFAULT_STEP

double
workspace_zoom_factor_get(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);
   return sd->zoom.factor;
}

static void
_ws_mouse_move_cb(void *data, Evas *e,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int x, y;
   int dx, dy;
   Evas_Object *ws_obj = (Evas_Object *)data;
   WS_DATA_GET(ws_obj, sd)
   evas_pointer_output_xy_get (e, &x, &y);
   evas_object_geometry_get(sd->ruler_hor, &dx, NULL, NULL, NULL);
   evas_object_geometry_get(sd->ruler_ver, NULL, &dy, NULL, NULL);
   ewe_ruler_marker_absolute_set(sd->ruler_hor, sd->pointer_hor, x - dx);
   ewe_ruler_marker_absolute_set(sd->ruler_ver, sd->pointer_ver, y - dy);
}

static void
_highlight_drag_start_cb(void *data,
                         Evas_Object * obj __UNUSED__,
                         void *ei)
{
   Evas_Object *ws_obj = (Evas_Object *)data;
   WS_DATA_GET(ws_obj, sd)
   Highlight_Events *events = (Highlight_Events *)ei;
   Part_ *part = sd->highlight.part;

   assert(sd->change == NULL);

   if (events->descr != MIDDLE)
     {
        sd->old_max_w = edje_edit_state_max_w_get(sd->group->edit_object, part->name,
                                                  part->current_state->parsed_name,
                                                  part->current_state->parsed_val);
        sd->old_max_h = edje_edit_state_max_h_get(sd->group->edit_object, part->name,
                                                  part->current_state->parsed_name,
                                                  part->current_state->parsed_val);
     }
   else
     {
        sd->old_align_x = edje_edit_state_align_x_get(sd->group->edit_object, part->name,
                                                      part->current_state->parsed_name,
                                                      part->current_state->parsed_val);
        sd->old_align_y = edje_edit_state_align_y_get(sd->group->edit_object, part->name,
                                                      part->current_state->parsed_name,
                                                      part->current_state->parsed_val);
     }
   sd->change = change_add(NULL);
}

static void
_highlight_drag_stop_cb(void *data,
                        Evas_Object * obj __UNUSED__,
                        void *ei)
{
   Evas_Object *ws_obj = (Evas_Object *)data;
   WS_DATA_GET(ws_obj, sd)
   Highlight_Events *events = (Highlight_Events *)ei;
   int new_max_w, new_max_h;
   double new_align_x, new_align_y;
   Eina_Stringshare *msg;
   Part_ *part = sd->highlight.part;

   assert(sd->change != NULL);

   if (events->descr != MIDDLE)
     {
        new_max_w = edje_edit_state_max_w_get(sd->group->edit_object, part->name,
                                              part->current_state->parsed_name,
                                              part->current_state->parsed_val);
        new_max_h = edje_edit_state_max_h_get(sd->group->edit_object, part->name,
                                              part->current_state->parsed_name,
                                              part->current_state->parsed_val);
        if ((new_max_h != sd->old_max_h) || (new_max_w != sd->old_max_w))
          {
             msg = eina_stringshare_printf(_("max size changed from [%dx%d] to [%dx%d]"), sd->old_max_w, sd->old_max_h, new_max_w, new_max_h);
             change_description_set(sd->change, msg);
             eina_stringshare_del(msg);
             history_change_add(sd->group->history, sd->change);
          }
        else
          change_free(sd->change);
     }
   else
     {
        new_align_x = edje_edit_state_align_x_get(sd->group->edit_object, part->name,
                                                      part->current_state->parsed_name,
                                                      part->current_state->parsed_val);
        new_align_y = edje_edit_state_align_y_get(sd->group->edit_object, part->name,
                                                      part->current_state->parsed_name,
                                                      part->current_state->parsed_val);
        if ((fabs(new_align_x - sd->old_align_x) > DBL_EPSILON) ||
            (fabs(new_align_y - sd->old_align_y) > DBL_EPSILON))
          {
             msg = eina_stringshare_printf(_("align changed from [%.2fx%.2f] to [%.2fx%.2f]"),
                                           sd->old_align_x, sd->old_align_y, new_align_x, new_align_y);
             change_description_set(sd->change, msg);
             eina_stringshare_del(msg);
             history_change_add(sd->group->history, sd->change);
          }
        else
          change_free(sd->change);
     }
   sd->change = NULL;
}

static void
_highlight_changed_cb(void *data,
                      Evas_Object * obj __UNUSED__,
                      void *ei)
{
   Highlight_Events *events = (Highlight_Events *)ei;

   Evas_Object *ws_obj = (Evas_Object *)data;
   WS_DATA_GET(ws_obj, sd)

   Part_ *part = sd->highlight.part;

   if ((!sd->group) || (!part)) return;
   Evas_Object *obj_area = groupedit_part_object_area_get(sd->groupedit);
   Evas_Coord x, y, w, h;
   evas_object_geometry_get(obj_area, &x, &y, &w, &h);

   assert(sd->change != NULL);

   if (events->descr != MIDDLE)
     {
        int old_max_w = 0, old_max_h = 0, new_max_w, new_max_h;
        int min_w = 0, min_h = 0;
        min_w = edje_edit_state_min_w_get(sd->group->edit_object, part->name,
                                          part->current_state->parsed_name,
                                          part->current_state->parsed_val);
        min_h = edje_edit_state_min_h_get(sd->group->edit_object, part->name,
                                          part->current_state->parsed_name,
                                          part->current_state->parsed_val);

        old_max_w = edje_edit_state_max_w_get(sd->group->edit_object, part->name,
                                              part->current_state->parsed_name,
                                              part->current_state->parsed_val);
        old_max_h = edje_edit_state_max_h_get(sd->group->edit_object, part->name,
                                              part->current_state->parsed_name,
                                              part->current_state->parsed_val);
        new_max_w = (events->w / sd->zoom.factor) <= min_w ? min_w : (events->w / sd->zoom.factor);
        new_max_h = (events->h / sd->zoom.factor) <= min_h ? min_h : (events->h / sd->zoom.factor);
        if (new_max_w != old_max_w)
          {
             editor_state_max_w_set(sd->group->edit_object, sd->change, true, part->name,
                                    part->current_state->parsed_name, part->current_state->parsed_val,
                                    new_max_w);
          }
        if (new_max_h != old_max_h)
          {
             editor_state_max_h_set(sd->group->edit_object, sd->change, true, part->name,
                                    part->current_state->parsed_name, part->current_state->parsed_val,
                                    new_max_h);
          }
     }
   else
     {
        double align_x = (double)(events->x - x) / (double)(w - events->w);
        double align_y = (double)(events->y - y) / (double)(h - events->h);
        double old_align_x, old_align_y;

        if ((w == events->w) || (align_x < 0)) align_x = 0;
        if ((h == events->h) || (align_y < 0)) align_y = 0;

        if (align_x > 1.0) align_x = 1.0;
        if (align_y > 1.0) align_y = 1.0;
        old_align_x = edje_edit_state_align_x_get(sd->group->edit_object, part->name,
                                                  part->current_state->parsed_name,
                                                  part->current_state->parsed_val);
        old_align_y = edje_edit_state_align_y_get(sd->group->edit_object, part->name,
                                                  part->current_state->parsed_name,
                                                  part->current_state->parsed_val);

        if (align_x != old_align_x)
          {
             editor_state_align_x_set(sd->group->edit_object, sd->change, true, part->name,
                                      part->current_state->parsed_name, part->current_state->parsed_val,
                                      align_x);
          }
        if (align_y != old_align_y)
          {
             editor_state_align_y_set(sd->group->edit_object, sd->change, true, part->name,
                                      part->current_state->parsed_name, part->current_state->parsed_val,
                                      align_y);
          }
     }

   /*project_changed(false);
   workspace_edit_object_recalc(ws_obj);*/
   evas_object_smart_callback_call(ws_obj, "part,changed", part);
}

static Eina_Bool
_workspace_highlight_set(Evas_Object *obj, Part_ *part)
{
   Evas_Object *follow;
   WS_DATA_GET(obj, sd)

   assert(part != NULL);
   assert(sd->groupedit != NULL);

   sd->highlight.part = part;

   groupedit_edit_object_part_select(sd->groupedit, part->name);
   follow = groupedit_edit_object_part_draw_get(sd->groupedit, part->name);
   if (!follow)
     {
        sd->highlight.part = NULL;
        return false;
     }
   highlight_object_follow(sd->highlight.highlight, follow);

   follow = groupedit_part_object_area_get(sd->groupedit);
   highlight_object_follow(sd->highlight.space_hl, follow);

   if (!groupedit_edit_object_parts_separated_is(sd->groupedit))
     {
        evas_object_hide(sd->highlight.space_hl);
        evas_object_show(sd->highlight.highlight);

        evas_object_event_callback_add(sd->highlight.highlight,
                                       EVAS_CALLBACK_MOUSE_MOVE,
                                       _ws_mouse_move_cb, obj);
        evas_object_smart_callback_add(sd->highlight.highlight, "hl,changed",
                                       _highlight_changed_cb, obj);
        evas_object_smart_callback_add(sd->highlight.highlight, "hl,drag,start",
                                       _highlight_drag_start_cb, obj);
        evas_object_smart_callback_add(sd->highlight.highlight, "hl,drag,stop",
                                       _highlight_drag_stop_cb, obj);
     }
   return true;
}

Eina_Bool
workspace_highlight_unset(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd)

   if ((!sd->highlight.highlight) || (!sd->highlight.space_hl)) return false;
   highlight_object_unfollow(sd->highlight.highlight);
   highlight_object_unfollow(sd->highlight.space_hl);
   sd->highlight.part = NULL;
   evas_object_hide(sd->highlight.space_hl);
   evas_object_hide(sd->highlight.highlight);
   if (sd->groupedit)
     groupedit_edit_object_part_select(sd->groupedit, NULL);

   evas_object_event_callback_del(sd->highlight.highlight,
                                  EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb);
   evas_object_smart_callback_del(sd->highlight.highlight, "hl,changed",
                                  _highlight_changed_cb);
   evas_object_smart_callback_del(sd->highlight.highlight, "hl,drag,start",
                                  _highlight_drag_start_cb);
   evas_object_smart_callback_del(sd->highlight.highlight, "hl,drag,stop",
                                  _highlight_drag_stop_cb);
   return true;
}

static void
_sc_wheel_move(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Evas_Event_Mouse_Wheel *ev = (Evas_Event_Mouse_Wheel *)event_info;
   Evas_Object *workspace = (Evas_Object *)data;
   const Evas_Modifier *mods;

   mods = evas_key_modifier_get(e);
   if (evas_key_modifier_is_set(mods, "Control") && (ev->direction == 0))
     {
        /* if wheel goes up (zoom in), but Evas Event returns -1
           if wheel goes down (zoom out), but Evas Event returns +1 */
        double factor = workspace_zoom_factor_get(workspace);
        workspace_zoom_factor_set(workspace, factor - ev->z * 0.1);
     }
}

static void
_workspace_smart_add(Evas_Object *o)
{
   assert(o != NULL);

   /* Allocate memory for workspace smart data*/
   EVAS_SMART_DATA_ALLOC(o, Ws_Smart_Data)

   _workspace_parent_sc->add(o);
}

static void
_mode_changed(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Workspace_Mode mode;
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;

   /* delete all object besides groupedit, because all these objects we created
    * on mode chage. It's do for create, alwayes actual and correct object, and
    * eliminates from updates, like live_view update mehanism. So, Profit! */
   container_confine_unset(sd->container.obj);
   if ((sd->active_mode == MODE_NORMAL) || (sd->active_mode == MODE_SEPARATE))
     evas_object_hide(sd->groupedit);
   else
     evas_object_del(sd->active_mode_object);

   mode = elm_radio_state_value_get(obj);
   switch (mode)
     {
      case MODE_NORMAL:
         container_content_set(sd->container.obj, sd->groupedit);
         evas_object_show(sd->groupedit);
         workspace_separate_mode_set(sd->obj, false);
         break;
      case MODE_SEPARATE:
         container_content_set(sd->container.obj, sd->groupedit);
         evas_object_show(sd->groupedit);
         workspace_separate_mode_set(sd->obj, true);
         container_border_hide(sd->container.obj);
         break;
      /* case MODE_ANIMATOR: break; */
      /* case MODE_CODE: break; */
      case MODE_DEMO:
         {
            /* return container to default state */
            workspace_separate_mode_set(sd->obj, false);
            container_border_show(sd->container.obj);

            sd->active_mode_object = demo_add(sd->scroller, sd->group);
            evas_object_show(sd->active_mode_object);
            container_content_set(sd->container.obj, sd->active_mode_object);
            break;
         }
      default: break;
     }
   sd->active_mode = mode;
}

static void
_zoom_part_add(Ws_Smart_Data *sd)
{
   Evas_Object *image, *slider_zoom;
   Evas_Object *button_resize, *button_zoom;

   button_resize = elm_button_add(sd->scroller);
   elm_box_pack_end(sd->bottom_box, button_resize);
   evas_object_show(button_resize);

   button_zoom = elm_button_add(sd->scroller);
   elm_object_text_set(button_zoom, N_("100%"));
   elm_box_pack_end(sd->bottom_box, button_zoom);
   evas_object_show(button_zoom);

   slider_zoom = elm_slider_add(sd->scroller);
   image = elm_image_add(sd->bottom_box);
   elm_image_file_set(image, EFLETE_IMG_PATH"scale-smaller.png", NULL);
   elm_object_part_content_set(slider_zoom, "elm.swallow.icon", image);

   image = elm_image_add(sd->scroller);
   elm_image_file_set(image, EFLETE_IMG_PATH"scale-larger.png", NULL);
   elm_object_part_content_set(slider_zoom, "elm.swallow.end", image);
   elm_box_pack_end(sd->bottom_box, slider_zoom);
   evas_object_show(slider_zoom);
}

static void
_mode_part_add(Ws_Smart_Data *sd)
{
   Evas_Object *radio_mode, *radio_group;

   radio_group = radio_mode = elm_radio_add(sd->scroller);
   elm_radio_state_value_set(radio_mode, MODE_NORMAL);
   elm_radio_value_set(radio_mode, true);
   evas_object_smart_callback_add(radio_mode, "changed", _mode_changed, sd);
   elm_box_pack_end(sd->bottom_box, radio_mode);
   evas_object_show(radio_mode);

   radio_mode = elm_radio_add(sd->scroller);
   elm_radio_state_value_set(radio_mode, MODE_SEPARATE);
   evas_object_smart_callback_add(radio_mode, "changed", _mode_changed, sd);
   elm_box_pack_end(sd->bottom_box, radio_mode);
   evas_object_show(radio_mode);
   elm_radio_group_add(radio_mode, radio_group);

   /*
   radio_mode = elm_radio_add(sd->scroller);
   elm_radio_state_value_set(radio_mode, MODE_ANIMATOR);
   evas_object_smart_callback_add(radio_mode, "changed", _mode_changed, sd);
   elm_box_pack_end(sd->bottom_box, radio_mode);
   evas_object_show(radio_mode);
   elm_radio_group_add(radio_mode, radio_group);
   */

   /*
   radio_mode = elm_radio_add(sd->scroller);
   elm_radio_state_value_set(radio_mode, MODE_CODE);
   evas_object_smart_callback_add(radio_mode, "changed", _mode_changed, sd);
   elm_box_pack_end(sd->bottom_box, radio_mode);
   evas_object_show(radio_mode);
   elm_radio_group_add(radio_mode, radio_group);
   */

   radio_mode = elm_radio_add(sd->scroller);
   elm_radio_state_value_set(radio_mode, MODE_DEMO);
   evas_object_smart_callback_add(radio_mode, "changed", _mode_changed, sd);
   elm_box_pack_end(sd->bottom_box, radio_mode);
   evas_object_show(radio_mode);
   elm_radio_group_add(radio_mode, radio_group);
}

static void
_resize_part_add(Ws_Smart_Data *sd)
{
   Evas_Object *check_binding, *image;
   Evas_Object *spinner_widght, *spinner_height, *check_size_chanage;

   image = elm_image_add(sd->scroller);
   elm_image_resizable_set(image, EINA_TRUE, EINA_FALSE);
   elm_image_aspect_fixed_set(image, EINA_TRUE);
   elm_image_file_set(image, EFLETE_IMG_PATH"crop.png", NULL);
   elm_box_pack_end(sd->bottom_box, image);
   evas_object_show(image);

   spinner_widght = elm_spinner_add(sd->scroller);
   elm_box_pack_end(sd->bottom_box, spinner_widght);
   evas_object_show(spinner_widght);

   check_binding = elm_check_add(sd->scroller);
   elm_box_pack_end(sd->bottom_box, check_binding);
   evas_object_show(check_binding);

   spinner_height = elm_spinner_add(sd->scroller);
   elm_box_pack_end(sd->bottom_box, spinner_height);
   evas_object_show(spinner_height);

   check_size_chanage = elm_check_add(sd->scroller);
   elm_box_pack_end(sd->bottom_box, check_size_chanage);
   evas_object_show(check_size_chanage);
}

static void
_bottom_panel_add(Ws_Smart_Data *sd)
{

#define SEPARATOR_ADD() \
   separator = elm_separator_add(sd->bottom_box); \
   elm_box_pack_end(sd->bottom_box, separator); \
   evas_object_show(separator);

   Evas_Object *separator;

   sd->bottom_box = elm_box_add(sd->scroller);
   elm_box_align_set(sd->bottom_box, 0.0, 0.5);
   elm_box_horizontal_set(sd->bottom_box, true);
   elm_box_padding_set(sd->bottom_box, 6, 0);
   elm_object_part_content_set(sd->scroller, "bottom_panel", sd->bottom_box);
   evas_object_show(sd->bottom_box);

   _zoom_part_add(sd);
   SEPARATOR_ADD()
   _mode_part_add(sd);
   SEPARATOR_ADD()
   _resize_part_add(sd);
   SEPARATOR_ADD()

#undef SEPARATOR_ADD
}

static Eina_Bool
_workspace_child_create(Evas_Object *o, Evas_Object *parent)
{
   WS_DATA_GET(o, priv);

   assert(parent != NULL);

   /* adding self-reference */
   priv->obj = o;

   Evas *e = evas_object_evas_get(o);
   Evas_Object *icon = NULL;

   /* Here create evas image, whitch will be background for workspace*/
   IMAGE_ADD_NEW(parent, priv->background, "bg", "tile");
   evas_object_smart_member_add(priv->background, o);

   /* Clipper needed for check mouse events*/
   priv->events = evas_object_rectangle_add(e);
   evas_object_color_set(priv->events, 0, 0, 0, 0);
   evas_object_smart_member_add(priv->events, o);
   evas_object_event_callback_add(priv->events, EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_smart_mouse_move_cb, o);
   evas_object_event_callback_add(priv->events, EVAS_CALLBACK_MOUSE_DOWN,
                                  _ws_smart_mouse_click_cb, o);

   /* create empty groupspace object */
   priv->groupedit = NULL;

   /* add panes */
   priv->panes = elm_panes_add(parent);
   evas_object_size_hint_weight_set(priv->panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(priv->panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_right_min_size_set(priv->panes, 225);
   elm_panes_content_right_size_set(priv->panes, 0); /* default is min size */
   evas_object_smart_member_add(priv->panes, o);

   /* using scroller in workspace, with special style*/
   priv->scroll_flag = 0;
   priv->zoom.factor = 1.0;
   priv->scroller = elm_scroller_add(parent);
   elm_object_style_set(priv->scroller, "workspace");
   elm_scroller_content_min_limit(priv->scroller, false, false);

   elm_object_part_content_set(priv->panes, "left",
                               priv->scroller);

   evas_object_event_callback_add(priv->scroller, EVAS_CALLBACK_MOUSE_WHEEL,
                                  _sc_wheel_move, o);
   evas_object_smart_callback_add(priv->scroller, "scroll",
                                  _sc_smart_move_cb, o);
   evas_object_smart_callback_add(priv->scroller, "vbar,drag",
                                  _sc_smart_drag_y_cb, o);
   evas_object_smart_callback_add(priv->scroller, "hbar,drag",
                                  _sc_smart_drag_x_cb, o);
   evas_object_smart_member_add(priv->scroller, o);

   /* button for switch mode of view: separated or normal*/
   priv->button_separate = elm_button_add(priv->scroller);
   elm_object_style_set(priv->button_separate, "anchor");
   IMAGE_ADD_NEW(priv->scroller, icon, "icon", "separate");
   elm_object_part_content_set(priv->button_separate, NULL, icon);
   evas_object_smart_callback_add(priv->button_separate, "clicked",
                                  _separate_smart_on_click, o);
   evas_object_smart_member_add(priv->button_separate, o);

   /* Add members of workspace into scroller markup field''s*/
   elm_object_part_content_set(priv->scroller, "elm.swallow.background",
                               priv->background);
   elm_object_part_content_set(priv->scroller, "elm.swallow.overlay",
                               priv->events);
   elm_object_part_content_set(priv->scroller, "elm.swallow.button",
                               priv->button_separate);

   /* Add bottom panel to workspace */
   _bottom_panel_add(priv);
   priv->active_mode = MODE_NORMAL;

   Evas_Object *edje = elm_layout_edje_get(priv->scroller);
   priv->clipper = (Evas_Object *) edje_object_part_object_get(edje, "clipper");

   /* create rulers*/
#define RULER(RUL, SCAL, POINT, SWALL) \
   RUL = ewe_ruler_add(priv->scroller); \
   SCAL = ewe_ruler_scale_add(RUL, "relative"); \
   ewe_ruler_format_set(RUL, SCAL, "%.1f"); \
   ewe_ruler_scale_visible_set(RUL, SCAL, false); \
   ewe_ruler_value_step_set(RUL, SCAL, 0.5); \
   POINT = ewe_ruler_marker_add(RUL, "pointer"); \
   elm_object_part_content_set(priv->scroller, SWALL, RUL); \
   evas_object_smart_member_add(RUL, o);

   RULER(priv->ruler_hor, priv->scale_rel_hor, priv->pointer_hor, "elm.swallow.hruler");
   RULER(priv->ruler_ver, priv->scale_rel_ver, priv->pointer_ver, "elm.swallow.vruler");
   ewe_ruler_horizontal_set(priv->ruler_ver, false);
   evas_object_smart_callback_add(o, "ruler,toggle",
                                  _ws_ruler_toggle_cb, NULL);

#undef RULER
   /* init context menu */
   _init_ctx_menu(priv, parent);
   evas_object_smart_member_add(priv->menu.obj, o);

   /* Simple initialize variavbles of smart data */
   priv->zoom.factor = 1.0;
   priv->zoom.control = NULL;

   priv->group = NULL;
   priv->guides = NULL;

   return true;
}


static void
_workspace_smart_del(Evas_Object *o)
{
   WS_DATA_GET(o, sd);

   evas_object_del(sd->highlight.space_hl);
   evas_object_del(sd->highlight.highlight);
   _workspace_parent_sc->del(o);
}

static void
_workspace_smart_show(Evas_Object *o)
{
   if (evas_object_visible_get(o)) return;

   WS_DATA_GET(o, sd)

   evas_object_show(sd->button_separate);
   evas_object_show(sd->scroller);
   evas_object_show(sd->panes);
   evas_object_show(sd->events);

   if (sd->groupedit)
     evas_object_show(sd->groupedit);
   if (sd->ruler_hor)
     evas_object_show(sd->ruler_hor);
   if (sd->ruler_ver)
     evas_object_show(sd->ruler_ver);

   evas_object_show(sd->background);
   _workspace_parent_sc->show(o);
}

static void
_workspace_smart_hide(Evas_Object *o)
{
   if (!evas_object_visible_get(o)) return;

   WS_DATA_GET(o, sd);

   if (sd->groupedit) evas_object_hide(sd->groupedit);
   if (sd->ruler_hor) evas_object_hide(sd->ruler_hor);
   if (sd->ruler_ver) evas_object_hide(sd->ruler_ver);

   evas_object_hide(sd->button_separate);
   evas_object_hide(sd->scroller);
   evas_object_hide(sd->panes);
   evas_object_hide(sd->events);

   _workspace_parent_sc->hide(o);
}

static void
_workspace_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ox, oy, ow, oh;
   WS_DATA_GET(o, sd)

   evas_object_geometry_get(o, &ox, &oy, &ow, &oh);
   if ((ow == w) && (oh == h)) return;
   evas_object_resize(sd->panes, w, h);

   evas_object_smart_changed(o);
}

static void
_workspace_color_set(Evas_Object *o, int r, int g, int b, int a)
{
   WS_DATA_GET(o, sd);

   evas_object_color_set(sd->background, r, g, b, a);
}

static void
_workspace_smart_set_user(Evas_Smart_Class *sc)
{
   assert(sc != NULL);

   sc->add = _workspace_smart_add;
   sc->del = _workspace_smart_del;
   sc->show = _workspace_smart_show;
   sc->hide = _workspace_smart_hide;
   sc->color_set = _workspace_color_set;

   sc->resize = _workspace_smart_resize;
   sc->calculate = NULL;
}

static void
_on_container_TL_move(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;
   Container_Geom *geom = (Container_Geom *)event_info;

   assert(sd != NULL);
   assert(geom != NULL);

   Evas_Coord x, y, w, h;
   elm_scroller_region_get(sd->scroller, &x, &y, &w, &h);
   if (geom->dx < 0) x -= geom->dx;
   if (geom->dy < 0) y -= geom->dy;
   elm_scroller_region_show(sd->scroller, x, y, w, h);

   sd->container.dx += geom->dx;
   sd->container.dy += geom->dy;
}

#define PADDING_SIZE 40

static void
_on_groupedit_geometry_changed(void *data,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   Ws_Smart_Data *sd __UNUSED__= (Ws_Smart_Data *)data;

   assert(sd != NULL);

   Container_Geom *geom __UNUSED__ = (Container_Geom *)event_info;

   Evas_Coord x, y, w, h, ruler_ver_w, ruler_hor_h, hrb_w, hrb_h;

   /* getting size of rulers and handlers of container. Bottom and Left paddings
      are calculating according to them
      (PADDING_SIZE - ruler_size - handler_size).
   */
   evas_object_geometry_get(sd->groupedit, &x, &y, &w, &h);
   evas_object_geometry_get(sd->ruler_ver, NULL, NULL, &ruler_ver_w, NULL);
   evas_object_geometry_get(sd->ruler_hor, NULL, NULL, NULL, &ruler_hor_h);
   container_handler_size_get(sd->container.obj, NULL, NULL, &hrb_w, &hrb_h);

   /* !!doing some padding calculation, so we could show real size of groupedit!!
      =====================================================================
      Real size of groupedit is being used here as additional for paddings.

      >  For top and left padding it's default size of padding + difference
      between x/y position of groupedit and real x/y position of the most left
      part (it's pad_x/pad_y).

      >  For right and bottom padding we need to calculate different value.
      it is actually difference between size of groupedit ((x + w) or (y + h))
      and real size of groupedit (all of that including position of it).
      Also we should compensate additional size of padding and ruler size
      (ruler covers most of top/left pads). */
   Evas_Coord pad_x, pad_y, pad_w, pad_h;
   pad_x = PADDING_SIZE + abs(geom->x - x);
   pad_y = PADDING_SIZE + abs(geom->y - y);
   pad_w = PADDING_SIZE + abs((geom->x + geom->w) - (x + w)) - ruler_ver_w - hrb_w;
   pad_h = PADDING_SIZE + abs((geom->y + geom->h) - (y + h)) - ruler_hor_h - hrb_h;

   container_padding_size_set(sd->container.obj, pad_x, pad_y, pad_w, pad_h);
}

static void
_on_groupedit_part_select(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Part_ *part = event_info;

   WS_DATA_GET(workspace, sd);

   group_navigator_part_select(sd->group_navigator, part);
}

static void
_on_group_navigator_part_select(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Part_ *part = event_info;

   WS_DATA_GET(workspace, sd);

   TODO("Combine this methods to one")
   workspace_highlight_unset(workspace);
   _workspace_highlight_set(workspace, part);
   groupedit_edit_object_part_item_selected_set(sd->groupedit, part->current_item_name, true);

   evas_object_smart_callback_call(ap.win, SIGNAL_PART_SELECTED, (void *)part);
}

static void
_on_group_navigator_part_restacked(void *data,
                                   Evas_Object *obj __UNUSED__,
                                   void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Editor_Part_Restack *ei = event_info;

   WS_DATA_GET(workspace, sd);

   groupedit_edit_object_part_restack(sd->groupedit, ei->part_name, ei->relative_part_name);
}

static void
_on_group_navigator_part_state_select(void *data,
                                Evas_Object *obj __UNUSED__,
                                void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Part_ *part = event_info;

   WS_DATA_GET(workspace, sd);

   groupedit_edit_object_part_state_set(sd->groupedit, part);

   evas_object_smart_callback_call(ap.win, SIGNAL_PART_STATE_SELECTED, (void *)part);
}

static void
_on_group_navigator_part_visible_changed(void *data,
                                Evas_Object *obj __UNUSED__,
                                void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Part_ *part = event_info;

   WS_DATA_GET(workspace, sd);

   groupedit_part_visible_set(sd->groupedit, part);
}


static void
_on_groupedit_part_unselect(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   Evas_Object *workspace = (Evas_Object *)data;

   WS_DATA_GET(workspace, sd);

   group_navigator_part_select(sd->group_navigator, NULL);
}

Evas_Object *
workspace_add(Evas_Object *parent, Group *group)
{
   Evas *e = NULL;
   Evas_Object *obj = NULL;
   Evas_Coord ruler_ver_w, ruler_hor_h, hrb_w, hrb_h;


   assert(parent != NULL);

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _workspace_smart_class_new());
   if (!_workspace_child_create(obj, parent))
     {
        evas_object_del(obj);
        return NULL;
     }
   WS_DATA_GET(obj, sd);

   sd->group_navigator = group_navigator_add(group);
   evas_object_size_hint_weight_set(sd->group_navigator, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(sd->group_navigator, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_member_add(sd->group_navigator, obj);
   elm_object_part_content_set(sd->panes, "right", sd->group_navigator);
   evas_object_smart_callback_add(sd->group_navigator, SIGNAL_GROUP_NAVIGATOR_PART_SELECTED,
                                  _on_group_navigator_part_select, obj);
   evas_object_smart_callback_add(sd->group_navigator, SIGNAL_GROUP_NAVIGATOR_PART_RESTACKED,
                                  _on_group_navigator_part_restacked, obj);
   evas_object_smart_callback_add(sd->group_navigator, SIGNAL_GROUP_NAVIGATOR_PART_STATE_SELECTED,
                                  _on_group_navigator_part_state_select, obj);
   evas_object_smart_callback_add(sd->group_navigator, SIGNAL_GROUP_NAVIGATOR_PART_VISIBLE_CHANGED,
                                  _on_group_navigator_part_visible_changed, obj);


   /* create conteiner with handlers */
   sd->container.obj = container_add(sd->scroller);
   evas_object_smart_callback_add(sd->container.obj, "handler,TL,moved",
                                  _on_container_TL_move, sd);
   container_style_set(sd->container.obj, "scroller");

   /* create groupedit - editable */
   sd->groupedit = groupedit_add(sd->scroller, group);
   /* it temporary solution white not implemented preference module
      and not finished config module */
   evas_object_color_set(sd->groupedit, 0, 0, 0, 255);
   evas_object_smart_callback_add(sd->groupedit, "geometry,changed",
                                  _on_groupedit_geometry_changed, sd);
   container_content_set(sd->container.obj, sd->groupedit);

   // ???
   evas_object_focus_set(sd->groupedit, true);

   sd->group = group;
   elm_menu_item_icon_name_set(sd->menu.items.mode_normal,
                               EFLETE_IMG_PATH"context_menu-bullet.png");
   elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");

   container_handler_size_set(sd->container.obj, 8, 8, 8, 8);
   evas_object_smart_callback_add(sd->groupedit, SIGNAL_GROUPEDIT_PART_SELECTED,
                                  _on_groupedit_part_select, obj);
   evas_object_smart_callback_add(sd->groupedit, SIGNAL_GROUPEDIT_PART_UNSELECTED,
                                  _on_groupedit_part_unselect, obj);
   evas_object_smart_callback_add(sd->groupedit, "container,changed",
                                  _ws_ruler_abs_zero_move_cb, obj);
   evas_object_smart_callback_add(sd->groupedit, "object,area,changed",
                                  _ws_ruler_rel_zero_move_cb, obj);
   elm_object_content_set(sd->scroller, sd->container.obj);

   /* getting size of rulers and handlers of container. Bottom and Left paddings
      are calculating according to them
      (PADDING_SIZE - ruler_size - handler_size)
   */
   evas_object_geometry_get(sd->ruler_ver, NULL, NULL, &ruler_ver_w, NULL);
   evas_object_geometry_get(sd->ruler_hor, NULL, NULL, NULL, &ruler_hor_h);
   container_handler_size_get(sd->container.obj, NULL, NULL, &hrb_w, &hrb_h);
   container_padding_size_set(sd->container.obj,
                              PADDING_SIZE,
                              PADDING_SIZE,
                              PADDING_SIZE - ruler_ver_w - hrb_w,
                              PADDING_SIZE - ruler_hor_h - hrb_h);

   elm_scroller_policy_set(sd->scroller, ELM_SCROLLER_POLICY_AUTO,
                           ELM_SCROLLER_POLICY_AUTO);

   /* Create highlights for object and relative space */
   TODO("remake scroller and layout with rulers etc. because highlight work wrong because of that")
   if (!sd->highlight.space_hl)
     {
        sd->highlight.space_hl = highlight_add(sd->scroller);
        evas_object_color_set(sd->highlight.space_hl, OBG_AREA_COLOR);
        highlight_handler_disabled_set(sd->highlight.space_hl, true);
        evas_object_smart_member_add(sd->highlight.space_hl, obj);
        evas_object_smart_callback_add(obj, "highlight,visible",
                                       _obj_area_visible_change, sd->highlight.space_hl);
        evas_object_clip_set(sd->highlight.space_hl, sd->clipper);
     }

   if (!sd->highlight.highlight)
     {
        sd->highlight.highlight = highlight_add(sd->scroller);
        evas_object_color_set(sd->highlight.highlight, HIGHLIGHT_COLOR);
        evas_object_smart_member_add(sd->highlight.highlight, obj);
        evas_object_clip_set(sd->highlight.highlight, sd->clipper);
     }

   elm_object_item_disabled_set(sd->menu.items.mode_normal, false);
   elm_object_item_disabled_set(sd->menu.items.mode_separate, false);
   elm_object_item_disabled_set(sd->menu.items.zoom, false);

   Evas_Coord min_w, max_w, min_h, max_h;
   min_w = edje_edit_group_min_w_get(sd->group->edit_object);
   min_h = edje_edit_group_min_h_get(sd->group->edit_object);
   max_w = edje_edit_group_max_w_get(sd->group->edit_object);
   max_h = edje_edit_group_max_h_get(sd->group->edit_object);
   container_min_size_set(sd->container.obj, min_w, min_h);
   container_max_size_set(sd->container.obj, max_w, max_h);

   /*
   TODO("need refactoring (All communications beetween submodules should be implemented in ui_connector)")
   if (ap.live_view)
     {
        container_min_size_set(ap.live_view->live_view, min_w, min_h);
        container_max_size_set(ap.live_view->live_view, max_w, max_h);
     }
   */

   return obj;
}

Evas_Object *
ws_groupedit_get(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);

   return sd->groupedit;
}

void
workspace_group_navigator_update_part(Evas_Object *obj, Part_ *part)
{
   WS_DATA_GET(obj, sd);
   assert(part != NULL);

   group_navigator_part_update(sd->group_navigator, part);
}
#define PADDING_SIZE 40

void
workspace_groupview_soft_update(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);

   assert(sd->groupedit != NULL);

   groupedit_soft_update(sd->groupedit);
}

void
workspace_groupview_hard_update(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);

   assert(sd->groupedit != NULL);

   groupedit_hard_update(sd->groupedit);
}
Eina_Bool
workspace_edit_object_recalc(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);

   assert(sd->groupedit != NULL);

   Evas_Coord min_w, max_w, min_h, max_h;
   min_w = edje_edit_group_min_w_get(sd->group->edit_object);
   min_h = edje_edit_group_min_h_get(sd->group->edit_object);
   max_w = edje_edit_group_max_w_get(sd->group->edit_object);
   max_h = edje_edit_group_max_h_get(sd->group->edit_object);
   container_min_size_set(sd->container.obj, min_w, min_h);
   container_max_size_set(sd->container.obj, max_w, max_h);
   /*
   TODO("need refactoring (All communications beetween submodules should be implemented in ui_connector)")
   if (ap.live_view)
     {
        container_min_size_set(ap.live_view->live_view, min_w, min_h);
        container_max_size_set(ap.live_view->live_view, max_w, max_h);
     }
   */
   return groupedit_edit_object_recalc_all(sd->groupedit);
}

void
workspace_edit_object_hard_update(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);
   assert(sd->groupedit != NULL);

   Evas_Coord min_w, max_w, min_h, max_h;
   min_w = edje_edit_group_min_w_get(sd->group->edit_object);
   min_h = edje_edit_group_min_h_get(sd->group->edit_object);
   max_w = edje_edit_group_max_w_get(sd->group->edit_object);
   max_h = edje_edit_group_max_h_get(sd->group->edit_object);
   container_min_size_set(sd->container.obj, min_w, min_h);
   container_max_size_set(sd->container.obj, max_w, max_h);

   groupedit_hard_update(sd->groupedit);
}

void
workspace_edit_object_soft_update(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);
   assert(sd->groupedit != NULL);
   groupedit_soft_update(sd->groupedit);
}

Eina_Bool
workspace_edit_object_part_add(Evas_Object *obj, Part_ *part)
{
   WS_DATA_GET(obj, sd);
   assert(part != NULL);

   return groupedit_edit_object_part_add(sd->groupedit, part);
}

Eina_Bool
workspace_edit_object_part_del(Evas_Object *obj, Part_ *part)
{
   WS_DATA_GET(obj, sd);
   assert(part != NULL);

   return groupedit_edit_object_part_del(sd->groupedit, part);
}

Eina_Bool
workspace_edit_object_part_state_set(Evas_Object *obj, Part_ *part)
{
   WS_DATA_GET(obj, sd);
   assert(part != NULL);

   TODO("fix state set from external sources");
   return false;
}

Eina_Bool
workspace_edit_object_visible_set(Evas_Object *obj,
                                  const char *part,
                                  Eina_Bool visible __UNUSED__)
{
   WS_DATA_GET(obj, sd);
   assert(part != NULL);

   //groupedit_part_visible_set(sd->groupedit, part);
   return true;
}

Eina_Bool
workspace_separate_mode_set(Evas_Object *obj, Eina_Bool separate)
{
   Evas_Object *follow;
   const char *name = NULL;
   Eina_Bool sep, success;

   WS_DATA_GET(obj, sd)
   assert(sd->groupedit != NULL);

   sep = groupedit_edit_object_parts_separated_is(sd->groupedit);
   if (sep == separate) return false;

   if (separate)
     {
        highlight_object_unfollow(sd->highlight.highlight);
        highlight_object_unfollow(sd->highlight.space_hl);
        evas_object_hide(sd->highlight.space_hl);
        evas_object_hide(sd->highlight.highlight);

        elm_menu_item_icon_name_set(sd->menu.items.mode_separate,
                                    EFLETE_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.mode_normal, "");
     }
   else
     {
        if (sd->highlight.part)
          {
             name = sd->highlight.part->name;
             follow = groupedit_edit_object_part_draw_get(sd->groupedit, name);
             highlight_object_follow(sd->highlight.highlight, follow);
          }
        follow = groupedit_part_object_area_get(sd->groupedit);
        highlight_object_follow(sd->highlight.space_hl, follow);

        evas_object_hide(sd->highlight.space_hl);
        if (sd->highlight.part)
          evas_object_show(sd->highlight.highlight);

        elm_menu_item_icon_name_set(sd->menu.items.mode_normal,
                                    EFLETE_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
     }

   success = groupedit_edit_object_parts_separated(sd->groupedit, separate);
   if (!success) return false;

   /* the code below is really important to be here, because after function that
      was called before groupedit changed it's size, so we can deal with container
      now (setting him propriate size and such stuff). */
   Evas_Coord xpad, ypad, wpad, hpad;
   if (separate)
     {
        Evas_Coord x, y, w, h;
        container_container_size_get(sd->container.obj,
                                 &sd->container.prev_w,
                                 &sd->container.prev_h);
        container_border_hide(sd->container.obj);

        evas_object_geometry_get(sd->groupedit, &x, &y, &w, &h);
        container_padding_size_get(sd->container.obj, &xpad, &ypad, &wpad, &hpad);
        container_container_size_set(sd->container.obj,
                           w + sd->container.dx + xpad + wpad,
                           h + sd->container.dy + ypad + hpad);
     }
   else
     {
        container_container_size_set(sd->container.obj,
                           sd->container.prev_w,
                           sd->container.prev_h);
        container_border_show(sd->container.obj);
     }

   return true;
}

#undef PADDING_SIZE

Eina_Bool
workspace_separate_mode_get(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd)
   return groupedit_edit_object_parts_separated_is(sd->groupedit);
}

Eina_Bool
workspace_highlight_align_visible_set(Evas_Object *obj, Eina_Bool flag)
{
   WS_DATA_GET(obj, sd);
   if (flag)
     highlight_handler_align_show(sd->highlight.highlight);
   else
     highlight_handler_align_hide(sd->highlight.highlight);
   return true;
}

Eina_Bool
workspace_highlight_align_visible_get(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);
   assert(sd->highlight.highlight != NULL);

   return highlight_handler_align_visible_get(sd->highlight.highlight);
}

Eina_Bool
workspace_object_area_visible_set(Evas_Object *obj, Eina_Bool flag)
{
   WS_DATA_GET(obj, sd);
   groupedit_part_object_area_visible_set(sd->groupedit, flag);
   return true;
}

Eina_Bool
workspace_object_area_visible_get(Evas_Object *obj)
{
   WS_DATA_GET(obj, sd);
   return groupedit_part_object_area_visible_get(sd->groupedit);
}

Eina_Bool
workspace_edit_object_part_item_selected_set(Evas_Object *obj,
                                             Eina_Stringshare *item_name,
                                             Eina_Bool selected)
{
   WS_DATA_GET(obj, sd);
   assert(item_name != NULL);

   groupedit_edit_object_part_item_selected_set(sd->groupedit, item_name, selected);
   return true;
}
