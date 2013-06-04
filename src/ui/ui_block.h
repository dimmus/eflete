#ifndef UI_BLOCK_HEADER_H
#define UI_BLOCK_HEADER_H

/**
 * @defgroup Block Block
 *
 * A Block is a container for part of application UI
 * It item has a title.
 */

#include "efl_tet.h"

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
 *
 * @ingroup Block
 */
void
ui_block_title_visible(Evas_Object *block, Eina_Bool vis);

/**
 * Set the title of the block
 *
 * @param block the block object
 * @param title the title to set
 *
 * @ingroup Block
 */
void
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
 * Get the demo object
 *
 * @param ap the application data
 * @return the demo object
 *
 * @ingroup Block
 */
Evas_Object *
ui_block_demo_view_get(App_Data *ap);

/**
 * Set the content in to the widget list block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @ingroup Block
 */
void
ui_block_widget_list_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the signal list block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @ingroup Block
 */
void
ui_block_signal_list_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the state list block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @ingroup Block
 */
void
ui_block_state_list_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the property block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @ingroup Block
 */
void
ui_block_property_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the graph vision block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @ingroup Block
 */
void
ui_block_graph_vision_set(App_Data *ap, Evas_Object *content);

/**
 * Set the content in to the demo block
 *
 * @param ap the application data
 * @param content the content object
 *
 * @ingroup Block
 */
void
ui_block_demo_view_set(App_Data *ap, Evas_Object *content);

#endif /* UI_BLOCK_HEADER_H */
