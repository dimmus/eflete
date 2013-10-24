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

#ifndef ADD_PART_DIALOG_H
#define ADD_PART_DIALOG_H

/**
 * @defgroup New_Part_Dialog New_Part_Dialog
 * @ingroup Dialogs
 *
 * Show dialog window for choise part type.
 */

#include "efl_ete.h"
#include "Evas.h"
#include "widget_manager.h"
#include "ui_groupspace.h"
#include "modal_window.h"
#include "widget_define.h"
#include "image_editor.h"

/**
 * Add new dialog for choise part type add inwin object.
 *
 * @param parent The parent object. Its must be layout of main window
 * object.
 * @param groupspace The groupspace layout pointer.
 * @return Pointer to inwin object.
 *
 * @ingroup New_Part_Dialog
 */
Evas_Object *
new_part_dialog_add(Evas_Object *parent, Evas_Object *groupspace);

#endif /* ADD_PART_DIALOG_H */
