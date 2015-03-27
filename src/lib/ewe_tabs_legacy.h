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

#ifndef EWE_TABS_LEGACY_H
#define EWE_TABS_LEGACY_H

/**
 * This adds tabs to @p parent object.
 *
 * @param parent The parent object
 * @return The new object or NULL if it cannot be created
 *
 * @ingroup tabs
 */
EAPI Evas_Object *
ewe_tabs_add(Evas_Object *parent);

/**
 * This adds an item after specified item.
 * If no item specified adds as last.
 *
 * @param obj The tabs object
 * @param item The tabs item
 * @param title The tabs title
 * @param style The tabs style
 * @return The new tabs item object or NULL if it cannot be created
 *
 * @ingroup tabs
 */
EAPI Ewe_Tabs_Item *
ewe_tabs_item_append(Evas_Object          *obj,
                     Ewe_Tabs_Item        *item,
                     Eina_Stringshare     *title,
                     Eina_Stringshare     *style);

/**
 * This adds an item before specified item.
 * If no item specified adds as first.
 *
 * @param obj The tabs object
 * @param item The tabs item
 * @param title The tabs title
 * @param style The tabs style
 * @return The new tabs item object or NULL if it cannot be created
 *
 * @ingroup tabs
 */
EAPI Ewe_Tabs_Item *
ewe_tabs_item_prepend(Evas_Object          *obj,
                      Ewe_Tabs_Item        *item,
                      Eina_Stringshare     *title,
                      Eina_Stringshare     *style);

/**
 * Delete tabs item.
 *
 * @param item The tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_del(Evas_Object       *obj,
                  Ewe_Tabs_Item     *item);

/**
 * Get internal items list.
 *
 * @param obj The tabs object
 * @return list of items on success, NULL otherwise
 *
 * @ingroup tabs
 */
EAPI const Eina_List *
ewe_tabs_items_list_get(const Evas_Object *obj);

/**
 * This setts content to tabs item.
 *
 * @param item The tabs item
 * @param content The object to be setted as content to the tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_content_set(Evas_Object   *obj,
                          Ewe_Tabs_Item *item,
                          Evas_Object   *content);

/**
 * This unsetts content from tabs item.
 *
 * @param item The tabs item
 * @return Evas_Object *content
 *
 * @ingroup tabs
 */
EAPI Evas_Object *
ewe_tabs_item_content_unset(Evas_Object       *obj,
                            Ewe_Tabs_Item     *item);

/**
 * This gets content from tabs item.
 *
 * @param item The tabs item
 * @return Evas_Object *content
 *
 * @ingroup tabs
 */
EAPI const Evas_Object *
ewe_tabs_item_content_get(Evas_Object       *obj,
                          Ewe_Tabs_Item     *item);


/**
 * This setts icon to tabs item.
 *
 * @param item The tabs item
 * @param icon The object to be setted as icon to the tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_icon_set(Evas_Object   *obj,
                       Ewe_Tabs_Item *item,
                       Evas_Object   *icon);

/**
 * This unsetts icon from tabs item.
 *
 * @param item The tabs item
 * @return Evas_Object *icon
 *
 * @ingroup tabs
 */
EAPI Evas_Object *
ewe_tabs_item_icon_unset(Evas_Object   *obj,
                         Ewe_Tabs_Item *item);

/**
 * This setts button to tabs item.
 *
 * @param item The tabs item
 * @param button The object to be setted as button to the tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_button_set(Evas_Object   *obj,
                         Ewe_Tabs_Item *item,
                         Evas_Object   *button);

/**
 * This unsetts button from tabs item.
 *
 * @param item The tabs item
 * @return Evas_Object *button
 *
 * @ingroup tabs
 */
EAPI Evas_Object *
ewe_tabs_item_button_unset(Evas_Object       *obj,
                           Ewe_Tabs_Item     *item);

/**
 * This activates tabs item.
 *
 * @param item The tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_active_item_set(Evas_Object       *obj,
                         Ewe_Tabs_Item     *item);

/**
 * Get active item item.
 *
 * @return Ewe_Tabs_Item* on success, NULL otherwise
 *
 * @ingroup tabs
 */
EAPI Ewe_Tabs_Item *
ewe_tabs_active_item_get(const Evas_Object *obj);

/**
 * This setts title of the tabs item.
 *
 * @param item The tabs item
 * @param title Text to be setted as title of the tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_title_set(Evas_Object       *obj,
                        Ewe_Tabs_Item     *item,
                        Eina_Stringshare  *title);

/**
 * This getts title of the tabs item.
 *
 * @param item The tabs item
 * @return items title on success, NULL otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Stringshare *
ewe_tabs_item_title_get(const Evas_Object   *obj,
                        Ewe_Tabs_Item       *item);

/**
 * This enables or disables tabs item.
 *
 * Notes: user cannot switch to disabled tabs item.
 * If it is active allready or is activated from code
 * items content will be also rendered disabled.
 *
 * @param item The tabs item
 * @param disabled EINA_TRUE to disable, EINA_FALSE to enable tabs item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_disabled_set(Evas_Object       *obj,
                           Ewe_Tabs_Item     *item,
                           Eina_Bool         disabled);

/**
 * Gets items disabled state.
 *
 * @param item The tabs item
 * @return EINA_TRUE if item is disabled, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_item_disabled_get(const Evas_Object *obj,
                           Ewe_Tabs_Item     *item);

/**
 * Set horizontal or vertical orient for tabs.
 *
 * @param horizontal EINA_TRUE to set horizontal orientation, EINA_FALSE to set vertical orientation for tabs
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_orient_horizontal_set(Evas_Object   *obj,
                               Eina_Bool   horizontal);

/**
 * Gets tabs orientation.
 *
 * @return EINA_TRUE if tabs is horizontal, EINA_FALSE if tabs is vertical
 *
 * @ingroup tabs
 */
EAPI Eina_Bool
ewe_tabs_orient_horizontal_get(const Evas_Object   *obj);

#endif /* EWE_TABS_LEGACY_H */
