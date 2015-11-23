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

#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif /* include eflete_config.h */


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
#include <assert.h>

/* don't forget to update on major changes */
#define PROJECT_FILE_VERSION 3

typedef struct _Enventor_Data Enventor_Data;

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
   /* version of project file */
   int version;
   /** The project name */
   Eina_Stringshare *name;
   /** project path */
   Eina_Stringshare *pro_path;
   /** this is worrking file, all changes are happened in this file. */
   Eina_Stringshare *dev;
   /** ecore evas buffer for project objects */
   Ecore_Evas *ecore_evas;
   /** edje_edit_object for global operations */
   Evas_Object *global_object;
   /** this is saved file. */
   Eina_Stringshare *saved_edj;

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

   Eina_List *groups;
   Eina_List *images;
   Eina_List *sounds;
   Eina_List *tones;
   Eina_List *fonts;
   Eina_List *colorclasses;
   Eina_List *styles;

   Eina_File *mmap_file; /**< mmaped dev file*/

   Eina_List *nsimage_list;

   TODO("Fix paths in enventor mode. Looks like enventor project can't be moved")
   struct
   {
      Eina_List *images;  /**< pathes to the image dirs */
      Eina_List *sounds;  /**< pathes to the sound dirs */
      Eina_List *fonts;   /**< pathes to the font dirs */
   } res;  /**< pathes to resource directories, which needed for opened project */

#ifdef HAVE_ENVENTOR
   Enventor_Data *enventor;
#endif

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
 * Free the Project Thread object.
 *
 * @return EINA_TRUE on success, EINA_FALSE if thread running.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_thread_free(void);

/**
 * Get project result.
 *
 * @return project thread result.
 *
 * @ingroup ProjectManager
 */
PM_Project_Result
pm_project_thread_result_get(void);

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
 * @ingroup ProjectManager
 */
void
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data) EINA_ARG_NONNULL(1, 2, 3);

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
 * @note Function will not check a edc file, and directories.
 *
 * @ingroup ProjectManager
 */
void
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *import_options,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data) EINA_ARG_NONNULL(1, 2, 3, 4);

/**
 * Get the Project object from thread. If thread not finished, function will
 * return NULL.
 *
 * @return Project object, or NULL if thread not finished or finished with error.
 *
 * @ingroup ProjectManager
 */
Project *
pm_project_thread_project_get();

/**
 * Open Eflete project.
 *
 * @param path The path to the Eflete project file.
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @ingroup ProjectManager
 */
void
pm_project_open(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data);

/**
 * Reloading dev file. Should be called after every save.
 *
 * @param project The project.
 *
 * @ingroup ProjectManager
 */
void
pm_dev_file_reload(Project *project);

/**
 * Save all changes in current project to the dev file.
 *
 * @param project The project what should be saved.
 * @param func_progress The progress callback;
 * @param func_end The end callback, this callback be called on the end of
 *        Project progress;
 * @param data The user data.
 *
 * @ingroup ProjectManager
 */
void
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data) EINA_ARG_NONNULL(1);

/**
 * Cancel the Project thread, and called func_end.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager
 */
Eina_Bool
pm_project_thread_cancel(void);


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
 * @param dir_path export directory path;
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager.
 */
Eina_Bool
pm_project_resource_export(Project *pro, const char* dir_path);

/**
 * Export the source code of Style (edje object) to file.
 * If file is NULL, file will be saved to develop folder of project by name
 * "tmp.edc"
 *
 * @param pro The opened project;
 * @param style The style to save the source code;
 * @param file The file for save.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager.
 */
Eina_Bool
pm_project_style_source_code_export(Project *pro, Style *style, const char *file);

/**
 * Export the source code of Project (for each style edje objects) to a directory
 * with general file 'project_name.edc' that includes each 'style.edc', which are
 * created separately, and top level blocks.
 * build.sh file is created after.
 *
 * @param pro The opened project;
 * @param dir_path directory path where source code would be exported;
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup ProjectManager.
 */
void
pm_project_source_code_export(Project *project,
                              const char *path,
                              PM_Project_Progress_Cb func_progress,
                              PM_Project_End_Cb func_end,
                              const void *data);

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
 * @ingroup ProjectManager.
 */
void
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
 * @warning Use only in enventor mode.
 *
 * @ingroup ProjectManager
 */
void
pm_project_enventor_save(Project *project,
                         PM_Project_Progress_Cb func_progress,
                         PM_Project_End_Cb func_end,
                         const void *data) EINA_ARG_NONNULL(1);

/**
 * Export the resources, that uses into given style.
 *
 * This function collect lists of the resources, that uses in tgiven style. It
 * can be images, sounds or fonts.
 * Export images into directory path/images, in case if image stored in edj
 * have path (like: O/border.png) path will restored (path/images/O/border.png).
 * Will exported all images for group, include images from tween lists.
 * Export sounds, that uses in programs. Will saved into directory path/sounds
 * Export fonts into directory path/fonts
 *
 * @param pro The opened project;
 * @param style The style to save the source code;
 * @param path The dir, where resources will stored.
 *
 * @return EINA_TRUE if resources exported successful or EINA_FALSE in otherwise.
 *
 * @ingroup ProjectManager.
 */
Eina_Bool
pm_style_resource_export(Project *pro, Style *style, Eina_Stringshare *path);


/**
 * @struct _Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _Resource
{
   Eina_Stringshare *name;
   Eina_List *used_in;
};

/**
 * @typedef Resource
 * @ingroup ProjectManager
 */
typedef struct _Resource Resource;

/**
 * @struct _External_Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _External_Resource
{
   Eina_Stringshare *name;
   Eina_List *used_in;
   Eina_Stringshare *source;
};

/**
 * @struct _Tone_Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _Tone_Resource
{
   Eina_Stringshare *name;
   Eina_List *used_in;
   int freq;
};

/**
 * @struct _Colorclass_Resource
 *
 * Common structure for resources that can be used somewhere (images, sounds,
 * states etc.)
 *
 * @ingroup ProjectManager
 */
struct _Colorclass_Resource
{
   Eina_Stringshare *name;
   Eina_List *used_in;

   struct {
      int r,g,b,a;
   } color1;
   struct {
      int r,g,b,a;
   } color2;
   struct {
      int r,g,b,a;
   } color3;
};

/**
 * @typedef Colorclass_Resource
 * @ingroup ProjectManager
 */
typedef struct _Colorclass_Resource Colorclass_Resource;


/**
 * @typedef Tone_Resource
 * @ingroup ProjectManager
 */
typedef struct _Tone_Resource Tone_Resource;


/**
 * @typedef External_Resource
 * @ingroup ProjectManager
 */
typedef struct _External_Resource External_Resource;


static int
resource_cmp(Resource *res1, Resource *res2)
{
   return strcmp(res1->name, res2->name);
}

/**
 * Find resource in sorted list by its name.
 *
 * @param list Resources list
 * @param name Name of the resource to be found
 *
 * @return pointer to resource or NULL if it was not found
 *
 * @ingroup ProjectManager.
 */
static inline void*
pm_resource_get(Eina_List *list, Eina_Stringshare *name)
{
   Resource res;
   res.name = name;
   return eina_list_search_sorted(list, (Eina_Compare_Cb)resource_cmp, &res);
}

/**
 * Find resource in not sorted list by its name.
 *
 * @param list Resources list
 * @param name Name of the resource to be found
 *
 * @return pointer to resource or NULL if it was not found
 *
 * @ingroup ProjectManager.
 */
static inline void*
pm_resource_unsorted_get(Eina_List *list, Eina_Stringshare *name)
{
   Resource res;
   res.name = name;
   return eina_list_search_unsorted(list, (Eina_Compare_Cb)resource_cmp, &res);
}


/**
 * Add reference to resource with info where it is used (i.e. part for images)
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used
 *
 * @ingroup ProjectManager.
 */
static inline Eina_Bool
pm_resource_usage_add(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_get(list, name);

   if (!res)
      return false;

   res->used_in = eina_list_sorted_insert(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);
   return true;
}

/**
 * Add reference to resource with info where it is used (i.e. part for images)
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used
 *
 * @ingroup ProjectManager.
 */
static inline Eina_Bool
pm_resource_usage_unsorted_add(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_unsorted_get(list, name);

   if (!res)
      return false;

   res->used_in = eina_list_sorted_insert(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);
   return true;
}

/**
 * Remove resource.
 *
 * @param list Resources list
 * @param res Resource structure
 *
 * @ingroup ProjectManager.
 */
static inline Eina_List *
pm_resource_del(Eina_List *list, void *res)
{
   assert(res != NULL);
   assert(list != NULL);
   list = eina_list_remove(list, res);
   return list;
}

/**
 * Remove reference to resource.
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used. Must be added to usage list.
 *
 * @ingroup ProjectManager.
 */
static inline void
pm_resource_usage_del(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_get(list, name);
   Eina_List *l_del;

   assert(res != NULL);

   l_del = eina_list_search_sorted_list(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);

   assert(l_del);

   res->used_in = eina_list_remove_list(res->used_in, l_del);
}

/**
 * Remove reference to resource.
 *
 * @param list Resources list
 * @param name Name of the resource. Must be in the list.
 * @param usage_data Place where resource is used. Must be added to usage list.
 *
 * @ingroup ProjectManager.
 */
static inline void
pm_resource_usage_unsorted_del(Eina_List *list, Eina_Stringshare *name, void *usage_data)
{
   Resource *res = (Resource *) pm_resource_unsorted_get(list, name);
   Eina_List *l_del;

   assert(res != NULL);

   l_del = eina_list_search_sorted_list(res->used_in, (Eina_Compare_Cb)resource_cmp, usage_data);

   assert(l_del);

   res->used_in = eina_list_remove_list(res->used_in, l_del);
}
#endif /* PROJECT_MANAGER_H */
