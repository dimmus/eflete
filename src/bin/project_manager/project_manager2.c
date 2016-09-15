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

#define _GNU_SOURCE
#include "project_manager2.h"
#include <fcntl.h>

#ifndef _WIN32
#include <sys/wait.h>
#else
#include <win32.h>
#endif

#define PROJECT_FILE_KEY      "project"

#define PROJECT_KEY_NAME         "edje/name"
#define PROJECT_KEY_AUTHORS      "edje/authors"
#define PROJECT_KEY_FILE_VERSION "edje/file_version"
#define PROJECT_KEY_LICENSE      "edje/license"
#define PROJECT_KEY_COMMENT      "edje/comment"

struct _Project_Process_Data
{
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
};
typedef struct _Project_Process_Data Project_Process_Data;

static void
_project_descriptor_init(Project_Process_Data *ppd)
{
   Eet_Data_Descriptor_Class eddc;

   assert(ppd != NULL);

   if (ppd->eed_project) return;

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc),
                                             "Project", sizeof(Project));
   ppd->eed_project = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC       (ppd->eed_project, Project, "version", version, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC       (ppd->eed_project, Project, "dev", dev, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC       (ppd->eed_project, Project, "saved_edj", saved_edj, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC       (ppd->eed_project, Project, "develop_path", develop_path, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC       (ppd->eed_project, Project, "release_options", release_options, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING (ppd->eed_project, Project, "images", res.images);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING (ppd->eed_project, Project, "sounds", res.sounds);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING (ppd->eed_project, Project, "fonts", res.fonts);
}

void
_pm_project_descriptor_shutdown(Project_Process_Data *ppd)
{
   eet_data_descriptor_free(ppd->eed_project);
   ppd->eed_project = NULL;
}

static void
_project_special_group_add(Project *project)
{
   Eina_List *groups;
   Evas_Object *obj;
   Ecore_Evas *ecore_evas;

   assert(project != NULL);

   groups = edje_file_collection_list(project->saved_edj);
   ecore_evas = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ecore_evas));
   edje_object_file_set(obj, project->saved_edj, eina_list_data_get(groups));

   you_shall_not_pass_editor_signals(NULL);
   CRIT_ON_FAIL(editor_internal_group_add(obj));
   you_shall_pass_editor_signals(NULL);

   ecore_evas_free(ecore_evas);
   edje_edit_string_list_free(groups);
}

static void
_project_dummy_sample_add(Project *project)
{
   char buf[PATH_MAX];
   Eina_List *groups;
   Evas_Object *obj;
   Ecore_Evas *ecore_evas;

   assert(project != NULL);

   groups = edje_file_collection_list(project->saved_edj);
   ecore_evas = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ecore_evas));
   edje_object_file_set(obj, project->saved_edj, eina_list_data_get(groups));

   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_SAMPLE_NAME, ap.path.sound_path);
   you_shall_not_pass_editor_signals(NULL);
   if (editor_sound_sample_add(obj, EFLETE_DUMMY_SAMPLE_NAME, buf, false))
     {
        CRIT_ON_FAIL(editor_save(obj));
     }
   you_shall_pass_editor_signals(NULL);

   ecore_evas_free(ecore_evas);
   edje_edit_string_list_free(groups);
}

static void
_project_dummy_image_add(Project *project)
{
   char buf[PATH_MAX];
   Eina_List *groups;
   Evas_Object *obj;
   Ecore_Evas *ecore_evas;

   assert(project != NULL);

   groups = edje_file_collection_list(project->saved_edj);
   ecore_evas = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ecore_evas));
   edje_object_file_set(obj, project->saved_edj, eina_list_data_get(groups));

   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_IMAGE_NAME, ap.path.image_path);
   you_shall_not_pass_editor_signals(NULL);
   CRIT_ON_FAIL(editor_image_add(obj, buf, false));
   you_shall_pass_editor_signals(NULL);

   ecore_evas_free(ecore_evas);
   edje_edit_string_list_free(groups);
}

static Eina_Bool
_project_dev_file_create(Project *pro)
{
   DBG("Create the .dev file.");
   return ecore_file_cp(pro->saved_edj, pro->dev);
}

static void
_end_send(void *data)
{
   PM_Project_End_Cb func;
   PM_Project_Result result;
   Project *project;
   void *udata;
   Project_Process_Data *ppd = data;

   /** Copy the links to callback and meesage, to fast release worker resource. */
   func = ppd->func_end;
   result = ppd->result;
   udata = ppd->data;
   project = ppd->project;
   ecore_event_handler_del(ppd->del_handler);
   ecore_event_handler_del(ppd->error_handler);
   ecore_event_handler_del(ppd->data_handler);

   if (ppd->tmp_dirname != NULL)
     {
        ecore_file_recursive_rm(ppd->tmp_dirname);
        eina_tmpstr_del(ppd->tmp_dirname);
     }

   free(ppd);
   func(udata, result, project);
}

#ifndef _WIN32
static Eina_Bool
_lock_try(const char *path, Eina_Bool check, int *pro_fd)
{
   struct flock lock, savelock;

   int fd = open(path, O_RDWR);
   lock.l_type    = F_WRLCK;   /* Test for any lock on any part of file. */
   lock.l_whence  = SEEK_SET;
   lock.l_start   = 0;
   lock.l_len     = 0;
   lock.l_pid     = 0;
   savelock = lock;
   fcntl(fd, F_GETLK, &lock);  /* Overwrites lock structure with preventors. */
   if ((lock.l_pid != 0) && ((lock.l_type == F_WRLCK) || (lock.l_type == F_RDLCK)))
     {
        ERR("Process %d has a write lock already!", lock.l_pid);
        return false;
     }

   /* if flag check is false not need to lock the file, just close handler */
   if (!check)
     {
        close(fd);
        return true;
     }

   if (pro_fd)
     {
        savelock.l_pid = getpid();
        fcntl(fd, F_SETLK, &savelock);
        *pro_fd = fd;
        return true;
     }
   close(fd);
   return false;
}
#else
static Eina_Bool
_lock_try(const char *path, Eina_Bool check, HANDLE *pro_fd)
{
   LPCTSTR  lpFileName = path;
   DWORD dwDesiredAccess = GENERIC_READ|GENERIC_WRITE;
   DWORD  dwShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
   LPSECURITY_ATTRIBUTES  lpSecurityAttributes = NULL;
   DWORD  dwCreationDisposition = OPEN_EXISTING;
   DWORD  dwFlagsAndAttributes = 0;
   HANDLE  hTemplateFile = NULL;
   HANDLE fd = CreateFile(lpFileName,
                          dwDesiredAccess,
                          dwShareMode,
                          lpSecurityAttributes,
                          dwCreationDisposition,
                          dwFlagsAndAttributes,
                          hTemplateFile);

   if (fd == INVALID_HANDLE_VALUE)
     {
        ERR("The file '%s' cannot be opened in mode read-write!", path);
        return !check;
     }

   if (!check)
     {
        CloseHandle(fd);
        return true;
     }
    if (pro_fd)
     {
        *pro_fd = fd;
        return true;
     }

    CloseHandle(fd);
    return false;
}
#endif


static Eina_Bool
_exe_output_print(void *data,
                  int type __UNUSED__,
                  void *event_info)
{
   int i;
   Ecore_Exe_Event_Data *edje_cc_msg = (Ecore_Exe_Event_Data *)event_info;
   Project_Process_Data *ppd = data;

   if (!edje_cc_msg) return ECORE_CALLBACK_DONE;

   for (i = 0; edje_cc_msg->lines[i].line != NULL; i++)
     ppd->func_progress(NULL, edje_cc_msg->lines[i].line);

   return ECORE_CALLBACK_DONE;
}


static Eina_Bool
_exe_finish(void *data,
            int type __UNUSED__,
            void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;
   Project *project = (Project *) ppd->project;

   resource_manager_init(project);

   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

static void
_project_open_internal(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   Project_Process_Data *ppd;
   char *spath;
   Ecore_Exe_Flags flags;
   char cmd[512];
   char *file_dir;

#ifdef _WIN32
    HANDLE pro_fd = NULL;
#else
    int pro_fd;
#endif

   assert(path != NULL);

   spath = eina_file_path_sanitize(path);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(spath);
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;

   Eet_File *ef;
   char *tmp;
   int tmp_len;
   Edje_Load_Error error;

   edje_file_cache_flush();

   if (!_lock_try(ppd->path, true,  &pro_fd))
     {
        /* really this case is unlickly, but we need handle it */
        ERR("Project file already locked by another application");
        return;
     }

   ef = eet_open(ppd->path, EET_FILE_MODE_READ_WRITE);
   if (!ef)
     {
        ERR("Failed to open project file handler");
        return;
     }

   _project_descriptor_init(ppd);
   ppd->project = eet_data_read(ef, ppd->eed_project, PROJECT_FILE_KEY);
   _pm_project_descriptor_shutdown(ppd);
   if (!ppd->project)
     {
        ERR("Failed to load project data");
        return;
     }

   ppd->project->ef = ef;
   ppd->project->pro_path = eina_stringshare_add(ppd->path);
   ppd->project->pro_fd = pro_fd;

   /* updating .dev file path */
   tmp = strdup(ppd->path);
   tmp_len = strlen(tmp);
   tmp[tmp_len - 3] = 'd';
   tmp[tmp_len - 2] = 'e';
   tmp[tmp_len - 1] = 'v';
   eina_stringshare_replace(&ppd->project->dev, tmp);
   free(tmp);
   /* updating .edj file path */
   tmp = strdup(ppd->path);
   tmp[tmp_len - 3] = 'e';
   tmp[tmp_len - 2] = 'd';
   tmp[tmp_len - 1] = 'j';
   eina_stringshare_replace(&ppd->project->saved_edj, tmp);
   free(tmp);

   ppd->project->mmap_file = eina_file_open(ppd->project->dev, false);

   ppd->project->changed = false;
   ppd->project->close_request = false;
   ppd->project->ecore_evas = ecore_evas_buffer_new(0, 0);
   ppd->project->global_object = edje_edit_object_add(ecore_evas_get(ppd->project->ecore_evas));
   edje_object_mmap_set(ppd->project->global_object, ppd->project->mmap_file, EFLETE_INTERNAL_GROUP_NAME);


   //message = eina_stringshare_add(_("Checking project version"));

   /* checking for older project versions and upgrading them version-by-version */
   if (ppd->project->version < 2) /* upgrade to version 2 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 2"));
        INFO("Updating project files to version 2");
        ecore_file_mv(ppd->project->dev, ppd->project->saved_edj);
        ppd->project->version = 2;
     }
   if (ppd->project->version < 3) /* upgrade to version 3 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 3"));
        INFO("Updating project files to version 3");
        _project_special_group_add(ppd->project);
        ppd->project->version = 3;
     }
   if (ppd->project->version < 4) /* upgrade to version 4 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 4"));
        INFO("Updating project files to version 4");
        _project_dummy_image_add(ppd->project);
        ppd->project->version = 4;
     }
   if (ppd->project->version < 5) /* upgrade to version 4 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 5"));
        INFO("Updating project files to version 5");
        _project_dummy_sample_add(ppd->project);
        ppd->project->version = 5;
     }
    TODO("Add crash recovery prompt here")

   if (!edje_file_group_exists(ppd->project->saved_edj, EFLETE_INTERNAL_GROUP_NAME))
     {
        CRIT("Missing internal group '"EFLETE_INTERNAL_GROUP_NAME" in file %s\n",
             ppd->project->saved_edj);
        return;
     }
   _project_dev_file_create(ppd->project);
   ppd->project->mmap_file = eina_file_open(ppd->project->dev, false);

   ppd->project->changed = false;
   ppd->project->close_request = false;
   ppd->project->ecore_evas = ecore_evas_buffer_new(0, 0);
   ppd->project->global_object = edje_edit_object_add(ecore_evas_get(ppd->project->ecore_evas));
   edje_object_mmap_set(ppd->project->global_object, ppd->project->mmap_file, EFLETE_INTERNAL_GROUP_NAME);
   error = edje_object_load_error_get(ppd->project->global_object);
   if (EDJE_LOAD_ERROR_NONE != error)
     {
         CRIT("Could not load internal object: %s\n", edje_load_error_str(error));
         return;
     }

   pm_project_meta_data_get(ppd->project, &ppd->project->name, NULL, NULL, NULL, NULL);
   if (!ppd->project->name)
     ppd->project->name = eina_stringshare_add(ecore_file_strip_ext(ecore_file_file_get(spath)));

   _project_dev_file_create(ppd->project);

/******************************************************************************/
   file_dir = ecore_file_dir_get(spath);
   snprintf(cmd, strlen(ppd->project->saved_edj) + strlen("eflete_exporter --edj ") + strlen(file_dir) + strlen(" --path /develop") + 1,
            "eflete_exporter --edj %s --path %s/develop", ppd->project->saved_edj, file_dir);

   flags = ECORE_EXE_PIPE_READ | ECORE_EXE_PIPE_READ_LINE_BUFFERED |
           ECORE_EXE_PIPE_ERROR | ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   ecore_exe_pipe_run(cmd, flags, NULL);
   //ecore_exe_auto_limits_set(exe_cmd, 0, -1, 1, 1);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_print, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_output_print, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _exe_finish, ppd);

   free(file_dir);
   free(spath);
}

void
pm_project_open(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   _project_open_internal(path, func_progress, func_end, data);
}
