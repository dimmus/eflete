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
#include "project_manager_new.h"
#include "alloc.h"
#include <sys/wait.h>

#define PROJECT_FILE_KEY      "project"
#define THREAD_TESTCANCEL pthread_testcancel()

#define WORKER_CREATE(FUNC_PROGRESS, FUNC_END, DATA, \
                      NAME, PATH, EDJ, EDC, BUILD_OPTIONS) \
{ \
   worker = (Project_Thread *)mem_malloc(sizeof(Project_Thread)); \
   worker->func_progress = FUNC_PROGRESS; \
   worker->func_end = FUNC_END; \
   worker->data = (void *)DATA; \
   worker->project = NULL; \
   worker->name = eina_stringshare_add(NAME); \
   worker->path = eina_stringshare_add(PATH); \
   worker->edj = eina_stringshare_add(EDJ); \
   worker->edc = eina_stringshare_add(EDC); \
   worker->build_options = eina_stringshare_add(BUILD_OPTIONS); \
   eina_lock_new(&worker->mutex); \
}

#define WORKER_FREE() \
{ \
   eina_lock_free(&worker->mutex); \
   eina_stringshare_del(worker->name); \
   eina_stringshare_del(worker->path); \
   eina_stringshare_del(worker->edj); \
   eina_stringshare_del(worker->build_options); \
   free(worker); \
   worker = NULL; \
}

#define WORKER_LOCK_TAKE            eina_lock_take(&worker->mutex)
#define WORKER_LOCK_RELEASE         eina_lock_release(&worker->mutex)

#define PROGRESS_SEND(FMT, ...) \
{ \
   if (worker->func_progress) \
      { \
         WORKER_LOCK_TAKE; \
            worker->message = eina_stringshare_printf(FMT, ## __VA_ARGS__); \
         WORKER_LOCK_RELEASE; \
         ecore_main_loop_thread_safe_call_async(_progress_send, worker); \
      } \
}

#define END_SEND(STATUS) \
{ \
   if (worker->func_end) \
     { \
         WORKER_LOCK_TAKE; \
            worker->result = STATUS; \
         WORKER_LOCK_RELEASE; \
         ecore_main_loop_thread_safe_call_async(_end_send, worker); \
     } \
}

static Eet_Data_Descriptor *eed_project = NULL;

static void
_project_descriptor_init(void)
{
   Eet_Data_Descriptor_Class eddc;

   if (eed_project) return;

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc),
                                             "Project", sizeof(Project));
   eed_project = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "dev", dev, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "develop_path", develop_path, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "release_path", release_path, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "release_options", release_options, EET_T_STRING);
}

static void
_pm_project_descriptor_shutdown(void)
{
   eet_data_descriptor_free(eed_project);
}

static Eina_Bool
_pm_project_descriptor_data_write(const char *path, Project *project)
{
   Eet_File *ef;
   Eina_Bool ok = false;

   ef = eet_open(path, EET_FILE_MODE_WRITE);
   if (ef)
     ok = eet_data_write(ef, eed_project, PROJECT_FILE_KEY, project, 1);

   eet_close(ef);
   return ok;
}

static void
_progress_send(void *data)
{
   Project_Thread *worker;
   PM_Project_Progress_Cb func;
   Eina_Stringshare *message;
   void *udata;

   worker = (Project_Thread *)data;
   /** Copy the links to callback and meesage, to fast release worker resource. */
   WORKER_LOCK_TAKE;
      func = worker->func_progress;
      message = eina_stringshare_ref(worker->message);
      udata = worker->data;
   WORKER_LOCK_RELEASE;
   func(udata, message);
   eina_stringshare_del(message);
}

static void
_end_send(void *data)
{
   Project_Thread *worker;
   PM_Project_End_Cb func;
   PM_Project_Result result;
   void *udata;

   worker = (Project_Thread *)data;
   /** Copy the links to callback and meesage, to fast release worker resource. */
   WORKER_LOCK_TAKE;
      func = worker->func_end;
      result = worker->result;
      udata = worker->data;
   WORKER_LOCK_RELEASE;
   func(udata, result);
}

static Project *
_project_files_create(Project_Thread *worker)
{
   Project *pro;
   Eina_Stringshare *folder_path, *pro_path;
   Eina_Bool error = false;

   _project_descriptor_init();

   WORKER_LOCK_TAKE;
      folder_path = eina_stringshare_printf("%s/%s", worker->path, worker->name);
      ecore_file_mkdir(folder_path);
      DBG("Create the folder '%s' for new project '%s'", folder_path, worker->name);
   WORKER_LOCK_RELEASE;

   THREAD_TESTCANCEL;
   pro = (Project *)mem_malloc(sizeof(Project));
   WORKER_LOCK_TAKE;
      pro->dev = eina_stringshare_printf("%s/%s.dev", folder_path, worker->name);
      pro->develop_path = eina_stringshare_printf("%s/develop", folder_path);
      pro->release_path = eina_stringshare_printf("%s/release", folder_path);
      pro->release_options = NULL;

      pro_path = eina_stringshare_printf("%s/%s.pro", folder_path, worker->name);
   WORKER_LOCK_RELEASE;
   if (!_pm_project_descriptor_data_write(pro_path, pro))
     error = true;

   DBG("Create a specific project file '%s'", pro_path);
   THREAD_TESTCANCEL;
   _pm_project_descriptor_shutdown();
   eina_stringshare_del(folder_path);
   eina_stringshare_del(pro_path);
   if (error)
     {
        ERR("Could't create a .pro file! ")
        eina_stringshare_del(pro->dev);
        eina_stringshare_del(pro->develop_path);
        eina_stringshare_del(pro->release_path);
        eina_stringshare_del(pro->release_options);
        free(pro);
        pro = NULL;
     }
   return pro;
}

static Eina_Bool
_copy_file_progress_cb(void *data,
                       unsigned long long done,
                       unsigned long long total)
{
   short int percentage;
   Project_Thread *worker;

   worker = (Project_Thread *)data;
   percentage = (short int)(done / total) * 100;
   PROGRESS_SEND(_("Importing... %u"), percentage);

   return true;
}

static Eina_Bool
_project_dev_file_copy(Project_Thread *worker)
{
   Eina_Stringshare *src, *dst;
   Eina_Bool result;

   WORKER_LOCK_TAKE;
      src = eina_stringshare_ref(worker->edj);
      dst = eina_stringshare_ref(worker->project->dev);
   WORKER_LOCK_RELEASE;
      result = eina_file_copy(src, dst,
                              EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                              _copy_file_progress_cb, worker);

   DBG("Copy the .dev file to project folder.");
   eina_stringshare_del(src);
   eina_stringshare_del(dst);
   return result;
}

static void *
_project_import_edj(void *data,
                    Eina_Thread *thread)
{
   Project_Thread *worker;
   pthread_attr_t attr;

   /** try to change the detach state */
   if (!pthread_getattr_np(*thread, &attr))
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   pthread_attr_destroy(&attr);

   worker = (Project_Thread *)data;

   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Creating a specifiec file and folders..."));
   worker->project = _project_files_create(worker);
   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Importing..."));
   _project_dev_file_copy(worker);

   END_SEND(PM_PROJECT_SUCCESS);

   return NULL;
}

Project_Thread *
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end ,
                      const void *data)
{
   Project_Thread *worker;
   Eina_Bool result;

   WORKER_CREATE(func_progress, func_end, data,
                 name, path, edj, NULL, NULL);

   result = eina_thread_create(&worker->thread, EINA_THREAD_URGENT, -1,
                               (void *)_project_import_edj, worker);
   if (!result)
     WORKER_FREE();

   return worker;
}

static Eina_Bool
_exe_exit(void *data,
          int type __UNUSED__,
          void *event)
{
   Ecore_Exe_Event_Del *e = (Ecore_Exe_Event_Del *)event;
   Project_Thread *worker = (Project_Thread *)data;

   if (e->exit_code)
     END_SEND(PM_PROJECT_ERROR);

   return EINA_TRUE;
}

static Eina_Bool
_exe_data(void *data,
          int type __UNUSED__,
          void *event)
{
   int i;
   Ecore_Exe_Event_Data *ev = event;
   Project_Thread *worker = (Project_Thread *)data;

   if (ev->lines)
     {
        for (i = 0; ev->lines[i].line; i++)
          {
             PROGRESS_SEND("%s", ev->lines[i].line);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static void *
_project_import_edc(void *data,
                    Eina_Thread *thread)
{
   Project_Thread *worker;
   pthread_attr_t attr;
   Ecore_Event_Handler *cb_exit = NULL,
                       *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   Eina_Stringshare *cmd;
   Ecore_Exe *exe_cmd;
   pid_t exe_pid;

   /** try to change the detach state */
   if (!pthread_getattr_np(*thread, &attr))
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   pthread_attr_destroy(&attr);

   worker = (Project_Thread *)data;

   cb_exit = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _exe_exit, worker);
   WORKER_LOCK_TAKE;
      if (worker->func_progress)
        {
           cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, worker);
           cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, worker);
        }
      worker->edj = eina_stringshare_printf("/tmp/build_%ld.edj", time(NULL));
      cmd = eina_stringshare_printf("edje_cc -v %s %s %s",
                                    worker->build_options,
                                    worker->edc,
                                    worker->edj);
      THREAD_TESTCANCEL;
   WORKER_LOCK_RELEASE;
   DBG("Run command for compile: %s", cmd);
   exe_cmd = ecore_exe_pipe_run(cmd, flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   /* TODO: it's work only in Posix system, need add to Ecore Spawing Functions
    * function what provide wait end of forked process.*/
   waitpid(exe_pid, NULL, 0);

   ecore_event_handler_del(cb_exit);
   if (worker->func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Creating a specifiec file and folders..."));
   worker->project = _project_files_create(worker);
   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Importing..."));
   _project_dev_file_copy(worker);

   END_SEND(PM_PROJECT_SUCCESS)
   return NULL;
}

Project_Thread *
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *import_options,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data)
{
   Project_Thread *worker;
   Eina_Bool result;

   WORKER_CREATE(func_progress, func_end, data,
                 name, path, NULL, edc, import_options);

   result = eina_thread_create(&worker->thread, EINA_THREAD_URGENT, -1,
                               (void *)_project_import_edc, worker);
   if (!result)
     WORKER_FREE();

   return worker;
}

Eina_Bool
pm_project_thread_cancel(Project_Thread *worker)
{
   int ret;

   WORKER_LOCK_TAKE;
      ret = pthread_cancel(worker->thread);
   WORKER_LOCK_RELEASE;
   if (ret)
     return false;

   END_SEND(PM_PROJECT_CANCEL);
   DBG("Project Thread %p stoped by user!", worker);
   return true;
}

Eina_Bool
pm_project_thread_free(Project_Thread *worker)
{
   if (worker->result == 0)
     {
        WORKER_FREE();
        return true;
     }
   return false;
}

Project *
pm_project_open(const char *path __UNUSED__)
{
   return NULL;
}

Eina_Bool
pm_project_save(Project *project __UNUSED__)
{
   return false;
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
pm_project_close(Project *project __UNUSED__)
{
   return false;
}

void
pm_project_meta_data_get(Project *project __UNUSED__,
                         char *name __UNUSED__,
                         char *authors __UNUSED__,
                         char *version __UNUSED__,
                         char *license __UNUSED__,
                         char *comment __UNUSED__)
{

}

Eina_Bool
pm_project_meta_data_set(Project *project __UNUSED__,
                         const char *name __UNUSED__,
                         const char *authors __UNUSED__,
                         const char *version __UNUSED__,
                         const char *license __UNUSED__,
                         const char *comment __UNUSED__)
{
   return false;
}
