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

#ifndef STATE_DIALOG_H
#define STATE_DIALOG_H

/**
 * @defgroup New_State_Dialog New_State_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for choise part type.
 */

#include "eflete.h"
#include "widget_manager.h"
#include "widget_macro.h"
#include "ui_states_list.h"
#include "notify.h"

/**
 * Add new dialog for input params state, like name and value.
 *
 * @param ap The App_Data struct pointer.
 * @return Pointer to popup object.
 *
 * @ingroup New_Part_Dialog
 */
Evas_Object *
state_dialog_state_add(App_Data *ap);

/**
 * Add new dialog with question, what user want do: delete or not delete a
 * state of the part.
 *
 * @param ap The App_Data struct pointer.
 * @return Pointer to popup object.
 *
 * @ingroup New_Part_Dialog
 */
Evas_Object *
state_dialog_state_del(App_Data *ap);

#endif /* STATE_DIALOG_H */
