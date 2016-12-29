/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#ifndef PROJECT_MANAGER_DATA_H
#define PROJECT_MANAGER_DATA_H

#include <Eina.h>
#include <Eet.h>
#include <Ecore_Evas.h>

/* don't forget to update on major changes */
#define PROJECT_FILE_VERSION 5

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
   /** the .pro file descriptor */
   Eet_File *ef;
   /** this is worrking file, all changes are happened in this file. */
   Eina_Stringshare *dev;
   /** ecore evas buffer for project objects */
   Ecore_Evas *ecore_evas;
   /** edje_edit_object for global operations */
   Evas_Object *global_object;
   /** this is saved file. */
   Eina_Stringshare *saved_edj;

   /** File descriptor of open "project.lock" file. Needed for keep that file locked*/
#ifdef _WIN32
   HANDLE fd_lock;
#else
   int fd_lock;
#endif

   /** path where will be saved the develop edj file */
   Eina_Stringshare *develop_path;
   /** compile options for release edj file. see edje_cc reference */
   Eina_Stringshare *release_options;

   struct {
      Eina_List *groups;
      Eina_List *tones;
      Eina_List *sounds;
      Eina_List *vectors;
      Eina_List *images;
      Eina_List *image_sets;
      Eina_List *fonts;
      Eina_List *colorclasses;
      Eina_List *styles;
      Eina_List *global_data;
   } RM; /* rename to resource_manager,
            this is just to make coding simplier. easier and faster */

   Eina_List *groups;
   Eina_List *images;
   Eina_List *image_sets;
   Eina_List *sounds;
   Eina_List *tones;
   Eina_List *fonts;
   Eina_List *colorclasses;
   Eina_List *styles;
   Eina_List *global_data;

   Eina_File *mmap_file; /**< mmaped dev file*/

   Eina_List *nsimage_list;

#ifdef HAVE_ENVENTOR
   Enventor_Data *enventor;
#endif

   Eina_Bool changed : 1;
   Eina_Bool close_request : 1;
};

/**
 *
 */
typedef struct _Project Project;

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
   PM_PROJECT_SUCCESS = 0,
   PM_PROJECT_CANCEL,
   PM_PROJECT_ERROR,
   PM_PROJECT_LOCKED,
   PM_PROJECT_CREATE_PRO_FAILED,
   PM_PROJECT_OPEN_PRO_FAILED,
   PM_PROJECT_READ_PRO_FAILED,
   PM_PROJECT_WRITE_PRO_FAILED,
   PM_PROJECT_ADD_SPEC_GROUP_FAILED,
   PM_PROJECT_ADD_SPEC_IMAGE_FAILED,
   PM_PROJECT_ADD_SPEC_SAMPLE_FAILED,
   PM_PROJECT_LOAD_INTERNAL_OBJ_FAILED,
   PM_PROJECT_LOAD_GROUP_FAILED,
   PM_PROJECT_COPY_GROUP_FAILED,
   PM_PROJECT_COPY_FILE_FAILED,
   PM_PROJECT_EXPORT_CREATE_PATH_FAILED,
   PM_PROJECT_EXPORT_COPY_FILE_FAILED,
   PM_PROJECT_EXPORT_SAVE_IMAGE_FAILED,
   PM_PROJECT_EXPORT_WRONG_IMAGE_ID,
   PM_PROJECT_EXPORT_CREATE_FILE_FAILED,
   PM_PROJECT_EXPORT_SAVE_SAMPLE_FAILED,
   PM_PROJECT_EXPORT_READ_EDJ_FONT_FAILED,
   PM_PROJECT_EXPORT_SAVE_FONT_FAILED,
   PM_PROJECT_EXPORT_CREATE_OBJECT_FAILED,
   PM_PROJECT_EXPORT_DEVELOP_EDJ_FAILED,
   PM_PROJECT_EXPORT_RELEASE_EDJ_FAILED,
   PM_PROJECT_BUILD_SOURCE_EDC_FAILED,
   PM_PROJECT_LOCKED_PROC_MISS,
   PM_PROJECT_LOCKED_PERMISSION,
   PM_PROJECT_LAST
};

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
(* PM_Project_End_Cb)(void *data, PM_Project_Result result, Project *project);



#endif /* PROJECT_MANAGER_DATA_H */
