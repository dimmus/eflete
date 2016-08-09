/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#ifndef RESOURCE_MANAGER2_H
#define RESOURCE_MANAGER2_H

#include "eflete.h"

enum _Resource_Type
{
   RESOURCE_TYPE_GROUP = 0,
   RESOURCE_TYPE_PART,
   RESOURCE_TYPE_STATE,
   RESOURCE_TYPE_ITEMS,
   RESOURCE_TYPE_PROGRAM,
   RESOURCE_TYPE_LIMIT, /* not yet */
   RESOURCE_TYPE_DATA_GLOBAL,
   RESOURCE_TYPE_DATA_GROUP,

   /* other managers */
   RESOURCE_TYPE_DATA_IMAGE,
   RESOURCE_TYPE_DATA_IMAGE_SET,
   RESOURCE_TYPE_DATA_SAMPLE,
   RESOURCE_TYPE_DATA_TONE,
   RESOURCE_TYPE_DATA_FONT,
   RESOURCE_TYPE_DATA_COLORCLASS,
   RESOURCE_TYPE_DATA_STYLE,
   RESOURCE_TYPE_DATA_SIZECLASS,
   RESOURCE_TYPE_DATA_TEXTCLASS,
   RESOURCE_TYPE_DATA_VIBRO, /* something for now? */
   RESOURCE_TYPE_LAST
};
typedef enum _Resource_Type Resource_Type;

struct _Resource_Internal
{
   unsigned int id;
   Eina_Stringshare *name;
   Resource_Type type;
   Eina_List *used_in; /* where is this resource actually used in? */
   Eina_List *uses___; /* what kind of reources this res is actually usiung now? */
};
typedef struct _Resource_Internal Resource_Internal;

/*********************************************/
/*********************************************/
/******* SOME COMMON RESOURCE STRUCTS ********/
/*********************************************/
/*********************************************/

struct _Resource
{
   Resource_Internal common;
};

struct _Group
{
   Resource_Internal common;
   Eina_List *parts;
   Eina_List *programs;
   Eina_List *data_items;
   Eina_List *limits;
};

struct _Part
{
   Resource_Internal common;
   Edje_Part_Type type;       /**< part type */
   Eina_List *states;         /**< list of states */
   State *current_state;      /**< pointer to selected state */
   Eina_List *items;          /**< list of item names. Used only for BOX and TABLE parts */
   Eina_Stringshare * current_item_name; /**< name of selected item */
   Group *group;              /**< pointer to group */
   Eina_Bool visible;         /**< is part visible on workspace*/
};

struct _State
{
   Resource_Internal common;
   double val;                /**< parsed state value */
   Part *part;                /**< pointer to part */
};

struct _Part_Item
{
   Resource_Internal common;
   Part *part;
};

struct _Program
{
   Resource_Internal common;
   Edje_Action_Type type;
   Eina_List *targets;
   Eina_List *afters;
};

struct _Style
{
   Resource_Internal common;
   Eina_Stringshare *raw_style; //for set to textblock and to edje_edit

   /* all parsed values goes below */
   Eina_Stringshare *font;
   int font_size;
   int font_style_weight;
   int font_style_width;
   struct {
      int r, g, b, a;
   } color, bg_color, outer_color, inner_color, shadow_color;
   int font_align_hor;
   int font_valign_hor;
   int left_margin, right_margin;
   int wrap;
   int linerelsize, linesize, tabstops;
   Eina_Bool pass, bg_check, ellipsis_check;
   int ellipsis_value;
   int glow_style;
   int direction;
   Eina_Bool strikethrough_check;
   struct {
      int r, g, b, a;
   } underone_color, undertwo_color, strikethrough_color, dash_color;
   int underline;
   int dash_width, dash_gap;
};

struct _Image
{
   Resource_Internal common;
   const char *source;
   Edje_Edit_Image_Comp comp_type;
   int quality;
   Eina_Bool is_used;
   Eina_Bool is_set;
   int height, width;
};

/**************************************************/

typedef struct _Resource Resource;
typedef struct _Group Group;
typedef struct _Part Part;
typedef struct _Part_Item Part_Item;
typedef struct _State State;
typedef struct _Part Part;
typedef struct _Program Program;
typedef struct _Style Style;
typedef struct _Image Image;

/**************************************************/

Eina_Bool
resource_manager_init(Project *project);

Eina_Bool
resource_manager_shutdown(Project *project);

Resource *
resource_manager_find(const Eina_List *list, Eina_Stringshare *name);

Resource *
resource_manager_v_find(const Eina_List *list, Eina_Stringshare *name, double value);

#endif /* RESOURCE_MANAGER2_H */
