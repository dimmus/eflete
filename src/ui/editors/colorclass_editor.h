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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#ifndef COLORCLASS_VIEWER_DIALOG_H
#define COLORCLASS_VIEWER_DIALOG_H

/**
 * @defgroup Colorclass_Viewer Colorclass_Viewer
 * @ingroup Dialogs
 *
 * Show color classes, which uses in project.
 */

#include "eflete.h"
#include "widget_manager.h"
#include "modal_window.h"
#include "widget_macro.h"
#include "string_macro.h"

/**
 * Add new colorclass viewer inwin object.
 *
 * @param project A project that was early loaded.
 *
 * @return Pointer to inwin object.
 *
 * @ingroup Colorclass_Viewer
 */
Evas_Object *
colorclass_viewer_add(Project *project);

#endif /* COLORCLASS_VIEWER_DIALOG_H */
