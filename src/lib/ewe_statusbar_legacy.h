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

#ifndef EWE_STATUSBAR_LEGACY_H
#define EWE_STATUSBAR_LEGACY_H

/**
 * Add a new statusbar widget to the given parent object.
 *
 * @param parent The parent object
 * @return a new statusbar widget handle, or NULL, on errors.
 *
 * This function inserts a new statusbar widget on the canvas.
 *
 * @ingroup Statusbar
 */
EAPI Evas_Object *
ewe_statusbar_add(Evas_Object *parent);

/**
 * Remove all items from a given statusbar widget.
 *
 * @param obj The statusbar object
 *
 * @return EINA_TRUE if items clear sucessful, or EINA_FALSE in otherwise.
 *
 * This removes (and deletes) all items in given statusbar, leaving it empty.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_clear(Evas_Object *obj);

/**
 * Append a new item in a given statusbar widget.
 *
 * @param obj The statusbar object
 * @param content The object, which will be set into statusbar item as content.
 *                Can be NULL, if planned set content with
 *                @see ewe_statusbar_item_content_set .
 * @param type Item type.
 * @param func Convenience function called when the item is clicked.
 * @param func_data Data passed to func above.
 *
 * @return A handle to the item added or NULL if not possible
 *
 * This adds the new item to the end of the list childrens in statusbar widget.
 *
 * @ingroup Statusbar
 */
EAPI Ewe_Statusbar_Item *
ewe_statusbar_item_append(Evas_Object *obj, Evas_Object *content,
                          Ewe_Statusbar_Items_Type type,
                          Evas_Smart_Cb func, void *func_data);

/**
 * Prepend a new item in a given statusbar widget.
 *
 * @param obj The statusbar object
 * @param content The object, which will be set into statusbar item as content.
 *                Can be NULL, if planned set content with
 *                @see ewe_statusbar_item_content_set .
 * @param type Item type.
 * @param func Convenience function called when the item is clicked.
 * @param func_data Data passed to func above.
 *
 * @return A handle to the item added or NULL if not possible
 *
 * This adds the new item to the beginnig of the list childrens in statusbar widget.
 *
 * @ingroup Statusbar
 */
EAPI Ewe_Statusbar_Item *
ewe_statusbar_item_prepend(Evas_Object *obj, Evas_Object *content,
                          Ewe_Statusbar_Items_Type type,
                          Evas_Smart_Cb func, void *func_data);

/**
 * Insert a existent item before another in a given statusbar widget.
 *
 * @param obj The statusbar object
 * @param item The item to place before another item.
 * @param before The item to place this new one before.
 *
 * @return EINA_TRUE if item inserted sucessful, or EINA_FALSE in otherwise.
 *
 * This insert the existent item before another in the list childrens in statusbar widget.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_item_insert_before(Evas_Object *obj, Ewe_Statusbar_Item *item,
                                 Ewe_Statusbar_Item *before);

/**
 * Insert a existent item after another in a given statusbar widget.
 *
 * @param obj The statusbar object
 * @param item The item to place after another item.
 * @param after The item to place this new one before.
 *
 * @return EINA_TRUE if item inserted sucessful, or EINA_FALSE in otherwise.
 *
 * This insert the existent item after another in the list childrens in statusbar widget.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_item_insert_after(Evas_Object *obj, Ewe_Statusbar_Item *item,
                          Ewe_Statusbar_Item *after);

/**
 * Swap statusbar items positions into statusbar widgets.
 *
 * @param obj The statusbar object
 * @param item_first The item, which will be swapped.
 * @param item_second The item, which will be swapped.
 *
 * @return EINA_TRUE if swap is sucessful, or EINA_FALSE in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_items_swap(Evas_Object *obj,
						 Ewe_Statusbar_Item *item_first,
                         Ewe_Statusbar_Item *item_second);

/**
 * Retrieve a list of the items, which exist into the statusbar.
 *
 * @param obj The statusbar object
 *
 * @return a new Eina_List with a pointer to Ewe_Statusbar_Item in its nodes, or
 *           NULL on errors.
 *
 * @ingroup Statusbar
 */
EAPI const  Eina_List *
ewe_statusbar_items_list_get(Evas_Object *obj);

/**
 * Set the padding between the statusbar items.
 *
 * @param obj The statusbar object.
 * @param padding The space between items.
 *
 * @return EINA_TRUE if padding set sucessful, or EINA_FALSE in otherwise.
 *
 * Extra space in pixels that will be added between a statusbar items.
 * This padding is set for all items in the statusbar widget.
 *
 * @ingroup Statusbar
 */
EAPI void
ewe_statusbar_items_padding_set(Evas_Object *obj, int padding);

/**
 * Get the padding between the statusbar items.
 *
 * @param obj The statusbar object.
 *
 * @return padding size in pixels between items.
 *
 * @ingroup Statusbar
 */
EAPI int
ewe_statusbar_items_padding_get(const Evas_Object *obj);

/**
 * Set the alignment of the whole bounding statusbar of items.
 *
 * @param obj The statusbar object.
 * @param align The alignment of items.
 *
 * Sets how the bounding box containing all the items of the statusbar, after
 * their sizes and position has been calculated, will be aligned within
 * the space given for the whole statusbar widget.
 *
 * @ingroup Statusbar
 */
EAPI void
ewe_statusbar_items_align_set(Evas_Object *obj, Ewe_Statusbar_Items_Align align);

/**
 * Get the alignment of the whole bounding statusbar of items.
 *
 * @param obj The statusbar object.
 *
 * @return The alignment of items.
 *
 * @ingroup Statusbar
 */
EAPI Ewe_Statusbar_Items_Align
ewe_statusbar_items_align_get(const Evas_Object *obj);

/**
 * Remove item from statusbar widget.
 *
 * @param obj The statusbar object.
 * @param item a statusbar item.
 *
 * @return EINA_TRUE if item removed sucessful, or EINA_FALSE in otherwise.
 *
 * @ingroup Statusbar
 */
EAPI Eina_Bool
ewe_statusbar_item_remove(Evas_Object *obj, Ewe_Statusbar_Item *item);

#endif /* EWE_STATUSBAR_LEGACY_H */
