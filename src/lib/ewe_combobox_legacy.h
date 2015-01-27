
#ifndef EWE_COMBOBOX_LEGACY_H
#define EWE_COMBOBOX_LEGACY_H

/**
 * Add a new combobox to the parent
 *
 * @param parent The parent object
 * @return The new object or @c NULL if it cannot be created
 *
 * @ingroup combobox
 */
EAPI Evas_Object                 *ewe_combobox_add(Evas_Object *parent);

/**
 * @brief Returns whether the combobox is expanded.
 *
 * @param obj The combobox object
 * @return  This will return EINA_TRUE if the combobox is expanded or
 * EINA_FALSE if it is not expanded.
 *
 * @ingroup Combobox
 */
EAPI Eina_Bool                    ewe_combobox_expanded_get(Evas_Object *obj);

/**
* This adds an item to combobox object.
*
* @param obj The combobox object
* @param title The combobox title
* @return The new combobox item object or NULL if it cannot be created
*
* @ingroup combobox
*/
EAPI Ewe_Combobox_Item *
ewe_combobox_item_add(Evas_Object          *obj,
                      Eina_Stringshare     *title);

/**
 * Delete combobox item.
 *
 * @param item The combobox item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_item_del(Evas_Object         *obj,
                      Ewe_Combobox_Item   *item);

/**
 * Combobox items list free.
 *
 * @param item The combobox item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_items_list_free(Evas_Object        *obj,
                             Eina_Bool          empty);

/**
 * Get internal items list.
 *
 * @param obj The combobox object
 * @return list of items on success, NULL otherwise
 *
 * @ingroup combobox
 */
EAPI const Eina_List *
ewe_combobox_items_list_get(const Evas_Object *obj);

/**
 * This setts title of the combobox item.
 *
 * @param index The combobox item index
 * @param title Text to be setted as title of the combobox item
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_item_title_set(Evas_Object *obj,
                            int index,
                            Eina_Stringshare  *title);

/**
 * This sets title for the combobox.
 *
 * @param title Text to be setted as title of the combobox
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_text_set(Evas_Object *obj,
                      Eina_Stringshare  *title);
/**
 * This gets title of the combobox item.
 *
 * @param item The combobox item
 * @return items title on success, NULL otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Stringshare *
ewe_combobox_item_title_get(Evas_Object   *obj,
                            const int     index);

/**
 * This gets a select item from combobox object.
 *
 * @param obj The combobox object
 * @return The select combobox item object or NULL if it cannot be created
 *
 * @ingroup combobox
 */
EAPI Ewe_Combobox_Item *
ewe_combobox_select_item_get(Evas_Object *obj);

/**
 * Sets select combobox item.
 *
 * @param obj The combobox object
 * @param index The combobox index
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_select_item_set(Evas_Object *obj,
                             const int index);

/**
 * Sets the style for combobox item.
 *
 * @param obj The combobox obj
 * @param item The combobox item
 * @param style The name of style
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_item_style_set(Evas_Object        *obj,
                            Ewe_Combobox_Item  *item,
                            Eina_Stringshare   *style);

/**
 * Sets the style for combobox.
 *
 * @param obj The combobox obj
 * @param style The name of style
 * @return EINA_TRUE on success, EINA_FALSE otherwise
 *
 * @ingroup combobox
 */
EAPI Eina_Bool
ewe_combobox_style_set(Evas_Object        *obj,
                       Eina_Stringshare   *style);

#endif /* EWE_COMBOBOX_LEGACY_H */
