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

enum _Property_Control {
   PROPERTY_CONTROL_NONE,

   PROPERTY_CONTROL_CHECK,
   PROPERTY_CONTROL_BUTTON,
   PROPERTY_CONTROL_COMBOBOX,
   PROPERTY_CONTROL_SPINNER,
   PROPERTY_CONTROL_ENTRY,
   PROPERTY_CONTROL_COLOR,
   PROPERTY_CONTROL_COLORSEL,

   PROPERTY_CONTROL_LAST
};
typedef enum _Property_Control Property_Control;
typedef struct _Property_Attribute Property_Attribute;
typedef struct _Property_Action Property_Action;
typedef struct {
   Evas_Object *genlist;
   Property_Mode mode;
   Property_Action *started_action;
   Property_Attribute *started_pa;

   Elm_Genlist_Item_Class *itc_caption;
   Elm_Genlist_Item_Class *itc_1swallow;
   Elm_Genlist_Item_Class *itc_2swallow;

   /* Stores what item class to use with each pair of controls */
   Elm_Genlist_Item_Class *item_classes[PROPERTY_CONTROL_LAST][PROPERTY_CONTROL_LAST];
} Property_Data;

extern Property_Data pd;

typedef void (* Property_Cb) (Property_Attribute *, Property_Action *);
typedef Eina_List * (* Property_Expand_Cb) (Property_Attribute *);

struct _Property_Action {
   Property_Control control_type;
   Eina_Stringshare *name;
   Eina_Stringshare *units;
   union {
      Attribute attribute; /**< type for group properties */
   } type; /**< submodule-specific enums */
   Evas_Object *control; /**< pointer to widget */
   Property_Cb init_cb;
   Property_Cb start_cb;
   Property_Cb change_cb;
   Property_Cb stop_cb;
   Property_Cb update_cb; /**< called when item is realized and control's values should be updated */
};

struct _Property_Attribute {
   Eina_Stringshare *name;
   Eina_Stringshare *icon_name;
   void *data;
   Elm_Object_Item *glit; /**< reference to gennlist item iteself */
   Property_Action action1; /**< first control */
   Property_Action action2; /**< second control */
   Property_Expand_Cb expand_cb; /**< This cb will be called to get subitems */
   Eina_Bool expandable : 1; /**< if true  item is tree node */
   Eina_Bool expanded : 1; /**< if true initial state will be expanded. */
   Eina_Bool realized : 1; /**< if item is not realized update callback will not be called */
};

/* color_control */
Evas_Object *
property_color_control_add(Evas_Object *parent);

void
property_color_control_color_set(Evas_Object *control, int r, int g, int b, int a);

void
property_color_control_color_get(Evas_Object *control, int *r, int *g, int *b, int *a);

/* dummy submodule. for implementation tests. will be deleted later */
void
property_dummy_init();

Eina_List *
property_dummy_items_get();

#endif /* PROPERTY_PRIVATE_H */
