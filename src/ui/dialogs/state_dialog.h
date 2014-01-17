/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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
* along with this program; If not, see http://www.gnu.org/licenses/gpl-2.0.html.
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
//#include "ui_groupspace.h"
#include "notify.h"
/**
 * Add new dialog for input params state, like name and value.
 *
 * @param ap The App_Data struct pointer.
 * @return Pointer to inwin object.
 *
 * @ingroup New_Part_Dialog
 */
Evas_Object *
state_dialog_add(App_Data *ap);

#endif /* STATE_DIALOG_H */
