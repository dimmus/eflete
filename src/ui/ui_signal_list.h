#ifndef UI_SIGNAL_LIST_H
#define UI_SIGNAL_LIST_H

/**
 * @defgroup SignalList SignalList
 *
 * A Signal List used for view list of part signal
 */

#include "efl_tet.h"
#include <Elementary.h>

/**
 * Add a new Signal List
 *
 * @param ap the application data
 * @param part the part whose signal must be show
 * @return the signal list object
 *
 * @ingroup SignalList
 */
Evas_Object *
ui_signal_list_add(App_Data *ap, Eina_List *signals);

#endif /* UI_SIGNAL_LIST_H */
