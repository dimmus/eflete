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

#ifndef EWE_WIDGET_TABS_H
#define EWE_WIDGET_TABS_H

#include "ewe_tabs.h"

typedef struct _Ewe_Tabs_Smart_Data Ewe_Tabs_Smart_Data;
struct _Ewe_Tabs_Smart_Data
{
   Eo                   *obj;
   Evas_Object          *layout;
   Evas_Object          *items_box;
   Eina_List            *items;
   Ewe_Tabs_Item        *active_item;
   Eina_Bool            horizontal;

   Eina_Stringshare     *style;
};

#define EWE_TABS_DATA_GET(o, sd) \
  Ewe_Tabs_Smart_Data * sd = eo_data_scope_get(o, EWE_TABS_CLASS)

#define EWE_TABS_CHECK(obj) \
  if (EINA_UNLIKELY(!eo_isa((obj), EWE_TABS_CLASS))) \
    return

#endif /* EWE_WIDGET_TABS_H */
