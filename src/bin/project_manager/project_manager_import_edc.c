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

static Eina_Bool
_finish_from_edje_cc(void *data,
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

   if (ptd->func_progress)
     {
        Eina_Stringshare *msg = eina_stringshare_printf(_("Data for importing prepared"));
        ptd->func_progress(NULL, msg);
        eina_stringshare_del(msg);
     }

   _project_import_edj(ptd);

   return ECORE_CALLBACK_DONE;
}

void
_project_import_edc(Project_Thread *ptd)
{
   assert(ptd != NULL);

   Eina_Stringshare *msg = eina_stringshare_printf(_("Start import '%s' file as new project"), ptd->edc);
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

   eina_file_mkdtemp("eflete_build_XXXXXX", &ptd->tmp_dirname);
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
   edje_cc_data->exe_pid = ecore_exe_pid_get(edje_cc_data->exe_cmd);

   ptd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _data_from_edje_cc, edje_cc_data);
   ptd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _data_from_edje_cc, edje_cc_data);
   ptd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _finish_from_edje_cc, edje_cc_data);

   return;
}
