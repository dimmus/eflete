#ifndef UI_SIGNAL_LIST_H
#define UI_SIGNAL_LIST_H

/**
 * @defgroup SignalList SignalList
 * @ingroup Block
 *
 * This widget inherit form the Genlist
 *
 * A 'signal list' widget emits the following signals:
 * @li "sl,signal,select": the user selected on the signal name
 *
 * A 'signal list' used for view list of part signal.
 */

#include "efl_tet.h"
#include <Elementary.h>

/**
 * Add a new Signal List.
 *
 * @param parent The parent object
 * @return A new 'signal list' widget handler or NULL, on errors
 *
 * @ingroup SignalList
 */
Evas_Object *
ui_signal_list_add(Evas_Object *parent);

/**
 * Set to a 'signal list' widget a data of signal with using in a part.
 *
 * @param object A 'signal list' object
 * @param signals A list of using signals
 * @return EINA_TRUE - it all ok, EINA_FALSE - samsing wrong
 *
 * @ingroup SignalList
 */
Eina_Bool
ui_signal_list_data_set(Evas_Object *object, Eina_List *signals);

#endif /* UI_SIGNAL_LIST_H */
