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

#ifndef PROPERTY_PRIVATE_H
#define PROPERTY_PRIVATE_H

typedef struct {
   Evas_Object *genlist;
   Property_Mode mode;

   Elm_Genlist_Item_Class *itc_caption;
} Property_Data;

extern Property_Data pd;

typedef struct _Property_Attribute Property_Attribute;
typedef void (* Property_Cb) (Property_Attribute *);
typedef Eina_List * (* Property_Expand_Cb) (Property_Attribute *);

typedef struct {
   Evas_Object *control; /**< pointer to widget */
   Property_Cb start_cb;
   Property_Cb change_cd;
   Property_Cb stop_cb;
} Property_Action;

struct _Property_Attribute {
   Eina_Stringshare *name;
   Attribute type;
   void *data;
   Elm_Genlist_Item_Class *itc;
   Elm_Object_Item *glit; /**< reference to gennlist item iteself */
   Property_Cb update_cb; /**< called when item is realized and control's values should be updated */
   Property_Action action1; /**< first control */
   Property_Action action2; /**< second control */
   Property_Expand_Cb expand_cb; /**< if not NULL - item is tree node. This cb will be called to get subitems */
   Eina_Bool expanded : 1; /**< if true initial state will be expanded */
   Eina_Bool realized : 1; /**< if item is not realized update callback will not be called */
};
#endif /* PROPERTY_PRIVATE_H */
