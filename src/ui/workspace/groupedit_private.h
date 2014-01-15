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

#define EDJE_EDIT_IS_UNSTABLE_AND_I_KNOW_ABOUT_IT
#include "Edje_Edit.h"
#include "Elementary.h"
#include "groupedit.h"
#include "common_macro.h"
#include "logger.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

typedef struct _Ws_Groupedit_Smart_Data Ws_Groupedit_Smart_Data;
typedef struct _Groupedit_Part Groupedit_Part;

static const char SIG_CHANGED[] = "container,changed";
static const char SIG_PART_SEPARETE_OPEN[] = "parts,separete,open";
static const char SIG_PART_SEPARETE_CLOSE[] = "parts,separete,close";
static const char SIG_PART_ACTIVATED[] = "part,activated";
static const char SIG_PART_SELECTED[] = "part,selected";
static const char SIG_OBJ_AREA_CHANGED[] = "object,area,changed";
static const char TEXT_TOOLTIP[] = "gs.current.size.tooltip";
static const char SWALLOW_FOR_EDIT[] = "gs.swallow.edit";

/* smart callbacks coming from elm button objects (besides the ones)
 * coming from elm layout): */
static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   /* about types see here: http://docs.enlightenment.org/auto/evas/group__Evas__Keys.html#gaf0d4ce3d62a068eab1b89a34abb056ad */
   {SIG_CHANGED, "(iiii)"},
   {SIG_PART_SEPARETE_OPEN, ""},
   {SIG_PART_SEPARETE_CLOSE, "s"},
   {SIG_PART_ACTIVATED, ""},
   {SIG_PART_SELECTED, ""},
   {SIG_OBJ_AREA_CHANGED, "(iiii)"},
   {NULL, NULL}
};

struct _Ws_Groupedit_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas *e;
   Evas_Object *obj;
   Evas_Object *container;
   Evas_Object *parent;
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
   const char *style;
   Evas_Object *edit_obj;
   const char *edit_obj_file;
   Eina_List *parts;
   struct {
      Evas_Object *obj;
      Groupedit_Part *gp;
      Groupedit_Geom *geom;
      Eina_Bool visible : 1;
   } obj_area;
   Eina_Bool separeted : 1;
   Groupedit_Part *selected;
   Groupedit_Part *to_select;
   Evas_Object *bg;
   Evas_Coord downx;
   Evas_Coord downy;
};

#define WS_GROUPEDIT_DATA_GET(o, ptr)  \
   Ws_Groupedit_Smart_Data *ptr = evas_object_smart_data_get(o);

#define WS_GROUPEDIT_DATA_GET_OR_RETURN_VAL(o, ptr, val)                \
   WS_GROUPEDIT_DATA_GET(o, ptr)                                        \
   if (!ptr)                                                            \
     {                                                                  \
        fprintf(stderr, "No Groupedit data for object %p (%s)!",        \
                o, evas_object_type_get(o));                            \
        return val;                                                     \
     }

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
 *       - clipper;
 *       - bg.
 */
struct _Groupedit_Part
{
   Eina_Stringshare *name;    /**< The part name.  */
   Evas_Object *draw;         /**< The evas primitive to be draw in groupedit.
                                   The valid evas object types: image, rectangle,
                                   text and textblock.*/
   Eina_Bool visible : 1;     /**< Visible or not on canvas. */
   Evas_Object *border;       /**< The object border, use for next part types:
                                   TEXT, TEXTBLOCK, SWALLOW, SPACER. This object
                                   created only for these parts types, in other
                                   cases object is NULL. */
   Evas_Object *item;         /**< The object border in the separete mode */
   Evas_Object *bg;           /**< The item background, it object overlaps
                                   enother groupedit part. */
   Evas_Object *clipper;      /**< The background clipper, need to draw item bg
                                   in the separete mode.*/
};

void
_parts_list_new(Ws_Groupedit_Smart_Data *sd);

void
_parts_list_free(Ws_Groupedit_Smart_Data *sd);

Groupedit_Part *
_parts_list_find(Eina_List *parts, const char *part);

void
_parts_recalc(Ws_Groupedit_Smart_Data *sd);

void
_edit_object_load(Ws_Groupedit_Smart_Data *sd);

Eina_Bool
_edit_object_part_add(Ws_Groupedit_Smart_Data *sd, const char *part,
                      Edje_Part_Type type, const char *data);

Eina_Bool
_edit_object_part_del(Ws_Groupedit_Smart_Data *sd, const char *part);

void
_select_item_move_to_top(Ws_Groupedit_Smart_Data *sd);

void
_selected_item_return_to_place(Ws_Groupedit_Smart_Data *sd);
