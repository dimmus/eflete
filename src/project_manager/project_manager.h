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

#include "widget_manager.h"
#include "logger.h"
#include <Eet.h>

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
   /** The project name */
   Eina_Stringshare *name;
   /** Eet_File descriptior of specific project file. */
   Eet_File *pro;
   /** this is worrking file, all changes are happened in this file. */
   Eina_Stringshare *dev;

   /** path where will be saved the develop edj file */
   Eina_Stringshare *develop_path;
   /** compile options for release edj file. see edje_cc reference */
   Eina_Stringshare *release_options;

   /** current editing group */
   Style *current_style;
   /** list of widgets and they styles in that theme */
   Eina_Inlist *widgets;
   /**< list of custom layouts int loaded theme */
   Eina_Inlist *layouts;

   Eina_List *added_sounds;

   Eina_Bool changed : 1;
   Eina_Bool close_request : 1;
   //Ecore_Timer *autosave_timer;
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
 * @enum _PM_Project_Result
 *
 * The Project process result, it's means result of all project process: import,
 * new project creation, save, etc.
 *
 * @ingroup ProjectManager
 */
enum _PM_Project_Result
{
   PM_PROJECT_SUCCESS,
   PM_PROJECT_CANCEL,
   PM_PROJECT_ERROR,
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
typedef enum _PM_Project_Result PM_Project_Result;

/**
 * @typedef Project_Thread
 * @ingroup ProjectManager
 */
typedef struct _Project_Thread Project_Thread;

/**
 * @typedef PM_Project_Progress_Cb
 *
 * The Project process callback, this callback be called to receive the progress
 * data (string).
 *
 * @param data The user data;
 * @param progress_string The progress string from running Project thread.
 *
 * @return EINA_TRUE if callback done success, otherwise EINA_FALSE.
 *
 * @note If callback return EINA_FALSE the Project thread will be stoped.
 *
 * @ingroup ProjectManager
 */
typedef Eina_Bool
(* PM_Project_Progress_Cb)(void *data, Eina_Stringshare *progress_string);

/**
 * @typedef PM_Project_End_Cb
 *
 * This callback be called on the end of Project process.
 *
 * @param data The user data;
 * @param result The project pro.
 *
 * @ingroup ProjectManager
 */
typedef void
(* PM_Project_End_Cb)(void *data, PM_Project_Result result);

/**
 * @struct _Project_Thread
 *
 * A handler for Project process.
 *
 * @ingroup ProjectManager
 */
struct _Project_Thread
{
   /** The handler of Project thread. */
   Eina_Thread thread;
   /** The progress callback. See #PM_Project_Progress_Cb.*/
   PM_Project_Progress_Cb func_progress;
   /** The end callback. See #PM_Project_End_Cb. */
   PM_Project_End_Cb func_end;
   /** The project process result. */
   PM_Project_Result result;
   /** The user data. */
   void *data;
   /** The new project, was created in the Project process. This pointer will be
    * NULL until the Project process finished it's job.*/
   Project *project;
   /** Name of project what must be created. */
   const char *name;
   /** Path to new project. */
   const char *path;
   /** Path to imported edj file. */
   const char *edj;
   /** Path to imported edc file. */
   const char *edc;
   /** edje_cc options. Used for 'new project' and 'import from edc'. */
   const char *build_options;
   /** Message string */
   Eina_Stringshare *message;
   /** Mutex, I say no more then. */
   Eina_Lock mutex;
};

/**
 * Free the Project Thread object.
 *
 * @param worker The Project thread.
 *
 * @return EINA_TRUE on success, EINA_FALSE if thread running.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_thread_free(Project_Thread *worker) EINA_ARG_NONNULL(1);

/**
 * Create a new project which based on the imported edj file.
 *
 * @param name The name of new project;
 * @param path The path of new project, by this path will be created a project
 *             folder;
 * @param edj The path to the edj file wich will imported;
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @return The new #Project_Thread object, othewise NULL.
 *
 * @ingroup ProjectManager
 */
Project_Thread *
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data) EINA_ARG_NONNULL(1, 2, 3) EINA_WARN_UNUSED_RESULT;

/**
 * Create a new project which base on the imported edc file.
 * This function try to compile a current edc file, if copilation success,
 * object Project will be returned.
 *
 * @param name The name of new project,
 * @param path The path of new project, by this path will be created a project
 *             folder,
 * @param edc The path to the edc file wich will imported,
 * @param import_options The import oprions, it's mean the addtional options for
 *                       edje_cc, like '-id' - image directory, '-fd', '-sd',
 *                       etc.
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @return The new #Project_Thread object, othewise NULL.
 *
 * @note Function will not check a edc file, and directories.
 *
 * @ingroup ProjectManager
 */
Project_Thread *
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *import_options,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data) EINA_ARG_NONNULL(1, 2, 3, 4) EINA_WARN_UNUSED_RESULT;

/**
 * Get the Project object from thread. If thread not finished, function will
 * return NULL.
 *
 * @param worker The Project thread.
 *
 * @return Project object, or NULL if thread not finished or finished with error.
 *
 * @ingroup ProjectManager
 */
inline Project *
pm_project_thread_project_get(Project_Thread *worker);

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
pm_project_open(const char *path) EINA_ARG_NONNULL(1);

/**
 * Save all changes in current project to the dev file.
 *
 * @param project The project what should be saved.
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @return The new #Project_Thread object, othewise NULL.
 *
 * @ingroup ProjectManager
 */
Project_Thread *
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data) EINA_ARG_NONNULL(1);

/**
 * Cancel the Project thread, and called func_end.
 *
 * @param worker The Project thread.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_thread_cancel(Project_Thread *worker) EINA_ARG_NONNULL(1);


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
pm_project_close(Project *project) EINA_ARG_NONNULL(1);

/**
 * Mark project as changed
 *
 * Changed flag will be automatically dropped when project will be saved.
 * When closing project that is marked as changed, warning will be shown
 *
 * @param project A Project structure;
 *
 * @ingroup ProjectManager
 */
void
pm_project_changed(Project *project) EINA_ARG_NONNULL(1);

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
                         Eina_Stringshare **name,
                         Eina_Stringshare **authors,
                         Eina_Stringshare **version,
                         Eina_Stringshare **license,
                         Eina_Stringshare **comment) EINA_ARG_NONNULL(1);

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
                         const char *comment) EINA_ARG_NONNULL(1);

/**
 * Export opened project resource. Export images, sounds, fonts and data from
 * opened project to specific folder-container.
 * This data will be used on compile debug and release edj file. Also Enventor
 * used this data.
 *
 * @param pro The opened project.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager.
 */
Eina_Bool
pm_project_resource_export(Project *pro);

/**
 * Export the source code of Style (edje object) to file.
 * If file is NULL, file will be saved to develop folder of project by name
 * "tmp.edc"
 *
 * @param pro The opened project;
 * @param style The style to save the source code;
 * @param file The file for save.
 *
 * @return The source code on success or NULL otherwise.
 *
 * @ingroup ProjectManager.
 */
Eina_Stringshare *
pm_project_style_source_code_export(Project *pro, Style *style, const char *file);

/**
 * Export the edj develop file from current project. The develop edj file
 * include unused images, sounds, fonts and data files.
 *
 * @param pro The opened project;
 * @param path Path to save the edj file.
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @return The new #Project_Thread object, othewise NULL.
 *
 * @ingroup ProjectManager.
 */
Project_Thread *
pm_project_develop_export(Project *pro,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * Save the current editing style as edj file.
 *
 * @param project The project what should be saved.
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @return The new #Project_Thread object, othewise NULL.
 *
 * @warning Use only in enventor mode.
 *
 * @ingroup ProjectManager
 */
Project_Thread *
pm_project_enventor_save(Project *project,
                         PM_Project_Progress_Cb func_progress,
                         PM_Project_End_Cb func_end,
                         const void *data) EINA_ARG_NONNULL(1);




#endif /* PROJECT_MANAGER_H */
