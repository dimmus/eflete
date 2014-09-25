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

#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

/**
 * @defgroup ProjectManager Project Manager
 * @ingroup Eflete
 *
 * Project manager is module management the Projects of Eflete. Main task of
 * this module is a maintained work with Edje files, as binary, it's mean all
 * edj files, and with raw, edc files, make from edc a binary files.
 *
 * Project manager managed a Eflete project, it's a folder with specific files.
 * Of these files is .pro file, main Eflete project file. This file consist all
 * information about project in the current folder.
 */

#include "edje_compile.h"
#include "widget_manager.h"
#include "logger.h"

/**
 * @struct _Project
 *
 * Main struct of 'Project' in the Eflete. It struct consist a data of a opened
 * project.
 * Eflete project term - it's a eet file with extension '.pro', which consist
 * all techical information about project.
 *
 * @ingroup ProjectManager
 */
struct _Project
{
   /** this is worrking file, all changes are happened in this file. */
   Eina_Stringshare *dev;

   /** path where will be saved the develop edj file */
   Eina_Stringshare *develop_path;
   /** path where will be compiled the release edj file */
   Eina_Stringshare *release_path;
   /** compile options for release edj file. see edje_cc reference */
   Eina_Stringshare *release_options;


   Eina_Inlist *widgets;
   Eina_Inlist *layouts;

   Eina_Bool is_saved;

   /** current editing group */
   Style *current_style;
};

/**
 * @enum _Build
 *
 * The build options.
 *
 * @ingroup ProjectManager
 */
enum _Build
{
   /** all unsed data will be saved in the resulting file. */
   BUILD_DEVELOP,
   /** resulting file is optimizated, all unused data excluted from file */
   BUILD_RELEASE,
   BUILD_LAST
};

/**
 * @enum PM_Project_Status
 * @ingroup ProjectManager
 */
enum _PM_Project_Status
{
   PM_PROJECT_ERROR,
   PM_PROJECT_SUCCESS,
   PM_PROJECT_LAST
};

/**
 * @typedef Project
 * @ingroup ProjectManager
 */
typedef struct _Project Project;

/**
 * @typedef Build
 * @ingroup ProjectManager
 */
typedef enum _Build Build;

/**
 * @typedef PM_Project_Status
 * @ingroup ProjectManager
 */
typedef enum _PM_Project_Status PM_Project_Status;

/**
 * @typedef PM_Project_Thread
 * @ingroup ProjectManager
 */
typedef Ecore_Thread PM_Project_Thread;

/**
 * @typedef PM_Project_Progress_Cb
 * @ingroup ProjectManager
 */
typedef Eina_Bool
(* PM_Project_Progress_Cb)(void *data,
                           Eina_Stringshare *progress_string,
                           Eina_Stringshare *progress_substring);

/**
 * @typedef PM_Project_Cb
 * @ingroup ProjectManager
 */
typedef Eina_Bool
(* PM_Project_Cb)(void *data,
                  PM_Project_Status status,
                  Project *project);

/**
 * Stop project process.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_job_stop(PM_Project_Thread *pm_process);

/**
 * Create a new project which based on the imported edj file.
 *
 * @param name The name of new project,
 * @param path The path of new project, by this path will be created a project
 *             folder,
 * @param edj The path to the edj file wich will imported.
 *
 * @return The new #Project object, othewise NULL.
 *
 * @note You must sure that by path no folder with 'name'. If folder 'name' is
 *       there project files will be rewriten.
 *
 * @ingroup ProjectManager
 */
PM_Project_Thread *
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_Cb func_end,
                      void *data);

/**
 * Create a new project which base on the imported edc file.
 * This function try to compile a current edc file, if copilation success,
 * object Project will be returned.
 *
 * @param name The name of new project,
 * @param path The path of new project, by this path will be created a project
 *             folder,
 * @param edc The path to the edc file wich will imported,
 * @param image_directory Directory to look in for relative path images,
 * @param sound_directory Directory to look in for relative path sounds samples,
 * @param font_directory Directory to look in for relative path fonts,
 * @param data_directory Directory to look in for relative path data.file
 *                       entries,
 * @param vibration_directory Directory to look in for relative path vibration
 *                            samples.
 *
 * @return The new #Project object, otherwise NULL.
 *
 * @note Function will not check a edc file, and directories.
 *
 * @ingroup ProjectManager
 */
Project *
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *image_directory,
                      const char *sound_directory,
                      const char *font_directory,
                      const char *data_directory,
                      const char *vibration_directory);

/**
 * Open Eflete project.
 *
 * @param path The path to the Eflete project file.
 *
 * @return The #Project object, otherwise NULL.
 *
 * @ingroup ProjectManager
 */
Project *
pm_project_open(const char *path);

/**
 * Save all changes in current project to the dev file.
 *
 * @param project The project what should be saved.
 *
 * @param EINA_TRUE if project saved, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_save(Project *project);

/**
 * Save a current opened Style source code by the path. Backup file will created
 * after success project save.
 *
 * @param project The current opened project,
 * @param file The path to text file where will be saved a source code.
 *
 * @return EINA_TRUE if file is saved, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_style_save_as(Project *project, const char *file);

/**
 * Build the current project by one of profile #Build.
 *
 * @param project The current opened project,
 * @param build_profile The #Build profile.
 *
 * @return EINA_TRUE if project build succesful, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_build(Project *project, Build build_profile);

/**
 * Close the current project. Information about the project will be updated in
 * the '.pro' file and delete backup file.
 *
 * @param project The current opened project.
 *
 * @return EINA_TRUE if project save succesful, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_close(Project *project);

/**
 * Get a meta data from Project.
 *
 * @param project The project object.
 * @param name A pointer to variable where to store the project name,
 * @param authors A pointer to variable where to store the project authors,
 * @param version A pointer to variable where to store the project version,
 * @param license A pointer to variable where to store the project license,
 * @param comment A pointer to variable where to store the project comment.
 *
 * @note Use NULL pointers on the meta data you are not intrested.
 *
 * @ingroup ProjectManager
 */
void
pm_project_meta_data_get(Project *project,
                         char *name,
                         char *authors,
                         char *version,
                         char *license,
                         char *comment);

/**
 * Set a new meta data to the project.
 *
 * @param project The project object.
 * @param name A new project name,
 * @param authors A new project authors,
 * @param version A new project version,
 * @param license A new project license,
 * @param comment A new project comment.
 *
 * @return EINA_TRUE data is set, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_meta_data_set(Project *project,
                         const char *name,
                         const char *authors,
                         const char *version,
                         const char *license,
                         const char *comment);

#endif /* PROJECT_MANAGER_H */
