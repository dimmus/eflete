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

/* Export develop edj file */
static Eina_Bool
_develop_edj_export_data_cb(void *data,
                     int type __UNUSED__,
                     void *event_info)
{
   Project_Thread *ptd = (Project_Thread *)data;
   Ecore_Exe_Event_Data *edje_pick_msg = (Ecore_Exe_Event_Data *)event_info;
   int i = 0;

   for (i = 0; edje_pick_msg->lines[i].line != NULL; i++)
      ptd->func_progress(NULL, edje_pick_msg->lines[i].line);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_develop_edj_export_del_cb(void *data,
                     int type __UNUSED__,
                     void *event_info)
{
   Project_Thread *ptd = (Project_Thread *)data;
   Ecore_Exe_Event_Del *edje_pick_exit = (Ecore_Exe_Event_Del *)event_info;
   Eina_Stringshare *msg = NULL;

   if (edje_pick_exit->exit_code != 0)
     {
        msg = eina_stringshare_printf(_("Export to file '%s' FAILED"), ptd->path);
        ptd->func_progress(NULL, msg);
        eina_stringshare_del(msg);

        ptd->result = PM_PROJECT_ERROR;
        _end_send(ptd);
        return ECORE_CALLBACK_DONE;
     }

   if (ptd->tmp_dirname)
     {
        ecore_file_recursive_rm(ptd->path);
        ecore_file_mv(ptd->tmp_dirname, ptd->path);
        eina_stringshare_del(ptd->tmp_dirname);
        ptd->tmp_dirname = NULL;
     }
   msg = eina_stringshare_printf(_("Export to file '%s' successfull"), ptd->path);
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

  ptd->result = PM_PROJECT_SUCCESS;
  _end_send(ptd);

  return ECORE_CALLBACK_DONE;
}

void
_develop_export(Project_Thread *ptd)
{
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   Eina_Stringshare *cmd;
   Eina_List *l;
   Group *group;

   CRIT_ON_FAIL(editor_save_all(ptd->project->global_object));
   Eina_Stringshare *msg = eina_stringshare_printf(_("Export to file '%s'"), ptd->path);
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

   if (!ecore_file_exists(ptd->path))
     cmd = eina_stringshare_printf("edje_pick -o %s", ptd->path);
   else
     {
        eina_file_mkstemp("eflete_export_XXXXXX", &ptd->tmp_dirname);
        cmd = eina_stringshare_printf("edje_pick -o %s", ptd->tmp_dirname);
        cmd = eina_stringshare_printf("%s -a %s", cmd, ptd->path);
     }
   cmd = eina_stringshare_printf("%s -i %s", cmd, ptd->project->dev);

   EINA_LIST_FOREACH(ptd->project->groups, l, group)
     {
        cmd = eina_stringshare_printf("%s -g %s", cmd, group->name);
     }
   DBG("Run command for export: %s", cmd);
   ecore_exe_pipe_run(cmd, flags, NULL);

   ptd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _develop_edj_export_data_cb, ptd);
   ptd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _develop_edj_export_data_cb, ptd);
   ptd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _develop_edj_export_del_cb, ptd);

   return;
}

/* Export release edj file */
static Eina_Bool
_data_from_edje_cc(void *data,
                   int type __UNUSED__,
                   void *event_info)
{
   int i = 0;
   Ecore_Exe_Event_Data *edje_cc_msg = (Ecore_Exe_Event_Data *)event_info;
   Edje_Exe_Data *edje_cc_data = (Edje_Exe_Data*)data;
   Project_Thread *ptd = (Project_Thread *)edje_cc_data->data;
   if (!edje_cc_msg) return ECORE_CALLBACK_DONE;

   for (i = 0; edje_cc_msg->lines[i].line != NULL; i++)
         ptd->func_progress(NULL, edje_cc_msg->lines[i].line);

   return ECORE_CALLBACK_DONE;
}

static unsigned char
_finish_from_release_edje_cc(void *data,
                             int type __UNUSED__,
                             void *event_info)
{
   Edje_Exe_Data *edje_cc_data = (Edje_Exe_Data *)data;
   Project_Thread *ptd = (Project_Thread *)edje_cc_data->data;
   Ecore_Exe_Event_Del *edje_cc_exit = (Ecore_Exe_Event_Del *)event_info;

   eina_stringshare_del(edje_cc_data->cmd);
   free(edje_cc_data);
   if (edje_cc_exit->exit_code != 0)
     {
        ptd->result = PM_PROJECT_ERROR;
        _end_send(ptd);
        return ECORE_CALLBACK_DONE;
     }

   Eina_Stringshare *msg = eina_stringshare_printf(_("Data for importing prepared"));
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

   Eina_Stringshare *edj = eina_stringshare_printf("%s/out.edj", ptd->tmp_dirname);
   ecore_file_cp(edj, ptd->tmp_path);
   ptd->result = PM_PROJECT_SUCCESS;
   _end_send(ptd);

   return ECORE_CALLBACK_DONE;
}

static void
_project_source_code_release_export_end_cb(void *data,
                                           Ecore_Thread *th __UNUSED__)
{
   Project_Thread *ptd = (Project_Thread *)data;

   Eina_Stringshare *msg = eina_stringshare_printf(_("Start import '%s' file as new project"), ptd->edc);
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

   ptd->edc = eina_stringshare_printf("%s/%s/generated.edc", ptd->tmp_dirname, ptd->project->name);
   ptd->build_options = eina_stringshare_printf("-id \"%s/%s/images\" "
                                                "-sd \"%s/%s/sounds\" "
                                                "-fd \"%s/%s/fonts\" "
                                                "-v",
                                                ptd->tmp_dirname, ptd->project->name,
                                                ptd->tmp_dirname, ptd->project->name,
                                                ptd->tmp_dirname, ptd->project->name);

   ptd->edj = eina_stringshare_printf("%s/out.edj", ptd->tmp_dirname);
   Edje_Exe_Data *edje_cc_data = mem_malloc(sizeof(Edje_Exe_Data));
   edje_cc_data->cmd = eina_stringshare_printf("edje_cc -v %s %s %s",
                                               ptd->edc,
                                               ptd->edj,
                                               ptd->build_options);
   edje_cc_data->flags  = ECORE_EXE_PIPE_READ |
      ECORE_EXE_PIPE_READ_LINE_BUFFERED |
      ECORE_EXE_PIPE_ERROR |
      ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   edje_cc_data->data = (void *)ptd;

   edje_cc_data->exe_cmd = ecore_exe_pipe_run(edje_cc_data->cmd, edje_cc_data->flags, NULL);

   ptd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _data_from_edje_cc, edje_cc_data);
   ptd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _data_from_edje_cc, edje_cc_data);
   ptd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _finish_from_release_edje_cc, edje_cc_data);
}

void
_release_export(Project_Thread *ptd)
{
   Eina_Stringshare *msg = eina_stringshare_printf(_("Export to file '%s'"), ptd->edj);
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

   eina_file_mkdtemp("eflete_build_XXXXXX", &ptd->tmp_dirname);
   ptd->tmp_path = eina_stringshare_add(ptd->path);
   ptd->path = eina_stringshare_add(ptd->tmp_dirname);
   ecore_thread_feedback_run(_project_source_code_export_feedback_job, _project_source_code_export_feedback_cb,
                             _project_source_code_release_export_end_cb, _project_source_code_export_cancel_cb, ptd,
                             true);
}
