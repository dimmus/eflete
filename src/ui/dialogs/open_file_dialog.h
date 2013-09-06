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

#ifndef OPEN_FILE_DIALOG_H
#define OPEN_FILE_DIALOG_H

#include "efl_tet.h"
#include "alloc.h"
#include "ui_block.h"
#include "ui_widget_list.h"

/**
 * @defgroup OpenFileDialog Open file dialog
 * The widget provide a open project functionality.
 */

/**
 * Create and show a dialog for open project from a edj file.
 *
 * @param ap The Application data.
 * @return EINA_TRUE if a project opened, else EIN_FALSE.
 *
 * @ingroup OpenFileDialog
 */
Eina_Bool
open_edj_file(App_Data *ap);

/**
 * Create and show a dialog for open project from a edc file.
 *
 * @param ap The Apllication data.
 * @return EINA_TRUE if a project opened, else EIN_FALSE.
 *
 * @ingroup OpenFileDialog
 */
Eina_Bool
open_edc_file(App_Data *ap);

/**
 * Add callbacks to widget list. Callbacks are using next signals:
 * "wl,group,select"
 * "wl,part,select",
 * "wl,part,back",
 * "wl,group,back",
 *
 * @param wd_list A pointer to widget list object.
 * @param ap The Apllication data.
 *
 * @ingroup OpenFileDialog
 */
void
add_callbacks_wd(Evas_Object *wd_list, App_Data *ap);
#endif /* OPEN_FILE_DIALOG_H */
