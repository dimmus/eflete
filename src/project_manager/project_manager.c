/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "project_manager.h"
#include "alloc.h"

static const char *dst_path;

static Eina_Bool
_on_quit(Eina_Bool force, void *data __UNUSED__)
{
   return force;
}

static void
_on_copy_done_cb(void *data,
                 Eio_File *handler __UNUSED__)
{
   char *file_name = (char *)data;
   DBG("Copy file '%s' is finished!", file_name);
   loop_quit(true);
}

static void
_on_copy_done_save_as_cb(void *data,
                 Eio_File *handler __UNUSED__)
{
   Project *project = (Project *)data;
   if (project->edj)
     free(project->edj);
   project->edj = strdup(dst_path);
   DBG("Copy file '%s' is finished!", dst_path);
   dst_path = NULL;
   loop_quit(true);
}

static void
_on_copy_error_cb(void *data,
                  Eio_File *handler __UNUSED__,
                  int error)
{
   char *file_name = (char *)data;
   ERR("Copy file '%s' is failed. Something wrong has happend: %s\n",
       file_name, strerror(error));
   loop_quit(true);
}

static void
_on_unlink_done_cb(void *data,
                   Eio_File *handler __UNUSED__)
{
   Project *project = (Project *)data;
   if (project->edc) free(project->edc);
   if (project->edj) free(project->edj);
   if (project->image_directory) free(project->image_directory);
   if (project->font_directory) free(project->font_directory);
   if (project->sound_directory) free(project->sound_directory);
   INFO ("Closed project: %s", project->name);
   free(project->name);
   wm_widget_list_free(project->widgets);
   DBG ("Project data is released.");
   loop_quit(true);
}

static void
_on_unlink_error_cb(void *data,
                    Eio_File *handler __UNUSED__,
                    int error)
{
   char *file_name = (char *)data;
   ERR("Unlink file '%s' is failed. Something wrong has happend: %s\n",
       file_name, strerror(error));
   loop_quit(true);
}

Eina_Bool
pm_free(Project *project)
{
   if (!project) return false;

   eio_file_unlink(project->swapfile, _on_unlink_done_cb,
                   _on_unlink_error_cb, project);
   loop_begin(_on_quit, NULL);

   if (project->swapfile) free(project->swapfile);
   free(project);
   project = NULL;
   return true;
}

static Project *
_pm_project_add(const char *name,
               const char *path,
               const char *id, /* image directory */
               const char *fd, /* font directory */
               const char *sd  /* sound directory */)
{
   Project *pro;
   char *tmp = NULL;
   int len;

   if (!name)
     {
        ERR("Project name is NULL");
        return NULL;
     }
   if (!path)
     {
        ERR("Path is NULL");
        return NULL;
     }
   if (!ecore_file_exists(path)) return NULL;

   pro = mem_calloc(1, sizeof(Project));
   pro->name = strdup(name);
   DBG ("Project name: '%s'", pro->name);

   /* set path to edc */
   pro->edc = strdup(path);

   len = strlen(pro->edc);
   if (len > 4) // 4 == strlen(".edj")
     {
        tmp = pro->edc + len - 4;
        if (!strcmp(tmp, ".edj")) strncpy(tmp, ".edc", 4);
        if (strcmp(tmp, ".edc"))
          {
             free(pro->name);
             free(pro->edc);
             free(pro);
             return NULL;
          }
     }
   else
     {
        free(pro->name);
        free(pro->edc);
        free(pro);
        return NULL;
     }

   /* set path to edj */
   pro->edj = strdup(path);

   tmp = pro->edj + len - 4;
   if (!strcmp(tmp, ".edc")) strncpy(tmp, ".edj", 4);

   DBG ("Path to edc-file: '%s'", pro->edc);
   DBG ("Path to edj-file: '%s'", pro->edj);

   /* set path to swap file */
   pro->swapfile = mem_malloc((strlen(pro->edj) + 6) * sizeof(char));
   strcpy(pro->swapfile, pro->edj);
   strncat(pro->swapfile, ".swap", 5);
   DBG ("Path to swap file: '%s'", pro->swapfile);

   /* set path to image directory */
   pro->image_directory = id ? strdup(id) : NULL;
   DBG ("Path to image direcotory: '%s'", pro->image_directory);

   /* set path to font directory */
   pro->font_directory = fd ? strdup(fd) : NULL;
   DBG("Path to font direcotory: '%s'", pro->font_directory);

   /* set default path to sound directory */
   pro->sound_directory = sd ? strdup(sd) : NULL;
   DBG ("Path to sound direcotory: '%s'", pro->sound_directory);

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

   return !decompile(project->edj, edc_dir, log_cb);
}

Project *
pm_open_project_edj(const char *name,
                    const char *path)
{
   Project *project;

   if (!path)
     {
        ERR("Can not open project path is NULL!");
        return NULL;
     }

   INFO("Open project! Path to project: '%s'.", path);
   project = _pm_project_add(name, path, NULL, NULL, NULL);

   if (!project) return NULL;

   eio_file_copy(project->edj, project->swapfile, NULL,
                 _on_copy_done_cb, _on_copy_error_cb, project->swapfile);
   loop_begin(_on_quit, NULL);
   project->widgets = wm_widget_list_new(project->swapfile);
   INFO("Project '%s' is open!", project->name);

   return project;
}

Eina_Bool
pm_save_project_edj(Project *project)
{
   if (!project) return false;
   eio_file_copy(project->swapfile, project->edj, NULL,
                 _on_copy_done_cb, _on_copy_error_cb, project->swapfile);
   loop_begin(_on_quit, NULL);
   return true;
}

Eina_Bool
pm_save_as_project_edj(Project *project, const char *path)
{
   if (!project) return false;
   if (!path) return false;

   dst_path = path;
   eio_file_copy(project->swapfile, path, NULL,
                 _on_copy_done_save_as_cb, _on_copy_error_cb, project);
   loop_begin(_on_quit, NULL);
   return true;
}

Eina_Bool
pm_save_project_to_swap(Project *project)
{
   Widget *widget;
   Style *style;
   Class *class_st;

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

   return true;
}
