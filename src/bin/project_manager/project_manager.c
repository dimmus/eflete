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
#include "alloc.h"
#include <sys/wait.h>

#define PROJECT_FILE_KEY      "project"

#define PROJECT_KEY_NAME         "edje/name"
#define PROJECT_KEY_AUTHORS      "edje/authors"
#define PROJECT_KEY_FILE_VERSION "edje/file_version"
#define PROJECT_KEY_LICENSE      "edje/license"
#define PROJECT_KEY_COMMENT      "edje/comment"

static Eet_Compression compess_level = EET_COMPRESSION_HI;

#define WORKER_CREATE(FUNC_PROGRESS, FUNC_END, DATA, PROJECT, \
                      NAME, PATH, EDJ, EDC, BUILD_OPTIONS) \
{ \
   worker = (Project_Thread *)mem_calloc(1, sizeof(Project_Thread)); \
   worker->func_progress = FUNC_PROGRESS; \
   worker->func_end = FUNC_END; \
   worker->data = (void *)DATA; \
   worker->project = PROJECT; \
   worker->result = PM_PROJECT_LAST; \
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
#define THREAD_TESTCANCEL pthread_testcancel()

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
   EET_DATA_DESCRIPTOR_ADD_BASIC(eed_project, Project, "release_options", release_options, EET_T_STRING);
}

static void
_pm_project_descriptor_shutdown(void)
{
   eet_data_descriptor_free(eed_project);
   eed_project = NULL;
}

static Eina_Bool
_pm_project_descriptor_data_write(const char *path, Project *project)
{
   Eina_Bool ok = false;

   project->pro = eet_open(path, EET_FILE_MODE_READ_WRITE);
   if (project->pro)
     ok = eet_data_write(project->pro, eed_project, PROJECT_FILE_KEY,
                         project, compess_level);
   eet_close(project->pro);

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

#define MKDIR(NAME) \
   tmp = eina_stringshare_printf("%s/"NAME, pro->develop_path); \
   ecore_file_mkdir(tmp); \
   eina_stringshare_del(tmp)

static Project *
_project_files_create(Project_Thread *worker)
{
   Project *pro;
   Eina_Stringshare *folder_path, *pro_path, *tmp;
   Eina_Bool error = false;

   _project_descriptor_init();

   WORKER_LOCK_TAKE;
      folder_path = eina_stringshare_printf("%s/%s", worker->path, worker->name);
      if (ecore_file_mkdir(folder_path))
        {
           DBG("Create the folder '%s' for new project '%s'", folder_path, worker->name);
        }
      else
        {
           ERR("Could't create a project folder!");
           error = true;
        }
      eina_stringshare_del(folder_path);
   WORKER_LOCK_RELEASE;
   if (error) return NULL;

   THREAD_TESTCANCEL;
   pro = (Project *)mem_calloc(1, sizeof(Project));
   WORKER_LOCK_TAKE;
      folder_path = eina_stringshare_printf("%s/%s", worker->path, worker->name);
      pro->name = eina_stringshare_add(worker->name);
      pro->dev = eina_stringshare_printf("%s/%s.dev", folder_path, worker->name);
      pro->develop_path = eina_stringshare_printf("%s/develop", folder_path);

      pro_path = eina_stringshare_printf("%s/%s.pro", folder_path, worker->name);
      ecore_file_mkdir(pro->develop_path);
      MKDIR("images");
      MKDIR("sounds");
      MKDIR("fonts");
      MKDIR("data");
      eina_stringshare_del(folder_path);
   WORKER_LOCK_RELEASE;
   if (!_pm_project_descriptor_data_write(pro_path, pro))
     error = true;

   DBG("Create a specific project file '%s': %s", pro_path,
       error ? "failsed" : "success");
   THREAD_TESTCANCEL;
   _pm_project_descriptor_shutdown();
   eina_stringshare_del(pro_path);
   if (error)
     {
        ERR("Could't create a .pro file! ")
        eina_stringshare_del(pro->name);
        eina_stringshare_del(pro->dev);
        eina_stringshare_del(pro->develop_path);
        free(pro);
        pro = NULL;
     }
   return pro;
}

#undef MKDIR

static Eina_Bool
_copy_file_progress_cb(void *data,
                       unsigned long long done,
                       unsigned long long total)
{
   short int percentage;
   Project_Thread *worker;

   worker = (Project_Thread *)data;
   percentage = (short int)((double)done / total) * 100;
   PROGRESS_SEND(_("Importing... %u"), percentage);

   return true;
}

static void
_copy_meta_data_to_pro(Project_Thread *worker)
{
   Eet_File *ef;
   char *name, *authors, *version, *license, *comment;

   WORKER_LOCK_TAKE;
      ef = eet_open(worker->edj, EET_FILE_MODE_READ_WRITE);
   WORKER_LOCK_RELEASE;

   name = strdup(worker->name);
   authors = eet_read(ef, PROJECT_KEY_AUTHORS, NULL);
   version = eet_read(ef, PROJECT_KEY_FILE_VERSION, NULL);
   license = eet_read(ef, PROJECT_KEY_LICENSE, NULL);
   comment = eet_read(ef, PROJECT_KEY_COMMENT, NULL);
   eet_close(ef);

   WORKER_LOCK_TAKE;
      pm_project_meta_data_set(worker->project, name, authors,
                               version, license, comment);
   WORKER_LOCK_RELEASE;

   if (name) free(name);
   if (authors) free(authors);
   if (version) free(version);
   if (license) free(license);
   if (comment) free(comment);
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

static Eina_Bool
_project_linked_images_copy(Project_Thread *worker)
{
   Eina_List *list, *l;
   Evas_Object *edje_edit_obj;
   Evas *e;
   Ecore_Evas *ee;
   const char *name;
   const char *file_name;
   Eina_Strbuf *strbuf_to = eina_strbuf_new();
   Eina_Strbuf *strbuf_from = eina_strbuf_new();
   Edje_Edit_Image_Comp comp;
   Eina_Bool is_changed = false;

   ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   list = edje_file_collection_list(worker->project->dev);
   edje_edit_obj = edje_edit_object_add(e);
   if (!edje_object_file_set(edje_edit_obj, worker->project->dev, eina_list_data_get(list)))
     {
        evas_object_del(edje_edit_obj);
        return false;
     }
   edje_edit_string_list_free(list);

   list = edje_edit_images_list_get(edje_edit_obj);
   EINA_LIST_FOREACH(list, l, name)
     {
        /* for supporting themes that were compilled
           with edje_cc version less than 1.10 */
        if (!name) continue;

        comp = edje_edit_image_compression_type_get(edje_edit_obj, name);
        if (comp != EDJE_EDIT_IMAGE_COMP_USER)
          continue;
        is_changed = true;

        eina_strbuf_reset(strbuf_to);
        eina_strbuf_reset(strbuf_from);

        if (name[0] == '/')
          eina_strbuf_append(strbuf_from, name);
        else
          eina_strbuf_append_printf(strbuf_from, "%s/%s", worker->path, name);
        if (!ecore_file_exists(eina_strbuf_string_get(strbuf_from)))
          {
             edje_edit_image_rename(edje_edit_obj,
                                    name, eina_strbuf_string_get(strbuf_from));
             continue;
          }

        file_name = ecore_file_file_get(name);

        eina_strbuf_append_printf(strbuf_to, "%s/images/%s",
                                  worker->project->develop_path, file_name);

        eina_file_copy(eina_strbuf_string_get(strbuf_from),
                       eina_strbuf_string_get(strbuf_to),
                       EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                       NULL, NULL);
        edje_edit_image_rename(edje_edit_obj,
                               name, eina_strbuf_string_get(strbuf_to));
     }
   if (is_changed)
     edje_edit_without_source_save(edje_edit_obj, false);
   edje_edit_string_list_free(list);
   eina_strbuf_free(strbuf_to);
   eina_strbuf_free(strbuf_from);
   return true;
}

static void *
_project_import_edj(void *data,
                    Eina_Thread *thread __UNUSED__)
{
   Project_Thread *worker;
   Eina_Stringshare *path_pro;

   sleep(1);

   worker = (Project_Thread *)data;

   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Creating a specifiec file and folders..."));
   worker->project = _project_files_create(worker);
   THREAD_TESTCANCEL;
   path_pro = eina_stringshare_printf("%s/%s/%s.pro", worker->path, worker->name, worker->name);
   WORKER_LOCK_TAKE;
      worker->project->pro = eet_open(path_pro, EET_FILE_MODE_READ_WRITE);
   WORKER_LOCK_RELEASE;
   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Importing..."));
   _project_dev_file_copy(worker);
   _copy_meta_data_to_pro(worker);
   WORKER_LOCK_TAKE;
      worker->project->mmap_file = eina_file_open(worker->project->dev, false);
      worker->project->widgets = wm_widgets_list_new(worker->project->dev);
      worker->project->layouts = wm_layouts_list_new(worker->project->dev);
   WORKER_LOCK_RELEASE;
   THREAD_TESTCANCEL;
   WORKER_LOCK_TAKE;
      pm_project_resource_export(worker->project);
      _project_linked_images_copy(worker);
   WORKER_LOCK_RELEASE;

   END_SEND(PM_PROJECT_SUCCESS);
   eina_stringshare_del(path_pro);

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

   WORKER_CREATE(func_progress, func_end, data, NULL,
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
             DBG("%s", ev->lines[i].line);
             PROGRESS_SEND("%s", ev->lines[i].line);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static void *
_project_import_edc(void *data,
                    Eina_Thread *thread __UNUSED__)
{
   Project_Thread *worker;
   Eina_Stringshare *path_pro;
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
   int edje_cc_res = 0, waitpid_res = 0;

   sleep(1);

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
   waitpid_res = waitpid(exe_pid, &edje_cc_res, 0);

   ecore_event_handler_del(cb_exit);
   if (worker->func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_cc_res) && WEXITSTATUS(edje_cc_res) != 0 ))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   THREAD_TESTCANCEL;
   PROGRESS_SEND("%s", _("Creating a specifiec file and folders..."));
   worker->project = _project_files_create(worker);
   THREAD_TESTCANCEL;
   path_pro = eina_stringshare_printf("%s/%s/%s.pro", worker->path, worker->name, worker->name);
   WORKER_LOCK_TAKE;
      worker->project->pro = eet_open(path_pro, EET_FILE_MODE_READ_WRITE);
   WORKER_LOCK_RELEASE;
   PROGRESS_SEND("%s", _("Importing..."));
   _project_dev_file_copy(worker);
   _project_linked_images_copy(worker);
   _copy_meta_data_to_pro(worker);
   WORKER_LOCK_TAKE;
      worker->project->mmap_file = eina_file_open(worker->project->dev, false);
      worker->project->widgets = wm_widgets_list_new(worker->project->dev);
      worker->project->layouts = wm_layouts_list_new(worker->project->dev);
      pm_project_resource_export(worker->project);
   WORKER_LOCK_RELEASE;

   END_SEND(PM_PROJECT_SUCCESS)
   eina_stringshare_del(path_pro);

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

   WORKER_CREATE(func_progress, func_end, data, NULL,
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
      ret = pthread_cancel((pthread_t)worker->thread);
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
pm_project_open(const char *path)
{
   Eet_File *ef;
   Project *pro = NULL;
   char *dev;
   int dev_len;
   struct _Project_Lock
     {
       Project *project;
       Eina_Lock mutex;
     };

   edje_file_cache_flush();
   struct _Project_Lock *pro_lock = mem_calloc(1, sizeof(struct _Project_Lock));
   eina_lock_new(&pro_lock->mutex);

   _project_descriptor_init();
   ef = eet_open(path, EET_FILE_MODE_READ_WRITE);
   if (!ef)
     goto error;

   pro_lock->project = eet_data_read(ef, eed_project, PROJECT_FILE_KEY);
   _pm_project_descriptor_shutdown();
   if (!pro_lock->project) goto error;

   /* updating .dev file path */
   dev = strdup(path);
   dev_len = strlen(dev);
   dev[dev_len - 3] = 'd';
   dev[dev_len - 2] = 'e';
   dev[dev_len - 1] = 'v';
   eina_stringshare_replace(&pro_lock->project->dev, dev);
   free(dev);

   pro_lock->project->mmap_file = eina_file_open(pro_lock->project->dev, false);
   eina_lock_take(&pro_lock->mutex);

   pro_lock->project->changed = false;
   pro_lock->project->close_request = false;
   pro_lock->project->pro = ef;
   pro_lock->project->widgets = wm_widgets_list_new(pro_lock->project->dev);
   pro_lock->project->layouts = wm_layouts_list_new(pro_lock->project->dev);
   pm_project_meta_data_get(pro_lock->project, &pro_lock->project->name, NULL, NULL, NULL, NULL);
   if (!pro_lock->project->name) pro_lock->project->name = eina_stringshare_add(_("No title"));
   eina_lock_release(&pro_lock->mutex);

error:
   eina_lock_free(&pro_lock->mutex);
   pro = pro_lock->project;
   free(pro_lock);
   return pro;
}

static Eina_Bool
_backup_progress_cb(void *data,
                    unsigned long long done,
                    unsigned long long total)
{
   Project_Thread *worker;
   short int percentage;

   worker = (Project_Thread *)data;

   percentage = (short int)(done / total) * 100;
   PROGRESS_SEND(_("Backuping... %u"), percentage);

   return true;
}

static Eina_Bool
_project_backup(Project_Thread *worker)
{
   Eina_Bool result;
   Eina_Stringshare *src, *dst;

   WORKER_LOCK_TAKE;
      src = worker->project->dev;
   WORKER_LOCK_RELEASE;
   dst = eina_stringshare_printf("%s.backup", worker->project->dev);
   result = eina_file_copy(src, dst,
                           EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                           _backup_progress_cb, worker);

   DBG("Make the backup");
   eina_stringshare_del(dst);
   return result;
}


static void *
_project_save(void *data,
              Eina_Thread *thread __UNUSED__)
{
   Project_Thread *worker;
   Widget *widget;
   Style *style;
   Class *class_st;

   sleep(1);

   worker = (Project_Thread *)data;

   PROGRESS_SEND("Saving...");
   WORKER_LOCK_TAKE;
      EINA_INLIST_FOREACH(worker->project->widgets, widget)
        {
           EINA_INLIST_FOREACH(widget->classes, class_st)
             {
                EINA_INLIST_FOREACH(class_st->styles, style)
                  {
                     if (style->isModify)
                       {
                          style->isModify = false;
                          edje_edit_without_source_save(style->obj, true);
                       }
                  }
             }
        }

      EINA_INLIST_FOREACH(worker->project->layouts, style)
        {
           if (style->isModify)
             {
                style->isModify = false;
                edje_edit_without_source_save(style->obj, true);
             }
        }
   eina_file_close(worker->project->mmap_file);
   worker->project->mmap_file = eina_file_open(worker->project->dev, false);
   if (worker->project->current_style)
     {
        edje_object_mmap_set(worker->project->current_style->obj,
                             worker->project->mmap_file,
                             worker->project->current_style->full_group_name);
     }

   WORKER_LOCK_RELEASE;

   PROGRESS_SEND("Making the project backup...");
   _project_backup(worker);

   PROGRESS_SEND("Saved.");
   END_SEND(PM_PROJECT_SUCCESS);
   return NULL;
}

Project_Thread *
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   Project_Thread *worker;
   Eina_Bool result;

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, NULL, NULL, NULL);

   result = eina_thread_create(&worker->thread, EINA_THREAD_URGENT, -1,
                               (void *)_project_save, worker);
   if (!result)
     WORKER_FREE();

   return worker;
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
   Eina_Stringshare *backup;

   backup = eina_stringshare_printf("%s.backup", project->dev);
   ecore_file_remove(backup);

   wm_widgets_list_free(project->widgets);
   wm_layouts_list_free(project->layouts);

   eet_close(project->pro);
   eina_file_close(project->mmap_file);
   eina_stringshare_del(project->name);
   eina_stringshare_del(project->dev);
   eina_stringshare_del(project->develop_path);

   eina_stringshare_del(backup);

#ifdef HAVE_ENVENTOR
   eina_stringshare_del(project->enventor.file);
   if (project->enventor.path)
     {
        ecore_file_recursive_rm(project->enventor.path);
        eina_stringshare_del(project->enventor.path);
     }
#endif /* HAVE_ENVENTOR */

   free(project);

   return true;
}

void
pm_project_changed(Project *project)
{
   eina_file_close(project->mmap_file);
   project->mmap_file = eina_file_open(project->dev, false);
   if (project->current_style)
     {
        eina_file_map_free(project->mmap_file, project->current_style->obj);
        edje_object_mmap_set(project->current_style->obj,
                             project->mmap_file,
                             project->current_style->full_group_name);
     }
   project->changed = true;
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

#define DATA_READ(DATA, KEY) \
   if (DATA) \
     { \
        tmp = eet_read(project->pro, KEY, NULL); \
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

   res = true;

#define DATA_WRITE(DATA, KEY) \
   if (DATA) \
     { \
        size = (strlen(DATA) + 1) * sizeof(char); \
        bytes = eet_write(project->pro, KEY, DATA, size, compess_level); \
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
_image_resources_export(Eina_List *images, Eina_Stringshare *destination,
                        Eina_Stringshare *source, Eina_Stringshare *dev,
                        Evas_Object *edje_edit)
{
  Eina_Stringshare *image_name, *source_file, *dest_file;
  Eina_List *l;
  Ecore_Evas *ee;
  Evas *e;
  Evas_Object *im;
  int id;
  char *file_dir;

  if (images)
    {
       if (!ecore_file_mkpath(destination))
         {
            ERR("Failed create path %s for export images", destination);
            return false;
         }
       ee = ecore_evas_buffer_new(0, 0);
       e = ecore_evas_get(ee);
    }
  else return false;
  EINA_LIST_FOREACH(images, l, image_name)
    {
       /* for supporting old themes, which were compilled
        * with edje_cc version less than 1.10 */
       if (!image_name) continue;
       source_file = eina_stringshare_printf("%s/%s", source,
                                             ecore_file_file_get(image_name));
       dest_file = eina_stringshare_printf("%s/%s", destination, image_name);
       file_dir = ecore_file_dir_get(dest_file);
       ecore_file_mkpath(file_dir);
       free(file_dir);
       if ((source) && (ecore_file_exists(source_file)))
         {
            ecore_file_cp(source_file, dest_file);
         }
       else
         {
            im = evas_object_image_add(e);
            id = edje_edit_image_id_get(edje_edit, image_name);
            if (id < 0)
              {
                 WARN("Image %s coudn't be exported", image_name);
                 continue;
              }
            source_file = eina_stringshare_printf("edje/images/%i", id);
            evas_object_image_file_set(im, dev, source_file);
            evas_object_image_save(im, dest_file, NULL, NULL);
            evas_object_del(im);
         }
       eina_stringshare_del(source_file);
       eina_stringshare_del(dest_file);
    }
  ecore_evas_free(ee);

  return true;
}

static Eina_Bool
_sound_resources_export(Eina_List *sounds, Eina_Stringshare *destination,
                        Eina_Stringshare *source, Evas_Object *edje_edit)
{
  Eina_Stringshare *sound_name, *source_file, *dest_file, *sound_file;
  Eina_List *l;
  Eina_Binbuf *sound_bin;
  FILE *f;
  char *file_dir;

  if (sounds)
    {
       if (!ecore_file_mkpath(destination))
         {
            ERR("Failed create path %s for export sounds", destination);
            return false;
         }
    }
  else return false;
  EINA_LIST_FOREACH(sounds, l, sound_name)
    {
       sound_file = edje_edit_sound_samplesource_get(edje_edit, sound_name);
       source_file = eina_stringshare_printf("%s/%s", source, sound_file);
       dest_file = eina_stringshare_printf("%s/%s", destination, sound_file);
       file_dir = ecore_file_dir_get(dest_file);
       ecore_file_mkpath(file_dir);
       free(file_dir);
       if ((source) && (ecore_file_exists(source_file)))
         {
            ecore_file_cp(source_file, dest_file);
         }
       else
         {
            sound_bin = edje_edit_sound_samplebuffer_get(edje_edit, sound_name);
            if (!(f = fopen(dest_file, "wb")))
              {
                 ERR("Could not open file: %s", dest_file);
                 continue;
              }
            if (fwrite(eina_binbuf_string_get(sound_bin),
                       eina_binbuf_length_get(sound_bin), 1, f) != 1)
              ERR("Could not write font: %s", strerror(errno));
            if (f) fclose(f);
            eina_binbuf_free(sound_bin);
         }
       edje_edit_string_free(sound_file);
       eina_stringshare_del(source_file);
       eina_stringshare_del(dest_file);
    }

  return true;
}

static Eina_Bool
_font_resources_export(Eina_List *fonts, Eina_Stringshare *destination,
                        Eina_Stringshare *source, Eina_Stringshare *dev,
                        Evas_Object *edje_edit)
{
  Eet_File *ef;
  Eina_List *l;
  Eina_Stringshare *font_name, *source_file, *dest_file, *font_file;
  void *font;
  FILE *f;
  int size;

  if (fonts)
    {
       if (!ecore_file_mkpath(destination))
         {
            ERR("Failed create path %s for export fonts", destination);
            return false;
         }
       ef = eet_open(dev, EET_FILE_MODE_READ);
    }
  else return false;
  EINA_LIST_FOREACH(fonts, l, font_name)
    {
       font_file = edje_edit_font_path_get(edje_edit, font_name);
       source_file = eina_stringshare_printf("%s/%s", source, font_file);
       dest_file = eina_stringshare_printf("%s/%s", destination, font_file);
       edje_edit_string_free(font_file);
       if ((source) && (ecore_file_exists(source_file)))
         {
            ecore_file_cp(source_file, dest_file);
         }
       else
         {
             font_file = eina_stringshare_printf("edje/fonts/%s", font_name);
             font = eet_read(ef, font_file, &size);
             if (!font) continue;
             if (!(f = fopen(dest_file, "wb")))
               {
                  ERR("Could not open file: %s", dest_file);
                  continue;
               }
             if (fwrite(font, size, 1, f) != 1)
               ERR("Could not write font: %s", strerror(errno));
             if (f) fclose(f);
             free(font);
             eina_stringshare_del(font_file);
         }
       eina_stringshare_del(source_file);
       eina_stringshare_del(dest_file);
    }
  eet_close(ef);
  return true;
}

Eina_Bool
pm_style_resource_export(Project *pro ,
                         Style *style,
                         Eina_Stringshare *path)
{
   Eina_List *l, *l_next, *parts, *state_list, *l_states, *tween_list, *l_tween;
   Eina_List *programs;

   Eina_List *images = NULL, *sounds = NULL, *fonts = NULL;

   Eina_Stringshare *dest, *source;
   const char *data_name, *state_name, *data;
   const char *state = NULL; double value = 0; char **state_split;
   Edje_Part_Type part_type = EDJE_PART_TYPE_NONE;
   Edje_Action_Type action_type = EDJE_ACTION_TYPE_NONE;

   if ((!pro) || (!style) || (!style->obj) || (!path)) return false;

   /* Collect lists with info needed resources */
   parts = edje_edit_parts_list_get(style->obj);
   EINA_LIST_FOREACH_SAFE(parts, l, l_next, data_name)
     {
        part_type = edje_edit_part_type_get(style->obj, data_name);
        switch (part_type)
          {
           case EDJE_PART_TYPE_IMAGE:
              state_list = edje_edit_part_states_list_get(style->obj, data_name);
              EINA_LIST_FOREACH(state_list, l_states, state_name)
                {
                  state_split = eina_str_split(state_name, " ", 2);
                  state = eina_stringshare_add(state_split[0]);
                  value = atof(state_split[1]);
                  free(state_split[0]);
                  free(state_split);

                  data = edje_edit_state_image_get(style->obj, data_name,
                                                   state, value);
                  if ((data) && (!eina_list_data_find(images, data)))
                    images = eina_list_append(images, eina_stringshare_add(data));
                  edje_edit_string_free(data);

                  tween_list = edje_edit_state_tweens_list_get(style->obj,
                                                               data_name,
                                                               state, value);
                  EINA_LIST_FOREACH(tween_list, l_tween, data)
                    {
                       if (!eina_list_data_find(images, data))
                         images = eina_list_append(images, eina_stringshare_add(data));
                    }
                  edje_edit_string_list_free(tween_list);
                }
              edje_edit_string_list_free(state_list);
           break;
           case EDJE_PART_TYPE_TEXT:
              state_list = edje_edit_part_states_list_get(style->obj, data_name);
              EINA_LIST_FOREACH(state_list, l_states, state_name)
                {
                  state_split = eina_str_split(state_name, " ", 2);
                  state = eina_stringshare_add(state_split[0]);
                  value = atof(state_split[1]);
                  free(state_split[0]);
                  free(state_split);

                  data = edje_edit_state_font_get(style->obj, data_name,
                                                  state, value);
                  if ((data) && (!eina_list_data_find(fonts, data)))
                    fonts = eina_list_append(fonts, eina_stringshare_add(data));
                  edje_edit_string_free(data);
                }
              edje_edit_string_list_free(state_list);
           break;
           case EDJE_PART_TYPE_TEXTBLOCK:
           break;
           default:
           break;
          }
     }
   edje_edit_string_list_free(parts);

   programs = edje_edit_programs_list_get(style->obj);
   EINA_LIST_FOREACH_SAFE(programs, l, l_next, data_name)
     {
        action_type = edje_edit_program_action_get(style->obj, data_name);
        if (action_type == EDJE_ACTION_TYPE_SOUND_SAMPLE)
          {
             data = edje_edit_program_sample_name_get(style->obj, data_name);
             if ((data) && (!eina_list_data_find(sounds, data)))
               sounds = eina_list_append(sounds, data);
          }
     }
   edje_edit_string_list_free(programs);

   source = eina_stringshare_printf("%s/images", pro->develop_path);
   dest = eina_stringshare_printf("%s/images", path);

   if (!_image_resources_export(images, dest, source, pro->dev, style->obj))
     WARN("Failed export images");
   eina_stringshare_del(source);
   eina_stringshare_del(dest);
   EINA_LIST_FREE(images, data)
     eina_stringshare_del(data);

   source = eina_stringshare_printf("%s/sounds", pro->develop_path);
   dest = eina_stringshare_printf("%s/sounds", path);
   if (!_sound_resources_export(sounds, dest, source, style->obj))
     ERR("Failed export sounds");
   eina_stringshare_del(source);
   eina_stringshare_del(dest);
   EINA_LIST_FREE(sounds, data)
     eina_stringshare_del(data);

   source = eina_stringshare_printf("%s/fonts", pro->develop_path);
   dest = eina_stringshare_printf("%s/fonts", path);
   if (!_font_resources_export(fonts, dest, source, pro->dev, style->obj))
     WARN("Failed export fonts");
   eina_stringshare_del(source);
   eina_stringshare_del(dest);
   EINA_LIST_FREE(fonts, data)
     eina_stringshare_del(data);

   return true;
}

Eina_Bool
pm_project_resource_export(Project *pro)
{
   Eina_List *list;
   Evas_Object *edje_edit_obj;
   Evas *e;
   Eina_Stringshare *dest;

   Ecore_Evas *ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   list = edje_file_collection_list(pro->dev);
   edje_edit_obj = edje_edit_object_add(e);
   if (!edje_object_mmap_set(edje_edit_obj, pro->mmap_file, eina_list_data_get(list)))
     {
        evas_object_del(edje_edit_obj);
        return false;
     }
   edje_edit_string_list_free(list);

   /* export images */
   list = edje_edit_images_list_get(edje_edit_obj);
   dest = eina_stringshare_printf("%s/images", pro->develop_path);
   _image_resources_export(list, dest, NULL, pro->dev, edje_edit_obj);
   edje_edit_string_list_free(list);
   eina_stringshare_del(dest);

   /* export fonts */
   list = edje_edit_fonts_list_get(edje_edit_obj);
   dest = eina_stringshare_printf("%s/fonts", pro->develop_path);
   _font_resources_export(list, dest, NULL, pro->dev, edje_edit_obj);
   edje_edit_string_list_free(list);
   eina_stringshare_del(dest);

   /* export sounds */
   list = edje_edit_sound_samples_list_get(edje_edit_obj);
   dest = eina_stringshare_printf("%s/sounds", pro->develop_path);
   _sound_resources_export(list, dest, NULL, edje_edit_obj);
   edje_edit_string_list_free(list);
   eina_stringshare_del(dest);

   ecore_evas_free(ee);

   return true;
}

Eina_Bool
pm_project_style_source_code_export(Project *pro, Style *style, const char *file)
{
   Eina_Stringshare *code = NULL;
   Eina_Stringshare *path = NULL;
   FILE *f;

   if (!style->obj) return false;

   if (file) path = eina_stringshare_add(file);
   else path = eina_stringshare_printf("%s/tmp.edc", pro->develop_path);
   f = fopen(path, "w");
   if (!f)
     {
        ERR("Could't open file '%s'", path);
        eina_stringshare_del(path);
        return false;
     }
   code = edje_edit_source_generate(style->obj);
   fputs(code, f);
   fclose(f);

   eina_stringshare_del(code);
   eina_stringshare_del(path);
   return true;
}

static void *
_develop_export(void *data,
                Eina_Thread *thread __UNUSED__)
{
   Project_Thread *worker;
   Evas_Object *edje_edit_obj;
   Eina_Stringshare *str;

   sleep(1);

   worker = (Project_Thread *)data;

   PROGRESS_SEND(_("Export edj..."));
   WORKER_LOCK_TAKE;
      if (worker->project->changed)
        _project_backup(worker);

      GET_OBJ(worker->project, edje_edit_obj);
      edje_edit_save_all(edje_edit_obj);
      str = eina_stringshare_add(worker->edj);
      if (!strstr(worker->edj, ".edj"))
        {
           str = eina_stringshare_printf("%s.edj", worker->edj);
           worker->edj = str;
        }
      eina_file_copy(worker->project->dev, worker->edj,
                     EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                     NULL, NULL);

   WORKER_LOCK_RELEASE;

   PROGRESS_SEND("Saved.");
   END_SEND(PM_PROJECT_SUCCESS);
   eina_stringshare_del(str);
   return NULL;
}

Project_Thread *
pm_project_develop_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   Project_Thread *worker;
   Eina_Bool result;

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, path, NULL, data);

   result = eina_thread_create(&worker->thread, EINA_THREAD_URGENT, -1,
                               (void *)_develop_export, worker);
   if (!result)
     WORKER_FREE();

   return worker;
}

#ifdef HAVE_ENVENTOR
static void *
_enventor_save(void *data,
               Eina_Thread *thread __UNUSED__)
{
   Project_Thread *worker;
   Ecore_Event_Handler *cb_exit = NULL,
                       *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   Eina_Stringshare *cmd, *edj, *options;
   Ecore_Exe *exe_cmd;
   pid_t exe_pid;

   sleep(1);

   worker = (Project_Thread *)data;

   cb_exit = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _exe_exit, worker);
   WORKER_LOCK_TAKE;
      if (worker->func_progress)
        {
           cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, worker);
           cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, worker);
        }
      edj = eina_stringshare_printf("%s/build.edj", worker->project->enventor.path);
      options = eina_stringshare_printf("-id %s/images -fd %s/fonts -sd %s/sounds -dd %s/data",
                                        worker->project->enventor.path,
                                        worker->project->enventor.path,
                                        worker->project->enventor.path,
                                        worker->project->enventor.path);
      cmd = eina_stringshare_printf("edje_cc -v %s %s %s", options,
                                    worker->project->enventor.file, edj);
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
   eina_stringshare_del(cmd);
   eina_stringshare_del(options);

   THREAD_TESTCANCEL;

   WORKER_LOCK_TAKE;
      worker->edj = eina_stringshare_printf("%s/enbuild.edj",
                                            worker->project->enventor.path);
      cmd = eina_stringshare_printf("edje_pick -o %s -a %s -i %s -g %s",
                                    worker->edj,
                                    worker->project->dev, edj,
                                    worker->project->current_style->full_group_name);
   WORKER_LOCK_RELEASE;

   DBG("Run command for compile: %s", cmd);
   exe_cmd = ecore_exe_pipe_run(cmd, flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   waitpid(exe_pid, NULL, 0);

   ecore_event_handler_del(cb_exit);
   if (worker->func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   WORKER_LOCK_TAKE;
      eina_file_copy(worker->edj, worker->project->dev,
                     EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                     NULL, NULL);
   WORKER_LOCK_RELEASE;

   eina_stringshare_del(edj);
   END_SEND(PM_PROJECT_SUCCESS)

   return NULL;
}

Project_Thread *
pm_project_enventor_save(Project *project,
                         PM_Project_Progress_Cb func_progress,
                         PM_Project_End_Cb func_end,
                         const void *data)
{
   Project_Thread *worker;
   Eina_Bool result;

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, NULL, NULL, NULL);

   result = eina_thread_create(&worker->thread, EINA_THREAD_URGENT, -1,
                               (void *)_enventor_save, worker);
   if (!result)
     WORKER_FREE();

   return worker;
}
#endif /* HAVE_ENVENTOR */
