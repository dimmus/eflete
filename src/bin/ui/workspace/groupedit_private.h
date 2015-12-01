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

#define EDJE_EDIT_IS_UNSTABLE_AND_I_KNOW_ABOUT_IT
#include "eflete.h"
#include "Edje_Edit.h"
#include "Elementary.h"
#include "groupedit.h"
#include "common_macro.h"
#include "logger.h"
#include "group_manager.h"

typedef struct _Ws_Groupedit_Smart_Data Ws_Groupedit_Smart_Data;
typedef struct _Groupedit_Part Groupedit_Part;
typedef struct _Groupedit_Item Groupedit_Item;

static const char SIG_CHANGED[] = "container,changed";
static const char SIG_PART_SEPARETE_OPEN[] = "parts,separete,open";
static const char SIG_PART_SEPARETE_CLOSE[] = "parts,separete,close";
static const char SIG_PART_UNSELECTED[] = "part,unselected";
static const char SIG_OBJ_AREA_CHANGED[] = "object,area,changed";
static const char SIG_GEOMETRY_CHANGED[] = "geometry,changed";
static const char TEXT_TOOLTIP[] = "gs.current.size.tooltip";
static const char SWALLOW_FOR_EDIT[] = "gs.swallow.edit";

/* smart callbacks coming from elm button objects (besides the ones)
 * coming from elm layout): */
static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   /* about types see here: http://docs.enlightenment.org/auto/evas/group__Evas__Keys.html#gaf0d4ce3d62a068eab1b89a34abb056ad */
   {SIG_CHANGED, "(iiii)"},
   {SIG_PART_SEPARETE_OPEN, ""},
   {SIG_PART_SEPARETE_CLOSE, ""},
   {SIG_PART_UNSELECTED, "s"},
   {SIG_OBJ_AREA_CHANGED, "(iiii)"},
   {SIG_GEOMETRY_CHANGED, "(iiii)"},
   {NULL, NULL}
};

struct _Ws_Groupedit_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Group *group;
   Evas *e;
   Evas_Object *obj;
   Evas_Object *event;
   Evas_Object *container;
   Evas_Object *parent;
   Evas_Object *box;
   /* Paddings which solve scroller issue,
      when container data move to 0,0 coords */
   struct {
      int t_left;
      int t_top;
      int bottom;
   } paddings;
   /* Minimal and maximum size of the container,
      i.e size of the edie_edit object */
   struct {
      int w; /* default: 0 */
      int h; /* default: 0 */
   } con_size_min;
   struct {
      int w; /* default: -1, size is not limited */
      int h; /* default: -1, size is not limited */
   } con_size_max;
   Groupedit_Geom *con_current_size;
   Groupedit_Geom *real_size;
   struct {
      Evas_Object *obj;
      int w, h;
   } handler_TL;
   struct{
      Evas_Object *obj;
      int w, h;
   } handler_BR;
   Eina_Bool handler_TL_pressed : 1;
   Eina_Bool handler_BR_pressed : 1;
   Evas_Object *edit_obj_clipper;
   Eina_List *parts;
   struct {
      Evas_Object *obj;
      Groupedit_Geom *geom;
      Eina_Bool visible : 1;
      Eina_Bool show_now : 1;
   } obj_area;
   Eina_Bool separated : 1;
   Groupedit_Part *selected;
   Groupedit_Part *to_select;
   Evas_Object *bg;  /**< The item background, it object overlaps
                         enother groupedit part. */
   Evas_Object *clipper;      /**< The background clipper, need to draw item bg
                                   in the separete mode.*/
   double zoom_factor; /**< current zoom factor. */
   Evas_Coord downx;
   Evas_Coord downy;
   Eina_Bool manual_calc : 1;
};

#define WS_GROUPEDIT_DATA_GET(o, ptr)  \
   assert(o != NULL); \
   Ws_Groupedit_Smart_Data *ptr = evas_object_smart_data_get(o); \
   assert(ptr != NULL);

/**
 * It a padding for parts layer in the separete mod.
 */
#define SEP_ITEM_PAD_X 50
#define SEP_ITEM_PAD_Y 30

/**
 * Groupedit part this struct contains all object for draw edit object part in
 * groupedit object.
 *
 * The evas objects of groupedit has next stack:
 *    in normal mode:
 *       - border;
 *       - draw;
 *    in separete mode:
 *       - item;
 *       - border;
 *       - draw;
 */
struct _Groupedit_Part
{
   Part_ *part;               /**< Pointer to part */
   Groupedit_Geom geom;
   Groupedit_Geom object_area_geom;
   Evas_Object *bg;           /**< The background, uses for container parts TABLE or BOX */
   Evas_Object *draw;         /**< The evas primitive to be draw in groupedit.
                                   The valid evas object types: image, rectangle,
                                   text and textblock.*/
   Evas_Object *proxy_part;
   Evas_Object *layout;
   Evas_Object *container;    /**< Used for box/table parts */
   Evas_Object *item;         /**< The object border in the separete mode */
   Eina_List *items;          /**< The items, for TABLE, BOX part types */
   Groupedit_Item *current_item;
};

struct _Groupedit_Item
{
   Eina_Stringshare *name;    /**< The item name.  */
   Evas_Object *layout;       /**< The item border and hilight */
};

void
_parts_list_new(Ws_Groupedit_Smart_Data *sd);

void
_parts_list_free(Ws_Groupedit_Smart_Data *sd);

Groupedit_Part *
_parts_list_find(Eina_List *parts, const char *part);

Groupedit_Item *
_part_item_search(Eina_List *items, const char *item_name);

Eina_Bool
_parts_recalc(Ws_Groupedit_Smart_Data *sd);

Eina_Bool
_edit_object_part_add(Ws_Groupedit_Smart_Data *sd, Part_ *part);

Eina_Bool
_edit_object_part_del(Ws_Groupedit_Smart_Data *sd, Part_ *part);

void
_select_item_move_to_top(Ws_Groupedit_Smart_Data *sd);

void
_selected_item_return_to_place(Ws_Groupedit_Smart_Data *sd);

Eina_Bool
_edit_object_part_item_del(Ws_Groupedit_Smart_Data *sd, Eina_Stringshare *part,
                           Eina_Stringshare *item);

void /* custom 'diagonal' layout */
_parts_stack_layout(Evas_Object          *o,
                    Evas_Object_Box_Data *p,
                    void                 *data);

/**
 * Stack part above above in groupedit module.
 * If part_above param is NULL, then part will restack above by one position,
 * in stack, else part will restack above part_above.
 * Restack complete in three steps:
 *  - restack part in edje edit object. (Used edje_edit_part_restack* func's).
 *  - restack part in parts list, which stored in groupedit smart data.
 *  - restack evas primitive of part at canvas (Used evas_object_stack* func's).
 *
 * @param sd The groupedit smart data structure.
 * @param part The name of part to stack above.
 * @param part_above The name of part above which to stack.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
_edit_object_part_restack_above(Ws_Groupedit_Smart_Data *sd,
                                const char *part,
                                const char *part_above);
/**
 * Stack part below below in groupedit module.
 * If part_below param is NULL, then part will restack below by one position,
 * in stack, else part will restack below part_below.
 * Restack complete in three steps:
 * - restack part in edje edit object. (Used edje_edit_part_restack* func's).
 * - restack part in parts list, which stored in groupedit smart data.
 * - restack evas primitive of part at canvas (Used evas_object_stack* func's).
 *
 * @param sd The groupedit smart data structure.
 * @param part The name of part to stack below.
 * @param part_below The name of part below which to stack.
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 * @ingroup Groupedit
 */
Eina_Bool
_edit_object_part_restack_below(Ws_Groupedit_Smart_Data *sd,
                                const char *part,
                                const char *part_below);

