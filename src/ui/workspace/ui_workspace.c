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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include <ui_workspace.h>
#include "highlight.h"
#include "groupedit.h"
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
   Evas_Object *clipper;         /**< Needed for processing common events,\
                                   like mouse move or mouse click. */
   Evas_Object *background;      /**< A backround image, \
                                   which swallowed into scroller.*/
   Evas_Object *ruler_hor;       /**< A ruler object, which created and \
                                   managed with ui_ruler API. Horizontal.*/
   Evas_Object *ruler_ver;       /**< A ruler object, which created and \
                                   managed with ui_ruler API. Vertical.*/
   Evas_Object *scroller;        /**< A scroler with 'eflete/workspace' style. \
                                   Implement scrollable interface.*/
   char scroll_flag;             /**< Needed for control drag bar's in scroller*/
   Evas_Object *groupedit;       /**< A groupedit smart object, \
                                   needed for view and edit style.*/
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
   struct {
        Evas_Object *view;        /**< A elementary layout with special \
                                    markup stored in legend.edc. TDD*/
        Eina_Bool visible;        /**< Boolean flag for legend view visibly. */
   } legend;
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

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_PART_SELECTED, "s"},
   {NULL, NULL}
};

EVAS_SMART_SUBCLASS_NEW(_evas_smart_ws, _workspace,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static void
_obj_area_visible_change(void *data,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);
   Evas_Object *highlight = (Evas_Object *)data;
   Eina_Bool visible = evas_object_visible_get(highlight);
   if (!groupedit_edit_object_parts_separated_is(sd->groupedit))
     {
        if (visible)
          evas_object_hide(highlight);
        else
          evas_object_show(highlight);
     }
}

static void
_menu_rulers_enabled_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("rulers"));
}

static void
_menu_rulers_abs_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("abs"));
}

static void
_menu_rulers_rel_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,toggle", strdup("rel"));
}

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
   const char *name = NULL;
   Evas_Object *o = (Evas_Object *)data;
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   Eina_Bool sep = groupedit_edit_object_parts_separated_is(sd->groupedit);
   if (sep) return;
   if (sd->highlight.part)
      name = sd->highlight.part->name;
   highlight_object_unfollow(sd->highlight.highlight);
   highlight_object_unfollow(sd->highlight.space_hl);
   evas_object_hide(sd->highlight.space_hl);
   evas_object_hide(sd->highlight.highlight);

   elm_menu_item_icon_name_set(sd->menu.items.mode_normal, "");
   elm_menu_item_icon_name_set(sd->menu.items.mode_separate, TET_IMG_PATH"context_menu-bullet.png");
   groupedit_edit_object_parts_separated(sd->groupedit, !sep, name);
}

static void
_normal_mode_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *follow;
   const char *name = NULL;
   Evas_Object *o = (Evas_Object *)data;
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   Eina_Bool sep = groupedit_edit_object_parts_separated_is(sd->groupedit);
   if (!sep) return;
   if (sd->highlight.part)
      name = sd->highlight.part->name;
   follow = groupedit_edit_object_part_draw_get(sd->groupedit, name);
   highlight_object_follow(sd->highlight.highlight, follow);

   follow = groupedit_part_object_area_get(sd->groupedit);
   highlight_object_follow(sd->highlight.space_hl, follow);

   evas_object_hide(sd->highlight.space_hl);
   evas_object_show(sd->highlight.highlight);

   elm_menu_item_icon_name_set(sd->menu.items.mode_normal, TET_IMG_PATH"context_menu-bullet.png");
   elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
   groupedit_edit_object_parts_separated(sd->groupedit, !sep, name);
}

static void
_init_ctx_menu(Ws_Smart_Data *ws, Evas_Object *parent)
{
   Evas_Object *menu;
   Ws_Menu *items = &ws->menu.items;
   ws->menu.obj = menu = elm_menu_add(elm_object_top_widget_get (parent));
   elm_object_style_set(menu, "eflete/default");

   items->undo = elm_menu_item_add(menu, NULL, NULL, "Undo", NULL, NULL);
   elm_object_item_disabled_set(items->undo, true);
   items->redo = elm_menu_item_add(menu, NULL, NULL, "Redo", NULL, NULL);
   elm_object_item_disabled_set(items->redo, true);
   elm_menu_item_separator_add(menu, NULL);

   items->rulers = elm_menu_item_add(menu, NULL, NULL, "Rulers", NULL, NULL);
   items->rulers_enable = elm_menu_item_add(menu, items->rulers, NULL, "Rulers enabled", _menu_rulers_enabled_cb, ws->obj);
   elm_menu_item_icon_name_set(items->rulers_enable, TET_IMG_PATH"context_menu-check.png");
   elm_menu_item_separator_add(menu, items->rulers);
   items->rulers_abs = elm_menu_item_add(menu, items->rulers, NULL, "Absolute scale", _menu_rulers_abs_cb, ws->obj);
   elm_menu_item_icon_name_set(items->rulers_abs, TET_IMG_PATH"context_menu-bullet.png");
   items->rulers_rel = elm_menu_item_add(menu, items->rulers, NULL, "Relative scale", _menu_rulers_rel_cb, ws->obj);
   items->rulers_both = elm_menu_item_add(menu, items->rulers, NULL, "Both", _menu_rulers_both_cb, ws->obj);

   items->zoom = elm_menu_item_add(menu, NULL, NULL, "Zoom", NULL, NULL);
   items->zoom_in = elm_menu_item_add(menu, items->zoom, NULL, "Zoom in", NULL, NULL);
   elm_object_item_disabled_set(items->zoom_in, true);
   items->zoom_out = elm_menu_item_add(menu, items->zoom, NULL, "Zoom out", NULL, NULL);
   elm_object_item_disabled_set(items->zoom_out, true);
   elm_menu_item_separator_add(menu, items->zoom);
   items->zoom_fit = elm_menu_item_add(menu, items->zoom, NULL, "Fit", NULL, NULL);
   elm_object_item_disabled_set(items->zoom_fit, true);
   elm_menu_item_separator_add(menu, items->zoom);
   items->zoom_far = elm_menu_item_add(menu, items->zoom, NULL, "20%", NULL, NULL);
   elm_object_item_disabled_set(items->zoom_far, true);
   items->zoom_normal = elm_menu_item_add(menu, items->zoom, NULL, "100%", NULL, NULL);
   elm_object_item_disabled_set(items->zoom_normal, true);
   items->zoom_near = elm_menu_item_add(menu, items->zoom, NULL, "500%", NULL, NULL);
   elm_object_item_disabled_set(items->zoom_near, true);
   elm_menu_item_separator_add(menu, NULL);

   items->mode_normal = elm_menu_item_add(menu, NULL, NULL, "Normal mode", _normal_mode_click, ws->obj);
   elm_menu_item_icon_name_set(items->mode_normal, TET_IMG_PATH"context_menu-bullet.png");
   items->mode_separate = elm_menu_item_add(menu, NULL, NULL, "Separate mode", _separate_mode_click, ws->obj);
   elm_menu_item_separator_add(menu, NULL);
   items->settings = elm_menu_item_add(menu, NULL, NULL, "Settings...", NULL, NULL);
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
        if (ui_ruler_visible_get(sd->ruler_hor))
          {
             elm_layout_signal_emit(sd->scroller, "ruler,hide,hor", "");
             elm_layout_signal_emit(sd->scroller, "ruler,hide,ver", "");
             elm_menu_item_icon_name_set(sd->menu.items.rulers_enable, "");
             ui_ruler_hide(sd->ruler_hor);
             ui_ruler_hide(sd->ruler_ver);
          }
        else
          {
             ui_ruler_show(sd->ruler_hor);
             ui_ruler_show(sd->ruler_ver);
             elm_menu_item_icon_name_set(sd->menu.items.rulers_enable, TET_IMG_PATH"context_menu-check.png");
             elm_layout_signal_emit(sd->scroller, "ruler,show,hor", "");
             elm_layout_signal_emit(sd->scroller, "ruler,show,ver", "");
          }
     }
   else if (!strcmp(data_info, "abs"))
     {
        ui_ruler_scale_absolute_visible_set(sd->ruler_hor, true);
        ui_ruler_scale_absolute_visible_set(sd->ruler_ver, true);
        ui_ruler_scale_relative_visible_set(sd->ruler_hor, false);
        ui_ruler_scale_relative_visible_set(sd->ruler_ver, false);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, TET_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_both, "");
     }
   else if (!strcmp(data_info, "rel"))
     {
        ui_ruler_scale_absolute_visible_set(sd->ruler_hor, false);
        ui_ruler_scale_absolute_visible_set(sd->ruler_ver, false);
        ui_ruler_scale_relative_visible_set(sd->ruler_hor, true);
        ui_ruler_scale_relative_visible_set(sd->ruler_ver, true);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, TET_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_both, "");
     }
   else if (!strcmp(data_info, "abs&rel"))
     {
        ui_ruler_scale_absolute_visible_set(sd->ruler_hor, true);
        ui_ruler_scale_absolute_visible_set(sd->ruler_ver, true);
        ui_ruler_scale_relative_visible_set(sd->ruler_hor, true);
        ui_ruler_scale_relative_visible_set(sd->ruler_ver, true);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, "");
        elm_menu_item_icon_name_set(sd->menu.items.rulers_both, TET_IMG_PATH"context_menu-bullet.png");
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

   ui_ruler_scale_absolute_position_zero_set(sd->ruler_hor, ge_geom->x);
   ui_ruler_scale_absolute_position_zero_set(sd->ruler_ver, ge_geom->y);

   ui_ruler_redraw(sd->ruler_hor);
   ui_ruler_redraw(sd->ruler_ver);
}

static void
_ws_ruler_rel_zero_move_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   Groupedit_Geom *ge_geom = (Groupedit_Geom *)event_info;
   WS_DATA_GET_OR_RETURN_VAL(workspace, sd, RETURN_VOID)

   ui_ruler_scale_relative_position_set(sd->ruler_hor, ge_geom->x, ge_geom->x + ge_geom->w);
   ui_ruler_scale_relative_position_set(sd->ruler_ver, ge_geom->y, ge_geom->y + ge_geom->h);

   ui_ruler_redraw(sd->ruler_hor);
   ui_ruler_redraw(sd->ruler_ver);
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
   Evas_Object *ws = (Evas_Object *)data;
   WS_DATA_GET_OR_RETURN_VAL(ws, sd, RETURN_VOID)
   evas_pointer_output_xy_get(e, &x, &y);
   ui_ruler_pointer_pos_set(sd->ruler_hor, x);
   ui_ruler_pointer_pos_set(sd->ruler_ver, y);
}

static void
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
      _separate_mode_click(o, NULL, NULL);
   else
      _normal_mode_click(o, NULL, NULL);
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
        groupedit_container_size_get(sd->groupedit, &w_cont, NULL);

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
        groupedit_container_size_get(sd->groupedit, NULL, &h_cont);

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
   evas_object_geometry_get(sd->ruler_hor, &cross_size, NULL, NULL, NULL);
   evas_object_geometry_get(sd->background, &bg_x, &bg_y, NULL, NULL);
   evas_object_geometry_get(sd->style->obj, &gs_x, &gs_y, &gs_w, &gs_h);
   cross_size -= bg_x;
   ui_ruler_scale_absolute_position_zero_set(sd->ruler_hor, gs_x - bg_x - cross_size);
   ui_ruler_scale_absolute_position_zero_set(sd->ruler_ver, gs_y - bg_y - cross_size);
   ui_ruler_scale_relative_position_set(sd->ruler_hor, gs_x - bg_x - cross_size,
                                        gs_x + gs_w - bg_x - cross_size);
   ui_ruler_scale_relative_position_set(sd->ruler_ver, gs_y - bg_y - cross_size,
                                        gs_y + gs_h - bg_y - cross_size);
   ui_ruler_redraw(sd->ruler_hor);
   ui_ruler_redraw(sd->ruler_ver);

   if ((groupedit_edit_object_parts_separated_is(sd->groupedit))
       && (sd->scroll_flag < 2))
     evas_object_smart_callback_call(sd->scroller, "vbar,drag", NULL);
   else
     groupedit_edit_object_recalc_all(sd->groupedit);
   sd->scroll_flag = 0;

}

static Eina_Bool
_background_load(Evas_Object *image, const char *path)
{
   Evas_Load_Error err;
   int w, h;
   evas_object_image_file_set(image, path, NULL);
   err = evas_object_image_load_error_get(image);
   if (err != EVAS_LOAD_ERROR_NONE)
     {
        ERR("Could not load image [%s]. Error is \"%s\"", path,
            evas_load_error_str(err));
        /* Load default background*/
        evas_object_image_file_set(image, TET_IMG_PATH"bg_demo.png", NULL);
        return false;
     }
   evas_object_image_size_get(image, &w, &h);
   evas_object_image_filled_set(image, false);
   evas_object_image_fill_set(image, 0, 0, w, h);
   return true;
}

static Eina_Bool
_zoom_factor_update(Evas_Object *obj, double factor)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   sd->zoom.factor = factor;
   /*
    * TODO:
    * Create method, which update current zoom in groupedit and workspace
    */
   return true;
}

Eina_Bool
ws_zoom_factor_set(Evas_Object *obj, double factor)
{
   if ((factor < 0.01) || (factor > 20 )) return false;
   return _zoom_factor_update(obj, factor);
}

double
ws_zoom_factor_get(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, 0);
   return sd->zoom.factor;
}

Eina_Bool
ws_background_image_set(Evas_Object *obj, const char *path)
{
   if (!path) return false;
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   if (!sd->background) return false;
   if (!_background_load(sd->background, path)) return false;
   return true;
}

Eina_Bool
ws_legend_visible_set(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)
   if (!sd->legend.visible)
    elm_layout_signal_emit(obj, "legend,hide", "");
   else
    elm_layout_signal_emit(obj, "legend,show", "");
   sd->legend.visible = !sd->legend.visible;
   return true;
}

static void
_ws_mouse_move_cb(void *data, Evas *e,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int x, y;
   Evas_Object *ws_obj = (Evas_Object *)data;
   WS_DATA_GET_OR_RETURN_VAL(ws_obj, sd, RETURN_VOID)
   evas_pointer_output_xy_get (e, &x, &y);
   ui_ruler_pointer_pos_set(sd->ruler_hor, x);
   ui_ruler_pointer_pos_set(sd->ruler_ver, y);
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
                             events->w);
   edje_edit_state_max_h_set(sd->style->obj, part->name,
                             part->curr_state, part->curr_state_value,
                             events->h);
   if (!sd->style->isModify) sd->style->isModify = true;
   workspace_edit_object_recalc(ws_obj);
   evas_object_smart_callback_call(ws_obj, "part,changed", part);
}

Eina_Bool
workspace_highlight_set(Evas_Object *obj, Part *part)
{
   Evas_Object *follow;
   if ((!obj) || (!part)) return false;
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   groupedit_part_object_area_set(sd->groupedit, part->name);
   sd->highlight.part = part;

   if (groupedit_edit_object_parts_separated_is(sd->groupedit))
     groupedit_edit_object_part_select(sd->groupedit, part->name);
   else
     {
        follow = groupedit_edit_object_part_draw_get(sd->groupedit, part->name);
        highlight_object_follow(sd->highlight.highlight, follow);

        follow = groupedit_part_object_area_get(sd->groupedit);
        highlight_object_follow(sd->highlight.space_hl, follow);

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
   elm_layout_file_set(priv->layout, TET_EDJ, "eflete/workspace/base/smart");
   evas_object_smart_member_add(priv->layout, o);

   /* Here create evas image, whitch will be background for workspace*/
   priv->background = evas_object_image_filled_add(e);
   _background_load(priv->background, TET_IMG_PATH"bg_demo.png");
   evas_object_smart_member_add(priv->background, o);

   /* Clipper needed for check mouse events*/
   priv->clipper = evas_object_rectangle_add(e);
   evas_object_color_set(priv->clipper, 0, 0, 0, 0);
   evas_object_smart_member_add(priv->clipper, o);
   evas_object_event_callback_add(priv->clipper, EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_smart_mouse_move_cb, o);
   evas_object_event_callback_add(priv->clipper, EVAS_CALLBACK_MOUSE_DOWN,
                                  _ws_smart_mouse_click_cb, o);

   /* create empty groupspace object */
   priv->groupedit = NULL;

   /* using scroller in workspace, with special style*/
   priv->scroll_flag = 0;
   priv->scroller = elm_scroller_add(priv->layout);
   elm_object_style_set(priv->scroller, "eflete/workspace");
   elm_scroller_policy_set(priv->scroller, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);
   elm_scroller_content_min_limit(priv->scroller, false, false);
   elm_object_part_content_set(priv->layout, "groupspace", priv->scroller);

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
   icon = elm_icon_add(priv->scroller);
   elm_image_file_set(icon, TET_IMG_PATH"icon-separate.png", NULL);
   elm_image_no_scale_set(icon, true);
   elm_object_part_content_set(priv->button_separate, NULL, icon);
   evas_object_smart_callback_add(priv->button_separate, "clicked",
                                  _separate_smart_on_click, o);
   evas_object_smart_member_add(priv->button_separate, o);


   /* Add members of workspace into scroller markup field''s*/
   elm_object_part_content_set(priv->scroller, "elm.swallow.background",
                               priv->background);
   elm_object_part_content_set(priv->scroller, "elm.swallow.overlay",
                               priv->clipper);
   elm_object_part_content_set(priv->scroller, "cross.swallow",
                               priv->button_separate);

   /* create rulers, using ui_ruler.h API*/
   priv->ruler_hor = ui_ruler_add(priv->scroller);
   elm_object_part_content_set(priv->scroller, "ruler.swallow.hor", priv->ruler_hor);
   evas_object_smart_callback_add(o, "ruler,toggle",
                                  _ws_ruler_toggle_cb, NULL);
   evas_object_smart_member_add(priv->ruler_hor, o);

   priv->ruler_ver = ui_ruler_add(priv->scroller);
   ui_ruler_orient_set(priv->ruler_ver, VERTICAL);
   elm_object_part_content_set(priv->scroller, "ruler.swallow.ver", priv->ruler_ver);
   evas_object_smart_member_add(priv->ruler_ver, o);

   /* init context menu */
   _init_ctx_menu(priv, parent);
   evas_object_smart_member_add(priv->menu.obj, o);

   /* Simple initialize variavbles of smart data */
   priv->zoom.factor = 1.0;
   priv->zoom.control = NULL;

   priv->legend.view = NULL;
   priv->legend.visible = false;

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
   evas_object_smart_member_del(sd->clipper);
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
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_show(sd->button_separate);
   evas_object_show(sd->scroller);
   evas_object_show(sd->clipper);

   if (sd->groupedit)
     evas_object_show(sd->groupedit);
   if (sd->ruler_hor)
     ui_ruler_show(sd->ruler_hor);
   if (sd->ruler_ver)
     ui_ruler_show(sd->ruler_ver);

   evas_object_show(sd->background);
   _workspace_parent_sc->show(o);
}

static void
_workspace_smart_hide(Evas_Object *o)
{
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   if (sd->groupedit) evas_object_hide(sd->groupedit);
   if (sd->ruler_hor) ui_ruler_hide(sd->ruler_hor);
   if (sd->ruler_ver) ui_ruler_hide(sd->ruler_ver);

   evas_object_hide(sd->button_separate);
   evas_object_hide(sd->scroller);
   evas_object_hide(sd->clipper);

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
_workspace_smart_set_user(Evas_Smart_Class *sc)
{
   //evas_object_smart_clipped_smart_set(sc);
   sc->add = _workspace_smart_add;
   sc->del = _workspace_smart_del;
   sc->show = _workspace_smart_show;
   sc->hide = _workspace_smart_hide;

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

static void
_on_part_select(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info)
{
   Evas_Object *workspace = (Evas_Object *)data;
   evas_object_smart_callback_call(workspace, SIG_PART_SELECTED, event_info);
}

Eina_Bool
workspace_edit_object_set(Evas_Object *obj, Style *style, const char *file)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if ((!style) || (!file)) return false;
   if (!sd->groupedit) sd->groupedit = groupedit_add(sd->scroller);
   else groupedit_edit_object_unset(sd->groupedit);
   sd->style = style;
   elm_menu_item_icon_name_set(sd->menu.items.mode_normal, TET_IMG_PATH"context_menu-bullet.png");
   elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
   groupedit_handler_size_set(sd->groupedit, 8, 8, 8, 8);
   groupedit_edit_object_set(sd->groupedit, style->obj, file);
   evas_object_smart_callback_add(sd->groupedit, "part,selected",
                                  _on_part_select, obj);
   evas_object_smart_callback_add(sd->groupedit, "container,changed",
                                  _ws_ruler_abs_zero_move_cb, obj);
   evas_object_smart_callback_add(sd->groupedit, "object,area,changed",
                                  _ws_ruler_rel_zero_move_cb, obj);
   groupedit_bg_set(sd->groupedit, sd->background);
   elm_object_content_set(sd->scroller, sd->groupedit);
   evas_object_show(sd->groupedit);

   /* Create highlights for object and relative space */
   /*TODO: remake scroller and layout with rulers etc.
           because highlight work wrong because of that */
   if (!sd->highlight.space_hl)
     {
        sd->highlight.space_hl = highlight_add(sd->scroller);
        highlight_bg_color_set(sd->highlight.space_hl, OBG_AREA_BG_COLOR);
        highlight_border_color_set(sd->highlight.space_hl, OBG_AREA_COLOR);
        highlight_handler_disabled_set(sd->highlight.space_hl, true);
        evas_object_smart_member_add(sd->highlight.space_hl, obj);
        evas_object_smart_callback_add(obj, "highlight,visible",
                                       _obj_area_visible_change, sd->highlight.space_hl);
     }

   if (!sd->highlight.highlight)
     {
        sd->highlight.highlight = highlight_add(sd->scroller);
        highlight_bg_color_set(sd->highlight.highlight, HIGHLIGHT_BG_COLOR);
        highlight_handler_color_set(sd->highlight.highlight, HIGHLIGHT_COLOR);
        highlight_border_color_set(sd->highlight.highlight, HIGHLIGHT_COLOR);
        evas_object_smart_member_add(sd->highlight.highlight, obj);
     }

   return true;
}

void
workspace_edit_object_unset(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);

   sd->style = NULL;
   if (sd->groupedit)
     {
        groupedit_edit_object_unset(sd->groupedit);
        elm_object_content_unset(sd->scroller);
        evas_object_del(sd->groupedit);
        sd->groupedit = NULL;
     }
}

Style *
workspace_edit_object_get(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   return sd->style;
}

void
workspace_edit_object_recalc(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);
   groupedit_edit_object_recalc_all(sd->groupedit);
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
        ERR("Can't set state '%s %f' to part '%s' in the group %s!",
            part->curr_state, part->curr_state_value, part->name,
            sd->style->full_group_name)
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
        ERR("Can't %s the part '%s' in the group %s!", visible ? "show" : "false",
            part, sd->style->full_group_name)
        return false;
     }

   return groupedit_part_visible_set(sd->groupedit, part, visible);
}
