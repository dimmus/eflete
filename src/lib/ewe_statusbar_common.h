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

#ifndef EWE_STATUSBAR_COMMON_H
#define EWE_STATUSBAR_COMMON_H

/**
 * Defines items align into statusbar
 *
 * @ingroup Statusbar
 */
typedef enum
{
   EWE_STATUSBAR_ITEMS_ALIGN_RIGHT = 0,
   EWE_STATUSBAR_ITEMS_ALIGN_CENTER = 1,
   EWE_STATUSBAR_ITEMS_ALIGN_LEFT = 2,
   EWE_STATUSBAR_ITEMS_ALIGN_LAST
} Ewe_Statusbar_Items_Align;


/**
 * Defines statusbar item types.
 *
 * @ingroup Statusbar
 */
typedef enum
{
   EWE_STATUSBAR_ITEM_TYPE_OBJECT = 0, /**<  item with content */
   EWE_STATUSBAR_ITEM_TYPE_SEPARATOR = 1 /**< separate item */
} Ewe_Statusbar_Items_Type;

/**
 * Structure data for statusbar item.
 */
typedef struct _Ewe_Statusbar_Item Ewe_Statusbar_Item;

struct _Ewe_Statusbar_Item
{
   Evas_Object *parent; /**< the pointer to parent statusbar object */
   Evas_Object *content; /**< the object, which added into item */
   Evas_Object *markup; /**< the object which load markup from edj file */

   Ewe_Statusbar_Items_Type type; /**< item type: separator or item witch content*/

   Evas_Smart_Cb func; /**< callback function, which call when item clicked */
   void *cb_data; /**< data, which will be sended into item callback */


   Eina_Bool disabled : 1; /**< the item disabled status: EINA_TRUE - enabled */
   int id; /**< the item identificator */
   int width; /**< the item width, if less that 0, then ulimited width will set*/
};

#define DEFAULT_ITEM_WIDTH 150
#define SEPARATOR_WIDTH 2

/**
 * Get the index of the statusbar item.
 *
 * @param item a statusbar item.
 *
 * @return the identificator of the statusbar item.
 *
 * @ingroup Statusbar
 */
EAPI int
ewe_statusbar_item_id_get(Ewe_Statusbar_Item *item);

/**
 * Get the width of the statusbar item.
 *
 * @param item a statusbar item.
 *
 * @return the width of the item (-1 is unlimited width)).
 *
 * @ingroup Statusbar
 */
EAPI int
ewe_statusbar_item_width_get(Ewe_Statusbar_Item *item);

/**
 * Set the width for the statusbar item.
 *
 * @param item a statusbar item.
 * @param width a width, which will be set as fixed for statusbar item. If
 *               param equal -1, then width of statusbar item will be unlimited.
 *
 * @return EINA_TRUE if new width set sucessful, or EINA_FALSE in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_item_width_set(Ewe_Statusbar_Item *item, int width);

/**
 * Get text from item label.
 *
 * @param item a statusbar item.
 *
 * @return The currently displayed text or NULL on errors.
 *
 * @ingroup Statusbar
 */
EAPI char *
ewe_statusbar_item_label_get(Ewe_Statusbar_Item *item);

/**
 * Set the object as content into statusbar item.
 *
 * @param item a statusbar item.
 * @param label The label to be displayed.
 *
 * @return EINA_TRUE if content set sucessful, or EINA_FALSE in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_item_label_set(Ewe_Statusbar_Item *item, const char *label);

/**
 * Get the object, which stored into statusbar item.
 *
 * @param item a statusbar item.
 *
 * @return pointer to Evas_Object, which was setted into item, or NULL in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Evas_Object *
ewe_statusbar_item_content_get(Ewe_Statusbar_Item *item);

/**
 * Set the object as content into statusbar item.
 *
 * @param item a statusbar item.
 * @param content a Evas_Object, which will be stored as content in statusbar item.
 *
 * @return EINA_TRUE if content set sucessful, or EINA_FALSE in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_item_content_set(Ewe_Statusbar_Item *item, Evas_Object *content);

/**
 * Unset content object from statusbar item.
 *
 * @param item a statusbar item.
 *
 * @return pointer to Evas_Object, which was setted into item, or NULL in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Evas_Object *
ewe_statusbar_item_content_unset(Ewe_Statusbar_Item *item);

/**
 * Get the statusbar object from statusbar item.
 *
 * @param item a statusbar item.
 *
 * @return a pointer to statusbar Evas_Object, or NULL in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Evas_Object *
ewe_statusbar_item_statusbar_get(Ewe_Statusbar_Item *item);

#endif /* EWE_STATUSBAR_COMMON_H */
