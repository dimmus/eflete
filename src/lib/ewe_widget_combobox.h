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

#ifndef EWE_WIDGET_COMBOBOX_H
#define EWE_WIDGET_COMBOBOX_H

#include "ewe_combobox.h"

/**
 * Base layout smart data extended with combobox instance data.
 */
typedef struct _Ewe_Combobox_Smart_Data Ewe_Combobox_Smart_Data;
struct _Ewe_Combobox_Smart_Data
{
   Eo                   *obj;
   Evas_Object          *combobox, *scroller, *expander;
   Evas_Object          *items_box;
   Evas_Object          *win, *top_win;
   Eina_List            *items;
   Ewe_Combobox_Item    *selected;
   Eina_Bool            expanded   : 1;
};

#define EWE_COMBOBOX_DATA_GET(o, sd) \
  Ewe_Combobox_Smart_Data * sd = eo_data_scope_get(o, EWE_COMBOBOX_CLASS)

#define EWE_COMBOBOX_DATA_GET_OR_RETURN(o, ptr) \
  EWE_COMBOBOX_DATA_GET(o, ptr); \
  if (EINA_UNLIKELY(!ptr)) \
    { \
       CRI("No widget data for object %p (%s)", \
           o, evas_object_type_get(o)); \
       return; \
    }

#define EWE_COMBOBOX_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  EWE_COMBOBOX_DATA_GET(o, ptr); \
  if (EINA_UNLIKELY(!ptr)) \
    { \
       CRI("No widget data for object %p (%s)", \
           o, evas_object_type_get(o)); \
       return val; \
    }

#define EWE_COMBOBOX_CHECK(obj) \
  if (!obj || EINA_UNLIKELY(!eo_isa((obj), EWE_COMBOBOX_CLASS))) \
    return

#endif /* EWE_WIDGET_COMBOBOX_H */
