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

#include "groupedit_private.h"
#include "cursor.h"

#define MY_CLASS_NAME "Ws_Groupedit"

/* here we create and define some functions for creating our groupedit smart
  object that is inherited from general Smart Object. */
EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _groupedit,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, _smart_callbacks);

static void
_unselect_part(void *data,
               Evas *e __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Evas_Object *o = (Evas_Object *)data;
   Evas_Event_Mouse_Down *ev = event_info;
   WS_GROUPEDIT_DATA_GET(o, sd)

   if (ev->button != 1) return;
   if (!sd->selected) return;
   evas_object_smart_callback_call(o, SIGNAL_GROUPEDIT_PART_UNSELECTED,
                                   (void *)sd->selected->part);
   sd->selected = NULL;
}

/* create and setup a new example smart object's internals */
static void
_groupedit_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Ws_Groupedit_Smart_Data)

   _groupedit_parent_sc->add(o);

   priv->e = evas_object_evas_get(o);

   priv->event = evas_object_rectangle_add(priv->e);
   evas_object_color_set(priv->event, 0, 0, 0, 0);

   evas_object_event_callback_add(priv->event, EVAS_CALLBACK_MOUSE_UP,
                                  _unselect_part, o);

   priv->obj = o;
   priv->parts = NULL;
   priv->separated = false;
   priv->selected = NULL;

   evas_object_smart_member_add(priv->event, o);
}

static void
_groupedit_smart_del(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET(o, sd)

   _parts_list_free(sd);
   evas_object_smart_member_del(sd->group->edit_object);
   evas_object_smart_member_del(sd->clipper);
   evas_object_hide(sd->group->edit_object);

   _groupedit_parent_sc->del(o);
}

static void
_groupedit_smart_show(Evas_Object *o)
{
   if (evas_object_visible_get(o)) return;

   WS_GROUPEDIT_DATA_GET(o, sd);

   evas_object_show(sd->event);

   _groupedit_parent_sc->show(o);
}

static void
_groupedit_smart_hide(Evas_Object *o)
{
   if (!evas_object_visible_get(o)) return;

   WS_GROUPEDIT_DATA_GET(o, sd)

   evas_object_hide(sd->event);

   _groupedit_parent_sc->hide(o);
}

static void
_groupedit_smart_move(Evas_Object *o,
                      Evas_Coord x,
                      Evas_Coord y)
{
   Evas_Coord ox, oy;
   WS_GROUPEDIT_DATA_GET(o, sd)

   _groupedit_parent_sc->move(o, x, y);

   evas_object_geometry_get(o, &ox, &oy, NULL, NULL);
   if ((ox == x) && (oy == y)) return;

   evas_object_smart_changed(o);
}

static void
_groupedit_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ow, oh;
   WS_GROUPEDIT_DATA_GET(o, sd)

   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   evas_object_smart_changed(o);
}

/* groupedit geometry calculate */
static void
_groupedit_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord px, py, pw, ph;

   WS_GROUPEDIT_DATA_GET(o, priv)
      evas_object_geometry_get(priv->parent, &px, &py, &pw, &ph);
   evas_object_resize(priv->event, pw, ph);
   evas_object_move(priv->event, px, py);

   evas_object_geometry_get(o, &x, &y, &w, &h);

   if ((priv->geom.x != x) || (priv->geom.y != y) ||
       (priv->geom.w != w) || (priv->geom.h != h))
     {
        priv->geom.x = x;
        priv->geom.y = y;
        priv->geom.w = w;
        priv->geom.h = h;

        evas_object_move(priv->group->edit_object,x ,y);
        evas_object_resize(priv->group->edit_object, w, h);
        evas_object_move(priv->box, x, y);
        evas_object_resize(priv->box, w, h);
     }
   else
     elm_box_recalculate(priv->box);

   priv->manual_calc = false;

   DBG("Groupedit geometry: x[%i] y[%i] w[%i] h[%i]", x, y, w, h);
   evas_object_smart_callback_call(o, SIG_CHANGED, &priv->geom);
}

/* this need for macro EVAS_SMART_SUBCLASS_NEW */
static void
_groupedit_smart_set_user(Evas_Smart_Class *sc)
{
   assert(sc != NULL);

   evas_object_smart_clipped_smart_set(sc);

   sc->add = _groupedit_smart_add;
   sc->del = _groupedit_smart_del;
   sc->show = _groupedit_smart_show;
   sc->hide = _groupedit_smart_hide;
   sc->color_set = NULL;

   /* clipped smart object has no hook on resizes or calculations */
   sc->move = _groupedit_smart_move;
   sc->resize = _groupedit_smart_resize;
   sc->calculate = _groupedit_smart_calculate;
}

Evas_Object *
groupedit_add(Evas_Object *parent, Group *group)
{
   Evas *e;
   Evas_Object *obj;

   assert(parent != NULL);
   assert(group != NULL);

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _groupedit_smart_class_new());
   WS_GROUPEDIT_DATA_GET(obj, sd);
   sd->parent = parent;

   edje_object_animation_set(group->edit_object, false);
   TODO("set the state for all parts to default 0.0")
   sd->group = group;
   evas_object_smart_member_add(sd->group->edit_object, obj);

   sd->box = elm_box_add(parent);
   elm_box_layout_set(sd->box, _parts_stack_layout, sd, NULL);
   evas_object_show(sd->box);
   evas_object_smart_member_add(sd->box, obj);
   _parts_list_new(sd);

   /* hide the editing object by using clipper (clipper is small, it's size is 0,0)
    * with such clipper object invisible and calculate geometry. */
   evas_object_show(sd->group->edit_object);
   sd->clipper = evas_object_rectangle_add(sd->e);
   evas_object_clip_set(sd->group->edit_object, sd->clipper);
   evas_object_smart_member_add(sd->clipper, obj);
   evas_object_show(sd->clipper);

   return obj;
}

/******************************************************************************/
/******************************************************************************/
Eina_Bool
groupedit_edit_object_recalc_all(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   sd->manual_calc = true;
   evas_object_smart_changed(sd->obj);
   return true;
}

void
groupedit_soft_update(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);
   evas_object_smart_changed(sd->obj);
}

void
groupedit_hard_update(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);
   sd->manual_calc = true;
   TODO("Check what recalculation is needed. (don't forget add part_item)");
   elm_box_recalculate(sd->box);
   evas_object_smart_changed(sd->obj);
}

Evas_Object *
groupedit_edit_object_part_draw_get(Evas_Object *obj, const char *part)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET(obj, sd)

   assert(part != NULL);

   gp = _parts_list_find(sd->parts, part);

   assert(gp != NULL);

   return gp->draw;
}

Eina_Bool
groupedit_edit_object_part_add(Evas_Object *obj, Part_ *part)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_add(sd, part);
}

Eina_Bool
groupedit_edit_object_part_del(Evas_Object *obj, Part_ *part)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_del(sd, part);
}

Eina_Bool
groupedit_edit_object_part_above(Evas_Object *obj, const char *part)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_restack_above(sd, part, NULL);
}

Eina_Bool
groupedit_edit_object_part_below(Evas_Object *obj, const char *part)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_restack_below(sd, part, NULL);
}

Eina_Bool
groupedit_edit_object_part_move_above(Evas_Object *obj,
                                      const char *part,
                                      const char *above)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_restack_above(sd, part, above);
}

Eina_Bool
groupedit_edit_object_part_move_below(Evas_Object *obj,
                                      const char *part,
                                      const char *below)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   return _edit_object_part_restack_below(sd, part, below);
}

Eina_Bool
groupedit_edit_object_part_state_set(Evas_Object *obj, Part_ *part)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   ret = edje_edit_part_selected_state_set(sd->group->edit_object,
                                           part->name,
                                           part->current_state->parsed_name,
                                           part->current_state->parsed_val);
   evas_object_smart_changed(sd->obj);
   return ret;
}

Evas_Object *
groupedit_part_object_area_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd)

   return sd->obj;
}

void
groupedit_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible __UNUSED__)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   if (!sd->selected) return;
}

Eina_Bool
groupedit_part_object_area_visible_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);
   return false;
}

Eina_Bool
groupedit_edit_object_parts_separated(Evas_Object *obj __UNUSED__,
                                      Eina_Bool separated __UNUSED__)
{
#if 0
   int w, h, count;
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(sd->group->edit_object != NULL);

   if (!sd->parts) return false;
   if (sd->separated == separated) return true;

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
     }
   else
     {
        evas_object_resize(obj, w - (SEP_ITEM_PAD_X * count), h - (SEP_ITEM_PAD_Y * count));
        /* emit the signal that the groupedit returned to the normal mode.
           send the name of selected item(part), for hilight and widget list
           events. */
        DBG("Separate mod off; selected part is %s",
            sd->selected ? sd->selected->part->name : NULL);
        evas_object_smart_callback_call(obj, SIG_PART_SEPARETE_CLOSE, NULL);
        _selected_item_return_to_place(sd);
     }
#endif /*  */
   return true;
}

Eina_Bool
groupedit_edit_object_parts_separated_is(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);
   return sd->separated;
}

void
groupedit_edit_object_part_select(Evas_Object *obj, const char *part)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET(obj, sd);

   if (part)
     {
        gp = _parts_list_find(sd->parts, part);
        assert(gp != NULL);
        sd->selected = gp;
     }
   else
     {
        if (sd->selected && sd->selected->current_item)
          elm_object_signal_emit(sd->selected->current_item->layout, "border,part_item,hilight,off", "eflete");
        sd->selected = NULL;
     }
}

TODO("remove this from public API and use callback from part list");

void
groupedit_part_visible_set(Evas_Object *obj, Part_ *part)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET(obj, sd);

   assert(part != NULL);

   gp = _parts_list_find(sd->parts, part->name);

   assert(gp != NULL);
   if (part->visible)
     evas_object_show(gp->draw);
   else
     evas_object_hide(gp->draw);
}

Eina_Bool
groupedit_zoom_factor_set(Evas_Object *obj, double factor __UNUSED__)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   return true;
}

void
groupedit_edit_object_part_item_selected_set(Evas_Object *obj,
                                             Eina_Stringshare *item_name,
                                             Eina_Bool selected)
{
   Groupedit_Part *gp;
   Groupedit_Item *item;
   WS_GROUPEDIT_DATA_GET(obj, sd);

   gp = sd->selected;
   if (!gp) return;

   if (!item_name) return;

   if (selected)
     {
        item = _part_item_search(gp->items, item_name);
        if (gp->current_item)
          elm_object_signal_emit(gp->current_item->layout, "border,part_item,hilight,off", "eflete");
        elm_object_signal_emit(item->layout, "border,part_item,hilight,on", "eflete");
        gp->current_item = item;
     }
   else
     {
        elm_object_signal_emit(gp->current_item->layout, "border,part_item,hilight,off", "eflete");
        gp->current_item = NULL;
     }
}

const Groupedit_Geom *
groupedit_protrusion_get(Evas_Object *obj)
{
   WS_GROUPEDIT_DATA_GET(obj, sd);

   return &sd->protrusion;
}

#undef MY_CLASS_NAME
#undef WS_GROUPSPACE_DATA_GET
#undef WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL
