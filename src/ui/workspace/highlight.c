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

#include "highlight.h"
#include "alloc.h"
#include "common_macro.h"

#define SIZE 30
#define MINSIZE 120 /* 120 * 0.25 = 30 */
#define COEFF 0.25
#define MAXSIZE 200

#define HIGHLIGHT_DATA_GET(o, ptr) \
   Highlight *ptr = evas_object_smart_data_get(o);

#define HIGHLIGHT_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
   HIGHLIGHT_DATA_GET(o, ptr) \
   if (!ptr) \
     { \
        fprintf(stderr, "No highlight data for object %p (%s)!", \
                o, evas_object_type_get(o)); \
        return val; \
     }

/**
  * TODO: add some comments.
  */
enum _Handler_Corner {
     RB, RT, LB, LT
};

typedef enum _Handler_Corner Handler_Corner;
typedef struct _Highlight Highlight;

struct _Handler {
   Evas_Object *border; /* border layout for showing handler's border image. */
   Handler_Corner descr; /* type of the handler (which corner). */
   Highlight *highlight;
   int x, y, w, h, dx, dy;
};
typedef struct _Handler Handler;

struct _Highlight
{
   Evas_Object_Smart_Clipped_Data data;
   Evas_Object *border; /* border layout for showing handler's border image. */
   Evas_Object *parent; /* need to know parent object for knowing about bounds */
   Evas_Object *bg;     /* background */
   Handler *handler_RB;
   Handler *handler_RT;
   Handler *handler_LB;
   Handler *handler_LT;
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

   if (highlight->handlers_disabled) return;

   evas_object_geometry_get(highlight->border, &x, &y, &w, &h);

   /*
      If width or height of highlight is below MINSIZE, then inner handlers will
      be located outside of the highlighted part.
    */
   if((w < MINSIZE) || (h < MINSIZE)) highlight->outside = true;
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
     }
}
/*
   This function will recalc positions of the handlers based on highlight size.
 */
static void
_handler_pos_recalc(Highlight *highlight)
{
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
     }

   _apply_changes(highlight->handler_RB);
   _apply_changes(highlight->handler_RT);
   _apply_changes(highlight->handler_LB);
   _apply_changes(highlight->handler_LT);
}

static void
_handler_down_cb(void *data,
                 Evas *evas,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Handler *handler = (Handler *)data;
   Highlight *highlight = handler->highlight;

   Evas_Coord curX, curY;
   evas_pointer_output_xy_get(evas, &curX, &curY);

   handler->dx = curX - handler->x;
   handler->dy = curY - handler->y;

   highlight->clicked = true;
}

static void
_handler_move_cb(void *data,
                 Evas *evas,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Handler *handler = (Handler *)data;
   Highlight *highlight = handler->highlight;
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
        events->x = xpos;
        events->y = ypos;
        events->w = width;
        events->h = height;

        evas_object_smart_callback_call(handler->highlight->smart_object,
                                        "hl,resize", events);
        _handler_pos_recalc(handler->highlight);
     }
}

static void
_handler_up_cb(void *data,
               Evas *evas __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Handler *handler = (Handler *)data;
   _handler_size_recalc(handler->highlight);
   _handler_pos_recalc(handler->highlight);

   Evas_Coord x, y, w, h;
   evas_object_geometry_get(handler->highlight->border, &x, &y, &w, &h);
   evas_object_resize(handler->highlight->smart_object, w, h);
   evas_object_move(handler->highlight->smart_object, x, y);

   handler->highlight->clicked = false;
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

   if (highlight->handlers_disabled) return;
   if (highlight->mode == HIGHLIGHT_STATIC_HANDLERS) return;

   evas_object_hide(highlight->handler_RB->border);
   evas_object_hide(highlight->handler_RT->border);
   evas_object_hide(highlight->handler_LB->border);
   evas_object_hide(highlight->handler_LT->border);
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

   if (highlight->handlers_disabled) return;
   if (highlight->mode == HIGHLIGHT_STATIC_HANDLERS) return;

   evas_object_show(highlight->handler_RB->border);
   evas_object_show(highlight->handler_RT->border);
   evas_object_show(highlight->handler_LB->border);
   evas_object_show(highlight->handler_LT->border);
}

Handler *
_handler_object_add(Evas_Object *parent,
                    Highlight *highlight,
                    Handler_Corner descr)
{
   Handler *handler;
   handler = mem_calloc(1, sizeof(Handler));
   Evas_Object *border;

   border = edje_object_add(evas_object_evas_get(parent));
   edje_object_file_set(border, TET_EDJ, "base/groupspace/part/highlight");
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

   evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_IN,
                                  _handler_mouse_in_cb, highlight);
   evas_object_event_callback_add(border, EVAS_CALLBACK_MOUSE_OUT,
                                  _handler_mouse_out_cb, highlight);

   return handler;
}

Evas_Object *
_bg_object_add(Evas_Object *parent, Highlight *highlight)
{
   Evas *e;
   Evas_Object *background = NULL;

   e = evas_object_evas_get(parent);

   background = evas_object_rectangle_add(e);
   edje_object_part_swallow(highlight->border, "bg", background);
   return background;
}

/* create and setup a new example smart object's internals */
static void
_smart_add(Evas_Object *parent)
{
   Evas_Object *border;

   EVAS_SMART_DATA_ALLOC(parent, Highlight)
   _highlight_parent_sc->add(parent);

   border = edje_object_add(evas_object_evas_get(parent));
   edje_object_file_set(border, TET_EDJ, "base/groupspace/part/highlight");

   priv->border = border;
   evas_object_smart_member_add(border, parent);
   priv->bg = _bg_object_add(parent, priv);

   priv->handler_RB = _handler_object_add(parent, priv, RB);
   priv->handler_RT = _handler_object_add(parent, priv, RT);
   priv->handler_LB = _handler_object_add(parent, priv, LB);
   priv->handler_LT = _handler_object_add(parent, priv, LT);
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

   evas_object_smart_member_del(highlight->border);
   evas_object_smart_member_del(highlight->handler_RB->border);
   evas_object_smart_member_del(highlight->handler_RT->border);
   evas_object_smart_member_del(highlight->handler_LB->border);
   evas_object_smart_member_del(highlight->handler_LT->border);

   evas_object_del(highlight->border);
   evas_object_del(highlight->handler_RB->border);
   evas_object_del(highlight->handler_RT->border);
   evas_object_del(highlight->handler_LB->border);
   evas_object_del(highlight->handler_LT->border);

   free(highlight->handler_RB);
   free(highlight->handler_RT);
   free(highlight->handler_LB);
   free(highlight->handler_LT);

   evas_object_del(highlight->bg);

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
     }

   _highlight_parent_sc->hide(obj);
}

static void
_smart_move(Evas_Object *obj,
            Evas_Coord x,
            Evas_Coord y)
{
   HIGHLIGHT_DATA_GET(obj, highlight)
   evas_object_move(highlight->border, x, y);
   if (!highlight->clicked)
     {
        _handler_size_recalc(highlight);
        _handler_pos_recalc(highlight);
     }
}

static void
_smart_resize(Evas_Object *obj,
              Evas_Coord w,
              Evas_Coord h)
{
   HIGHLIGHT_DATA_GET(obj, highlight)
   evas_object_resize(highlight->border, w, h);
   if (!highlight->clicked)
     {
        _handler_size_recalc(highlight);
        _handler_pos_recalc(highlight);
     }
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
}

Evas_Object *
highlight_add(Evas_Object *parent)
{
   if(!parent)
   {
      ERR("parent is NULL");
      return NULL;
   }

   Evas *e;
   Evas_Object *obj;

   if (!parent) return NULL;

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _highlight_smart_class_new());
   HIGHLIGHT_DATA_GET(obj, highlight);
   highlight->smart_object = obj;

   return obj;
}

Eina_Bool
highlight_handler_color_set(Evas_Object *hl,
                            Evas_Coord r,
                            Evas_Coord g,
                            Evas_Coord b,
                            Evas_Coord a)
{
   if (!hl) return false;
   HIGHLIGHT_DATA_GET_OR_RETURN_VAL(hl, highlight, false)
   evas_object_color_set(highlight->handler_LT->border,
                         r * a / 255, g * a / 255, b * a / 255, a);
   evas_object_color_set(highlight->handler_LB->border,
                         r * a / 255, g * a / 255, b * a / 255, a);
   evas_object_color_set(highlight->handler_RT->border,
                         r * a / 255, g * a / 255, b * a / 255, a);
   evas_object_color_set(highlight->handler_RB->border,
                         r * a / 255, g * a / 255, b * a / 255, a);
   return true;
}

Eina_Bool
highlight_border_color_set(Evas_Object *hl,
                           Evas_Coord r,
                           Evas_Coord g,
                           Evas_Coord b,
                           Evas_Coord a)
{
   if (!hl) return false;
   HIGHLIGHT_DATA_GET_OR_RETURN_VAL(hl, highlight, false)
   evas_object_color_set(highlight->border,
                         r * a / 255, g * a / 255, b * a / 255, a);
   return true;
}

Eina_Bool
highlight_bg_color_set(Evas_Object *hl,
                       Evas_Coord r,
                       Evas_Coord g,
                       Evas_Coord b,
                       Evas_Coord a)
{
   if (!hl) return false;
   HIGHLIGHT_DATA_GET_OR_RETURN_VAL(hl, highlight, false)
   evas_object_color_set(highlight->bg,
                         r * a / 255, g * a / 255, b * a / 255, a);
   return true;
}

Eina_Bool
highlight_handler_disabled_set(Evas_Object *hl, Eina_Bool disabled)
{
   if (!hl) return false;
   HIGHLIGHT_DATA_GET_OR_RETURN_VAL(hl, highlight, false)
   if (highlight->handlers_disabled == disabled) return false;
   highlight->handlers_disabled = disabled;
   if (disabled)
     {
        evas_object_hide(highlight->handler_RB->border);
        evas_object_hide(highlight->handler_RT->border);
        evas_object_hide(highlight->handler_LB->border);
        evas_object_hide(highlight->handler_LT->border);
     }
   else
     {
        evas_object_show(highlight->handler_RB->border);
        evas_object_show(highlight->handler_RT->border);
        evas_object_show(highlight->handler_LB->border);
        evas_object_show(highlight->handler_LT->border);
     }
   return true;
}

Eina_Bool
highlight_handler_mode_set(Evas_Object *hl, Highlight_Mode mode)
{
   if (!hl) return false;
   HIGHLIGHT_DATA_GET_OR_RETURN_VAL(hl, highlight, false)
   highlight->mode = mode;
   return true;
}

#undef SIZE
#undef MINSIZE
#undef MAXSIZE
#undef COEFF
#undef HIGHLIGHT_DATA_GET
#undef HIGHLIGHT_DATA_GET_OR_RETURN_VAL
