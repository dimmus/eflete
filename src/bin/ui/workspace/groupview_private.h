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

#include "eflete.h"
#include "highlight.h"
#include "resource_manager2.h"

typedef struct _Groupview_Smart_Data Groupview_Smart_Data;
typedef struct _Groupview_Part Groupview_Part;
typedef struct _Groupview_Item Groupview_Item;

static const char SIG_PART_SEPARETE_OPEN[] = "parts,separete,open";
static const char SIG_PART_SEPARETE_CLOSE[] = "parts,separete,close";
static const char SIG_OBJ_AREA_CHANGED[] = "object,area,changed";
static const char SIG_GEOMETRY_CHANGED[] = "geometry,changed";
static const char TEXT_TOOLTIP[] = "gs.current.size.tooltip";
static const char SWALLOW_FOR_EDIT[] = "gs.swallow.edit";

/* smart callbacks coming from elm button objects (besides the ones)
 * coming from elm layout): */
static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   /* about types see here: http://docs.enlightenment.org/auto/evas/group__Evas__Keys.html#gaf0d4ce3d62a068eab1b89a34abb056ad */
   {SIG_PART_SEPARETE_OPEN, ""},
   {SIG_PART_SEPARETE_CLOSE, ""},
   {SIG_OBJ_AREA_CHANGED, "(iiii)"},
   {SIG_GEOMETRY_CHANGED, "(iiii)"},
   {NULL, NULL}
};

struct _Groupview_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Group2 *group;
   Evas_Object *obj;
   Evas_Object *event;
   Evas_Object *parent;
   Evas_Object *box;
   Groupview_Geom geom;
   Groupview_Geom protrusion;
   Groupview_Geom zoom1;
   Eina_List *parts;
   Groupview_Part *selected;
   Evas_Object *highlight;
   Evas_Object *object_area;
   Eina_Bool obj_area_visible : 1;
   Eina_Bool separated : 1;
   Eina_Bool manual_calc : 1;
   double zoom;
};

#define GROUPVIEW_DATA_GET(o, ptr)  \
   assert(o != NULL); \
   Groupview_Smart_Data *ptr = evas_object_smart_data_get(o); \
   assert(ptr != NULL);

/**
 * Groupview part this struct contains all object for draw edit object part in
 * groupview object.
 *
 * The evas objects of groupview has next stack:
 *    in normal mode:
 *       - border;
 *       - draw;
 *    in separete mode:
 *       - item;
 *       - border;
 *       - draw;
 */
struct _Groupview_Part
{
   Part2 *part;               /**< Pointer to part */
   Groupview_Geom geom;
   Groupview_Geom object_area_geom;
   Evas_Object *draw;         /**< The evas primitive to be draw in groupview.
                                   The valid evas object types: image, rectangle,
                                   text and textblock.*/
   Evas_Object *proxy_part, *proxy_dead_part;
   Evas_Object *layout;
   Evas_Object *container;    /**< Used for box/table parts */
   Eina_List *items;          /**< The items, for TABLE, BOX part types */
   Groupview_Item *current_item;
};

struct _Groupview_Item
{
   Eina_Stringshare *name;    /**< The item name.  */
   Evas_Object *layout;       /**< The item border and hilight */
};

void
_parts_list_new(Groupview_Smart_Data *sd);

void
_parts_list_free(Groupview_Smart_Data *sd);

Groupview_Part *
_parts_list_find(Eina_List *parts, const char *part);

Groupview_Item *
_part_item_search(Eina_List *items, const char *item_name);

Eina_Bool
_edit_object_part_add(Groupview_Smart_Data *sd, Part2 *part);

Eina_Bool
_edit_object_part_del(Groupview_Smart_Data *sd, Part2 *part);

void
_select_item_move_to_top(Groupview_Smart_Data *sd);

void
_selected_item_return_to_place(Groupview_Smart_Data *sd);

Eina_Bool
_edit_object_part_item_del(Groupview_Smart_Data *sd, Eina_Stringshare *part,
                           Eina_Stringshare *item);

void
_parts_stack_layout(Evas_Object          *o,
                    Evas_Object_Box_Data *p,
                    void                 *data);

void
_part_object_area_calc(Groupview_Smart_Data *sd, Groupview_Part *gp);

/**
 * Stack part below below in groupview module.
 * If part_below param is NULL, then part will restack below by one position,
 * in stack, else part will restack below part_below.
 * Restack complete in three steps:
 * - restack part in edje edit object. (Used edje_view_part_restack* func's).
 * - restack part in parts list, which stored in groupview smart data.
 * - restack evas primitive of part at canvas (Used evas_object_stack* func's).
 *
 * @param sd The groupview smart data structure.
 * @param part The name of part to stack below.
 * @param part_below The name of part below which to stack.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupview
 */
Eina_Bool
_edit_object_part_restack_below(Groupview_Smart_Data *sd,
                                const char *part,
                                const char *part_below);

