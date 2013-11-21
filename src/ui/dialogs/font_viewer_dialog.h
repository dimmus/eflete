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

#ifndef FONT_VIEWER_DIALOG_H
#define FONT_VIEWER_DIALOG_H

/**
 * @defgroup Font_Viewer Font_Viewer
 * @ingroup Dialogs
 *
 * Font viewer its subwindow displays all the fonts used by the project.
 */

#include "efl_ete.h"
#include "Evas.h"
#include "widget_manager.h"
#include "widget_macro.h"
/**
 * Add new font viewer inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object.
 *
 * @ingroup Font_Viewer
 */
Evas_Object *
font_viewer_add(Evas_Object *parent);

/**
 * Initialize font viewer. It's method show fonts of the project in font
 * viewer dialog window.
 *
 * @param font_view Pointer to inwin object, which was created with
 * font_viewer_add method.
 * @param project A project that was early loaded.
 *
 * @ingroup Font_Viewer
 */
void
font_viewer_init(Evas_Object *font_view, Project *project);

#endif /*FONT_VIEWER_DIALOG_H*/
