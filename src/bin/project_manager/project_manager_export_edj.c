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
_develop_edj_export_data_cb(void *data __UNUSED__,
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
                     void *event_info __UNUSED__)
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

