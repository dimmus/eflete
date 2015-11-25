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
_part_draw_add(Ws_Groupedit_Smart_Data *sd, Part_ *part);

static void
_move_border_to_top(Ws_Groupedit_Smart_Data *sd);


static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, Part_ *part);

static Groupedit_Item *
_item_draw_add(Ws_Groupedit_Smart_Data *sd, Part_ *part, Eina_Stringshare *item);

static void
_item_draw_del(Groupedit_Item *ge_item);

static void
_part_object_area_calc(Ws_Groupedit_Smart_Data *sd);

static void
_common_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_image_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_proxy_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_table_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp);

static void
_box_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp);

Eina_Bool
_edit_object_part_add(Ws_Groupedit_Smart_Data *sd,
                      Part_ *part)
{
   Groupedit_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = _part_draw_add(sd, part);
   sd->parts = eina_list_append(sd->parts, gp);
   _move_border_to_top(sd);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_del(Ws_Groupedit_Smart_Data *sd, Part_ *part)
{
   assert(sd != NULL);
   assert(sd->parts != NULL);
   assert(part != NULL);

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
   if (!edje_edit_part_item_append(sd->group->edit_object, part, item, source))
     return false;

   ge_item = _item_draw_add(sd, gp->part, item);
   if (!ge_item) return false;
   type = edje_edit_part_type_get(sd->group->edit_object, part);

   if (type == EDJE_PART_TYPE_TABLE)
     {
        /*Position new item in cell with coords [0;0]*/
        /*edit_part_item_position_col_set(sd->group->edit_object, part, item, 0);
        edje_edit_part_item_position_row_set(sd->group->edit_object, part, item, 0);*/
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

   if (!edje_edit_part_item_del(sd->group->edit_object, part, item))
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
        evas_object_stack_below(ge_part->item, ge_part_to->draw);
     }
   else
     {
        evas_object_stack_above(sd->bg, ge_part_to->draw);
        evas_object_stack_above(sd->clipper, ge_part_to->draw);
        evas_object_stack_above(ge_part->item, ge_part_to->draw);
        evas_object_stack_above(ge_part->draw, ge_part_to->draw);
        evas_object_stack_above(ge_part->bg, ge_part_to->draw);
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
     ret = edje_edit_part_restack_part_below(sd->group->edit_object, part, part_above);
   else
     ret = edje_edit_part_restack_below(sd->group->edit_object, part);

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
     ret = edje_edit_part_restack_part_above(sd->group->edit_object, part, part_below);
   else
     ret = edje_edit_part_restack_above(sd->group->edit_object, part);

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
   Eina_List *l;
   Part_ *part;

   assert(sd != NULL);

   evas_event_freeze(sd->e);
   EINA_LIST_FOREACH(sd->group->parts, l, part)
     {
        gp = _part_draw_add(sd, part);
        sd->parts = eina_list_append(sd->parts, gp);
     }
   evas_event_thaw(sd->e);
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
   Groupedit_Item *ge_item;

   assert(gp != NULL);

   evas_object_del(gp->draw);
   if (gp->item)
     {
        evas_object_smart_member_del(gp->item);
        evas_object_del(gp->item);
     }
   if (gp->bg)
     evas_object_del(gp->bg);

   EINA_LIST_FREE(gp->items, ge_item)
     _item_draw_del(ge_item);
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
        if ((gp->part->name == part))
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
       (is_below) ? "belove" : "above", sd->selected->part->name, gp->part->name);

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

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->draw, "sd");

   assert(sd != NULL);

   Evas_Event_Mouse_Down *emd = (Evas_Event_Mouse_Down *)event_info;

   if (emd->button != 1) return;
   sd->to_select = gp;
   evas_object_smart_callback_call(sd->obj, SIGNAL_GROUPEDIT_PART_SELECTED,
                                  (void *)gp->part);
}

static void
_part_separete_mod_mouse_in_cb(void *data,
                               Evas *e __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;

   assert(gp != NULL);

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->draw, "sd");

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

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->draw, "sd");

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

   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->draw, "sd");

   assert(sd != NULL);

   Evas_Event_Mouse_Down *emd = (Evas_Event_Mouse_Down *)event_info;

   if (emd->button != 1) return;
   sd->to_select = gp;
   evas_object_smart_callback_call(sd->obj, SIGNAL_GROUPEDIT_PART_SELECTED,
                                  (void *)gp->part);
}

static void
_part_table_items_add(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp, Eina_Stringshare ***items_draw)
{
   const Evas_Object *table;
   Evas_Object *cell;
   Eina_Stringshare *item_name, *item_source;
   int w, h; /* Geometry values */
   int min_w, max_w, min_h, max_h; /* Hints values */
   int i, j, col, row;
   unsigned char span_col, span_row;

   assert(gp->container != NULL);

   table = edje_object_part_object_get(sd->group->edit_object, gp->part->name);
   evas_object_table_col_row_size_get(table, &col, &row);
   evas_object_table_clear(gp->container, true);

   for (i = 0; i < col; i++)
     {
        for (j = 0; j < row; j++)
          {
             if (items_draw[i][j] == (Eina_Stringshare *) -1) continue;

             span_col = 1;
             span_row = 1;

             if (items_draw[i][j] != NULL)
               {
                  item_name = items_draw[i][j];
                  span_col = edje_edit_part_item_span_col_get(sd->group->edit_object, gp->part->name, item_name);
                  span_row = edje_edit_part_item_span_row_get(sd->group->edit_object, gp->part->name, item_name);

                  cell = edje_object_add(sd->e);
                  item_source = edje_edit_part_item_source_get(sd->group->edit_object, gp->part->name, item_name);
                  edje_object_file_set(cell, ap.project->dev, item_source);
                  /* hide this object, it need only for calculate cell size */
                  evas_object_hide(cell);

                  min_w = edje_edit_part_item_min_w_get(sd->group->edit_object, gp->part->name, item_name);
                  min_h = edje_edit_part_item_min_h_get(sd->group->edit_object, gp->part->name, item_name);

                  // Calculation according to box/table item implementation in efl 1.13 at edje_load.c
                  if ((min_w <= 0) && (min_h <= 0))
                    {
                       edje_object_size_min_get(cell, &w, &h);
                       if ((w <= 0) && (h <= 0))
                         edje_object_size_min_calc(cell, &w, &h);
                    }
                  if (((min_w <= 0) && (min_h <= 0)) && ((w > 0) || (h > 0)))
                    evas_object_size_hint_min_set(cell, w, h);
                  else
                    evas_object_size_hint_min_set(cell, min_w, min_h);

                  max_w = edje_edit_part_item_max_w_get(sd->group->edit_object, gp->part->name, item_name);
                  max_h = edje_edit_part_item_max_h_get(sd->group->edit_object, gp->part->name, item_name);
                  evas_object_size_hint_max_set(cell, max_w, max_h);
                  evas_object_table_pack(gp->container, cell, i, j, span_col, span_row);
               }
             cell = elm_layout_add(sd->parent);
             evas_object_size_hint_align_set(cell, EVAS_HINT_FILL, EVAS_HINT_FILL);

             elm_layout_theme_set(cell, "layout", "groupview", "default");
             evas_object_show(cell);
             elm_object_signal_emit(cell, "border,part_item", "eflete");

             evas_object_table_pack(gp->container, cell, i, j, span_col, span_row);
          }
     }
}

static void
_part_table_add(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp)
{

   Eina_List *l;
   Eina_Stringshare *str;
   int span_c, span_r, col, row, i, j;
   const Evas_Object *table;
   Eina_Stringshare ***items_draw;

   assert(gp->container == NULL);

   gp->container = evas_object_table_add(sd->e);
   elm_box_pack_end(gp->draw, gp->container);
   evas_object_show(gp->container);

   table = edje_object_part_object_get(sd->group->edit_object, gp->part->name);
   evas_object_table_col_row_size_get(table, &col, &row);
   fprintf(stdout, "%5d %5d\n", col, row);
   items_draw = (Eina_Stringshare ***)mem_calloc(1, sizeof(Eina_Stringshare **) * col);
   for (i = 0; i < col; i++)
     items_draw[i] = (Eina_Stringshare **)mem_calloc(1, sizeof(Eina_Stringshare *) * row);
   EINA_LIST_FOREACH(gp->part->items, l, str)
     {
        col = edje_edit_part_item_position_col_get(sd->group->edit_object, gp->part->name, str);
        row = edje_edit_part_item_position_row_get(sd->group->edit_object, gp->part->name, str);
        span_c = edje_edit_part_item_span_col_get(sd->group->edit_object, gp->part->name, str);
        span_r = edje_edit_part_item_span_row_get(sd->group->edit_object, gp->part->name, str);

        for (i = col; i < (col + span_c); i++)
          {
             for (j = row; j < (row + span_r); j++)
               items_draw[i][j] = (Eina_Stringshare *) -1;
          }
        items_draw[col][row] = eina_stringshare_add(str);
     }
   _part_table_items_add(sd, gp, items_draw);

   evas_object_table_col_row_size_get(table, &col, &row);
   for (i = 0; i < col; i++)
     {
        for (j = 0; j < row; j++)
          {
             if (items_draw[i][j] == (Eina_Stringshare *) -1) continue;
             eina_stringshare_del(items_draw[i][j]);
          }
     }
   for (i = 0; i < col; i++)
     free(items_draw[i]);
   free(items_draw);


}

/* get from edje_util.c:2935 */
struct edje_box_layout_builtin
{
   const char            *name;
   Evas_Object_Box_Layout cb;
};

static Evas_Object_Box_Layout
_edje_box_layout_builtin_find(const char *name)
{
   const struct edje_box_layout_builtin _edje_box_layout_builtin[] = {
      {"horizontal", evas_object_box_layout_horizontal},
      {"horizontal_flow", evas_object_box_layout_flow_horizontal},
      {"horizontal_homogeneous", evas_object_box_layout_homogeneous_horizontal},
      {"horizontal_max", evas_object_box_layout_homogeneous_max_size_horizontal},
      {"stack", evas_object_box_layout_stack},
      {"vertical", evas_object_box_layout_vertical},
      {"vertical_flow", evas_object_box_layout_flow_vertical},
      {"vertical_homogeneous", evas_object_box_layout_homogeneous_vertical},
      {"vertical_max", evas_object_box_layout_homogeneous_max_size_vertical},
      {NULL, NULL}
   };
   const struct edje_box_layout_builtin *base;

   switch (name[0])
     {
      case 'h':
        base = _edje_box_layout_builtin + 0;
        break;

      case 's':
        base = _edje_box_layout_builtin + 4;
        break;

      case 'v':
        base = _edje_box_layout_builtin + 5;
        break;

      default:
        return NULL;
     }

   for (; (base->name) && (base->name[0] == name[0]); base++)
     if (strcmp(base->name, name) == 0)
       return base->cb;

   return NULL;
}

static void
_part_box_add(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp)
{

   Eina_List *l;
   Eina_Stringshare *str, *item_source;
   int spread_w, spread_h;
   int min_w, min_h, max_w, max_h,  w, h, i;
   Evas_Object *cell, *cell_content;

   assert(gp->container == NULL);

   fprintf(stdout, "%s\n", gp->part->name);
   gp->container = evas_object_box_add(sd->e);
   elm_box_pack_end(gp->draw, gp->container);
   evas_object_show(gp->container);

   EINA_LIST_FOREACH(gp->part->items, l, str)
     {
        spread_w = edje_edit_part_item_spread_w_get(sd->group->edit_object, gp->part->name, str);
        spread_h = edje_edit_part_item_spread_h_get(sd->group->edit_object, gp->part->name, str);
        for (i = 0; i < (spread_w * spread_h); i++)
          {
             cell = elm_layout_add(sd->parent);
             evas_object_size_hint_align_set(cell, EVAS_HINT_FILL, EVAS_HINT_FILL);
             elm_layout_theme_set(cell, "layout", "groupview", "default");
             evas_object_show(cell);
             elm_object_signal_emit(cell, "border,part_item", "eflete");

             cell_content = edje_object_add(sd->e);
             item_source = edje_edit_part_item_source_get(sd->group->edit_object, gp->part->name, str);
             edje_object_file_set(cell_content, ap.project->dev, item_source);

             min_w = edje_edit_part_item_min_w_get(sd->group->edit_object, gp->part->name, str);
             min_h = edje_edit_part_item_min_h_get(sd->group->edit_object, gp->part->name, str);

             if ((min_w <= 0) && (min_h <= 0))
               {
                  edje_object_size_min_get(cell_content, &w, &h);
                  if ((w <= 0) && (h <= 0))
                    edje_object_size_min_calc(cell_content, &w, &h);
               }
             if (((min_w <= 0) && (min_h <= 0)) && ((w > 0) || (h > 0)))
               evas_object_size_hint_min_set(cell_content, w, h);
             else
               evas_object_size_hint_min_set(cell_content, min_w, min_h);
             max_w = edje_edit_part_item_max_w_get(sd->group->edit_object, gp->part->name, str);
             max_h = edje_edit_part_item_max_h_get(sd->group->edit_object, gp->part->name, str);
             evas_object_size_hint_max_set(cell_content, max_w, max_h);

             elm_object_content_set(cell, cell_content);
             evas_object_hide(cell_content);
             evas_object_box_append(gp->container, cell);
          }
     }
}

static void
_box_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp)
{
   double align_x = 0, align_y = 0;
   int pad_x, pad_y;
   Eina_Stringshare *layout;

   assert(sd != NULL);
   assert(gp != NULL);

   PART_STATE_GET(sd->group->edit_object, gp->part->name)

   layout = edje_edit_state_box_layout_get(sd->group->edit_object, gp->part->name, state, value);
   evas_object_box_layout_set(gp->container, _edje_box_layout_builtin_find(layout), NULL, NULL);

   align_x = edje_edit_state_container_align_x_get(sd->group->edit_object, gp->part->name, state, value);
   align_y = edje_edit_state_container_align_y_get(sd->group->edit_object, gp->part->name, state, value);
   evas_object_box_align_set(gp->container, align_x, align_y);

   pad_x = edje_edit_state_container_padding_x_get(sd->group->edit_object, gp->part->name, state, value);
   pad_y = edje_edit_state_container_padding_y_get(sd->group->edit_object, gp->part->name, state, value);
   evas_object_box_padding_set(gp->container, pad_x, pad_y);

   PART_STATE_FREE
}

static void
_part_container_del(Groupedit_Part *gp)
{
   assert(gp->container != NULL);

   elm_box_unpack(gp->draw, gp->container);
   evas_object_del(gp->container);
   gp->container = NULL;
}

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
_part_recalc_apply(Ws_Groupedit_Smart_Data *sd,
                   Groupedit_Part *gp,
                   int offset_x,
                   int offset_y)
{
   Evas_Coord x, y, xe, ye, w, h;
   Evas_Coord part_x, part_y, abs_x, abs_y;
   Evas_Coord ro_w, ro_h;
   Eina_List *l, *l_n, *l_sp, *l_sp_n;
   Groupedit_Item *ge_item = NULL, *sp_item = NULL;
   const Evas_Object *ro;

   assert(sd != NULL);
   assert(gp != NULL);

   edje_object_part_geometry_get(sd->group->edit_object, gp->part->name, &x, &y, &w, &h);
   evas_object_geometry_get(sd->group->edit_object, &xe, &ye, NULL, NULL);

   evas_object_resize(gp->draw, w, h);
   evas_object_move(gp->draw, (x * sd->zoom_factor + xe + offset_x),
                              (y * sd->zoom_factor + ye + offset_y));
   evas_object_size_hint_min_set(gp->layout, w, h);
   evas_object_size_hint_max_set(gp->layout, w, h);

   if (gp->container) evas_object_size_hint_max_set(gp->container, w, h);

   evas_object_resize(gp->container, w, h);
   evas_object_move(gp->container, (x * sd->zoom_factor + xe + offset_x),
                                   (y * sd->zoom_factor + ye + offset_y));

   /* set the proxy part size */
   ro = edje_object_part_object_get(sd->group->edit_object, gp->part->name);
   evas_object_geometry_get(ro, NULL, NULL, &ro_w, &ro_h);
   evas_object_size_hint_min_set(gp->proxy_part, ro_w, ro_h);
   evas_object_size_hint_max_set(gp->proxy_part, ro_w, ro_h);

   if (gp->part->type == EDJE_PART_TYPE_TEXT)
     {
        double x_align, y_align;

        x_align = edje_edit_state_text_align_x_get(sd->group->edit_object,
                                                   gp->part->name,
                                                   gp->part->current_state->parsed_name,
                                                   gp->part->current_state->parsed_val);
        y_align = edje_edit_state_text_align_y_get(sd->group->edit_object,
                                                   gp->part->name,
                                                   gp->part->current_state->parsed_name,
                                                   gp->part->current_state->parsed_val);
        evas_object_size_hint_align_set(gp->proxy_part, x_align, y_align);
     }


   GP_REAL_GEOMETRY_CALC(part_x, part_y, abs_x, abs_y)

   if (gp->part->type == EDJE_PART_TYPE_BOX)
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
                       if (gp->part->type == EDJE_PART_TYPE_BOX)
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

#undef GP_REAL_GEOMETRY_CALC
#undef ZOOM_APPLY

Eina_Bool
_parts_recalc(Ws_Groupedit_Smart_Data *sd)
{
   Eina_List *l;
   Groupedit_Part *gp;
   Eina_Stringshare *str;
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

        switch (gp->part->type)
          {
           case EDJE_PART_TYPE_TEXT:
           case EDJE_PART_TYPE_TEXTBLOCK:
              str = edje_edit_state_text_get(sd->group->edit_object, gp->part->name,
                                             gp->part->current_state->parsed_name,
                                             gp->part->current_state->parsed_val);
              if (!str)
                edje_object_part_text_set(sd->group->edit_object, gp->part->name, gp->part->name);
              eina_stringshare_del(str);
              _common_param_update(gp, sd->group->edit_object);
              break;
           case EDJE_PART_TYPE_RECTANGLE:
           case EDJE_PART_TYPE_GROUP:
              _common_param_update(gp, sd->group->edit_object);
              break;
           case EDJE_PART_TYPE_IMAGE:
              if (sd->manual_calc) _image_param_update(gp, sd->group->edit_object);
              break;
           case EDJE_PART_TYPE_PROXY: // it part like image
              if (sd->manual_calc) _proxy_param_update(gp, sd->group->edit_object);
              break;
           case EDJE_PART_TYPE_TABLE:
              if (sd->manual_calc)
                {
                   _part_container_del(gp);
                   _part_table_add(sd, gp);
                }
              _table_param_update(sd, gp);
              _common_param_update(gp, sd->group->edit_object);
              break;
           case EDJE_PART_TYPE_BOX:
              if (sd->manual_calc)
                {
                   _part_container_del(gp);
                   _part_box_add(sd, gp);
                }
              _box_param_update(sd, gp);
              _common_param_update(gp, sd->group->edit_object);
              break;
           case EDJE_PART_TYPE_SPACER:
           case EDJE_PART_TYPE_SWALLOW:
           case EDJE_PART_TYPE_EXTERNAL:
           default:
              break;
          }
        _part_recalc_apply(sd, gp, offset_x, offset_y);
        if (gp->part->visible)
          {
             evas_object_show(gp->bg);
             evas_object_show(gp->draw);
          }
        else
          {
             evas_object_hide(gp->bg);
             evas_object_hide(gp->draw);
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

static Groupedit_Part *
_part_draw_add(Ws_Groupedit_Smart_Data *sd, Part_ *part)
{
   Groupedit_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = mem_calloc(1, sizeof(Groupedit_Part));
   gp->part = part;
   gp->item = NULL;


#define PART_VIEW_ADD() \
   gp->draw = elm_box_add(sd->parent); \
   elm_box_layout_set(gp->draw, evas_object_box_layout_stack, NULL, NULL); \
   gp->layout = elm_layout_add(sd->parent); \
   elm_layout_theme_set(gp->layout, "layout", "groupview", "default"); \
   evas_object_show(gp->layout); \
   elm_box_pack_end(gp->draw, gp->layout);

#define PART_VIEW_PROXY_SET() \
   gp->proxy_part = evas_object_image_filled_add(sd->e); \
   elm_box_pack_end(gp->draw, gp->proxy_part); \
   evas_object_show(gp->proxy_part); \

   switch (part->type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         break;
      case EDJE_PART_TYPE_TEXT:
      case EDJE_PART_TYPE_TEXTBLOCK:
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,text", "eflete");
         break;
      case EDJE_PART_TYPE_IMAGE:
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,text", "eflete");
         break;
      case EDJE_PART_TYPE_PROXY: // it part like image
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,proxy", "eflete");
         break;
      case EDJE_PART_TYPE_SWALLOW:
         PART_VIEW_ADD()
         elm_object_signal_emit(gp->layout, "border,swallow", "eflete");
         break;
      case EDJE_PART_TYPE_SPACER:
         PART_VIEW_ADD()
         elm_object_signal_emit(gp->layout, "border,spacer", "eflete");
         break;
      case EDJE_PART_TYPE_GROUP:
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,group", "eflete");
         break;
      case EDJE_PART_TYPE_TABLE:
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,table", "eflete");
         _part_table_add(sd, gp);
         break;
      case EDJE_PART_TYPE_BOX:
         PART_VIEW_ADD()
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,box", "eflete");
         _part_box_add(sd, gp);

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

   evas_object_data_set(gp->draw, "sd", sd);
   evas_object_event_callback_add(gp->draw, EVAS_CALLBACK_MOUSE_DOWN,
                                  _part_select, gp);

   if (gp->bg)
     evas_object_smart_member_add(gp->bg, sd->obj);
   evas_object_smart_member_add(gp->draw, sd->obj);
   //evas_object_smart_member_add(gp->border, sd->obj);

   return gp;
}

static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, Part_ *part)
{
   Groupedit_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = _parts_list_find(sd->parts, part->name);

   assert(gp != NULL);

   sd->parts = eina_list_remove(sd->parts, gp);
   _groupedit_part_free(gp);
}

static Groupedit_Item *
_item_draw_add(Ws_Groupedit_Smart_Data *sd, Part_ *part,
               Eina_Stringshare *item)
{
   Groupedit_Item *ge_item = NULL;
   Eina_Stringshare *item_source;

   assert(sd != NULL);
   assert(part != NULL);
   assert(item != NULL);

   item_source = edje_edit_part_item_source_get(sd->group->edit_object, part->name, item);
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
        edje_object_file_set(ge_item->draw, ap.project->dev, item_source);
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
     edje_edit_state_color_get(edit_obj, gp->part->name, state, value, r, g, b, a);
   if (r2 || g2 || b2 || a2)
     edje_edit_state_color2_get(edit_obj, gp->part->name, state, value, r2, g2, b2, a2);
   if (r3 || g3 || b3 || a3)
     edje_edit_state_color3_get(edit_obj, gp->part->name, state, value, r3, g3, b3, a3);

   color_class = edje_edit_state_color_class_get(edit_obj, gp->part->name, state, value);
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

   if (gp->part->type == EDJE_PART_TYPE_TEXT || gp->part->type == EDJE_PART_TYPE_TEXTBLOCK)
     _colors_get(gp, edit_obj, state, value, &r, &g, &b, &a, &r2, &g2, &b2, &a2, &r3, &g3, &b3, &a3);
   else
     _colors_get(gp, edit_obj, state, value, &r, &g, &b, &a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

   evas_object_color_set(gp->draw, r, g, b, a);
   if (gp->part->type == EDJE_PART_TYPE_TEXT || gp->part->type == EDJE_PART_TYPE_TEXTBLOCK)
     {
        evas_object_text_shadow_color_set(gp->draw, r2, g2, b2, a2);
        evas_object_text_outline_color_set(gp->draw, r3, g3, b3, a3);
        evas_object_text_glow_color_set(gp->draw, r2, g2, b2, a2);
        evas_object_text_glow2_color_set(gp->draw, r3, g3, b3, a3);
     }
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

   PART_STATE_GET(edit_obj, gp->part->name)

   _color_apply(gp, edit_obj, state, value);

   /* setups settings from filled block  into evas image object*/
   evas_object_image_smooth_scale_set(image,
            edje_edit_state_fill_smooth_get(edit_obj, gp->part->name, state, value));

   /* take fill params here, because need to know is default params set*/
   fill_x = edje_edit_state_fill_origin_relative_x_get(edit_obj, gp->part->name, state, value);
   fill_y = edje_edit_state_fill_origin_relative_y_get(edit_obj, gp->part->name, state, value);
   fill_w = edje_edit_state_fill_size_relative_x_get(edit_obj, gp->part->name, state, value);
   fill_h = edje_edit_state_fill_size_relative_y_get(edit_obj, gp->part->name, state, value);
   fill_origin_offset_x = edje_edit_state_fill_origin_offset_x_get(edit_obj, gp->part->name, state, value);
   fill_origin_offset_y = edje_edit_state_fill_origin_offset_y_get(edit_obj, gp->part->name, state, value);
   fill_size_offset_x = edje_edit_state_fill_size_offset_x_get(edit_obj, gp->part->name, state, value);
   fill_size_offset_y = edje_edit_state_fill_size_offset_y_get(edit_obj, gp->part->name, state, value);
   if (edje_edit_state_fill_type_get(edit_obj, gp->part->name, state, value))
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
_image_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   Evas_Load_Error err;
   const char *image_normal;
   const char *buf = NULL;
   int id;
   int bl, br, bt, bb;
   unsigned char middle;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->part->name)

   image_normal = edje_edit_state_image_get(edit_obj, gp->part->name, state, value);
   if (!image_normal) return;
   id = edje_edit_image_id_get(edit_obj, image_normal);
   edje_edit_string_free(image_normal);
   buf = eina_stringshare_printf("edje/images/%i", id);
   //fprintf(stdout, "%s\n", buf);
   evas_object_image_file_set(gp->proxy_part, ap.project->dev, buf);
   err = evas_object_image_load_error_get(gp->proxy_part);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN("Could not update image:\"%s\"\n", evas_load_error_str(err));

   edje_edit_state_image_border_get(edit_obj, gp->part->name, state, value,
                                    &bl, &br, &bt, &bb);
   evas_object_image_border_set(gp->proxy_part, bl, br, bt, bb);

   middle  = edje_edit_state_image_border_fill_get(edit_obj, gp->part->name, state, value);
   if (middle == 0)
     evas_object_image_border_center_fill_set(gp->proxy_part, EVAS_BORDER_FILL_NONE);
   else if (middle == 1)
     evas_object_image_border_center_fill_set(gp->proxy_part, EVAS_BORDER_FILL_DEFAULT);
   else if (middle == 2)
     evas_object_image_border_center_fill_set(gp->proxy_part, EVAS_BORDER_FILL_SOLID);

   _image_proxy_common_param_update(gp->proxy_part, gp, edit_obj);

   PART_STATE_FREE
   eina_stringshare_del(buf);
}

static void
_proxy_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   const char *proxy_source;
   Ws_Groupedit_Smart_Data *sd;
   Groupedit_Part *source;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->part->name)

   sd = evas_object_data_get(gp->draw, "sd");
   proxy_source = edje_edit_state_proxy_source_get(edit_obj, gp->part->name, state, value);
   if (proxy_source)
     {
        elm_object_signal_emit(gp->layout, "border,default", "eflete");
        source = _parts_list_find(sd->parts, proxy_source);
        evas_object_image_source_set(gp->proxy_part, source->proxy_part);
        evas_object_image_source_clip_set(gp->proxy_part, false);
        _image_proxy_common_param_update(gp->proxy_part, gp, edit_obj);
     }
   else
     {
        elm_object_signal_emit(gp->layout, "border,proxy", "eflete");
     }
}

static void
_common_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   assert(gp != NULL);
   assert(edit_obj != NULL);

   evas_object_image_source_set(gp->proxy_part, NULL);
   evas_object_image_source_set(gp->proxy_part,
                                (Evas_Object *)edje_object_part_object_get(edit_obj, gp->part->name));
   evas_object_image_source_clip_set(gp->proxy_part, false);
}

static void
_table_param_update(Ws_Groupedit_Smart_Data *sd, Groupedit_Part *gp)
{
   double align_x = 0, align_y = 0;
   int pad_x, pad_y;
   unsigned char homogeneous;

   assert(sd != NULL);
   assert(gp != NULL);

   PART_STATE_GET(sd->group->edit_object, gp->part->name)

   homogeneous = edje_edit_state_table_homogeneous_get(sd->group->edit_object, gp->part->name, state, value);
   evas_object_table_homogeneous_set(gp->container, homogeneous);

   align_x = edje_edit_state_container_align_x_get(sd->group->edit_object, gp->part->name, state, value);
   align_y = edje_edit_state_container_align_y_get(sd->group->edit_object, gp->part->name, state, value);
   evas_object_table_align_set(gp->container, align_x, align_y);

   pad_x = edje_edit_state_container_padding_x_get(sd->group->edit_object, gp->part->name, state, value);
   pad_y = edje_edit_state_container_padding_y_get(sd->group->edit_object, gp->part->name, state, value);
   evas_object_table_padding_set(gp->container, pad_x, pad_y);

   PART_STATE_FREE
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
        PART_STATE_GET(sd->group->edit_object, sd->selected->part->name)
        const char *name = sd->selected->part->name;

        evas_object_geometry_get(sd->group->edit_object, &xg, &yg, &wg, &hg);

        xc = xg;
        wc = wg;
        rel_to = edje_edit_state_rel1_to_x_get(sd->group->edit_object, name, state, value);
        relative = edje_edit_state_rel1_relative_x_get(sd->group->edit_object, name, state, value);
        offset = edje_edit_state_rel1_offset_x_get(sd->group->edit_object, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, &xc, NULL, &wc, NULL);
          }
        x = (xc + (int)(wc * relative * sd->zoom_factor)) + offset;
        edje_edit_string_free(rel_to);

        yc = yg;
        hc = hg;
        rel_to = edje_edit_state_rel1_to_y_get(sd->group->edit_object, name, state, value);
        relative = edje_edit_state_rel1_relative_y_get(sd->group->edit_object, name, state, value);
        offset = edje_edit_state_rel1_offset_y_get(sd->group->edit_object, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, NULL, &yc, NULL, &hc);
          }
        y = (yc + (int)(hc * relative * sd->zoom_factor)) + offset;
        edje_edit_string_free(rel_to);

        xc = xg; wc = sd->con_current_size->w;
        rel_to = edje_edit_state_rel2_to_x_get(sd->group->edit_object, name, state, value);
        relative = edje_edit_state_rel2_relative_x_get(sd->group->edit_object, name, state, value);
        offset = edje_edit_state_rel2_offset_x_get(sd->group->edit_object, name, state, value);
        if (rel_to)
          {
             rel_part = _parts_list_find(sd->parts, rel_to);
             evas_object_geometry_get(rel_part->draw, &xc, NULL, &wc, NULL);
          }
        w = ((xc - x) + (int)(wc * relative * sd->zoom_factor)) + offset;
        if (w < 0) { x += w; w = 0; }
        edje_edit_string_free(rel_to);

        yc = yg; hc = sd->con_current_size->h;
        rel_to = edje_edit_state_rel2_to_y_get(sd->group->edit_object, name, state, value);
        relative = edje_edit_state_rel2_relative_y_get(sd->group->edit_object, name, state, value);
        offset = edje_edit_state_rel2_offset_y_get(sd->group->edit_object, name, state, value);
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
