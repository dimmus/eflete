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

#define RESOURCE_COMMON \
   Eina_Stringshare *name; \
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

/**
 * @struct _External_Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _External_Resource
{
   RESOURCE_COMMON;

   Eina_Stringshare *source;
};

/**
 * @struct _Tone_Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _Tone_Resource
{
   RESOURCE_COMMON;

   int freq;
};

struct _State
{
   RESOURCE_COMMON;

   Eina_Stringshare *parsed_name;  /**< parsed state name */
   double parsed_val;          /**< parsed state value */
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
   Part *current_part;        /**< pointer to selected part */
   Program *current_program;  /**< pointer to selected program */
   Resource *current_group_data;  /**< pointer to selected group_data */

   History *history;          /**< history of changes in the group */
};

/**
 * @struct _Colorclass_Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
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

int
resource_cmp(Resource *res1, Resource *res2);

/**
 * Find resource in sorted list by its name.
 *
 * @param list Resources list
 * @param name Name of the resource to be found
 *
 * @return pointer to resource or NULL if it was not found
 *
 * @ingroup ProjectManager.
 */
void *
pm_resource_get(Eina_List *list, Eina_Stringshare *name);

/**
 * Find resource in not sorted list by its name.
 *
 * @param list Resources list
 * @param name Name of the resource to be found
 *
 * @return pointer to resource or NULL if it was not found
 *
 * @ingroup ProjectManager.
 */
void *
pm_resource_unsorted_get(Eina_List *list, Eina_Stringshare *name);

/**
 * Add reference to resource with info where it is used (i.e. part for images)
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used
 *
 * @ingroup ProjectManager.
 */
Eina_Bool
pm_resource_usage_add(Eina_List *list, Eina_Stringshare *name, void *usage_data);

/**
 * Add reference to resource with info where it is used (i.e. part for images)
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used
 *
 * @ingroup ProjectManager.
 */
Eina_Bool
pm_resource_usage_unsorted_add(Eina_List *list, Eina_Stringshare *name, void *usage_data);

/**
 * Remove resource.
 *
 * @param list Resources list
 * @param res Resource structure
 *
 * @ingroup ProjectManager.
 */
Eina_List *
pm_resource_del(Eina_List *list, void *res);

/**
 * Remove reference to resource.
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used. Must be added to usage list.
 *
 * @ingroup ProjectManager.
 */
void
pm_resource_usage_del(Eina_List *list, Eina_Stringshare *name, void *usage_data);

/**
 * Remove reference to resource.
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used. Must be added to usage list.
 *
 * @ingroup ProjectManager.
 */
void
pm_resource_usage_unsorted_del(Eina_List *list, Eina_Stringshare *name, void *usage_data);

#endif /* RESOURCE_MANAGER_H */
