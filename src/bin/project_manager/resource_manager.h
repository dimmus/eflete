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

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "eflete.h"

typedef enum
{
   RESOURCE_TYPE_NONE,
   RESOURCE_TYPE_GROUP,
   RESOURCE_TYPE_PART,
   RESOURCE_TYPE_STATE,
   RESOURCE_TYPE_PROGRAM,
   RESOURCE_TYPE_ITEM,
   RESOURCE_TYPE_DATA,
   RESOURCE_TYPE_IMAGE,
   RESOURCE_TYPE_SOUND,
   RESOURCE_TYPE_TONE,
   RESOURCE_TYPE_STYLE,
   RESOURCE_TYPE_TAG,
   RESOURCE_TYPE_COLORCLASS,
   RESOURCE_TYPE_FONT,
} Resource_Type;

#define RESOURCE_COMMON \
   Eina_Stringshare *name; \
   Resource_Type resource_type; \
   Eina_List *used_in; \

/**
 * @struct _Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _Resource
{
   RESOURCE_COMMON;
};

struct _Part_Item
{
   RESOURCE_COMMON;

   Part *part;
};

struct _External_Resource
{
   RESOURCE_COMMON;

   Eina_Stringshare *source;
};

struct _Tone_Resource
{
   RESOURCE_COMMON;

   int freq;
};

struct _State
{
   RESOURCE_COMMON;

   double val;                /**< parsed state value */
   Part *part;                /**< pointer to part */
};

struct _Part
{
   RESOURCE_COMMON;

   Edje_Part_Type type;       /**< part type */
   Eina_List *states;         /**< list of states */
   State *current_state;      /**< pointer to selected state */
   Eina_List *items;          /**< list of item names. Used only for BOX and TABLE parts */
   Eina_Stringshare * current_item_name; /**< name of selected item */
   Group *group;              /**< pointer to group */
   Eina_Bool visible;         /**< is part visible on workspace*/
};

struct _Program
{
   RESOURCE_COMMON;

   Edje_Action_Type type;
   Eina_List *targets;
   Eina_List *afters;
};

struct _Group
{
   RESOURCE_COMMON;

   Eina_Stringshare *widget;  /**< parsed widget name */
   Eina_Stringshare *class;   /**< parsed class name */
   Eina_Stringshare *style;   /**< parsed style name */
   Group *main_group;         /**< pointer to main group. NULL if group is not an alias */
   Eina_List *aliases;        /**< list of pointers to aliases. NULL if group is an alias */
   Eina_List *parts;          /**< list of parts */
   Eina_List *programs;       /**< list of programs */
   Eina_List *data_items;     /**< list of data */

   Evas_Object *edit_object;  /**< object needed to access group with edje_edit functions. Should be NULL if group is not open */
   Resource *current_selected;

   History *history;          /**< history of changes in the group */
};

struct _Colorclass_Resource
{
   RESOURCE_COMMON;

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

/**
 * @typedef Colorclass_Resource
 * @ingroup ProjectManager
 */
typedef struct _Colorclass_Resource Colorclass_Resource;

/**
 * @typedef Tone_Resource
 * @ingroup ProjectManager
 */
typedef struct _Tone_Resource Tone_Resource;

/**
 * @typedef External_Resource
 * @ingroup ProjectManager
 */
typedef struct _External_Resource External_Resource;

Resource *
resource_add(const char *name, Resource_Type type);

void
resource_free(Resource *res);

Resource *
resource_get(const Eina_List *list, const Resource *request);

void
resource_insert(Eina_List **list, const Resource *res);

void
resource_insert_after(Eina_List **list, const Resource *after, const Resource *res);

Resource *
resource_remove(Eina_List **list, const Resource *res);

void
resource_used_in_add(Resource *res, const Resource *used_in);

void
resource_used_in_del(Resource *res, const Resource *used_in);

Eina_Bool
resource_used_in_get(Resource *res, const Resource *used_in);

#endif /* RESOURCE_MANAGER_H */
