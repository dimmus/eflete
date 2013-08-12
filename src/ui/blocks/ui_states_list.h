#ifndef UI_STATES_LIST_H
#define UI_STATES_LIST_H

/**
 * @defgroup StateList StateList
 * @ingroup Block
 *
 * A State List used for view list of part states
 */

#include "efl_tet.h"
#include "ui_block.h"
#include "ui_property.h"
#include <Elementary.h>

/**
 * Add a new State List
 *
 * @param parent The parent object
 * @return A new 'state list' widget handler or NULL, on errors
 *
 * @ingroup StateList
 */
Evas_Object *
ui_states_list_add(Evas_Object *parent);

/**
 * Set to a 'states list' widget a data of a part states.
 *
 * @param object The 'states list' object.
 * @param group The Group object.
 * @param part The Part object.
 * @return EINA_TRUE - it all ok, EINA_FALSE - samsing wrong
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_data_set(Evas_Object *object, Group *group, Part *part);

/**
 * Get Part object, wich states currently showed.
 *
 * @param object A 'states list' object
 * @return A Part object
 *
 * @ingroup StateList
 */
Part *
ui_state_list_part_get(Evas_Object *obj);

#endif /* UI_STATES_LIST_H */
