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
#include "project_manager.h"
#include "project_manager_private.h"

void
_project_open_cancel_cb(void *data __UNUSED__, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ERR("Project opening process canceled");

   /*Procedure of correct freing resources */
   _pm_project_descriptor_shutdown(ptd);

   ptd->result = PM_PROJECT_ERROR;
   _end_send(ptd);
}

void
_project_open_end_cb(void *data, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   _project_open_internal(ptd);
}

void
_project_open_feedback_cb(void *data,
                          Ecore_Thread *th __UNUSED__,
                          void *msg_data)
{
   Project_Thread *ptd = (Project_Thread *)data;
   Eina_Stringshare *message = (Eina_Stringshare *)msg_data;
   assert(message != NULL);
   assert(ptd != NULL);

   ptd->func_progress(NULL, message);
   eina_stringshare_del(message);
}

void
_project_open_feedback_job(void *data, Ecore_Thread *th)
{
   Project_Thread *ptd = (Project_Thread *)data;
   if (!eina_lock_take(&ptd->mutex))
     {
       ERR("Failed access to the thread data");
       ecore_thread_cancel(th);
       return;
     }

   Eet_File *ef;
   char *tmp;
   int tmp_len;
   Eina_Stringshare *message = NULL;
#ifdef _WIN32
    HFILE pro_fd = -1;
#else
    int pro_fd;
#endif
   assert(ptd->path != NULL);

   edje_file_cache_flush();

   if (!_lock_try(ptd->path, true,  &pro_fd))
     {
        /* really this case is unlickly, but we need handle it */
        eina_lock_release(&ptd->mutex);
        ERR("Project file already locked by another application");
        ecore_thread_cancel(th);
        return;
     }

   message = eina_stringshare_printf(_("Opening project \"%s\""), ptd->path);
   ecore_thread_feedback(th, message);

   _project_descriptor_init(ptd);
   ef = eet_open(ptd->path, EET_FILE_MODE_READ_WRITE);
   if (!ef)
     {
        _pm_project_descriptor_shutdown(ptd);
        eina_lock_release(&ptd->mutex);
        ERR("Failed to open project file handler");
        ecore_thread_cancel(th);
        return;
     }

   message = eina_stringshare_add(_("Reading project descriptor"));
   ecore_thread_feedback(th, message);

   ptd->project = eet_data_read(ef, ptd->eed_project, PROJECT_FILE_KEY);
   _pm_project_descriptor_shutdown(ptd);
   if (!ptd->project)
     {
        eina_lock_release(&ptd->mutex);
        ERR("Failed to load project data");
        ecore_thread_cancel(th);
        return;
     }

   ptd->project->ef = ef;
   ptd->project->pro_path = eina_stringshare_add(ptd->path);
   ptd->project->pro_fd = pro_fd;

   /* updating .dev file path */
   tmp = strdup(ptd->path);
   tmp_len = strlen(tmp);
   tmp[tmp_len - 3] = 'd';
   tmp[tmp_len - 2] = 'e';
   tmp[tmp_len - 1] = 'v';
   eina_stringshare_replace(&ptd->project->dev, tmp);
   free(tmp);
   /* updating .edj file path */
   tmp = strdup(ptd->path);
   tmp[tmp_len - 3] = 'e';
   tmp[tmp_len - 2] = 'd';
   tmp[tmp_len - 1] = 'j';
   eina_stringshare_replace(&ptd->project->saved_edj, tmp);
   free(tmp);

   message = eina_stringshare_add(_("Checking project version"));
   ecore_thread_feedback(th, message);

   /* checking for older project versions and upgrading them version-by-version */
   if (ptd->project->version < 2) /* upgrade to version 2 */
     {
        message = eina_stringshare_add(_("Updating project files to version 2"));
        ecore_thread_feedback(th, message);
        INFO("Updating project files to version 2");
        ecore_file_mv(ptd->project->dev, ptd->project->saved_edj);
        ptd->project->version = 2;
     }
   if (ptd->project->version < 3) /* upgrade to version 3 */
     {
        message = eina_stringshare_add(_("Updating project files to version 3"));
        ecore_thread_feedback(th, message);
        INFO("Updating project files to version 3");
        _project_special_group_add(ptd->project);
        ptd->project->version = 3;
     }
   if (ptd->project->version < 4) /* upgrade to version 4 */
     {
        message = eina_stringshare_add(_("Updating project files to version 4"));
        ecore_thread_feedback(th, message);
        INFO("Updating project files to version 4");
        _project_dummy_image_add(ptd->project);
        ptd->project->version = 4;
     }
   if (ptd->project->version < 5) /* upgrade to version 4 */
     {
        message = eina_stringshare_add(_("Updating project files to version 5"));
        ecore_thread_feedback(th, message);
        INFO("Updating project files to version 5");
        _project_dummy_sample_add(ptd->project);
        ptd->project->version = 5;
     }
    TODO("Add crash recovery prompt here")

   pm_project_meta_data_get(ptd->project, &ptd->project->name, NULL, NULL, NULL, NULL);
   if (!ptd->project->name)
     ptd->project->name = ecore_file_strip_ext(ecore_file_file_get(ptd->project->dev));

   eina_lock_release(&ptd->mutex);
}

