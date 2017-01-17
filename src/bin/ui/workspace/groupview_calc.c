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

#include "groupview.h"
#include "groupview_private.h"
#include "project_manager2.h"

#define PART_STATE_GET(obj, part) \
   const char *state; \
   double value; \
   state = edje_edit_part_selected_state_get(obj, part, &value);

#define PART_STATE_FREE \
   edje_edit_string_free(state);

static void
_groupview_part_free(Groupview_Part *gp);

static Groupview_Part *
_part_draw_add(Groupview_Smart_Data *sd, Part2 *part);

static void
_part_draw_del(Groupview_Smart_Data *sd, Part2 *part);

static void
_item_draw_del(Groupview_Item *ge_item);

static void
_common_param_update(Groupview_Part *gp, Evas_Object *edit_obj);

static void
_image_param_update(Groupview_Part *gp, Groupview_Smart_Data *sd);

static void
_proxy_param_update(Groupview_Part *gp, Evas_Object *edit_obj);

static void
_table_param_update(Groupview_Smart_Data *sd, Groupview_Part *gp);

static void
_box_param_update(Groupview_Smart_Data *sd, Groupview_Part *gp);

Eina_Bool
_edit_object_part_add(Groupview_Smart_Data *sd,
                      Part2 *part)
{
   Groupview_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = _part_draw_add(sd, part);
   sd->parts = eina_list_append(sd->parts, gp);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_del(Groupview_Smart_Data *sd, Part2 *part)
{
   assert(sd != NULL);
   assert(sd->parts != NULL);
   assert(part != NULL);

   _part_draw_del(sd, part);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_restack_below(Groupview_Smart_Data *sd,
                                const char *part,
                                const char *rel_part)
{
   Groupview_Part *ge_rel_part, *ge_part;

   assert(sd != NULL);
   assert(sd->parts != NULL);
   assert(part != NULL);

   ge_part = _parts_list_find(sd->parts, part);
   assert(ge_part != NULL);

   sd->parts = eina_list_remove(sd->parts, ge_part);
   elm_box_unpack(sd->box, ge_part->draw);
   if (rel_part)
     {
        ge_rel_part = _parts_list_find(sd->parts, rel_part);
        assert(ge_rel_part != NULL);
        sd->parts = eina_list_prepend_relative(sd->parts, ge_part, ge_rel_part);
        elm_box_pack_before(sd->box, ge_part->draw, ge_rel_part->draw);
     }
   else
     {
        sd->parts = eina_list_append(sd->parts, ge_part);
        elm_box_pack_end(sd->box, ge_part->draw);
     }

   evas_object_smart_changed(sd->obj);
   return true;
}

void
_parts_list_new(Groupview_Smart_Data *sd)
{
   Groupview_Part *gp;
   Eina_List *l;
   Part2 *part;
   Eina_List *parts;

   assert(sd != NULL);

   if (!sd->group->main_group)
     parts = sd->group->parts;
   else
     parts = sd->group->main_group->parts;
   EINA_LIST_FOREACH(parts, l, part)
     {
        gp = _part_draw_add(sd, part);
        sd->parts = eina_list_append(sd->parts, gp);
     }
}

void
_parts_list_free(Groupview_Smart_Data *sd)
{
   Groupview_Part *gp;

   assert(sd != NULL);

   if (!sd->parts) return;

   EINA_LIST_FREE(sd->parts, gp)
     _groupview_part_free(gp);
   sd->parts = NULL;
}

static void
_groupview_part_free(Groupview_Part *gp)
{
   Groupview_Item *ge_item;

   assert(gp != NULL);

   evas_object_del(gp->draw);
   EINA_LIST_FREE(gp->items, ge_item)
     _item_draw_del(ge_item);
   free(gp);
}

Groupview_Part *
_parts_list_find(Eina_List *parts, const char *part)
{
   Eina_List *l;
   Groupview_Part *gp;

   assert(parts != NULL);
   assert(part != NULL);

   EINA_LIST_FOREACH(parts, l, gp)
     {
        if (gp->part->common.name == part)
          return gp;
     }
   return NULL;
}

Groupview_Item *
_part_item_search(Eina_List *items, const char *item_name)
{
   Eina_List *l;
   Groupview_Item *item;

   assert(items != NULL);
   assert(item_name != NULL);

   EINA_LIST_FOREACH(items, l, item)
     {
        /* we are sure that all strings are stringshare, and for compare two
         * strings enough compare they pointers */
        if (item->name == item_name)
          return item;
     }
   return NULL;
}

static void
_part_select(void *data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Groupview_Part *gp = (Groupview_Part *)data;

   assert(gp != NULL);

   Groupview_Smart_Data *sd = evas_object_data_get(gp->draw, "sd");

   assert(sd != NULL);

   Evas_Event_Mouse_Down *emd = (Evas_Event_Mouse_Down *)event_info;

   if (emd->button != 1) return;
   sd->selected = gp;
   evas_object_smart_callback_call(sd->obj, SIGNAL_GROUPVIEW_CLICKED, gp->part);
}

static Evas_Object *
_conteiner_cell_sizer_add(Groupview_Smart_Data *sd, Groupview_Part *gp, unsigned int index)
{
   Evas_Object *cell_content;
   Eina_Stringshare *item_source;
   int min_w, min_h, max_w, max_h, w, h;

   cell_content = edje_object_add(evas_object_evas_get(sd->obj));
   item_source = edje_edit_part_item_index_source_get(sd->group->edit_object, gp->part->common.name, index);
   edje_object_file_set(cell_content, ap.project->dev, item_source);
   eina_stringshare_del(item_source);
   /* hide this object, it need only for calculate cell size */
   evas_object_hide(cell_content);

   min_w = edje_edit_part_item_index_min_w_get(sd->group->edit_object, gp->part->common.name, index);
   min_h = edje_edit_part_item_index_min_h_get(sd->group->edit_object, gp->part->common.name, index);

   // Calculation according to box/table item implementation in efl 1.13 at edje_load.c
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

   max_w = edje_edit_part_item_index_max_w_get(sd->group->edit_object, gp->part->common.name, index);
   max_h = edje_edit_part_item_index_max_h_get(sd->group->edit_object, gp->part->common.name, index);
   evas_object_size_hint_max_set(cell_content, max_w, max_h);

   return cell_content;
}

static void
_part_table_items_add(Groupview_Smart_Data *sd, Groupview_Part *gp, Eina_List ***items_draw, int col, int row)
{
   Evas_Object *cell, *cell_content;
   Eina_List *l;
   Part_Item2 *part_item;
   int i, j;
   unsigned char span_col, span_row;
   Groupview_Item *item;

   assert(gp->container != NULL);
   assert(gp->items == NULL);

   evas_object_table_clear(gp->container, true);

   for (i = 0; i < col; i++)
     {
        for (j = 0; j < row; j++)
          {
             if (items_draw[i][j] == (Eina_List *) -1) continue;

             span_col = 1;
             span_row = 1;

             cell = elm_layout_add(sd->parent);
             evas_object_size_hint_align_set(cell, EVAS_HINT_FILL, EVAS_HINT_FILL);

             elm_layout_theme_set(cell, "layout", "groupview", "default");
             evas_object_show(cell);
             elm_object_signal_emit(cell, "border,part_item", "eflete");
             EINA_LIST_FOREACH(items_draw[i][j], l, part_item)
               {
                  span_col = edje_edit_part_item_index_span_col_get(sd->group->edit_object, gp->part->common.name, part_item->common.id);
                  span_row = edje_edit_part_item_index_span_row_get(sd->group->edit_object, gp->part->common.name, part_item->common.id);

                  cell_content = _conteiner_cell_sizer_add(sd, gp, part_item->common.id);
                  evas_object_table_pack(gp->container, cell_content, i, j, span_col, span_row);

                  item = mem_malloc(sizeof(Groupview_Item));
                  item->name = eina_stringshare_add(part_item->common.name);
                  item->layout = cell;
                  gp->items = eina_list_append(gp->items, item);
               }
             evas_object_table_pack(gp->container, cell, i, j, span_col, span_row);
          }
     }
}

static void
_part_table_add(Groupview_Smart_Data *sd, Groupview_Part *gp)
{

   Eina_List *l;
   Part_Item2 *item;
   int span_c, span_r, col, row, col_pos, row_pos, i, j;
   const Evas_Object *table;
   Eina_List ***items_draw;

   assert(gp->container == NULL);

   gp->container = evas_object_table_add(evas_object_evas_get(sd->obj));
   elm_box_pack_before(gp->draw, gp->container, gp->proxy_part);
   evas_object_show(gp->container);

   table = edje_object_part_object_get(sd->group->edit_object, gp->part->common.name);
   evas_object_table_col_row_size_get(table, &col, &row);
   items_draw = (Eina_List ***)mem_calloc(1, sizeof(Eina_List **) * col);
   for (i = 0; i < col; i++)
     items_draw[i] = (Eina_List **)mem_calloc(1, sizeof(Eina_List *) * row);
   EINA_LIST_FOREACH(gp->part->items, l, item)
     {
        col_pos = edje_edit_part_item_index_position_col_get(sd->group->edit_object, gp->part->common.name, item->common.id);
        row_pos = edje_edit_part_item_index_position_row_get(sd->group->edit_object, gp->part->common.name, item->common.id);
        span_c = edje_edit_part_item_index_span_col_get(sd->group->edit_object, gp->part->common.name, item->common.id);
        span_r = edje_edit_part_item_index_span_row_get(sd->group->edit_object, gp->part->common.name, item->common.id);

        if (items_draw[col_pos][row_pos] == (Eina_List *)-1) items_draw[col_pos][row_pos] = NULL;
        items_draw[col_pos][row_pos] = eina_list_append(items_draw[col_pos][row_pos], item);
        for (i = col_pos; i < (col_pos + span_c); i++)
          {
             for (j = row_pos; j < (row_pos + span_r); j++)
               if (!items_draw[i][j]) items_draw[i][j] = (Eina_List *) -1;
          }
     }
   _part_table_items_add(sd, gp, items_draw, col, row);

   for (i = 0; i < col; i++)
     {
        for (j = 0; j < row; j++)
          {
             if (items_draw[i][j] == (Eina_List *) -1) continue;
             eina_list_free(items_draw[i][j]);
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

   if (name)
     {
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
     }
   else
     return _edje_box_layout_builtin[0].cb;
   return NULL;
}

static void
_part_box_add(Groupview_Smart_Data *sd, Groupview_Part *gp)
{
   Eina_List *l;
   Part_Item2 *part_item;
   int i, spread_w, spread_h;
   Evas_Object *cell, *cell_content;
   Groupview_Item *item;

   assert(gp->container == NULL);

   gp->container = evas_object_box_add(evas_object_evas_get(sd->obj));
   elm_box_pack_before(gp->draw, gp->container, gp->proxy_part);
   evas_object_show(gp->container);

   EINA_LIST_FOREACH(gp->part->items, l, part_item)
     {
        spread_w = edje_edit_part_item_index_spread_w_get(sd->group->edit_object, gp->part->common.name, part_item->common.id);
        spread_h = edje_edit_part_item_index_spread_h_get(sd->group->edit_object, gp->part->common.name, part_item->common.id);
        for (i = 0; i < (spread_w * spread_h); i++)
          {
             cell = elm_layout_add(sd->parent);
             evas_object_size_hint_align_set(cell, EVAS_HINT_FILL, EVAS_HINT_FILL);
             elm_layout_theme_set(cell, "layout", "groupview", "default");
             evas_object_show(cell);
             elm_object_signal_emit(cell, "border,part_item", "eflete");

             cell_content = _conteiner_cell_sizer_add(sd, gp, part_item->common.id);
             elm_object_content_set(cell, cell_content);
             evas_object_box_append(gp->container, cell);
             if (i == 0)
               {
                  item = mem_malloc(sizeof(Groupview_Item));
                  item->name = eina_stringshare_add(part_item->common.name);
                  item->layout = cell;
                  gp->items = eina_list_append(gp->items, item);
               }
          }
     }
}

static void
_box_param_update(Groupview_Smart_Data *sd, Groupview_Part *gp)
{
   double align_x = 0, align_y = 0;
   int pad_x, pad_y;
   Eina_Stringshare *layout;

   assert(sd != NULL);
   assert(gp != NULL);

   PART_STATE_GET(sd->group->edit_object, gp->part->common.name)

   layout = edje_edit_state_box_layout_get(sd->group->edit_object, gp->part->common.name, state, value);
   evas_object_box_layout_set(gp->container, _edje_box_layout_builtin_find(layout), NULL, NULL);

   align_x = edje_edit_state_container_align_x_get(sd->group->edit_object, gp->part->common.name, state, value);
   align_y = edje_edit_state_container_align_y_get(sd->group->edit_object, gp->part->common.name, state, value);
   evas_object_box_align_set(gp->container, align_x, align_y);

   pad_x = edje_edit_state_container_padding_x_get(sd->group->edit_object, gp->part->common.name, state, value);
   pad_y = edje_edit_state_container_padding_y_get(sd->group->edit_object, gp->part->common.name, state, value);
   evas_object_box_padding_set(gp->container, pad_x, pad_y);

   PART_STATE_FREE
}

static void
_part_container_del(Groupview_Part *gp)
{
   Groupview_Item *item;
   assert(gp->container != NULL);

   EINA_LIST_FREE(gp->items, item)
     _item_draw_del(item);

   elm_box_unpack(gp->draw, gp->container);
   evas_object_del(gp->container);
   gp->container = NULL;
}

static void
_part_calc(Groupview_Smart_Data *sd, Groupview_Part *gp)
{
   Evas_Coord x, y, xe, ye, w, h, we, he;
   int protrusion;

   assert(sd != NULL);
   assert(gp != NULL);

   edje_object_part_geometry_get(sd->group->edit_object, gp->part->common.name, &x, &y, &w, &h);
   evas_object_geometry_get(sd->group->edit_object, &xe, &ye, &we, &he);

   gp->geom.x = x * sd->zoom + xe;
   gp->geom.y = y * sd->zoom + ye;
   gp->geom.w = w * sd->zoom;
   gp->geom.h = h * sd->zoom;

   protrusion = abs((x <= 0) ? x : 0);
   sd->protrusion.x = (protrusion > sd->protrusion.x) ? protrusion : sd->protrusion.x;
   protrusion = abs((y <= 0) ? y : 0);
   sd->protrusion.y = (protrusion > sd->protrusion.y) ? protrusion : sd->protrusion.y;
   protrusion = ((x + w) > he) ? (x + w - we) : 0;
   sd->protrusion.w = (protrusion > sd->protrusion.w) ? protrusion : sd->protrusion.w;
   protrusion = ((y + h) > he) ? (y + h - he) : 0;
   sd->protrusion.h = (protrusion > sd->protrusion.h) ? protrusion : sd->protrusion.h;

   if ((gp->part->type == EDJE_PART_TYPE_TEXT) ||
       (gp->part->type == EDJE_PART_TYPE_TEXTBLOCK))
     {
        double x_align, y_align;
        const Evas_Object *ro;
        Evas_Coord ro_w, ro_h;
        ro = edje_object_part_object_get(sd->group->edit_object, gp->part->common.name);
        evas_object_geometry_get(ro, NULL, NULL, &ro_w, &ro_h);
        evas_object_resize(gp->proxy_part, ro_w * sd->zoom, ro_h * sd->zoom);

        x_align = edje_edit_state_text_align_x_get(sd->group->edit_object,
                                                   gp->part->common.name,
                                                   gp->part->current_state->common.name,
                                                   gp->part->current_state->val);
        y_align = edje_edit_state_text_align_y_get(sd->group->edit_object,
                                                   gp->part->common.name,
                                                   gp->part->current_state->common.name,
                                                   gp->part->current_state->val);
        evas_object_size_hint_align_set(gp->proxy_part, x_align, y_align);
     }
}

static Eina_Bool
_part_update(Groupview_Smart_Data *sd, Groupview_Part *gp)
{
   Eina_Stringshare *str;

   assert(sd != NULL);

   if (!sd->parts) return false;

   switch (gp->part->type)
     {
      case EDJE_PART_TYPE_TEXT:
      case EDJE_PART_TYPE_TEXTBLOCK:
         str = edje_edit_state_text_get(sd->group->edit_object,
                                        gp->part->common.name,
                                        gp->part->current_state->common.name,
                                        gp->part->current_state->val);
         if (!str || !strcmp(str, ""))
           edje_object_part_text_set(sd->group->edit_object, gp->part->common.name, gp->part->common.name);
         else
           edje_object_part_text_set(sd->group->edit_object, gp->part->common.name, str);
         eina_stringshare_del(str);
         _common_param_update(gp, sd->group->edit_object);
         break;
      case EDJE_PART_TYPE_VECTOR:
      case EDJE_PART_TYPE_RECTANGLE:
      case EDJE_PART_TYPE_GROUP:
         _common_param_update(gp, sd->group->edit_object);
         break;
      case EDJE_PART_TYPE_IMAGE:
         _image_param_update(gp, sd);
         break;
      case EDJE_PART_TYPE_PROXY:
         _proxy_param_update(gp, sd->group->edit_object);
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
   //evas_object_smart_callback_call(sd->obj, SIG_GEOMETRY_CHANGED, (void *)sd->real_size);

   return true;
}

static Groupview_Part *
_part_draw_add(Groupview_Smart_Data *sd, Part2 *part)
{
   Groupview_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = mem_calloc(1, sizeof(Groupview_Part));
   gp->part = part;

   gp->draw = elm_box_add(sd->parent);
   elm_box_layout_set(gp->draw, evas_object_box_layout_stack, NULL, NULL);
   evas_object_show(gp->draw);
   elm_box_pack_end(sd->box, gp->draw);

   gp->layout = elm_layout_add(sd->parent);
   evas_object_size_hint_weight_set(gp->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(gp->layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_layout_theme_set(gp->layout, "layout", "groupview", "default");
   evas_object_show(gp->layout);
   elm_box_pack_end(gp->draw, gp->layout);

   gp->part->visible = true;

#define PART_VIEW_PROXY_SET() \
   gp->proxy_part = evas_object_image_filled_add(evas_object_evas_get(sd->obj)); \
   evas_object_size_hint_weight_set(gp->proxy_part, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(gp->proxy_part, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_box_pack_end(gp->draw, gp->proxy_part); \
   evas_object_show(gp->proxy_part); \

   switch (part->type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
         PART_VIEW_PROXY_SET()
         break;
      case EDJE_PART_TYPE_TEXT:
      case EDJE_PART_TYPE_TEXTBLOCK:
      case EDJE_PART_TYPE_VECTOR:
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,text", "eflete");
         break;
      case EDJE_PART_TYPE_IMAGE:
         PART_VIEW_PROXY_SET()
         gp->proxy_dead_part = evas_object_image_filled_add(evas_object_evas_get(sd->obj));
         elm_object_signal_emit(gp->layout, "border,text", "eflete");
         break;
      case EDJE_PART_TYPE_PROXY: // it part like image
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,proxy", "eflete");
         break;
      case EDJE_PART_TYPE_SWALLOW:
         elm_object_signal_emit(gp->layout, "border,swallow", "eflete");
         break;
      case EDJE_PART_TYPE_SPACER:
         elm_object_signal_emit(gp->layout, "border,spacer", "eflete");
         break;
      case EDJE_PART_TYPE_GROUP:
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,group", "eflete");
         break;
      case EDJE_PART_TYPE_TABLE:
         PART_VIEW_PROXY_SET()
         elm_object_signal_emit(gp->layout, "border,table", "eflete");
         _part_table_add(sd, gp);
         break;
      case EDJE_PART_TYPE_BOX:
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
         gp->draw = evas_object_rectangle_add(evas_object_evas_get(sd->obj));
         evas_object_color_set(gp->draw, 0, 0, 0, 0);
         break;
     }

   evas_object_data_set(gp->draw, "sd", sd);
   if (!sd->group->main_group)
     evas_object_event_callback_add(gp->draw, EVAS_CALLBACK_MOUSE_DOWN, _part_select, gp);

   return gp;
}

static void
_part_draw_del(Groupview_Smart_Data *sd, Part2 *part)
{
   Groupview_Part *gp;

   assert(sd != NULL);
   assert(part != NULL);

   gp = _parts_list_find(sd->parts, part->common.name);

   assert(gp != NULL);

   sd->parts = eina_list_remove(sd->parts, gp);
   _groupview_part_free(gp);
}

static void
_item_draw_del(Groupview_Item *item)
{
   assert(item != NULL);

   eina_stringshare_del(item->name);
   /* no sence to delete ge_item->layout, because we recreate table/box on add
    * and delete item event. It will be deleted with conteiner object. */
   free(item);
}

static inline void
_color_apply(Groupview_Part *gp, Evas_Object *edit_obj, const char *state, double value)
{
   int r, g, b, a;
   int cr, cg, cb, ca;
   Eina_Stringshare *color_class;

   assert(gp != NULL);
   assert(edit_obj != NULL);
   assert(state != NULL);

   edje_edit_state_color_get(edit_obj, gp->part->common.name, state, value, &r, &g, &b, &a);
   color_class = edje_edit_state_color_class_get(edit_obj, gp->part->common.name, state, value);
   if (color_class)
     {
        if (edje_edit_color_class_colors_get(edit_obj, color_class, &cr, &cg, &cb, &ca,
                                             NULL, NULL, NULL, NULL,
                                             NULL, NULL, NULL, NULL))
          {
             r = ((cr + 1) * r) >> 8;
             g = ((cg + 1) * g) >> 8;
             b = ((cb + 1) * b) >> 8;
             a = ((ca + 1) * a) >> 8;
          }
        eina_stringshare_del(color_class);
     }
   evas_color_argb_premul(a, &r, &g, &b);
   evas_object_color_set(gp->draw, r, g, b, a);
}

static void
_image_proxy_common_param_update(Evas_Object *image, Groupview_Part *gp, Evas_Object *edit_obj)
{
   int x, y, w, h;
   int img_w, img_h;
   double fill_w, fill_h, fill_x, fill_y;
   int fill_origin_offset_x, fill_origin_offset_y, fill_size_offset_x, fill_size_offset_y;

   assert(image != NULL);
   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->part->common.name)

   _color_apply(gp, edit_obj, state, value);

   /* setups settings from filled block  into evas image object*/
   evas_object_image_smooth_scale_set(image,
            edje_edit_state_fill_smooth_get(edit_obj, gp->part->common.name, state, value));

   /* take fill params here, because need to know is default params set*/
   fill_x = edje_edit_state_fill_origin_relative_x_get(edit_obj, gp->part->common.name, state, value);
   fill_y = edje_edit_state_fill_origin_relative_y_get(edit_obj, gp->part->common.name, state, value);
   fill_w = edje_edit_state_fill_size_relative_x_get(edit_obj, gp->part->common.name, state, value);
   fill_h = edje_edit_state_fill_size_relative_y_get(edit_obj, gp->part->common.name, state, value);
   fill_origin_offset_x = edje_edit_state_fill_origin_offset_x_get(edit_obj, gp->part->common.name, state, value);
   fill_origin_offset_y = edje_edit_state_fill_origin_offset_y_get(edit_obj, gp->part->common.name, state, value);
   fill_size_offset_x = edje_edit_state_fill_size_offset_x_get(edit_obj, gp->part->common.name, state, value);
   fill_size_offset_y = edje_edit_state_fill_size_offset_y_get(edit_obj, gp->part->common.name, state, value);
   if (edje_edit_state_fill_type_get(edit_obj, gp->part->common.name, state, value))
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
_image_param_update(Groupview_Part *gp, Groupview_Smart_Data *sd)
{
   Evas_Object *edit_obj = sd->group->edit_object;
   const char *buf = NULL;
   int bl, br, bt, bb;
   int img_w = 0, img_h = 0;
   void *image_data = NULL;
   const Evas_Object *part_image = NULL;
   Evas_Colorspace cspace;
   unsigned char middle;
   Eina_Bool isalpha = false;
    /* map values */
   Evas_Map *m;
   Eina_Bool map_on;
   const char *perpective = NULL, *light = NULL, *rot_part = NULL;
   double rotX, rotY, rotZ;
   int center_x, center_y, center_z; /* for rotations */
   int persp_x, persp_y;
   int rx, ry, rw, rh, w, h, xe, ye; /* for perspective */
   int lx, ly, lz, lr, lg, lb, lar, lag, lab; /* colors and geom for light */
   int r, g, b, a;
   int iw = 1, ih = 1;
   int zplane = 0, focal = 1000;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->part->common.name)
   part_image = edje_object_part_object_get(edit_obj, gp->part->common.name);
   evas_object_image_source_set(gp->proxy_dead_part, NULL);
   evas_object_image_source_set(gp->proxy_dead_part, (Evas_Object *) part_image);
   evas_object_image_source_visible_set(gp->proxy_dead_part, false);
   evas_object_hide(gp->proxy_dead_part);

   evas_object_image_size_get(part_image, &img_w, &img_h);
   if (img_w == 0 && img_h == 0)
     {
        WARN("Image part[%s] size is 0x0. Image wouldn't draw.", gp->part->common.name);
        return;
     }
   evas_object_image_size_set(gp->proxy_part, img_w, img_h);
   cspace = evas_object_image_colorspace_get(part_image);
   evas_object_image_colorspace_set(gp->proxy_part, cspace);
   isalpha = evas_object_image_alpha_get(part_image);
   evas_object_image_alpha_set(gp->proxy_part, isalpha);
   image_data = evas_object_image_data_get(part_image, false);
   if (!image_data)
     {
        ERR("Image part[%s] doesn't contain any raw image data!", gp->part->common.name);
        return;
     }
   evas_object_image_data_set(gp->proxy_part, image_data);

   edje_edit_state_image_border_get(edit_obj, gp->part->common.name, state, value,
                                    &bl, &br, &bt, &bb);
   evas_object_image_border_set(gp->proxy_part, bl, br, bt, bb);

   middle  = edje_edit_state_image_border_fill_get(edit_obj, gp->part->common.name, state, value);
   if (middle == 0)
     evas_object_image_border_center_fill_set(gp->proxy_part, EVAS_BORDER_FILL_NONE);
   else if (middle == 1)
     evas_object_image_border_center_fill_set(gp->proxy_part, EVAS_BORDER_FILL_DEFAULT);
   else if (middle == 2)
     evas_object_image_border_center_fill_set(gp->proxy_part, EVAS_BORDER_FILL_SOLID);

   _image_proxy_common_param_update(gp->proxy_part, gp, edit_obj);

   /* ===================================================================
      WORKING WITH MAP!
      All that happening in here just because image part can't be proxed.
      Proxy doesn't apply somehow all maps and borders etc, so we need to
      calculate it with our own hands.

      This calculation was taken from edje_calc.c functions
      =================================================================== */
   map_on = edje_edit_state_map_on_get(edit_obj, gp->part->common.name, state, value);
   if (map_on)
     {
        edje_object_part_geometry_get(edit_obj, gp->part->common.name, NULL, NULL, &w, &h);
        rot_part = edje_edit_state_map_rotation_center_get(edit_obj, gp->part->common.name, state, value);

        if (rot_part)
          edje_object_part_geometry_get(edit_obj, rot_part, &rx, &ry, &rw, &rh);
        else
          edje_object_part_geometry_get(edit_obj, gp->part->common.name, &rx, &ry, &rw, &rh);

        evas_object_geometry_get(edit_obj, &xe, &ye, NULL, NULL);

        center_x = xe + rx * sd->zoom + (rw * sd->zoom / 2);
        center_y = ye + ry * sd->zoom + (rh * sd->zoom / 2);
        center_z = 0;

        m = evas_map_new(4);
        evas_map_smooth_set(m, edje_edit_state_map_smooth_get(edit_obj, gp->part->common.name, state, value));
        evas_map_alpha_set(m, edje_edit_state_map_alpha_get(edit_obj, gp->part->common.name, state, value));
        evas_map_util_points_populate_from_object(m, gp->proxy_part);

        evas_object_image_size_get(gp->proxy_part, &iw, &ih);
        evas_map_point_image_uv_set(m, 0, 0.0, 0.0);
        evas_map_point_image_uv_set(m, 1, iw, 0.0);
        evas_map_point_image_uv_set(m, 2, iw, ih);
        evas_map_point_image_uv_set(m, 3, 0.0, ih);

        /* map color */
        edje_edit_state_map_point_color_get(edit_obj, gp->part->common.name, state, value, 0, &r, &g, &b, &a);
        evas_map_point_color_set(m, 0, r, g, b, a);
        edje_edit_state_map_point_color_get(edit_obj, gp->part->common.name, state, value, 1, &r, &g, &b, &a);
        evas_map_point_color_set(m, 1, r, g, b, a);
        edje_edit_state_map_point_color_get(edit_obj, gp->part->common.name, state, value, 2, &r, &g, &b, &a);
        evas_map_point_color_set(m, 2, r, g, b, a);
        edje_edit_state_map_point_color_get(edit_obj, gp->part->common.name, state, value, 3, &r, &g, &b, &a);
        evas_map_point_color_set(m, 3, r, g, b, a);

        /* zoom */
        TODO("Implement edje edit API and apply it here")
        evas_map_util_zoom(m, 1.0, 1.0, center_x, center_y);

        /* rotate */
        edje_edit_state_map_rotation_get(edit_obj, gp->part->common.name, state, value, &rotX, &rotY, &rotZ);
        evas_map_util_3d_rotate(m, rotX, rotY, rotZ, center_x, center_y, center_z);

        /* calculate perspective point */
        if (edje_edit_state_map_perspective_on_get(edit_obj, gp->part->common.name, state, value))
          {
             perpective = edje_edit_state_map_perspective_get(edit_obj, gp->part->common.name, state, value);
             if (perpective)
               {
                  edje_object_part_geometry_get(edit_obj, perpective, &rx, &ry, &rw, &rh);
                  zplane = edje_edit_state_map_perspective_zplane_get(edit_obj, perpective, state, value);
                  focal = edje_edit_state_map_perspective_focal_get(edit_obj, perpective, state, value);

                  persp_x = xe + rx * sd->zoom + (rw * sd->zoom / 2);
                  persp_y = ye + ry * sd->zoom + (rh * sd->zoom / 2);
               }
             else
               {
                  persp_x = xe + (w / 2);
                  persp_y = ye + (h / 2);
               }
             evas_map_util_3d_perspective(m, persp_x, persp_y, zplane, focal);
          }

        /* calculate light color & position etc. if there is one */
        light = edje_edit_state_map_light_get(edit_obj, gp->part->common.name, state, value);
        if (light)
          {
             edje_object_part_geometry_get(edit_obj, light, &rx, &ry, &rw, &rh);
             edje_edit_state_color_get(edit_obj, light, state, value, &lr, &lg, &lb, NULL);
             /* outline because color2 being used in edje_calc */
             edje_edit_state_outline_color_get(edit_obj, light, state, value, &lar, &lag, &lab, NULL);
             lx = xe + rx * sd->zoom + (rw * sd->zoom / 2);
             ly = ye + ry * sd->zoom + (rh * sd->zoom / 2);
             lz = edje_edit_state_map_perspective_zplane_get(edit_obj, perpective, state, value);
             evas_map_util_3d_lighting(m, lx, ly, lz, lr, lg, lb, lar, lag, lab);
          }

        /* handle backface culling (object is facing away from view */
        if (edje_edit_state_map_backface_cull_get(edit_obj, gp->part->common.name, state, value))
          {
             if (evas_map_util_clockwise_get(m))
               evas_object_show(gp->proxy_part);
             else evas_object_hide(gp->proxy_part);
          }

        evas_object_map_set(gp->proxy_part, m);
     }
   evas_object_map_enable_set(gp->proxy_part, map_on);

   PART_STATE_FREE
   eina_stringshare_del(buf);
}

static void
_proxy_param_update(Groupview_Part *gp, Evas_Object *edit_obj)
{
   const char *proxy_source;
   Groupview_Smart_Data *sd;
   Groupview_Part *source;

   assert(gp != NULL);
   assert(edit_obj != NULL);

   PART_STATE_GET(edit_obj, gp->part->common.name)

   sd = evas_object_data_get(gp->draw, "sd");
   proxy_source = edje_edit_state_proxy_source_get(edit_obj, gp->part->common.name, state, value);
   if (proxy_source)
     {
        elm_object_signal_emit(gp->layout, "border,default", "eflete");
        source = _parts_list_find(sd->parts, proxy_source);
        if (source)
          {
             evas_object_image_source_set(gp->proxy_part, source->proxy_part);
             evas_object_image_source_clip_set(gp->proxy_part, false);
             evas_object_image_source_visible_set (gp->proxy_part, false);
             _image_proxy_common_param_update(gp->proxy_part, gp, edit_obj);
             return; /* no need in unsetting */
          }
     }
   /* if part was deleted or no proxy set yet we go in this code line */
   evas_object_image_source_set(gp->proxy_part, NULL);
   elm_object_signal_emit(gp->layout, "border,proxy", "eflete");
}

static void
_common_param_update(Groupview_Part *gp, Evas_Object *edit_obj)
{
   assert(gp != NULL);
   assert(edit_obj != NULL);

   evas_object_image_source_set(gp->proxy_part, NULL);
   evas_object_image_source_set(gp->proxy_part,
                                (Evas_Object *)edje_object_part_object_get(edit_obj, gp->part->common.name));
   evas_object_image_source_visible_set(gp->proxy_part, false);
}

static void
_table_param_update(Groupview_Smart_Data *sd, Groupview_Part *gp)
{
   double align_x = 0, align_y = 0;
   int pad_x, pad_y;
   unsigned char homogeneous;

   assert(sd != NULL);
   assert(gp != NULL);

   PART_STATE_GET(sd->group->edit_object, gp->part->common.name)

   homogeneous = edje_edit_state_table_homogeneous_get(sd->group->edit_object, gp->part->common.name, state, value);
   evas_object_table_homogeneous_set(gp->container, homogeneous);

   align_x = edje_edit_state_container_align_x_get(sd->group->edit_object, gp->part->common.name, state, value);
   align_y = edje_edit_state_container_align_y_get(sd->group->edit_object, gp->part->common.name, state, value);
   evas_object_table_align_set(gp->container, align_x, align_y);

   pad_x = edje_edit_state_container_padding_x_get(sd->group->edit_object, gp->part->common.name, state, value);
   pad_y = edje_edit_state_container_padding_y_get(sd->group->edit_object, gp->part->common.name, state, value);
   evas_object_table_padding_set(gp->container, pad_x, pad_y);

   PART_STATE_FREE
}

void
_part_object_area_calc(Groupview_Smart_Data *sd, Groupview_Part *gp)
{
   Eina_Stringshare *rel_to;
   int xc, yc, wc, hc;
   int x = 0, w = 0, y = 0, h = 0;
   double relative;
   int offset;

   Groupview_Part *rel_part = NULL;

   assert(sd != NULL);

   PART_STATE_GET(sd->group->edit_object, gp->part->common.name)
   const char *name = gp->part->common.name;

   xc = sd->geom.x;
   wc = sd->geom.w;
   rel_to = edje_edit_state_rel1_to_x_get(sd->group->edit_object, name, state, value);
   relative = edje_edit_state_rel1_relative_x_get(sd->group->edit_object, name, state, value);
   offset = edje_edit_state_rel1_offset_x_get(sd->group->edit_object, name, state, value);
   if (rel_to)
     {
        rel_part = _parts_list_find(sd->parts, rel_to);
        evas_object_geometry_get(rel_part->draw, &xc, NULL, &wc, NULL);
     }
   x = xc + (int)(wc * relative) + offset;
   edje_edit_string_free(rel_to);

   yc = sd->geom.y;
   hc = sd->geom.h;
   rel_to = edje_edit_state_rel1_to_y_get(sd->group->edit_object, name, state, value);
   relative = edje_edit_state_rel1_relative_y_get(sd->group->edit_object, name, state, value);
   offset = edje_edit_state_rel1_offset_y_get(sd->group->edit_object, name, state, value);
   if (rel_to)
     {
        rel_part = _parts_list_find(sd->parts, rel_to);
        evas_object_geometry_get(rel_part->draw, NULL, &yc, NULL, &hc);
     }
   y = yc + (int)(hc * relative) + offset;
   edje_edit_string_free(rel_to);

   xc = sd->geom.x;
   wc = sd->geom.w;
   rel_to = edje_edit_state_rel2_to_x_get(sd->group->edit_object, name, state, value);
   relative = edje_edit_state_rel2_relative_x_get(sd->group->edit_object, name, state, value);
   offset = edje_edit_state_rel2_offset_x_get(sd->group->edit_object, name, state, value);
   if (rel_to)
     {
        rel_part = _parts_list_find(sd->parts, rel_to);
        evas_object_geometry_get(rel_part->draw, &xc, NULL, &wc, NULL);
     }
   w = (xc - x) + (int)(wc * relative) + offset;
   if (w < 0) { x += w; w = 0; }
   edje_edit_string_free(rel_to);

   yc = sd->geom.y;
   hc = sd->geom.h;
   rel_to = edje_edit_state_rel2_to_y_get(sd->group->edit_object, name, state, value);
   relative = edje_edit_state_rel2_relative_y_get(sd->group->edit_object, name, state, value);
   offset = edje_edit_state_rel2_offset_y_get(sd->group->edit_object, name, state, value);
   if (rel_to)
     {
        rel_part = _parts_list_find(sd->parts, rel_to);
        evas_object_geometry_get(rel_part->draw, NULL, &yc, NULL, &hc);
     }
   h = (yc - y) + (int)(hc * relative) + offset;
   if (h < 0) { y += h; h = 0; }
   edje_edit_string_free(rel_to);

   PART_STATE_FREE

   gp->object_area_geom.x = x;
   gp->object_area_geom.y = y;
   gp->object_area_geom.w = w;
   gp->object_area_geom.h = h;
}

void
_parts_stack_layout(Evas_Object          *o __UNUSED__,
                    Evas_Object_Box_Data *p __UNUSED__,
                    void                 *data)

{
   Groupview_Smart_Data *sd = data;
   Groupview_Part *gp;
   Eina_List *l;

   DBG("Recalc %p object. Object parts count: %d", sd->obj, eina_list_count(sd->parts))
   sd->protrusion.x = 0;
   sd->protrusion.y = 0;
   sd->protrusion.w = 0;
   sd->protrusion.h = 0;
   EINA_LIST_FOREACH(sd->parts, l, gp)
     {
        _part_calc(sd, gp);
        _part_update(sd, gp);

        if (gp->part->visible)
          {
             evas_object_resize(gp->draw, gp->geom.w, gp->geom.h);
             evas_object_move(gp->draw, gp->geom.x, gp->geom.y);
          }

        if (gp->container)
          {
             evas_object_resize(gp->container, gp->geom.w, gp->geom.h);
             evas_object_move(gp->container, gp->geom.x, gp->geom.y);
          }
        evas_object_raise(gp->draw);
     }

   if (sd->selected)
     {
        _part_object_area_calc(sd, sd->selected);
        evas_object_geometry_set(sd->highlight,
                                 sd->selected->geom.x,
                                 sd->selected->geom.y,
                                 sd->selected->geom.w,
                                 sd->selected->geom.h);
        if (sd->obj_area_visible)
          evas_object_geometry_set(sd->object_area,
                                   sd->selected->object_area_geom.x,
                                   sd->selected->object_area_geom.y,
                                   sd->selected->object_area_geom.w,
                                   sd->selected->object_area_geom.h);
     }

   sd->manual_calc = false;
}
