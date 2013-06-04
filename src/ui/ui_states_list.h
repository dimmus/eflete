#ifndef UI_STATES_LIST_H
#define UI_STATES_LIST_H

/**
 * @defgroup StateList StateList
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
 * @param ap the application data
 * @param part the part whose state must be show
 * @return the state list object
 *
 * @ingroup StateList
 */
Evas_Object *
ui_states_list_add(App_Data *ap, Part *part);

#endif /* UI_STATES_LIST_H */
