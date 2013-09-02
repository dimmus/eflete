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

#ifndef COLORCLASSES_EDIT_API_H
#define COLORCLASSES_EDIT_API_H

/**
 * @defgroup ColorclassesEditApi Colorclasses Edit API
 *
 * Funcitons for colorclasses manipulations in the project.
 */

#include "efl_tet.h"
#include "edit_common.h"

/**
 * Get the list of all the images in the given project.
 *
 * @param project Project being editing.
 * @return List of strings, each being the name for an image.
 *
 * @ingroup ColorClassEditApi
 */
Eina_List *
colorclasses_edit_list_get(Project *project);

/** Get all the colors that compose the class.
 *
 * You can pass NULL to colors you are not intrested in.
 *
 * @param project Project being editing.
 * @param class_name Color class to fetch values.
 * @param r Red component of main color.
 * @param g Green component of main color.
 * @param b Blue component of main color.
 * @param a Alpha component of main color.
 * @param r2 Red component of secondary color.
 * @param g2 Green component of secondary color.
 * @param b2 Blue component of secondary color.
 * @param a2 Alpha component of secondary color.
 * @param r3 Red component of tertiary color.
 * @param g3 Green component of tertiary color.
 * @param b3 Blue component of tertiary color.
 * @param a3 Alpha component of tertiary color.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup ColorClassEditApi
 */
EAPI Eina_Bool
colorclasses_edit_colors_get (Project *project,
   const char *class_name, int *r, int *g, int *b, int *a,
   int *r2, int *g2, int *b2, int *a2, int *r3, int *g3, int *b3, int *a3);

/** Set the colors for the given color class.
 *
 * If you set a color to -1 it will not be touched.
 *
 * @param project Project being editing.
 * @param class_name Color class to fetch values.
 * @param r Red component of main color.
 * @param g Green component of main color.
 * @param b Blue component of main color.
 * @param a Alpha component of main color.
 * @param r2 Red component of secondary color.
 * @param g2 Green component of secondary color.
 * @param b2 Blue component of secondary color.
 * @param a2 Alpha component of secondary color.
 * @param r3 Red component of tertiary color.
 * @param g3 Green component of tertiary color.
 * @param b3 Blue component of tertiary color.
 * @param a3 Alpha component of tertiary color.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup ColorClassEditApi
 */
EAPI Eina_Bool
colorclasses_edit_colors_set(Project *project,
const char *class_name, int	r, int g, int b, int a, int r2, int g2, int b2,
int a2, int r3, int g3, int b3, int a3);

/*
 *Create a new color class object in the given edje.
 *
 *If another class with the same name exists nothing is created and EINA_FALSE is returned.
 *
 * @param project Project being edited.
 * @name Name for the new color class.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup ColorClassEditApi
 */
EAPI Eina_Bool
colorclass_edit_add (Project *project, const char *name);

/*
 *Create a new color class object in the given edje.
 *
 * @param project Project being edited.
 * @name Color class to delete.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup ColorClassEditApi
 */
EAPI Eina_Bool
colorclass_edit_del (Project *project, const char *name);
#endif /* COLORCLASSES_EDIT_API_H */
