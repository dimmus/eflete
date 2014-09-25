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

#include "project_manager_new.h"
#include "alloc.h"

#define PROJECT_FILE_KEY      "project"
#define PROJECT_DATA_KEY      "project_data"

#define PM_RESOURCE_TAKE      eina_lock_take(&input->mutex)
#define PM_RESOURCE_RELEASE   eina_lock_release(&input->mutex)

struct _Input
{
   const char *name;
   const char *path;
   const char *edc;
   const char *edj;
   const char *image_directory;
   const char *sound_directory;
   const char *font_directory;
   const char *data_directory;
   const char *vibration_directory;
   PM_Project_Progress_Cb func_progress;
   PM_Project_Cb func_end;
   void *data;
   Eina_Lock mutex;
};

typedef struct _Input Input;

static Eet_Data_Descriptor *eed_project = NULL;

static Input *
_pm_input_new(void)
{
   Input *input;

   input = (Input *)mem_calloc(1, sizeof(Input));
   eina_lock_new(&input->mutex);

   return input;
}

static void
_pm_input_free(Input *input)
{
   eina_lock_free(&input->mutex);

   eina_stringshare_del(input->name);
   eina_stringshare_del(input->path);
   eina_stringshare_del(input->edc);
   eina_stringshare_del(input->image_directory);
   eina_stringshare_del(input->sound_directory);
   eina_stringshare_del(input->font_directory);
   eina_stringshare_del(input->data_directory);
   eina_stringshare_del(input->vibration_directory);

   free(input);
}

static void
_pm_project_descriptor_init(void)
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

static Eina_Bool
_copy_file_progress_cb(void *data,
                       unsigned long long done,
                       unsigned long long total)
{
   short int percentage;
   Eina_Stringshare *message;
   PM_Project_Thread *pm_thread;

   pm_thread = (PM_Project_Thread *)data;
   percentage = (short int)(done / total) * 100;
   message = eina_stringshare_printf("%u", percentage);
   ecore_thread_feedback(pm_thread, message);

   return true;
}

static void
_pm_project_import_edj(void *data, PM_Project_Thread *pm_thread)
{
   Eina_Stringshare *folder_path, *pro_path;
   Eina_Stringshare *message;
   Project *pro;
   Input *input;

   input = (Input *)data;
   _pm_project_descriptor_init();

   PM_RESOURCE_TAKE;
   folder_path = eina_stringshare_printf("%s/%s", input->path, input->name);
   PM_RESOURCE_RELEASE;
   ecore_file_mkdir(folder_path);
   message = eina_stringshare_printf(_("Create the project folder: %s"), folder_path);
   ecore_thread_feedback(pm_thread, message);

   pro = (Project *)mem_malloc(sizeof(Project));
   PM_RESOURCE_TAKE;
   pro->dev = eina_stringshare_printf("%s/%s.dev", folder_path, input->name);
   pro->develop_path = eina_stringshare_printf("%s/develop/%s.edj",
                                               folder_path, input->name);
   pro->release_path = eina_stringshare_printf("%s/release/%s.edj",
                                               folder_path, input->name);
   pro->release_options = NULL;

   pro_path = eina_stringshare_printf("%s/%s.pro", folder_path, input->name);
   PM_RESOURCE_RELEASE;
   if (!_pm_project_descriptor_data_write(pro_path, pro))
     goto error;


   eina_file_copy(pro->dev, input->edj,
                  EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                  _copy_file_progress_cb, (void *)pm_thread);

   pro->widgets = wm_widget_list_new(pro->dev);
   pro->layouts = wm_widget_list_layouts_load(pro->dev);
   pro->is_saved = true;
   PM_RESOURCE_RELEASE;

   ecore_thread_local_data_add(pm_thread, PROJECT_DATA_KEY,
                               (void *)pro, NULL, true);
error:
   _pm_project_descriptor_shutdown();
   eina_stringshare_del(folder_path);
   eina_stringshare_del(pro_path);
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




static void
_pm_project_notify_cb(void *data,
                      PM_Project_Thread *pm_thread __UNUSED__,
                      void *msg_data)
{
   Input *input;
   Eina_Stringshare *message;

   input = (Input *)data;
   message = (Eina_Stringshare *)msg_data;

   PM_RESOURCE_TAKE;
   input->func_progress(input->data, message, NULL);
   PM_RESOURCE_RELEASE;
}

static void
_pm_project_end_cb(void *data,
                   PM_Project_Thread *pm_thread)
{
   Input *input;
   Project *pro;

   input = (Input *)data;

   pro = (Project *)ecore_thread_local_data_find(pm_thread, PROJECT_DATA_KEY);
   input->func_end(input->data, PM_PROJECT_SUCCESS, pro);

   _pm_input_free(input);
}

static void
_pm_project_cancel_cb(void *data,
                      PM_Project_Thread *pm_thread __UNUSED__)
{
   Input *input;

   input = (Input *)data;
   _pm_input_free(input);
}

/*************************************/
/*             PUBLIC API            */
/*************************************/

Eina_Bool
pm_project_job_stop(PM_Project_Thread *pm_process)
{
   return ecore_thread_cancel((Ecore_Thread *)pm_process);
}

PM_Project_Thread *
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_Cb func_end,
                      void *data)
{
   Input *input;
   PM_Project_Thread *import_process;

   input = _pm_input_new();

   PM_RESOURCE_TAKE;
   input->name = eina_stringshare_add(name);
   input->path = eina_stringshare_add(path);
   input->edj = eina_stringshare_add(edj);
   input->func_progress = func_progress;
   input->func_end = func_end;
   input->data = data;
   PM_RESOURCE_RELEASE;

   import_process = ecore_thread_feedback_run(_pm_project_import_edj,
                                              _pm_project_notify_cb,
                                              _pm_project_end_cb,
                                              _pm_project_cancel_cb,
                                              (void *)input,
                                              false);

   return import_process;
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
