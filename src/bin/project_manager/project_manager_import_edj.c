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

static Eet_Compression compess_level = EET_COMPRESSION_HI;

#define MKDIR(NAME) \
   tmp = eina_stringshare_printf("%s/"#NAME, pro->develop_path); \
   ecore_file_mkdir(tmp); \
   pro->res.NAME = eina_list_append(pro->res.NAME, eina_stringshare_add(tmp)); \
   eina_stringshare_del(tmp)

static Project *
_project_files_create(Project_Thread *ptd)
{
   Project *pro;
   Eina_Stringshare *folder_path, *pro_path, *tmp;
   Eina_Bool error = false;

   _project_descriptor_init(ptd);

   folder_path = eina_stringshare_printf("%s/%s", ptd->path, ptd->name);
   if (ecore_file_mkdir(folder_path))
     {
        DBG("Create the folder '%s' for new project '%s'", folder_path, ptd->name);
     }
   else
     {
        ERR("Could't create a project folder!");
        error = true;
     }
   eina_stringshare_del(folder_path);
   if (error) return NULL;

   pro = (Project *)mem_calloc(1, sizeof(Project));
   folder_path = eina_stringshare_printf("%s/%s", ptd->path, ptd->name);
   pro->version = PROJECT_FILE_VERSION;
   pro->name = eina_stringshare_add(ptd->name);
   pro->dev = eina_stringshare_printf("%s/%s.dev", folder_path, ptd->name);
   pro->saved_edj = eina_stringshare_printf("%s/%s.edj", folder_path, ptd->name);
   pro->develop_path = eina_stringshare_printf("%s/develop", folder_path);

   pro_path = eina_stringshare_printf("%s/%s.pro", folder_path, ptd->name);
   pro->ef = eet_open(pro_path, EET_FILE_MODE_READ_WRITE);
   ecore_file_mkdir(pro->develop_path);
   MKDIR(images);
   MKDIR(sounds);
   MKDIR(fonts);
   eina_stringshare_del(folder_path);

   if (!eet_data_write(pro->ef, ptd->eed_project, PROJECT_FILE_KEY, pro, compess_level))
     error = true;

   DBG("Create a specific project file '%s': %s", pro_path, error ? "failed" : "success");
   _pm_project_descriptor_shutdown(ptd);
   eina_stringshare_del(pro_path);
   if (error)
     {
        ERR("Could't create a .pro file! ")
        eina_stringshare_del(pro->name);
        eina_stringshare_del(pro->dev);
        eina_stringshare_del(pro->saved_edj);
        eina_stringshare_del(pro->develop_path);
        free(pro);
        pro = NULL;
     }
   else
     eet_sync(pro->ef);

   return pro;
}
#undef MKDIR


static Eina_Bool
_data_from_edje_pick_cb(void *data,
                        int type __UNUSED__,
                        void *event_info)
{
   int i = 0;
   Edje_Exe_Data *edje_pick_data = (Edje_Exe_Data *)data;
   assert(edje_pick_data != NULL);
   Project_Thread *ptd = (Project_Thread *)edje_pick_data->data;
   assert(ptd->func_progress != NULL);

   Ecore_Exe_Event_Data *edje_pick_msg = (Ecore_Exe_Event_Data *)event_info;
   if (!edje_pick_msg) return ECORE_CALLBACK_DONE;

   for (i = 0; edje_pick_msg->lines[i].line != NULL; i++)
      ptd->func_progress(NULL, edje_pick_msg->lines[i].line);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_edje_pick_end_cb(void *data,
                  int type __UNUSED__,
                  void *event_info)
{
   Edje_Exe_Data *edje_pick_data = (Edje_Exe_Data *)data;
   assert(edje_pick_data != NULL);
   Project_Thread *ptd = (Project_Thread *)edje_pick_data->data;
   assert(ptd->func_progress != NULL);
   Ecore_Exe_Event_Del *edje_pick_exit = (Ecore_Exe_Event_Del *)event_info;

   eina_stringshare_del(edje_pick_data->cmd);
   free(edje_pick_data);

   if (edje_pick_exit->exit_code != 0)
     {
        ptd->result = PM_PROJECT_ERROR;
        ecore_file_unlink(ptd->source_edj);
        _end_send(ptd);
        return ECORE_CALLBACK_DONE;
     }

   _project_edj_file_copy(ptd);
   _copy_meta_data_to_pro(ptd);
   _project_special_group_add(ptd->project);
   _project_dummy_image_add(ptd->project);
   _project_open_internal(ptd);
   ecore_file_unlink(ptd->source_edj);

   return ECORE_CALLBACK_DONE;
}

void
_project_import_edj(void *data)
{
   Project_Thread *ptd = (Project_Thread *)data;

   Eina_Stringshare *edj_in, *edj_out;
   Eina_List *l, *groups;
   Eina_Stringshare *group;
   Evas_Object *obj = NULL;
   Eina_Strbuf *strbuf;
   Edje_Exe_Data *edje_pick_data;

   Eina_Stringshare *msg = eina_stringshare_printf(_("Start import '%s' file as new project"), ptd->edj);
   ptd->func_progress(NULL, msg);
   eina_stringshare_del(msg);

   /* Replace void with ptd */
   ptd->project = _project_files_create(ptd);
   assert(ptd->project != NULL);
   TODO("Add correct error handling here (if project == NULL). Probably we should add negative TC where directory already exist");
   ptd->project->pro_path = eina_stringshare_printf("%s/%s/%s.pro", ptd->path, ptd->name, ptd->name);

   if (!_lock_try(ptd->project->pro_path, true))
     {
        /* really this case is unlickly, but we need handle it */
        ptd->result = PM_PROJECT_LOCKED;
        _end_send(ptd);
        return;
     }
   groups = edje_file_collection_list(ptd->edj);

   if (ptd->widgets && (eina_list_count(groups) != eina_list_count(ptd->widgets)))
     {
        msg = eina_stringshare_printf(_("Merging groups from choosen file"));
        ptd->func_progress(NULL, msg);
        eina_stringshare_del(msg);

        eina_file_mkdtemp("eflete_build_XXXXXX", &ptd->tmp_dirname);
        edj_in = eina_stringshare_printf("%s/in.edj", ptd->tmp_dirname);
        edj_out = eina_stringshare_printf("%s/out.edj", ptd->tmp_dirname);
        ecore_file_cp(ptd->edj, edj_in);

        /* prepare the cmd string for run edje_pick */
        strbuf = eina_strbuf_new();
        eina_strbuf_append_printf(strbuf, "edje_pick -o %s -i %s", edj_out, edj_in);

        /* load any group for coping */
        if (ptd->widgets)
          {
             obj = edje_edit_object_add(evas_object_evas_get(ap.win));
             if (!edje_object_file_set(obj, edj_in, eina_list_data_get(ptd->widgets)))
               {
                  CRIT("Can't load object");
                  abort();
               }
          }
        EINA_LIST_FOREACH(ptd->widgets, l, group)
          {
             if ((group[0] == 'c') && (group[1] == 'p') && (group[2] == '*') && (group[3] == '*') && (group[4] == '*'))
               {
                  char **arr = eina_str_split(group, "***", 0);
                  you_shall_not_pass_editor_signals(NULL);
                  if (!editor_group_copy(obj, arr[1], arr[2]))
                    {
                       CRIT("Can not copy group %s, to %s", arr[1], arr[2]);
                       abort();
                    }
                  you_shall_not_pass_editor_signals(NULL);
                  eina_strbuf_append_printf(strbuf, " -g %s", arr[2]);
                  free(arr[0]);
                  free(arr);
               }
             else
               eina_strbuf_append_printf(strbuf, " -g %s", group);
          }

        eina_stringshare_del(ptd->edj);
        ptd->edj = eina_stringshare_ref(edj_out);
        ptd->source_edj = eina_stringshare_ref(edj_in);

        edje_pick_data = mem_malloc(sizeof(Edje_Exe_Data));
        edje_pick_data->cmd = eina_stringshare_add(eina_strbuf_string_get(strbuf));
        edje_pick_data->flags  = ECORE_EXE_PIPE_READ |
                                 ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                                 ECORE_EXE_PIPE_ERROR |
                                 ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
        edje_pick_data->data = (void *)ptd;
        edje_pick_data->exe_cmd = ecore_exe_pipe_run(edje_pick_data->cmd, edje_pick_data->flags, NULL);
        edje_pick_data->exe_pid = ecore_exe_pid_get(edje_pick_data->exe_cmd);
        eina_strbuf_free(strbuf);

        eina_stringshare_del(edj_in);
        eina_stringshare_del(edj_out);

        ptd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _data_from_edje_pick_cb,
                                                    (void *)edje_pick_data);
        ptd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _edje_pick_end_cb,
                                                   (void *)edje_pick_data);
        ptd->error_handler = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _data_from_edje_pick_cb,
                                                     (void *)edje_pick_data);
    }
   else
     {
        msg = eina_stringshare_printf(_("Import processing"));
        ptd->func_progress(NULL, msg);
        eina_stringshare_del(msg);

        _project_edj_file_copy(ptd);
        _copy_meta_data_to_pro(ptd);
        _project_special_group_add(ptd->project);
        _project_dummy_image_add(ptd->project);
        _project_open_internal(ptd);

        msg = eina_stringshare_printf(_("Import finished. Project '%s' created"), ptd->project->name);
        ptd->func_progress(NULL, msg);
        eina_stringshare_del(msg);

     }
   edje_edit_string_list_free(groups);
   return;
}

