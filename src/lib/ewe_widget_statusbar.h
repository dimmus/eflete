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

#ifndef EWE_WIDGET_STATUSBAR_H
#define EWE_WIDGET_STATUSBAR_H

#include "ewe_statusbar.h"
/**
 * @addtogroup Widget
 * @{
 *  FIXME: add some description here.
 *
 */


/**
 * Smart data for statusbar widget.
 */
typedef struct _Ewe_Statusbar_Smart_Data Ewe_Statusbar_Smart_Data;

struct _Ewe_Statusbar_Smart_Data
{
   Evas_Object *obj; /**< the object itself */
   Evas_Object *markup; /**< the object which load markup from edj file */
   Evas_Object *box; /**< container for items */
   int height; /**< max height of statusbar. Value load from style of statusbar */
   Eina_List *items; /**< the list of items */
};


/**
 * @}
 */

#define EWE_STATUSBAR_DATA_GET(o, sd) \
  Ewe_Statusbar_Smart_Data *sd = eo_data_scope_get(o, EWE_STATUSBAR_CLASS)

#define EWE_STATUSBAR_DATA_GET_OR_RETURN(o, ptr) \
  EWE_STATUSBAR_DATA_GET(o, ptr); \
  if (EINA_UNLIKELY(!ptr)) \
    { \
       CRI("No widget data for object %p (%s)", \
           o, evas_object_type_get(o)); \
       return; \
    }

#define EWE_STATUSBAR_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  EWE_STATUSBAR_DATA_GET(o, ptr); \
  if (EINA_UNLIKELY(!ptr)) \
    { \
       CRI("No widget data for object %p (%s)", \
           o, evas_object_type_get(o)); \
       return val; \
    }

#define EWE_STATUSBAR_CHECK(obj) \
  if (EINA_UNLIKELY(!eo_isa((obj), EWE_STATUSBAR_CLASS))) return

#endif /* EWE_WIDGET_STATUSBAR_H */
