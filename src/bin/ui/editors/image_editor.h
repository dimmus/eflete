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

#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

/**
 * @defgroup ImageEditor Image Editor
 *
 * Image editor is subwindow that contain information about
 * all the images used by the project.
 */

#include "eflete.h"
#include "modal_window.h"
#include "widget_macro.h"

#define SIG_IMAGE_SELECTED "image_selected"

typedef enum {
   SINGLE,
   MULTIPLE,
   TWEENS
} Image_Editor_Mode;

/**
 * Add new image editor inwin object.
 *
 * @param mode its enumerate: SINGLE if single-selection mode,
 *             MULTIPLE if multi-selection mode.
 *
 * @return Pointer to inwin object, which contain image grid and buttons.
 *
 * @ingroup ImageEditor
 */
Evas_Object *
image_editor_window_add(Image_Editor_Mode mode);

#endif /*IMAGE_EDITOR_H*/
