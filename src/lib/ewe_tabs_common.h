/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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

#ifndef EWE_TABS_COMMON_H
#define EWE_TABS_COMMON_H

typedef struct _Ewe_Tabs_Item Ewe_Tabs_Item;

struct _Ewe_Tabs_Item
{
   Evas_Object          *owner;
   Evas_Object          *head;
   Evas_Object          *content;
   Eina_Stringshare     *title;
   Eina_Stringshare     *style;
   Eina_Bool            disabled;
};

#endif /* EWE_TABS_COMMON_H */
