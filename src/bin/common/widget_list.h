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
};
typedef struct _Tree_Item_Data Tree_Item_Data;

struct _End_Item_Data
{
   const char *name;
   Eina_Bool check;
};
typedef struct _End_Item_Data End_Item_Data;

const char *widget_name_get(const Eina_Stringshare *group_name);
const char *style_name_get(const Eina_Stringshare *group_name);
const char *item_style_name_get(const Eina_Stringshare *group_name, Eina_List *style_list);
Eina_Bool style_name_check(const Eina_Stringshare *group_name, const char *style_name);
Eina_Bool item_style_name_check(const Eina_Stringshare *group_name, const char *style_name, Eina_List *style_list);
const char *option_widget_name_get(const char *str, Eina_List **style_list);

#endif /* WIDGET_LIST_H */
