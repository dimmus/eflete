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

/**
 * @defgroup Preference Preference
 * @ingroup EFLETE
 *
 * View of Eflete config.
 */

#include "eflete.h"

/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @param project -currently opened project
 *
 * @return Pointer to the inwin object that contain preferences
 *
 * @ingroup Preference
 */
Evas_Object *
preferences_window_add(Project *project);

/**
 * Function for toggling autosaving of edj_file by period of ecore_timer
 * accordingly to the profile settings.
 *
 * @param project -currently opened project
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Preference
 */
Eina_Bool
preferences_project_autosave_update(Project *project);
