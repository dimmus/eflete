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

#include "groupedit_private.h"
#include "cursor.h"

#define MY_CLASS_NAME "Ws_Groupedit"

#define H_WIGTH (sd->handler_TL.w + sd->handler_BR.w)
#define H_HEIGHT (sd->handler_TL.h + sd->handler_BR.h)

#define PADDING_INIT 50

/* here we create and define some functions for creating our groupedit smart
  object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _groupedit,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static Eina_Bool
_user_size_calc(Evas_Object *o)
{
   int nw, nh;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, false)

   nw = sd->con_current_size->w + H_WIGTH;
   nh = sd->con_current_size->h + H_HEIGHT;

   evas_object_resize(o, nw, nh);

   return true;
}

static void
_mouse_down_hTL_cb(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   if (ev->button != 1) return;

   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->downx = ev->canvas.x;
   sd->downy = ev->canvas.y;
   sd->handler_TL_pressed = true;
   edje_object_signal_emit(sd->container, "tooltip,show", "eflete");
}

static void
_mouse_down_hRB_cb(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   if (ev->button != 1) return;

   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->downx = ev->canvas.x;
   sd->downy = ev->canvas.y;
   sd->handler_BR_pressed = true;
   edje_object_signal_emit(sd->container, "tooltip,show", "eflete");
}

static void
_mouse_up_hTL_cb(void *data,
                 Evas *e __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->handler_TL_pressed = false;
   edje_object_signal_emit(sd->container, "tooltip,hide", "eflete");

   /*----some scroll logic-----Move from here-------*/
   int w, h, ge_w, ge_h, gec_w, gec_h;
   int padding_val_left = PADDING_INIT;
   int padding_val_top = PADDING_INIT;

   evas_object_geometry_get(sd->parent, NULL, NULL, &w, &h);
   evas_object_geometry_get(sd->container, NULL, NULL, &gec_w, &gec_h);
   evas_object_geometry_get(data, NULL, NULL, &ge_w, &ge_h);

   if ((ge_w > w) || (ge_h > h))
     {
        /*---------------smart recalc for paddings.---------------*/
        if (gec_w < w) padding_val_left = (w - gec_w) / 2;
        sd->paddings.t_left = padding_val_left;

        if (gec_h < h) padding_val_top = (h - gec_h) / 2;
        sd->paddings.t_top = padding_val_top;

        evas_object_resize(data,gec_w + padding_val_left + PADDING_INIT,
                           gec_h + padding_val_top + PADDING_INIT);
     }
}

static void
_mouse_up_hRB_cb(void *data,
                 Evas *e __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   WS_GROUPEDIT_DATA_GET(data, sd)
   sd->handler_BR_pressed = false;
   edje_object_signal_emit(sd->container, "tooltip,hide", "eflete");
}

static void
_mouse_move_cb(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Evas_Coord x, y, w, h;
   Evas_Coord lw, lh;
   Evas_Coord nw, nh;
   Evas_Coord dx, dy;
   Evas_Event_Mouse_Move *ev = event_info;
   Evas_Object *o = data;

   WS_GROUPEDIT_DATA_GET(o, sd)
   evas_object_geometry_get(o, &x, &y, &w, &h);
   evas_object_geometry_get(sd->container, NULL, NULL, &lw, &lh);
   dx = (ev->cur.canvas.x - sd->downx);
   dy = (ev->cur.canvas.y - sd->downy);

   /* check container size, if size equal min or max size,
      no sense to calc new geometry and render it */
   if (sd->handler_TL_pressed)
     {
        if ((dx < 0) && (dy < 0) &&
            (lw == sd->con_size_max.w) && (lh == sd->con_size_max.h))
          return;

        if ((dx > 0) && (dy > 0) &&
            (lw == sd->con_size_min.w) && (lh == sd->con_size_min.h))
          return;
     }

   if (sd->handler_BR_pressed)
     {
        if ((dx > 0) && (dy > 0) &&
            (lw == sd->con_size_max.w) && (lh == sd->con_size_max.h))
          return;

        if ((dx < 0) && (dy < 0) &&
            (lw == sd->con_size_min.w) && (lh == sd->con_size_min.h))
          return;
     }

   if (sd->handler_TL_pressed)
     {
        /* calc x and wigth */
        nw = w - dx;
        if (nw <= sd->con_size_min.w + H_WIGTH)
          {
             nw = sd->con_size_min.w + H_WIGTH;
          }
        else
          {
             if ((sd->con_size_max.w != -1)
                 && (nw >= sd->con_size_max.w + H_WIGTH))
               nw = sd->con_size_max.w + H_WIGTH;
          }

        /* calc y and height */
        nh = h - dy;
        if (nh <= sd->con_size_min.h + H_HEIGHT)
          {
             nh = sd->con_size_min.h + H_HEIGHT;
          }
        else
          {
             if ((sd->con_size_max.h != -1)
                 && (nh >= sd->con_size_max.h + H_HEIGHT))
               nh = sd->con_size_max.h + H_HEIGHT;
          }

        if ((sd->con_current_size->h) || (dy < 0)) sd->paddings.t_top += dy;
        if ((sd->con_current_size->w) || (dx < 0)) sd->paddings.t_left += dx;

        if ((sd->paddings.t_top < sd->paddings.bottom) ||
            (sd->paddings.t_left < sd->paddings.bottom))
          evas_object_resize(o, nw, nh);

        if (sd->paddings.t_top <= 0) sd->paddings.t_top = sd->paddings.bottom;
        if (sd->paddings.t_left <= 0) sd->paddings.t_left = sd->paddings.bottom;
        evas_object_smart_changed(o);
     }
   if (sd->handler_BR_pressed)
     {
        /* calc wigth and heght */
        nw = w + dx;
        if (nw <= sd->con_size_min.w + H_WIGTH)
          nw = sd->con_size_min.w + H_WIGTH;
        else
          {
             if ((sd->con_size_max.w != -1)
                 && (nw >= sd->con_size_max.w + H_WIGTH))
               nw = sd->con_size_max.w + H_WIGTH;
             else nw = w + dx;
          }

        nh = h + dy;
        if (nh <= sd->con_size_min.h + H_HEIGHT)
          nh = sd->con_size_min.h + H_HEIGHT;
        else
          {
             if ((sd->con_size_max.h != -1)
                 && (nh >= sd->con_size_max.h + H_HEIGHT))
               nh = sd->con_size_max.h + H_HEIGHT;
             else nh = h + dy;
          }
        evas_object_resize(o, nw, nh);
        evas_object_smart_changed(o);
     }
   sd->downx = ev->cur.canvas.x;
   sd->downy = ev->cur.canvas.y;
}

static void
_style_set(Evas_Object *o, const char *style)
{
   char group[512]; /* FIXME: change 512 to BUFF_MAX */
   WS_GROUPEDIT_DATA_GET(o, sd)

   #define GROUP_NAME(item, style) \
      sprintf(group, "eflete/groupedit/%s/%s", item, style);

   GROUP_NAME("base", style)
   if (!edje_object_file_set(sd->container, EFLETE_EDJ, group))
     {
        GROUP_NAME("base", "default")
        edje_object_file_set(sd->container, EFLETE_EDJ, group);
     }
   GROUP_NAME("handler_TL", style)
   if (!edje_object_file_set(sd->handler_TL.obj, EFLETE_EDJ, group))
     {
        GROUP_NAME("handler_TL", "default")
        edje_object_file_set(sd->handler_TL.obj, EFLETE_EDJ, group);
     }
   GROUP_NAME("handler_BR", style)
   if (!edje_object_file_set(sd->handler_BR.obj, EFLETE_EDJ, group))
     {
        GROUP_NAME("handler_BR", "default")
        edje_object_file_set(sd->handler_BR.obj, EFLETE_EDJ, group);
     }
   GROUP_NAME("object_area", style)
   if (!edje_object_file_set(sd->obj_area.obj, EFLETE_EDJ, group))
     {
        GROUP_NAME("object_area", "default")
        edje_object_file_set(sd->obj_area.obj, EFLETE_EDJ, group);
     }

   if (sd->style) free((void *)sd->style);
   sd->style = strdup(style);

   #undef GROUP_NAME
}

static void
_key_down(void *data __UNUSED__,
          Evas *e __UNUSED__,
          Evas_Object *obj,
          void *event_info)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID)
   Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;

   if (sd->obj_area.visible) return;
   if ((!strcmp(ev->keyname, "Alt_L")) || (!strcmp(ev->keyname, "Alt_R")))
     {
        if (sd->selected)
          {
             evas_object_show(sd->obj_area.obj);
             sd->obj_area.show_now = true;
          }
     }
}

static void
_key_up(void *data __UNUSED__,
        Evas *e __UNUSED__,
        Evas_Object *obj,
        void *event_info)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID)
   Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;

   if (sd->obj_area.visible) return;
   if ((!strcmp(ev->keyname, "Alt_L")) || (!strcmp(ev->keyname, "Alt_R")))
     {
        evas_object_hide(sd->obj_area.obj);
        sd->obj_area.show_now = false;
     }
}

static void
_focus_set(void *data __UNUSED__,
           Evas *e __UNUSED__,
           Evas_Object *obj,
           void *event_info __UNUSED__)
{
   evas_object_focus_set(obj, true);
}

static void
_unselect_part(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Evas_Object *o = (Evas_Object *)data;
   Evas_Event_Mouse_Down *ev = event_info;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   if (ev->button != 1) return;
   if (!sd->selected) return;
   if (sd->separated)
     {
        if (!sd->selected) return;
        _selected_item_return_to_place(sd);
     }
   evas_object_hide(sd->obj_area.obj);
   evas_object_smart_callback_call(o, SIG_PART_UNSELECTED,
                                   (void *)sd->selected->name);
   sd->selected = NULL;
}

/* create and setup a new example smart object's internals */
static void
_groupedit_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Ws_Groupedit_Smart_Data)

   _groupedit_parent_sc->add(o);

   priv->e = evas_object_evas_get(o);
   priv->container = edje_object_add(priv->e);
   evas_object_repeat_events_set(priv->container, true);
   priv->event = evas_object_rectangle_add(priv->e);
   evas_object_color_set(priv->event, 0, 0, 0, 0);

   priv->handler_TL.obj = edje_object_add(priv->e);
   priv->handler_TL.w = priv->handler_TL.h = 5;
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hTL_cb, o);
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hTL_cb, o);
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_cb, o);
   cursor_type_set(priv->handler_TL.obj, CURSOR_SIZING);

   priv->handler_BR.obj = edje_object_add(priv->e);
   priv->handler_BR.w = priv->handler_BR.h = 5;
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_cb, o);
   cursor_type_set(priv->handler_BR.obj, CURSOR_SIZING);

   evas_object_event_callback_add(priv->event, EVAS_CALLBACK_MOUSE_UP,
                                  _unselect_part, o);

   evas_object_event_callback_add(o, EVAS_CALLBACK_MOUSE_IN,
                                  _focus_set, NULL);
   evas_object_event_callback_add(o, EVAS_CALLBACK_KEY_DOWN,
                                  _key_down, NULL);
   evas_object_event_callback_add(o, EVAS_CALLBACK_KEY_UP,
                                  _key_up, NULL);

   priv->obj = o;
   priv->con_size_min.w = 0;
   priv->con_size_min.h = 0;
   priv->con_size_max.w = -1;
   priv->con_size_max.h = -1;
   priv->con_current_size = (Groupedit_Geom *)malloc(sizeof(Groupedit_Geom));
   priv->edit_obj = NULL;
   priv->parts = NULL;
   priv->handler_TL_pressed = false;
   priv->handler_BR_pressed = false;
   priv->obj_area.obj = edje_object_add(priv->e);
   evas_object_repeat_events_set(priv->obj_area.obj, true);
   priv->obj_area.visible = false;
   priv->obj_area.show_now = false;
   priv->obj_area.geom = (Groupedit_Geom *)malloc(sizeof(Groupedit_Geom));
   priv->separated = false;
   priv->selected = NULL;
   priv->to_select = NULL;

   priv->paddings.t_left = PADDING_INIT;
   priv->paddings.t_top = PADDING_INIT;
   priv->paddings.bottom = PADDING_INIT;

   evas_object_smart_member_add(priv->container, o);
   evas_object_smart_member_add(priv->handler_TL.obj, o);
   evas_object_smart_member_add(priv->handler_BR.obj, o);
   evas_object_smart_member_add(priv->obj_area.obj, o);
   evas_object_smart_member_add(priv->event, o);

   _style_set(o, "default");
}

static void
_groupedit_smart_del(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   _parts_list_free(sd);

   evas_object_smart_member_del(sd->container);
   evas_object_smart_member_del(sd->handler_TL.obj);
   evas_object_smart_member_del(sd->handler_BR.obj);
   evas_object_smart_member_del(sd->event);

   _groupedit_parent_sc->del(o);
}

static void
_groupedit_smart_show(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   if (sd->separated)
     {
        evas_object_hide(sd->handler_TL.obj);
        evas_object_hide(sd->handler_BR.obj);
        evas_object_hide(sd->container);
     }
   else
     {
        evas_object_show(sd->handler_TL.obj);
        evas_object_show(sd->handler_BR.obj);
        evas_object_show(sd->container);
     }
   evas_object_show(sd->event);

   _groupedit_parent_sc->show(o);
}

static void
_groupedit_smart_hide(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_hide(sd->handler_TL.obj);
   evas_object_hide(sd->handler_BR.obj);
   evas_object_hide(sd->container);
   evas_object_hide(sd->event);

   _groupedit_parent_sc->hide(o);
}

static void
_groupedit_smart_color_set(Evas_Object *o, int r, int g, int b, int a)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_color_set(sd->container, r, g, b, a);
   evas_object_color_set(sd->handler_TL.obj, r, g, b, a);
   evas_object_color_set(sd->handler_BR.obj, r, g, b, a);
}

static void
_groupedit_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ow, oh;
   //WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;
   /* FIXME: uncomment it, when the paddings logic will be removed
   if (w > sd->con_size_max.w + H_WIGTH) w = sd->con_size_max.w + H_WIGTH;
   if (w < sd->con_size_min.w + H_WIGTH) w = sd->con_size_min.w + H_WIGTH;
   if (h > sd->con_size_max.h + H_HEIGHT) h = sd->con_size_max.h + H_HEIGHT;
   if (h < sd->con_size_min.h + H_HEIGHT) h = sd->con_size_min.h + H_HEIGHT;
   */

   evas_object_size_hint_min_set(o, w, h);
   evas_object_smart_changed(o);
}

/* groupedit geometry calculate */
static void
_groupedit_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord cw, ch, pw, ph;
   int htl_w, htl_h;
   int hrb_w, hrb_h;
   char buff[16];

   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, priv, RETURN_VOID)
   evas_object_geometry_get(priv->parent, NULL, NULL, &pw, &ph);
   evas_object_resize(priv->event, pw, ph);

   evas_object_geometry_get(o, &x, &y, &w, &h);
   htl_w = priv->handler_TL.w;
   htl_h = priv->handler_TL.h;
   hrb_w = priv->handler_BR.w;
   hrb_h = priv->handler_BR.h;

   cw = w - (htl_w + hrb_w) - (priv->paddings.t_left + priv->paddings.bottom);
   ch = h - (htl_h + hrb_h) - (priv->paddings.t_top + priv->paddings.bottom);

   if ((cw < 0)) cw = 0; if ((ch < 0)) ch = 0;

   x += priv->paddings.t_left;
   y += priv->paddings.t_top;

   if (!priv->separated)
     {
        evas_object_resize(priv->container, cw, ch);
        evas_object_move(priv->container, x + htl_w, y + htl_h);
        priv->con_current_size->x = priv->paddings.t_left + htl_w;
        priv->con_current_size->y = priv->paddings.t_top + htl_h;
        priv->con_current_size->w = cw;
        priv->con_current_size->h = ch;
        sprintf(buff, "%i %i", priv->con_current_size->w, priv->con_current_size->h);
        edje_object_part_text_set(priv->container, TEXT_TOOLTIP, buff);

        evas_object_resize(priv->handler_TL.obj, htl_w, htl_h);
        evas_object_move(priv->handler_TL.obj, x, y);

        evas_object_resize(priv->handler_BR.obj, hrb_w, hrb_h);
        evas_object_move(priv->handler_BR.obj, x + htl_w + cw, y + htl_h + ch);

        evas_object_show(priv->container);
        evas_object_show(priv->handler_TL.obj);
        evas_object_show(priv->handler_BR.obj);
     }
   else
     {
        evas_object_hide(priv->container);
        evas_object_hide(priv->handler_TL.obj);
        evas_object_hide(priv->handler_BR.obj);
     }

   DBG("Groupedit geometry: x[%i] y[%i] w[%i] h[%i]", x, y, w, h);

   _parts_recalc(priv);

   evas_object_focus_set(o, true);
   evas_object_smart_callback_call(o, SIG_CHANGED, (void *)priv->con_current_size);
}

/* this need for macro EVAS_SMART_SUBCLASS_NEW */
static void
_groupedit_smart_set_user(Evas_Smart_Class *sc)
{
   evas_object_smart_clipped_smart_set(sc);

   sc->add = _groupedit_smart_add;
   sc->del = _groupedit_smart_del;
   sc->show = _groupedit_smart_show;
   sc->hide = _groupedit_smart_hide;
   sc->color_set = _groupedit_smart_color_set;

   /* clipped smart object has no hook on resizes or calculations */
   sc->resize = _groupedit_smart_resize;
   sc->calculate = _groupedit_smart_calculate;
}

Evas_Object *
groupedit_add(Evas_Object *parent)
{
   Evas *e;
   Evas_Object *obj;

   if (!parent) return NULL;

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _groupedit_smart_class_new());
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);
   sd->parent = parent;

   return obj;
}

Eina_Bool
groupedit_handler_size_set(Evas_Object *obj, int htl_w, int htl_h, int hbr_w, int hbr_h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (htl_w < 5) sd->handler_TL.w = 5;
   else sd->handler_TL.w = htl_w;
   if (htl_h < 5) sd->handler_TL.h = 5;
   else sd->handler_TL.h = htl_h;
   if (hbr_w < 5) sd->handler_BR.w = 5;
   else sd->handler_BR.w = hbr_w;
   if (hbr_h < 5) sd->handler_BR.h = 5;
   else sd->handler_BR.h = hbr_h;

   return true;
}

void
groupedit_handler_size_get(Evas_Object *obj, int *htl_w, int *htl_h, int *hbr_w, int *hbr_h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);

   if (htl_w) *htl_w = sd->handler_TL.w;
   if (htl_h) *htl_h = sd->handler_TL.h;
   if (hbr_w) *hbr_w = sd->handler_BR.w;
   if (hbr_h) *hbr_h = sd->handler_BR.h;
}

Eina_Bool
groupedit_min_size_set(Evas_Object *obj, int w, int h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w < 0) sd->con_size_min.w = 0;
   else sd->con_size_min.w = w;
   if (h < 0) sd->con_size_min.h = 0;
   else sd->con_size_min.h = h;

   return true;
}

Eina_Bool
groupedit_max_size_set(Evas_Object *obj, int w, int h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w < 0) sd->con_size_max.w = -1;
   else sd->con_size_max.w = w;
   if (h < 0) sd->con_size_max.h = -1;
   else sd->con_size_max.h = h;

   return true;
}

Eina_Bool
groupedit_container_size_set(Evas_Object *obj, int w, int h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w <= sd->con_size_min.w) sd->con_current_size->w = sd->con_size_min.w;
   else
     {
        if ((sd->con_size_max.w != -1) && (w > sd->con_size_max.w))
          sd->con_current_size->w = sd->con_size_max.w;
        else sd->con_current_size->w = w;
     }

   if (h <= sd->con_size_min.h) sd->con_current_size->h = sd->con_size_min.h;
   else
     {
        if ((sd->con_size_max.h != -1) && (h > sd->con_size_max.h))
          sd->con_current_size->h = sd->con_size_max.h;
        else sd->con_current_size->h = h;
     }

   return _user_size_calc(obj);
}

Eina_Bool
groupedit_container_size_get(Evas_Object *obj, int *w, int *h)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w) *w = sd->con_current_size->w;
   if (h) *h = sd->con_current_size->h;

   return true;
}

Eina_Bool
groupedit_style_set(Evas_Object *obj, const char *style)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!style) return false;
   if (!strcmp(sd->style, style)) return true;
   _style_set(obj, style);

   return true;
}

const char *
groupedit_style_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);
   return sd->style;
}

/******************************************************************************/
/******************************************************************************/

Eina_Bool
groupedit_edit_object_set(Evas_Object *obj,
                          Evas_Object *edit_obj,
                          const char *file)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   /* check input edit_obj, if it not a edje object return false */
   if ((!edit_obj) || (strcmp("edje", evas_object_type_get(edit_obj))))
     {
        /* TODO: add error message */
        return false;
     }
   if ((!file) || (!ecore_file_exists(file))) return false;

   edje_object_animation_set(edit_obj, false);
   edje_object_part_swallow(sd->container, SWALLOW_FOR_EDIT, edit_obj);
   /* hide the editing object, we can not use evas_object_hide, because object
    * will be showed again, after changing, for example part add, or change
    * part state. so set the object opacity 0 - object invisible and calculate
    * geometry. */
   evas_object_color_set(edit_obj, 0, 0, 0, 0);
   /*TODO: set the state for all parts to default 0.0 */
   sd->edit_obj = edit_obj;
   sd->edit_obj_file = file;
   sd->con_size_min.w = edje_edit_group_min_w_get(edit_obj);
   sd->con_size_min.h = edje_edit_group_min_h_get(edit_obj);
   /*  */
   if (edje_edit_group_max_w_get(edit_obj) == 0)
     sd->con_size_max.w = -1;
   else
     sd->con_size_max.w = edje_edit_group_max_w_get(edit_obj);
   if (edje_edit_group_max_h_get(edit_obj) == 0)
     sd->con_size_max.h = -1;
   else
     sd->con_size_max.h = edje_edit_group_max_h_get(edit_obj);

   _edit_object_load(sd);
   if (sd->parts) _parts_list_free(sd);
   _parts_list_new(sd);

   evas_object_smart_changed(sd->obj);

   return true;
}

Evas_Object *
groupedit_edit_object_unset(Evas_Object *obj)
{
   Evas_Object *ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   if ((!sd->edit_obj) && (!sd->parts)) return NULL;

   _parts_list_free(sd);
   ret = sd->edit_obj;
   sd->edit_obj = NULL;

   sd->con_size_min.w = 0;
   sd->con_size_min.h = 0;
   sd->con_size_max.w = -1;
   sd->con_size_max.h = -1;

   return ret;
}

Eina_Bool
groupedit_edit_object_recalc_all(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   return _parts_recalc(sd);
}

Evas_Object *
groupedit_edit_object_part_draw_get(Evas_Object *obj, const char *part)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL)
   if (!part) return NULL;

   gp = _parts_list_find(sd->parts, part);

   return gp->border;
}

Eina_Bool
groupedit_edit_object_part_add(Evas_Object *obj, const char *part,
                               Edje_Part_Type type, const char *data)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part) return false;

   return _edit_object_part_add(sd, part, type, data);
}

Eina_Bool
groupedit_edit_object_part_del(Evas_Object *obj, const char *part)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part) return false;

   return _edit_object_part_del(sd, part);
}

Eina_Bool
groupedit_edit_object_part_above(Evas_Object *obj, const char *part)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part) return false;
   return _edit_object_part_restack_above(sd, part, NULL);
}

Eina_Bool
groupedit_edit_object_part_below(Evas_Object *obj, const char *part)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!part) return false;
   return _edit_object_part_restack_below(sd, part, NULL);
}

Eina_Bool
groupedit_edit_object_part_move_above(Evas_Object *obj,
                                      const char *part,
                                      const char *above)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   return _edit_object_part_restack_above(sd, part, above);
}

Eina_Bool
groupedit_edit_object_part_move_below(Evas_Object *obj,
                                      const char *part,
                                      const char *below)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   return _edit_object_part_restack_below(sd, part, below);
}

Eina_Bool
groupedit_edit_object_part_state_set(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   ret = edje_edit_part_selected_state_set(sd->edit_obj, part, state, value);

   evas_object_smart_changed(sd->obj);
   return ret;
}

Eina_Bool
groupedit_edit_object_part_state_add(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state)) return false;

   ret = edje_edit_state_add(sd->edit_obj, part, state, value);
   ret &= edje_edit_part_selected_state_set(sd->edit_obj, part, state, value);

   if (ret) evas_object_smart_changed(sd->obj);
   return ret;
}

Eina_Bool
groupedit_edit_object_part_state_copy(Evas_Object *obj, const char *part,
                                      const char *state_from, double value_from,
                                      const char *state_to, double value_to)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state_from) || (!state_to)) return false;

   ret = edje_edit_state_copy(sd->edit_obj, part, state_from, value_from,
                              state_to, value_to);
   ret &= edje_edit_part_selected_state_set(sd->edit_obj, part, state_to, value_to);

   if (ret) evas_object_smart_changed(sd->obj);
   return ret;
}

Eina_Bool
groupedit_edit_object_part_state_del(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state)) return false;

   ret = edje_edit_state_del(sd->edit_obj, part, state, value);

   evas_object_smart_changed(sd->obj);
   return ret;
}

Evas_Object *
groupedit_part_object_area_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL)

   return sd->obj_area.obj;
}

void
groupedit_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);
   sd->obj_area.visible = visible;

   if (!sd->selected) return;

   if (visible) evas_object_show(sd->obj_area.obj);
   else evas_object_hide(sd->obj_area.obj);
}

Eina_Bool
groupedit_part_object_area_visible_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   return sd->obj_area.visible;
}

void
groupedit_edit_object_parts_separated(Evas_Object *obj,
                                      Eina_Bool separated)
{
   int w, h, count;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);
   if ((!sd->edit_obj) || (!sd->parts)) return;
   if (sd->separated == separated) return;

   sd->separated = separated;
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   count = eina_list_count(sd->parts);
   /* after resize the groupedit object it will be marked as dirty,
      and parts will be recalced. */
   if (separated)
     {
        evas_object_resize(obj, w + (SEP_ITEM_PAD_X * count), h + (SEP_ITEM_PAD_Y * count));
        evas_object_smart_callback_call(obj, SIG_PART_SEPARETE_OPEN, NULL);
        if (sd->selected)
          {
             sd->to_select = sd->selected;
             sd->selected = NULL;
          }
        _select_item_move_to_top(sd);
     }
   else
     {
        evas_object_resize(obj, w - (SEP_ITEM_PAD_X * count), h - (SEP_ITEM_PAD_Y * count));
        /* emit the signal that the groupedit returned to the normal mode.
           send the name of selected item(part), for hilight and widget list
           events. */
        DBG("%s", sd->selected ? sd->selected->name : NULL);
        evas_object_smart_callback_call(obj, SIG_PART_SEPARETE_CLOSE, NULL);
        _selected_item_return_to_place(sd);
     }
}

Eina_Bool
groupedit_edit_object_parts_separated_is(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   return sd->separated;
}

void
groupedit_edit_object_part_select(Evas_Object *obj, const char *part)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, RETURN_VOID);

   gp = _parts_list_find(sd->parts, part);
   if (!sd->separated) sd->selected = gp;
   else
     {
        if (!part) _selected_item_return_to_place(sd);
        else
          {
             if (gp) sd->to_select = gp;
             _select_item_move_to_top(sd);
          }
     }
}

Eina_Bool
groupedit_part_visible_set(Evas_Object *obj, const char *part, Eina_Bool visible)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!part) return false;

   gp = _parts_list_find(sd->parts, part);
   if (!gp) return false;
   gp->visible = visible;

   _parts_recalc(sd);
   return true;
}

Eina_Bool
groupedit_bg_set(Evas_Object *obj, Evas_Object *bg)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!bg) return false;

   sd->bg = bg;
   return true;
}

Evas_Object *
groupedit_bg_unset(Evas_Object *obj)
{
   Evas_Object *bg;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   bg = sd->bg;
   sd->bg = NULL;

   return bg;
}

#undef MY_CLASS_NAME
#undef WS_GROUPSPACE_DATA_GET
#undef WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL
#undef H_WIGTH
#undef H_HEIGHT
