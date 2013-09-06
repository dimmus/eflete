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

#ifndef COLORCLASS_VIEWER_DIALOG_H
#define COLORCLASS_VIEWER_DIALOG_H

/**
 * @defgroup Colorclass_Viewer Colorclass_Viewer
 * @ingroup Dialogs
 *
 * Show color classes, which uses in project.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "widget_manager.h"
#include "modal_window.h"
#include "colorclasses_edit_api.h"

/**
 * Add new colorclass viewer inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object.
 *
 * @ingroup Colorclass_Viewer
 */
Evas_Object *
colorclass_viewer_add(Evas_Object *parent);

/**
 * Initialize colorclass viewer. It's method show color classes of project.
 *
 * @param ccl_view Pointer to inwin object, which was created with
 * colorclass_viewer_add method.
 * @param project A project that was early loaded.
 *
 * @ingroup Colorclass_Viewer
 */
void
colorclass_viewer_init(Evas_Object *ccl_view, Project *project);

#endif /* COLORCLASS_VIEWER_DIALOG_H */
