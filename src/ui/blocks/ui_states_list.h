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

#include "ui_block.h"
#include "ui_property.h"

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
 * @param group The Style data.
 * @param part The Part object.
 * @return EINA_TRUE - successful, EINA_FALSE - something wrong.
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_data_set(Evas_Object *object, Style *style, Part *part);

/**
 * Get Part object, wich states currently showed.
 *
 * @param object A 'states list' object
 * @return A Part object
 *
 * @ingroup StateList
 */
Part *
ui_states_list_part_get(Evas_Object *obj);

/**
 * Add new state to current selected part in state list.
 *
 * @param object A 'states list' object
 * @param state new state name that is going to be added.
 * @return EINA_TRUE - successful, EINA_FALSE - something wrong.
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_state_add(Evas_Object *obj, const char *state);

/**
 * Delete state from given states list.
 *
 * @param object A 'states list' object
 * @return EINA_TRUE - successful, EINA_FALSE - something wrong.
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_selected_state_del(Evas_Object *obj);

/**
 * Return currently selected state in 'states list' object.
 *
 * @param object A 'states list' object
 * @return selected state's name.
 *
 * @ingroup StateList
 */
Eina_Stringshare *
ui_states_list_selected_state_get(Evas_Object *obj);

/**
 * Unset data of a part states from 'states list'.
 *
 * @param object A 'states list' object
 * @return EINA_TRUE - successful, EINA_FALSE - something wrong.
 *
 * @ingroup StateList
 */
Eina_Bool
ui_states_list_data_unset(Evas_Object *obj);
#endif /* UI_STATES_LIST_H */
