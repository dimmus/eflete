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

#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

/**
 * @defgroup ProjectManager Project Manager
 *
 * It is a basic object. Project manager object consist a information
 * about opened project. Project manager object consist the list of widgets
 * styles.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include "edje_compile.h"
#include "widget_manager.h"
#include <Eio.h>
#include "logger.h"

/**
 * @struct _Project
 * Main struct of $project. It struct consist a data of a opened project.
 *
 * @ingroup ProjectManager
 */
struct _Project
{
   /** name of project */
   char *name;
   /** path to edc file(open/save) */
   char *edc;
   /** path to edj file(open/save) */
   char *edj;
   /** path to swap(work) file */
   char *swapfile;
   /** path to image directory, for compile */
   char *image_directory;
   /** path to font directory, for compile */
   char *font_directory;
   /** path to sound direcory, for compile */
   char *sound_directory;
   /** list of widgets and they styles in that theme */
   Eina_Inlist *widgets;
   /** opened group */
   Style *current_style;
};

/**
 * @typedef Project
 * @ingroup ProjectManager
 */
typedef struct _Project Project;

/**
 * Open project from edj-file.
 *
 * @param name The name of a project.
 * @param path Path to edj-file.
 * @return The Project object.
 *
 * @ingroup ProjectManager
 */
Project *
pm_open_project_edj(const char *name,
                    const char *path);

/**
 * Save project into edc.
 *
 * This function actually decompile the resulted and changed by user EDC file.
 *
 * @param project A Project structure.
 * @param edc_dir Output directory.
 * @param log_cb This callback will be called after every line returned from edje_decc.
 * @return EINA_TRUE if saved successfully.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_export_to_edc(Project *project,
                 Eina_Stringshare *edc_dir,
                 Edje_Compile_Log_Cb log_cb);

/**
 * Save opened EDJ-project.
 *
 * @param project A Project structure.
 * @return EINA_TRUE if saved successfully.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_save_project_edj(Project *project);

/**
 * Save project into specific edj-file that is in another location.
 *
 * @param project A Project structure.
 * @param path Path to edj-file.
 * @return TRUE if saved successfully.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_save_as_project_edj(Project *project, const char *path);

/**
 *
 */
Eina_Bool
pm_save_project_to_swap(Project *project);

/**
 *
 */
/* Eina_Bool pm_save_group_to_swap(Group *group); */

/**
 *
 */
/*TODO: Add new project, template project. pm_project_new(void) */

/**
 * Delete the Project object
 *
 * @param project the given Project object
 * @return EINA_TRUE - object deleted, EINA_FALSE - can not delete object
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_free(Project *project);

#endif /* PROJECT_MANAGER_H */
