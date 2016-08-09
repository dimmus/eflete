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

/* Export source from a single group */

void
_project_source_code_export_cancel_cb(void *data, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ptd->result = PM_PROJECT_ERROR;
   _end_send(ptd);
}

void
_project_source_code_export_end_cb(void *data,
                                   Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ptd->result = PM_PROJECT_SUCCESS;
   _end_send(ptd);
}

void
_project_source_code_export_feedback_cb(void *data,
                                        Ecore_Thread *th __UNUSED__,
                                        void *msg_data)
{
   Project_Thread *ptd = (Project_Thread *)data;
   Eina_Stringshare *message = (Eina_Stringshare *)msg_data;
   assert(message != NULL);

   ptd->func_progress(NULL, message);
   eina_stringshare_del(message);
}

void
_project_source_code_export_feedback_job(void *data, Ecore_Thread *th)
{
   Project_Thread *ptd = (Project_Thread *)data;
   if (!eina_lock_take(&ptd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }

   Eina_Stringshare *message = eina_stringshare_printf(_("Generate source code ..."));
   ecore_thread_feedback(th, message);

   Eina_Stringshare *path  =  eina_stringshare_printf("%s/%s", ptd->path, ptd->project->name);

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
        ERR("Could't open file '%s'", eina_strbuf_string_get(buf));
        eina_strbuf_free(buf);
        eina_lock_release(&ptd->mutex);
        ecore_thread_cancel(th);
        return;
     }
   eina_strbuf_reset(buf);

   /* get the full source code of given project */
   TODO("Move this into thread, because it is really laggy");
   ecore_thread_main_loop_begin();
   code = edje_edit_full_source_generate(ptd->project->global_object);
   ecore_thread_main_loop_end();

   fputs(code, f);
   free(code);
   if (f) fclose(f);

   /* export resource */
   if (ptd->project->images)
     {
        eina_strbuf_append_printf(buf, "%s/images/", path);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        _external_resources_export(ptd->project->images, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
     }
   if (ptd->project->sounds)
     {
        eina_strbuf_append_printf(buf, "%s/sounds/", path);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        _external_resources_export(ptd->project->sounds, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
     }
   if (ptd->project->fonts)
     {
        eina_strbuf_append_printf(buf, "%s/fonts/", path);
        ecore_file_mkdir(eina_strbuf_string_get(buf));
        _external_resources_export(ptd->project->fonts, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
     }

   eina_strbuf_reset(buf);
   eina_strbuf_append_printf(buf, "%s/build.sh", path);
   _build_script_write(eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   eina_lock_release(&ptd->mutex);
}
