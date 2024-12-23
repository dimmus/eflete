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

#ifndef EWE_RULER_COMMON_H
#define EWE_RULER_COMMON_H

typedef struct _Ewe_Ruler_Scale Ewe_Ruler_Scale;
struct _Ewe_Ruler_Scale
{
   Evas_Object       *box;
   Eina_List         *dashes;
   Eina_List         *markers;
   Eina_Stringshare  *format;
   Eina_Stringshare  *style;
   Eina_Stringshare  *full_style;
   double            mark_value_step;
   int               mark_step;
   int               zero_pos;
   Eina_Bool         visible : 1;
   Eina_Bool         show_middle_mark : 1;
};

typedef struct _Ewe_Ruler_Marker Ewe_Ruler_Marker;
struct _Ewe_Ruler_Marker
{
   Evas_Object       *obj;
   Ewe_Ruler_Scale   *scale;
   Eina_Stringshare  *style;
   double            rel_position;
   int               abs_position;
   int               size;
   Eina_Bool         visible : 1;
   Eina_Bool         relative : 1;
};

#endif /* EWE_RULER_COMMON_H */
