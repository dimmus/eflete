/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
   Evas_Object *events;         /**< Needed for processing common events,\
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
   items->zoom_in = elm_menu_item_add(menu, items->zoom, NULL, _("Zoom in"), NULL, NULL);
   elm_object_item_disabled_set(items->zoom_in, true);
   items->zoom_out = elm_menu_item_add(menu, items->zoom, NULL, _("Zoom out"), NULL, NULL);
   elm_object_item_disabled_set(items->zoom_out, true);
   elm_menu_item_separator_add(menu, items->zoom);
   items->zoom_fit = elm_menu_item_add(menu, items->zoom, NULL, _("Fit"), NULL, NULL);
   elm_object_item_disabled_set(items->zoom_fit, true);
   elm_menu_item_separator_add(menu, items->zoom);
   items->zoom_far = elm_menu_item_add(menu, items->zoom, NULL, _("20%"), NULL, NULL);
   elm_object_item_disabled_set(items->zoom_far, true);
   items->zoom_normal = elm_menu_item_add(menu, items->zoom, NULL, _("100%"), NULL, NULL);
   elm_object_item_disabled_set(items->zoom_normal, true);
   items->zoom_near = elm_menu_item_add(menu, items->zoom, NULL, _("500%"), NULL, NULL);
   elm_object_item_disabled_set(items->zoom_near, true);
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
        if (ui_ruler_visible_get(sd->ruler_hor))
          {
             elm_layout_signal_emit(sd->scroller, "rulers,hide", "eflete");
             elm_menu_item_icon_name_set(sd->menu.items.rulers_enable, "");
             ui_ruler_hide(sd->ruler_hor);
             ui_ruler_hide(sd->ruler_ver);
          }
        else
          {
             ui_ruler_show(sd->ruler_hor);
             ui_ruler_show(sd->ruler_ver);
             elm_menu_item_icon_name_set(sd->menu.items.rulers_enable, EFLETE_IMG_PATH"context_menu-check.png");
             elm_layout_signal_emit(sd->scroller, "rulers,show", "eflete");
          }
     }
   else if (!strcmp(data_info, "abs"))
     {
        ui_ruler_scale_absolute_visible_set(sd->ruler_hor, true);
        ui_ruler_scale_absolute_visible_set(sd->ruler_ver, true);
        ui_ruler_scale_relative_visible_set(sd->ruler_hor, false);
        ui_ruler_scale_relative_visible_set(sd->ruler_ver, false);
        elm_menu_item_icon_name_set(sd->menu.items.rulers_abs, EFLETE_IMG_PATH"context_menu-bullet.png");
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
        elm_menu_item_icon_name_set(sd->menu.items.rulers_rel, EFLETE_IMG_PATH"context_menu-bullet.png");
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

   int cross_size = 0;
   evas_object_geometry_get(sd->ruler_hor, NULL, NULL, NULL, &cross_size);

   ui_ruler_scale_absolute_position_zero_set(sd->ruler_hor, ge_geom->x - cross_size);
   ui_ruler_scale_absolute_position_zero_set(sd->ruler_ver, ge_geom->y - cross_size);

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

   int cross_size = 0;
   evas_object_geometry_get(sd->ruler_hor, NULL, NULL, NULL, &cross_size);


   ui_ruler_scale_relative_position_set(sd->ruler_hor, ge_geom->x - cross_size,
                                        ge_geom->x + ge_geom->w - cross_size);
   ui_ruler_scale_relative_position_set(sd->ruler_ver, ge_geom->y - cross_size,
                                        ge_geom->y + ge_geom->h - cross_size);

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


   Evas_Object *object_area = groupedit_part_object_area_get(sd->groupedit);

   evas_object_geometry_get(sd->ruler_hor, &cross_size, NULL, NULL, NULL);
   evas_object_geometry_get(sd->background, &bg_x, &bg_y, NULL, NULL);
   evas_object_geometry_get(sd->style->obj, &gs_x, &gs_y, &gs_w, &gs_h);
   cross_size -= bg_x;
   ui_ruler_scale_absolute_position_zero_set(sd->ruler_hor, gs_x - bg_x - cross_size);
   ui_ruler_scale_absolute_position_zero_set(sd->ruler_ver, gs_y - bg_y - cross_size);



   if ((groupedit_edit_object_parts_separated_is(sd->groupedit))
       && (sd->scroll_flag < 2))
     evas_object_smart_callback_call(sd->scroller, "vbar,drag", NULL);
   else
     groupedit_edit_object_recalc_all(sd->groupedit);
   sd->scroll_flag = 0;

   evas_object_geometry_get(object_area, &gs_x, &gs_y, &gs_w, &gs_h);
   ui_ruler_scale_relative_position_set(sd->ruler_hor, gs_x - bg_x - cross_size,
                                        gs_x + gs_w - bg_x - cross_size);
   ui_ruler_scale_relative_position_set(sd->ruler_ver, gs_y - bg_y - cross_size,
                                        gs_y + gs_h - bg_y - cross_size);
   ui_ruler_redraw(sd->ruler_hor);
   ui_ruler_redraw(sd->ruler_ver);

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
        evas_object_image_file_set(image, EFLETE_IMG_PATH"bg_demo.png", NULL);
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
workspace_zoom_factor_set(Evas_Object *obj, double factor)
{
   if ((factor < 0.01) || (factor > 20 )) return false;
   return _zoom_factor_update(obj, factor);
}

double
workspace_zoom_factor_get(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, 0);
   return sd->zoom.factor;
}

Eina_Bool
workspace_background_image_set(Evas_Object *obj, const char *path)
{
   if (!path) return false;
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   if (!sd->background) return false;
   if (!_background_load(sd->background, path)) return false;
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
   priv->background = evas_object_image_filled_add(e);
   _background_load(priv->background, EFLETE_IMG_PATH"bg_demo.png");
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
   priv->scroller = elm_scroller_add(priv->layout);
   elm_object_style_set(priv->scroller, "eflete/workspace");
   elm_scroller_policy_set(priv->scroller, ELM_SCROLLER_POLICY_ON,
                           ELM_SCROLLER_POLICY_ON);
   elm_scroller_content_min_limit(priv->scroller, false, false);
   elm_layout_content_set(priv->layout, "groupspace", priv->scroller);

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
   elm_image_file_set(icon, EFLETE_IMG_PATH"icon-separate.png", NULL);
   elm_image_no_scale_set(icon, true);
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
   WS_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_show(sd->button_separate);
   evas_object_show(sd->scroller);
   evas_object_show(sd->events);

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

Eina_Bool
workspace_edit_object_set(Evas_Object *obj, Style *style, const char *file)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if ((!style) || (!file)) return false;
   if (!sd->groupedit)
     {
        sd->groupedit = groupedit_add(sd->scroller);
        /* it temporary solution white not implemented preference module
           and not finished config module */
        evas_object_color_set(sd->groupedit, 0, 0, 0, 255);
     }
   else groupedit_edit_object_unset(sd->groupedit);
   evas_object_focus_set(sd->groupedit, true);
   sd->style = style;
   elm_menu_item_icon_name_set(sd->menu.items.mode_normal,
                               EFLETE_IMG_PATH"context_menu-bullet.png");
   elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
   if (!groupedit_edit_object_set(sd->groupedit, style->obj, file)) return false;
   groupedit_handler_size_set(sd->groupedit, 8, 8, 8, 8);
   evas_object_smart_callback_add(sd->groupedit, "part,selected",
                                  _on_part_select, obj);
   evas_object_smart_callback_add(sd->groupedit, "part,unselected",
                                  _on_part_unselect, obj);
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
        evas_object_color_set(sd->highlight.space_hl, OBG_AREA_COLOR);
        highlight_handler_disabled_set(sd->highlight.space_hl, true);
        evas_object_smart_member_add(sd->highlight.space_hl, obj);
        evas_object_smart_callback_add(obj, "highlight,visible",
                                       _obj_area_visible_change, sd->highlight.space_hl);
     }

   if (!sd->highlight.highlight)
     {
        sd->highlight.highlight = highlight_add(sd->scroller);
        evas_object_color_set(sd->highlight.highlight, HIGHLIGHT_COLOR);
        evas_object_smart_member_add(sd->highlight.highlight, obj);
     }

   elm_object_item_disabled_set(sd->menu.items.mode_normal, false);
   elm_object_item_disabled_set(sd->menu.items.mode_separate, false);

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
   elm_object_content_unset(sd->scroller);
   evas_object_del(sd->groupedit);
   sd->groupedit = NULL;

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

        elm_menu_item_icon_name_set(sd->menu.items.mode_separate, EFLETE_IMG_PATH"context_menu-bullet.png");
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

        elm_menu_item_icon_name_set(sd->menu.items.mode_normal, EFLETE_IMG_PATH"context_menu-bullet.png");
        elm_menu_item_icon_name_set(sd->menu.items.mode_separate, "");
     }

   groupedit_edit_object_parts_separated(sd->groupedit, separate);
   return true;
}

Eina_Bool
workspace_separate_mode_get(Evas_Object *obj)
{
   WS_DATA_GET_OR_RETURN_VAL(obj, sd, false)
   return groupedit_edit_object_parts_separated_is(sd->groupedit);
}
