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

#include "container.h"

#define MY_CLASS_NAME "Container"

#define H_WIGTH (sd->handler_TL.w + sd->handler_BR.w)
#define H_HEIGHT (sd->handler_TL.h + sd->handler_BR.h)

typedef struct _Container_Smart_Data Container_Smart_Data;

static const char SIG_CHANGED[] = "container,changed";

static const char TEXT_TOOLTIP[] = "eflete.size.tooltip";
static const char SWALLOW[] = "eflete.swallow.container";

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {SIG_CHANGED, "(iiii)"},
   {NULL, NULL}
};

struct _Geom {
   int x, y, w, h;
};
typedef struct _Geom Container_Geom;

struct _Container_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas *e;
   Evas_Object *obj;
   Evas_Object *container;
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
   Container_Geom *con_current_size;
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
   const char *style;
   Evas_Coord downx;
   Evas_Coord downy;
   struct {
      Evas_Object *obj;
      Evas_Coord x, y, w, h;
   } confine;
};

#define CONTAINER_DATA_GET(o, ptr) \
   Container_Smart_Data *ptr = evas_object_smart_data_get(o);

#define CONTAINER_DATA_GET_OR_RETURN_VAL(o, ptr, val)                   \
   CONTAINER_DATA_GET(o, ptr)                                           \
   if (!ptr)                                                            \
     {                                                                  \
        ERR("No container data for object %p (%s)!",                    \
            o, evas_object_type_get(o));                                \
        return val;                                                     \
     }

/* here we create and define some functions for creating our container smart
 * object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _container,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static Eina_Bool
_user_size_calc(Evas_Object *o)
{
   int nw, nh;
   CONTAINER_DATA_GET_OR_RETURN_VAL(o, sd, false)

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
   Evas_Coord x, y, w, h;
   Evas_Coord lw, lh;
   Evas_Coord nx, ny, nw, nh;
   Evas_Coord dx, dy;
   Evas_Event_Mouse_Move *ev = event_info;
   Evas_Object *o = data;

   CONTAINER_DATA_GET(o, sd)
   evas_object_geometry_get(o, &x, &y, &w, &h);
   evas_object_geometry_get(sd->container, NULL, NULL, &lw, &lh);
   dx = (ev->cur.canvas.x - sd->downx);
   dy = (ev->cur.canvas.y - sd->downy);

   if (!sd->handler_TL_pressed) return;
        /* calc x and wigth */

   nw = w - dx;
   /*TODO: need do refactoring here */
   if (nw <= sd->con_size_min.w + H_WIGTH)
     {
        nw = sd->con_size_min.w + H_WIGTH;
        nx = x + (w - nw);
     }
   else
     {
        if ((sd->con_size_max.w != -1)
            && (nw >= sd->con_size_max.w + H_WIGTH))
          {
             nw = sd->con_size_max.w + H_WIGTH;
             nx = x + (w - nw);
          }
        else nx = x + dx;
        if ((sd->confine.obj) && (nx < sd->confine.x))
          {
             if (nx != sd->confine.x)
               {
                 nw = w + (x - sd->confine.x);
                 nx = sd->confine.x;
               }
          }
     }

   /* calc y and height */
   nh = h - dy;
   if (nh <= sd->con_size_min.h + H_HEIGHT)
     {
        nh = sd->con_size_min.h + H_HEIGHT;
        ny = y + (h - nh);
     }
   else
     {
        if ((sd->con_size_max.h != -1)
            && (nh >= sd->con_size_max.h + H_HEIGHT))
          {
             nh = sd->con_size_max.h + H_HEIGHT;
             ny = y + (h - nh);
          }
        else ny = y + dy;
        if ((sd->confine.obj) && (ny < sd->confine.y))
          {
             if (ny != sd->confine.y)
               {
                  nh = h + (y - sd->confine.y);
                  ny = sd->confine.y;
               }
          }
     }

   evas_object_resize(o, nw, nh);
   evas_object_move(o, nx, ny);

   evas_object_smart_changed(o);

   sd->downx = ev->cur.canvas.x;
   sd->downy = ev->cur.canvas.y;
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
   /*TODO: need do refactoring here */
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
   if (sd->confine.obj)
     {
        if ((sd->confine.x + sd->confine.w) < (x + nw))
          nw = sd->confine.w - (x - sd->confine.x);
        if ((sd->confine.y + sd->confine.h) < (y + nh))
          nh = sd->confine.h - (y - sd->confine.y);
     }
   evas_object_resize(o, nw, nh);

   evas_object_smart_changed(o);

   sd->downx = ev->cur.canvas.x;
   sd->downy = ev->cur.canvas.y;
}

static void
_style_set(Evas_Object *o, const char *style)
{
   char group[512]; /* FIXME: change 512 to BUFF_MAX */
   CONTAINER_DATA_GET(o, sd)

   #define GROUP_NAME(item, style) \
      sprintf(group, "eflete/container/%s/%s", item, style);

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

   if (sd->style) free((void *)sd->style);
   sd->style = strdup(style);

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

   priv->handler_BR.obj = edje_object_add(priv->e);
   priv->handler_BR.w = priv->handler_BR.h = 5;
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_DOWN,
                                  _mouse_down_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_UP,
                                  _mouse_up_hRB_cb, o);
   evas_object_event_callback_add(priv->handler_BR.obj, EVAS_CALLBACK_MOUSE_MOVE,
                                  _mouse_move_hBR_cb, o);

   priv->obj = o;
   priv->con_size_min.w = 0;
   priv->con_size_min.h = 0;
   priv->con_size_max.w = -1;
   priv->con_size_max.h = -1;
   priv->con_current_size = (Container_Geom *)mem_calloc(1, sizeof(Container_Geom));
   priv->handler_TL_pressed = false;
   priv->handler_BR_pressed = false;
   priv->confine.obj = NULL;

   evas_object_smart_member_add(priv->container, o);
   evas_object_smart_member_add(priv->handler_TL.obj, o);
   evas_object_smart_member_add(priv->handler_BR.obj, o);

   _style_set(o, "default");
}

static void
_container_smart_del(Evas_Object *o)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_smart_member_del(sd->container);
   evas_object_smart_member_del(sd->handler_TL.obj);
   evas_object_smart_member_del(sd->handler_BR.obj);

   _container_parent_sc->del(o);
}

static void
_container_smart_show(Evas_Object *o)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   if (sd->handler_TL.obj) evas_object_show(sd->handler_TL.obj);
   if (sd->handler_BR.obj) evas_object_show(sd->handler_BR.obj);

   evas_object_show(sd->container);
   _container_parent_sc->show(o);
}

static void
_container_smart_hide(Evas_Object *o)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   if (sd->handler_TL.obj) evas_object_hide(sd->handler_TL.obj);
   if (sd->handler_BR.obj) evas_object_hide(sd->handler_BR.obj);

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

   CONTAINER_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)
   if (!sd->confine.obj)
     evas_object_size_hint_min_set(o, w, h);

   evas_object_smart_changed(o);
}

static void
_container_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord cw, ch;
   int htl_w, htl_h;
   int hrb_w, hrb_h;
   char buff[16];

   CONTAINER_DATA_GET_OR_RETURN_VAL(o, priv, RETURN_VOID)
   evas_object_geometry_get(o, &x, &y, &w, &h);
   htl_w = priv->handler_TL.w; htl_h = priv->handler_TL.h;
   hrb_w = priv->handler_BR.w; hrb_h = priv->handler_BR.h;

   cw = w - (htl_w + hrb_w);
   ch = h - (htl_h + hrb_h);
   evas_object_resize(priv->container, cw, ch);
   evas_object_move(priv->container, x + htl_w, y + htl_h);
   priv->con_current_size->x = x + htl_w;
   priv->con_current_size->y = y + htl_h;
   priv->con_current_size->w = cw;
   priv->con_current_size->h = ch;
   sprintf(buff, "%i %i", priv->con_current_size->w, priv->con_current_size->h);
   edje_object_part_text_set(priv->container, TEXT_TOOLTIP, buff);

   evas_object_resize(priv->handler_TL.obj, htl_w, htl_h);
   evas_object_move(priv->handler_TL.obj, x, y);

   evas_object_resize(priv->handler_BR.obj, hrb_w, hrb_h);
   evas_object_move(priv->handler_BR.obj, x + w - hrb_w, y + h - hrb_h);

   evas_object_smart_callback_call(o, SIG_CHANGED, (void *)priv->con_current_size);
}

/* this need for macro EVAS_SMART_SUBCLASS_NEW */
static void
_container_smart_set_user(Evas_Smart_Class *sc)
{
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

   if (!parent) return NULL;

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _container_smart_class_new());

   return obj;
}

Eina_Bool
container_handler_size_set(Evas_Object *obj, int htl_w, int htl_h, int hrb_w, int hrb_h)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

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
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (htl_w) *htl_w = sd->handler_TL.w;
   if (htl_h) *htl_h = sd->handler_TL.h;
   if (hbr_w) *hbr_w = sd->handler_BR.w;
   if (hbr_h) *hbr_h = sd->handler_BR.h;

   return true;
}

Eina_Bool
container_min_size_set(Evas_Object *obj, int w, int h)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w < 0) sd->con_size_min.w = 0;
   else sd->con_size_min.w = w;
   if (h < 0) sd->con_size_min.h = 0;
   else sd->con_size_min.h = h;

   return true;
}

Eina_Bool
container_max_size_set(Evas_Object *obj, int w, int h)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w < 0) sd->con_size_max.w = -1;
   else sd->con_size_max.w = w;
   if (h < 0) sd->con_size_max.h = -1;
   else sd->con_size_max.h = h;

   return true;
}

Eina_Bool
container_container_size_set(Evas_Object *obj, int w, int h)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

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
container_container_size_get(Evas_Object *obj, int *w, int *h)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (w) *w = sd->con_current_size->w;
   if (h) *h = sd->con_current_size->h;

   return true;
}

Eina_Bool
container_style_set(Evas_Object *obj, const char *style)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!style || (!strcmp(sd->style, style))) return false;
   _style_set(obj, style);

   return true;
}

const char *
container_style_get(Evas_Object *obj)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);
   return sd->style;
}

Eina_Bool
container_content_set(Evas_Object *obj, Evas_Object *content)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!content) return false;
   edje_object_part_swallow(sd->container, SWALLOW, content);

   return true;
}

Evas_Object *
container_content_unset(Evas_Object *obj)
{
   Evas_Object *ret;
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   ret = edje_object_part_swallow_get(sd->container, SWALLOW);
   edje_object_part_unswallow(sd->container, ret);
   return ret;
}

static void
_confine_changed(void *data,
                 Evas *e __UNUSED__,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   Container_Smart_Data *sd = (Container_Smart_Data *)data;

   evas_object_geometry_get(obj,
                            &sd->confine.x,
                            &sd->confine.y,
                            &sd->confine.w,
                            &sd->confine.h);

}

Eina_Bool
container_confine_set(Evas_Object *obj, Evas_Object *confine)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!confine) return false;

   sd->confine.obj = confine;
   evas_object_geometry_get(confine,
                            &sd->confine.x,
                            &sd->confine.y,
                            &sd->confine.w,
                            &sd->confine.h);
   evas_object_event_callback_add(confine, EVAS_CALLBACK_MOVE,
                                  _confine_changed, sd);
   evas_object_event_callback_add(confine, EVAS_CALLBACK_RESIZE,
                                  _confine_changed, sd);

   return true;
}

Eina_Bool
container_confine_unset(Evas_Object *obj)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (!sd->confine.obj) return false;

   evas_object_event_callback_del_full(sd->confine.obj, EVAS_CALLBACK_MOVE,
                                       _confine_changed, sd);
   evas_object_event_callback_del_full(sd->confine.obj, EVAS_CALLBACK_RESIZE,
                                       _confine_changed, sd);
   sd->confine.obj = NULL;

   return true;
}

Eina_Bool
container_border_hide(Evas_Object *obj)
{
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false);

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
   CONTAINER_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   if (sd->handler_TL.obj)
     evas_object_show(sd->handler_TL.obj);
   if (sd->handler_BR.obj)
     evas_object_show(sd->handler_BR.obj);
   if (sd->container)
     edje_object_signal_emit(sd->container, "container,show", "eflete");

   return true;
}

#undef MY_CLASS_NAME
#undef CONTAINER_DATA_GET
#undef CONTAINER_DATA_GET_OR_RETURN_VAL
#undef H_WIGTH
#undef H_HEIGHT
