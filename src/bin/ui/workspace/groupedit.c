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
_style_set(Evas_Object *o, const char *style)
{
   char group[BUFF_MAX];
   WS_GROUPEDIT_DATA_GET(o, sd)

   #define GROUP_NAME(item, style) \
      snprintf(group, BUFF_MAX, "eflete/groupedit/%s/%s", item, style);

   GROUP_NAME("object_area", style)
   if (!edje_object_file_set(sd->obj_area.obj, EFLETE_EDJ, group))
     {
        GROUP_NAME("object_area", "default")
        if (!edje_object_file_set(sd->obj_area.obj, EFLETE_EDJ, group))
          ERR("Could not set up default theme for object area!");
     }

   #undef GROUP_NAME
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

   priv->event = evas_object_rectangle_add(priv->e);
   evas_object_color_set(priv->event, 0, 0, 0, 0);

   evas_object_event_callback_add(priv->event, EVAS_CALLBACK_MOUSE_UP,
                                  _unselect_part, o);

   priv->obj = o;
   priv->con_current_size = (Groupedit_Geom *)mem_calloc(1, sizeof(Groupedit_Geom));
   priv->real_size = (Groupedit_Geom *)mem_calloc(1, sizeof(Groupedit_Geom));
   priv->zoom_factor = 1.0;
   priv->edit_obj = NULL;
   priv->parts = NULL;
   priv->obj_area.obj = edje_object_add(priv->e);
   evas_object_repeat_events_set(priv->obj_area.obj, true);
   priv->obj_area.visible = false;
   priv->obj_area.show_now = false;
   priv->obj_area.geom = (Groupedit_Geom *)mem_calloc(1, sizeof(Groupedit_Geom));
   priv->separated = false;
   priv->selected = NULL;
   priv->to_select = NULL;

   evas_object_smart_member_add(priv->obj_area.obj, o);
   evas_object_smart_member_add(priv->event, o);

   _style_set(o, "default");
}

static void
_groupedit_smart_del(Evas_Object *o)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   _parts_list_free(sd);

   free(sd->real_size);
   free(sd->con_current_size);
   free(sd->obj_area.geom);

   _groupedit_parent_sc->del(o);
}

static void
_groupedit_smart_show(Evas_Object *o)
{
   if (evas_object_visible_get(o)) return;

   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   evas_object_show(sd->event);

   _groupedit_parent_sc->show(o);
}

static void
_groupedit_smart_hide(Evas_Object *o)
{
   if (!evas_object_visible_get(o)) return;

   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_hide(sd->event);

   _groupedit_parent_sc->hide(o);
}

static void
_groupedit_smart_move(Evas_Object *o,
                      Evas_Coord x,
                      Evas_Coord y)
{
   Evas_Coord ox, oy;
   Evas_Object *bg;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   _groupedit_parent_sc->move(o, x, y);

   evas_object_geometry_get(o, &ox, &oy, NULL, NULL);
   if ((ox == x) && (oy == y)) return;

   if (sd->separated)
     {
        bg = evas_object_image_source_get(sd->bg);
        evas_object_geometry_get(bg, &ox, &oy, NULL, NULL);
        evas_object_move(sd->bg, ox, oy);
        if (sd->selected)
          {
             evas_object_geometry_get(sd->selected->item, &ox, &oy, NULL, NULL);
             evas_object_move(sd->clipper, ox, oy);
          }
     }
}

static void
_groupedit_smart_resize(Evas_Object *o,
                        Evas_Coord w,
                        Evas_Coord h)
{
   Evas_Coord ow, oh;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

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
   char buff[16];

   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, priv, RETURN_VOID)
   evas_object_geometry_get(priv->parent, &px, &py, &pw, &ph);
   evas_object_resize(priv->event, pw, ph);
   evas_object_move(priv->event, px, py);

   evas_object_geometry_get(o, &x, &y, &w, &h);

   if (!priv->separated)
     {
        priv->con_current_size->x = x;
        priv->con_current_size->y = y;
        priv->con_current_size->w = w;
        priv->con_current_size->h = h;
        snprintf(buff, 16, "%i %i", priv->con_current_size->w, priv->con_current_size->h);
     }

   DBG("Groupedit geometry: x[%i] y[%i] w[%i] h[%i]", x, y, w, h);
   if (!priv->separated)
     {
        evas_object_move(priv->edit_obj, priv->con_current_size->x,
                                         priv->con_current_size->y);
     }
   evas_object_resize(priv->edit_obj, priv->con_current_size->w,
                                      priv->con_current_size->h);

   _parts_recalc(priv);

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
   sc->color_set = NULL;

   /* clipped smart object has no hook on resizes or calculations */
   sc->move = _groupedit_smart_move;
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
        TODO("add error message")
        return false;
     }
   if ((!file) || (!ecore_file_exists(file))) return false;

   edje_object_animation_set(edit_obj, false);
   /* hide the editing object, we can not use evas_object_hide, because object
    * will be showed again, after changing, for example part add, or change
    * part state. so set the object opacity 0 - object invisible and calculate
    * geometry. */
   evas_object_color_set(edit_obj, 0, 0, 0, 0);
   TODO("set the state for all parts to default 0.0")
   sd->edit_obj = edit_obj;
   evas_object_smart_member_add(sd->edit_obj, obj);
   sd->edit_obj_file = file;

   _edit_object_load(sd);
   _parts_list_free(sd);
   _parts_list_new(sd);

   evas_object_smart_changed(sd->obj);

   return true;
}

Evas_Object *
groupedit_edit_object_unset(Evas_Object *obj)
{
   Evas_Object *ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   groupedit_bg_unset(obj);
   if ((!sd->edit_obj) && (!sd->parts)) return NULL;

   _parts_list_free(sd);
   ret = sd->edit_obj;
   evas_object_smart_member_del(sd->edit_obj);
   sd->edit_obj = NULL;

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

   if (!gp) return NULL;
   return gp->border;
}

Eina_Bool
groupedit_edit_object_part_rename(Evas_Object *obj,
                                  const char *part_old_name,
                                  const char *part_new_name)
{
   Groupedit_Part *gp;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false)

   gp = _parts_list_find(sd->parts, part_old_name);
   if (!gp) return false;

   eina_stringshare_replace(&gp->name, part_new_name);

   return true;
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
groupedit_edit_object_part_item_add(Evas_Object *obj, Eina_Stringshare *part,
                                    Eina_Stringshare *item, Eina_Stringshare *source)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   return _edit_object_part_item_add(sd, part, item, source);
}

Eina_Bool
groupedit_edit_object_part_item_del(Evas_Object *obj, Eina_Stringshare *part,
                                    Eina_Stringshare *item)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   return _edit_object_part_item_del(sd, part, item);
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
   const char *img = NULL;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state)) return false;

   ret = edje_edit_state_add(sd->edit_obj, part, state, value);
   ret &= edje_edit_part_selected_state_set(sd->edit_obj, part, state, value);
   img = edje_edit_state_image_get(sd->edit_obj, part, "default", 0.0);
   edje_edit_state_image_set(sd->edit_obj, part, state, value, img);

   if (ret)
     {
        evas_object_smart_changed(sd->obj);
        history_diff_add(sd->edit_obj, STATE_TARGET, ADD, "unsued", part, state,
                         value, "state");
     }
   return ret;
}

Eina_Bool
groupedit_edit_object_part_state_copy(Evas_Object *obj, const char *part,
                                      const char *state_from, double value_from,
                                      const char *state_to, double value_to)
{
   Eina_Bool ret;
   const char *img = NULL;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state_from) || (!state_to)) return false;

   ret = edje_edit_state_copy(sd->edit_obj, part, state_from, value_from,
                              state_to, value_to);
   ret &= edje_edit_part_selected_state_set(sd->edit_obj, part, state_to, value_to);
   img = edje_edit_state_image_get(sd->edit_obj, part, state_from, value_from);
   edje_edit_state_image_set(sd->edit_obj, part, state_to, value_to, img);

   if (ret)
     {
        evas_object_smart_changed(sd->obj);
        history_diff_add(sd->edit_obj, STATE_TARGET, ADD, "unused", part, state_to,
                         value_to, "state");
     }

   return ret;
}

Eina_Bool
groupedit_edit_object_part_state_del(Evas_Object *obj, const char *part,
                                     const char *state, double value)
{
   Eina_Bool ret;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!part) || (!state)) return false;

   history_diff_add(sd->edit_obj, STATE_TARGET, DEL, "unused", part, state, value,
                    "state");
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

Eina_Bool
groupedit_edit_object_parts_separated(Evas_Object *obj,
                                      Eina_Bool separated)
{
   int w, h, count;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if ((!sd->edit_obj) || (!sd->parts)) return false;
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
            sd->selected ? sd->selected->name : NULL);
        evas_object_smart_callback_call(obj, SIG_PART_SEPARETE_CLOSE, NULL);
        _selected_item_return_to_place(sd);
     }
   return true;
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

static void
_bg_changed(void *data,
                Evas *evas __UNUSED__,
                Evas_Object *o,
                void *einfo __UNUSED__)
{
   int w, h;
   Evas_Object *bg = (Evas_Object *)data;
   evas_object_geometry_get(o, NULL, NULL, &w, &h);
   evas_object_resize(bg, w, h);
}

Eina_Bool
groupedit_bg_set(Evas_Object *obj, Evas_Object *bg)
{
   int w, h;

   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   if (!bg) return false;

   if (!bg)
     {
        Evas_Object *old_bg = evas_object_image_source_get(sd->bg);
        evas_object_event_callback_del_full(old_bg,
                                            EVAS_CALLBACK_RESIZE,
                                            _bg_changed, sd->bg);
     }

   sd->bg = evas_object_image_filled_add(sd->e);
   sd->clipper = evas_object_rectangle_add(sd->e);

   evas_object_geometry_get(bg, NULL, NULL, &w, &h);
   evas_object_image_source_set(sd->bg, bg);
   evas_object_resize(sd->bg, w, h);
   evas_object_event_callback_add(bg,
                                  EVAS_CALLBACK_RESIZE,
                                  _bg_changed, sd->bg);

   evas_object_smart_member_add(sd->bg, obj);
   evas_object_smart_member_add(sd->clipper, obj);

   return true;
}

Evas_Object *
groupedit_bg_unset(Evas_Object *obj)
{
   Evas_Object *bg;
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);

   bg = evas_object_image_source_get(sd->bg);
   evas_object_event_callback_del_full(bg,
                                       EVAS_CALLBACK_RESIZE,
                                       _bg_changed, sd->bg);
   evas_object_del(sd->bg);
   sd->bg = NULL;

   return bg;
}

Eina_Bool
groupedit_zoom_factor_set(Evas_Object *obj, double factor)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   sd->zoom_factor = factor;

   return true;
}

Eina_Bool
groupedit_edit_object_part_item_selected_set(Evas_Object *obj,
                                             Eina_Stringshare *item_name,
                                             Eina_Bool selected)
{
   WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(obj, sd, false);
   Groupedit_Part *gp = sd->selected;
   Eina_List *l, *l_n;
   Groupedit_Item *ge_item = NULL;

   if (!gp) return false;

   EINA_LIST_FOREACH_SAFE(gp->items, l, l_n, ge_item)
     {
        if (ge_item->name == item_name)
          {
             ge_item->selected = selected;
             if (selected)
               {
                  edje_object_file_set(ge_item->highlight, EFLETE_RESOURCES, BORDER_2PX_IMG);
                  evas_object_color_set(ge_item->highlight, 0, 253, 255, 255);
               }
             else
               {
                  edje_object_file_set(ge_item->highlight, EFLETE_RESOURCES, BORDER_IMG);
                  evas_object_color_set(ge_item->highlight, 49, 140, 141, 255);
               }
             _parts_recalc(sd);
             return true;
          }
     }
   return false;
}

#undef MY_CLASS_NAME
#undef WS_GROUPSPACE_DATA_GET
#undef WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL
