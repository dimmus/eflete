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

#include "container.h"
#include "cursor.h"

#define MY_CLASS_NAME "Container"

#define BASE_PADDING 50
#define H_WIGTH (sd->handler_TL.w + sd->handler_BR.w + (BASE_PADDING * 2))
#define H_HEIGHT (sd->handler_TL.h + sd->handler_BR.h + (BASE_PADDING * 2))

typedef struct _Container_Smart_Data Container_Smart_Data;

static const char SIG_CHANGED[] = "container,changed";
static const char SIG_HANDLER_TL_MOVE[] = "handler,TL,moved";
static const char SIG_HANDLER_BR_MOVE[] = "handler,BR,moved";

static const char TEXT_TOOLTIP[] = "eflete.size.tooltip";
static const char SWALLOW[] = "eflete.swallow.container";

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_CHANGED, "(iiii)"},
   {SIG_HANDLER_TL_MOVE, "(ii)"},
   {SIG_HANDLER_BR_MOVE, "(ii)"},
   {NULL, NULL}
};

struct _Container_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas *e;
   Evas_Object *obj;
   Evas_Object *container;
   Evas_Object *bg; /* background is important for working in scroller. */
   /* Minimal and maximum size of the container,
      i.e size of the edie_edit object */
   struct {
      Evas_Coord w; /* default: 0 */
      Evas_Coord h; /* default: 0 */
   } con_size_min;
   struct {
      Evas_Coord w; /* default: -1, size is not limited */
      Evas_Coord h; /* default: -1, size is not limited */
   } con_size_max;
   struct {
      Evas_Coord w;
      Evas_Coord h;
   } pad_left_top;
   struct {
      Evas_Coord w;
      Evas_Coord h;
   } pad_right_bottom;
   Container_Geom *size;
   struct {
      Evas_Object *obj;
      Evas_Coord w, h;
   } handler_TL;
   struct{
      Evas_Object *obj;
      Evas_Coord w, h;
   } handler_BR;
   Eina_Bool handler_TL_pressed : 1;
   Eina_Bool handler_BR_pressed : 1;
   Eina_Stringshare *style;
   Evas_Coord downx;
   Evas_Coord downy;
   Evas_Coord dx, dy;
};

#define CONTAINER_DATA_GET(o, ptr) \
   assert(o != NULL); \
   Container_Smart_Data *ptr = evas_object_smart_data_get(o); \
   assert(ptr != NULL);

/* here we create and define some functions for creating our container smart
 * object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _container,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static void
_mouse_down_hTL_cb(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   if (ev->button != 1) return;

   CONTAINER_DATA_GET(data, sd)
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

   CONTAINER_DATA_GET(data, sd)
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
   CONTAINER_DATA_GET(data, sd)
   sd->handler_TL_pressed = false;
   edje_object_signal_emit(sd->container, "tooltip,hide", "eflete");
}

static void
_mouse_up_hRB_cb(void *data,
                 Evas *e __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   CONTAINER_DATA_GET(data, sd)
   sd->handler_BR_pressed = false;
   edje_object_signal_emit(sd->container, "tooltip,hide", "eflete");
}

static void
_mouse_move_hTL_cb(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Coord x1, y1, w1, h1, x2, y2, w2, h2, x, y, w, h;
   Evas_Coord nx = 0, ny = 0, nw, nh;
   Evas_Coord bgx, bgy, bgw, bgh;
   Evas_Coord dx, dy;
   Evas_Event_Mouse_Move *ev = event_info;
   Evas_Object *o = data;

   CONTAINER_DATA_GET(o, sd)
   evas_object_geometry_get(o, &nx, &ny, &nw, &nh);
   evas_object_geometry_get(o, &x, &y, &w, &h);
   evas_object_geometry_get(sd->handler_TL.obj, &x1, &y1, &w1, &h1);
   evas_object_geometry_get(sd->handler_BR.obj, &x2, &y2, &w2, &h2);
   evas_object_geometry_get(sd->bg, &bgx, &bgy, &bgw, &bgh);
   dx = (ev->cur.canvas.x - sd->downx);
   dy = (ev->cur.canvas.y - sd->downy);

   if (!sd->handler_TL_pressed) return;

   Evas_Coord container_width = x2 - (x1 + w1 + dx);
   Evas_Coord container_height = y2 - (y1 + h1 + dy);
   if ((container_width < sd->con_size_min.w) ||
       ((sd->con_size_max.w != 0) && (container_width > sd->con_size_max.w)))
     dx = 0;

   if ((container_height < sd->con_size_min.h) ||
       ((sd->con_size_max.h != 0) && (container_height > sd->con_size_max.h)))
     dy = 0;

   sd->dx += dx;
   sd->dy += dy;

   if (x1 + dx - sd->pad_left_top.w < bgx)
     {
        nw = w - dx;
        TODO("need do refactoring here")
        if (nw <= sd->con_size_min.w + H_WIGTH)
          {
             nw = sd->con_size_min.w + H_WIGTH + sd->pad_left_top.w + sd->pad_right_bottom.w;
             nx = x + (w - nw);
          }
        else
          {
             if ((sd->con_size_max.w != 0)
                 && (nw >= sd->con_size_max.w + H_WIGTH))
               {
                  nw = sd->con_size_max.w + H_WIGTH + sd->pad_left_top.w + sd->pad_right_bottom.w;
                  nx = x + (w - nw);
               }
             else nx = x + dx;
          }
     }
   sd->pad_left_top.w += dx;

   if (y1 + dy - sd->pad_left_top.h < bgy)
     {
        /* calc y and height */
        nh = h - dy;
        if (nh <= sd->con_size_min.h + H_HEIGHT)
          {
             nh = sd->con_size_min.h + H_HEIGHT + sd->pad_left_top.h + sd->pad_right_bottom.h;
             ny = y + (h - nh);
          }
        else
          {
             if ((sd->con_size_max.h != 0)
                 && (nh >= sd->con_size_max.h + H_HEIGHT))
               {
                  nh = sd->con_size_max.h + H_HEIGHT + sd->pad_left_top.h + sd->pad_right_bottom.h;
                  ny = y + (h - nh);
               }
             else ny = y + dy;
          }
     }
   sd->pad_left_top.h += dy;

   evas_object_resize(o, nw, nh);
   evas_object_move(o, nx, ny);

   sd->size->dx = dx;
   sd->size->dy = dy;
   evas_object_smart_changed(o);

   sd->downx = ev->cur.canvas.x;
   sd->downy = ev->cur.canvas.y;

   evas_object_smart_callback_call(o, SIG_HANDLER_TL_MOVE, (void *)sd->size);
}

static void
_mouse_move_hBR_cb(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Coord x, y, w, h;
   Evas_Coord nw, nh;
   Evas_Coord dx, dy;
   Evas_Event_Mouse_Move *ev = event_info;
   Evas_Object *o = data;

   CONTAINER_DATA_GET(o, sd)
   evas_object_geometry_get(o, &x, &y, &w, &h);
   dx = (ev->cur.canvas.x - sd->downx);
   dy = (ev->cur.canvas.y - sd->downy);

   if (!sd->handler_BR_pressed) return;

   /* calc wigth and heght */
   TODO("need do refactoring here")
   if (sd->size->w + H_WIGTH + dx <= sd->con_size_min.w + H_WIGTH)
     nw = sd->con_size_min.w + H_WIGTH + sd->dx + sd->pad_left_top.w + sd->pad_right_bottom.w;
   else
     {
        if ((sd->con_size_max.w != 0)
            && (sd->size->w + H_WIGTH + dx >= sd->con_size_max.w + H_WIGTH))
          nw = sd->con_size_max.w + H_WIGTH + sd->dx + sd->pad_left_top.w + sd->pad_right_bottom.w;
        else nw = w + dx;
     }

   if (sd->size->h + H_HEIGHT + dy <= sd->con_size_min.h + H_HEIGHT)
     nh = sd->con_size_min.h + H_HEIGHT + sd->dy + sd->pad_left_top.h + sd->pad_right_bottom.h;
   else
     {
        if ((sd->con_size_max.h != 0)
            && (sd->size->h + H_HEIGHT + dy >= sd->con_size_max.h + H_HEIGHT))
          nh = sd->con_size_max.h + H_HEIGHT + sd->dy + sd->pad_left_top.h + sd->pad_right_bottom.h;
        else nh = h + dy;
     }
   evas_object_resize(o, nw, nh);

   sd->size->dx = dx;
   sd->size->dy = dy;
   evas_object_smart_changed(o);

   sd->downx = ev->cur.canvas.x;
   sd->downy = ev->cur.canvas.y;

   sd->size->dx = dx;
   sd->size->dy = dy;
   evas_object_smart_callback_call(o, SIG_HANDLER_BR_MOVE, (void *)sd->size);
}

static void
_style_set(Evas_Object *o, const char *style)
{
   char group[BUFF_MAX];
   CONTAINER_DATA_GET(o, sd)

   #define GROUP_NAME(item, style) \
      snprintf(group, BUFF_MAX, "eflete/container/%s/%s", item, style);

   GROUP_NAME("base", style)
   if (!edje_object_file_set(sd->container, EFLETE_EDJ, group))
     {
        GROUP_NAME("base", "default")
        if (!edje_object_file_set(sd->container, EFLETE_EDJ, group))
          {
             ERR("Couldn't load default style for base border of container.");
             abort();
          }
     }
   GROUP_NAME("handler_TL", style)
   if (!edje_object_file_set(sd->handler_TL.obj, EFLETE_EDJ, group))
     {
        GROUP_NAME("handler_TL", "default")
        if (!edje_object_file_set(sd->handler_TL.obj, EFLETE_EDJ, group))
          {
             ERR("Couldn't load default style for top-left handler of container.");
             abort();
          }
     }
   GROUP_NAME("handler_BR", style)
   if (!edje_object_file_set(sd->handler_BR.obj, EFLETE_EDJ, group))
     {
        GROUP_NAME("handler_BR", "default")
        if (!edje_object_file_set(sd->handler_BR.obj, EFLETE_EDJ, group))
          {
             ERR("Couldn't load default style for bottom-right handler of container.");
             abort();
          }
     }

   eina_stringshare_del(sd->style);
   sd->style = eina_stringshare_add(style);

   #undef GROUP_NAME
}

/* create and setup a new example smart object's internals */
static void
_container_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Container_Smart_Data)

   _container_parent_sc->add(o);
   priv->e = evas_object_evas_get(o);
   priv->container = edje_object_add(priv->e);

   priv->handler_TL.obj = edje_object_add(priv->e);
   priv->handler_TL.w = priv->handler_TL.h = 5;
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hTL_cb, o);
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hTL_cb, o);
   evas_object_event_callback_add(priv->handler_TL.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_hTL_cb, o);
   cursor_type_set(priv->handler_TL.obj, CURSOR_SIZING);

   priv->handler_BR.obj = edje_object_add(priv->e);
   priv->handler_BR.w = priv->handler_BR.h = 5;
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_hBR_cb, o);
   cursor_type_set(priv->handler_BR.obj, CURSOR_SIZING);

   priv->bg = evas_object_rectangle_add(priv->e);
   evas_object_color_set(priv->bg, 0, 0, 0, 0);

   priv->obj = o;
   priv->con_size_min.w = 0;
   priv->con_size_min.h = 0;
   priv->con_size_max.w = 0;
   priv->con_size_max.h = 0;
   priv->pad_left_top.w = 0;
   priv->pad_left_top.h = 0;
   priv->pad_right_bottom.w = 0;
   priv->pad_right_bottom.h = 0;
   priv->size = (Container_Geom *)mem_calloc(1, sizeof(Container_Geom));
   priv->size->w = 0;
   priv->size->h = 0;
   priv->handler_TL_pressed = false;
   priv->handler_BR_pressed = false;

   evas_object_smart_member_add(priv->bg, o);
   evas_object_smart_member_add(priv->container, o);
   evas_object_smart_member_add(priv->handler_TL.obj, o);
   evas_object_smart_member_add(priv->handler_BR.obj, o);

   _style_set(o, "default");
   evas_object_resize(o,
                      priv->handler_TL.w + priv->handler_BR.w + (BASE_PADDING * 2),
                      priv->handler_TL.h + priv->handler_BR.h + (BASE_PADDING * 2));
}

static void
_container_smart_del(Evas_Object *o)
{
   CONTAINER_DATA_GET(o, sd)

   eina_stringshare_del(sd->style);
   free(sd->size);

   _container_parent_sc->del(o);
}

static void
_container_smart_show(Evas_Object *o)
{
   CONTAINER_DATA_GET(o, sd);

   if (sd->handler_TL.obj) evas_object_show(sd->handler_TL.obj);
   if (sd->handler_BR.obj) evas_object_show(sd->handler_BR.obj);
   if (sd->bg) evas_object_show(sd->bg);

   evas_object_show(sd->container);
   _container_parent_sc->show(o);
}

static void
_container_smart_hide(Evas_Object *o)
{
   CONTAINER_DATA_GET(o, sd)

   if (sd->handler_TL.obj) evas_object_hide(sd->handler_TL.obj);
   if (sd->handler_BR.obj) evas_object_hide(sd->handler_BR.obj);
   if (sd->bg) evas_object_hide(sd->bg);

   evas_object_hide(sd->container);
   _container_parent_sc->hide(o);
}

static void
_container_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ow, oh;
   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   evas_object_size_hint_min_set(o, w, h);

   evas_object_smart_changed(o);
}

static void
_container_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord cx, cy, cw, ch;
   Evas_Coord htl_x, htl_y, htl_w, htl_h;
   Evas_Coord hrb_x, hrb_y, hrb_w, hrb_h;
   char buff[16];

   CONTAINER_DATA_GET(o, priv)
   evas_object_geometry_get(o, &x, &y, &w, &h);

   evas_object_move(priv->bg, x, y);
   evas_object_resize(priv->bg, w, h);


   htl_w = priv->handler_TL.w; htl_h = priv->handler_TL.h;
   hrb_w = priv->handler_BR.w; hrb_h = priv->handler_BR.h;

   if (priv->dx < 0) priv->dx = 0;
   if (priv->dy < 0) priv->dy = 0;

   cw = w - (htl_w + hrb_w + priv->pad_left_top.w + priv->pad_right_bottom.w + priv->dx + (BASE_PADDING * 2));
   ch = h - (htl_h + hrb_h + priv->pad_left_top.h + priv->pad_right_bottom.h + priv->dy + (BASE_PADDING * 2));
   cx = x + priv->pad_left_top.w + htl_w + priv->dx + BASE_PADDING;
   cy = y + priv->pad_left_top.h + htl_h + priv->dy + BASE_PADDING;

   /* calculating min and max of groups */
   if ((priv->con_size_min.w > cw) && (priv->con_size_min.w != 0))
     cw = priv->con_size_min.w;
   if ((priv->con_size_max.w < cw) && (priv->con_size_max.w != 0))
     cw = priv->con_size_max.w;
   if ((priv->con_size_min.h > ch) && (priv->con_size_min.h != 0))
     ch = priv->con_size_min.h;
   if ((priv->con_size_max.h < ch) && (priv->con_size_max.h != 0))
     ch = priv->con_size_max.h;

   htl_x = x + priv->pad_left_top.w + priv->dx + BASE_PADDING;
   htl_y = y + priv->pad_left_top.h + priv->dy + BASE_PADDING;
   hrb_x = htl_x + htl_w + cw;
   hrb_y = htl_y + htl_h + ch;

   evas_object_resize(priv->container, cw, ch);
   evas_object_move(priv->container, cx, cy);

   priv->size->x = cx;
   priv->size->y = cy;
   priv->size->w = cw;
   priv->size->h = ch;

   snprintf(buff, 16, "%i %i", priv->size->w, priv->size->h);
   edje_object_part_text_set(priv->container, TEXT_TOOLTIP, buff);

   evas_object_resize(priv->handler_TL.obj, htl_w, htl_h);
   evas_object_move(priv->handler_TL.obj, htl_x, htl_y);

   evas_object_resize(priv->handler_BR.obj, hrb_w, hrb_h);
   evas_object_move(priv->handler_BR.obj, hrb_x, hrb_y);

   evas_object_smart_callback_call(o, SIG_CHANGED, (void *)priv->size);
}

/* this need for macro EVAS_SMART_SUBCLASS_NEW */
static void
_container_smart_set_user(Evas_Smart_Class *sc)
{
   assert(sc != NULL);

   sc->add = _container_smart_add;
   sc->del = _container_smart_del;
   sc->show = _container_smart_show;
   sc->hide = _container_smart_hide;

   /* clipped smart object has no hook on resizes or calculations */
   sc->resize = _container_smart_resize;
   sc->calculate = _container_smart_calculate;
}

Evas_Object *
container_add(Evas_Object *parent)
{
   Evas *e;
   Evas_Object *obj;

   assert(parent != NULL);

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _container_smart_class_new());

   return obj;
}

Eina_Bool
container_handler_size_set(Evas_Object *obj, int htl_w, int htl_h, int hrb_w, int hrb_h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (htl_w < 5) sd->handler_TL.w = 5;
   else sd->handler_TL.w = htl_w;
   if (htl_h < 5) sd->handler_TL.h = 5;
   else sd->handler_TL.h = htl_h;
   if (hrb_w < 5) sd->handler_BR.w = 5;
   else sd->handler_BR.w = hrb_w;
   if (hrb_h < 5) sd->handler_BR.h = 5;
   else sd->handler_BR.h = hrb_h;

   return true;
}

Eina_Bool
container_handler_size_get(Evas_Object *obj, int *htl_w, int *htl_h, int *hbr_w, int *hbr_h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (htl_w) *htl_w = sd->handler_TL.w;
   if (htl_h) *htl_h = sd->handler_TL.h;
   if (hbr_w) *hbr_w = sd->handler_BR.w;
   if (hbr_h) *hbr_h = sd->handler_BR.h;

   return true;
}

Eina_Bool
container_min_size_set(Evas_Object *obj, int w, int h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (w < 0) sd->con_size_min.w = 0;
   else sd->con_size_min.w = w;
   if (h < 0) sd->con_size_min.h = 0;
   else sd->con_size_min.h = h;

   if (sd->size->w <= w) sd->size->w = w;
   if (sd->size->h <= h) sd->size->h = h;

   evas_object_smart_changed(obj);
   return true;
}

Eina_Bool
container_max_size_set(Evas_Object *obj, int w, int h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (w < 0) sd->con_size_max.w = 0;
   else sd->con_size_max.w = w;
   if (h < 0) sd->con_size_max.h = 0;
   else sd->con_size_max.h = h;

   if ((w != 0) && (sd->size->w > w))
     sd->size->w = w;
   if ((h != 0) && (sd->size->h > h))
     sd->size->h = h;

   evas_object_smart_changed(obj);
   return true;
}

Eina_Bool
container_container_size_set(Evas_Object *obj, int w, int h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (w <= sd->con_size_min.w) sd->size->w = sd->con_size_min.w;
   else
     {
        if ((sd->con_size_max.w != 0) && (w > sd->con_size_max.w))
          sd->size->w = sd->con_size_max.w;
        else sd->size->w = w;
     }

   if (h <= sd->con_size_min.h) sd->size->h = sd->con_size_min.h;
   else
     {
        if ((sd->con_size_max.h != 0) && (h > sd->con_size_max.h))
          sd->size->h = sd->con_size_max.h;
        else sd->size->h = h;
     }

   evas_object_smart_changed(obj);
   return true;
}

Eina_Bool
container_container_size_get(Evas_Object *obj, int *w, int *h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (w) *w = sd->size->w;
   if (h) *h = sd->size->h;

   return true;
}

Eina_Bool
container_style_set(Evas_Object *obj, const char *style)
{
   CONTAINER_DATA_GET(obj, sd);

   assert(style != NULL);

   if (!strcmp(sd->style, style)) return false;
   _style_set(obj, style);

   return true;
}

const char *
container_style_get(Evas_Object *obj)
{
   CONTAINER_DATA_GET(obj, sd);
   return sd->style;
}

Eina_Bool
container_content_set(Evas_Object *obj, Evas_Object *content)
{
   CONTAINER_DATA_GET(obj, sd);

   assert(content != NULL);

   edje_object_part_swallow(sd->container, SWALLOW, content);

   return true;
}

Evas_Object *
container_content_unset(Evas_Object *obj)
{
   Evas_Object *ret;
   CONTAINER_DATA_GET(obj, sd);

   ret = edje_object_part_swallow_get(sd->container, SWALLOW);
   edje_object_part_unswallow(sd->container, ret);
   return ret;
}

Eina_Bool
container_border_hide(Evas_Object *obj)
{
   CONTAINER_DATA_GET(obj, sd);

   if (sd->handler_TL.obj)
     evas_object_hide(sd->handler_TL.obj);
   if (sd->handler_BR.obj)
     evas_object_hide(sd->handler_BR.obj);
   if (sd->container)
     edje_object_signal_emit(sd->container, "container,hide", "eflete");

   return true;
}

Eina_Bool
container_border_show(Evas_Object *obj)
{
   CONTAINER_DATA_GET(obj, sd)

   if (sd->handler_TL.obj)
     evas_object_show(sd->handler_TL.obj);
   if (sd->handler_BR.obj)
     evas_object_show(sd->handler_BR.obj);
   if (sd->container)
     edje_object_signal_emit(sd->container, "container,show", "eflete");

   return true;
}

Eina_Bool
container_padding_size_set(Evas_Object *obj, int tl_w, int tl_h, int rb_w, int rb_h)
{
   CONTAINER_DATA_GET(obj, sd);

   Evas_Coord x, y, w, h;
   Evas_Coord tlw, tlh, rbw, rbh;
   evas_object_geometry_get(obj, &x, &y, &w, &h);

   tlw = sd->pad_left_top.w;
   tlh = sd->pad_left_top.h;
   rbw = sd->pad_right_bottom.w;
   rbh = sd->pad_right_bottom.h;

   if (tl_w < 0) sd->pad_left_top.w = 0;
   else sd->pad_left_top.w = tl_w;
   if (tl_h < 0) sd->pad_left_top.h = 0;
   else sd->pad_left_top.h = tl_h;
   if (rb_w < 0) sd->pad_right_bottom.w = 0;
   else sd->pad_right_bottom.w = rb_w;
   if (rb_h < 0) sd->pad_right_bottom.h = 0;
   else sd->pad_right_bottom.h = rb_h;

   evas_object_resize(obj, w + (rb_w - rbw) + (tl_w - tlw),
                           h + (rb_h - rbh) + (tl_h - tlh));

   evas_object_smart_changed(obj);

   return true;
}

Eina_Bool
container_padding_size_get(Evas_Object *obj, int *tl_w, int *tl_h, int *br_w, int *br_h)
{
   CONTAINER_DATA_GET(obj, sd);

   if (tl_w) *tl_w = sd->pad_left_top.w;
   if (tl_h) *tl_h = sd->pad_left_top.h;
   if (br_w) *br_w = sd->pad_right_bottom.w;
   if (br_h) *br_h = sd->pad_right_bottom.h;

   return true;
}

#undef MY_CLASS_NAME
#undef CONTAINER_DATA_GET
#undef CONTAINER_DATA_GET_OR_RETURN_VAL
#undef H_WIGTH
#undef H_HEIGHT
