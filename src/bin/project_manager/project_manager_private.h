/*
 * Efl Edje Theme Editor
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

#ifndef PROJECT_MANAGER_PRIVATE_H
#define PROJECT_MANAGER_PRIVATE_H

#include "eflete.h"
#include "logger.h"

#define PROJECT_FILE_KEY      "project"

#define PROJECT_KEY_NAME         "edje/name"
#define PROJECT_KEY_AUTHORS      "edje/authors"
#define PROJECT_KEY_FILE_VERSION "edje/file_version"
#define PROJECT_KEY_LICENSE      "edje/license"
#define PROJECT_KEY_COMMENT      "edje/comment"


/* A handler for Project process. */
typedef struct
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
   /** */
   const char *source_edj;
   /** The checked widgets. Used for loading just checked widgets. */
   Eina_List *widgets;
   /** The temporary directory. Should be stored here for a correct clean temporary files. */
   Eina_Tmpstr *tmp_dirname;
   /** The group, which source should be exported into edc. */
   Group *group;
   /** Temporary path, for deliver works path between pipes and threads */
   Eina_Stringshare *tmp_path;

   /** Handlers for monitoring events from exe pipes. */
   Ecore_Event_Handler *data_handler;
   Ecore_Event_Handler *error_handler;
   Ecore_Event_Handler *del_handler;

   /** Desriptor for read data form eet file */
   Eet_Data_Descriptor *eed_project;

   /** Mutext for resolve multithread resource usage */
   Eina_Lock mutex;
} Project_Thread;

/* A struture for pass data into ecore_exe callbacks */
typedef struct
{
   /** Full command that was passed into exe_pipe. */
   Eina_Stringshare *cmd;
   /** A process handle to the spawned process.*/
   Ecore_Exe *exe_cmd;
   /** The process ID. */
   pid_t exe_pid;
   /** The flag parameters for how to deal with inter-process I/O. */
   Ecore_Exe_Flags flags;
   /** User data that will be passed into callback functions. */
   void *data;
} Edje_Exe_Data;

typedef struct {
     int queue;
     Eina_Lock mutex;
     Eina_Condition condition;
} Feedback_Thread_Data;

/* General funcions */

/* Send result of works one of project manager submodules.
 * This function finalize process. It's mean clear data
 * of Project_Thread structure.
 * @param data - pointer to Project_Thread structure.
 * */
void _end_send(void *data);

/* Prepare descriptor structure for reading *.pro file */
void _project_descriptor_init(Project_Thread *ptd);

/* Destroy descriptor, that provide ability to read *.pro files */
void _pm_project_descriptor_shutdown(Project_Thread *ptd);

/* Used for fill project structure with valid data.
 * Open shared file handler for a *.dev file.
 * Create global edje_edit object.
 * Create and load all resource files.
 * Load Group tree structure.
 */
void _project_open_internal(Project_Thread *ptd);

/* Try to lock *.pro file for avoid situations when two or more
 * instances of Eflete works with the same project in the same time.
 *
 * @param path - *.pro file.
 * @param check - If false - do not lock file, if true - file will be locked.
 *
 * @return true - file locked succesfull, else failed to lock.
 */
Eina_Bool _lock_try(const char *path, Eina_Bool check);

/* Add dummy group with name ___eflete_internal_group___.
 * This group will be used as pointer to edje_edit object,
 * for manipulate Edje structures by using edje_edit API.
 *
 * @param project - valid struture, for add global_object.
 */
void _project_special_group_add(Project *project);

/* Add dummy image with name ___eflete_dummy_image___.
 *
 * @param project - valid struture with global_object.
 */
void _project_dummy_image_add(Project *project);

/* Copy current edj file into path, that stored for save */
Eina_Bool _project_edj_file_copy(void);

/* Direct read meta data from eet file and copy it
 * inside *.pro file.
 */
void _copy_meta_data_to_pro(void);

/* Allocate Project structure and prepare data
 * for files tree of project.
 */
Project *_project_files_create(Project_Thread *ptd);

/*------- Group tree load functions ---------*/
void _gm_group_load_cancel_cb(void *data, Ecore_Thread *th);
void _gm_group_load_end_cb(void *data, Ecore_Thread *th);
void _gm_group_load_feedback_job(void *data, Ecore_Thread *th);

/*------- Export resources functions --------*/
void _image_resources_feedback_job(void *data, Ecore_Thread *th);
void _sound_resources_feedback_job(void *data, Ecore_Thread *th);
void _font_resources_feedback_job(void *data, Ecore_Thread *th);
void _tones_resources_feedback_job(void *data, Ecore_Thread *th);
void _colorclasses_resources_feedback_job(void *data, Ecore_Thread *th);
void _styles_resources_feedback_job(void *data, Ecore_Thread *th);
void _resources_export_end_cb(void *data, Ecore_Thread *th);
void _resources_export_feedback_cb(void *data, Ecore_Thread *th, void *msg_data);
void _resources_export_cancel_cb(void *data, Ecore_Thread *th);

/*------- Open Project functions --------*/
void _project_open_cancel_cb(void *data, Ecore_Thread *th);

void _project_open_end_cb(void *data, Ecore_Thread *th);

void _project_open_feedback_cb(void *data, Ecore_Thread *th, void *msg_data);

void _project_open_feedback_job(void *data, Ecore_Thread *th);

/*------- Import form edj functions -----*/
void _project_import_edj(void *data);

#endif
