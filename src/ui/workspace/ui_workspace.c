/**
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

#include <ui_workspace.h>
#include "main_window.h"
#include "highlight.h"
#include "groupedit.h"
#include "container.h"
#include "eflete.h"

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
   Evas_Object *groupedit;       /**< A groupedit smart object, \
                                   needed for view and edit style.*/

   struct {
        Evas_Object *obj;        /**< Container that contains groupedit.*/
        Evas_Coord prev_w;       /**< Container's width before separated mode.*/
        Evas_Coord prev_h;       /**< Container's height before separated mode.*/
        Evas_Coord dx;           /**< Groupedit's horizontal shift inside of \
                                   the container (for separate mode purpose). */
        Evas_Coord dy;           /**< Groupedit's vertical shift inside of the
                                   container (for separate mode purpose). */
   } container;
   Evas_Object *layout;          /**< A elementary layout object, \
                                   which markup with workspace.edc file.*/
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
   Style *style;

   struct {
        Evas_Object *highlight; /**< A highlight object */
        Evas_Object *space_hl; /**< A object area highlight*/
        Part *part; /**< Contain part name and it's state. need for callbacks of highlight. */
   } highlight;
};
typedef struct _Ws_Smart_Data Ws_Smart_Data;

#define _evas_smart_ws "Evas_Smart_Ws"

#define WS_DATA_GET(o, ptr) \
   Ws_Smart_Data *ptr = evas_object_smart_data_get(o);

#define WS_DATA_GET_OR_RETURN_VAL(o, ptr, val)      \
   WS_DATA_GET(o, ptr);                             \
   if (!ptr)                                        \
    {                                               \
       ERR("No workspace data for object %p (%s)!", \
           o, evas_object_type_get(o));             \
       return val;                                  \
    }

static const char SIG_PART_SELECTED[] = "ws,part,selected";
static const char SIG_PART_UNSELECTED[] = "ws,part,unselected";

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_PART_SELECTED, "s"},
   {SIG_PART_UNSELECTED, "s"},
   {NULL, NULL}
};

EVAS_SMART_SUBCLASS_NEW(_evas_smart_ws, _workspace,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static void
_obj_area_visible_change(void *data __UNUSED__,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
   Eina_Bool visible;

   WS_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);
   if (!groupedit_edit_object_parts_separated_is(sd->groupedit))
     {
        visible = groupedit_part_object_area_visible_get(sd->groupedit);
        groupedit_part_object_area_visible_set(sd->groupedit, !visible);
     }
}

/*TODO: need to remake it. create the public functions for workspace
  for ruler manipulation. */
static void
_menu_rulers_enabled_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("rulers"));
}

/*TODO: need to remake it. create the public functions for workspace
  for ruler manipulation. */
static void
_menu_rulers_abs_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("abs"));
}

/*TODO: need to remake it. create the public functions for workspace
  for ruler manipulation. */
static void
_menu_rulers_rel_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("rel"));
}

/*TODO: need to remake it. create the public functions for workspace
  for ruler manipulation. */
static void
_menu_rulers_both_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("abs&rel"));
}

static void
_separate_mode_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;
   workspace_separate_mode_set(sd->obj, true);
}

static void
_normal_mode_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;
   workspace_separate_mode_set(sd->obj, false);
}

static void
_zoom_factor_50(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   workspace_zoom_factor_set(ws, 0.5);
}

static void
_zoom_factor_100(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   workspace_zoom_factor_set(ws, 1.0);
}

static void
_zoom_factor_200(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   workspace_zoom_factor_set(ws, 2.0);
}

static void
_zoom_factor_in(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Ws_Smart_Data *ws = (Ws_Smart_Data *)data;
   workspace_zoom_factor_set(ws->obj, ws->zoom.factor + 0.1);
}

static void
_zoom_factor_out(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Ws_Smart_Data *ws = (Ws_Smart_Data *)data;
   workspace_zoom_factor_set(ws->obj, ws->zoom.factor - 0.1);
}

static void
_init_ctx_menu(Ws_Smart_Data *ws, Evas_Object *parent)
{
   Evas_Object *menu;
   Ws_Menu *items = &ws->menu.items;
   ws->menu.obj = menu = elm_menu_add(elm_object_top_widget_get (parent));
   elm_object_style_set(menu, "eflete/default");

   items->undo = elm_menu_item_add(menu, NULL, NULL, _("Undo"), NULL, NULL);
   elm_object_item_disabled_set(items->undo, true);
   items->redo = elm_menu_item_add(menu, NULL, NULL, _("Redo"), NULL, NULL);
   elm_object_item_disabled_set(items->redo, true);
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
   elm_object_item_disabled_set(items->zoom_fit, true);
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
}

static void
_ws_ruler_toggle_cb(void *data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID)
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
   WS_DATA_GET_OR_RETURN_VAL(workspace, sd, RETURN_VOID)

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
   WS_DATA_GET_OR_RETURN_VAL(workspace, sd, RETURN_VOID)

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
   WS_DATA_GET_OR_RETURN_VAL(data, sd, RETURN_VOID);

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
   WS_DATA_GET_OR_RETURN_VAL(ws, sd, RETURN_VOID)
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
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   if (!sd->style) return;
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
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

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
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

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
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   if (!sd->style) return;

   Evas_Object *object_area = groupedit_part_object_area_get(sd->groupedit);

   evas_object_geometry_get(sd->ruler_hor, &cross_size, NULL, NULL, NULL);
   evas_object_geometry_get(sd->background, &bg_x, &bg_y, NULL, NULL);
   evas_object_geometry_get(sd->style->obj, &gs_x, &gs_y, &gs_w, &gs_h);
   cross_size -= bg_x;
   ewe_ruler_zero_offset_set(sd->ruler_hor, NULL, gs_x - bg_x - cross_size);
   ewe_ruler_zero_offset_set(sd->ruler_ver, NULL, gs_y - bg_y - cross_size);

   if ((groupedit_edit_object_parts_separated_is(sd->groupedit))
       && (sd->scroll_flag < 2))
     evas_object_smart_callback_call(sd->scroller, "vbar,drag", NULL);
   else
     groupedit_edit_object_recalc_all(sd->groupedit);
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
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   if (sd->zoom.factor == factor) return false;
   sd->zoom.factor = factor;

   groupedit_zoom_factor_set(sd->groupedit, factor);

   /* calculate step for VERTICAL RELATIVE */
   ewe_ruler_step_set(sd->ruler_ver, sd->scale_rel_ver, DEFAULT_STEP * sd->zoom.factor);

   /* calculate step for VERTICAL ABSOLUTE */
   ewe_ruler_step_set(sd->ruler_ver, NULL, DEFAULT_STEP * sd->zoom.factor);

   /* calculate step for VERTICAL RELATIVE */
   ewe_ruler_step_set(sd->ruler_hor, sd->scale_rel_hor, DEFAULT_STEP * sd->zoom.factor);

   /* calculate step for VERTICAL ABSOLUTE */
   ewe_ruler_step_set(sd->ruler_hor, NULL, DEFAULT_STEP * sd->zoom.factor);

   if (fabs(factor - 1.0) > 0.00001)
     {
        container_border_hide(sd->container.obj);

        App_Data *app = app_data_get();
        ui_menu_disable_set(app->menu_hash, _("Separate"), true);

        Ws_Menu *items = &sd->menu.items;
        elm_object_item_disabled_set(items->mode_normal, true);
        elm_object_item_disabled_set(items->mode_separate, true);
        elm_object_disabled_set(sd->button_separate, true);
     }
   else
     {
        container_border_show(sd->container.obj);

        App_Data *app = app_data_get();
        ui_menu_disable_set(app->menu_hash, _("Separate"), false);

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
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, 0);
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
   WS_DATA_GET_OR_RETURN_VAL(ws_obj, sd, RETURN_VOID)
   evas_pointer_output_xy_get (e, &x, &y);
   evas_object_geometry_get(sd->ruler_hor, &dx, NULL, NULL, NULL);
   evas_object_geometry_get(sd->ruler_ver, NULL, &dy, NULL, NULL);
   ewe_ruler_marker_absolute_set(sd->ruler_hor, sd->pointer_hor, x - dx);
   ewe_ruler_marker_absolute_set(sd->ruler_ver, sd->pointer_ver, y - dy);
}

static void
_on_resize(void *data,
            Evas_Object * obj __UNUSED__,
            void *ei)
{
   Highlight_Events *events = (Highlight_Events *)ei;
   Evas_Object *ws_obj = (Evas_Object *)data;
   WS_DATA_GET_OR_RETURN_VAL(ws_obj, sd, RETURN_VOID)

   Part *part = sd->highlight.part;
   if ((!sd->style) && (!part)) return;
   edje_edit_state_max_w_set(sd->style->obj, part->name,
                             part->curr_state, part->curr_state_value,
                             events->w / sd->zoom.factor);
   edje_edit_state_max_h_set(sd->style->obj, part->name,
                             part->curr_state, part->curr_state_value,
                             events->h / sd->zoom.factor);
   if (!sd->style->isModify) sd->style->isModify = true;
   workspace_edit_object_recalc(ws_obj);
   evas_object_smart_callback_call(ws_obj, "part,changed", part);
}

Eina_Bool
workspace_highlight_set(Evas_Object *obj, Part *part)
{
   Evas_Object *follow;
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   if (!part) return false;
   if (!sd->groupedit) return false;

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
        evas_object_smart_callback_add(sd->highlight.highlight, "hl,resize",
                                       _on_resize, obj);
     }
   return true;
}

Eina_Bool
workspace_highlight_unset(Evas_Object *obj)
{
   if (!obj) return false;
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)
   if ((!sd->highlight.highlight) || (!sd->highlight.space_hl)) return false;
   highlight_object_unfollow(sd->highlight.highlight);
   highlight_object_unfollow(sd->highlight.space_hl);
   sd->highlight.part = NULL;
   evas_object_hide(sd->highlight.space_hl);
   evas_object_hide(sd->highlight.highlight);

   evas_object_event_callback_del(sd->highlight.highlight,
                                  EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb);
   evas_object_smart_callback_del(sd->highlight.highlight, "hl,resize",
                                  _on_resize);
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
   /* Allocate memory for workspace smart data*/
   EVAS_SMART_DATA_ALLOC(o, Ws_Smart_Data)

   _workspace_parent_sc->add(o);
}

static Eina_Bool
_workspace_child_create(Evas_Object *o, Evas_Object *parent)
{
   WS_DATA_GET_OR_RETURN_VAL(o, priv, false);
   /* adding self-reference */
   priv->obj = o;

   Evas *e = evas_object_evas_get(o);
   Evas_Object *icon = NULL;

   /* Load main markup layout into smart_data->layout*/
   priv->layout = elm_layout_add(parent);
   elm_layout_file_set(priv->layout, EFLETE_EDJ, "eflete/workspace/base/smart");
   evas_object_smart_member_add(priv->layout, o);

   /* Here create evas image, whitch will be background for workspace*/
   GET_IMAGE(priv->background, e, "bg_demo");
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

   /* using scroller in workspace, with special style*/
   priv->scroll_flag = 0;
   priv->zoom.factor = 1.0;
   priv->scroller = elm_scroller_add(priv->layout);
   elm_object_style_set(priv->scroller, "eflete/workspace");
   elm_scroller_policy_set(priv->scroller, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);
   elm_scroller_content_min_limit(priv->scroller, false, false);
   elm_layout_content_set(priv->layout, "groupspace", priv->scroller);

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
   elm_object_style_set(priv->button_separate, "eflete/simple");
   GET_IMAGE(icon, priv->scroller, "icon-separate");
   elm_object_part_content_set(priv->button_separate, NULL, icon);
   evas_object_smart_callback_add(priv->button_separate, "clicked",
                                  _separate_smart_on_click, o);
   evas_object_smart_member_add(priv->button_separate, o);

   /* Add members of workspace into scroller markup field''s*/
   elm_object_part_content_set(priv->scroller, "elm.swallow.background",
                               priv->background);
   elm_object_part_content_set(priv->scroller, "elm.swallow.overlay",
                               priv->events);
   elm_object_part_content_set(priv->scroller, "cross.swallow",
                               priv->button_separate);

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

   RULER(priv->ruler_hor, priv->scale_rel_hor, priv->pointer_hor, "ruler.swallow.hor");
   RULER(priv->ruler_ver, priv->scale_rel_ver, priv->pointer_ver, "ruler.swallow.ver");
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

   priv->style = NULL;
   priv->guides = NULL;

   return true;
}


static void
_workspace_smart_del(Evas_Object *o)
{
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   evas_object_smart_member_del(sd->ruler_hor);
   evas_object_smart_member_del(sd->ruler_ver);
   evas_object_smart_member_del(sd->layout);
   evas_object_smart_member_del(sd->background);
   evas_object_smart_member_del(sd->button_separate);
   evas_object_smart_member_del(sd->scroller);
   evas_object_smart_member_del(sd->events);
   evas_object_smart_member_del(sd->highlight.highlight);
   evas_object_smart_member_del(sd->highlight.space_hl);
   evas_object_smart_member_del(sd->menu.obj);
   if (sd->groupedit)
     evas_object_smart_member_del(sd->groupedit);

   _workspace_parent_sc->del(o);
}

static void
_workspace_smart_show(Evas_Object *o)
{
   if (evas_object_visible_get(o)) return;

   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_show(sd->button_separate);
   evas_object_show(sd->scroller);
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

   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   if (sd->groupedit) evas_object_hide(sd->groupedit);
   if (sd->ruler_hor) evas_object_hide(sd->ruler_hor);
   if (sd->ruler_ver) evas_object_hide(sd->ruler_ver);

   evas_object_hide(sd->button_separate);
   evas_object_hide(sd->scroller);
   evas_object_hide(sd->events);

   _workspace_parent_sc->hide(o);
}

static void
_workspace_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ox, oy, ow, oh;
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_geometry_get(o, &ox, &oy, &ow, &oh);
   if ((ow == w) && (oh == h)) return;
   evas_object_resize(sd->layout, w, h);
   evas_object_smart_changed(o);
}

static void
_workspace_color_set(Evas_Object *o, int r, int g, int b, int a)
{
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   evas_object_color_set(sd->background, r, g, b, a);
}

static void
_workspace_smart_set_user(Evas_Smart_Class *sc)
{
   sc->add = _workspace_smart_add;
   sc->del = _workspace_smart_del;
   sc->show = _workspace_smart_show;
   sc->hide = _workspace_smart_hide;
   sc->color_set = _workspace_color_set;

   sc->resize = _workspace_smart_resize;
   sc->calculate = NULL;
}


Evas_Object *
workspace_add(Evas_Object *parent)
{
   Evas *e = NULL;
   Evas_Object *obj = NULL;

   if (!parent) return NULL;

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _workspace_smart_class_new());
   if (_workspace_child_create(obj, parent)) return obj;
   else
     {
        evas_object_del(obj);
        return NULL;
     }
}

Evas_Object *
ws_groupedit_get(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   return sd->groupedit;
}

static void
_on_part_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   evas_object_smart_callback_call(workspace, SIG_PART_SELECTED, event_info);
}

static void
_on_part_unselect(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   evas_object_smart_callback_call(workspace, SIG_PART_UNSELECTED, event_info);
   workspace_highlight_unset(workspace);
}

static void
_on_container_TL_move(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;
   Container_Geom *geom = (Container_Geom *)event_info;

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
                               void *event_info)
{
   Ws_Smart_Data *sd = (Ws_Smart_Data *)data;
   Container_Geom *geom = (Container_Geom *)event_info;

   Evas_Coord x, y, w, h;
   evas_object_geometry_get(sd->groupedit, &x, &y, &w, &h);

   container_padding_size_set(sd->container.obj,
                              PADDING_SIZE + abs(geom->x - x),
                              PADDING_SIZE + abs(geom->y - y),
                              PADDING_SIZE + abs((geom->x + geom->w) - (x + w)) * sd->zoom.factor,
                              PADDING_SIZE + abs((geom->y + geom->h) - (y + h)) * sd->zoom.factor);
}

Eina_Bool
workspace_edit_object_set(Evas_Object *obj, Style *style, const char *file)
{
   Evas_Coord x, y, w, h;
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if ((!style) || (!file)) return false;
   if (!sd->groupedit)
     {
        sd->groupedit = groupedit_add(sd->scroller);
        /* it temporary solution white not implemented preference module
           and not finished config module */
        evas_object_color_set(sd->groupedit, 0, 0, 0, 255);
        evas_object_smart_callback_add(sd->groupedit, "geometry,changed",
                                       _on_groupedit_geometry_changed, sd);
     }
   else groupedit_edit_object_unset(sd->groupedit);

   if (!sd->container.obj)
     {
        sd->container.obj = container_add(sd->scroller);
        evas_object_smart_callback_add(sd->container.obj, "handler,TL,moved",
                                       _on_container_TL_move, sd);
        container_style_set(sd->container.obj, "scroller");
     }
   else container_content_unset(sd->container.obj);

   container_content_set(sd->container.obj, sd->groupedit);

   evas_object_focus_set(sd->groupedit, true);

   sd->style = style;
   elm_menu_item_icon_name_set(sd->menu.items.mode_normal,
                               EFLETE_IMG_PATH"context_menu-bullet.png");
   elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
   if (!groupedit_edit_object_set(sd->groupedit, style->obj, file)) return false;
   container_handler_size_set(sd->container.obj, 8, 8, 8, 8);
   evas_object_smart_callback_add(sd->groupedit, "part,selected",
                                  _on_part_select, obj);
   evas_object_smart_callback_add(sd->groupedit, "part,unselected",
                                  _on_part_unselect, obj);
   evas_object_smart_callback_add(sd->groupedit, "container,changed",
                                  _ws_ruler_abs_zero_move_cb, obj);
   evas_object_smart_callback_add(sd->groupedit, "object,area,changed",
                                  _ws_ruler_rel_zero_move_cb, obj);
   groupedit_bg_set(sd->groupedit, sd->background);
   elm_object_content_set(sd->scroller, sd->container.obj);
   evas_object_show(sd->container.obj);
   evas_object_show(sd->groupedit);

   /* Create highlights for object and relative space */
   /*TODO: remake scroller and layout with rulers etc.
           because highlight work wrong because of that */
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

   evas_object_geometry_get(sd->container.obj, &x, &y, &w, &h);
   evas_object_resize(sd->container.obj, w - PADDING_SIZE * 2, h - PADDING_SIZE * 2);
   elm_scroller_region_get(sd->container.obj, &x, &y, &w, &h);
   elm_scroller_region_show(sd->container.obj, x / 2, y / 2, w, h);

   workspace_zoom_factor_set(obj, 1.0);

   return true;
}

Eina_Bool
workspace_edit_object_unset(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   Eina_Bool is_unset = false;
   sd->style = NULL;
   if (!sd->groupedit) return false;

   if (groupedit_edit_object_unset(sd->groupedit)) is_unset = true;
   container_content_unset(sd->container.obj);
   elm_object_content_unset(sd->scroller);
   evas_object_del(sd->groupedit);
   evas_object_del(sd->container.obj);
   sd->groupedit = NULL;
   sd->container.obj = NULL;

   elm_object_item_disabled_set(sd->menu.items.mode_normal, true);
   elm_object_item_disabled_set(sd->menu.items.mode_separate, true);

   return is_unset;
}

Style *
workspace_edit_object_get(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   return sd->style;
}

Eina_Bool
workspace_edit_object_recalc(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   return groupedit_edit_object_recalc_all(sd->groupedit);
}

Eina_Bool
workspace_edit_object_part_add(Evas_Object *obj, const char *part,
                               Edje_Part_Type type, const char *data)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part)
     {
        ERR("Can't add the part '%s' to the group '%s'!",
            part, sd->style->full_group_name)
        return false;
     }

   return groupedit_edit_object_part_add(sd->groupedit, part, type, data);
}

Eina_Bool
workspace_edit_object_part_del(Evas_Object *obj, const char *part)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part)
     {
        ERR("Can't delete the part '%s' from the group '%s'!",
            part, sd->style->full_group_name)
        return false;
     }

   return groupedit_edit_object_part_del(sd->groupedit, part);
}

Eina_Bool
workspace_edit_object_part_above(Evas_Object *obj, const char *part)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part)
     {
        ERR("Can't restack above the part '%s' in the group %s!",
            part, sd->style->full_group_name)
        return false;
     }

   return groupedit_edit_object_part_above(sd->groupedit, part);
}

Eina_Bool
workspace_edit_object_part_below(Evas_Object *obj, const char *part)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part)
     {
        ERR("Can't restack below the part '%s' in the group %s!",
            part, sd->style->full_group_name)
        return false;
     }

   return groupedit_edit_object_part_below(sd->groupedit, part);
}

Eina_Bool
workspace_edit_object_part_state_set(Evas_Object *obj, Part *part)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!part)
     {
        ERR("Can't set state to NULL pointer part");
        return false;
     }
   return groupedit_edit_object_part_state_set(sd->groupedit, part->name,
                                               part->curr_state,
                                               part->curr_state_value);
}

Eina_Bool
workspace_edit_object_part_state_add(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state))
     {
        ERR("Can't add state '%s %f' to part '%s' in the group %s!",
            state, value, part, sd->style->full_group_name)
        return false;
     }

   return groupedit_edit_object_part_state_add(sd->groupedit, part, state, value);
}

Eina_Bool
workspace_edit_object_part_restack(Evas_Object *obj,
                                   const char *part,
                                   const char *rel_part,
                                   Eina_Bool direct)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part || !rel_part)
     {
        ERR("Input arguments wrong: part[%s] rel_part[%s]", part, rel_part);
        return false;
     }

   if (!direct)
      return groupedit_edit_object_part_move_above(sd->groupedit, part, rel_part);
   else
      return groupedit_edit_object_part_move_below(sd->groupedit, part, rel_part);
}


Eina_Bool
workspace_edit_object_part_state_copy(Evas_Object *obj, const char *part,
                                     const char *state_from, double value_from,
                                     const char *state_to, double value_to)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state_from) || (!state_to))
     return false;

   return groupedit_edit_object_part_state_copy(sd->groupedit, part, state_from,
                                               value_from, state_to, value_to);
}

Eina_Bool
workspace_edit_object_part_state_del(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state))
     {
        ERR("Can't delete state '%s %f' from part '%s' in the group %s!",
            state, value, part, sd->style->full_group_name)
        return false;
     }

   return groupedit_edit_object_part_state_del(sd->groupedit, part, state, value);
}

Eina_Bool
workspace_edit_object_visible_set(Evas_Object *obj,
                                  const char *part,
                                  Eina_Bool visible)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!part)
     {
        ERR("Can't change visible params for NULL pointer");
        return false;
     }

   return groupedit_part_visible_set(sd->groupedit, part, visible);
}

Eina_Bool
workspace_separate_mode_set(Evas_Object *obj, Eina_Bool separate)
{
   Evas_Object *follow;
   const char *name = NULL;
   Eina_Bool sep;

   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   if (!sd->groupedit) return false;
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
          name = sd->highlight.part->name;
        follow = groupedit_edit_object_part_draw_get(sd->groupedit, name);
        highlight_object_follow(sd->highlight.highlight, follow);
        follow = groupedit_part_object_area_get(sd->groupedit);
        highlight_object_follow(sd->highlight.space_hl, follow);

        evas_object_hide(sd->highlight.space_hl);
        if (sd->highlight.part)
          evas_object_show(sd->highlight.highlight);

        elm_menu_item_icon_name_set(sd->menu.items.mode_normal,
                                    EFLETE_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
     }

   groupedit_edit_object_parts_separated(sd->groupedit, separate);

   /* the code below is really important to be here, because after function that
      was called before groupedit changed it's size, so we can deal with container
      now (setting him propriate size and such stuff). */
   Evas_Coord xpad, ypad, wpad, hpad;
   if (separate)
     {
        Evas_Coord x, y, w, h;
        evas_object_geometry_get(sd->container.obj, NULL, NULL,
                                 &sd->container.prev_w,
                                 &sd->container.prev_h);
        container_border_hide(sd->container.obj);

        evas_object_geometry_get(sd->groupedit, &x, &y, &w, &h);
        container_padding_size_get(sd->container.obj, &xpad, &ypad, &wpad, &hpad);
        evas_object_resize(sd->container.obj,
                           w + sd->container.dx + xpad + wpad,
                           h + sd->container.dy + ypad + hpad);

        /* padding compensation */
        sd->container.prev_w -= xpad + wpad - PADDING_SIZE * 2;
        sd->container.prev_h -= ypad + hpad - PADDING_SIZE * 2;
     }
   else
     {
        evas_object_resize(sd->container.obj,
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
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)
   return groupedit_edit_object_parts_separated_is(sd->groupedit);
}
