/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

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

#include "eflete.h"

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
 * @param object The 'signal list' object;
 * @param style The structure @Style object.
 * @return EINA_TRUE - it all ok, EINA_FALSE - samsing wrong
 *
 * @ingroup SignalList
 */
Eina_Bool
ui_signal_list_data_set(Evas_Object *object, Style *style);

/**
 * Unset the 'signal list' data, and clear the list.
 *
 * @param object The 'signal list' object.
 *
 * @return The Style object with been setted as data or NULL if any Style not
 *         been setted.
 *
 * @ingroup SignalList
 */
Style *
ui_signal_list_data_unset(Evas_Object *object);

#endif /* UI_SIGNAL_LIST_H */
