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


#define FLAGS ECORE_EXE_PIPE_READ | ECORE_EXE_PIPE_READ_LINE_BUFFERED | \
              ECORE_EXE_PIPE_ERROR | ECORE_EXE_PIPE_ERROR_LINE_BUFFERED

static Eet_Compression compess_level = EET_COMPRESSION_HI;

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
   Group2 *group;
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

static Project *
_project_create(Project_Process_Data *ppd)
{
   Project *pro;
   Eina_Bool error = false;
   char buf[PATH_MAX];

   _project_descriptor_init(ppd);

   snprintf(buf, sizeof(buf), "%s/%s", ppd->path, ppd->name);
   if (ecore_file_mkdir(buf))
     {
        DBG("Create the folder '%s' for new project '%s'", buf, ppd->name);
     }
   else
     {
        ERR("Could't create a project folder!");
        error = true;
     }
   if (error) return NULL;

   pro = (Project *)mem_calloc(1, sizeof(Project));
   pro->version = PROJECT_FILE_VERSION;
   pro->name = eina_stringshare_add(ppd->name);
   pro->dev = eina_stringshare_printf("%s/%s.dev", buf, ppd->name);
   pro->saved_edj = eina_stringshare_printf("%s/%s.edj", buf, ppd->name);
   pro->develop_path = eina_stringshare_printf("%s/develop", buf);

   snprintf(buf, sizeof(buf), "%s/%s/%s.pro", ppd->path, ppd->name, ppd->name);
   pro->ef = eet_open(buf, EET_FILE_MODE_READ_WRITE);
   DBG("Create a specific project file '%s': %s", buf, error ? "failed" : "success");
   pro->pro_fd = -1;
   ecore_file_mkdir(pro->develop_path);
   snprintf(buf, sizeof(buf), "%s/images", pro->develop_path);
   ecore_file_mkdir(buf);
   snprintf(buf, sizeof(buf), "%s/sounds", pro->develop_path);
   ecore_file_mkdir(buf);
   snprintf(buf, sizeof(buf), "%s/fonts", pro->develop_path);
   ecore_file_mkdir(buf);

   if (!eet_data_write(pro->ef, ppd->eed_project, PROJECT_FILE_KEY, pro, compess_level))
     error = true;

   _pm_project_descriptor_shutdown(ppd);
   if (error)
     {
        ERR("Could't create a .pro file! ")
        eina_stringshare_del(pro->name);
        eina_stringshare_del(pro->dev);
        eina_stringshare_del(pro->saved_edj);
        eina_stringshare_del(pro->develop_path);
        free(pro);
        pro = NULL;
     }

   eet_close(pro->ef);
   return pro;
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
_project_edj_file_copy(Project_Process_Data *ppd)
{
   Eina_Stringshare *src, *dst;
   Eina_Bool result;

   src = eina_stringshare_ref(ppd->edj);
   dst = eina_stringshare_ref(ppd->project->saved_edj);
   result = eina_file_copy(src, dst,
                           EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                           NULL, NULL);

   DBG("Copy the .edj file to project folder.");
   eina_stringshare_del(src);
   eina_stringshare_del(dst);

   return result;
}

static void
_copy_meta_data_to_pro(Project_Process_Data *ppd)
{
   Eet_File *ef;
   char *name, *authors, *version, *license, *comment;

   ef = eet_open(ppd->edj, EET_FILE_MODE_READ_WRITE);

   name = strdup(ppd->name);
   authors = eet_read(ef, PROJECT_KEY_AUTHORS, NULL);
   version = eet_read(ef, PROJECT_KEY_FILE_VERSION, NULL);
   license = eet_read(ef, PROJECT_KEY_LICENSE, NULL);
   comment = eet_read(ef, PROJECT_KEY_COMMENT, NULL);
   eet_close(ef);

   pm_project_meta_data_set(ppd->project, name, authors,
                            version, license, comment);

   if (name) free(name);
   if (authors) free(authors);
   if (version) free(version);
   if (license) free(license);
   if (comment) free(comment);
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
_exe_output_handler(void *data,
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
_exe_error_handler(void *data,
                   int type __UNUSED__,
                   void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   ppd->result = PM_PROJECT_ERROR;
   _end_send(ppd);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_exporter_finish_handler(void *data,
                         int type __UNUSED__,
                         void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;
   Project *project = (Project *) ppd->project;

   resource_manager_init(project);

   ppd->result = PM_PROJECT_SUCCESS;
   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_project_open_internal(Project_Process_Data *ppd)
{
   char cmd[PATH_MAX];
   char *file_dir;

#ifdef _WIN32
    HANDLE pro_fd = NULL;
#else
    int pro_fd;
#endif

   Eet_File *ef;
   char *tmp;
   int tmp_len;
   Edje_Load_Error error;

   edje_file_cache_flush();

   if (!_lock_try(ppd->path, true,  &pro_fd))
     {
        /* really this case is unlickly, but we need handle it */
        ERR("Project file already locked by another application");
        return false;
     }

   ef = eet_open(ppd->path, EET_FILE_MODE_READ_WRITE);
   if (!ef)
     {
        ERR("Failed to open project file handler");
        return false;
     }

   _project_descriptor_init(ppd);
   ppd->project = eet_data_read(ef, ppd->eed_project, PROJECT_FILE_KEY);
   _pm_project_descriptor_shutdown(ppd);
   if (!ppd->project)
     {
        ERR("Failed to load project data");
        return false;
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
        return false;
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
         return false;
     }

   pm_project_meta_data_get(ppd->project, &ppd->project->name, NULL, NULL, NULL, NULL);
   if (!ppd->project->name)
     ppd->project->name = eina_stringshare_add(ecore_file_strip_ext(ecore_file_file_get(ppd->path)));


/******************************************************************************/
   file_dir = ecore_file_dir_get(ppd->path);
   snprintf(cmd, sizeof(cmd),
            "eflete_exporter --edj %s --path %s/develop", ppd->project->saved_edj, file_dir);

   ecore_exe_pipe_run(cmd, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_error_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _exporter_finish_handler, ppd);

   free(file_dir);

   return true;
}

void
pm_project_open(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   Project_Process_Data *ppd;
   char *spath;

   assert(path != NULL);

   spath = eina_file_path_sanitize(path);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(spath);
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;

   if (!_project_open_internal(ppd))
     {
        eina_stringshare_del(ppd->path);
        free(ppd);
     }

   free(spath);
}

static Eina_Bool
_edje_pick_finish_handler(void *data,
                          int type __UNUSED__,
                          void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   /* reset path variable for open project. _project_open_internal use this
    * var for path to .pro file */
   eina_stringshare_del(ppd->path);
   ppd->path = eina_stringshare_ref(ppd->project->pro_path);

   ecore_event_handler_del(ppd->del_handler);
   ecore_event_handler_del(ppd->error_handler);
   ecore_event_handler_del(ppd->data_handler);

   ecore_file_cp(ppd->edj, ppd->project->saved_edj);
   if (!_project_open_internal(ppd))
     return ECORE_CALLBACK_CANCEL;
   else
     return ECORE_CALLBACK_DONE;
}

void
_project_import_edj(Project_Process_Data *ppd)
{
   Eina_Stringshare *edj_in, *edj_out;
   Eina_List *l, *groups;
   Eina_Stringshare *group;
   Evas_Object *obj = NULL;
   Eina_Strbuf *strbuf;
   char buf[PATH_MAX];
   unsigned int count;

   //Eina_Stringshare *msg = eina_stringshare_printf(_("Start import '%s' file as new project"), ptd->edj);
   snprintf(buf, sizeof(buf), "Start import '%s' file as new project", ppd->edj);
   if (ppd->func_progress) ppd->func_progress(NULL, buf);
   //eina_stringshare_del(msg);

   /* Replace void with ptd */
   ppd->project = _project_create(ppd);

   assert(ppd->project != NULL);

   TODO("Add correct error handling here (if project == NULL). Probably we should add negative TC where directory already exist");
   ppd->project->pro_path = eina_stringshare_printf("%s/%s/%s.pro", ppd->path, ppd->name, ppd->name);

   if (!_lock_try(ppd->project->pro_path, true, &ppd->project->pro_fd))
     {
        /* really this case is unlickly, but we need handle it */
        ppd->result = PM_PROJECT_LOCKED;
        _end_send(ppd);
        return;
     }

   groups = edje_file_collection_list(ppd->edj);
   count = eina_list_count(groups);
   edje_edit_string_list_free(groups);
   if (ppd->widgets && (count != eina_list_count(ppd->widgets)))
     {
        //msg = eina_stringshare_printf(_("Merging groups from choosen file"));
        snprintf(buf, sizeof(buf), "Merging groups from choosen file");
        if (ppd->func_progress) ppd->func_progress(NULL, buf);
        //eina_stringshare_del(msg);

        eina_file_mkdtemp("eflete_build_XXXXXX", &ppd->tmp_dirname);
        edj_in = eina_stringshare_printf("%s/in.edj", ppd->tmp_dirname);
        edj_out = eina_stringshare_printf("%s/out.edj", ppd->tmp_dirname);
        ecore_file_cp(ppd->edj, edj_in);

        /* prepare the cmd string for run edje_pick */
        strbuf = eina_strbuf_new();
        eina_strbuf_append_printf(strbuf, "edje_pick -o %s -i %s", edj_out, edj_in);

        EINA_LIST_FOREACH(ppd->widgets, l, group)
          {
             if ((group[0] == 'c') && (group[1] == 'p') && (group[2] == '*') && (group[3] == '*') && (group[4] == '*'))
               {
                  char **arr = eina_str_split(group, "***", 0);
                  you_shall_not_pass_editor_signals(NULL);
                  /* load any group for coping */
                  if (!obj)
                    {
                       obj = edje_edit_object_add(evas_object_evas_get(ap.win));
                       if (!edje_object_file_set(obj, edj_in, arr[1]))
                         {
                            CRIT("Can't load object");
                            abort();
                         }
                    }
                  if (!editor_group_copy(obj, arr[1], arr[2], false))
                    {
                       CRIT("Can not copy group %s, to %s", arr[1], arr[2]);
                       abort();
                    }
                  you_shall_pass_editor_signals(NULL);
                  eina_strbuf_append_printf(strbuf, " -g %s", arr[2]);
                  free(arr[0]);
                  free(arr);
               }
             else
               eina_strbuf_append_printf(strbuf, " -g %s", group);
          }

        eina_stringshare_del(ppd->edj);
        ppd->edj = eina_stringshare_ref(edj_out);
        ppd->source_edj = eina_stringshare_ref(edj_in);

        ecore_exe_pipe_run(eina_strbuf_string_get(strbuf), FLAGS, NULL);
        eina_strbuf_free(strbuf);

        ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
        ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_error_handler, ppd);
        ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _edje_pick_finish_handler, ppd);
     }
   else
     {
        /* reset path variable for open project. _project_open_internal use this
         * var for path to .pro file */
        eina_stringshare_del(ppd->path);
        ppd->path = eina_stringshare_ref(ppd->project->pro_path);

        TODO("check result")
        _project_edj_file_copy(ppd);
        _copy_meta_data_to_pro(ppd);
        _project_special_group_add(ppd->project);
        _project_dummy_image_add(ppd->project);
        _project_dummy_sample_add(ppd->project);
        if (!_project_open_internal(ppd))
          {
             eina_stringshare_del(ppd->path);
             free(ppd);
             return;
          }
     }
}

void
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      Eina_List *list,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end ,
                      const void *data)
{
   Project_Process_Data *ppd;

   assert(name != NULL);
   assert(path != NULL);
   assert(edj != NULL);

   char *spath = eina_file_path_sanitize(path);
   char *sedj = eina_file_path_sanitize(edj);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->result = PM_PROJECT_LAST;
   ppd->name = eina_stringshare_add(name);
   ppd->path = eina_stringshare_add(spath);
   ppd->edj = eina_stringshare_add(sedj);
   ppd->widgets = list;

   _project_import_edj(ppd);

   free(spath);
   free(sedj);
}

static Eina_Bool
_finish_from_edje_cc(void *data,
                     int type __UNUSED__,
                     void *event_info)
{
   Project_Process_Data *ppd = data;
   Ecore_Exe_Event_Del *edje_cc_exit = (Ecore_Exe_Event_Del *)event_info;
   char buf[PATH_MAX];

   if (edje_cc_exit->exit_code != 0)
     {
        ppd->result = PM_PROJECT_ERROR;
        _end_send(ppd);
        return ECORE_CALLBACK_DONE;
     }

   if (ppd->func_progress)
     {
        snprintf(buf, sizeof(buf), "Data for importing prepare");
        if (ppd->func_progress) ppd->func_progress(NULL, buf);
     }

   ecore_event_handler_del(ppd->del_handler);
   ecore_event_handler_del(ppd->error_handler);
   ecore_event_handler_del(ppd->data_handler);

   _project_import_edj(ppd);

   return ECORE_CALLBACK_DONE;
}

void
_project_import_edc(void *data)
{
   Project_Process_Data *ppd = data;
   char buf[PATH_MAX];

   assert(ppd != NULL);

   snprintf(buf, sizeof(buf), "Start import '%s' file as new project", ppd->edc);
   if (ppd->func_progress) ppd->func_progress(NULL, buf);

   eina_file_mkdtemp("eflete_build_XXXXXX", &ppd->tmp_dirname);
   ppd->edj = eina_stringshare_printf("%s/out.edj", ppd->tmp_dirname);
   snprintf(buf, sizeof(buf),
            "edje_cc -v %s %s %s", ppd->edc, ppd->edj, ppd->build_options);
   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _finish_from_edje_cc, ppd);

   return;
}

void
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *import_options,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data)
{
   Project_Process_Data *ppd;

   assert(name != NULL);
   assert(path != NULL);
   assert(edc != NULL);

   char *spath = eina_file_path_sanitize(path);
   char *sedc = eina_file_path_sanitize(edc);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->result = PM_PROJECT_LAST;
   ppd->name = eina_stringshare_add(name);
   ppd->path = eina_stringshare_add(spath);
   ppd->edc = eina_stringshare_add(sedc);
   ppd->build_options = eina_stringshare_add(import_options);

   _project_import_edc(ppd);
   free(spath);
   free(sedc);
}

Eina_Bool
pm_project_close(Project *project)
{
   char buf[PATH_MAX];

   assert(project != NULL);

   snprintf(buf, sizeof(buf),
            "%s.backup", project->dev);
   ecore_file_unlink(buf);
   snprintf(buf, sizeof(buf),
            "%s/images", project->develop_path);
   ecore_file_recursive_rm(buf);
   snprintf(buf, sizeof(buf),
            "%s/sounds", project->develop_path);
   ecore_file_recursive_rm(buf);
   snprintf(buf, sizeof(buf),
            "%s/fonts", project->develop_path);
   ecore_file_recursive_rm(buf);

   evas_object_del(project->global_object);
   ecore_evas_free(project->ecore_evas);

   eina_file_close(project->mmap_file);
   ecore_file_unlink(project->dev);

   eina_stringshare_del(project->name);
   eina_stringshare_del(project->dev);
   eina_stringshare_del(project->develop_path);
   eina_stringshare_del(project->pro_path);

#ifdef HAVE_ENVENTOR
   if (enventor_object_project_unload(project))
     free(project->enventor);
#endif /* HAVE_ENVENTOR */

   eet_close(project->ef);
#ifdef _WIN32
   if (project->pro_fd != INVALID_HANDLE_VALUE)
     CloseHandle(project->pro_fd);
#else
   if (project->pro_fd != -1)
     close(project->pro_fd);
#endif
   resource_manager_shutdown(project);

   evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_CLOSED, NULL);
   free(project);

   return true;
}

static Eina_Bool
_copy_progress(void *data, unsigned long long done, unsigned long long total)
{
   Project_Process_Data * ppd = data;

   if (done == total)
     {
        ppd->result = PM_PROJECT_SUCCESS;
        _end_send(ppd);
     }
   return true;
}

void
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   assert(project != NULL);

   Project_Process_Data *ppd;
   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->project = project;
   if (!editor_save_all(ppd->project->global_object))
     {
        ERR("Failed to save project.");
        ppd->result = PM_PROJECT_ERROR;
        _end_send(ppd);
        return;
     }
   //ecore_file_cp(ppd->project->dev, ppd->project->saved_edj);
   eina_file_copy(ppd->project->dev, ppd->project->saved_edj,
                  EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR, _copy_progress, ppd);
}

void
pm_project_meta_data_get(Project *project,
                         Eina_Stringshare **name,
                         Eina_Stringshare **authors,
                         Eina_Stringshare **version,
                         Eina_Stringshare **license,
                         Eina_Stringshare **comment)
{
   char *tmp;

   assert(project != NULL);
   assert(project->ef != NULL);

#define DATA_READ(DATA, KEY) \
   if (DATA) \
     { \
        tmp = eet_read(project->ef, KEY, NULL); \
        *DATA = eina_stringshare_add(tmp); \
        free(tmp); \
     }

   DATA_READ(name,    PROJECT_KEY_NAME);
   DATA_READ(authors, PROJECT_KEY_AUTHORS);
   DATA_READ(version, PROJECT_KEY_FILE_VERSION);
   DATA_READ(license, PROJECT_KEY_LICENSE);
   DATA_READ(comment, PROJECT_KEY_COMMENT);

#undef DATA_READ
}

Eina_Bool
pm_project_meta_data_set(Project *project,
                         const char *name,
                         const char *authors,
                         const char *version,
                         const char *license,
                         const char *comment)
{
   int bytes, size;
   Eina_Bool res;

   assert(project != NULL);
   assert(project->ef != NULL);

   res = true;

#define DATA_WRITE(DATA, KEY) \
   if (DATA) \
     { \
        size = (strlen(DATA) + 1) * sizeof(char); \
        bytes = eet_write(project->ef, KEY, DATA, size, compess_level); \
        if (bytes <= 0 ) res = false; \
     }

   DATA_WRITE(name,    PROJECT_KEY_NAME);
   DATA_WRITE(authors, PROJECT_KEY_AUTHORS);
   DATA_WRITE(version, PROJECT_KEY_FILE_VERSION);
   DATA_WRITE(license, PROJECT_KEY_LICENSE);
   DATA_WRITE(comment, PROJECT_KEY_COMMENT);

#undef DATA_WRITE
   return res;
}

static Eina_Bool
_group_export_finish_handler(void *data,
                             int type __UNUSED__,
                             void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   ppd->result = PM_PROJECT_SUCCESS;
   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

void
pm_group_source_code_export(Project *project,
                            Group2 *group,
                            const char *path,
                            PM_Project_Progress_Cb func_progress,
                            PM_Project_End_Cb func_end,
                            const void *data)
{
   Project_Process_Data *ppd;
   char buf[PATH_MAX];

   assert(project != NULL);
   assert(group != NULL);
   assert(path != NULL);


   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->result = PM_PROJECT_LAST;

   snprintf(buf, sizeof(buf),
            "eflete_exporter --edj %s --path %s -g %s -s", project->saved_edj, path, group->common.name);

   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_error_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _group_export_finish_handler, ppd);
}

void
pm_project_source_code_export(Project *project,
                              const char *path,
                              PM_Project_Progress_Cb func_progress,
                              PM_Project_End_Cb func_end,
                              const void *data)
{
   Project_Process_Data *ppd;
   char buf[PATH_MAX];

   assert(project != NULL);
   assert(path != NULL);


   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->result = PM_PROJECT_LAST;

   snprintf(buf, sizeof(buf),
            "eflete_exporter --edj %s --path %s -s", project->saved_edj, path);

   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_error_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _group_export_finish_handler, ppd);

}

static Eina_Bool
_develop_export_finish_handler(void *data,
                               int type __UNUSED__,
                               void *event_info)
{
   Project_Process_Data *ppd = data;
   Ecore_Exe_Event_Del *edje_pick_exit = (Ecore_Exe_Event_Del *)event_info;
   char buf[PATH_MAX];

   ecore_event_handler_del(ppd->del_handler);
   ecore_event_handler_del(ppd->error_handler);
   ecore_event_handler_del(ppd->data_handler);

   if (edje_pick_exit->exit_code != 0)
     {
        ppd->result = PM_PROJECT_ERROR;
        _end_send(ppd);
        return ECORE_CALLBACK_DONE;
     }

   if (ppd->tmp_dirname)
     {
        ecore_file_recursive_rm(ppd->path);
        ecore_file_mv(ppd->tmp_dirname, ppd->path);
        eina_tmpstr_del(ppd->tmp_dirname);
        ppd->tmp_dirname = NULL;
     }
   snprintf(buf, sizeof(buf), _("Export to file '%s' successfull"), ppd->path);
   if (ppd->func_progress) ppd->func_progress(NULL, buf);

  ppd->result = PM_PROJECT_SUCCESS;
  _end_send(ppd);

  return ECORE_CALLBACK_DONE;
}

void
pm_project_develop_export(Project *project,
                          const char *path,
                          Eina_List *groups,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   Project_Process_Data *ppd;
   Eina_Strbuf *cmd;
   Eina_List *l;
   Group2 *group;

   assert(project != NULL);
   assert(path != NULL);
   assert(groups != NULL);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(path);
   ppd->project = project;
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->result = PM_PROJECT_LAST;

   CRIT_ON_FAIL(editor_save_all(project->global_object));

   cmd = eina_strbuf_new();
   if (!ecore_file_exists(ppd->path))
     eina_strbuf_append_printf(cmd, "edje_pick -o %s", path);
   else
     {
        eina_file_mkstemp("eflete_export_XXXXXX", &ppd->tmp_dirname);
        eina_strbuf_append_printf(cmd, "edje_pick -o %s", ppd->tmp_dirname);
        eina_strbuf_append_printf(cmd, " -a %s", path);
     }
   eina_strbuf_append_printf(cmd, " -i %s", project->dev);

   EINA_LIST_FOREACH(groups, l, group)
     eina_strbuf_append_printf(cmd, " -g %s", group->common.name);

   DBG("Run command for export: %s", eina_strbuf_string_get(cmd));
   ecore_exe_pipe_run(eina_strbuf_string_get(cmd), FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_error_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _develop_export_finish_handler, ppd);

   return;
}

static Eina_Bool
_release_export_build_finish_handler(void *data,
                                     int type __UNUSED__,
                                     void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   ppd->result = PM_PROJECT_SUCCESS;
   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_release_export_finish_handler(void *data,
                             int type __UNUSED__,
                             void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;
   Eina_Strbuf *buf;
   char folder[PATH_MAX];

   ecore_event_handler_del(ppd->del_handler);
   ecore_event_handler_del(ppd->error_handler);
   ecore_event_handler_del(ppd->data_handler);

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "edje_cc -v %s %s", ppd->edc, ppd->edj);
   snprintf(folder, sizeof(folder), "%s/images", ppd->tmp_dirname);
   if (ecore_file_exists(folder))
     eina_strbuf_append_printf(buf, " -id %s", folder);
   snprintf(folder, sizeof(folder), "%s/sounds", ppd->tmp_dirname);
   if (ecore_file_exists(folder))
     eina_strbuf_append_printf(buf, " -sd %s", folder);
   snprintf(folder, sizeof(folder), "%s/fonts", ppd->tmp_dirname);
   if (ecore_file_exists(folder))
     eina_strbuf_append_printf(buf, " -fd %s", folder);
   ecore_exe_pipe_run(eina_strbuf_string_get(buf), FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _release_export_build_finish_handler, ppd);

   eina_strbuf_free(buf);
   return ECORE_CALLBACK_DONE;
}

void
pm_project_release_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   Project_Process_Data *ppd;
   char buf[PATH_MAX];

   assert(project != NULL);
   assert(path != NULL);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(path);
   ppd->project = project;
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->edj = eina_stringshare_add(path);
   ppd->data = (void *)data;
   ppd->result = PM_PROJECT_LAST;

   CRIT_ON_FAIL(editor_save_all(project->global_object));

   eina_file_mkdtemp("eflete_export_XXXXXX", &ppd->tmp_dirname);
   snprintf(buf, sizeof(buf),
            "eflete_exporter --edj %s --path %s -s", ppd->project->saved_edj, ppd->tmp_dirname);
   ppd->edc = eina_stringshare_printf("%s/generated.edc", ppd->tmp_dirname);
   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_error_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _release_export_finish_handler, ppd);
}

Eina_Bool
pm_lock_check(const char *path)
{
   return _lock_try(path, false, NULL);
}
