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
#include "edit_common.h"

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
   /** path to demo(for live view) file */
   char *demofile;
   /**  */
   Edje_CC *compiler;
   /**  */
   Edje_DeCC *decompiler;
   /** path to image directory, for compile */
   char *image_directory;
   /** path to font directory, for compile */
   char *font_directory;
   /** path to sound direcory, for compile */
   char *sound_directory;
   /** list of widgets and they styles in that theme */
   Eina_Inlist *widgets;
   /** opened group */
   Group *current_group;
};

/**
 * @typedef Project
 * @ingroup ProjectManager
 */
typedef struct _Project Project;

/**
 * Open project from edc-file.
 *
 * @param name The name of a project.
 * @param path Path to a edc-file.
 * @param image_directory Path to a image directory of a project.
 * @param font_directory Path to a font directory of a project.
 * @param sound_direcotory Path to a sound directory of a project.
 *
 * @ingroup ProjectManager
 */
Project *
pm_open_project_edc(const char *name,
                    const char *path,
                    const char *image_directory,
                    const char *font_directory,
                    const char *sound_direcotory);

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
 *
 */
/*TODO: Add save project pm_save_project_edc(Project *pm) */

/**
 *
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
