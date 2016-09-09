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

enum _Resource2_Type
{
   RESOURCE2_TYPE_GROUP = 0,
   RESOURCE2_TYPE_PART,
   RESOURCE2_TYPE_STATE,
   RESOURCE2_TYPE_ITEM,
   RESOURCE2_TYPE_PROGRAM,
   RESOURCE2_TYPE_LIMIT, /* not yet */
   RESOURCE2_TYPE_DATA_GLOBAL,
   RESOURCE2_TYPE_DATA_GROUP,

   /* other managers */
   RESOURCE2_TYPE_IMAGE,
   RESOURCE2_TYPE_IMAGE_SET,
   RESOURCE2_TYPE_SAMPLE,
   RESOURCE2_TYPE_TONE,
   RESOURCE2_TYPE_FONT,
   RESOURCE2_TYPE_COLORCLASS,
   RESOURCE2_TYPE_STYLE,
   RESOURCE2_TYPE_SIZECLASS,
   RESOURCE2_TYPE_TEXTCLASS,
   RESOURCE2_TYPE_VIBRO, /* something for now? */
   RESOURCE2_TYPE_LAST
};
typedef enum _Resource2_Type Resource2_Type;

struct _Resource2_Internal
{
   unsigned int id;
   Eina_Stringshare *name;
   Resource2_Type type;
   Eina_List *used_in; /* where is this resource actually used in? */
   Eina_List *uses___; /* what kind of reources this res is actually usiung now? */
};
typedef struct _Resource2_Internal Resource2_Internal;

/*********************************************/
/*********************************************/
/******* SOME COMMON RESOURCE2 STRUCTS ********/
/*********************************************/
/*********************************************/
typedef struct _Resource2 Resource2;
typedef struct _Group2 Group2;
typedef struct _Part2 Part2;
typedef struct _Part_Item2 Part_Item2;
typedef struct _State2 State2;
typedef struct _Program2 Program2;
typedef struct _Style2 Style2;
typedef struct _Image2 Image2;
typedef struct _Image_Set2 Image_Set2;
typedef struct _Tone2 Tone2;
typedef struct _Colorclass2 Colorclass2;

typedef struct _Sound2 Sound2;
typedef struct _Sound2 Font2;
typedef struct _Sound2 Global_Data2;
typedef struct _Sound2 Group_Data2;

struct _Resource2
{
   Resource2_Internal common;
};

struct _Group2
{
   Resource2_Internal common;
   Eina_List *parts;
   Eina_List *programs;
   Eina_List *data_items;
   Eina_List *limits;
   Group2 *main_group;         /**< pointer to main group. NULL if group is not an alias */
   Eina_List *aliases;        /**< list of pointers to aliases. NULL if group is an alias */

   Eina_Stringshare *widget;  /**< parsed widget name */
   Eina_Stringshare *class;   /**< parsed class name */
   Eina_Stringshare *style;   /**< parsed style name */

   Evas_Object *edit_object;  /**< object needed to access group with edje_edit functions. Should be NULL if group is not open */
//   Resource *current_selected;
   History *history;          /**< history of changes in the group */
};

struct _Part2
{
   Resource2_Internal common;
   Edje_Part_Type type;       /**< part type */
   Eina_List *states;         /**< list of states */
   State2 *current_state;      /**< pointer to selected state */
   Eina_List *items;          /**< list of item names. Used only for BOX and TABLE parts */
   Eina_Stringshare * current_item_name; /**< name of selected item */
   Group2 *group;              /**< pointer to group */
   Eina_Bool visible;         /**< is part visible on workspace*/
};

struct _State2
{
   Resource2_Internal common;
   double val;                /**< parsed state value */

   Eina_Stringshare *normal;   /* normal image being used (could be NONE) */
   Eina_List *tweens;          /* list of tweens in correct order */
   Part2 *part;                /**< pointer to part */
};

struct _Part_Item2
{
   Resource2_Internal common;
   Eina_Stringshare *source;
   Part2 *part;
};

struct _Program2
{
   Resource2_Internal common;
   Edje_Action_Type type;
   Eina_List *targets;
   Eina_List *afters;
   Group2 *group;
};

struct _Style2
{
   Resource2_Internal common;
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

struct _Image2
{
   Resource2_Internal common;
   const char *source;
   Edje_Edit_Image_Comp comp_type;
   int quality;
   Eina_Bool is_used;
   int height, width;
};

struct _Image_Set2
{
   Resource2_Internal common;
   Eina_Bool is_used;
};

struct _Tone2
{
   Resource2_Internal common;
   int freq;
};

struct _Sound2
{
   Resource2_Internal common;
   const char *source;
};

struct _Colorclass2
{
   Resource2_Internal common;
   struct {
      int r,g,b,a;
   } color1;
   struct {
      int r,g,b,a;
   } color2;
   struct {
      int r,g,b,a;
   } color3;
};

Eina_Bool
resource_manager_init(Project *project);

Eina_Bool
resource_manager_shutdown(Project *project);

Resource2 *
resource_manager_find(const Eina_List *list, Eina_Stringshare *name);

Resource2 *
resource_manager_v_find(const Eina_List *list, Eina_Stringshare *name, double value);

Resource2 *
resource_manager_id_find(const Eina_List *list, unsigned int id);

#endif /* RESOURCE_MANAGER2_H */
