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
* along with this program; If not, see .
*/

#ifndef ADD_STYLE_DIALOG_H
#define ADD_STYLE_DIALOG_H

/**
 * @defgroup New_Style_Dialog New_Style_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for create new style and/or class.
 */

#include "efl_ete.h"
#include "Evas.h"
#include "widget_manager.h"
#include "ui_groupspace.h"
#include "modal_window.h"
#include "widget_define.h"
#include "ui_block.h"
#include "ui_states_list.h"
#include "ui_groupspace.h"
#include "notify.h"
#include "ui_widget_list.h"

/**
 * Add new dialog for input params style and/or class, like names of style and class.
 *
 * @param ap The App_Data struct pointer.
 *
 * @ingroup New_Style_Dialog
 */
void
new_style_dialog_add(App_Data *ap);

#endif /* ADD_STYLE_DIALOG_H */
