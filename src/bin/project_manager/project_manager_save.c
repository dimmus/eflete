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
_project_save_cancel_cb(void *data, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ERR("Project saving process canceled");

   ptd->result = PM_PROJECT_ERROR;
   _end_send(ptd);
}

void
_project_save_end_cb(void *data, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ptd->result = PM_PROJECT_ERROR;
   _end_send(ptd);
}

void
_project_save_feedback_cb(void *data,
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
_project_save_feedback_job(void *data, Ecore_Thread *th)
{
   Project_Thread *ptd = (Project_Thread *)data;
   if (!eina_lock_take(&ptd->mutex))
     {
       ERR("Failed access to the thread data");
       eina_lock_release(&ptd->mutex);
       ecore_thread_cancel(th);
       return;
     }

   assert(ptd->project != NULL);
   Eina_Stringshare *message = eina_stringshare_printf(_("Save project '%s'"), ptd->project->name);
   ecore_thread_feedback(th, message);

   if (!editor_save_all(ptd->project->global_object))
     {
        ERR("Failed to save project.");
        eina_lock_release(&ptd->mutex);
        ecore_thread_cancel(th);
        return;
     }

   ecore_file_cp(ptd->project->dev, ptd->project->saved_edj);

   message = eina_stringshare_printf(_("Save done."));
   ecore_thread_feedback(th, message);

   eina_lock_release(&ptd->mutex);
   return;
}
