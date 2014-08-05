/**
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

#ifndef OPEN_FILE_DIALOG_H
#define OPEN_FILE_DIALOG_H

#include "eflete.h"
#include "main_window.h"

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
#endif /* OPEN_FILE_DIALOG_H */
