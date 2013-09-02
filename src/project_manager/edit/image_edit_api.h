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

#ifndef IMAGE_EDIT_API_H
#define IMAGE_EDIT_API_H

/**
 * @defgroup ImageEditApi Image Edit API
 *
 * Funcitons for image manipulations in the project.
 */

#include "efl_tet.h"
#include "edit_common.h"

/**
 * Get the list of all the images in the given project.
 *
 * @param project Project being editing.
 * @return List of strings, each being the name for an image.
 *
 * @ingroup ImageEditApi
 */
Eina_List *
image_edit_images_list_get(Project *project);

/**
 * Get the image id by name
 *
 * @param project Project being editing.
 * @param image_name Filename of the image.
 * @return Id of the image.
 *
 * @ingroup ImageEditApi
 */
EAPI int
image_edit_image_id_get (Project *project, const char *image_name);

/**
 * Get the commpression type of the image.
 *
 * @param project Project being editing.
 * @param image_name Filename of the image.
 * @return Compression type of the image.
 *
 * @ingroup ImageEditApi
 */
EAPI Edje_Edit_Image_Comp
image_edit_image_compression_type_get (Project *project, const char *image);

/**
 * Get the image compression rate
 *
 * @param project Project being editing.
 * @param image_name Filename of the image.
 * @return Image compression rate.
 *
 * @ingroup ImageEditApi
 */
EAPI int
image_edit_image_compression_rate_get(Project *project, const char *image);

#endif /* IMAGE_EDIT_API_H */
