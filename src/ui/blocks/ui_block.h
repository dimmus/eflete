/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#ifndef UI_BLOCK_HEADER_H
#define UI_BLOCK_HEADER_H

/**
 * @defgroup Block Block
 *
 * A Block is a container for part of application UI
 * It item has a title.
 */

#include "eflete.h"

/**
 * Add a new block to the parent
 *
 * @param parent The parent object
 * @return The new object or NULL if it cannot be created
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_add(Evas_Object *parent);

/**
 * Show/Hide a title of block
 *
 * @param block the block object
 * @param vis if EINA_TRUE - title show, EINA_FALSE - title hide
 * @return EINA_TRUE if succesfull, EINA_FALSE - otherwise
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_title_visible(Evas_Object *block, Eina_Bool vis);

/**
 * Set the title of the block
 *
 * @param block the block object
 * @param title the title to set
 * @return EINA_TRUE if succesfull, EINA_FALSE - otherwise
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_title_text_set(Evas_Object *block, const char *title);

/**
 * Get the title of the block
 *
 * @param block the block object
 * @return the title
 *
 * @ingroup Block
 */
const char *
ui_block_title_text_get(Evas_Object *block);

/**
 * Get the widget list object
 *
 * @param ap the application data
 * @return the widget list object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_widget_list_get(App_Data *ap);

/**
 * Get the part signal list object
 *
 * @param ap the application data
 * @return the signal list object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_signal_list_get(App_Data *ap);

/**
 * Get the part state list object
 *
 * @param ap the application data
 * @return the state list object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_state_list_get(App_Data *ap);

/**
 * Get the property object
 *
 * @param ap the application data
 * @return the Property object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_property_get(App_Data *ap);

/**
 * Get the graph vision object
 *
 * @param ap the application data
 * @return the graph vision object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_graph_vision_get(App_Data *ap);

/**
 * Get the live view object
 *
 * @param ap the application data
 * @return the demo object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_live_view_get(App_Data *ap);

/**
 * Set the content in to the widget list block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_widget_list_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the signal list block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_signal_list_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the state list block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_state_list_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the state workspace block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_ws_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the property block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_property_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the graph vision block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_graph_vision_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the live view block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @return EINA_TRUE on success, EINA_FALSE otherwise.
 *
 * @ingroup Block
 */
Eina_Bool
ui_block_live_view_set(App_Data *ap, Evas_Object *content);

#endif /* UI_BLOCK_HEADER_H */
