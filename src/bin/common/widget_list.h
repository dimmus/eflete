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

#ifndef WIDGET_LIST_H
#define WIDGET_LIST_H

struct _Tree_Item_Data
{
   const char *name;
   Eina_Bool check;
   Eina_List *list;
   Eina_List *copy;
};
typedef struct _Tree_Item_Data Tree_Item_Data;

struct _End_Item_Data
{
   const char *name;
   Eina_Bool check;
};
typedef struct _End_Item_Data End_Item_Data;

Eina_Stringshare *widget_name_get(const Eina_Stringshare *group_name);
Eina_Stringshare *style_name_get(const Eina_Stringshare *group_name);
Eina_Stringshare *item_style_name_get(const Eina_Stringshare *group_name, Eina_Stringshare *style_name);
Eina_Stringshare *option_widget_name_get(const char *str, Eina_List **style_list);
Eina_Stringshare *option_style_name_get(const char *str, Eina_List **item_style_list, Eina_List **cp_style_list);
Eina_Stringshare *option_item_style_name_get(const char *str, Eina_List **cp_style_list);
Eina_List *widget_prefix_list_get(Eina_List *collections, const char *widget_name, const char *style_name);

/*
 * Get the part of group name, based of the depth level.
 * Func parce and return the part of group name.
 *
 * @note Given group name 'elm/button/base/default', level is 2. Funcx will
 * return 'elm/button'.
 *
 * @param group_name The group name;
 * @param level The depth of prefix;
 * @param len The lenght of finded prefix.
 */
Eina_Stringshare *
widget_prefix_get(const char *group_name, int level, int *len);

/*
 * Get the lists of groups with given prefix.
 *
 * @param groups The list of @Group;
 * @param prefix The string with prefix;
 * @param folders_out The list of finded groups which contained more group;
 * @param groups_out The list of @Group object.
 */
void
widget_tree_items_get(Eina_List *groups,
                      const char *prefix,
                      Eina_List **folders_out,
                      Eina_List **groups_out);

#endif /* WIDGET_LIST_H */
