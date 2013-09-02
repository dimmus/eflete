/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
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
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

/**
 * @defgroup ImageEditor Image Editor
 *
 * Image editor is subwindow that contain information about
 * all the images used by the project.
 */

#include "efl_tet.h"
#include "Evas.h"
#include "modal_window.h"
#include "widget_manager.h"
#include "image_edit_api.h"

/**
 * Add new image editor inwin object.
 *
 * @param parent The parent object. Its must be main window object.
 * @return Pointer to inwin object, which contain image grid and buttons.
 *
 * @ingroup ImageEditor
 */
Evas_Object *
image_editor_window_add(Evas_Object *parent);

/**
 * Initialize image editor. It's method show images of the project on image
 * editor dialog window.
 *
 * @param img_view Pointer to inwin object, which was created with
 * image_editor_add method.
 * @param project A project that was earlier loaded.
 *
 * @ingroup ImageEditor
 */
void
image_editor_init(Evas_Object *img_edit, Project *project);

#endif /*IMAGE_EDITOR_H*/
