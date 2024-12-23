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

#include "groupview.h"
#include "groupview_private.h"
#include "project_manager2.h"

#define MY_CLASS_NAME "Groupview"

/* here we create and define some functions for creating our groupview smart
  object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _groupview,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

/******************************************************************************/
/*                            HIGHLIGHT CALLBACKS                             */
/******************************************************************************/
static Groupview_HL_Event *
_grouopview_hl_event_new(Highlight_Events *event, Groupview_Smart_Data *sd)
{
   Groupview_HL_Event *ev;

   ev = mem_malloc(sizeof(Groupview_HL_Event));

   ev->part = sd->selected->part;
   ev->x = event->x / sd->zoom;
   ev->y = event->y / sd->zoom;
   ev->w = event->w / sd->zoom;
   ev->h = event->h / sd->zoom;
   ev->hl_type = event->descr;

   return ev;
}

static void
_hl_part_drag_start_cb(void *data,
                       Evas_Object *obj EINA_UNUSED,
                       void *event_info)
{
   Groupview_Smart_Data *sd = data;
   Groupview_HL_Event *event = _grouopview_hl_event_new(event_info, sd);

   evas_object_smart_callback_call(sd->obj, SIGNAL_GROUPVIEW_HL_PART_DRAG_START, event);

   free(event);
}

static void
_hl_part_changed_cb(void *data,
                    Evas_Object *obj EINA_UNUSED,
                    void *event_info)
{
   Groupview_Smart_Data *sd = data;
   Groupview_HL_Event *event = _grouopview_hl_event_new(event_info, sd);

   evas_object_smart_callback_call(sd->obj, SIGNAL_GROUPVIEW_HL_PART_CHANGED, event);

   free(event);
}

static void
_hl_part_drag_stop_cb(void *data,
                      Evas_Object * obj EINA_UNUSED,
                      void *event_info)
{
   Groupview_Smart_Data *sd = data;
   Groupview_HL_Event *event = _grouopview_hl_event_new(event_info, sd);

   evas_object_smart_callback_call(sd->obj, SIGNAL_GROUPVIEW_HL_PART_DRAG_STOP, event);

   free(event);
}
/******************************************************************************/

static void
_unselect_part(void *data,
               Evas *e EINA_UNUSED,
               Evas_Object *obj EINA_UNUSED,
               void *event_info)
{
   Evas_Object *o = (Evas_Object *)data;
   Evas_Event_Mouse_Down *ev = event_info;

   GROUPVIEW_DATA_GET(o, sd)

   if (ev->button != 1) return;

   evas_object_smart_callback_call(o, SIGNAL_GROUPVIEW_CLICKED, NULL);
   sd->selected = NULL;
}

/* create and setup a new example smart object's internals */
static void
_groupview_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Groupview_Smart_Data)

   _groupview_parent_sc->add(o);

   priv->event = evas_object_rectangle_add(evas_object_evas_get(o));
   evas_object_color_set(priv->event, 0, 0, 0, 0);
   evas_object_resize(priv->event, 9999, 9999);

   evas_object_event_callback_add(priv->event, EVAS_CALLBACK_MOUSE_UP, _unselect_part, o);

   priv->obj = o;
   priv->parts = NULL;
   priv->separated = false;
   priv->selected = NULL;

   evas_object_smart_member_add(priv->event, o);
}

static void
_groupview_smart_del(Evas_Object *o)
{
   GROUPVIEW_DATA_GET(o, sd)

   _parts_list_free(sd);
   evas_object_smart_member_del(sd->group->edit_object);
   evas_object_hide(sd->group->edit_object);

   _groupview_parent_sc->del(o);
}

static void
_groupview_smart_show(Evas_Object *o)
{
   if (evas_object_visible_get(o)) return;

   GROUPVIEW_DATA_GET(o, sd);

   evas_object_show(sd->event);
   if (sd->selected)
     evas_object_show(sd->highlight);

   _groupview_parent_sc->show(o);
}

static void
_groupview_smart_hide(Evas_Object *o)
{
   if (!evas_object_visible_get(o)) return;

   GROUPVIEW_DATA_GET(o, sd)

   evas_object_hide(sd->event);
   evas_object_hide(sd->highlight);

   _groupview_parent_sc->hide(o);
}

static void
_groupview_smart_move(Evas_Object *o,
                      Evas_Coord x,
                      Evas_Coord y)
{
   Evas_Coord ox, oy;
   GROUPVIEW_DATA_GET(o, sd)

   _groupview_parent_sc->move(o, x, y);

   evas_object_geometry_get(o, &ox, &oy, NULL, NULL);
   if ((ox == x) && (oy == y)) return;

   evas_object_move(sd->group->edit_object,x ,y);
   evas_object_move(sd->box, x, y);

   sd->geom.x = x;
   sd->geom.y = y;
   sd->zoom1.x = x;
   sd->zoom1.y = y;
}

static void
_groupview_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ow, oh;
   GROUPVIEW_DATA_GET(o, sd)

   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   sd->zoom1.w = w / sd->zoom;
   sd->zoom1.h = h / sd->zoom;

   evas_object_resize(sd->group->edit_object, sd->zoom1.w, sd->zoom1.h);
   evas_object_resize(sd->box, w, h);
   /* I hate box, we need force call the box calc for get the correct protrusion */
   elm_box_recalculate(sd->box);

   evas_object_smart_changed(o);
}

/* groupview geometry calculate */
static void
_groupview_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord px, py, pw, ph;

   GROUPVIEW_DATA_GET(o, sd)
   evas_object_geometry_get(sd->parent, &px, &py, &pw, &ph);
   evas_object_move(sd->event, px, py);

   evas_object_geometry_get(o, &x, &y, &w, &h);
   if ((sd->geom.w != w) || (sd->geom.h != h))
     {
        sd->geom.x = x;
        sd->geom.y = y;
        sd->geom.w = w;
        sd->geom.h = h;
     }
   sd->manual_calc = false;

   DBG("Groupview geometry: x[%i] y[%i] w[%i] h[%i]", x, y, w, h);
   DBG("Edit object geometry: x[%i] y[%i] w[%i] h[%i]", sd->zoom1.x, sd->zoom1.y, sd->zoom1.w, sd->zoom1.h);
   evas_object_smart_callback_call(o, SIG_GEOMETRY_CHANGED, &sd->geom);
}

/* this need for macro EVAS_SMART_SUBCLASS_NEW */
static void
_groupview_smart_set_user(Evas_Smart_Class *sc)
{
   assert(sc != NULL);

   evas_object_smart_clipped_smart_set(sc);

   sc->add = _groupview_smart_add;
   sc->del = _groupview_smart_del;
   sc->show = _groupview_smart_show;
   sc->hide = _groupview_smart_hide;
   sc->color_set = NULL;

   /* clipped smart object has no hook on resizes or calculations */
   sc->move = _groupview_smart_move;
   sc->resize = _groupview_smart_resize;
   sc->calculate = _groupview_smart_calculate;
}

Evas_Object *
groupview_add(Evas_Object *parent, Group2 *group)
{
   Evas *e;
   Evas_Object *obj;

   assert(parent != NULL);
   assert(group != NULL);

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _groupview_smart_class_new());
   GROUPVIEW_DATA_GET(obj, sd);
   sd->parent = parent;
   sd->zoom = 1.0;

   TODO("set the state for all parts to default 0.0")
   sd->group = group;
   evas_object_smart_member_add(sd->group->edit_object, obj);
   edje_object_scale_set(sd->group->edit_object, edje_object_base_scale_get(sd->group->edit_object));

   sd->box = elm_box_add(parent);
   elm_box_layout_set(sd->box, _parts_stack_layout, sd, NULL);
   evas_object_show(sd->box);
   evas_object_smart_member_add(sd->box, obj);
   _parts_list_new(sd);

   sd->object_area = elm_layout_add(sd->parent);
   elm_layout_theme_set(sd->object_area, "layout", "groupview", "object_area");
   evas_object_repeat_events_set(sd->object_area, true);
   evas_object_smart_member_add(sd->object_area, obj);

   sd->highlight = highlight_add(sd->box);
   evas_object_color_set(sd->highlight, HIGHLIGHT_COLOR);
   evas_object_smart_callback_add(sd->highlight, signals.eflete.highlight.drag_start, _hl_part_drag_start_cb, sd);
   evas_object_smart_callback_add(sd->highlight, signals.eflete.highlight.changed, _hl_part_changed_cb, sd);
   evas_object_smart_callback_add(sd->highlight, signals.eflete.highlight.drag_stop, _hl_part_drag_stop_cb, sd);
   evas_object_smart_member_add(sd->highlight, obj);

   /* hide the editing object by using clipper (clipper is small, it's size is 0,0)
    * with such clipper object invisible and calculate geometry. */
   evas_object_show(sd->group->edit_object);

   return obj;
}

/******************************************************************************/
/******************************************************************************/
Eina_Bool
groupview_recalc_all(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);

   sd->manual_calc = true;
   evas_object_smart_changed(sd->obj);
   return true;
}

void
groupview_soft_update(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);
   evas_object_smart_changed(sd->obj);
}

void
groupview_hard_update(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);
   sd->manual_calc = true;
   TODO("Check what recalculation is needed. (don't forget add part_item)");
   elm_box_recalculate(sd->box);
   evas_object_smart_changed(sd->obj);
}

const Groupview_Geom *
groupview_part_geom_get(Evas_Object *obj, const char *part)
{
   Groupview_Geom *geom = NULL;
   Groupview_Part *gp;

   GROUPVIEW_DATA_GET(obj, sd)

   gp = _parts_list_find(sd->parts, part);
   if (gp) geom = &gp->geom;

   return geom;
}

void
groupview_primitive_geom_get(Evas_Object *obj, const char *part, Evas_Coord *x1, Evas_Coord *y1, Evas_Coord *w1, Evas_Coord *h1)
{
   Groupview_Part *gp;

   GROUPVIEW_DATA_GET(obj, sd)

   gp = _parts_list_find(sd->parts, part);
   if (!gp) return;

   evas_object_geometry_get(gp->draw, x1, y1, w1, h1);
}

const Groupview_Geom *
groupview_part_selected_geom_get(Evas_Object *obj)
{
   Groupview_Geom *geom = NULL;

   GROUPVIEW_DATA_GET(obj, sd)

   if (sd->selected) geom = &sd->selected->geom;

   return geom;
}

void
groupview_primitive_selected_geom_get(Evas_Object *obj, Evas_Coord *x1, Evas_Coord *y1, Evas_Coord *w1, Evas_Coord *h1)
{
   GROUPVIEW_DATA_GET(obj, sd)

   evas_object_geometry_get(sd->selected->draw, x1, y1, w1, h1);
}

Eina_Bool
groupview_part_add(Evas_Object *obj, Part2 *part)
{
   GROUPVIEW_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_add(sd, part);
}

Eina_Bool
groupview_part_del(Evas_Object *obj, Part2 *part)
{
   GROUPVIEW_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_del(sd, part);
}

Eina_Bool
groupview_part_restack(Evas_Object *obj, const char *part, const char *rel_part)
{
   GROUPVIEW_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_restack_below(sd, part, rel_part);
}

const Groupview_Geom *
groupview_part_selected_object_area_geom_get(Evas_Object *obj)
{
   Groupview_Geom *geom = NULL;

   GROUPVIEW_DATA_GET(obj, sd)

   if (sd->selected) geom = &sd->selected->object_area_geom;

   return geom;
}

void
groupview_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible)
{
   GROUPVIEW_DATA_GET(obj, sd);

   sd->obj_area_visible = visible;

   if (sd->obj_area_visible && sd->selected)
     {
        _part_object_area_calc(sd, sd->selected);
        evas_object_geometry_set(sd->object_area,
                                 sd->selected->object_area_geom.x,
                                 sd->selected->object_area_geom.y,
                                 sd->selected->object_area_geom.w,
                                 sd->selected->object_area_geom.h);
        evas_object_show(sd->object_area);
     }
   else evas_object_hide(sd->object_area);
   highlight_handler_align_visible_set(sd->highlight, visible);
}

void
groupview_primitive_area_selected_geom_get(Evas_Object *obj, Evas_Coord *x1, Evas_Coord *y1, Evas_Coord *w1, Evas_Coord *h1)
{
   GROUPVIEW_DATA_GET(obj, sd)

   evas_object_geometry_get(sd->object_area, x1, y1, w1, h1);
}

Eina_Bool
groupview_part_object_area_visible_get(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);

   return sd->obj_area_visible;
}

Eina_Bool
groupview_parts_separated(Evas_Object *obj EINA_UNUSED,
                          Eina_Bool separated EINA_UNUSED)
{
#if 0
   int w, h, count;
   GROUPVIEW_DATA_GET(obj, sd);

   assert(sd->group->edit_object != NULL);

   if (!sd->parts) return false;
   if (sd->separated == separated) return true;

   sd->separated = separated;
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   count = eina_list_count(sd->parts);
   /* after resize the groupview object it will be marked as dirty,
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
     }
   else
     {
        evas_object_resize(obj, w - (SEP_ITEM_PAD_X * count), h - (SEP_ITEM_PAD_Y * count));
        /* emit the signal that the groupview returned to the normal mode.
           send the name of selected item(part), for hilight and widget list
           events. */
        DBG("Separate mod off; selected part is %s",
            sd->selected ? sd->selected->part->common.name : NULL);
        evas_object_smart_callback_call(obj, SIG_PART_SEPARETE_CLOSE, NULL);
        _selected_item_return_to_place(sd);
     }
#endif /*  */
   return true;
}

Eina_Bool
groupview_parts_separated_is(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);
   return sd->separated;
}

void
groupview_part_select(Evas_Object *obj, const char *part)
{
   Groupview_Part *gp;
   GROUPVIEW_DATA_GET(obj, sd);

   if (part)
     {
        gp = _parts_list_find(sd->parts, part);
        assert(gp != NULL);
        sd->selected = gp;
        evas_object_geometry_set(sd->highlight, gp->geom.x, gp->geom.y, gp->geom.w, gp->geom.h);
        evas_object_show(sd->highlight);
        if (sd->obj_area_visible)
          {
             evas_object_geometry_set(sd->object_area,
                                      gp->object_area_geom.x,
                                      gp->object_area_geom.y,
                                      gp->object_area_geom.w,
                                      gp->object_area_geom.h);
             evas_object_show(sd->object_area);
          }
     }
   else
     {
        if (sd->selected && sd->selected->current_item)
          elm_object_signal_emit(sd->selected->current_item->layout, "border,part_item,hilight,off", "eflete");
        sd->selected = NULL;
        evas_object_hide(sd->highlight);
        evas_object_hide(sd->object_area);
     }
   highlight_handler_align_visible_set(sd->highlight, sd->obj_area_visible);
}

TODO("remove this from public API and use callback from part list");

void
groupview_part_visible_set(Evas_Object *obj, Part2 *part)
{
   Groupview_Part *gp;
   GROUPVIEW_DATA_GET(obj, sd);

   assert(part != NULL);

   gp = _parts_list_find(sd->parts, part->common.name);

   assert(gp != NULL);
   if (part->visible)
     {
        evas_object_show(gp->draw);
        evas_object_resize(gp->draw, gp->geom.w, gp->geom.h);
        evas_object_move(gp->draw, gp->geom.x, gp->geom.y);
     }
   else
     evas_object_hide(gp->draw);
}

void
groupview_zoom_factor_set(Evas_Object *obj, double zoom)
{
   GROUPVIEW_DATA_GET(obj, sd);

   sd->zoom = zoom;
}

double
groupview_zoom_factor_get(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);

   return sd->zoom;
}

void
groupview_part_item_selected_set(Evas_Object *obj,
                                 Eina_Stringshare *item_name,
                                 Eina_Bool selected)
{
   Groupview_Part *gp;
   Groupview_Item *item;
   GROUPVIEW_DATA_GET(obj, sd);

   gp = sd->selected;
   if (!gp) return;

   if ((selected) && (item_name))
     {
        item = _part_item_search(gp->items, item_name);
        if (gp->current_item)
          elm_object_signal_emit(gp->current_item->layout, "border,part_item,hilight,off", "eflete");
        elm_object_signal_emit(item->layout, "border,part_item,hilight,on", "eflete");
        gp->current_item = item;
     }
   else if (gp->current_item)
     {
        elm_object_signal_emit(gp->current_item->layout, "border,part_item,hilight,off", "eflete");
        gp->current_item = NULL;
     }
}

Groupview_Geom *
groupview_protrusion_get(Evas_Object *obj)
{
   GROUPVIEW_DATA_GET(obj, sd);

   return &sd->protrusion;
}
