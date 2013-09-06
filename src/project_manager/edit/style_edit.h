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

#ifndef STYLE_EDIT_H
#define STYLE_EDIT_H

#include "efl_tet.h"
#include "edit_common.h"

/**
 * Get the list of all the text styles in the given project.
 *
 * @param project Project being editing.
 * @return List of strings, each being the name for a text style.
 *
 * @ingroup TextStyle
 */
Eina_List *
style_edit_styles_list_get(Project *project);

/**
 * Create a new text style object in the given project.
 * If another style with the same name exists nothing is created and EINA_FALSE
 * is returned.
 *
 * @param project Project being editing.
 * @param style Name for the new style.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup TextStyle
 */
Eina_Bool
style_edit_style_add(Project *project, const char *style);

/**
 * Delete the given text style and all the child tags.
 *
 * @param project Project being editing.
 * @param style Style to delete.
 *
 * @ingroup TextStyle
 */
void
style_edit_style_del(Project *project, const char *style);

/**
 * Get the list of all the tags name in the given text style.
 *
 * @param project Project being editing.
 * @param style Style to get the tags for.
 * @return List of strings, each being one tag in the given style.
 *
 * @ingroup TextStyle
 */
Eina_List *
style_edit_style_tags_list_get(Project *project, const char *style);

/**
 * Add a new tag to the given text style.
 * If another tag with the same name exists nothing is created and EINA_FALSE
 * is returned.
 *
 * @param project Project being editing.
 * @param style Style where to add the new tag.
 * @param tag_name Name for the new tag.
 * @param tag_value Value for the tag.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup TextStyle
 */
Eina_Bool
style_edit_style_tag_add(Project *project,
                         const char *style,
                         const char *tag_name,
                         const char *tag_value);

/**
 * Delete the given tag.
 *
 * @param project Project being editing.
 * @param style Style from where to remove the tag.
 * @param tag_name Tag to delete.
 *
 * @ingroup TextStyle
 */
void
style_edit_style_tag_del(Project *project,
                         const char *style,
                         const char *tag_name);

/**
 * Get the value of the given tag.
 *
 * @param project Project being editing.
 * @param style Style containing the tag being.
 * @param tag_name Tag to get the value for.
 * @return Value of the given tag.
 *
 * @ingroup TextStyle
 */
const char*
style_edit_style_tag_value_get(Project *project,
                               const char *style,
                               const char *tag_name);

/**
 * Set the value of the given tag.
 *
 * @param project Project being editing.
 * @param style Style containing the tag to change.
 * @param tag_name Name of the tag to set the value for.
 * @param new_value Value for the tag.
 *
 * @ingroup TextStyle
 */
void
style_edit_style_tag_value_set(Project *project,
                               const char* style,
                               const char* tag_name,
                               const char* new_value);

/**
 * Save the modified styles back to his file.
 *
 * @param project Project being saving.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup TextStyle
 */
Eina_Bool
style_edit_save(Project *project);

#endif /* STYLE_EDIT_H */
