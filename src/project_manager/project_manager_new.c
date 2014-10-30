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

#define PROJECT_FILE_KEY      "project"
#define THREAD_TESTCANCEL pthread_testcancel()

#define WORKER_CREATE(FUNC_PROGRESS, FUNC_END, DATA, \
                      NAME, PATH, EDJ, BUILD_OPTIONS) \
{ \
   worker = (Project_Thread *)mem_malloc(sizeof(Project_Thread)); \
   worker->func_progress = FUNC_PROGRESS; \
   worker->func_end = FUNC_END; \
   worker->data = DATA; \
   worker->project = NULL; \
   worker->name = eina_stringshare_add(NAME); \
   worker->path = eina_stringshare_add(PATH); \
   worker->edj = eina_stringshare_add(EDJ); \
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
_project_dev_file_copy(Project_Thread *worker)
{
   Eina_Bool result;

   WORKER_LOCK_TAKE;
      result = eina_file_copy(worker->edj, worker->project->dev,
                              EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                              NULL, NULL);
   WORKER_LOCK_RELEASE;

   DBG("Copy the .dev file to project folder.");
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
   worker->project = _project_files_create(worker);
   THREAD_TESTCANCEL;
   _project_dev_file_copy(worker);

   return worker;
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
                 name, path, edj, NULL);

   result = eina_thread_create(&worker->thread, EINA_THREAD_URGENT, -1,
                               (void *)_project_import_edj, worker);
   if (!result)
     WORKER_FREE();

   return worker;
}

Project *
pm_project_import_edc(const char *name __UNUSED__,
                      const char *path __UNUSED__,
                      const char *edc __UNUSED__,
                      const char *image_directory __UNUSED__,
                      const char *sound_directory __UNUSED__,
                      const char *font_directory __UNUSED__,
                      const char *data_directory __UNUSED__,
                      const char *vibration_directory __UNUSED__);

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
