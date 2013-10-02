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

#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

/**
 * @defgroup ImageEditor Image Editor
 *
 * Image editor is subwindow that contain information about
 * all the images used by the project.
 */

#include "efl_ete.h"
#include "Evas.h"
#include "modal_window.h"
#include "widget_manager.h"
#include "image_edit_api.h"

typedef enum {
   SINGLE,
   MULTIPLE
} Image_Editor_Mode;

/**
 * Add new image editor inwin object.
 *
 * @param parent The parent object. It must be main window object.
 * @param mode its enumerate: SINGLE if single-selection mode,
 *             MULTIPLE if multi-selection mode.
 * @return Pointer to inwin object, which contain image grid and buttons.
 *
 * @ingroup ImageEditor
 */
Evas_Object *
image_editor_window_add(Evas_Object *parent, Image_Editor_Mode mode);

/**
 * Initialize image editor. It's method show images of the project on image
 * editor dialog window.
 *
 * @param img_edit Pointer to inwin object, which was created with
 * image_editor_add method.
 * @param project A project that was earlier loaded.
 *
 * @ingroup ImageEditor
 */
void
image_editor_init(Evas_Object *img_edit, Project *project);

/**
 * This function will select the image by it's name. This function is very useful
 * with image property (setting another image).
 *
 * @param img_edit Pointer to inwin object, which was created with
 * image_editor_add function.
 * @param selected Name of selected image. If selected param is NULL, this
 * function do nothing.
 *
 * @ingroup ImageEditor
 */
void
image_editor_file_choose(Evas_Object *img_edit, const char *selected);

/**
 * Callback function. Registering function for sending the result back.
 *
 * @param img_edit Pointer to inwin object, which was created with
 * image_editor_add function.
 * @param func Function that will be called after clicking on "ok" button.
 * @param data Data that will be passed into given function.
 *
 * @ingroup ImageEditor
 */
void
image_editor_callback_add(Evas_Object *img_edit, Evas_Smart_Cb func, void *data);

#endif /*IMAGE_EDITOR_H*/
