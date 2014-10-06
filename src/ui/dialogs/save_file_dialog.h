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

#ifndef SAVE_FILE_DIALOG_H
#define SAVE_FILE_DIALOG_H


#include "eflete.h"
#include "alloc.h"
#include "modal_window.h"
#include "widget_macro.h"
#include "main_window.h"
/**
 * @defgroup SaveFileDialog Save file dialog
 * The widget provide a save project functionality.
 */

/**
 * Create and show a dialog for save project into a edj file.
 *
 * This function will ask user about the path where he would like
 * to save his file. If the file has wrong extension then it will be error noti
 * will be appeared (@see Notify). If the file is Exist then function will also
 * ask the user if he want to replace the existed file.
 *
 * @param ap The Application data.
 * @return EINA_TRUE if a project saved, else EINA_FALSE.
 *
 * @ingroup SaveFileDialog
 */
Eina_Bool
save_as_edj_file(App_Data *ap);

/**
 * Save current project. If project is new save dialog will be shown(@see save_as_edj_file).
 *
 * @param ap The Application data.
 * @return EINA_TRUE if a project saved, else EINA_FALSE.
 *
 * @ingroup SaveFileDialog
 */
Eina_Bool
save_edj_file(App_Data *ap);

/**
 * Create and show a dialog for save project into a edc file.
 *
 * @param ap The Apllication data.
 * @return EINA_TRUE if a project saved, else EINA_FALSE.
 *
 * @ingroup SaveFileDialog
 */
Eina_Bool
save_as_edc_file(App_Data *ap);

#endif /* SAVE_FILE_DIALOG_H */
