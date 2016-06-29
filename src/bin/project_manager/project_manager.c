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
#define _GNU_SOURCE
#include "project_manager.h"
#include <fcntl.h>
#ifndef _WIN32
#include <sys/wait.h>
#else
#include <win32.h>
#endif
#define PROJECT_FILE_KEY      "project"

#include "change.h"

#define PROJECT_KEY_NAME         "edje/name"
#define PROJECT_KEY_AUTHORS      "edje/authors"
#define PROJECT_KEY_FILE_VERSION "edje/file_version"
#define PROJECT_KEY_LICENSE      "edje/license"
#define PROJECT_KEY_COMMENT      "edje/comment"

static Eet_Compression compess_level = EET_COMPRESSION_HI;

typedef struct
{
   Eina_Stringshare *str;
   PM_Project_Progress_Cb func_print;
   void *data;
} Progress_Message;

typedef struct
{
   Eina_Stringshare *cmd;
   Ecore_Exe *exe_cmd;
   pid_t exe_pid;
   Ecore_Exe_Flags flags;
} Edje_CC_Data;

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
   /** The checked widgets. Used for loading just checked widgets. */
   Eina_List *widgets;
} Project_Thread;

static Project_Thread worker;
#define WORKER_CREATE(FUNC_PROGRESS, FUNC_END, DATA, PROJECT, \
                      NAME, PATH, EDJ, EDC, BUILD_OPTIONS, WIDGET_LIST) \
{ \
   worker.func_progress = FUNC_PROGRESS; \
   worker.func_end = FUNC_END; \
   worker.data = (void *)DATA; \
   worker.project = PROJECT; \
   worker.result = PM_PROJECT_LAST; \
   worker.name = eina_stringshare_add(NAME); \
   worker.path = eina_stringshare_add(PATH); \
   worker.edj = eina_stringshare_add(EDJ); \
   worker.edc = eina_stringshare_add(EDC); \
   worker.build_options = eina_stringshare_add(BUILD_OPTIONS); \
   worker.widgets = WIDGET_LIST; \
}

#define WORKER_FREE() \
{ \
   eina_stringshare_del(worker.name); \
   eina_stringshare_del(worker.path); \
   eina_stringshare_del(worker.edj); \
   eina_stringshare_del(worker.edc); \
   eina_stringshare_del(worker.build_options); \
}

#define THREAD_TESTCANCEL pthread_testcancel()

#define PROGRESS_SEND(FMT, ...) \
{ \
   if (worker.func_progress) \
      { \
         Progress_Message *message = mem_malloc(sizeof(Progress_Message)); \
         message->str = eina_stringshare_printf(FMT, ## __VA_ARGS__); \
         message->func_print = worker.func_progress; \
         message->data = worker.data; \
         ecore_main_loop_thread_safe_call_async(_progress_send, message); \
      } \
}

#define END_SEND(STATUS) \
{ \
   if (worker.func_end) \
     { \
        worker.result = STATUS; \
        ecore_main_loop_thread_safe_call_async(_end_send, NULL); \
     } \
}

static Eina_Bool
_image_resources_load(Project *project);

static Eina_Bool
_sound_resources_load(Project *project);

static Eina_Bool
_font_resources_load(Project *project);

static void
_tones_resources_load(Project *project);

static void
_colorclasses_resources_load(Project *project);

static void
_styles_resources_load(Project *project);

static Eina_Bool
_project_dev_file_create(Project *pro)
{
   Eina_Bool result;

   result = ecore_file_cp(pro->saved_edj, pro->dev);
   DBG("Create the .dev file.");
   return result;
}

static Eet_Data_Descriptor *eed_project = NULL;

static Eina_Bool
_lock_try(const char *path, Eina_Bool check)
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
   /* if flag check is false not need to lock the file */
   if (check)
     {
        savelock.l_pid = getpid();
        fcntl(fd, F_SETLK, &savelock);
     }

   return true;
}

static void
_project_descriptor_init(void)
{
   Eet_Data_Descriptor_Class eddc;

   if (eed_project) return;

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc),
                                             "Project", sizeof(Project));
   eed_project = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "version", version, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "dev", dev, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "saved_edj", saved_edj, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "develop_path", develop_path, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "release_options", release_options, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING(eed_project, Project, "images", res.images);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING(eed_project, Project, "sounds", res.sounds);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING(eed_project, Project, "fonts", res.fonts);
}

static void
_pm_project_descriptor_shutdown(void)
{
   eet_data_descriptor_free(eed_project);
   eed_project = NULL;
}

static void
_progress_send(void *data)
{
   Progress_Message *message;

   message = (Progress_Message *)data;
   assert(message != NULL);

   message->func_print(message->data, message->str);
   eina_stringshare_del(message->str);
   free(message);
}

static void
_end_send(void *data __UNUSED__)
{
   PM_Project_End_Cb func;
   PM_Project_Result result;
   void *udata;

   /** Copy the links to callback and meesage, to fast release worker resource. */
   worker.func_progress = NULL;
   func = worker.func_end;
   result = worker.result;
   udata = worker.data;
   func(udata, result);
}

static Eina_Bool
_build_script_write(const char *path)
{
   FILE *f;
   Eina_Bool res = true;
   Eina_Strbuf *buf;

   f = fopen(path, "w");
   if (!f)
     {
        ERR("Could't open file '%s'", path);
        res = false;
        goto exit;
     }

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "#!/bin/sh\n");
   eina_strbuf_append_printf(buf, "edje_cc -v generated.edc -id images/ -sd sounds/ -fd fonts/");
   fputs(eina_strbuf_string_get(buf), f);
   eina_strbuf_free(buf);
   chmod(path, S_IRWXU|S_IRWXG|S_IROTH|S_IWOTH);

exit:
   fclose(f);
   return res;
}

#define MKDIR(NAME) \
   tmp = eina_stringshare_printf("%s/"#NAME, pro->develop_path); \
   ecore_file_mkdir(tmp); \
   pro->res.NAME = eina_list_append(pro->res.NAME, eina_stringshare_add(tmp)); \
   eina_stringshare_del(tmp)

static Project *
_project_files_create(void)
{
   Project *pro;
   Eina_Stringshare *folder_path, *pro_path, *tmp;
   Eina_Bool error = false;

   _project_descriptor_init();

   folder_path = eina_stringshare_printf("%s/%s", worker.path, worker.name);
   if (ecore_file_mkdir(folder_path))
     {
        DBG("Create the folder '%s' for new project '%s'", folder_path, worker.name);
     }
   else
     {
        ERR("Could't create a project folder!");
        error = true;
     }
   eina_stringshare_del(folder_path);
   if (error) return NULL;

   THREAD_TESTCANCEL;
   pro = (Project *)mem_calloc(1, sizeof(Project));
   folder_path = eina_stringshare_printf("%s/%s", worker.path, worker.name);
   pro->version = PROJECT_FILE_VERSION;
   pro->name = eina_stringshare_add(worker.name);
   pro->dev = eina_stringshare_printf("%s/%s.dev", folder_path, worker.name);
   pro->saved_edj = eina_stringshare_printf("%s/%s.edj", folder_path, worker.name);
   pro->develop_path = eina_stringshare_printf("%s/develop", folder_path);

   pro_path = eina_stringshare_printf("%s/%s.pro", folder_path, worker.name);
   pro->ef = eet_open(pro_path, EET_FILE_MODE_READ_WRITE);
   ecore_file_mkdir(pro->develop_path);
   MKDIR(images);
   MKDIR(sounds);
   MKDIR(fonts);
   eina_stringshare_del(folder_path);

   if (!eet_data_write(pro->ef, eed_project, PROJECT_FILE_KEY, pro, compess_level))
     error = true;

   DBG("Create a specific project file '%s': %s", pro_path, error ? "failsed" : "success");
   THREAD_TESTCANCEL;
   _pm_project_descriptor_shutdown();
   eina_stringshare_del(pro_path);
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
   else
     eet_sync(pro->ef);

   return pro;
}

#undef MKDIR

static void
_copy_meta_data_to_pro(void)
{
   Eet_File *ef;
   char *name, *authors, *version, *license, *comment;

   ef = eet_open(worker.edj, EET_FILE_MODE_READ_WRITE);

   name = strdup(worker.name);
   authors = eet_read(ef, PROJECT_KEY_AUTHORS, NULL);
   version = eet_read(ef, PROJECT_KEY_FILE_VERSION, NULL);
   license = eet_read(ef, PROJECT_KEY_LICENSE, NULL);
   comment = eet_read(ef, PROJECT_KEY_COMMENT, NULL);
   eet_close(ef);

   pm_project_meta_data_set(worker.project, name, authors,
                            version, license, comment);

   if (name) free(name);
   if (authors) free(authors);
   if (version) free(version);
   if (license) free(license);
   if (comment) free(comment);
}

static Eina_Bool
_project_edj_file_copy(void)
{
   Eina_Stringshare *src, *dst;
   Eina_Bool result;

   src = eina_stringshare_ref(worker.edj);
   dst = eina_stringshare_ref(worker.project->saved_edj);
   result = ecore_file_cp(src, dst);

   DBG("Copy the .edj file to project folder.");
   eina_stringshare_del(src);
   eina_stringshare_del(dst);
   return result;
}

static void
_project_open_internal(Project *project)
{
   assert(project != NULL);

   _project_dev_file_create(project);
   project->mmap_file = eina_file_open(project->dev, false);

   project->changed = false;
   project->close_request = false;
   project->ecore_evas = ecore_evas_buffer_new(0, 0);
   project->global_object = edje_edit_object_add(ecore_evas_get(project->ecore_evas));
   edje_object_mmap_set(project->global_object, project->mmap_file, EFLETE_INTERNAL_GROUP_NAME);

   _image_resources_load(project);
   _sound_resources_load(project);
   _font_resources_load(project);
   _tones_resources_load(project);
   _colorclasses_resources_load(project);
   _styles_resources_load(project);

   gm_groups_load(project);

   edje_file_cache_flush();
}

static void
_project_special_group_add(Project *project)
{
   Evas *e;
   Evas_Object *edje_edit_obj;
   Eina_List *list;

   assert(project != NULL);

   ecore_thread_main_loop_begin();

   Ecore_Evas *ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   list = edje_file_collection_list(project->saved_edj);
   edje_edit_obj = edje_edit_object_add(e);

   edje_object_file_set(edje_edit_obj, project->saved_edj, eina_list_data_get(list));
   CRIT_ON_FAIL(editor_internal_group_add(edje_edit_obj));

   edje_edit_string_list_free(list);
   evas_object_del(edje_edit_obj);
   ecore_evas_free(project->ecore_evas);

   ecore_thread_main_loop_end();
}

static void
_project_dummy_image_add(Project *project)
{
   Evas *e;
   Evas_Object *edje_edit_obj;
   char buf[PATH_MAX];

   assert(project != NULL);

   ecore_thread_main_loop_begin();

   Ecore_Evas *ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   edje_edit_obj = edje_edit_object_add(e);

   edje_object_file_set(edje_edit_obj, project->saved_edj, EFLETE_INTERNAL_GROUP_NAME);
   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_IMAGE_NAME, ap.path.image_path);
   edje_edit_image_add(edje_edit_obj, buf);

   evas_object_del(edje_edit_obj);
   ecore_evas_free(project->ecore_evas);

   ecore_thread_main_loop_end();
}

static void *
_project_import_edj(void *data,
                    Eina_Thread *thread __UNUSED__)
{
   Eina_Bool send_end = (data) ? (*(Eina_Bool *)data) : true;

   PROGRESS_SEND(_("Start import '%s' file as new project"), worker.edj);
   PROGRESS_SEND(_("Creating a specifiec file and folders"));
   worker.project = _project_files_create();
   TODO("Add correct error handling here (if project == NULL). Probably we should add negative TC where directory already exist");
   THREAD_TESTCANCEL;
   worker.project->pro_path = eina_stringshare_printf("%s/%s/%s.pro", worker.path, worker.name, worker.name);
   THREAD_TESTCANCEL;

   if (!_lock_try(worker.project->pro_path, true))
     {
        /* really this case is unlickly, but we need handle it */
        END_SEND(PM_PROJECT_LOCKED);
        return NULL;
     }

   PROGRESS_SEND(_("Import processing"));
   _project_edj_file_copy();
   _copy_meta_data_to_pro();
   _project_special_group_add(worker.project);
   _project_dummy_image_add(worker.project);
   _project_open_internal(worker.project);
   THREAD_TESTCANCEL;

   PROGRESS_SEND(_("Import finished. Project '%s' created"), worker.project->name);
   if (send_end) END_SEND(PM_PROJECT_SUCCESS);

   return NULL;
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
   assert(name != NULL);
   assert(path != NULL);
   assert(edj != NULL);

   WORKER_CREATE(func_progress, func_end, data, NULL,
                 name, path, edj, NULL, NULL, list);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_project_import_edj, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

static Eina_Bool
_exe_data(void *data __UNUSED__,
          int type __UNUSED__,
          void *event)
{
   int i;
   Ecore_Exe_Event_Data *ev = event;

   if (ev->lines)
     {
        for (i = 0; ev->lines[i].line; i++)
          {
             DBG("%s", ev->lines[i].line);
             PROGRESS_SEND("%s", ev->lines[i].line);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static void *
_ecore_exe_edje_cc(void *data)
{
   Edje_CC_Data *edje_cc_data = (Edje_CC_Data *)data;

   edje_cc_data->exe_cmd = ecore_exe_pipe_run(edje_cc_data->cmd, edje_cc_data->flags, NULL);
   edje_cc_data->exe_pid = ecore_exe_pid_get(edje_cc_data->exe_cmd);

   return NULL;
}

static void *
_project_import_edc(void *data,
                    Eina_Thread *thread __UNUSED__)
{
   Eina_Bool send_end_child;
   Eina_Bool send_end = (data) ? (*(Eina_Bool *)data) : true;

   Eina_Tmpstr *tmp_dirname;
   Ecore_Event_Handler *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   int edje_cc_res = 0, waitpid_res = 0;

   PROGRESS_SEND(_("Start import '%s' file as new project"), worker.edc);
   PROGRESS_SEND(_("Creating a specifiec file and folders"));
   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, NULL);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, NULL);
     }
   eina_file_mkdtemp("eflete_build_XXXXXX", &tmp_dirname);
   worker.edj = eina_stringshare_printf("%s/out.edj", tmp_dirname);
   Edje_CC_Data *edje_cc_data = mem_malloc(sizeof(Edje_CC_Data));
   edje_cc_data->cmd = eina_stringshare_printf("edje_cc -v %s %s %s",
                                               worker.edc,
                                               worker.edj,
                                               worker.build_options);
   edje_cc_data->flags  = ECORE_EXE_PIPE_READ |
      ECORE_EXE_PIPE_READ_LINE_BUFFERED |
      ECORE_EXE_PIPE_ERROR |
      ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   THREAD_TESTCANCEL;
   ecore_main_loop_thread_safe_call_sync(_ecore_exe_edje_cc, edje_cc_data);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(edje_cc_data->exe_pid, &edje_cc_res, 0);
   eina_stringshare_del(edje_cc_data->cmd);
   free(edje_cc_data);
   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_cc_res) && (WEXITSTATUS(edje_cc_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   send_end_child = false;
   _project_import_edj(&send_end_child, NULL);

   PROGRESS_SEND(_("Removing temporary files..."));
   ecore_file_recursive_rm(tmp_dirname);
   eina_tmpstr_del(tmp_dirname);
   PROGRESS_SEND(_("Done."));

   if (send_end) END_SEND(PM_PROJECT_SUCCESS)

   return NULL;
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
   assert(name != NULL);
   assert(path != NULL);
   assert(edc != NULL);

   WORKER_CREATE(func_progress, func_end, data, NULL,
                 name, path, NULL, edc, import_options, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_project_import_edc, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

Eina_Bool
pm_project_thread_cancel()
{
   int ret;

   ret = pthread_cancel((pthread_t)worker.thread);
   if (ret)
     return false;

   END_SEND(PM_PROJECT_CANCEL);
   DBG("Project Thread stoped by user!");
   return true;
}

Eina_Bool
pm_project_thread_free()
{
   WORKER_FREE();
   return true;
}

PM_Project_Result
pm_project_thread_result_get(void)
{
   return worker.result;
}

Project *
pm_project_thread_project_get()
{
   return worker.project;
}

void *
_project_open(void *data,
              Eina_Thread *t __UNUSED__)
{
   Eet_File *ef;
   char *tmp;
   int tmp_len;
   Eina_Stringshare *path = data;

   assert(path != NULL);

   edje_file_cache_flush();

   if (!_lock_try(path, true))
     {
        /* really this case is unlickly, but we need handle it */
        END_SEND(PM_PROJECT_LOCKED);
        return NULL;
     }

   PROGRESS_SEND(_("Opening project \"%s\""), path);

   _project_descriptor_init();
   ef = eet_open(path, EET_FILE_MODE_READ_WRITE);
   if (!ef)
     {
        eina_stringshare_del(path);
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   PROGRESS_SEND(_("Reading project descriptor"));

   worker.project = eet_data_read(ef, eed_project, PROJECT_FILE_KEY);
   _pm_project_descriptor_shutdown();
   if (!worker.project)
     {
        eina_stringshare_del(path);
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   worker.project->ef = ef;
   worker.project->pro_path = eina_stringshare_add(path);

   /* updating .dev file path */
   tmp = strdup(path);
   tmp_len = strlen(tmp);
   tmp[tmp_len - 3] = 'd';
   tmp[tmp_len - 2] = 'e';
   tmp[tmp_len - 1] = 'v';
   eina_stringshare_replace(&worker.project->dev, tmp);
   free(tmp);
   /* updating .edj file path */
   tmp = strdup(path);
   tmp[tmp_len - 3] = 'e';
   tmp[tmp_len - 2] = 'd';
   tmp[tmp_len - 1] = 'j';
   eina_stringshare_replace(&worker.project->saved_edj, tmp);
   free(tmp);
   eina_stringshare_del(path);

   PROGRESS_SEND(_("Checking project version"));
   /* checking for older project versions and upgrading them version-by-version */
   if (worker.project->version < 2) /* upgrade to version 2 */
     {
        PROGRESS_SEND(_("Updating project files to version 2"));
        ecore_file_mv(worker.project->dev, worker.project->saved_edj);
        worker.project->version = 2;
     }
   if (worker.project->version < 3) /* upgrade to version 3 */
     {
        PROGRESS_SEND(_("Updating project files to version 3"));
        _project_special_group_add(worker.project);
        worker.project->version = 3;
     }
   if (worker.project->version < 4) /* upgrade to version 4 */
     {
        PROGRESS_SEND(_("Updating project files to version 4"));
        _project_dummy_image_add(worker.project);
        worker.project->version = 4;
     }
   TODO("Add crash recovery prompt here")

   pm_project_meta_data_get(worker.project, &worker.project->name, NULL, NULL, NULL, NULL);
   if (!worker.project->name)
     worker.project->name = ecore_file_strip_ext(ecore_file_file_get(worker.project->dev));

   _project_open_internal(worker.project);

   PROGRESS_SEND(_("Project is open"));
   END_SEND(PM_PROJECT_SUCCESS);

   return NULL;
}

void
pm_project_open(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   assert(path != NULL);

   WORKER_CREATE(func_progress, func_end, data, NULL,
                 NULL, NULL, NULL, NULL, NULL, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_project_open, eina_stringshare_add(path)))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

void
pm_dev_file_reload(Project *pr)
{
   eina_file_close(pr->mmap_file);
   pr->mmap_file = eina_file_open(pr->dev, false);
   edje_object_mmap_set(pr->global_object, pr->mmap_file, EFLETE_INTERNAL_GROUP_NAME);
}

static void *
_project_save(void *data __UNUSED__,
              Eina_Thread *thread __UNUSED__)
{
   ecore_thread_main_loop_begin();
   PROGRESS_SEND(_("Save project '%s'"), worker.project->name);
   CRIT_ON_FAIL(editor_save_all(worker.project->global_object));

   ecore_file_cp(worker.project->dev, worker.project->saved_edj);

   PROGRESS_SEND("Save done");

   ecore_thread_main_loop_end();

   END_SEND(PM_PROJECT_SUCCESS);
   return NULL;
}

void
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   assert(project != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, NULL, NULL, NULL, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_project_save, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

Eina_Bool
pm_project_style_save_as(Project *project __UNUSED__, const char *file __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_build(Project *project __UNUSED__, Build build_profile __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_close(Project *project)
{
   Eina_Stringshare *backup, *data;
   Eina_Stringshare *resource_folder;
   External_Resource *image, *sound, *font;
   Tone_Resource *tone;
   Colorclass_Resource *cc;
   Resource *style;

   assert(project != NULL);

   backup = eina_stringshare_printf("%s.backup", project->dev);
   ecore_file_remove(backup);
   eina_stringshare_del(backup);

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);
   ecore_file_recursive_rm(resource_folder);
   eina_stringshare_del(resource_folder);
   resource_folder = eina_stringshare_printf("%s/fonts", project->develop_path);
   ecore_file_recursive_rm(resource_folder);
   eina_stringshare_del(resource_folder);
   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);
   ecore_file_recursive_rm(resource_folder);
   eina_stringshare_del(resource_folder);

   evas_object_del(project->global_object);
   ecore_evas_free(project->ecore_evas);

   eina_file_close(project->mmap_file);
   ecore_file_unlink(project->dev);

   eina_stringshare_del(project->name);
   eina_stringshare_del(project->dev);
   eina_stringshare_del(project->develop_path);
   eina_stringshare_del(project->pro_path);

   gm_groups_free(project);
   EINA_LIST_FREE(project->images, image)
     {
        eina_stringshare_del(image->name);
        eina_list_free(image->used_in);
        eina_stringshare_del(image->source);
        free(image);
     }
   EINA_LIST_FREE(project->sounds, sound)
     {
        eina_stringshare_del(sound->name);
        eina_list_free(sound->used_in);
        eina_stringshare_del(sound->source);
        free(sound);
     }
   EINA_LIST_FREE(project->fonts, font)
     {
        eina_stringshare_del(font->name);
        eina_list_free(font->used_in);
        eina_stringshare_del(font->source);
        free(font);
     }
   EINA_LIST_FREE(project->tones, tone)
     {
        eina_stringshare_del(tone->name);
        eina_list_free(tone->used_in);
        free(tone);
     }
   EINA_LIST_FREE(project->colorclasses, cc)
     {
        eina_stringshare_del(cc->name);
        eina_list_free(cc->used_in);
        free(cc);
     }
   EINA_LIST_FREE(project->styles, style)
     {
        eina_stringshare_del(style->name);
        eina_list_free(style->used_in);
        free(style);
     }

   EINA_LIST_FREE(project->res.images, data)
      eina_stringshare_del(data);
   EINA_LIST_FREE(project->res.sounds, data)
      eina_stringshare_del(data);
   EINA_LIST_FREE(project->res.fonts, data)
      eina_stringshare_del(data);

#ifdef HAVE_ENVENTOR
   if (enventor_object_project_unload(project))
     free(project->enventor);
#endif /* HAVE_ENVENTOR */

   eet_close(project->ef);
   free(project);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_CLOSED, NULL);

   return true;
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
_image_resources_load(Project *project)
{
   External_Resource *res;
   Eina_List *images;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *image_name, *source_file;
   Eina_List *l;
   Evas *e;
   Evas_Object *im;
   int id;
   char *file_dir;
   int im_total, im_proc;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export images", resource_folder);
        eina_stringshare_del(resource_folder);
        return false;
     }

   images = edje_edit_images_list_get(project->global_object);

   e = ecore_evas_get(project->ecore_evas);
   im_total = eina_list_count(images);
   im_proc = 0;
   Edje_Edit_Image_Comp comp_type;
   PROGRESS_SEND(_("Start image processing, total %d:"), im_total);
   EINA_LIST_FOREACH(images, l, image_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!image_name) continue;

        im_proc++;
        PROGRESS_SEND(_("image processing (%d/%d): %s"),
                      im_proc, im_total, image_name);

        res = (External_Resource *) resource_add(image_name, RESOURCE_TYPE_IMAGE);
        comp_type = edje_edit_image_compression_type_get(project->global_object,
                                                         res->name);
        if (comp_type == EDJE_EDIT_IMAGE_COMP_USER)
          res->source = eina_stringshare_add(image_name);
        else
          res->source = eina_stringshare_printf("%s/%s", resource_folder, image_name);
        resource_insert(&project->images, (Resource *)res);

        if (!ecore_file_exists(res->source))
          {
             file_dir = ecore_file_dir_get(res->source);
             ecore_file_mkpath(file_dir);
             free(file_dir);
             ecore_thread_main_loop_begin();
             im = evas_object_image_add(e);
             id = edje_edit_image_id_get(project->global_object, image_name);
             if (id < 0)
               {
                  WARN("Image %s coudn't be exported", image_name);
                  ecore_thread_main_loop_end();
                  continue;
               }
             source_file = eina_stringshare_printf("edje/images/%i", id);
             evas_object_image_file_set(im, project->dev, source_file);
             ecore_thread_main_loop_end();
             evas_object_image_save(im, res->source, NULL, NULL);
             ecore_thread_main_loop_begin();
             evas_object_del(im);
             eina_stringshare_del(source_file);
             ecore_thread_main_loop_end();
          }
     }

   edje_edit_string_list_free(images);
   eina_stringshare_del(resource_folder);
   return true;
}

static Eina_Bool
_sound_resources_load(Project *project)
{
   External_Resource *res;
   Eina_List *sounds;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *sound_name, *sound_file;
   Eina_List *l;
   Eina_Binbuf *sound_bin;
   FILE *f;
   char *file_dir;
   int snd_total, snd_proc;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export sounds", resource_folder);
        eina_stringshare_del(resource_folder);
        return false;
     }

   sounds = edje_edit_sound_samples_list_get(project->global_object);

   snd_total = eina_list_count(sounds);
   snd_proc = 0;
   PROGRESS_SEND(_("Start sound processing: total %d:"), snd_total);
   EINA_LIST_FOREACH(sounds, l, sound_name)
     {
        sound_file = edje_edit_sound_samplesource_get(project->global_object, sound_name);
        snd_proc++;
        PROGRESS_SEND(_("sound processing (%d/%d): %s"),
                      snd_proc, snd_total, sound_file);

        res = (External_Resource*)resource_add(sound_name, RESOURCE_TYPE_SOUND);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, sound_file);
        resource_insert(&project->sounds, (Resource *)res);

        if (!ecore_file_exists(res->source))
          {
             file_dir = ecore_file_dir_get(res->source);
             ecore_file_mkpath(file_dir);
             free(file_dir);
             sound_bin = edje_edit_sound_samplebuffer_get(project->global_object, sound_name);
             if (!(f = fopen(res->source, "wb")))
               {
                  ERR("Could not open file: %s", res->source);
                  continue;
               }
             if (fwrite(eina_binbuf_string_get(sound_bin),
                        eina_binbuf_length_get(sound_bin), 1, f) != 1)
               ERR("Could not write sound: %s", strerror(errno));
             if (f) fclose(f);
             eina_binbuf_free(sound_bin);
          }
        edje_edit_string_free(sound_file);
     }

   edje_edit_string_list_free(sounds);
   eina_stringshare_del(resource_folder);
   return true;
}

static Eina_Bool
_font_resources_load(Project *project)
{
   External_Resource *res;
   Eina_List *fonts;
   Eina_Stringshare *resource_folder;
   Eet_File *ef;
   Eina_List *l;
   Eina_Stringshare *font_name, *font_file;
   void *font;
   FILE *f;
   int size, fnt_total, fnt_proc;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/fonts", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export fonts", resource_folder);
   eina_stringshare_del(resource_folder);
        return false;
     }

   fonts = edje_edit_fonts_list_get(project->global_object);

   ef = eet_open(project->dev, EET_FILE_MODE_READ);
   fnt_total = eina_list_count(fonts);
   fnt_proc = 0;
   PROGRESS_SEND(_("Start font processing, total %d:"), fnt_total);
   EINA_LIST_FOREACH(fonts, l, font_name)
     {
        font_file = edje_edit_font_path_get(project->global_object, font_name);
        fnt_proc++;
        PROGRESS_SEND(_("font processing (%d/%d): %s"),
                      fnt_proc, fnt_total, font_file);

        res = (External_Resource *)resource_add(font_file, RESOURCE_TYPE_FONT);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, font_file);
        resource_insert(&project->fonts, (Resource *)res);

        if (!ecore_file_exists(res->source))
          {
             edje_edit_string_free(font_file);
             font_file = eina_stringshare_printf("edje/fonts/%s", font_name);
             font = eet_read(ef, font_file, &size);
             if (!font) continue;
             if (!(f = fopen(res->source, "wb")))
               {
                  ERR("Could not open file: %s", res->source);
                  continue;
               }
             if (fwrite(font, size, 1, f) != 1)
               ERR("Could not write font: %s", strerror(errno));
             if (f) fclose(f);
             free(font);
             eina_stringshare_del(font_file);
          }
     }
   eet_close(ef);
   edje_edit_string_list_free(fonts);
   eina_stringshare_del(resource_folder);
   return true;
}

static void
_tones_resources_load(Project *project)
{
   Eina_List *tones, *l;
   Tone_Resource *res;
   Eina_Stringshare *name;
   int tones_total, tones_proc = 0;

   assert(project != NULL);

   tones = edje_edit_sound_tones_list_get(project->global_object);
   tones_total = eina_list_count(tones);

   PROGRESS_SEND(_("Start tone processing, total %d:"), tones_total);
   EINA_LIST_FOREACH(tones, l, name)
     {
        PROGRESS_SEND(_("tone processing (%d/%d): %s"),
                      ++tones_proc, tones_total, name);

        res = (Tone_Resource *)resource_add(name, RESOURCE_TYPE_TONE);
        res->freq = edje_edit_sound_tone_frequency_get(project->global_object, name);
        resource_insert(&project->tones, (Resource *)res);
     }

   edje_edit_string_list_free(tones);
}

static void
_colorclasses_resources_load(Project *project)
{
   Eina_List *colorclasses, *l;
   Colorclass_Resource *res;
   Eina_Stringshare *name;
   int cc_total, cc_proc = 0;

   assert(project != NULL);

   colorclasses = edje_edit_color_classes_list_get(project->global_object);
   cc_total = eina_list_count(colorclasses);

   PROGRESS_SEND(_("Start colorclass processing, total %d:"), cc_total);
   EINA_LIST_FOREACH(colorclasses, l, name)
     {
        PROGRESS_SEND(_("colorclass processing (%d/%d): %s"),
                      ++cc_proc, cc_total, name);

        res = (Colorclass_Resource *)resource_add(name, RESOURCE_TYPE_COLORCLASS);

        if (!edje_edit_color_class_colors_get(project->global_object, name,
                                              &res->color1.r, &res->color1.g, &res->color1.b, &res->color1.a,
                                              &res->color2.r, &res->color2.g, &res->color2.b, &res->color2.a,
                                              &res->color3.r, &res->color3.g, &res->color3.b, &res->color3.a))
          {
             eina_stringshare_del(res->name);
             resource_free((Resource *)res);
          }
        else
          resource_insert(&project->colorclasses, (Resource *)res);
     }

   edje_edit_string_list_free(colorclasses);
}

static void
_styles_resources_load(Project *project)
{
   Eina_List *styles, *l;
   Resource *res;
   Eina_Stringshare *name;
   int styles_total, styles_proc = 0;

   assert(project != NULL);

   styles = edje_edit_styles_list_get(project->global_object);
   styles_total = eina_list_count(styles);

   PROGRESS_SEND(_("Start style processing, total %d:"), styles_total);
   EINA_LIST_FOREACH(styles, l, name)
     {
        PROGRESS_SEND(_("style processing (%d/%d): %s"),
                      ++styles_proc, styles_total, name);

        res = resource_add(name, RESOURCE_TYPE_STYLE);
        resource_insert(&project->styles, res);
     }

   edje_edit_string_list_free(styles);
}

Eina_Bool
pm_project_resource_export(Project *pro __UNUSED__, const char* dir_path __UNUSED__)
{
   return false;
}

static void
_external_resources_export(Eina_List *resources, const char *dst)
{
   Eina_Strbuf *buf;
   Eina_List *l;
   External_Resource *res;
   char *path;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(resources, l, res)
     {
        eina_strbuf_append_printf(buf, "%s/%s", dst, res->name);
        path = ecore_file_dir_get(eina_strbuf_string_get(buf));
        if (!ecore_file_is_dir(path))
          {
             ecore_file_mkpath(path);
          }
        ecore_file_cp(res->source, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
        free(path);
        path = NULL;
     }
   eina_strbuf_free(buf);
}

static void
_external_resource_export(Eina_List *resources, Eina_Stringshare *name, const char *dst)
{
   Eina_Strbuf *buf;
   Eina_List *l;
   External_Resource *res;
   char *path;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(resources, l, res)
     {
        if (name == res->name)
          {
             eina_strbuf_append_printf(buf, "%s/%s", dst, res->name);
             path = ecore_file_dir_get(eina_strbuf_string_get(buf));
             if (!ecore_file_is_dir(path))
               {
                  ecore_file_mkpath(path);
               }
             ecore_file_cp(res->source, eina_strbuf_string_get(buf));
             eina_strbuf_reset(buf);
             free(path);
             break;
          }
     }
   eina_strbuf_free(buf);
}

static void *
_group_source_code_export(void *data, Eina_Thread *thread __UNUSED__)
{
   Eina_Stringshare *code;
   FILE *f;
   Group *group = (Group *)data;
   Part *part;
   State *state;
   Eina_List *l, *ls;
   Eina_Stringshare *resource;
   Resource *res;
   Eina_Strbuf *buf;
   char *name;

   PROGRESS_SEND(_("Generate group '%s' source code ..."), group->name);

   name = strdup(group->name);
   string_char_replace(name, '/', '_');
   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s/%s", worker.path, name);
   ecore_file_mkdir(eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);

   /* create and open file for collect the group source code */
   eina_strbuf_append_printf(buf, "%s/%s/generated.edc", worker.path, name);
   f = fopen(eina_strbuf_string_get(buf), "w");
   if (!f)
     {
        ERR("Could't open file '%s'", eina_strbuf_string_get(buf))
        END_SEND(PM_PROJECT_ERROR);
        goto exit;
     }

   /* get the full source code of given project */
   ecore_thread_main_loop_begin();
   code = edje_edit_source_generate(group->edit_object);
   ecore_thread_main_loop_end();

   fputs(code, f);
   edje_edit_string_free(code);
   fclose(f);

   /* export images and fonts in accordance with parts  */
   EINA_LIST_FOREACH(group->parts, l, part)
     {
        if (part->type == EDJE_PART_TYPE_IMAGE)
          {
             PROGRESS_SEND(_("Export images ..."));
             eina_strbuf_reset(buf);
             eina_strbuf_append_printf(buf, "%s/%s/images/", worker.path, name);
             ecore_file_mkdir(eina_strbuf_string_get(buf));
             EINA_LIST_FOREACH(part->states, ls, state)
               {
                  resource = edje_edit_state_image_get(group->edit_object, part->name, state->name, state->val);
                  PROGRESS_SEND(_("Export image '%s'"), resource);
                  _external_resource_export(worker.project->images, resource, eina_strbuf_string_get(buf));
                  eina_stringshare_del(resource);
               }
          }
        if (part->type == EDJE_PART_TYPE_TEXT)
          {
             PROGRESS_SEND(_("Export fonts ..."));
             eina_strbuf_reset(buf);
             eina_strbuf_append_printf(buf, "%s/%s/fonts/", worker.path, name);
             ecore_file_mkdir(eina_strbuf_string_get(buf));
             EINA_LIST_FOREACH(part->states, ls, state)
               {
                  resource = edje_edit_state_font_get(group->edit_object, part->name, state->name, state->val);
                  Eina_Stringshare *font_res = edje_edit_font_path_get(group->edit_object, resource);
                  PROGRESS_SEND(_("Export font '%s'"), font_res);
                  _external_resource_export(worker.project->fonts, font_res, eina_strbuf_string_get(buf));
                  eina_stringshare_del(resource);
                  eina_stringshare_del(font_res);
               }
          }
     }
   PROGRESS_SEND(_("Export sounds ..."));
   EINA_LIST_FOREACH(group->programs, l, res)
     {
        resource = edje_edit_program_sample_name_get(group->edit_object, res->name);
        if (!resource) continue;

        eina_strbuf_reset(buf);
        eina_strbuf_append_printf(buf, "%s/%s/sounds/", worker.path, name);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        PROGRESS_SEND(_("Export sound '%s'"), res->name);
        _external_resource_export(worker.project->sounds, resource, eina_strbuf_string_get(buf));
        eina_stringshare_del(resource);
     }

   eina_strbuf_reset(buf);
   eina_strbuf_append_printf(buf, "%s/%s/build.sh", worker.path, name);
   _build_script_write(eina_strbuf_string_get(buf));

   END_SEND(PM_PROJECT_SUCCESS);
exit:
   eina_strbuf_free(buf);
   free(name);
   return NULL;
}

void
pm_group_source_code_export(Project *project,
                            Group *group,
                            const char *path,
                            PM_Project_Progress_Cb func_progress,
                            PM_Project_End_Cb func_end,
                            const void *data)
{
   assert(project != NULL);
   assert(group != NULL);
   assert(path != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, path, NULL, NULL, data, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_group_source_code_export, (void *)group))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

static Eina_Bool
_project_src_export(const char *path)
{
   char *code;
   Eina_Strbuf *buf;
   FILE *f;

   buf = eina_strbuf_new();
   /* create a folder for collect the source files */
   ecore_file_mkdir(path);
   eina_strbuf_reset(buf);

   /* create and open edc file for print the source code of collection (project) */
   eina_strbuf_append_printf(buf, "%s/generated.edc", path);
   f = fopen(eina_strbuf_string_get(buf), "w");
   if (!f)
     {
        ERR("Could't open file '%s'", eina_strbuf_string_get(buf))
        return false;
     }
   eina_strbuf_reset(buf);

   /* get the full source code of given project */
   ecore_thread_main_loop_begin();
   code = edje_edit_full_source_generate(worker.project->global_object);
   ecore_thread_main_loop_end();

   fputs(code, f);
   free(code);
   if (f) fclose(f);

   /* export resource */
   if (worker.project->images)
     {
        eina_strbuf_append_printf(buf, "%s/images/", path);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        _external_resources_export(worker.project->images, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
     }
   if (worker.project->sounds)
     {
        eina_strbuf_append_printf(buf, "%s/sounds/", path);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        _external_resources_export(worker.project->sounds, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
     }
   if (worker.project->fonts)
     {
        eina_strbuf_append_printf(buf, "%s/fonts/", path);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        _external_resources_export(worker.project->fonts, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
     }

   eina_strbuf_reset(buf);
   eina_strbuf_append_printf(buf, "%s/build.sh", path);
   _build_script_write(eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   return true;
}

static void *
_source_code_export(void *data __UNUSED__, Eina_Thread *thread __UNUSED__)
{
   Eina_Strbuf *buf;

   PROGRESS_SEND(_("Generate source code ..."));
   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s/%s", worker.path, worker.project->name);
   if (_project_src_export(eina_strbuf_string_get(buf)))
     END_SEND(PM_PROJECT_SUCCESS)
   else
     END_SEND(PM_PROJECT_ERROR)
   eina_strbuf_free(buf);
   return NULL;
}

void
pm_project_source_code_export(Project *project,
                              const char *path,
                              PM_Project_Progress_Cb func_progress,
                              PM_Project_End_Cb func_end,
                              const void *data)
{
   assert(project != NULL);
   assert(path != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, path, NULL, NULL, data, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_source_code_export, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

static void *
_release_export(void *data __UNUSED__,
                Eina_Thread *thread __UNUSED__)
{
   /**
    * Comment by Andrii:
    * | problem with edje_cc is that it uses child process itself:
    * | when we create thread SIGCHLD is blocked for that
    * | thread by commit aeeda1f77d1b21b15e916852baac06bb530618e2,
    * | when we run edje_cc from that thread it's process
    * | inherits all blocked signals edje_cc starts embryo compiler
    * | if group has scripts it waits for child(compiler)
    * | process termination, but never receives it because of blocked SIGCHLD
    */
#ifndef _WIN32
   sigset_t oldset, newset;
   sigemptyset(&newset);
   sigaddset(&newset, SIGCHLD);
   sigprocmask(SIG_UNBLOCK, &newset, &oldset);
#endif

   Eina_Tmpstr *tmp_dirname;
   Eina_Strbuf *cmd;
   Ecore_Exe *exe_cmd;
   pid_t exe_pid;
   Ecore_Event_Handler *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   int waitpid_res = 0, edje_cc_res = 0;

   PROGRESS_SEND(_("Export project as release file"));
   PROGRESS_SEND(_("Export to file '%s'"), worker.edj);

   eina_file_mkdtemp("eflete_build_XXXXXX", &tmp_dirname);
   if (!_project_src_export(tmp_dirname))
     {
        END_SEND(PM_PROJECT_ERROR)
        goto exit0;
     }
   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, NULL);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, NULL);
     }
   cmd = eina_strbuf_new();
   eina_strbuf_append_printf(cmd, "edje_cc -v -id %s/images/ -sd %s/sounds/ -fd %s/fonts/ %s/generated.edc %s",
                             tmp_dirname, tmp_dirname, tmp_dirname, tmp_dirname, worker.edj);
   THREAD_TESTCANCEL;
   exe_cmd = ecore_exe_pipe_run(eina_strbuf_string_get(cmd), flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(exe_pid, &edje_cc_res, 0);
   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }
   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_cc_res) && (WEXITSTATUS(edje_cc_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        goto exit1;
     }

   PROGRESS_SEND("Export done");
   END_SEND(PM_PROJECT_SUCCESS);

exit1:
   eina_strbuf_free(cmd);
exit0:
   ecore_file_recursive_rm(tmp_dirname);
   eina_tmpstr_del(tmp_dirname);
   return NULL;
}

void
pm_project_release_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   assert(project != NULL);
   assert(path != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, path, NULL, data, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_release_export, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

static void *
_develop_export(void *data __UNUSED__,
                Eina_Thread *thread __UNUSED__)
{
   Ecore_Event_Handler *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   Eina_Tmpstr *tmp_filename = NULL;
   Eina_Stringshare *cmd;
   Eina_List *l;
   Group *group;
   Ecore_Exe *exe_cmd;
   pid_t exe_pid;
   int edje_pick_res = 0, waitpid_res = 0;

   PROGRESS_SEND(_("Export project as develop file"));
   PROGRESS_SEND(_("Export to file '%s'"), worker.edj);
   CRIT_ON_FAIL(editor_save_all(worker.project->global_object));

   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, NULL);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, NULL);
     }

   if (!ecore_file_exists(worker.edj))
     cmd = eina_stringshare_printf("edje_pick -o %s", worker.edj);
   else
     {
        eina_file_mkstemp("eflete_export_XXXXXX", &tmp_filename);
        cmd = eina_stringshare_printf("edje_pick -o %s", tmp_filename);
        cmd = eina_stringshare_printf("%s -a %s", cmd, worker.edj);
     }
   cmd = eina_stringshare_printf("%s -i %s", cmd, worker.project->dev);

   EINA_LIST_FOREACH(worker.project->groups, l, group)
     {
        cmd = eina_stringshare_printf("%s -g %s", cmd, group->name);
     }
   DBG("Run command for export: %s", cmd);
   exe_cmd = ecore_exe_pipe_run(cmd, flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(exe_pid, &edje_pick_res, 0);

   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_pick_res) && (WEXITSTATUS(edje_pick_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   if (tmp_filename)
     {
        ecore_file_recursive_rm(worker.edj);
        ecore_file_mv(tmp_filename, worker.edj);
     }

   PROGRESS_SEND("Export done");

   END_SEND(PM_PROJECT_SUCCESS);
   return NULL;
}

void
pm_project_develop_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   assert(project != NULL);
   assert(path != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, path, NULL, data, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_develop_export, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

#ifdef HAVE_ENVENTOR
static void *
_enventor_save(void *data __UNUSED__,
               Eina_Thread *thread __UNUSED__)
{
   /**
    * Comment by Andrii:
    * | problem with edje_cc is that it uses child process itself:
    * | when we create thread SIGCHLD is blocked for that
    * | thread by commit aeeda1f77d1b21b15e916852baac06bb530618e2,
    * | when we run edje_cc from that thread it's process
    * | inherits all blocked signals edje_cc starts embryo compiler
    * | if group has scripts it waits for child(compiler)
    * | process termination, but never receives it because of blocked SIGCHLD
    */
#ifndef _WIN32
   sigset_t oldset, newset;
   sigemptyset(&newset);
   sigaddset(&newset, SIGCHLD);
   sigprocmask(SIG_UNBLOCK, &newset, &oldset);
#endif

   Ecore_Event_Handler *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   Eina_Stringshare *cmd, *edj, *dir;
   Ecore_Exe *exe_cmd;
   pid_t exe_pid;
   Eina_List *l;
   Eina_Strbuf *buf = NULL;
   int edje_cc_res = 0, edje_pick_res = 0, waitpid_res = 0;

   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, worker);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, worker);
     }
   edj = eina_stringshare_printf("%s/build.edj", worker.project->enventor->path);
   buf = eina_strbuf_new();

   EINA_LIST_FOREACH(worker.project->res.images, l, dir)
     {
        eina_strbuf_append_printf(buf, " -id %s", dir);
     }
   EINA_LIST_FOREACH(worker.project->res.fonts, l, dir)
     {
        eina_strbuf_append_printf(buf, " -fd %s", dir);
     }
   EINA_LIST_FOREACH(worker.project->res.sounds, l, dir)
     {
        eina_strbuf_append_printf(buf, " -sd %s", dir);
     }
   cmd = eina_stringshare_printf("edje_cc -v %s %s %s", eina_strbuf_string_get(buf),
                                 worker.project->enventor->file, edj);
   eina_strbuf_free(buf);
   THREAD_TESTCANCEL;
   DBG("Run command for compile: %s", cmd);
   exe_cmd = ecore_exe_pipe_run(cmd, flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(exe_pid, &edje_cc_res, 0);

   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }
   eina_stringshare_del(cmd);

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_cc_res) && (WEXITSTATUS(edje_cc_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }
   THREAD_TESTCANCEL;

   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, worker);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, worker);
     }
   worker.edj = eina_stringshare_printf("%s/enbuild.edj",
                                         worker.project->enventor->path);
   cmd = eina_stringshare_printf("edje_pick -o %s -a %s -i %s -g %s",
                                 worker.edj,
                                 worker.project->dev, edj,
                                 worker.project->current_style->full_group_name);

   DBG("Run command for compile: %s", cmd);
   exe_cmd = ecore_exe_pipe_run(cmd, flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(exe_pid, &edje_pick_res, 0);

   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_pick_res) && (WEXITSTATUS(edje_pick_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   eina_file_copy(worker.edj, worker.project->dev,
                  EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                  NULL, NULL);

   eina_stringshare_del(edj);
   END_SEND(PM_PROJECT_SUCCESS)

   return NULL;
}

void
pm_project_enventor_save(Project *project,
                         PM_Project_Progress_Cb func_progress,
                         PM_Project_End_Cb func_end,
                         const void *data)
{
   assert(project != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, NULL, NULL, NULL, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_enventor_save, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}

#endif /* HAVE_ENVENTOR */

Eina_Bool
pm_lock_check(const char *path)
{
   return _lock_try(path, false);
}

Eina_Bool
pm_project_group_import(Project *project, const char *edj, const char *group)
{
   Change *change;
   Evas_Object *obj, *img;
   Evas *e;
   Eina_List *resources, *resources1, *l, *l1;
   Eina_Stringshare *data, *data1, *source, *res_file;
   char *res_dir;
   External_Resource *res;
   int id, size;
   int c1_r, c1_g, c1_b, c1_a,
       c2_r, c2_g, c2_b, c2_a,
       c3_r, c3_g, c3_b, c3_a;
   FILE *f;
   Eina_Binbuf *sound_bin;
   void *font;
   Eet_File *ef;
   Resource request;

   assert(edj != NULL);
   assert(group != NULL);

   ecore_thread_main_loop_begin();
   e = ecore_evas_get(project->ecore_evas);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, edj, group);

   /* import group dependencies */
   resources = gm_group_used_groups_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        pm_project_group_import(project, edj, data);
     }
   /* import images */
   resources = gm_group_used_images_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        if (project->images)
          {
             /* if images list is not empty get from first item the path to
              * resource folder, in currect case - path to folder with images */
             res = eina_list_data_get(project->images);
             res_dir = ecore_file_dir_get(res->source);
             request.resource_type = RESOURCE_TYPE_IMAGE;
             request.name = data;
             res = (External_Resource *)resource_get(project->images, &request);
             if ((res) &&
                 ((res->name == data) || (!strcmp(res->name, data))))
               {
                  TODO("implement resource replacing while importing, see comment bellow")
                  /* when efl merge window will be opened need to add logic for
                   * replace any resource. Now we can do it only for images, because
                   * edje_edit has a API for replace one image to enother, but for
                   * sounds, fonts, color_classes and styles API is missing. So
                   * be better if replacing functional been implemented for all
                   * resource type at the same time */
                  continue;
               }
          }
        else
          {
             res_dir = string_cat(project->develop_path, "images");
          }
        img = evas_object_image_add(e);
        id = edje_edit_image_id_get(obj, data);
        if (id < 0)
          {
             WARN("Image %s coudn't be exported", data);
             continue;
          }
        /* save image, it's need for insert to project dev file */
        source = eina_stringshare_printf("edje/images/%i", id);
        evas_object_image_file_set(img, edj, source);
        res_file = eina_stringshare_printf("%s/%s", res_dir, data);
        evas_object_image_save(img, res_file, NULL, NULL);
        evas_object_del(img);

        CRIT_ON_FAIL(edje_edit_image_add(project->global_object, res_file));
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_IMAGE);
        res->source = eina_stringshare_add(data);
        resource_insert(&project->images, (Resource *)res);

        eina_stringshare_del(source);
        eina_stringshare_del(res_file);
        free(res_dir);
     }
   /* images imported */

   /* import sound */
   resources = gm_group_used_sample_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        if (project->sounds)
          {
             res = eina_list_data_get(project->sounds);
             res_dir = ecore_file_dir_get(res->source);
             request.resource_type = RESOURCE_TYPE_SOUND;
             request.name = data;
             res = (External_Resource *)resource_get(project->sounds, &request);
             if ((res) &&
                 ((res->name == data) || (!strcmp(res->name, data))))
               {
                  TODO("implement resource replacing")
                  continue;
               }
          }
        else
          {
             res_dir = string_cat(project->develop_path, "sounds");
          }
        sound_bin = edje_edit_sound_samplebuffer_get(obj, data);
        res_file = eina_stringshare_printf("%s/%s", res_dir, data);
        if (!(f = fopen(res_file, "wb")))
          {
             ERR("Could not open file: %s", res_file);
             continue;
          }
        if (fwrite(eina_binbuf_string_get(sound_bin),
                   eina_binbuf_length_get(sound_bin), 1, f) != 1)
          ERR("Could not write sound: %s", strerror(errno));
        if (f) fclose(f);
        eina_binbuf_free(sound_bin);

        CRIT_ON_FAIL(edje_edit_sound_sample_add(project->global_object, data, res_file));
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_SOUND);
        res->source = eina_stringshare_add(res_file);
        resource_insert(&project->sounds, (Resource *)res);

        eina_stringshare_del(res_file);
        free(res_dir);
     }
   /* sounds imported */

   /* import fonts */
   resources = gm_group_used_fonts_edj_get(obj);
   ef = eet_open(edj, EET_FILE_MODE_READ);
   EINA_LIST_FOREACH(resources, l, data)
     {
        if (project->fonts)
          {
             res = eina_list_data_get(project->fonts);
             res_dir = ecore_file_dir_get(res->source);
             request.resource_type = RESOURCE_TYPE_FONT;
             request.name = data;
             res = (External_Resource *)resource_get(project->fonts, &request);
             if ((res) &&
                 ((res->name == data) || (!strcmp(res->name, data))))
               {
                  TODO("implement resource replacing")
                  continue;
               }
          }
        else
          {
             res_dir = string_cat(project->develop_path, "fonts");
          }
        source = eina_stringshare_printf("edje/fonts/%s", data);
        font = eet_read(ef, source, &size);
        res_file = eina_stringshare_printf("%s/%s", res_dir, data);
        if (!font) continue;
        if (!(f = fopen(res_file, "wb")))
          {
             ERR("Could not open file: %s", res_file);
             continue;
          }
        if (fwrite(font, size, 1, f) != 1)
          ERR("Could not write font: %s", strerror(errno));
        if (f) fclose(f);
        free(font);
        eina_stringshare_del(source);

        edje_edit_font_add(project->global_object, res_file, data);
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_FONT);
        res->source = eina_stringshare_add(res_file);
        resource_insert(&project->fonts, (Resource *)res);

        eina_stringshare_del(res_file);
        free(res_dir);
     }
   eet_close(ef);
   /* fonts imported */

   /* import color_classes */
   resources = gm_group_used_color_classes_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        request.resource_type = RESOURCE_TYPE_COLORCLASS;
        request.name = data;
        res = (External_Resource *)resource_get(project->colorclasses, &request);
        if ((res) &&
            ((res->name == data) || (!strcmp(res->name, data))))
          {
             TODO("implement resource replacing")
             continue;
          }
        edje_edit_color_class_colors_get(obj, data,
                                         &c1_r, &c1_g, &c1_b, &c1_a,
                                         &c2_r, &c2_g, &c2_b, &c2_a,
                                         &c3_r, &c3_g, &c3_b, &c3_a);
        CRIT_ON_FAIL(edje_edit_color_class_add(project->global_object, data));
        CRIT_ON_FAIL(edje_edit_color_class_colors_set(project->global_object, data,
                                                      c1_r, c1_g, c1_b, c1_a,
                                                      c2_r, c2_g, c2_b, c2_a,
                                                      c3_r, c3_g, c3_b, c3_a));
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_COLORCLASS);
        resource_insert(&project->colorclasses, (Resource *)res);
     }
   /* color_classes imported */

   /* import styles */
   resources = gm_group_used_styles_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        request.resource_type = RESOURCE_TYPE_STYLE;
        request.name = data;
        res = (External_Resource *)resource_get(project->styles, &request);
        if ((res) &&
            ((res->name == data) || (!strcmp(res->name, data))))
          {
             TODO("implement resource replacing")
             continue;
          }
        edje_edit_style_add(project->global_object, data);
        resources1 = edje_edit_style_tags_list_get(obj, data);
        EINA_LIST_FOREACH(resources1, l1, data1)
          {
             source = edje_edit_style_tag_value_get(obj, data, data1);
             CRIT_ON_FAIL(edje_edit_style_tag_add(project->global_object, data, data1));
             CRIT_ON_FAIL(edje_edit_style_tag_value_set(project->global_object, data, data1, source));
             eina_stringshare_del(source);
          }
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_STYLE);
        resource_insert(&project->styles, (Resource *)res);
     }
   /* styles imported */

   change = change_add(NULL);

   /* add fake change for min/max group attributes.
    * Try to set the default values */
   CRIT_ON_FAIL(editor_group_max_h_set(obj, change, true, false, 0));
   CRIT_ON_FAIL(editor_group_max_w_set(obj, change, true, false, 0));
   CRIT_ON_FAIL(editor_group_min_h_set(obj, change, true, false, 0));
   CRIT_ON_FAIL(editor_group_min_w_set(obj, change, true, false, 0));

   you_shall_not_pass_editor_signals(NULL);
   /* fake changes for imported group
    * reset all attributes for program and parts, for avoid errors in case
    * cycled dependencies;
    */
   editor_group_reset(obj, change, false);

   /* apply group to project */
   CRIT_ON_FAIL(editor_group_add(project->global_object, group));
   CRIT_ON_FAIL(editor_save_all(project->global_object));
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, project->dev, group);

   change_undo(obj, change);
   CRIT_ON_FAIL(editor_save_all(obj));
   /* reload file after group add */
   pm_dev_file_reload(project);
   /* add group to project structures */
   gm_group_add(project, group);

   you_shall_pass_editor_signals(NULL);

   change_free(change);
   ecore_thread_main_loop_end();

   return true;
}
