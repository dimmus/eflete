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

#include "project_manager.h"
#include "string_macro.h"
#include "alloc.h"
#include "sound_editor.h"

static const char *dst_path;
static Eina_Bool copy_success;

static void
_on_copy_done_cb(void *data,
                 Eio_File *handler __UNUSED__)
{
   char *file_name = (char *)data;
   DBG("Copy file '%s' is finished!", file_name);
   copy_success = true;
   ecore_main_loop_quit();
}

/*TODO: should be reorganizeted to save 'realese' project */
static void
_on_copy_done_save_as_cb(void *data,
                         Eio_File *handler __UNUSED__)
{
   Project *project = (Project *)data;
   if (project->edj)
     free(project->edj);
   project->edj = strdup(dst_path);
   if (project->name)
     {
        eina_stringshare_del(project->name);
        project->name = NULL;
     }
   GET_NAME_FROM_PATH(project->name, project->edj);
   DBG("Copy file '%s' is finished!", project->edj);
   dst_path = NULL;
   copy_success = true;
   ecore_main_loop_quit();
}

static void
_on_copy_error_cb(void *data,
                  Eio_File *handler __UNUSED__,
                  int error)
{
   char *file_name = (char *)data;
   ERR("Copy file '%s' is failed. Something wrong has happend: %s\n",
       file_name, strerror(error));
   copy_success = false;
   ecore_main_loop_quit();
}

static void
_on_unlink_done_cb(void *data,
                   Eio_File *handler __UNUSED__)
{
   Project *project = (Project *)data;

   if (project->edj) free(project->edj);
   INFO ("Closed project: %s", project->name);
   eina_stringshare_del(project->name);
   eina_stringshare_del(project->dev);
   project->name = NULL;
   project->dev = NULL;
   wm_widget_list_free(project->widgets);

   free(project);

   DBG ("Project data is released.");
   ecore_main_loop_quit();
}

static void
_on_unlink_error_cb(void *data,
                    Eio_File *handler __UNUSED__,
                    int error)
{
   char *file_name = (char *)data;
   ERR("Unlink file '%s' is failed. Something wrong has happend: %s\n",
       file_name, strerror(error));
   ecore_main_loop_quit();
}

static Project *
_pm_project_add(const char *path)
{
   Eet_File *ef;
   Project *pro;
   char *name;
   char *dir_name;

   ef = eet_open(path, EET_FILE_MODE_READ);
   if (!ef)
     {
        ERR("File [%s] can not be opened.", path);
        eet_close(ef);
        return NULL;
     }
   eet_close(ef);

   pro = mem_calloc(1, sizeof(Project));

   name = edje_file_data_get(path, "theme/name");
   if (!name)
     GET_NAME_FROM_PATH(name, path);
   pro->name = eina_stringshare_add(name);
   free(name);
   DBG ("Project name: '%s'", pro->name);

   /* set path to edj */
   pro->edj = strdup(path);

   DBG ("Path to edj-file: '%s'", pro->edj);

   /* set path to swap file */
   dir_name = ecore_file_dir_get(pro->edj);
   pro->dev = eina_stringshare_printf("%s/%s.dev", dir_name, pro->name);
   DBG ("Path to swap file: '%s'", pro->dev);
   free(dir_name);

   pro->close_request = false;

   return pro;
}

Eina_Bool
pm_export_to_edc(Project *project,
                 Eina_Stringshare *edc_dir,
                 Edje_Compile_Log_Cb log_cb)
{
   if (!project)
     {
        WARN("Project is missing. Please open one.");
        return false;
     }

   return !decompile(project->dev, edc_dir, log_cb);
}

Project *
pm_open_project_edj(const char *path)
{
   Project *project;

   if (!path)
     {
        ERR("Can not open project path is NULL!");
        return NULL;
     }

   INFO("Open project! Path to project: '%s'.", path);
   project = _pm_project_add(path);

   if (!project) return NULL;

   copy_success = false;
   eio_file_copy(project->edj, project->dev, NULL,
                 _on_copy_done_cb, _on_copy_error_cb, project->dev);
   ecore_main_loop_begin();
   if (!copy_success)
     {
        _on_unlink_done_cb(project, NULL); /* TODO: separate pm_free from unlink callback */
        return NULL;
     }
   project->widgets = wm_widget_list_new(project->dev);
   project->layouts = wm_widget_list_layouts_load(project->dev);
   project->is_saved = true;
   INFO("Project '%s' is open!", project->name);

   return project;
}

Eina_Bool
pm_project_close(Project *project)
{
   if (!project) return false;
   if (project->autosave_timer)
     {
        ecore_timer_del(project->autosave_timer);
        project->autosave_timer = NULL;
     }

   eio_file_unlink(project->dev, _on_unlink_done_cb,
                   _on_unlink_error_cb, project);
   if (project->added_sounds)
     sound_editor_added_sounds_free(project->added_sounds);
   ecore_main_loop_begin();

   return true;
}

Eina_Bool
pm_save_project_edj(Project *project)
{
   if (!project) return false;
   copy_success = false;
   eio_file_copy(project->dev, project->edj, NULL,
                 _on_copy_done_cb, _on_copy_error_cb, project->dev);
   ecore_main_loop_begin();
   if (!copy_success) return false;
   project->is_saved = true;
   return true;
}

Eina_Bool
pm_save_as_project_edj(Project *project, const char *path)
{
   if (!project) return false;
   if (!path) return false;

   dst_path = path;
   copy_success = false;
   eio_file_copy(project->dev, path, NULL,
                 _on_copy_done_save_as_cb, _on_copy_error_cb, project);
   ecore_main_loop_begin();
   if (!copy_success) return false;
   project->is_saved = true;
   return true;
}

Eina_Bool
pm_save_project_to_swap(Project *project)
{
   Widget *widget;
   Style *style;
   Class *class_st;

   if (!project) return false;

   EINA_INLIST_FOREACH(project->widgets, widget)
     {
        EINA_INLIST_FOREACH(widget->classes, class_st)
          {
             EINA_INLIST_FOREACH(class_st->styles, style)
               {
                  if (style->isModify)
                    {
                       style->isModify = false;
                       edje_edit_save(style->obj);
                    }
               }
          }
     }

   EINA_INLIST_FOREACH(project->layouts, style)
     {
        if (style->isModify)
          {
             style->isModify = false;
             edje_edit_save(style->obj);
          }
     }
   return true;
}

Eina_Bool
pm_project_changed(Project *project)
{
   DBG("Project marker as changed\n");
   if (!project) return false;
   project->is_saved = false;
   return true;
}
