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

/* Export source from a project */

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
   Eina_Stringshare *path;
   Project_Thread *ptd = (Project_Thread *)data;
   if (!eina_lock_take(&ptd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }

   Eina_Stringshare *message = eina_stringshare_printf(_("Generate source code ..."));
   ecore_thread_feedback(th, message);

   if (!ap.path.export_edc.folder)
     path  =  eina_stringshare_printf("%s/%s", ptd->path, ptd->project->name);
   else
     path  =  eina_stringshare_printf("%s/%s", ptd->path, ap.path.export_edc.folder);

   char *code;
   Eina_Strbuf *buf;
   FILE *f;

   buf = eina_strbuf_new();
   /* create a folder for collect the source files */
   ecore_file_mkdir(path);
   eina_strbuf_reset(buf);

   /* create and open edc file for print the source code of collection (project) */
   if (!ap.path.export_edc.file)
     eina_strbuf_append_printf(buf, "%s/generated.edc", path);
   else
     eina_strbuf_append_printf(buf, "%s/%s", path, ap.path.export_edc.file);
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



/* Export source from a project */
void
_group_source_code_export_cancel_cb(void *data, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ptd->result = PM_PROJECT_ERROR;
   _end_send(ptd);
}

void
_group_source_code_export_end_cb(void *data, Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;
   ptd->result = PM_PROJECT_SUCCESS;
   _end_send(ptd);
}

void
_group_source_code_export_feedback_cb(void *data,
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
_group_source_code_export_feedback_job(void *data, Ecore_Thread *th)
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

   Eina_Stringshare *code;
   FILE *f;
   Group *group = ptd->group;
   Part *part;
   State *state;
   Eina_List *l, *ls;
   Eina_Stringshare *resource;
   Resource *res;
   Eina_Strbuf *buf;
   char *name;

   message = eina_stringshare_printf(_("Generate group '%s' source code ..."), group->name);
   ecore_thread_feedback(th, message);

   name = strdup(group->name);
   string_char_replace(name, '/', '_');
   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s/%s", ptd->path, name);
   ecore_file_mkdir(eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);

   /* create and open file for collect the group source code */
   eina_strbuf_append_printf(buf, "%s/%s/generated.edc", ptd->path, name);
   f = fopen(eina_strbuf_string_get(buf), "w");
   if (!f)
     {
        ERR("Could't open file '%s'", eina_strbuf_string_get(buf));
        eina_lock_release(&ptd->mutex);
        ecore_thread_cancel(th);
        eina_strbuf_free(buf);
        free(name);
        return;
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
             message = eina_stringshare_printf(_("Export images ..."));
             ecore_thread_feedback(th, message);

             eina_strbuf_reset(buf);
             eina_strbuf_append_printf(buf, "%s/%s/images/", ptd->path, name);
             EINA_LIST_FOREACH(part->states, ls, state)
               {
                  resource = edje_edit_state_image_get(group->edit_object, part->name, state->name, state->val);
                  message = eina_stringshare_printf(_("Export image '%s'"), resource);
                  ecore_thread_feedback(th, message);

                  _external_resource_export(ptd->project->images, resource, eina_strbuf_string_get(buf));
                  eina_stringshare_del(resource);
               }
          }
        if (part->type == EDJE_PART_TYPE_TEXT)
          {
             message = eina_stringshare_printf(_("Export fonts ..."));
             ecore_thread_feedback(th, message);

             eina_strbuf_reset(buf);
             eina_strbuf_append_printf(buf, "%s/%s/fonts/", ptd->path, name);
             EINA_LIST_FOREACH(part->states, ls, state)
               {
                  resource = edje_edit_state_font_get(group->edit_object, part->name, state->name, state->val);
                  Eina_Stringshare *font_res = edje_edit_font_path_get(group->edit_object, resource);

                  message = eina_stringshare_printf(_("Export font '%s'"), font_res);
                  ecore_thread_feedback(th, message);

                  _external_resource_export(ptd->project->fonts, font_res, eina_strbuf_string_get(buf));
                  eina_stringshare_del(resource);
                  eina_stringshare_del(font_res);
               }
          }
     }
   message = eina_stringshare_printf(_("Export sounds ..."));
   ecore_thread_feedback(th, message);

   EINA_LIST_FOREACH(group->programs, l, res)
     {
        resource = edje_edit_program_sample_name_get(group->edit_object, res->name);
        if (!resource) continue;

        eina_strbuf_reset(buf);
        eina_strbuf_append_printf(buf, "%s/%s/sounds/", ptd->path, name);
        message = eina_stringshare_printf(_("Export sound '%s'"), res->name);
        ecore_thread_feedback(th, message);

        _external_resource_export(ptd->project->sounds, resource, eina_strbuf_string_get(buf));
        eina_stringshare_del(resource);
     }

   eina_strbuf_reset(buf);
   eina_strbuf_append_printf(buf, "%s/%s/build.sh", ptd->path, name);
   _build_script_write(eina_strbuf_string_get(buf));
   eina_lock_release(&ptd->mutex);
   eina_strbuf_free(buf);
   free(name);
   return;
}


