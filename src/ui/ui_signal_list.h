#ifndef UI_SIGNAL_LIST_H
#define UI_SIGNAL_LIST_H

/**
 * @defgroup SignalList SignalList
 *
 * This widget inherit form the Genlist
 *
 * A Signal list widget emits the following signals:
 * @li "sl,signal,select": the user selected on the signal name
 *
 * A Signal List used for view list of part signal.
 */

#include "efl_tet.h"
#include <Elementary.h>

/**
 * Add a new Signal List.
 *
 * @param parent The parent object
 * @param part The part whose signal must be show
 * @return The signal list object
 *
 * @ingroup SignalList
 */
Evas_Object *
ui_signal_list_add(Evas_Object *parent);

/**
 * Set to a signal list widget a data of signal with using in a part.
 *
 * @param object A signal list object
 * @param signals A list of using signals
 * @return EINA_TRUE - it all ok, EINA_FALSE - samsing wrong
 *
 * @ingroup SignalList
 */
Eina_Bool
ui_signal_list_data_set(Evas_Object *object, Eina_List *signals);

/**
 * Add (register) a callback function to a given Signal List object event.
 *
 * @param object A Signal List object
 * @param event The event's name string
 * @param func The callback function
 * @param data User data to be passed to the callback function
 *
 * @ingroup WidgetList
 */
void
ui_signal_list_callback_add(Evas_Object *object,
                            const char *event,
                            Edje_Signal_Cb func,
                            void *data);

#endif /* UI_SIGNAL_LIST_H */
