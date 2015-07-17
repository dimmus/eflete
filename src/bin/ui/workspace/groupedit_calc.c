/*
 * Edje Theme Editor
 * Copyright (C) 2013 Samsung Electronics.
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
#include "alloc.h"

#define PART_STATE_GET(obj, part) \
   const char *state; \
   double value; \
   state = edje_edit_part_selected_state_get(obj, part, &value);

#define PART_STATE_FREE \
   edje_edit_string_free(state);

static void
_groupedit_part_free(Groupedit_Part *gp);

static Groupedit_Part *
_part_draw_add(Ws_Groupedit_Smart_Data *sd, const char *part, Edje_Part_Type type);

static void
_move_border_to_top(Ws_Groupedit_Smart_Data *sd);

static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, const char *part);

static Groupedit_Item *
_item_draw_add(Ws_Groupedit_Smart_Data *sd, Eina_Stringshare *part, Eina_Stringshare *item);

static void
_item_draw_del(Groupedit_Item *ge_item);

static Evas_Object *
_part_container_add(Ws_Groupedit_Smart_Data *sd, Eina_Stringshare *part, Eina_List **items, Edje_Part_Type type);

static void
_part_object_area_calc(Ws_Groupedit_Smart_Data *sd);

static void
_rectangle_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_image_param_update(Groupedit_Part *gp, Evas_Object *edit_obj, const char *file);

static void
_proxy_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_text_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_textblock_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_group_param_update(Groupedit_Part *gp, Evas_Object *edit_obj, const char *file, Evas_Object *parent);

static void
_table_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp);

static void
_box_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp);

Eina_Bool
_edit_object_part_add(Ws_Groupedit_Smart_Data *sd, const char *part,
                      Edje_Part_Type type, const char *data __UNUSED__)
{
   Groupedit_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);
   assert(type < EDJE_PART_TYPE_LAST);
   if (!edje_edit_part_add(sd->edit_obj, part, type))
     {
        ERR("Cann't add part %s to edit object %p", part, sd->edit_obj);
        abort();
     }
   gp = _part_draw_add(sd, part, type);
   sd->parts = eina_list_append(sd->parts, gp);
   _move_border_to_top(sd);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_del(Ws_Groupedit_Smart_Data *sd, const char *part)
{
   assert(sd != NULL);
   assert(sd->parts != NULL);
   assert(part != NULL);
   if (!edje_edit_part_del(sd->edit_obj, part))
     {
        ERR("Failed to delete part from edje edit object");
        abort();
     }

   _selected_item_return_to_place(sd);
   _part_draw_del(sd, part);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_item_add(Ws_Groupedit_Smart_Data *sd,
                           Eina_Stringshare *part,
                           Eina_Stringshare *item,
                           Eina_Stringshare *source)
{
   Groupedit_Part *gp;
   Groupedit_Item *ge_item = NULL;
   unsigned char type = EDJE_PART_TYPE_NONE;

   assert(sd != NULL);
   assert(part != NULL);
   assert(item != NULL);
   assert(source != NULL);

   gp = _parts_list_find(sd->parts, part);
   if (!edje_edit_part_item_append(sd->edit_obj, part, item, source))
     return false;

   ge_item = _item_draw_add(sd, part, item);
   if (!ge_item) return false;
   type = edje_edit_part_type_get(sd->edit_obj, part);

   if (type == EDJE_PART_TYPE_TABLE)
     {
        /*Position new item in cell with coords [0;0]*/
        edje_edit_part_item_position_set(sd->edit_obj, part, item, 0, 0);
        evas_object_table_pack(gp->draw, ge_item->border, 0, 0, 1, 1);
        evas_object_table_pack(gp->draw, ge_item->draw, 0, 0, 1, 1);
        evas_object_table_pack(gp->draw, ge_item->highlight, 0, 0, 1, 1);
     }

   gp->items = eina_list_append(gp->items, ge_item);
   evas_object_smart_member_add(ge_item->border, gp->draw);
   evas_object_smart_member_add(ge_item->draw, gp->draw);
   evas_object_smart_member_add(ge_item->highlight, gp->draw);

   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_item_del(Ws_Groupedit_Smart_Data *sd __UNUSED__,
                           Eina_Stringshare *part __UNUSED__,
                           Eina_Stringshare *item __UNUSED__)
{
   Groupedit_Part *gp;
   Groupedit_Item *ge_item = NULL;
   Eina_List *l, *l_next;

   gp = _parts_list_find(sd->parts, part);

   if (!edje_edit_part_item_del(sd->edit_obj, part, item))
     return false;

   EINA_LIST_FOREACH_SAFE(gp->items, l, l_next, ge_item)
     {
        if (ge_item->name == item) break;
        ge_item = NULL;
     }
   if (!ge_item) return false;
   evas_object_table_unpack(gp->draw, ge_item->draw);
   gp->items = eina_list_remove(gp->items, ge_item);
   _item_draw_del(ge_item);

   evas_object_smart_changed(sd->obj);
   return true;
}

static void
_part_parts_layouts_update(Ws_Groupedit_Smart_Data *sd,
                           Groupedit_Part *ge_part,
                           Groupedit_Part *ge_part_to,
                           Eina_Bool mode)
{
   assert(sd != NULL);
   assert(ge_part != NULL);
   assert(ge_part_to != NULL);

   if (mode)
     {
        evas_object_stack_below(sd->bg, ge_part_to->draw);
        evas_object_stack_below(sd->clipper, ge_part_to->draw);
        evas_object_stack_below(ge_part->bg, ge_part_to->draw);
        evas_object_stack_below(ge_part->draw, ge_part_to->draw);
        evas_object_stack_below(ge_part->border, ge_part_to->draw);
        evas_object_stack_below(ge_part->item, ge_part_to->draw);
     }
   else
     {
        evas_object_stack_above(sd->bg, ge_part_to->border);
        evas_object_stack_above(sd->clipper, ge_part_to->border);
        evas_object_stack_above(ge_part->item, ge_part_to->border);
        evas_object_stack_above(ge_part->border, ge_part_to->border);
        evas_object_stack_above(ge_part->draw, ge_part_to->border);
        evas_object_stack_above(ge_part->bg, ge_part_to->border);
     }
}

/**
  * Internal function, which provide restack parts at parts list and restack
  * part primitives at canvas.
  * If param part_to is NULL, part, which name stored at param part, will
  * restack below or above at one position. Else part will restack to part which
  * name stored at part_to.
  *
  * @param sd The groupedit smart data structure for get access to parts list.
  * @param part The name of part to stack.
  * @param part_to The name of part which to stack.
  * @param mode If true operation restack above will be process, else is
  * restack below.
  *
  * @return false on failure, true on success.
  */
static Eina_Bool
_part_restack(Ws_Groupedit_Smart_Data *sd,
              const char *part,
              const char *part_to,
              Eina_Bool mode)
{
   Eina_List *l = NULL;
   Groupedit_Part *ge_part_to, *ge_part;

   assert(sd != NULL);
   assert(part != NULL);
   assert(part_to != NULL);

   ge_part = _parts_list_find(sd->parts, part);
   if (!ge_part) return false;
   if (sd->separated) sd->to_select = ge_part;
   /* Here find part_to in parts stack. ge_part will be restack abow or below
    * ge_part_to object. */
   if (part_to)
     ge_part_to = _parts_list_find(sd->parts, part_to);
   else
     {
        l = eina_list_data_find_list(sd->parts, ge_part);
        if (mode)
          ge_part_to = eina_list_data_get(eina_list_prev(l));
        else
          ge_part_to = eina_list_data_get(eina_list_next(l));
     }

   if (!ge_part_to) return false;

   /* For changing position ge_part in Eina_List of parts, at first need to
    * remove node with data ge_part, and for second prepend/append ge_part to
    * list of parts. (Operation prepend use when restack above, and
    * append if restack below)
    */
   sd->parts = eina_list_remove(sd->parts, ge_part);

   if (mode)
     sd->parts = eina_list_prepend_relative(sd->parts, ge_part, ge_part_to);
   else
     sd->parts = eina_list_append_relative(sd->parts, ge_part, ge_part_to);

   if (sd->separated)
     {
        _select_item_move_to_top(sd);
        return true;
     }
   _part_parts_layouts_update(sd, ge_part, ge_part_to, mode);

   return true;
}

Eina_Bool
_edit_object_part_restack_above(Ws_Groupedit_Smart_Data *sd,
                                const char *part,
                                const char *part_above)
{
   Eina_Bool ret = false;

   assert(sd != NULL);
   assert(sd->parts != NULL);
   assert(part != NULL);

   if (part_above)
     ret = edje_edit_part_restack_part_below(sd->edit_obj, part, part_above);
   else
     ret = edje_edit_part_restack_below(sd->edit_obj, part);

   if (!ret)
     {
        ERR("Failed restack in edje_edit object");
        abort();
     }
   _part_restack(sd, part, part_above, true);
   evas_object_smart_changed(sd->obj);
   return true;
}

Eina_Bool
_edit_object_part_restack_below(Ws_Groupedit_Smart_Data *sd,
                                const char *part,
                                const char *part_below)
{
   Eina_Bool ret = false;

   assert(sd != NULL);
   assert(sd->parts != NULL);
   assert(part != NULL);

   if (part_below)
     ret = edje_edit_part_restack_part_above(sd->edit_obj, part, part_below);
   else
     ret = edje_edit_part_restack_above(sd->edit_obj, part);

   if (!ret)
     {
        ERR("Failed restack in edje_edit object");
        abort();
     }
   _part_restack(sd, part, part_below, false);
   evas_object_smart_changed(sd->obj);
   return true;
}

void
_parts_list_new(Ws_Groupedit_Smart_Data *sd)
{
   Groupedit_Part *gp;
   Eina_List *parts_list, *l;
   const char *name;
   Edje_Part_Type type;

   assert(sd != NULL);

   parts_list = edje_edit_parts_list_get(sd->edit_obj);

   evas_event_freeze(sd->e);
   EINA_LIST_FOREACH(parts_list, l, name)
     {
        type = edje_edit_part_type_get(sd->edit_obj, name);
        gp = _part_draw_add(sd, name, type);
        sd->parts = eina_list_append(sd->parts, gp);
     }
   evas_event_thaw(sd->e);
   edje_edit_string_list_free(parts_list);
   _move_border_to_top(sd);
}

void
_parts_list_free(Ws_Groupedit_Smart_Data *sd)
{
   Groupedit_Part *gp;

   assert(sd != NULL);

   if (!sd->parts) return;

   EINA_LIST_FREE(sd->parts, gp)
     _groupedit_part_free(gp);
   sd->parts = NULL;
}

static void
_groupedit_part_free(Groupedit_Part *gp)
{
   Groupedit_Item *ge_item = NULL;
   /* just in case */
   Evas_Object *image = NULL;

   assert(gp != NULL);

   image = edje_object_part_swallow_get(gp->draw, "swallow.image");
   evas_object_del(image);
   evas_object_del(gp->draw);
   eina_stringshare_del(gp->name);
   evas_object_smart_member_del(gp->border);
   evas_object_del(gp->border);
   if (gp->item)
     {
        evas_object_smart_member_del(gp->item);
        evas_object_del(gp->item);
     }
   if (gp->bg)
     evas_object_del(gp->bg);

   EINA_LIST_FREE(gp->items, ge_item)
     _item_draw_del(ge_item);
   EINA_LIST_FREE(gp->fake_items, ge_item)
     {
        evas_object_del(ge_item->border);
        evas_object_del(ge_item->highlight);
        free(ge_item);
     }
   free(gp);
}

Groupedit_Part *
_parts_list_find(Eina_List *parts, const char *part)
{
   Eina_List *l;
   Groupedit_Part *gp;

   assert(parts != NULL);
   assert(part != NULL);

   EINA_LIST_FOREACH(parts, l, gp)
     {
        if ((gp->name == part) || (!strcmp(part, gp->name)))
          return gp;
     }
   return NULL;
}

void
_selected_item_return_to_place(Ws_Groupedit_Smart_Data *sd)
{
   Groupedit_Part *gp;
   Eina_List *l, *ln;
   Eina_Bool is_below = false;

   assert(sd != NULL);

   if (!sd->selected) return;
   l = eina_list_data_find_list(sd->parts, sd->selected);

   ln = eina_list_prev(l);
   if (!ln)
     {
        ln = eina_list_next(l);
        if (!ln) return;
        is_below = true;
     }

   gp = (Groupedit_Part *)eina_list_data_get(ln);
   _part_parts_layouts_update(sd, sd->selected, gp, is_below);
   DBG("Separete mode, return to place part %s. Restack %s the part %s",
       (is_below) ? "belove" : "above", sd->selected->name, gp->name);

   evas_object_hide(sd->clipper);
   evas_object_clip_unset(sd->bg);
   evas_object_hide(sd->bg);
   edje_object_signal_emit(sd->selected->item, "item,unselected", "eflete");
}

void
_select_item_move_to_top(Ws_Groupedit_Smart_Data *sd)
{
   assert(sd != NULL);

   int x, y, w, h;
   if (sd->selected == sd->to_select) return;
   if (sd->selected) _selected_item_return_to_place(sd);

   evas_object_geometry_get(sd->to_select->item, &x, &y, &w, &h);
   evas_object_resize(sd->clipper, w, h);
   evas_object_move(sd->clipper, x, y);

   DBG("clipper geometry is %i %i %i %i", x, y, w, h);

   evas_object_show(sd->bg);
   evas_object_show(sd->clipper);
   evas_object_clip_set(sd->bg, sd->clipper);

   evas_object_raise(sd->bg);
   evas_object_raise(sd->clipper);
   evas_object_raise(sd->to_select->bg);
   evas_object_raise(sd->to_select->draw);
   evas_object_raise(sd->to_select->border);
   evas_object_raise(sd->to_select->item);
   sd->selected = sd->to_select;
   edje_object_signal_emit(sd->selected->item, "item,selected", "eflete");
   evas_object_geometry_get(sd->bg, &x, &y, &w, &h);
   DBG("bg geometry is %i %i %i %i", x, y, w, h);

   sd->to_select = NULL;
}

static void
_part_separete_mod_mouse_click_cb(void *data,
                                  Evas *e __UNUSED__,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;

   assert(gp != NULL);

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->border, "sd");

   assert(sd != NULL);

   Evas_Event_Mouse_Down *emd = (Evas_Event_Mouse_Down *)event_info;

   if (emd->button != 1) return;
   sd->to_select = gp;
   evas_object_smart_callback_call(sd->obj, SIG_PART_SELECTED,
                                  (void *)gp->name);
}

static void
_part_separete_mod_mouse_in_cb(void *data,
                               Evas *e __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;

   assert(gp != NULL);

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->border, "sd");

   assert(sd != NULL);

   if (sd->selected == gp) return;
   edje_object_signal_emit(gp->item, "item,mouse,in", "eflete");
}

static void
_part_separete_mod_mouse_out_cb(void *data,
                                Evas *e __UNUSED__,
                                Evas_Object *obj __UNUSED__,
                                void *event_info __UNUSED__)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;

   assert(gp != NULL);

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->border, "sd");

   assert(sd != NULL);

   if (sd->selected == gp) return;
   edje_object_signal_emit(gp->item, "item,mouse,out", "eflete");
}

static void
_part_select(void *data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;

   assert(gp != NULL);

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->border, "sd");

   assert(sd != NULL);

   Evas_Event_Mouse_Down *emd = (Evas_Event_Mouse_Down *)event_info;

   if (emd->button != 1) return;
   sd->to_select = gp;
   evas_object_smart_callback_call(sd->obj, SIG_PART_SELECTED,
                                  (void *)gp->name);
}

#define GP_GEOMETRY_SET \
   if (gp->bg) \
     { \
        evas_object_resize(gp->bg, w * sd->zoom_factor, h * sd->zoom_factor); \
        evas_object_move(gp->bg, x * sd->zoom_factor + xe + offset_x, \
                                y * sd->zoom_factor + ye + offset_y); \
     } \
   evas_object_resize(gp->border, w * sd->zoom_factor, h * sd->zoom_factor); \
   evas_object_move(gp->border, x * sd->zoom_factor + xe + offset_x, \
                                y * sd->zoom_factor + ye + offset_y); \
   evas_object_resize(gp->item, sd->con_current_size->w, sd->con_current_size->h); \
   evas_object_move(gp->item, xe + offset_x, ye + offset_y);

#define GP_REAL_GEOMETRY_CALC(PART_X, PART_Y, ABS_X, ABS_Y) \
   w *= sd->zoom_factor; h *= sd->zoom_factor; \
   PART_X = x * sd->zoom_factor + xe + offset_x; \
   PART_Y = y * sd->zoom_factor + ye + offset_y; \
   ABS_X = abs(sd->real_size->x - PART_X); \
   ABS_Y = abs(sd->real_size->y - PART_Y); \
   if (PART_X > sd->real_size->x) \
     w += ABS_X; \
   else \
     { \
        sd->real_size->w += ABS_X; \
        sd->real_size->x = PART_X; \
     } \
   if (PART_Y > sd->real_size->y) \
     h += ABS_Y; \
   else \
     { \
        sd->real_size->h += ABS_Y; \
        sd->real_size->y = PART_Y; \
     } \
   if (sd->real_size->w < w) \
     sd->real_size->w = w; \
   if (sd->real_size->h < h) \
     sd->real_size->h = h;

#define ZOOM_APPLY(OBJECT) \
   if (fabs(sd->zoom_factor - 1.0) > 0.00001) \
     { \
        evas_object_geometry_get(OBJECT, &x, &y, NULL, NULL); \
        Evas_Map *m = evas_map_new(4); \
        evas_map_smooth_set(m, false); \
        evas_map_util_points_populate_from_object_full(m, OBJECT, 0); \
        evas_map_util_zoom(m, sd->zoom_factor, sd->zoom_factor, x, y); \
        evas_object_map_set(OBJECT, m); \
        evas_object_map_enable_set(OBJECT, true); \
        evas_map_free(m); \
        evas_object_repeat_events_set(OBJECT, true); \
     } \
   else \
     { \
        evas_object_map_enable_set(OBJECT, false); \
        evas_object_repeat_events_set(OBJECT, false); \
     }

static void
_part_text_recalc_apply(Ws_Groupedit_Smart_Data *sd,
                        Groupedit_Part *gp,
                        int offset_x,
                        int offset_y)
{
   Evas_Coord x, y, xe, ye, ro_x, ro_y;
   Evas_Coord part_x, part_y, abs_x, abs_y;
   int w, h, ro_w, ro_h;
   const Evas_Object *ro;

   assert(sd != NULL);
   assert(gp != NULL);

   edje_object_part_geometry_get(sd->edit_obj, gp->name, &x, &y, &w, &h);
   evas_object_geometry_get(sd->edit_obj, &xe, &ye, NULL, NULL);
   ro = edje_object_part_object_get(sd->edit_obj, gp->name);
   evas_object_geometry_get(ro, &ro_x, &ro_y, &ro_w, &ro_h);

   /* all of it because of text and textblock >:C */
   ro_x = xe + offset_x + (ro_x - xe - offset_x) * sd->zoom_factor;
   ro_y = ye + offset_y + (ro_y - ye - offset_y) * sd->zoom_factor;

   evas_object_resize(gp->draw, ro_w, ro_h);
   evas_object_move(gp->draw, ro_x + offset_x, ro_y + offset_y);

   GP_GEOMETRY_SET

   GP_REAL_GEOMETRY_CALC(part_x, part_y, abs_x, abs_y)

   ZOOM_APPLY(gp->draw)
}

static void
_part_recalc_apply(Ws_Groupedit_Smart_Data *sd,
                   Groupedit_Part *gp,
                   int offset_x,
                   int offset_y)
{
   Evas_Coord x, y, xe, ye, w, h;
   Evas_Coord part_x, part_y, abs_x, abs_y;
   Eina_List *l, *l_n, *l_sp, *l_sp_n;
   Groupedit_Item *ge_item = NULL, *sp_item = NULL;
   Edje_Part_Type type;

   assert(sd != NULL);
   assert(gp != NULL);

   edje_object_part_geometry_get(sd->edit_obj, gp->name, &x, &y, &w, &h);
   evas_object_geometry_get(sd->edit_obj, &xe, &ye, NULL, NULL);

   evas_object_resize(gp->draw, w, h);
   evas_object_move(gp->draw, (x * sd->zoom_factor + xe + offset_x),
                              (y * sd->zoom_factor + ye + offset_y));


   GP_GEOMETRY_SET

   GP_REAL_GEOMETRY_CALC(part_x, part_y, abs_x, abs_y)

   type = edje_edit_part_type_get(sd->edit_obj, gp->name);
   if (type == EDJE_PART_TYPE_BOX)
     evas_object_smart_calculate(gp->draw);

   /* We don't need xe or ye for box items */
   xe = 0;
   ye = 0;

   if (gp->items)
     {
        EINA_LIST_FOREACH_SAFE(gp->items, l, l_n, ge_item)
          {
             evas_object_geometry_get(ge_item->border, &x, &y, &w, &h);
             GP_REAL_GEOMETRY_CALC(part_x, part_y, abs_x, abs_y)

             if (ge_item->spread)
               {
                  EINA_LIST_FOREACH_SAFE(ge_item->spread, l_sp, l_sp_n, sp_item)
                    {
                       /* If it is BOX then there are borders exists.
                        * If border is not exist then it is TABLE's item.
                        */
                       if (type == EDJE_PART_TYPE_BOX)
                         evas_object_geometry_get(sp_item->border, &x, &y, &w, &h);
                       else
                         {
                            evas_object_geometry_get(sp_item->draw, &x, &y, &w, &h);
                            evas_object_smart_calculate(sp_item->draw);
                         }

                       GP_REAL_GEOMETRY_CALC(part_x, part_y, abs_x, abs_y)
                    }
               }
          }
     }

   ZOOM_APPLY(gp->draw)
}

#undef GP_GEOMETRY_SET
#undef GP_REAL_GEOMETRY_CALC
#undef ZOOM_APPLY

Eina_Bool
_parts_recalc(Ws_Groupedit_Smart_Data *sd)
{
   Eina_List *l;
   Groupedit_Part *gp;
   Edje_Part_Type ept;
   int i = 0, offset_x, offset_y;

   assert(sd != NULL);

   if (!sd->parts) return false;

   evas_object_geometry_get(sd->obj, &sd->real_size->x,
                                     &sd->real_size->y,
                                     &sd->real_size->w,
                                     &sd->real_size->h);
   DBG("Recalc %p object. Object parts count: %d", sd->obj, eina_list_count(sd->parts))

   EINA_LIST_FOREACH(sd->parts, l, gp)
     {
        offset_x = i * SEP_ITEM_PAD_X;
        offset_y = i * SEP_ITEM_PAD_Y;

        if (sd->separated)
          {
             i++;
             if (!gp->item)
               {
                  gp->item = edje_object_add(sd->e);
                  if (!edje_object_file_set(gp->item, EFLETE_EDJ, "eflete/group/item/default"))
                    ERR("Could not load style of part's item!");
                  evas_object_event_callback_add(gp->item, EVAS_CALLBACK_MOUSE_DOWN,
                                                 _part_separete_mod_mouse_click_cb, gp);
                  evas_object_event_callback_add(gp->item, EVAS_CALLBACK_MOUSE_IN,
                                                 _part_separete_mod_mouse_in_cb, gp);
                  evas_object_event_callback_add(gp->item, EVAS_CALLBACK_MOUSE_OUT,
                                                 _part_separete_mod_mouse_out_cb, gp);
                  evas_object_smart_member_add(gp->item, sd->obj);
                  evas_object_show(gp->item);
               }
          }
        else
          {
             if (gp->item)
               {
                  evas_object_hide(gp->item);
                  evas_object_event_callback_del_full(gp->item, EVAS_CALLBACK_MOUSE_DOWN,
                                                      _part_separete_mod_mouse_click_cb, gp);
                  evas_object_event_callback_del_full(gp->item, EVAS_CALLBACK_MOUSE_IN,
                                                      _part_separete_mod_mouse_in_cb, gp);
                  evas_object_event_callback_del_full(gp->item, EVAS_CALLBACK_MOUSE_OUT,
                                                      _part_separete_mod_mouse_out_cb, gp);

                  evas_object_smart_member_del(gp->item);
                  evas_object_del(gp->item);
                  gp->item = NULL;
               }
          }

        ept = edje_edit_part_type_get(sd->edit_obj, gp->name);
        switch (ept)
          {
           case EDJE_PART_TYPE_RECTANGLE:
              _rectangle_param_update(gp, sd->edit_obj);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_TEXT:
              _text_param_update(gp, sd->edit_obj);
              _part_text_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_IMAGE:
              _image_param_update(gp, sd->edit_obj, sd->edit_obj_file);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_PROXY: // it part like image
              _proxy_param_update(gp, sd->edit_obj);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_TEXTBLOCK:
              _textblock_param_update(gp, sd->edit_obj);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_GROUP:
              _group_param_update(gp, sd->edit_obj, sd->edit_obj_file, sd->obj);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_TABLE:
              _table_param_update(sd, gp);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_BOX:
              _box_param_update(sd, gp);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;
           case EDJE_PART_TYPE_SPACER:
           case EDJE_PART_TYPE_SWALLOW:
           case EDJE_PART_TYPE_EXTERNAL:
              _part_recalc_apply(sd, gp, offset_x, offset_y);
           default:
              break;
          }
        if (gp->visible)
          {
             evas_object_show(gp->bg);
             evas_object_show(gp->draw);
             evas_object_show(gp->border);
          }
        else
          {
             evas_object_hide(gp->bg);
             evas_object_hide(gp->draw);
             evas_object_hide(gp->border);
          }
     }
     if (!sd->separated) _part_object_area_calc(sd);
     else
       {
          if (sd->to_select) _select_item_move_to_top(sd);
          evas_object_hide(sd->obj_area.obj);
          evas_object_smart_callback_call(sd->obj, SIG_OBJ_AREA_CHANGED, sd->obj_area.geom);
       }

   evas_object_smart_callback_call(sd->obj, SIG_GEOMETRY_CHANGED, (void *)sd->real_size);

   return true;
}
#define BORDER_ADD(R, G, B, A) \
   IMAGE_ADD_NEW(sd->obj, gp->border, "border", "2px")\
   evas_object_color_set(gp->border, R*A/255, G*A/255, B*A/255, A);

#define IMAGE_PART_GROUP "eflete/groupedit/image/default"

static void
_image_delete(void *data __UNUSED__,
              Evas *e __UNUSED__,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   assert(obj != NULL);

   Evas_Object *image = edje_object_part_swallow_get(obj, "swallow.image");
   if (image) evas_object_del(image);
}

static Groupedit_Part *
_part_draw_add(Ws_Groupedit_Smart_Data *sd, const char *part, Edje_Part_Type type)
{
   Evas_Object *o;
   Groupedit_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = mem_calloc(1, sizeof(Groupedit_Part));
   gp->name = eina_stringshare_add(part);
   gp->visible = true;
   gp->border = NULL;
   gp->item = NULL;

   switch (type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
         gp->draw = evas_object_rectangle_add(sd->e);
         BORDER_ADD(0, 0, 0, 0)
         break;
      case EDJE_PART_TYPE_TEXT:
         gp->draw = evas_object_text_add(sd->e);
         BORDER_ADD(122, 122, 122, 255)
         break;
      case EDJE_PART_TYPE_IMAGE:
         gp->draw = edje_object_add(sd->e);
         if (!edje_object_file_set(gp->draw, EFLETE_EDJ, IMAGE_PART_GROUP))
           ERR("Image can't be loaded.\n");
         evas_object_event_callback_add(gp->draw, EVAS_CALLBACK_DEL,
                                        _image_delete, NULL);
         o = evas_object_image_add(sd->e);
         edje_object_part_swallow(gp->draw, "swallow.image", o);
         BORDER_ADD(0, 0, 0, 0)
         break;
      case EDJE_PART_TYPE_PROXY: // it part like image
         gp->draw = edje_object_add(sd->e);
         if (!edje_object_file_set(gp->draw, EFLETE_EDJ, IMAGE_PART_GROUP))
           ERR("Image can't be loaded.\n");
         evas_object_event_callback_add(gp->draw, EVAS_CALLBACK_DEL,
                                        _image_delete, NULL);
         IMAGE_ADD_NEW(sd->obj, o, "bg", "proxy")
         edje_object_part_swallow(gp->draw, "swallow.image", o);
         BORDER_ADD(255, 112, 49, 255);
         break;
      case EDJE_PART_TYPE_SWALLOW:
         IMAGE_ADD_NEW(sd->obj, gp->draw, "bg", "swallow")
         BORDER_ADD(120, 103, 140, 255)
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         gp->draw = evas_object_textblock_add(sd->e);
         BORDER_ADD(122, 122, 122, 255)
         break;
      case EDJE_PART_TYPE_SPACER:
         IMAGE_ADD_NEW(sd->obj, gp->draw, "bg", "spacer")
         BORDER_ADD(101, 117, 133, 255)
         break;
      case EDJE_PART_TYPE_GROUP:
         gp->draw = edje_object_add(sd->e);
         BORDER_ADD(255, 109, 109, 255)
         break;
      case EDJE_PART_TYPE_TABLE:
         IMAGE_ADD_NEW(sd->obj, gp->bg, "bg", "table")
         gp->draw = _part_container_add(sd, part, &(gp->items), type);
         BORDER_ADD(138, 125, 109, 255)
         break;
      case EDJE_PART_TYPE_BOX:
         IMAGE_ADD_NEW(sd->obj, gp->bg, "bg", "box")
         gp->draw = _part_container_add(sd, part, &(gp->items), type);
         BORDER_ADD(124, 129, 102, 255)
         break;
      case EDJE_PART_TYPE_EXTERNAL:
      default:
         /* Temporary solution for type parts, which not implemented yet.
          * Here created transparent rectangle as draw evas primitives.
          */
         TODO("add support for all part types.")
         gp->draw = evas_object_rectangle_add(sd->e);
         evas_object_color_set(gp->draw, 0, 0, 0, 0);
         break;
     }

   evas_object_data_set(gp->border, "sd", sd);
   evas_object_event_callback_add(gp->border, EVAS_CALLBACK_MOUSE_DOWN,
                                  _part_select, gp);

   if (gp->bg)
     evas_object_smart_member_add(gp->bg, sd->obj);
   evas_object_smart_member_add(gp->draw, sd->obj);
   evas_object_smart_member_add(gp->border, sd->obj);

   return gp;
}

#undef BORDER_ADD

static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, const char *part)
{
   Groupedit_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = _parts_list_find(sd->parts, part);

   assert(gp != NULL);

   sd->parts = eina_list_remove(sd->parts, gp);
   _groupedit_part_free(gp);
}

static Groupedit_Item *
_item_draw_add(Ws_Groupedit_Smart_Data *sd, Eina_Stringshare *part,
               Eina_Stringshare *item)
{
   Groupedit_Item *ge_item = NULL;
   Eina_Stringshare *item_source;

   assert(sd != NULL);
   assert(part != NULL);
   assert(item != NULL);

   item_source = edje_edit_part_item_source_get(sd->edit_obj, part, item);
   if (item_source)
     {
        ge_item = (Groupedit_Item *)mem_calloc(1, sizeof(Groupedit_Item));
        ge_item->name = eina_stringshare_add(item);

        IMAGE_ADD_NEW(sd->obj, ge_item->border, "bg", "part_item")
        evas_object_show(ge_item->border);
        evas_object_size_hint_min_set(ge_item->border,    EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_align_set(ge_item->border,  EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_weight_set(ge_item->border, EVAS_HINT_FILL, EVAS_HINT_FILL);

        ge_item->draw = edje_object_add(sd->e);
        edje_object_file_set(ge_item->draw, sd->edit_obj_file, item_source);
        evas_object_show(ge_item->draw);

        IMAGE_ADD_NEW(sd->obj, ge_item->highlight, "border", "1px");
        evas_object_size_hint_min_set(ge_item->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_align_set(ge_item->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_weight_set(ge_item->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_color_set(ge_item->highlight, 49, 140, 141, 255);
        evas_object_show(ge_item->highlight);
     }
   edje_edit_string_free(item_source);
   return ge_item;
}

static void
_item_draw_del(Groupedit_Item *ge_item)
{
   Groupedit_Item *spread_item = NULL;

   assert(ge_item != NULL);

   EINA_LIST_FREE(ge_item->spread, spread_item)
     {
        evas_object_del(spread_item->draw);
        if (spread_item->highlight)
          evas_object_del(spread_item->highlight);
        if (spread_item->border)
          evas_object_del(spread_item->border);
        free(spread_item);
     }
   eina_stringshare_del(ge_item->name);
   evas_object_del(ge_item->draw);
   evas_object_del(ge_item->border);
   evas_object_del(ge_item->highlight);
   free(ge_item);
}

static void
_move_border_to_top(Ws_Groupedit_Smart_Data *sd)
{
   assert(sd != NULL);

   evas_object_smart_member_del(sd->container);
   evas_object_smart_member_del(sd->handler_TL.obj);
   evas_object_smart_member_del(sd->handler_BR.obj);

   evas_object_smart_member_add(sd->container, sd->obj);
   evas_object_smart_member_add(sd->handler_TL.obj, sd->obj);
   evas_object_smart_member_add(sd->handler_BR.obj, sd->obj);
}

static Evas_Object *
_part_container_add(Ws_Groupedit_Smart_Data *sd, Eina_Stringshare *part, Eina_List **items, Edje_Part_Type type)
{
   Evas_Object *container;
   Eina_List *items_names = NULL, *l_items = NULL, *l_n_items = NULL;
   Eina_Stringshare *item_name = NULL;
   Groupedit_Item *ge_item = NULL;

   assert(sd != NULL);
   assert(part != NULL);

   if (type == EDJE_PART_TYPE_BOX)
     container = evas_object_box_add(sd->e);
   else if (type == EDJE_PART_TYPE_TABLE)
     container = evas_object_table_add(sd->e);
   else
     return NULL;
   evas_object_show(container);

   items_names = edje_edit_part_items_list_get(sd->edit_obj, part);
   EINA_LIST_FOREACH_SAFE(items_names, l_items, l_n_items, item_name)
     {
        ge_item = _item_draw_add(sd, part, item_name);
        *items = eina_list_append(*items, ge_item);
     }

   edje_edit_string_list_free(items_names);
   evas_object_smart_calculate(container);

   return container;
}

static inline void
_colors_get(Groupedit_Part *gp, Evas_Object *edit_obj, const char *state, double value,
            int *r, int *g, int *b, int *a,
            int *r2, int *g2, int *b2, int *a2,
            int *r3, int *g3, int *b3, int *a3)
{
   int cr, cg, cb, ca;
   int cr2, cg2, cb2, ca2;
   int cr3, cg3, cb3, ca3;
   Eina_Stringshare *color_class;

   assert(gp != NULL);
   assert(edit_obj != NULL);
   assert(state != NULL);

   if (r || g || b || a)
     edje_edit_state_color_get(edit_obj, gp->name, state, value, r, g, b, a);
   if (r2 || g2 || b2 || a2)
     edje_edit_state_color2_get(edit_obj, gp->name, state, value, r2, g2, b2, a2);
   if (r3 || g3 || b3 || a3)
     edje_edit_state_color3_get(edit_obj, gp->name, state, value, r3, g3, b3, a3);

   color_class = edje_edit_state_color_class_get(edit_obj, gp->name, state, value);
   if (color_class)
     {
        if (edje_edit_color_class_colors_get(edit_obj, color_class, &cr, &cg, &cb, &ca,
                                             &cr2, &cg2, &cb2, &ca2,
                                             &cr3, &cg3, &cb3, &ca3))
          {
             if (r) *r = ((cr + 1) * *r) >> 8;
             if (g) *g = ((cg + 1) * *g) >> 8;
             if (b) *b = ((cb + 1) * *b) >> 8;
             if (a) *a = ((ca + 1) * *a) >> 8;

             if (r2) *r2 = ((cr2 + 1) * *r2) >> 8;
             if (g2) *g2 = ((cg2 + 1) * *g2) >> 8;
             if (b2) *b2 = ((cb2 + 1) * *b2) >> 8;
             if (a2) *a2 = ((ca2 + 1) * *a2) >> 8;

             if (r3) *r3 = ((cr3 + 1) * *r3) >> 8;
             if (g3) *g3 = ((cg3 + 1) * *g3) >> 8;
             if (b3) *b3 = ((cb3 + 1) * *b3) >> 8;
             if (a3) *a3 = ((ca3 + 1) * *a3) >> 8;
          }
        eina_stringshare_del(color_class);
     }

   if (a) evas_color_argb_premul(*a, r, g, b);
   if (a2) evas_color_argb_premul(*a2, r2, g2, b2);
   if (a3) evas_color_argb_premul(*a3, r3, g3, b3);
}

static inline void
_color_apply(Groupedit_Part *gp, Evas_Object *edit_obj, const char *state, double value)
{
   int r, g, b, a;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;

   assert(gp != NULL);
   assert(edit_obj != NULL);
   assert(state != NULL);

   Edje_Part_Type ept = edje_edit_part_type_get(edit_obj, gp->name);
   if (ept == EDJE_PART_TYPE_TEXT || ept == EDJE_PART_TYPE_TEXTBLOCK)
     _colors_get(gp, edit_obj, state, value, &r, &g, &b, &a, &r2, &g2, &b2, &a2, &r3, &g3, &b3, &a3);
   else
     _colors_get(gp, edit_obj, state, value, &r, &g, &b, &a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

   evas_object_color_set(gp->draw, r, g, b, a);
   if (ept == EDJE_PART_TYPE_TEXT || ept == EDJE_PART_TYPE_TEXTBLOCK)
     {
        evas_object_text_shadow_color_set(gp->draw, r2, g2, b2, a2);
        evas_object_text_outline_color_set(gp->draw, r3, g3, b3, a3);
        evas_object_text_glow_color_set(gp->draw, r2, g2, b2, a2);
        evas_object_text_glow2_color_set(gp->draw, r3, g3, b3, a3);
     }
}
static void
_rectangle_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   PART_STATE_GET(edit_obj, gp->name);

   assert(gp != NULL);
   assert(edit_obj != NULL);

   _color_apply(gp, edit_obj, state, value);

   PART_STATE_FREE
}

static void
_image_proxy_common_param_update(Evas_Object *image, Groupedit_Part *gp, Evas_Object *edit_obj)
{
   int x, y, w, h;
   int img_w, img_h;
   double fill_w, fill_h, fill_x, fill_y;
   int fill_origin_offset_x, fill_origin_offset_y, fill_size_offset_x, fill_size_offset_y;

   assert(image != NULL);
   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->name)

   _color_apply(gp, edit_obj, state, value);

   /* setups settings from filled block  into evas image object*/
   evas_object_image_smooth_scale_set(image,
            edje_edit_state_fill_smooth_get(edit_obj, gp->name, state, value));

   /* take fill params here, because need to know is default params set*/
   fill_x = edje_edit_state_fill_origin_relative_x_get(edit_obj, gp->name, state, value);
   fill_y = edje_edit_state_fill_origin_relative_y_get(edit_obj, gp->name, state, value);
   fill_w = edje_edit_state_fill_size_relative_x_get(edit_obj, gp->name, state, value);
   fill_h = edje_edit_state_fill_size_relative_y_get(edit_obj, gp->name, state, value);
   fill_origin_offset_x = edje_edit_state_fill_origin_offset_x_get(edit_obj, gp->name, state, value);
   fill_origin_offset_y = edje_edit_state_fill_origin_offset_y_get(edit_obj, gp->name, state, value);
   fill_size_offset_x = edje_edit_state_fill_size_offset_x_get(edit_obj, gp->name, state, value);
   fill_size_offset_y = edje_edit_state_fill_size_offset_y_get(edit_obj, gp->name, state, value);
   if (edje_edit_state_fill_type_get(edit_obj, gp->name, state, value))
     {
        /* If image tiled, set fill param to evas image object */
        evas_object_image_size_get(image, &img_w, &img_h);
        x = (int)(img_w * fill_x) + fill_origin_offset_x;
        y = (int)(img_h * fill_y) + fill_origin_offset_y;
        w = (int)(img_w * fill_w) + fill_size_offset_x;
        h = (int)(img_h * fill_h) + fill_size_offset_y;
        evas_object_image_filled_set(image, false);
        evas_object_image_fill_set(image, x, y, w, h);
     }
   else if (fill_x || fill_y || (fill_w != 1) || (fill_h != 1) ||
            fill_origin_offset_x || fill_origin_offset_y ||
            fill_size_offset_x || fill_size_offset_y)
     {
        /* If image fill is scale and params are non default values, set
           this params to evas image object */
        evas_object_geometry_get(image, NULL, NULL, &img_w, &img_h);
        x = (int)(img_w * fill_x) + fill_origin_offset_x;
        y = (int)(img_h * fill_y) + fill_origin_offset_y;
        w = (int)(img_w * fill_w) + fill_size_offset_x;
        h = (int)(img_h * fill_h) + fill_size_offset_y;
        evas_object_image_filled_set(image, false);
        evas_object_image_fill_set(image, x, y, w, h);
     }
   else
     /* If image fill is scale with default params. */
     evas_object_image_filled_set(image, true);
}

static void
_image_param_update(Groupedit_Part *gp, Evas_Object *edit_obj, const char *file)
{
   Evas_Load_Error err;
   Evas_Object *image;
   const char *image_normal;
   const char *buf = NULL;
   int id;
   int bl, br, bt, bb;
   unsigned char middle;

   assert(gp != NULL);
   assert(edit_obj != NULL);
   assert(file != NULL);

   PART_STATE_GET(edit_obj, gp->name)

   image = edje_object_part_swallow_get(gp->draw, "swallow.image");

   image_normal = edje_edit_state_image_get(edit_obj, gp->name, state, value);
   if (!image_normal) return;
   id = edje_edit_image_id_get(edit_obj, image_normal);
   buf = eina_stringshare_printf("edje/images/%i", id);
   evas_object_image_file_set(image, file, buf);
   err = evas_object_image_load_error_get(image);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN("Could not update image:\"%s\"\n",  evas_load_error_str(err));
   edje_edit_string_free(image_normal);

   edje_edit_state_image_border_get(edit_obj, gp->name, state, value,
                                    &bl, &br, &bt, &bb);
   evas_object_image_border_set(image, bl, br, bt, bb);

   middle  = edje_edit_state_image_border_fill_get(edit_obj, gp->name, state, value);
   if (middle == 0)
     evas_object_image_border_center_fill_set(image, EVAS_BORDER_FILL_NONE);
   else if (middle == 1)
     evas_object_image_border_center_fill_set(image, EVAS_BORDER_FILL_DEFAULT);
   else if (middle == 2)
     evas_object_image_border_center_fill_set(image, EVAS_BORDER_FILL_SOLID);

   _image_proxy_common_param_update(image, gp, edit_obj);

   PART_STATE_FREE
   eina_stringshare_del(buf);
}

static void
_proxy_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   Evas_Object *image, *source_image = NULL;
   const char *proxy_source;
   Ws_Groupedit_Smart_Data *sd;
   Groupedit_Part *source;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->name)

   image = edje_object_part_swallow_get(gp->draw, "swallow.image");
   evas_object_del(image);

   sd = evas_object_data_get(gp->border, "sd");
   proxy_source = edje_edit_state_proxy_source_get(edit_obj, gp->name, state, value);
   if (proxy_source)
     {
        source = _parts_list_find(sd->parts, proxy_source);
        source_image = edje_object_part_swallow_get(source->draw, "swallow.image");
        if (!source_image)
          source_image = source->draw;
        image = evas_object_image_add(sd->e);
        edje_object_part_swallow(gp->draw, "swallow.image", image);
        evas_object_image_source_set(image, source_image);
        evas_object_image_source_clip_set(image, false);

        _image_proxy_common_param_update(image, gp, edit_obj);
     }
   else
     {
         IMAGE_ADD_NEW(sd->obj, image, "bg", "proxy")
         edje_object_part_swallow(gp->draw, "swallow.image", image);
     }
}

static void
_text_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   const char *font, *text;
   int text_size;
   double elipsis;
   Evas_Text_Style_Type style;
   Edje_Text_Effect effect;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->name)

   _color_apply(gp, edit_obj, state, value);

   font = edje_edit_state_font_get(edit_obj, gp->name, state, value);
   text_size = edje_edit_state_text_size_get(edit_obj, gp->name, state, value);
   evas_object_text_font_set(gp->draw, font, text_size);

   text = edje_edit_state_text_get(edit_obj, gp->name, state, value);
   if (text) evas_object_text_text_set(gp->draw, text);
   else
     {
         edje_object_part_text_set(edit_obj, gp->name, gp->name);
         evas_object_text_text_set(gp->draw, gp->name);
     }

   elipsis = edje_edit_state_text_elipsis_get(edit_obj, gp->name, state, value);
   evas_object_text_ellipsis_set(gp->draw, elipsis);

   effect = edje_edit_part_effect_get(edit_obj, gp->name);
   switch (effect & EDJE_TEXT_EFFECT_MASK_BASIC)
     {
      case EDJE_TEXT_EFFECT_NONE:
      case EDJE_TEXT_EFFECT_PLAIN:
         style = EVAS_TEXT_STYLE_PLAIN;
         break;
      case EDJE_TEXT_EFFECT_OUTLINE:
         style = EVAS_TEXT_STYLE_OUTLINE;
         break;
      case EDJE_TEXT_EFFECT_SOFT_OUTLINE:
         style = EVAS_TEXT_STYLE_SOFT_OUTLINE;
         break;
      case EDJE_TEXT_EFFECT_SHADOW:
         style = EVAS_TEXT_STYLE_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_SOFT_SHADOW:
         style = EVAS_TEXT_STYLE_SOFT_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_OUTLINE_SHADOW:
         style = EVAS_TEXT_STYLE_OUTLINE_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_OUTLINE_SOFT_SHADOW:
         style = EVAS_TEXT_STYLE_OUTLINE_SOFT_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_FAR_SHADOW:
         style = EVAS_TEXT_STYLE_FAR_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_FAR_SOFT_SHADOW:
         style = EVAS_TEXT_STYLE_FAR_SOFT_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_GLOW:
         style = EVAS_TEXT_STYLE_GLOW;
         break;
      default:
         style = EVAS_TEXT_STYLE_PLAIN;
         break;
     }

   switch (effect & EDJE_TEXT_EFFECT_MASK_SHADOW_DIRECTION)
     {
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM_RIGHT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_BOTTOM_RIGHT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_BOTTOM);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM_LEFT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_BOTTOM_LEFT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_LEFT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_LEFT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_TOP_LEFT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_TOP_LEFT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_TOP:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_TOP);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_TOP_RIGHT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_TOP_RIGHT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_RIGHT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_RIGHT);
         break;
      default:
         break;
     }
   evas_object_text_style_set(gp->draw, style);
   PART_STATE_FREE
   edje_edit_string_free(font);
   edje_edit_string_free(text);
}

static void
_textblock_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   Evas_Textblock_Style *ts = NULL;
   const Evas_Textblock_Style *obj_style;
   const Evas_Object *get_style;
   const char *text;
   double valign;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->name)

   _color_apply(gp, edit_obj, state, value);

   get_style = edje_object_part_object_get(edit_obj, gp->name);
   obj_style = evas_object_textblock_style_get(get_style);
   text = evas_textblock_style_get(obj_style);

   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, text);
   evas_object_textblock_style_set(gp->draw, ts);
   evas_textblock_style_free(ts);
   valign = edje_edit_state_text_align_y_get(edit_obj, gp->name, state, value);
   evas_object_textblock_valign_set(gp->draw, valign);

   text = edje_edit_state_text_get(edit_obj, gp->name, state, value);
   if (text) evas_object_textblock_text_markup_set(gp->draw, text);
   else
     {
        edje_object_part_text_set(edit_obj, gp->name, gp->name);
        evas_object_textblock_text_markup_set(gp->draw, gp->name);
     }

   PART_STATE_FREE
   edje_edit_string_free(text);
}

static void
_group_param_update(Groupedit_Part *gp, Evas_Object *edit_obj, const char *file, Evas_Object *parent)
{
   Eina_Stringshare *source = edje_edit_part_source_get(edit_obj, gp->name);
   Evas_Object *image = NULL;

   assert(gp != NULL);
   assert(edit_obj != NULL);
   assert(file != NULL);
   assert(parent != NULL);

   PART_STATE_GET(edit_obj, gp->name)

   image = edje_object_part_swallow_get(gp->draw, "swallow.image");
   evas_object_del(image);

   if (!source)
     {
        edje_object_file_set(gp->draw, EFLETE_EDJ, IMAGE_PART_GROUP);
        IMAGE_ADD_NEW(parent, image, "bg", "group");
        edje_object_part_swallow(gp->draw, "swallow.image", image);
     }
   else
     {
        _color_apply(gp, edit_obj, state, value);
        edje_object_file_set(gp->draw, file, source);
     }

   PART_STATE_FREE
   if (source) edje_edit_string_free(source);
}

static void
_table_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp)
{
   Groupedit_Item *ge_item = NULL, *spread_item = NULL;
   Eina_List *l_items, *l_n_items;
   Eina_Stringshare *part = gp->name;
   Eina_Stringshare *item_source = NULL;

   Evas_Object *child = NULL;
   int w, h; /* Geometry values */
   int min_w, max_w, prefer_w, min_h, max_h, prefer_h; /* Hints values */
   int spread_col, spread_row; /* Position values */

   short unsigned int pos_col = 0, pos_row = 0;
   unsigned char span_col = 0, span_row = 0, homogeneous = 0;
   Evas_Aspect_Control aspect = EVAS_ASPECT_CONTROL_NONE;
   int aspect_x, aspect_y;
   double align_x = 0, align_y = 0, weight_x = 0, weight_y = 0; /* Align object in cell */
   int pad_l = 0, pad_r = 0, pad_t = 0, pad_b = 0;
   int old_spread_row = 0, spread_pos_col = 0, spread_pos_row = 0;
   int r = 0, g = 0, b = 0, a = 0;

   /* fake_init - how much fake items, already created.
    * fake_count - how much fake items needed for filling empty cells.
    * fake_diff - deifference beetween fake_count and fake_init
    */
   int fake_init = 0, fake_count = 0, fake_diff = 0;
   Eina_List *fake_l;
   int i, k, col, row; /* counters for filling empty cells with fake items*/

   assert(sd != NULL);
   assert(gp != NULL);

   TODO("get TABLE attributes from edje object, after implementing functions"
        "in edje_edit libs. Until that time will be used default values.")
   PART_STATE_GET(sd->edit_obj, gp->name)
   homogeneous = edje_edit_state_table_homogeneous_get(sd->edit_obj, gp->name, state, value);
   evas_object_table_homogeneous_set(gp->draw, homogeneous);

   edje_edit_state_container_align_get(sd->edit_obj, gp->name, state, value, &align_x, &align_y);
   evas_object_table_align_set(gp->draw, align_x, align_y);

   edje_edit_state_container_padding_get(sd->edit_obj, gp->name, state, value, &pad_l, &pad_r);
   evas_object_table_padding_set(gp->draw, pad_l, pad_r);

   _colors_get(gp, sd->edit_obj, state, value, &r, &g, &b, &a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   PART_STATE_FREE

   /* Unpack all fake items, before pack real */
   fake_init = eina_list_count(gp->fake_items);
   EINA_LIST_FOREACH(gp->fake_items, fake_l, ge_item)
     {
        evas_object_table_unpack(gp->draw, ge_item->border);
        evas_object_table_unpack(gp->draw, ge_item->highlight);
     }

   EINA_LIST_FOREACH_SAFE(gp->items, l_items, l_n_items, ge_item)
     {
        item_source = edje_edit_part_item_source_get(sd->edit_obj, part, ge_item->name);
        edje_object_file_set(ge_item->draw, sd->edit_obj_file, item_source);

        edje_edit_part_item_position_get(sd->edit_obj, part, ge_item->name, &pos_col, &pos_row);
        edje_edit_part_item_span_get(sd->edit_obj, part, ge_item->name, &span_col, &span_row);

        min_w = edje_edit_part_item_min_w_get(sd->edit_obj, part, ge_item->name);
        min_h = edje_edit_part_item_min_h_get(sd->edit_obj, part, ge_item->name);

/* Calculation according to box/table item implementation in efl 1.13 at edje_load.c */
        if ((min_w <= 0) && (min_h <= 0))
          {
             edje_object_size_min_get(ge_item->draw, &w, &h);
             if ((w <= 0) && (h <= 0))
               edje_object_size_min_calc(ge_item->draw, &w, &h);
          }
        if (((min_w <= 0) && (min_h <= 0)) && ((w > 0) || (h > 0)))
          evas_object_size_hint_min_set(ge_item->draw, w, h);
        else
          {
             w = min_w;
             h = min_h;
             evas_object_size_hint_min_set(ge_item->draw, min_w, min_h);
          }
        max_w = edje_edit_part_item_max_w_get(sd->edit_obj, part, ge_item->name);
        max_h = edje_edit_part_item_max_h_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_max_set(ge_item->draw, max_w, max_h);
        prefer_w = edje_edit_part_item_prefer_w_get(sd->edit_obj, part, ge_item->name);
        prefer_h = edje_edit_part_item_prefer_h_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_request_set(ge_item->draw, prefer_w, prefer_h);
        edje_edit_part_item_padding_get(sd->edit_obj, part, ge_item->name, &pad_l, &pad_r, &pad_t, &pad_b);
        evas_object_size_hint_padding_set(ge_item->draw, pad_l, pad_r, pad_t, pad_b);
        align_x = edje_edit_part_item_align_x_get(sd->edit_obj, part, ge_item->name);
        align_y = edje_edit_part_item_align_y_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_align_set(ge_item->draw, align_x, align_y);
        weight_x = edje_edit_part_item_weight_x_get(sd->edit_obj, part, ge_item->name);
        weight_y = edje_edit_part_item_weight_y_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_weight_set(ge_item->draw, weight_x, weight_y);
        switch(edje_edit_part_item_aspect_mode_get(sd->edit_obj, part, ge_item->name))
          {
           case EDJE_ASPECT_CONTROL_NONE: aspect = EVAS_ASPECT_CONTROL_NONE; break;
           case EDJE_ASPECT_CONTROL_NEITHER: aspect = EVAS_ASPECT_CONTROL_NEITHER; break;
           case EDJE_ASPECT_CONTROL_HORIZONTAL: aspect = EVAS_ASPECT_CONTROL_HORIZONTAL; break;
           case EDJE_ASPECT_CONTROL_VERTICAL: aspect = EVAS_ASPECT_CONTROL_VERTICAL; break;
           case EDJE_ASPECT_CONTROL_BOTH: aspect = EVAS_ASPECT_CONTROL_BOTH; break;
          }
        aspect_x = edje_edit_part_item_aspect_w_get(sd->edit_obj, part, ge_item->name);
        aspect_y = edje_edit_part_item_aspect_h_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_aspect_set(ge_item->draw, aspect, aspect_x, aspect_y);
        evas_object_color_set(ge_item->draw, r, g, b, a);

        evas_object_table_pack(gp->draw, ge_item->border, pos_col,
                               pos_row, span_col, span_row);
        evas_object_table_pack(gp->draw, ge_item->draw, pos_col, pos_row, span_col, span_row);
        evas_object_table_pack(gp->draw, ge_item->highlight, pos_col,
                               pos_row, span_col, span_row);

        spread_col = edje_edit_part_item_spread_w_get(sd->edit_obj, part, ge_item->name);
        spread_row = edje_edit_part_item_spread_h_get(sd->edit_obj, part, ge_item->name);

        EINA_LIST_FREE(ge_item->spread, spread_item)
          {
             evas_object_table_unpack(gp->draw, spread_item->draw);
             evas_object_smart_member_del(spread_item->border);
             evas_object_smart_member_del(spread_item->draw);
             evas_object_smart_member_del(spread_item->highlight);
             evas_object_del(spread_item->border);
             evas_object_del(spread_item->draw);
             evas_object_del(spread_item->highlight);
             free(spread_item);
          }

        old_spread_row = spread_row;

        while((spread_col > 1) || (spread_row > 1))
          {
             spread_pos_col = pos_col + spread_col - 1 ;
             spread_pos_row = pos_row + spread_row - 1;

             spread_item = (Groupedit_Item *)mem_calloc(1, sizeof(Groupedit_Item));

             spread_item->draw = edje_object_add(sd->e);
             edje_object_file_set(spread_item->draw, sd->edit_obj_file, item_source);
             ge_item->spread = eina_list_append(ge_item->spread, spread_item);
             evas_object_size_hint_max_set(spread_item->draw, max_w, max_h);
             evas_object_size_hint_request_set(spread_item->draw, prefer_w, prefer_h);
             evas_object_size_hint_min_set(spread_item->draw, min_w, min_h);
             evas_object_size_hint_padding_set(spread_item->draw, pad_l, pad_r, pad_t, pad_b);
             evas_object_size_hint_align_set(spread_item->draw, align_x, align_y);
             evas_object_size_hint_weight_set(spread_item->draw, weight_x, weight_y);
             evas_object_size_hint_aspect_set(spread_item->draw, aspect, aspect_x, aspect_y);

             evas_object_table_pack(gp->draw, spread_item->draw, spread_pos_col,
                                    spread_pos_row, span_col, span_row);


             evas_object_show(spread_item->draw);
             evas_object_smart_member_add(spread_item->draw, gp->draw);
             if (spread_row > 1) spread_row--;
             else if (spread_col > 1) {spread_col--; spread_row = old_spread_row; }
             evas_object_color_set(spread_item->draw, r, g, b, a);
          }

        evas_object_smart_member_add(ge_item->border, gp->draw);
        evas_object_smart_member_add(ge_item->draw, gp->draw);
        evas_object_smart_member_add(ge_item->highlight, gp->draw);
     }

   /* Fill empty cells with fake items */
   evas_object_table_col_row_size_get(gp->draw, &col, &row);
   Eina_List *fake_queue = gp->fake_items;
   Groupedit_Item *fake = NULL;
   for (i = 0; i < col; i++)
     {
        for (k = 0; k < row; k++)
          {
             child = evas_object_table_child_get(gp->draw, i, k);
             if (!child)
               {
                  fake = eina_list_data_get(fake_queue);
                  fake_queue = eina_list_next(fake_queue);
                  if (!fake)
                    {
                       /* If there are no already created fakes items, then create new */
                       fake = (Groupedit_Item *)mem_calloc(1, sizeof(Groupedit_Item));
                       IMAGE_ADD_NEW(sd->obj, fake->border, "bg", "part_item")
                       evas_object_show(fake->border);
                       evas_object_size_hint_min_set(fake->border, EVAS_HINT_FILL, EVAS_HINT_FILL);
                       evas_object_size_hint_align_set(fake->border, EVAS_HINT_FILL, EVAS_HINT_FILL);
                       evas_object_size_hint_weight_set(fake->border, EVAS_HINT_FILL, EVAS_HINT_FILL);
                       IMAGE_ADD_NEW(sd->obj, fake->highlight, "border", "1px");
                       evas_object_size_hint_min_set(fake->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
                       evas_object_size_hint_align_set(fake->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
                       evas_object_size_hint_weight_set(fake->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
                       evas_object_color_set(fake->highlight, 49, 140, 141, 255);
                       evas_object_show(fake->highlight);
                       gp->fake_items = eina_list_append(gp->fake_items, fake);
                    }
                  evas_object_table_pack(gp->draw, fake->border, i, k, 1, 1);
                  evas_object_table_pack(gp->draw, fake->highlight, i, k, 1, 1);
                  fake_count++;
               }
          }
     }
   fake_diff = fake_count - fake_init;
   EINA_LIST_REVERSE_FOREACH(gp->fake_items, fake_l, fake)
     {
        if (fake_diff < 0)
          {
             evas_object_del(fake->border);
             evas_object_del(fake->highlight);
             free(fake);
             gp->fake_items = eina_list_remove_list(gp->fake_items, fake_l);
             fake_diff++;
          }
        else break;
     }
   evas_object_smart_calculate(gp->draw);
}

static Evas_Object_Box_Layout
_box_layout_function_get(const char *primary, const char *fallback)
{
   const struct edje_box_layouts *base = _edje_box_layouts;
   const struct edje_box_layouts *base_fallback = _edje_box_layouts;

   if (primary)
     {
        for (; base->name; base++)
          if (strcmp(base->name, primary) == 0)
            return base->cb;
     }
   if (fallback)
     {
        for (; base_fallback->name; base_fallback++)
          if (strcmp(base_fallback->name, fallback) == 0)
            return base_fallback->cb;
     }

   return _box_layout_horizontal;
}


static void
_box_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp)
{
   Groupedit_Item *ge_item = NULL, *spread_item = NULL;
   Eina_List *l_items, *l_n_items;
   Eina_Stringshare *part = gp->name;
   Eina_Stringshare *item_source = NULL;
   Eina_Stringshare *primary_layout = NULL;
   Eina_Stringshare *fallback_layout = NULL;

   int w, h; /* Geometry values */
   int min_w, max_w, prefer_w, min_h, max_h, prefer_h; /* Hints values */
   int spread_col, spread_row, old_spread_row; /* Position values */
   Evas_Aspect_Control aspect = EVAS_ASPECT_CONTROL_NONE;
   int aspect_x, aspect_y;
   double align_x = 0, align_y = 0, weight_x = 0, weight_y = 0; /* Align object in cell */
   int pad_l = 0, pad_r = 0, pad_t = 0, pad_b = 0;
   int r = 0, g = 0, b = 0, a = 0;
   double box_align_x, box_align_y;
   int pad_x, pad_y;

   assert(sd != NULL);
   assert(gp != NULL);

   TODO("get items from box and edje_unload them, remove them, destroy them")
   evas_object_box_remove_all(gp->draw, false);

   PART_STATE_GET(sd->edit_obj, gp->name)
   _colors_get(gp, sd->edit_obj, state, value, &r, &g, &b, &a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   PART_STATE_FREE

   edje_edit_state_container_align_get(sd->edit_obj, gp->name, state, value, &box_align_x, &box_align_y);
   evas_object_box_align_set(gp->draw, box_align_x, box_align_y);
   edje_edit_state_container_padding_get(sd->edit_obj, gp->name, state, value, &pad_x, &pad_y);
   evas_object_box_padding_set(gp->draw, pad_x, pad_y);

   /* Changing layout according to edje_edit params! */
   primary_layout = edje_edit_state_box_layout_get(sd->edit_obj, gp->name, state, value);
   fallback_layout = edje_edit_state_box_alt_layout_get(sd->edit_obj, gp->name, state, value);
   evas_object_box_layout_set(gp->draw,
          _box_layout_function_get(primary_layout, fallback_layout), gp->items, NULL);
   eina_stringshare_del(primary_layout);
   eina_stringshare_del(fallback_layout);

   EINA_LIST_FOREACH_SAFE(gp->items, l_items, l_n_items, ge_item)
     {
        evas_object_del(ge_item->draw);
        ge_item->draw = edje_object_add(sd->e);
        evas_object_show(ge_item->draw);

        item_source = edje_edit_part_item_source_get(sd->edit_obj, part, ge_item->name);
        edje_object_file_set(ge_item->draw, sd->edit_obj_file, item_source);

        evas_object_box_append(gp->draw, ge_item->draw);

        min_w = edje_edit_part_item_min_w_get(sd->edit_obj, part, ge_item->name);
        min_h = edje_edit_part_item_min_h_get(sd->edit_obj, part, ge_item->name);

        if ((min_w <= 0) && (min_h <= 0))
          {
             edje_object_size_min_get(ge_item->draw, &w, &h);
             if ((w <= 0) && (h <= 0))
               edje_object_size_min_calc(ge_item->draw, &w, &h);
          }
        if (((min_w <= 0) && (min_h <= 0)) && ((w > 0) || (h > 0)))
          evas_object_size_hint_min_set(ge_item->draw, w, h);
        else
          {
             w = min_w;
             h = min_h;
             evas_object_size_hint_min_set(ge_item->draw, min_w, min_h);
          }
        max_w = edje_edit_part_item_max_w_get(sd->edit_obj, part, ge_item->name);
        max_h = edje_edit_part_item_max_h_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_max_set(ge_item->draw, max_w, max_h);
        prefer_w = edje_edit_part_item_prefer_w_get(sd->edit_obj, part, ge_item->name);
        prefer_h = edje_edit_part_item_prefer_h_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_request_set(ge_item->draw, prefer_w, prefer_h);
        edje_edit_part_item_padding_get(sd->edit_obj, part, ge_item->name, &pad_l, &pad_r, &pad_t, &pad_b);
        evas_object_size_hint_padding_set(ge_item->draw, pad_l, pad_r, pad_t, pad_b);
        align_x = edje_edit_part_item_align_x_get(sd->edit_obj, part, ge_item->name);
        align_y = edje_edit_part_item_align_y_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_align_set(ge_item->draw, align_x, align_y);
        weight_x = edje_edit_part_item_weight_x_get(sd->edit_obj, part, ge_item->name);
        weight_y = edje_edit_part_item_weight_y_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_weight_set(ge_item->draw, weight_x, weight_y);
        switch(edje_edit_part_item_aspect_mode_get(sd->edit_obj, part, ge_item->name))
          {
           case EDJE_ASPECT_CONTROL_NONE: aspect = EVAS_ASPECT_CONTROL_NONE; break;
           case EDJE_ASPECT_CONTROL_NEITHER: aspect = EVAS_ASPECT_CONTROL_NEITHER; break;
           case EDJE_ASPECT_CONTROL_HORIZONTAL: aspect = EVAS_ASPECT_CONTROL_HORIZONTAL; break;
           case EDJE_ASPECT_CONTROL_VERTICAL: aspect = EVAS_ASPECT_CONTROL_VERTICAL; break;
           case EDJE_ASPECT_CONTROL_BOTH: aspect = EVAS_ASPECT_CONTROL_BOTH; break;
          }
        aspect_x = edje_edit_part_item_aspect_w_get(sd->edit_obj, part, ge_item->name);
        aspect_y = edje_edit_part_item_aspect_h_get(sd->edit_obj, part, ge_item->name);
        evas_object_size_hint_aspect_set(ge_item->draw, aspect, aspect_x, aspect_y);

        spread_col = edje_edit_part_item_spread_w_get(sd->edit_obj, part, ge_item->name);
        spread_row = edje_edit_part_item_spread_h_get(sd->edit_obj, part, ge_item->name);

        EINA_LIST_FREE(ge_item->spread, spread_item)
          {
             evas_object_smart_member_del(spread_item->border);
             evas_object_smart_member_del(spread_item->draw);
             evas_object_smart_member_del(spread_item->highlight);
             evas_object_del(spread_item->border);
             evas_object_del(spread_item->draw);
             evas_object_del(spread_item->highlight);
             free(spread_item);
          }
        old_spread_row = spread_row;

        while((spread_col > 1) || (spread_row > 1))
          {
             spread_item = (Groupedit_Item *)mem_calloc(1, sizeof(Groupedit_Item));

             IMAGE_ADD_NEW(sd->obj, spread_item->border, "bg", "part_item")
             evas_object_size_hint_min_set(spread_item->border, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_size_hint_align_set(spread_item->border, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_size_hint_weight_set(spread_item->border, EVAS_HINT_FILL, EVAS_HINT_FILL);

             IMAGE_ADD_NEW(sd->obj, spread_item->highlight, "border", "1px");
             evas_object_size_hint_min_set(spread_item->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_size_hint_align_set(spread_item->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);
             evas_object_size_hint_weight_set(spread_item->highlight, EVAS_HINT_FILL, EVAS_HINT_FILL);

             if (ge_item->selected)
               {
                  edje_object_file_set(spread_item->highlight, EFLETE_THEME, "elm/image/border/2px");
                  evas_object_color_set(spread_item->highlight, 0, 253, 255, 255);
               }
             else
               {
                  edje_object_file_set(spread_item->highlight, EFLETE_THEME, "elm/image/border/1px");
                  evas_object_color_set(spread_item->highlight, 49, 140, 141, 255);
               }

             spread_item->draw = edje_object_add(sd->e);
             edje_object_file_set(spread_item->draw, sd->edit_obj_file, item_source);
             evas_object_size_hint_max_set(spread_item->draw, max_w, max_h);
             evas_object_size_hint_request_set(spread_item->draw, prefer_w, prefer_h);
             evas_object_size_hint_min_set(spread_item->draw, min_w, min_h);
             evas_object_size_hint_padding_set(spread_item->draw, pad_l, pad_r, pad_t, pad_b);
             evas_object_size_hint_align_set(spread_item->draw, align_x, align_y);
             evas_object_size_hint_weight_set(spread_item->draw, weight_x, weight_y);
             evas_object_size_hint_aspect_set(spread_item->draw, aspect, aspect_x, aspect_y);

             ge_item->spread = eina_list_append(ge_item->spread, spread_item);
             evas_object_box_append(gp->draw, spread_item->draw);

             evas_object_smart_member_add(spread_item->draw, gp->draw);
             evas_object_smart_member_add(spread_item->border, gp->draw);
             evas_object_smart_member_add(spread_item->highlight, gp->draw);

             evas_object_show(spread_item->border);
             evas_object_show(spread_item->draw);
             evas_object_show(spread_item->highlight);
             evas_object_raise(spread_item->draw);
             evas_object_raise(spread_item->highlight);
             evas_object_lower(spread_item->border);

             if (spread_row > 1) spread_row--;
             else if (spread_col > 1) {spread_col--; spread_row = old_spread_row; }
             evas_object_color_set(spread_item->draw, r, g, b, a);
          }

        evas_object_color_set(ge_item->draw, r, g, b, a);

        evas_object_smart_member_add(ge_item->border, gp->draw);
        evas_object_smart_member_add(ge_item->draw, gp->draw);
        evas_object_smart_member_add(ge_item->highlight, gp->draw);
        evas_object_raise(ge_item->highlight);

        evas_object_show(ge_item->border);
        evas_object_show(ge_item->highlight);
     }
}

static void
_part_object_area_calc(Ws_Groupedit_Smart_Data *sd)
{
   Eina_Stringshare *rel_to;
   int xc, yc, wc, hc;
   int xg, yg, wg, hg; // groupedit geometry
   int x = 0, w = 0, y = 0, h = 0;
   double relative;
   int offset;

   Groupedit_Part *rel_part = NULL;

   assert(sd != NULL);

   if (!sd->selected)
     {
        x = sd->con_current_size->x; y = sd->con_current_size->y;
        w = sd->con_current_size->w; h = sd->con_current_size->h;
     }
   else
     {
        PART_STATE_GET(sd->edit_obj, sd->selected->name)
        const char *name = sd->selected->name;

        evas_object_geometry_get(sd->edit_obj, &xg, &yg, &wg, &hg);

        xc = xg;
        wc = wg;
        rel_to = edje_edit_state_rel1_to_x_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel1_relative_x_get(sd->edit_obj, name, state, value);
        offset = edje_edit_state_rel1_offset_x_get(sd->edit_obj, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, &xc, NULL, &wc, NULL);
          }
        x = (xc + (int)(wc * relative * sd->zoom_factor)) + offset;
        edje_edit_string_free(rel_to);

        yc = yg;
        hc = hg;
        rel_to = edje_edit_state_rel1_to_y_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel1_relative_y_get(sd->edit_obj, name, state, value);
        offset = edje_edit_state_rel1_offset_y_get(sd->edit_obj, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, NULL, &yc, NULL, &hc);
          }
        y = (yc + (int)(hc * relative * sd->zoom_factor)) + offset;
        edje_edit_string_free(rel_to);

        xc = xg; wc = sd->con_current_size->w;
        rel_to = edje_edit_state_rel2_to_x_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel2_relative_x_get(sd->edit_obj, name, state, value);
        offset = edje_edit_state_rel2_offset_x_get(sd->edit_obj, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, &xc, NULL, &wc, NULL);
          }
        w = ((xc - x) + (int)(wc * relative * sd->zoom_factor)) + offset;
        if (w < 0) { x += w; w = 0; }
        edje_edit_string_free(rel_to);

        yc = yg; hc = sd->con_current_size->h;
        rel_to = edje_edit_state_rel2_to_y_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel2_relative_y_get(sd->edit_obj, name, state, value);
        offset = edje_edit_state_rel2_offset_y_get(sd->edit_obj, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, NULL, &yc, NULL, &hc);
          }
        h = ((yc - y) + (int)(hc * relative * sd->zoom_factor)) + offset;
        if (h < 0) { y += h; h = 0; }
        edje_edit_string_free(rel_to);

        evas_object_geometry_get(sd->obj, &xc, &yc, NULL, NULL);

        evas_object_resize(sd->obj_area.obj, w, h);
        evas_object_move(sd->obj_area.obj, x, y);
        evas_object_stack_below(sd->obj_area.obj, sd->selected->draw);

        PART_STATE_FREE
     }

   if (sd->obj_area.visible) evas_object_show(sd->obj_area.obj);
   else evas_object_hide(sd->obj_area.obj);

   sd->obj_area.geom->x = x; sd->obj_area.geom->y = y;
   sd->obj_area.geom->w = w; sd->obj_area.geom->h = h;

   if ((sd->selected) && ((sd->obj_area.visible)))
     evas_object_show(sd->obj_area.obj);
   else
     evas_object_hide(sd->obj_area.obj);

   evas_object_smart_callback_call(sd->obj, SIG_OBJ_AREA_CHANGED, sd->obj_area.geom);
}
