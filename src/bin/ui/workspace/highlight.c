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

#include "highlight.h"
#include "cursor.h"

#define SIZE 30
#define MINSIZE 120 /* 120 * 0.25 = 30 */
#define COEFF 0.25
#define MAXSIZE 200

#define HIGHLIGHT_DATA_GET(o, ptr) \
   assert(o != NULL); \
   Highlight *ptr = evas_object_smart_data_get(o); \
   assert(ptr != NULL);

#define COLOR_CHECK \
   if ((r < 0) || (r > 255)) return false; \
   if ((g < 0) || (g > 255)) return false; \
   if ((b < 0) || (b > 255)) return false; \
   if ((a < 0) || (a > 255)) return false; \

typedef struct _Highlight Highlight;

struct _Handler {
   Evas_Object *border; /* border layout for showing handler's border image. */
   Handler_Type descr; /* type of the handler (which corner). */
   Highlight *highlight;
   int x, y, w, h, dx, dy;
};
typedef struct _Handler Handler;

struct _Highlight
{
   Evas_Object_Smart_Clipped_Data data;
   Evas_Object *border; /* border layout for showing handler's border image. */
   Evas_Object *parent; /* need to know parent object for knowing about bounds */
   Evas_Object *object; /* object that is being highlighted. */
   Handler *handler_RB;
   Handler *handler_RT;
   Handler *handler_LB;
   Handler *handler_LT;
   Handler *handler_MIDDLE;
   Eina_Bool middle_show; /* its TRUE, when middle is have to be shown. */
   Eina_Bool clicked; /* its TRUE, when user click his mouse on any handler. */
   Eina_Bool outside; /* its TRUE, when highlight in small mode (handlers are
                         outside of the highlighted part) */
   Eina_Bool handlers_disabled; /* its TRUE, when handlers for highlight are disabled*/
   Eina_Bool visible; /* its TRUE, when highlight visible*/
   Evas_Object *smart_object; /* smart object that contain all information
                                 mentioned above. */
   Highlight_Events *events;
   Highlight_Mode mode;
};

/* here we create and define some functions for creating our highlight smart
   object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW("highlight", _highlight,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, NULL);

/*
   This function will apply to handler all changes that was made previously.
 */
static void
_apply_changes(Handler *handler)
{
   assert(handler != NULL);

   evas_object_resize(handler->border, handler->w, handler->h);
   evas_object_move(handler->border, handler->x, handler->y);
}

/*
   This function will recalc handler sizes.

   It depends on the handler type.
   If they are inner handlers (inside of a highlighted object) then they will be
   having variable size. (Coefficient defined in COEFF).
   If those handlers are outside of the highlight, then they will be having fixed
   size, defined in SIZE.
 */
static void
_handler_size_recalc(Highlight *highlight)
{
   int x, y, w, h;

   assert(highlight != NULL);

   if (highlight->handlers_disabled) return;

   evas_object_geometry_get(highlight->border, &x, &y, &w, &h);

   /*
      If width or height of highlight is below MINSIZE, then inner handlers will
      be located outside of the highlighted part.
    */
   if ((w < MINSIZE) || (h < MINSIZE)) highlight->outside = true;
   else highlight->outside = false;

   if ((highlight->outside) || (highlight->mode == HIGHLIGHT_STATIC_HANDLERS))
     {
        (highlight->handler_RB)->w = SIZE;
        (highlight->handler_RB)->h = SIZE;
        (highlight->handler_RT)->w = SIZE;
        (highlight->handler_RT)->h = SIZE;
        (highlight->handler_LB)->w = SIZE;
        (highlight->handler_LB)->h = SIZE;
        (highlight->handler_LT)->w = SIZE;
        (highlight->handler_LT)->h = SIZE;
        (highlight->handler_MIDDLE)->w = w;
        (highlight->handler_MIDDLE)->h = h;
     }
   else
     {
        /*
           If size of handler is too big, then we just set fixed maximum size.
         */
        int newsizeW = (COEFF * w > MAXSIZE)  ? MAXSIZE : COEFF * w;
        int newsizeH = (COEFF * h > MAXSIZE)  ? MAXSIZE : COEFF * h;
        (highlight->handler_RB)->w = newsizeW;
        (highlight->handler_RB)->h = newsizeH;
        (highlight->handler_RT)->w = newsizeW;
        (highlight->handler_RT)->h = newsizeH;
        (highlight->handler_LB)->w = newsizeW;
        (highlight->handler_LB)->h = newsizeH;
        (highlight->handler_LT)->w = newsizeW;
        (highlight->handler_LT)->h = newsizeH;
        (highlight->handler_MIDDLE)->w = w - newsizeW * 2;
        (highlight->handler_MIDDLE)->h = h - newsizeH * 2;
     }
}
/*
   This function will recalc positions of the handlers based on highlight size.
 */
static void
_handler_pos_recalc(Highlight *highlight)
{
   assert(highlight != NULL);

   if (highlight->handlers_disabled) return;

   int x, y, w, h;
   evas_object_geometry_get(highlight->border, &x, &y, &w, &h);

   /*
      If width or height of highlight is below MINSIZE, then inner handlers will
      be located outside of the highlighted part.
    */
   if (!highlight->clicked)
     {
        if ((w < MINSIZE) || (h < MINSIZE)) highlight->outside = true;
        else highlight->outside = false;
     }

   if ((highlight->outside) || (highlight->mode == HIGHLIGHT_STATIC_HANDLERS))
     {
        highlight->handler_RB->x = w + x;
        highlight->handler_RB->y = h + y;

        highlight->handler_RT->x = w + x;
        highlight->handler_RT->y = y - highlight->handler_RT->h;

        highlight->handler_LB->x = x - highlight->handler_LB->w;
        highlight->handler_LB->y = h + y;

        highlight->handler_LT->x = x - highlight->handler_LB->w;
        highlight->handler_LT->y = y - highlight->handler_RT->h;

        highlight->handler_MIDDLE->x = x;
        highlight->handler_MIDDLE->y = y;
     }
   else
     {
        highlight->handler_RB->x = w + x - highlight->handler_RB->w;
        highlight->handler_RB->y = h + y - highlight->handler_RB->h;

        highlight->handler_RT->x = w + x - highlight->handler_RT->w;
        highlight->handler_RT->y = y;

        highlight->handler_LB->x = x;
        highlight->handler_LB->y = h + y - highlight->handler_LB->h;

        highlight->handler_LT->x = x;
        highlight->handler_LT->y = y;

        highlight->handler_MIDDLE->x = x + highlight->handler_LT->w;
        highlight->handler_MIDDLE->y = y + highlight->handler_LT->h;
     }

   _apply_changes(highlight->handler_RB);
   _apply_changes(highlight->handler_RT);
   _apply_changes(highlight->handler_LB);
   _apply_changes(highlight->handler_LT);
   _apply_changes(highlight->handler_MIDDLE);
}

static void
_handler_down_cb(void *data,
                 Evas *evas,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Handler *handler = (Handler *)data;

   Evas_Event_Mouse_Down *event = (Evas_Event_Mouse_Down *) event_info;
   if (event->button != 1) return;

   assert(handler != NULL);

   Highlight *highlight = handler->highlight;

   assert(highlight != NULL);

   Evas_Coord curX, curY;
   evas_pointer_output_xy_get(evas, &curX, &curY);

   handler->dx = curX - handler->x;
   handler->dy = curY - handler->y;

   highlight->clicked = true;
   evas_object_smart_callback_call(highlight->smart_object,
                                   "hl,drag,start", highlight->events);
}

static void
_handler_move_cb(void *data,
                 Evas *evas,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Handler *handler = (Handler *)data;

   assert(handler != NULL);

   Highlight *highlight = handler->highlight;

   assert(highlight != NULL);

   Highlight_Events *events = handler->highlight->events;
   if (highlight->handlers_disabled) return;

   if (highlight->clicked)
     {
        Evas_Coord x, y, w, h;
        Evas_Coord curX, curY;

        evas_pointer_output_xy_get(evas, &curX, &curY);
        evas_object_geometry_get(highlight->border, &x, &y, &w, &h);

        handler->x = curX - handler->dx;
        handler->y = curY - handler->dy;

        /*
           Resizing by moving handler and changing highlight (outside and inside)
         */
        int width = w;
        int height = h;
        int xpos = x;
        int ypos = y;

        /*
           Calculating position and size of highlight.
           The calculation differs and depends on type of handler is being dragged.
           How to fix this if-else insane?
         */
        if (handler->descr == RB)
          {
             if ((!highlight->outside) && (highlight->mode != HIGHLIGHT_STATIC_HANDLERS))
               {
                  width = handler->x - x + handler->w;
                  height = handler->y - y + handler->h;
               }
             else
               {
                  width = handler->x - x;
                  height = handler->y - y;
               }
             if (width < 1)
               {
                  width = 1;
                  handler->x = x + 1;
               }
             if (height < 1)
               {
                  height = 1;
                  handler->y = y + 1;
               }
          }
        else if (handler->descr == RT)
          {
             if ((!highlight->outside) && (highlight->mode != HIGHLIGHT_STATIC_HANDLERS))
               {
                  width = handler->x - x + handler->w;
                  height = h - (handler->y - y);
                  xpos = x;
                  ypos = handler->y;
               }
             else
               {
                  width = handler->x - x;
                  height = h - (handler->y - y) - handler->h;
                  xpos = x;
                  ypos = handler->y + handler->h;
               }
             if (width < 1)
               {
                  width = 1;
                  handler->x = x + 1;
               }
             if (height < 1)
               {
                  height = 1;
                  handler->y = y - 1 - handler->h;
                  ypos = y + h - 1;
               }
          }
        else if (handler->descr == LB)
          {
             if ((!highlight->outside) && (highlight->mode != HIGHLIGHT_STATIC_HANDLERS))
               {
                  width = w - (handler->x - x);
                  height = handler->y - y + handler->h;
                  xpos = handler->x;
                  ypos = y;
               }
             else
               {
                  width = w - (handler->x - x) - handler->w;
                  height = handler->y - y;
                  xpos = handler->x + handler->w;
                  ypos = y;
               }
             if (width < 1)
               {
                  width = 1;
                  handler->x = x - 1 - handler->w;
                  xpos = x + w - 1;
               }
             if (height < 1)
               {
                  height = 1;
                  handler->y = y + 1;
               }
          }
        else if (handler->descr == LT)
          {
             if ((!highlight->outside) && (highlight->mode != HIGHLIGHT_STATIC_HANDLERS))
               {
                  width = w - (handler->x - x);
                  height = h - (handler->y - y);
                  xpos = handler->x;
                  ypos = handler->y;
               }
             else
               {
                  width = w - (handler->x - x) - handler->w;
                  height = h - (handler->y - y) - handler->h;
                  xpos = handler->x + handler->w;
                  ypos = handler->y + handler->h;
               }
             if (width < 1)
               {
                  width = 1;
                  handler->x = x - handler->w + 1;
                  xpos = x + w - 1;
               }
             if (height < 1)
               {
                  height = 1;
                  handler->y = y - handler->h + 1;
                  ypos = y + h - 1;
               }
          }
        else if (handler->descr == MIDDLE)
          {
             if ((!highlight->outside) && (highlight->mode != HIGHLIGHT_STATIC_HANDLERS))
               {
                  xpos = handler->x - highlight->handler_LT->w;
                  ypos = handler->y - highlight->handler_LT->h;
               }
             else
               {
                  xpos = handler->x;
                  ypos = handler->y;
               }
          }
        events->x = xpos;
        events->y = ypos;
        events->w = width;
        events->h = height;
        events->descr = handler->descr;

        evas_object_smart_changed(handler->highlight->smart_object);
     }
}

static void
_handler_up_cb(void *data,
               Evas *evas __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Handler *handler = (Handler *)data;

   Evas_Event_Mouse_Down *event = (Evas_Event_Mouse_Down *) event_info;
   if (event->button != 1) return;

   assert(handler != NULL);

   _handler_size_recalc(handler->highlight);
   _handler_pos_recalc(handler->highlight);

   Evas_Coord x, y, w, h;
   evas_object_geometry_get(handler->highlight->border, &x, &y, &w, &h);
   evas_object_resize(handler->highlight->smart_object, w, h);
   evas_object_move(handler->highlight->smart_object, x, y);

   handler->highlight->clicked = false;

   evas_object_smart_callback_call(handler->highlight->smart_object,
                                   "hl,drag,stop", handler->highlight->events);
}

/*
   This callback will hide all handlers instead of pointed
   one.
 */
static void
_handler_mouse_in_cb(void *data,
                    Evas *evas __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Highlight *highlight = (Highlight *)data;

   assert(highlight != NULL);

   if (highlight->handlers_disabled) return;
   if (highlight->mode == HIGHLIGHT_STATIC_HANDLERS) return;

   evas_object_hide(highlight->handler_RB->border);
   evas_object_hide(highlight->handler_RT->border);
   evas_object_hide(highlight->handler_LB->border);
   evas_object_hide(highlight->handler_LT->border);
   evas_object_hide(highlight->handler_MIDDLE->border);
   evas_object_show(obj);
}

/*
   This function is callback called when mouse iis living border image of a border.
   This function will show all corner handler's.
 */
static void
_handler_mouse_out_cb(void *data,
                      Evas *evas __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Highlight *highlight = (Highlight *)data;

   assert(highlight != NULL);

   if (highlight->handlers_disabled) return;
   if (highlight->mode == HIGHLIGHT_STATIC_HANDLERS) return;

   evas_object_show(highlight->handler_RB->border);
   evas_object_show(highlight->handler_RT->border);
   evas_object_show(highlight->handler_LB->border);
   evas_object_show(highlight->handler_LT->border);
   if (highlight->middle_show)
     evas_object_show(highlight->handler_MIDDLE->border);
}



static Handler *
_handler_object_add(Evas_Object *parent,
                    Highlight *highlight,
                    Handler_Type descr,
                    const char *style)
{
   Handler *handler;
   handler = mem_calloc(1, sizeof(Handler));
   Evas_Object *border;

   assert(parent != NULL);
   assert(highlight != NULL);
   assert(style != NULL);

   border = edje_object_add(evas_object_evas_get(parent));
   if (!edje_object_file_set(border, ap.path.layout_edj, style))
     {
        ERR("Could not load style for handler's border!");
        abort();
     }
   evas_object_smart_member_add(border, parent);

   handler->descr = descr;
   handler->border = border;
   handler->highlight = highlight;

   evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_DOWN,
                                  _handler_down_cb, handler);
   evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_MOVE,
                                  _handler_move_cb, handler);
   evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_UP,
                                  _handler_up_cb, handler);

   if (descr != MIDDLE)
     {
        evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_IN,
                                       _handler_mouse_in_cb, highlight);
        evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_OUT,
                                       _handler_mouse_out_cb, highlight);
     }
   switch (descr)
     {
      case RB:
         cursor_type_set(border, CURSOR_BOTTOM_RIGHT_CORNER);
         break;
      case RT:
         cursor_type_set(border, CURSOR_TOP_RIGHT_CORNER);
         break;
      case LB:
         cursor_type_set(border, CURSOR_BOTTOM_LEFT_CORNER);
         break;
      case LT:
         cursor_type_set(border, CURSOR_TOP_LEFT_CORNER);
         break;
      case MIDDLE:
         cursor_type_set(border, CURSOR_FLEUR);
         break;
      default:
         ERR("Wrong handler type");
         abort();
         break;
     }

   return handler;
}

/* create and setup a new example smart object's internals */
static void
_smart_calc(Evas_Object *obj)
{
   Evas_Coord x, y, w, h;
   HIGHLIGHT_DATA_GET(obj, highlight)

   evas_object_geometry_get(obj, &x, &y, &w, &h);
   evas_object_resize(highlight->border, w, h);
   evas_object_move(highlight->border, x, y);

   if (!highlight->clicked)
      _handler_size_recalc(highlight);
   _handler_pos_recalc(highlight);

   if (highlight->clicked)
     evas_object_smart_callback_call(highlight->smart_object,
                                     "hl,changed", highlight->events);
}

/* create and setup a new example smart object's internals */
static void
_smart_add(Evas_Object *parent)
{
   Evas_Object *border;

   assert(parent != NULL);

   EVAS_SMART_DATA_ALLOC(parent, Highlight)
   _highlight_parent_sc->add(parent);

   border = edje_object_add(evas_object_evas_get(parent));
   if (!edje_object_file_set(border, ap.path.layout_edj, "eflete/highlight/border/default"))
     {
        ERR("Could not load style for main border!");
        abort();
     }
   evas_object_repeat_events_set(border, true);

   priv->border = border;
   evas_object_smart_member_add(border, parent);

   priv->handler_RB = _handler_object_add(parent, priv, RB,
                                          "eflete/highlight/handler_RB/default");
   priv->handler_RT = _handler_object_add(parent, priv, RT,
                                          "eflete/highlight/handler_RT/default");
   priv->handler_LB = _handler_object_add(parent, priv, LB,
                                          "eflete/highlight/handler_LB/default");
   priv->handler_LT = _handler_object_add(parent, priv, LT,
                                          "eflete/highlight/handler_LT/default");
   priv->handler_MIDDLE = _handler_object_add(parent, priv, MIDDLE,
                                              "eflete/highlight/handler_MIDDLE/default");

   priv->outside = false;
   priv->clicked = false;
   priv->events = (Highlight_Events *)mem_calloc(1, sizeof(Highlight_Events));
   priv->handlers_disabled = false;
   priv->visible = true;
   priv->mode = HIGHLIGHT_DYNAMIC_HANDLERS;
}

static void
_smart_del(Evas_Object *obj)
{
   HIGHLIGHT_DATA_GET(obj, highlight)

   free(highlight->handler_RB);
   free(highlight->handler_RT);
   free(highlight->handler_LB);
   free(highlight->handler_LT);
   free(highlight->handler_MIDDLE);
   free(highlight->events);

  _highlight_parent_sc->del(obj);
}

static void
_smart_show(Evas_Object *obj)
{
   HIGHLIGHT_DATA_GET(obj, highlight)

   evas_object_show(highlight->border);
   highlight->visible = true;
   if (!highlight->handlers_disabled)
     {
        evas_object_show(highlight->handler_RB->border);
        evas_object_show(highlight->handler_RT->border);
        evas_object_show(highlight->handler_LB->border);
        evas_object_show(highlight->handler_LT->border);
        if (highlight->middle_show)
          evas_object_show(highlight->handler_MIDDLE->border);
     }

   _highlight_parent_sc->show(obj);
}

static void
_smart_hide(Evas_Object *obj)
{
   HIGHLIGHT_DATA_GET(obj, highlight)

   evas_object_hide(highlight->border);
   highlight->visible = false;
   if (!highlight->handlers_disabled)
     {
        evas_object_hide(highlight->handler_RB->border);
        evas_object_hide(highlight->handler_RT->border);
        evas_object_hide(highlight->handler_LB->border);
        evas_object_hide(highlight->handler_LT->border);
        evas_object_hide(highlight->handler_MIDDLE->border);
     }

   _highlight_parent_sc->hide(obj);
}

static void
_smart_move(Evas_Object *obj,
            Evas_Coord x,
            Evas_Coord y)
{
   Evas_Coord ox, oy;
   evas_object_geometry_get(obj, &ox, &oy, NULL, NULL);
   if ((ox == x) && (oy == y)) return;
   evas_object_smart_changed(obj);
}

static void
_smart_resize(Evas_Object *obj,
              Evas_Coord w,
              Evas_Coord h)
{
   Evas_Coord ow, oh;
   evas_object_geometry_get(obj, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;
   evas_object_smart_changed(obj);
}

static void
_smart_color_set(Evas_Object *o, int r, int g, int b, int a)
{
   HIGHLIGHT_DATA_GET(o, highlight)

   evas_object_color_set(highlight->border, r, g, b, a);
   evas_object_color_set(highlight->handler_RB->border, r, g, b, a);
   evas_object_color_set(highlight->handler_RT->border, r, g, b, a);
   evas_object_color_set(highlight->handler_LB->border, r, g, b, a);
   evas_object_color_set(highlight->handler_LT->border, r, g, b, a);
   evas_object_color_set(highlight->handler_MIDDLE->border, r, g, b, a);
}

static void
_highlight_smart_set_user(Evas_Smart_Class *sc)
{
   sc->add       = _smart_add;
   sc->show      = _smart_show;
   sc->hide      = _smart_hide;
   sc->del       = _smart_del;
   sc->resize    = _smart_resize;
   sc->move      = _smart_move;
   sc->calculate = _smart_calc;
   sc->color_set = _smart_color_set;
}

Evas_Object *
highlight_add(Evas_Object *parent)
{
   Evas *e;
   Evas_Object *obj;

   assert(parent != NULL);

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _highlight_smart_class_new());
   HIGHLIGHT_DATA_GET(obj, highlight);
   highlight->smart_object = obj;

   return obj;
}

void
highlight_handler_align_visible_set(Evas_Object *hl, Eina_Bool visible)
{
   HIGHLIGHT_DATA_GET(hl, highlight)

   if (highlight->handlers_disabled) return;

   highlight->middle_show = visible;
   if (visible)
     evas_object_show(highlight->handler_MIDDLE->border);
   else
     evas_object_hide(highlight->handler_MIDDLE->border);
}

Eina_Bool
highlight_handler_align_visible_get(Evas_Object *hl)
{
   HIGHLIGHT_DATA_GET(hl, highlight)
   return highlight->middle_show;
}

Eina_Bool
highlight_handler_disabled_set(Evas_Object *hl, Eina_Bool disabled)
{
   HIGHLIGHT_DATA_GET(hl, highlight)
   if (highlight->handlers_disabled == disabled) return true;
   highlight->handlers_disabled = disabled;
   if (disabled)
     {
        evas_object_hide(highlight->handler_RB->border);
        evas_object_hide(highlight->handler_RT->border);
        evas_object_hide(highlight->handler_LB->border);
        evas_object_hide(highlight->handler_LT->border);
        evas_object_hide(highlight->handler_MIDDLE->border);
     }
   else
     {
        evas_object_show(highlight->handler_RB->border);
        evas_object_show(highlight->handler_RT->border);
        evas_object_show(highlight->handler_LB->border);
        evas_object_show(highlight->handler_LT->border);
        if (highlight->middle_show)
          evas_object_show(highlight->handler_MIDDLE->border);
     }
   return true;
}

Eina_Bool
highlight_handler_mode_set(Evas_Object *hl, Highlight_Mode mode)
{
   HIGHLIGHT_DATA_GET(hl, highlight)
   highlight->mode = mode;
   return true;
}
