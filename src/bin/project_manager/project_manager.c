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
#define _GNU_SOURCE
#include "project_manager.h"
#include "project_manager_private.h"
#include <fcntl.h>

#ifndef _WIN32
#include <sys/wait.h>
#else
#include <win32.h>
#endif

#include "change.h"

#define THREAD_CONTEXT_SWITCH_BEGIN    ecore_thread_main_loop_begin()
#define THREAD_CONTEXT_SWITCH_END      ecore_thread_main_loop_end()

static Eet_Compression compess_level = EET_COMPRESSION_HI;

static Eina_Bool
_project_dev_file_create(Project *pro)
{
   Eina_Bool result;

   result = ecore_file_cp(pro->saved_edj, pro->dev);
   DBG("Create the .dev file.");
   return result;
}


#ifndef _WIN32
Eina_Bool
_lock_try(const char *path, Eina_Bool check, int *pro_fd)
{
   struct flock lock, savelock;

   int fd = open(path, O_RDWR);
   lock.l_type    = F_WRLCK;   /* Test for any lock on any part of file. */
   lock.l_whence  = SEEK_SET;
   lock.l_start   = 0;
   lock.l_len     = 0;
   lock.l_pid     = 0;
   savelock = lock;
   fcntl(fd, F_GETLK, &lock);  /* Overwrites lock structure with preventors. */
   if ((lock.l_pid != 0) && ((lock.l_type == F_WRLCK) || (lock.l_type == F_RDLCK)))
     {
        ERR("Process %d has a write lock already!", lock.l_pid);
        return false;
     }

   /* if flag check is false not need to lock the file, just close handler */
   if (!check)
     {
        close(fd);
        return true;
     }

   if (pro_fd)
     {
        savelock.l_pid = getpid();
        fcntl(fd, F_SETLK, &savelock);
        *pro_fd = fd;
        return true;
     }
   close(fd);
   return false;
}
#else
Eina_Bool
_lock_try(const char *path, Eina_Bool check, HANDLE *pro_fd)
{
   LPCTSTR  lpFileName = path;
   DWORD dwDesiredAccess = GENERIC_READ|GENERIC_WRITE;
   DWORD  dwShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
   LPSECURITY_ATTRIBUTES  lpSecurityAttributes = NULL;
   DWORD  dwCreationDisposition = OPEN_EXISTING;
   DWORD  dwFlagsAndAttributes = 0;
   HANDLE  hTemplateFile = NULL;
   HANDLE fd = CreateFile(lpFileName, 
                          dwDesiredAccess, 
                          dwShareMode, 
                          lpSecurityAttributes, 
                          dwCreationDisposition,
                          dwFlagsAndAttributes,
                          hTemplateFile);
	
   if (fd == INVALID_HANDLE_VALUE) 
     {
        ERR("The file '%s' cannot be opened in mode read-write!", path);
        return !check;
     }

   if (!check)
     {
        CloseHandle(fd);
        return true;
     }
    if (pro_fd)
     {
        *pro_fd = fd;
        return true;
     }

    CloseHandle(fd);
    return false;
}
#endif

void
_project_descriptor_init(Project_Thread *ptd)
{
   Eet_Data_Descriptor_Class eddc;
   assert(ptd);

   if (ptd->eed_project) return;

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc),
                                             "Project", sizeof(Project));
   ptd->eed_project = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC(ptd->eed_project, Project, "version", version, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC(ptd->eed_project, Project, "dev", dev, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(ptd->eed_project, Project, "saved_edj", saved_edj, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(ptd->eed_project, Project, "develop_path", develop_path, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(ptd->eed_project, Project, "release_options", release_options, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING(ptd->eed_project, Project, "images", res.images);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING(ptd->eed_project, Project, "sounds", res.sounds);
   EET_DATA_DESCRIPTOR_ADD_LIST_STRING(ptd->eed_project, Project, "fonts", res.fonts);
}

void
_pm_project_descriptor_shutdown(Project_Thread *ptd)
{
   eet_data_descriptor_free(ptd->eed_project);
   ptd->eed_project = NULL;
}

void
_end_send(void *data)
{
   PM_Project_End_Cb func;
   Project_Thread *ptd = (Project_Thread *)data;
   PM_Project_Result result;
   Project *project;
   void *udata;

   /** Copy the links to callback and meesage, to fast release worker resource. */
   func = ptd->func_end;
   result = ptd->result;
   udata = ptd->data;
   project = ptd->project;
   ecore_event_handler_del(ptd->del_handler);
   ecore_event_handler_del(ptd->error_handler);
   ecore_event_handler_del(ptd->data_handler);

   if (ptd->tmp_dirname != NULL)
     {
        ecore_file_recursive_rm(ptd->tmp_dirname);
        eina_tmpstr_del(ptd->tmp_dirname);
     }

   eina_lock_free(&ptd->mutex);
   free(ptd);
   func(udata, result, project);
}

Eina_Bool
_build_script_write(const char *path)
{
   FILE *f;
   Eina_Bool res = true;
   Eina_Strbuf *buf;

   f = fopen(path, "w");
   if (!f)
     {
        ERR("Could't open file '%s'", path);
        res = false;
        goto exit;
     }

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "#!/bin/sh\n");
   eina_strbuf_append_printf(buf, "edje_cc -v generated.edc -id images/ -sd sounds/ -fd fonts/");
   fputs(eina_strbuf_string_get(buf), f);
   eina_strbuf_free(buf);
   chmod(path, S_IRWXU|S_IRWXG|S_IROTH|S_IWOTH);

exit:
   fclose(f);
   return res;
}

void
_gm_group_load_cancel_cb(void *data,
                         Ecore_Thread *th __UNUSED__)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   ecore_thread_global_data_del("ptd");
   edje_file_cache_flush();
   ptd->result = PM_PROJECT_ERROR;
   ERR("Failed to load group tree.");

   eina_lock_release(&ftd->mutex);
   TODO("Remove static worker from this module");
   free(ftd);
   _end_send(ptd);
}

void
_gm_group_load_end_cb(void *data,
                      Ecore_Thread *th __UNUSED__)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   ecore_thread_global_data_del("ptd");
   edje_file_cache_flush();
   ptd->result = PM_PROJECT_SUCCESS;

   eina_lock_release(&ftd->mutex);
   TODO("Remove static worker from this module");
   free(ftd);
   _end_send(ptd);
}

void
_gm_group_load_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project *project = (Project *) ptd->project;

   Eina_Stringshare *message = eina_stringshare_printf(_("Load group tree"));
   ecore_thread_feedback(th, message);

   gm_groups_load(project);
}

void
_copy_meta_data_to_pro(Project_Thread *ptd)
{
   Eet_File *ef;
   char *name, *authors, *version, *license, *comment;

   ef = eet_open(ptd->edj, EET_FILE_MODE_READ_WRITE);

   name = strdup(ptd->name);
   authors = eet_read(ef, PROJECT_KEY_AUTHORS, NULL);
   version = eet_read(ef, PROJECT_KEY_FILE_VERSION, NULL);
   license = eet_read(ef, PROJECT_KEY_LICENSE, NULL);
   comment = eet_read(ef, PROJECT_KEY_COMMENT, NULL);
   eet_close(ef);

   pm_project_meta_data_set(ptd->project, name, authors,
                            version, license, comment);

   if (name) free(name);
   if (authors) free(authors);
   if (version) free(version);
   if (license) free(license);
   if (comment) free(comment);
}

Eina_Bool
_project_edj_file_copy(Project_Thread *ptd)
{
   Eina_Stringshare *src, *dst;
   Eina_Bool result;

   src = eina_stringshare_ref(ptd->edj);
   dst = eina_stringshare_ref(ptd->project->saved_edj);
   result = ecore_file_cp(src, dst);

   DBG("Copy the .edj file to project folder.");
   eina_stringshare_del(src);
   eina_stringshare_del(dst);
   return result;
}

void
_project_open_internal(Project_Thread *ptd)
{
   assert(ptd != NULL);
   Project *project = ptd->project;

   _project_dev_file_create(project);
   project->mmap_file = eina_file_open(project->dev, false);

   project->changed = false;
   project->close_request = false;
   project->ecore_evas = ecore_evas_buffer_new(0, 0);
   project->global_object = edje_edit_object_add(ecore_evas_get(project->ecore_evas));
   edje_object_mmap_set(project->global_object, project->mmap_file, EFLETE_INTERNAL_GROUP_NAME);

   Feedback_Thread_Data *ftd;
   ftd = mem_calloc(1, sizeof(Feedback_Thread_Data));
   ftd->queue = 6;
   eina_lock_new(&ftd->mutex);
   ecore_thread_global_data_add("ptd", ptd, NULL, false);
   /* Launch images load inside thread with feedback */
   ecore_thread_feedback_run(_image_resources_feedback_job, _resources_export_feedback_cb,
                             _resources_export_end_cb, _resources_export_cancel_cb, ftd,
                             true);

   /*------------------------------------------------*/

   /* Launch sound load inside thread with feedback */
   ecore_thread_feedback_run(_sound_resources_feedback_job, _resources_export_feedback_cb,
                             _resources_export_end_cb, _resources_export_cancel_cb, ftd,
                             true);

   /*------------------------------------------------*/

   /* Launch font load inside thread with feedback */
   ecore_thread_feedback_run(_font_resources_feedback_job, _resources_export_feedback_cb,
                             _resources_export_end_cb, _resources_export_cancel_cb, ftd,
                             true);

   /*------------------------------------------------*/

   /* Launch tones load inside thread with feedback */
   ecore_thread_feedback_run(_tones_resources_feedback_job, _resources_export_feedback_cb,
                             _resources_export_end_cb, _resources_export_cancel_cb, ftd,
                             true);

   /*------------------------------------------------*/

   /* Launch colorclasses load inside thread with feedback */
   ecore_thread_feedback_run(_colorclasses_resources_feedback_job, _resources_export_feedback_cb,
                             _resources_export_end_cb, _resources_export_cancel_cb, ftd,
                             true);

   /*------------------------------------------------*/

   /* Launch colorclasses load inside thread with feedback */
   ecore_thread_feedback_run(_styles_resources_feedback_job, _resources_export_feedback_cb,
                             _resources_export_end_cb, _resources_export_cancel_cb, ftd,
                             true);

   /*------------------------------------------------*/
}

void
_project_special_group_add(Project *project)
{
   Evas *e;
   Evas_Object *edje_edit_obj;
   Eina_List *list;

   assert(project != NULL);

   THREAD_CONTEXT_SWITCH_BEGIN;

   Ecore_Evas *ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   list = edje_file_collection_list(project->saved_edj);
   edje_edit_obj = edje_edit_object_add(e);

   edje_object_file_set(edje_edit_obj, project->saved_edj, eina_list_data_get(list));
   CRIT_ON_FAIL(editor_internal_group_add(edje_edit_obj));

   edje_edit_string_list_free(list);
   evas_object_del(edje_edit_obj);
   ecore_evas_free(project->ecore_evas);

   THREAD_CONTEXT_SWITCH_END;
}

void
_project_dummy_sample_add(Project *project)
{
   Evas *e;
   Evas_Object *edje_edit_obj;
   char buf[PATH_MAX];

   assert(project != NULL);

   THREAD_CONTEXT_SWITCH_BEGIN;

   Ecore_Evas *ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   edje_edit_obj = edje_edit_object_add(e);

   edje_object_file_set(edje_edit_obj, project->saved_edj, EFLETE_INTERNAL_GROUP_NAME);
   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_SAMPLE_NAME, ap.path.sound_path);
   edje_edit_sound_sample_add(edje_edit_obj, EFLETE_DUMMY_SAMPLE_NAME, buf);

   you_shall_not_pass_editor_signals(NULL);
   CRIT_ON_FAIL(editor_save(edje_edit_obj));
   you_shall_pass_editor_signals(NULL);
   evas_object_del(edje_edit_obj);
   ecore_evas_free(project->ecore_evas);

   THREAD_CONTEXT_SWITCH_END;
}

void
_project_dummy_image_add(Project *project)
{
   Evas *e;
   Evas_Object *edje_edit_obj;
   char buf[PATH_MAX];

   assert(project != NULL);

   THREAD_CONTEXT_SWITCH_BEGIN;

   Ecore_Evas *ee = ecore_evas_buffer_new(0, 0);
   e = ecore_evas_get(ee);
   edje_edit_obj = edje_edit_object_add(e);

   edje_object_file_set(edje_edit_obj, project->saved_edj, EFLETE_INTERNAL_GROUP_NAME);
   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_IMAGE_NAME, ap.path.image_path);
   edje_edit_image_add(edje_edit_obj, buf);

   evas_object_del(edje_edit_obj);
   ecore_evas_free(project->ecore_evas);

   THREAD_CONTEXT_SWITCH_END;
}

void
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      Eina_List *list,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end ,
                      const void *data)
{
   assert(name != NULL);
   assert(path != NULL);
   assert(edj != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   ptd->result = PM_PROJECT_LAST;
   ptd->name = eina_stringshare_add(name);
   ptd->path = eina_stringshare_add(path);
   ptd->edj = eina_stringshare_add(edj);
   ptd->widgets = list;

   _project_import_edj(ptd);
}


void
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *import_options,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data)
{
   assert(name != NULL);
   assert(path != NULL);
   assert(edc != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   ptd->result = PM_PROJECT_LAST;
   ptd->name = eina_stringshare_add(name);
   ptd->path = eina_stringshare_add(path);
   ptd->edc = eina_stringshare_add(edc);
   ptd->build_options = eina_stringshare_add(import_options);

   _project_import_edc(ptd);
}

Eina_Bool
pm_project_thread_cancel()
{
   TODO("Need to rework this function");
   DBG("Project Thread stoped by user!");
   return false;
}

void
pm_project_open(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   assert(path != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->path = eina_stringshare_add(path);
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   eina_lock_new(&ptd->mutex);

   /* Launch open project inside thread with feedback */
   ecore_thread_feedback_run(_project_open_feedback_job, _project_open_feedback_cb,
                             _project_open_end_cb, _project_open_cancel_cb, ptd,
                             true);
}

void
pm_dev_file_reload(Project *pr)
{
   eina_file_close(pr->mmap_file);
   pr->mmap_file = eina_file_open(pr->dev, false);
   edje_object_mmap_set(pr->global_object, pr->mmap_file, EFLETE_INTERNAL_GROUP_NAME);
}

void
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   assert(project != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   ptd->project = project;
   if (!editor_save_all(ptd->project->global_object))
     {
        ERR("Failed to save project.");
        ptd->result = PM_PROJECT_ERROR;
        _end_send((void *)ptd);
        return;
     }
   ecore_file_cp(ptd->project->dev, ptd->project->saved_edj);
   ptd->result = PM_PROJECT_SUCCESS;
   _end_send((void *)ptd);
}

Eina_Bool
pm_project_style_save_as(Project *project __UNUSED__, const char *file __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_build(Project *project __UNUSED__, Build build_profile __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_close(Project *project)
{
   Eina_Stringshare *backup, *data;
   Eina_Stringshare *resource_folder;
   External_Resource *image, *sound, *font;
   Tone_Resource *tone;
   Colorclass_Resource *cc;
   Resource *style;

   assert(project != NULL);

   backup = eina_stringshare_printf("%s.backup", project->dev);
   ecore_file_remove(backup);
   eina_stringshare_del(backup);

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);
   ecore_file_recursive_rm(resource_folder);
   eina_stringshare_del(resource_folder);
   resource_folder = eina_stringshare_printf("%s/fonts", project->develop_path);
   ecore_file_recursive_rm(resource_folder);
   eina_stringshare_del(resource_folder);
   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);
   ecore_file_recursive_rm(resource_folder);
   eina_stringshare_del(resource_folder);

   evas_object_del(project->global_object);
   ecore_evas_free(project->ecore_evas);

   eina_file_close(project->mmap_file);
   ecore_file_unlink(project->dev);

   eina_stringshare_del(project->name);
   eina_stringshare_del(project->dev);
   eina_stringshare_del(project->develop_path);
   eina_stringshare_del(project->pro_path);

   gm_groups_free(project);
   EINA_LIST_FREE(project->images, image)
     {
        eina_stringshare_del(image->name);
        eina_list_free(image->used_in);
        eina_stringshare_del(image->source);
        free(image);
     }
   EINA_LIST_FREE(project->sounds, sound)
     {
        eina_stringshare_del(sound->name);
        eina_list_free(sound->used_in);
        eina_stringshare_del(sound->source);
        free(sound);
     }
   EINA_LIST_FREE(project->fonts, font)
     {
        eina_stringshare_del(font->name);
        eina_list_free(font->used_in);
        eina_stringshare_del(font->source);
        free(font);
     }
   EINA_LIST_FREE(project->tones, tone)
     {
        eina_stringshare_del(tone->name);
        eina_list_free(tone->used_in);
        free(tone);
     }
   EINA_LIST_FREE(project->colorclasses, cc)
     {
        eina_stringshare_del(cc->name);
        eina_list_free(cc->used_in);
        free(cc);
     }
   EINA_LIST_FREE(project->styles, style)
     {
        eina_stringshare_del(style->name);
        eina_list_free(style->used_in);
        free(style);
     }

   EINA_LIST_FREE(project->res.images, data)
      eina_stringshare_del(data);
   EINA_LIST_FREE(project->res.sounds, data)
      eina_stringshare_del(data);
   EINA_LIST_FREE(project->res.fonts, data)
      eina_stringshare_del(data);

#ifdef HAVE_ENVENTOR
   if (enventor_object_project_unload(project))
     free(project->enventor);
#endif /* HAVE_ENVENTOR */

   eet_close(project->ef);
#ifdef _WIN32
   if (project->pro_fd != INVALID_HANDLE_VALUE)
     CloseHandle(project->pro_fd);
#else
   if (project->pro_fd != -1)
     close(project->pro_fd);
#endif
   free(project);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_CLOSED, NULL);

   return true;
}

void
pm_project_meta_data_get(Project *project,
                         Eina_Stringshare **name,
                         Eina_Stringshare **authors,
                         Eina_Stringshare **version,
                         Eina_Stringshare **license,
                         Eina_Stringshare **comment)
{
   char *tmp;

   assert(project != NULL);
   assert(project->ef != NULL);

#define DATA_READ(DATA, KEY) \
   if (DATA) \
     { \
        tmp = eet_read(project->ef, KEY, NULL); \
        *DATA = eina_stringshare_add(tmp); \
        free(tmp); \
     }

   DATA_READ(name,    PROJECT_KEY_NAME);
   DATA_READ(authors, PROJECT_KEY_AUTHORS);
   DATA_READ(version, PROJECT_KEY_FILE_VERSION);
   DATA_READ(license, PROJECT_KEY_LICENSE);
   DATA_READ(comment, PROJECT_KEY_COMMENT);

#undef DATA_READ
}

Eina_Bool
pm_project_meta_data_set(Project *project,
                         const char *name,
                         const char *authors,
                         const char *version,
                         const char *license,
                         const char *comment)
{
   int bytes, size;
   Eina_Bool res;

   assert(project != NULL);
   assert(project->ef != NULL);

   res = true;

#define DATA_WRITE(DATA, KEY) \
   if (DATA) \
     { \
        size = (strlen(DATA) + 1) * sizeof(char); \
        bytes = eet_write(project->ef, KEY, DATA, size, compess_level); \
        if (bytes <= 0 ) res = false; \
     }

   DATA_WRITE(name,    PROJECT_KEY_NAME);
   DATA_WRITE(authors, PROJECT_KEY_AUTHORS);
   DATA_WRITE(version, PROJECT_KEY_FILE_VERSION);
   DATA_WRITE(license, PROJECT_KEY_LICENSE);
   DATA_WRITE(comment, PROJECT_KEY_COMMENT);

#undef DATA_WRITE
   return res;
}

Eina_Bool
pm_project_resource_export(Project *pro __UNUSED__, const char* dir_path __UNUSED__)
{
   return false;
}

void
_external_resources_export(Eina_List *resources, const char *dst)
{
   Eina_Strbuf *buf;
   Eina_List *l;
   External_Resource *res;
   char *path;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(resources, l, res)
     {
        if (res->resource_type == RESOURCE_TYPE_SOUND)
          {
             eina_strbuf_append_printf(buf, "%s/%s", dst, ecore_file_file_get(res->source));
          }
        else if (res->path == NULL)
          eina_strbuf_append_printf(buf, "%s/%s", dst, res->name);
        else
          {
             eina_strbuf_append_printf(buf, "%s/%s", dst, res->source);
          }
        path = ecore_file_dir_get(eina_strbuf_string_get(buf));
        if (!ecore_file_is_dir(path))
          {
             ecore_file_mkpath(path);
          }
        ecore_file_cp(res->path, eina_strbuf_string_get(buf));
        eina_strbuf_reset(buf);
        free(path);
        path = NULL;
     }
   eina_strbuf_free(buf);
}

void
_external_resource_export(Eina_List *resources, Eina_Stringshare *name, const char *dst)
{
   Eina_Strbuf *buf;
   Eina_List *l;
   External_Resource *res;
   char *path = NULL;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(resources, l, res)
     {
        if (name == res->name)
          {
             if (res->resource_type == RESOURCE_TYPE_SOUND)
               {
                  eina_strbuf_append_printf(buf, "%s/%s", dst, ecore_file_file_get(res->source));
               }
             else if (res->path == NULL)
               {
                  eina_strbuf_append_printf(buf, "%s/%s", dst, res->name);
               }
             else
               {
                  eina_strbuf_append_printf(buf, "%s/%s", dst, res->source);
               }

             path = ecore_file_dir_get(eina_strbuf_string_get(buf));

             if (!ecore_file_is_dir(path))
               {
                  ecore_file_mkpath(path);
               }
             ecore_file_cp(res->path, eina_strbuf_string_get(buf));
             eina_strbuf_reset(buf);
             free(path);
             break;
          }
     }
   eina_strbuf_free(buf);
}

void
pm_group_source_code_export(Project *project,
                            Group *group,
                            const char *path,
                            PM_Project_Progress_Cb func_progress,
                            PM_Project_End_Cb func_end,
                            const void *data)
{
   assert(project != NULL);
   assert(group != NULL);
   assert(path != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->path = eina_stringshare_add(path);
   ptd->project = project;
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   ptd->group = group;
   eina_lock_new(&ptd->mutex);

   ecore_thread_feedback_run(_group_source_code_export_feedback_job, _group_source_code_export_feedback_cb,
                             _group_source_code_export_end_cb, _group_source_code_export_cancel_cb, ptd,
                             true);
}

void
pm_project_source_code_export(Project *project,
                              const char *path,
                              PM_Project_Progress_Cb func_progress,
                              PM_Project_End_Cb func_end,
                              const void *data)
{
   assert(project != NULL);
   assert(path != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->path = eina_stringshare_add(path);
   ptd->project = project;
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   eina_lock_new(&ptd->mutex);

   ecore_thread_feedback_run(_project_source_code_export_feedback_job,
                             _project_source_code_export_feedback_cb,
                             _project_source_code_export_end_cb,
                             _project_source_code_export_cancel_cb, ptd,
                             true);
}

void
pm_project_release_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   assert(project != NULL);
   assert(path != NULL);

   Project_Thread *ptd;
   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->path = eina_stringshare_add(path);
   ptd->project = project;
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;

   _release_export(ptd);
}

void
pm_project_develop_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   assert(project != NULL);
   assert(path != NULL);

   Project_Thread *ptd;

   ptd = mem_calloc(1, sizeof(Project_Thread));
   ptd->func_progress = func_progress;
   ptd->func_end = func_end;
   ptd->data = (void *)data;
   ptd->project = project;
   ptd->result = PM_PROJECT_LAST;
   ptd->path = eina_stringshare_add(path);

   _develop_export(ptd);
}

#ifdef HAVE_ENVENTOR
/*
static void *
_enventor_save(void *data __UNUSED__,
               Eina_Thread *thread __UNUSED__)
{
   Ecore_Event_Handler *cb_msg_stdout = NULL,
                       *cb_msg_stderr = NULL;
   Eina_Stringshare *cmd, *edj, *dir;
   Eina_List *l;
   Eina_Strbuf *buf = NULL;
   int edje_cc_res = 0, edje_pick_res = 0, waitpid_res = 0;

   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, worker);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, worker);
     }
   edj = eina_stringshare_printf("%s/build.edj", worker.project->enventor->path);
   buf = eina_strbuf_new();

   EINA_LIST_FOREACH(worker.project->res.images, l, dir)
     {
        eina_strbuf_append_printf(buf, " -id %s", dir);
     }
   EINA_LIST_FOREACH(worker.project->res.fonts, l, dir)
     {
        eina_strbuf_append_printf(buf, " -fd %s", dir);
     }
   EINA_LIST_FOREACH(worker.project->res.sounds, l, dir)
     {
        eina_strbuf_append_printf(buf, " -sd %s", dir);
     }
   Edje_Exe_Data *edje_cc_data = mem_malloc(sizeof(Edje_Exe_Data));
   edje_cc_data->cmd = eina_stringshare_printf("edje_cc -v %s %s %s", eina_strbuf_string_get(buf),
                                               worker.project->enventor->file, edj);
   edje_cc_data->flags  = ECORE_EXE_PIPE_READ |
                          ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                          ECORE_EXE_PIPE_ERROR |
                          ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
   eina_strbuf_free(buf);
   THREAD_TESTCANCEL;
   DBG("Run command for compile: %s", edje_cc_data->cmd);
   ecore_main_loop_thread_safe_call_sync(_ecore_exe_edje_exe, edje_cc_data);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(edje_cc_data->exe_pid, &edje_cc_res, 0);
   eina_stringshare_del(edje_cc_data->cmd);
   free(edje_cc_data);

   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }
   eina_stringshare_del(cmd);

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_cc_res) && (WEXITSTATUS(edje_cc_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }
   THREAD_TESTCANCEL;

   if (worker.func_progress)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, worker);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, worker);
     }
   worker.edj = eina_stringshare_printf("%s/enbuild.edj",
                                         worker.project->enventor->path);
   cmd = eina_stringshare_printf("edje_pick -o %s -a %s -i %s -g %s",
                                 worker.edj,
                                 worker.project->dev, edj,
                                 worker.project->current_style->full_group_name);

   DBG("Run command for compile: %s", cmd);
   exe_cmd = ecore_exe_pipe_run(cmd, flags, NULL);
   exe_pid = ecore_exe_pid_get(exe_cmd);
   THREAD_TESTCANCEL;
   waitpid_res = waitpid(exe_pid, &edje_pick_res, 0);

   if (worker.func_progress)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }

   if ((waitpid_res == -1) ||
       (WIFEXITED(edje_pick_res) && (WEXITSTATUS(edje_pick_res) != 0 )))
     {
        END_SEND(PM_PROJECT_ERROR);
        return NULL;
     }

   eina_file_copy(worker.edj, worker.project->dev,
                  EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                  NULL, NULL);

   eina_stringshare_del(edj);
   END_SEND(PM_PROJECT_SUCCESS)

   return NULL;
}

void
pm_project_enventor_save(Project *project,
                         PM_Project_Progress_Cb func_progress,
                         PM_Project_End_Cb func_end,
                         const void *data)
{
   assert(project != NULL);

   WORKER_CREATE(func_progress, func_end, data, project,
                 NULL, NULL, NULL, NULL, NULL, NULL);

   if (!eina_thread_create(&worker.thread, EINA_THREAD_URGENT, -1,
                           (void *)_enventor_save, NULL))
     {
        ERR("System error: can't create thread");
        abort();
     }
}
*/
#endif /* HAVE_ENVENTOR */

Eina_Bool
pm_lock_check(const char *path)
{
   return _lock_try(path, false, NULL);
}

Eina_Bool
pm_project_group_import(Project *project, const char *edj, const char *group)
{
   Change *change;
   Evas_Object *obj, *img;
   Evas *e;
   Eina_List *resources, *resources1, *l, *l1;
   Eina_Stringshare *data, *data1, *source, *res_file;
   char *res_dir;
   External_Resource *res;
   int id, size;
   int c1_r, c1_g, c1_b, c1_a,
       c2_r, c2_g, c2_b, c2_a,
       c3_r, c3_g, c3_b, c3_a;
   FILE *f;
   Eina_Binbuf *sound_bin;
   void *font;
   Eet_File *ef;
   Resource request;

   assert(edj != NULL);
   assert(group != NULL);

   e = ecore_evas_get(project->ecore_evas);
   THREAD_CONTEXT_SWITCH_BEGIN;
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, edj, group);
   THREAD_CONTEXT_SWITCH_END;

   /* import group dependencies */
   resources = gm_group_used_groups_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        pm_project_group_import(project, edj, data);
     }
   /* import images */
   resources = gm_group_used_images_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        if (project->images)
          {
             /* if images list is not empty get from first item the path to
              * resource folder, in currect case - path to folder with images */
             res = eina_list_data_get(project->images);
             res_dir = ecore_file_dir_get(res->source);
             request.resource_type = RESOURCE_TYPE_IMAGE;
             request.name = data;
             res = (External_Resource *)resource_get(project->images, &request);
             if ((res) &&
                 ((res->name == data) || (!strcmp(res->name, data))))
               {
                  TODO("implement resource replacing while importing, see comment bellow")
                     /* when efl merge window will be opened need to add logic for
                      * replace any resource. Now we can do it only for images, because
                      * edje_edit has a API for replace one image to enother, but for
                      * sounds, fonts, color_classes and styles API is missing. So
                      * be better if replacing functional been implemented for all
                      * resource type at the same time */
                     continue;
               }
          }
        else
          {
             res_dir = string_cat(project->develop_path, "images");
          }
        id = edje_edit_image_id_get(obj, data);
        if (id < 0)
          {
             WARN("Image %s coudn't be exported", data);
             continue;
          }
        /* save image, it's need for insert to project dev file */
        res_file = eina_stringshare_printf("%s/%s", res_dir, data);
        source = eina_stringshare_printf("edje/images/%i", id);
        THREAD_CONTEXT_SWITCH_BEGIN;
        img = evas_object_image_add(e);
        evas_object_image_file_set(img, edj, source);
        evas_object_image_save(img, res_file, NULL, NULL);
        evas_object_del(img);
        THREAD_CONTEXT_SWITCH_END;

        CRIT_ON_FAIL(edje_edit_image_add(project->global_object, res_file));
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_IMAGE);
        res->source = eina_stringshare_add(data);
        resource_insert(&project->images, (Resource *)res);

        eina_stringshare_del(source);
        eina_stringshare_del(res_file);
        free(res_dir);
     }
   /* images imported */

   /* import sound */
   resources = gm_group_used_sample_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        if (project->sounds)
          {
             res = eina_list_data_get(project->sounds);
             res_dir = ecore_file_dir_get(res->source);
             request.resource_type = RESOURCE_TYPE_SOUND;
             request.name = data;
             res = (External_Resource *)resource_get(project->sounds, &request);
             if ((res) &&
                 ((res->name == data) || (!strcmp(res->name, data))))
               {
                  TODO("implement resource replacing")
                     continue;
               }
          }
        else
          {
             res_dir = string_cat(project->develop_path, "sounds");
          }
        THREAD_CONTEXT_SWITCH_BEGIN;
        sound_bin = edje_edit_sound_samplebuffer_get(obj, data);
        THREAD_CONTEXT_SWITCH_END;
        res_file = eina_stringshare_printf("%s/%s", res_dir, data);
        if (!(f = fopen(res_file, "wb")))
          {
             ERR("Could not open file: %s", res_file);
             continue;
          }
        if (fwrite(eina_binbuf_string_get(sound_bin),
                   eina_binbuf_length_get(sound_bin), 1, f) != 1)
          ERR("Could not write sound: %s", strerror(errno));
        if (f) fclose(f);
        eina_binbuf_free(sound_bin);

        THREAD_CONTEXT_SWITCH_BEGIN;
        CRIT_ON_FAIL(edje_edit_sound_sample_add(project->global_object, data, res_file));
        THREAD_CONTEXT_SWITCH_END;
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_SOUND);
        res->source = eina_stringshare_add(res_file);
        resource_insert(&project->sounds, (Resource *)res);

        eina_stringshare_del(res_file);
        free(res_dir);
     }
   /* sounds imported */

   /* import fonts */
   resources = gm_group_used_fonts_edj_get(obj);
   if (resources)
     {
        ef = eet_open(edj, EET_FILE_MODE_READ);
        EINA_LIST_FOREACH(resources, l, data)
          {
             if (project->fonts)
               {
                  res = eina_list_data_get(project->fonts);
                  res_dir = ecore_file_dir_get(res->source);
                  request.resource_type = RESOURCE_TYPE_FONT;
                  request.name = data;
                  res = (External_Resource *)resource_get(project->fonts, &request);
                  if ((res) &&
                      ((res->name == data) || (!strcmp(res->name, data))))
                    {
                       TODO("implement resource replacing")
                          continue;
                    }
               }
             else
               {
                  res_dir = string_cat(project->develop_path, "fonts");
               }
             source = eina_stringshare_printf("edje/fonts/%s", data);
             THREAD_CONTEXT_SWITCH_BEGIN;
             font = eet_read(ef, source, &size);
             THREAD_CONTEXT_SWITCH_END;
             res_file = eina_stringshare_printf("%s/%s", res_dir, data);
             if (!font) continue;
             if (!(f = fopen(res_file, "wb")))
               {
                  ERR("Could not open file: %s", res_file);
                  continue;
               }
             if (fwrite(font, size, 1, f) != 1)
               ERR("Could not write font: %s", strerror(errno));
             if (f) fclose(f);
             free(font);
             eina_stringshare_del(source);

             THREAD_CONTEXT_SWITCH_BEGIN;
             edje_edit_font_add(project->global_object, res_file, data);
             THREAD_CONTEXT_SWITCH_END;
             res = (External_Resource *)resource_add(data, RESOURCE_TYPE_FONT);
             res->source = eina_stringshare_add(res_file);
             resource_insert(&project->fonts, (Resource *)res);

             eina_stringshare_del(res_file);
             free(res_dir);
          }
        eet_close(ef);
     }
   /* fonts imported */

   /* import color_classes */
   resources = gm_group_used_color_classes_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        request.resource_type = RESOURCE_TYPE_COLORCLASS;
        request.name = data;
        res = (External_Resource *)resource_get(project->colorclasses, &request);
        if ((res) &&
            ((res->name == data) || (!strcmp(res->name, data))))
          {
             TODO("implement resource replacing")
                continue;
          }
        THREAD_CONTEXT_SWITCH_BEGIN;
        edje_edit_color_class_colors_get(obj, data,
                                         &c1_r, &c1_g, &c1_b, &c1_a,
                                         &c2_r, &c2_g, &c2_b, &c2_a,
                                         &c3_r, &c3_g, &c3_b, &c3_a);
        CRIT_ON_FAIL(edje_edit_color_class_add(project->global_object, data));
        CRIT_ON_FAIL(edje_edit_color_class_colors_set(project->global_object, data,
                                                      c1_r, c1_g, c1_b, c1_a,
                                                      c2_r, c2_g, c2_b, c2_a,
                                                      c3_r, c3_g, c3_b, c3_a));
        THREAD_CONTEXT_SWITCH_END;
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_COLORCLASS);
        resource_insert(&project->colorclasses, (Resource *)res);
     }
   /* color_classes imported */

   /* import styles */
   resources = gm_group_used_styles_edj_get(obj);
   EINA_LIST_FOREACH(resources, l, data)
     {
        request.resource_type = RESOURCE_TYPE_STYLE;
        request.name = data;
        res = (External_Resource *)resource_get(project->styles, &request);
        if ((res) &&
            ((res->name == data) || (!strcmp(res->name, data))))
          {
             TODO("implement resource replacing")
                continue;
          }
        edje_edit_style_add(project->global_object, data);
        THREAD_CONTEXT_SWITCH_BEGIN;
        resources1 = edje_edit_style_tags_list_get(obj, data);
        THREAD_CONTEXT_SWITCH_END;
        EINA_LIST_FOREACH(resources1, l1, data1)
          {
             THREAD_CONTEXT_SWITCH_END;
             source = edje_edit_style_tag_value_get(obj, data, data1);
             CRIT_ON_FAIL(edje_edit_style_tag_add(project->global_object, data, data1));
             CRIT_ON_FAIL(edje_edit_style_tag_value_set(project->global_object, data, data1, source));
             eina_stringshare_del(source);
             THREAD_CONTEXT_SWITCH_END;
          }
        res = (External_Resource *)resource_add(data, RESOURCE_TYPE_STYLE);
        resource_insert(&project->styles, (Resource *)res);
     }
   /* styles imported */

   change = change_add(NULL);

   /* add fake change for min/max group attributes.
    * Try to set the default values */
   THREAD_CONTEXT_SWITCH_BEGIN;
   CRIT_ON_FAIL(editor_group_max_h_set(obj, change, true, false, 0));
   CRIT_ON_FAIL(editor_group_max_w_set(obj, change, true, false, 0));
   CRIT_ON_FAIL(editor_group_min_h_set(obj, change, true, false, 0));
   CRIT_ON_FAIL(editor_group_min_w_set(obj, change, true, false, 0));
   THREAD_CONTEXT_SWITCH_END;

   you_shall_not_pass_editor_signals(NULL);
   /* fake changes for imported group
    * reset all attributes for program and parts, for avoid errors in case
    * cycled dependencies;
    */
   editor_group_reset(obj, change, false);

   /* apply group to project */
   THREAD_CONTEXT_SWITCH_BEGIN;
   CRIT_ON_FAIL(editor_group_add(project->global_object, group));
   CRIT_ON_FAIL(editor_save_all(project->global_object));
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, project->dev, group);

   change_undo(obj, change);
   CRIT_ON_FAIL(editor_save_all(obj));
   THREAD_CONTEXT_SWITCH_END;
   /* reload file after group add */
   //pm_dev_file_reload(project);
   /* add group to project structures */
   THREAD_CONTEXT_SWITCH_BEGIN;
   gm_group_add(project, group, false);
   THREAD_CONTEXT_SWITCH_END;

   you_shall_pass_editor_signals(NULL);

   change_free(change);

   return true;
}
