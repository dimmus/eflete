/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
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

#ifndef UI_STATES_LIST_H
#define UI_STATES_LIST_H

/**
 * @defgroup StateList StateList
 * @ingroup Block
 *
 * A State List used for view list of part states
 */

#include "main_window.h"

/**
 * The state data.
 *
 * @ingroup StateList
 */
struct _State_Data
{
   Part *part; /** The given part */
   Eina_Stringshare *state; /** The state name */
   Eina_Stringshare *duplicate_state; /** The state should be duplicated */
   double value; /** The state value */
};

typedef struct _State_Data State_Data;

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
 * @param group The Style data.
 * @param part The Part object.
 * @return EINA_TRUE - successful, EINA_FALSE - something wrong.
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_data_set(Style *style, Part *part);

/**
 * Unset data of a part states from 'states list'.
 *
 * @return EINA_TRUE - successful, EINA_FALSE - something wrong.
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_data_unset(void);

/**
 * Get Part object, wich states currently showed.
 *
 * @return A Part object
 *
 * @ingroup StateList
 */
Part *
ui_states_list_part_get(void);

#endif /* UI_STATES_LIST_H */
