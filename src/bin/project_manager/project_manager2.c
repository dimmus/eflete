/*
 * Edje Theme Editor
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

#define _GNU_SOURCE
#include "project_manager2.h"
#include <fcntl.h>
#include <errno.h>

#ifndef _WIN32
#include <sys/wait.h>
#else
#include <win32.h>
#include <windows.h>
static HANDLE hMutex = NULL;
#endif

#define PROJECT_FILE_KEY      "project"

#define PROJECT_KEY_NAME         "edje/name"
#define PROJECT_KEY_AUTHORS      "edje/authors"
#define PROJECT_KEY_FILE_VERSION "edje/file_version"
#define PROJECT_KEY_LICENSE      "edje/license"
#define PROJECT_KEY_COMMENT      "edje/comment"


#define FLAGS ECORE_EXE_PIPE_READ | ECORE_EXE_PIPE_READ_LINE_BUFFERED | \
              ECORE_EXE_PIPE_ERROR | ECORE_EXE_PIPE_ERROR_LINE_BUFFERED

#define LOCK_FILE ".project.lock"

static Eet_Compression compess_level = EET_COMPRESSION_HI;
static PM_Project_Result last_error = PM_PROJECT_LAST;

struct _Project_Process_Data
{
   /** The progress callback. See #PM_Project_Progress_Cb.*/
   PM_Project_Progress_Cb func_progress;
   /** The end callback. See #PM_Project_End_Cb. */
   PM_Project_End_Cb func_end;
   /** The user data. */
   void *data;
   /** The new project, was created in the Project process. This pointer will be
    * NULL until the Project process finished it's job.*/
   Project *project;
   /** Name of project what must be created. */
   const char *name;
   /** Path to new project. */
   const char *path;
   /** Path to imported edj file. */
   const char *edj;
   /** Path to imported edc file. */
   const char *edc;
   /** edje_cc options. Used for 'new project' and 'import from edc'. */
   const char *build_options;
   /** */
   const char *source_edj;
   /** The checked widgets. Used for loading just checked widgets. */
   Eina_List *widgets;
   /** The temporary directory. Should be stored here for a correct clean temporary files. */
   Eina_Tmpstr *tmp_dirname;
   /** The group, which source should be exported into edc. */
   Group2 *group;

   /** Handlers for monitoring events from exe pipes. */
   Ecore_Event_Handler *data_handler;
   Ecore_Event_Handler *error_handler;
   Ecore_Event_Handler *del_handler;

   /** Desriptor for read data form eet file */
   Eet_Data_Descriptor *eed_project;
};
typedef struct _Project_Process_Data Project_Process_Data;



static void
_project_descriptor_init(Project_Process_Data *ppd)
{
   Eet_Data_Descriptor_Class eddc;

   assert(ppd != NULL);

   if (ppd->eed_project) return;

   eet_eina_stream_data_descriptor_class_set(&eddc, sizeof(eddc),
                                             "Project", sizeof(Project));
   ppd->eed_project = eet_data_descriptor_stream_new(&eddc);

   EET_DATA_DESCRIPTOR_ADD_BASIC (ppd->eed_project, Project, "version", version, EET_T_INT);
   EET_DATA_DESCRIPTOR_ADD_BASIC (ppd->eed_project, Project, "name", name, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC (ppd->eed_project, Project, "release_options", release_options, EET_T_STRING);
}

void
_pm_project_descriptor_shutdown(Project_Process_Data *ppd)
{
   eet_data_descriptor_free(ppd->eed_project);
   ppd->eed_project = NULL;
}

static void
_ecore_event_handlers_del(Project_Process_Data *ppd)
{
   assert(ppd != NULL);

   if (ppd->data_handler)
     {
        ecore_event_handler_del(ppd->data_handler);
        ppd->data_handler = NULL;
     }
   if (ppd->error_handler)
     {
        ecore_event_handler_del(ppd->error_handler);
        ppd->error_handler = NULL;
     }
   if (ppd->del_handler)
     {
        ecore_event_handler_del(ppd->del_handler);
        ppd->del_handler = NULL;
     }
}

static void
_project_process_data_cleanup(Project_Process_Data *ppd)
{
   assert(ppd != NULL);

   if (ppd->name) eina_stringshare_del(ppd->name);
   if (ppd->path) eina_stringshare_del(ppd->path);
   if (ppd->edj) eina_stringshare_del(ppd->edj);
   if (ppd->edc) eina_stringshare_del(ppd->edc);
   if (ppd->build_options) eina_stringshare_del(ppd->build_options);

   if (ppd->tmp_dirname)
     {
        ecore_file_recursive_rm(ppd->tmp_dirname);
        eina_tmpstr_del(ppd->tmp_dirname);
     }
   _ecore_event_handlers_del(ppd);
   _pm_project_descriptor_shutdown(ppd);

   free(ppd);
}

__UNUSED_RESULT__ static PM_Project_Result
_project_special_group_add(Project *project)
{
   Eina_List *groups;
   Evas_Object *obj;
   Ecore_Evas *ecore_evas;

   assert(project != NULL);

   last_error = PM_PROJECT_SUCCESS;
   if (edje_file_group_exists(project->saved_edj, EFLETE_INTERNAL_GROUP_NAME))
     return last_error;

   groups = edje_file_collection_list(project->saved_edj);
   ecore_evas = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ecore_evas));
   edje_object_file_set(obj, project->saved_edj, eina_list_data_get(groups));

   you_shall_not_pass_editor_signals(NULL);
   if (!editor_internal_group_add(obj))
     {
        CRIT("Could not add Eflete spec group");
        last_error = PM_PROJECT_ADD_SPEC_GROUP_FAILED;
     }
   you_shall_pass_editor_signals(NULL);

   ecore_evas_free(ecore_evas);
   edje_edit_string_list_free(groups);

   return last_error;
}

static Project *
_project_create(Project_Process_Data *ppd)
{
   Project *pro;
   char buf[PATH_MAX];

   last_error = PM_PROJECT_SUCCESS;
   _project_descriptor_init(ppd);

   snprintf(buf, sizeof(buf), "%s/%s", ppd->path, ppd->name);
   if (ecore_file_mkdir(buf))
     {
        DBG("Create the folder '%s' for new project '%s'", buf, ppd->name);
     }
   else
     {
        ERR("Could't create a project folder!");
        last_error = PM_PROJECT_CREATE_PRO_FAILED;
        return NULL;
     }

   pro = (Project *)mem_calloc(1, sizeof(Project));
   pro->version = PROJECT_FILE_VERSION;
   pro->name = eina_stringshare_ref(ppd->name);
   pro->dev = eina_stringshare_printf("%s/%s.dev", buf, ppd->name);
   pro->saved_edj = eina_stringshare_printf("%s/%s.edj", buf, ppd->name);
   pro->develop_path = eina_stringshare_printf("%s/develop", buf);

   snprintf(buf, sizeof(buf), "%s/%s/%s.pro", ppd->path, ppd->name, ppd->name);
   pro->pro_path = eina_stringshare_add(buf);
   pro->ef = eet_open(buf, EET_FILE_MODE_READ_WRITE);
   if (!pro->ef)
     {
        CRIT("Failed to create a pro file '%s'", buf);
        last_error = PM_PROJECT_CREATE_PRO_FAILED;
        goto exit;
     }
   ecore_file_mkdir(pro->develop_path);
   snprintf(buf, sizeof(buf), "%s/images", pro->develop_path);
   ecore_file_mkdir(buf);
   snprintf(buf, sizeof(buf), "%s/sounds", pro->develop_path);
   ecore_file_mkdir(buf);
   snprintf(buf, sizeof(buf), "%s/fonts", pro->develop_path);
   ecore_file_mkdir(buf);

   if (!eet_data_write(pro->ef, ppd->eed_project, PROJECT_FILE_KEY, pro, compess_level))
     last_error = PM_PROJECT_WRITE_PRO_FAILED;

   eet_close(pro->ef);
   _pm_project_descriptor_shutdown(ppd);

exit:
   if (PM_PROJECT_SUCCESS != last_error)
     {
        ERR("Could't create a .pro file! ")
        eina_stringshare_del(pro->name);
        eina_stringshare_del(pro->dev);
        eina_stringshare_del(pro->saved_edj);
        eina_stringshare_del(pro->develop_path);
        free(pro);
        pro = NULL;
     }

   return pro;
}

__UNUSED_RESULT__ static PM_Project_Result
_project_dummy_sample_add(Project *project)
{
   char buf[PATH_MAX];
   Eina_List *list, *l;
   Evas_Object *obj;
   Ecore_Evas *ecore_evas;
   const char *data;

   assert(project != NULL);

   last_error = PM_PROJECT_SUCCESS;
   list = edje_file_collection_list(project->saved_edj);
   ecore_evas = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ecore_evas));
   edje_object_file_set(obj, project->saved_edj, eina_list_data_get(list));
   edje_edit_string_list_free(list);

   /* check if sample exist */
   list = edje_edit_sound_samples_list_get(obj);
   EINA_LIST_FOREACH(list, l, data)
     {
        if (!strcmp(data, EFLETE_DUMMY_SAMPLE_NAME))
          {
             edje_edit_string_list_free(list);
             return last_error;
          }
     }
   edje_edit_string_list_free(list);

   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_SAMPLE_NAME, ap.path.sound_path);
   you_shall_not_pass_editor_signals(NULL);
   if (!editor_sound_sample_add(obj, EFLETE_DUMMY_SAMPLE_NAME, buf, false))
     {
        CRIT("Could not add Efelte dummy sample");
        last_error = PM_PROJECT_ADD_SPEC_SAMPLE_FAILED;
     }
   you_shall_pass_editor_signals(NULL);

   ecore_evas_free(ecore_evas);

   return last_error;
}

__UNUSED_RESULT__ static PM_Project_Result
_project_dummy_image_add(Project *project)
{
   char buf[PATH_MAX];
   Eina_List *list, *l;
   Evas_Object *obj;
   Ecore_Evas *ecore_evas;
   const char *data;

   assert(project != NULL);

   last_error = PM_PROJECT_SUCCESS;
   list = edje_file_collection_list(project->saved_edj);
   ecore_evas = ecore_evas_buffer_new(0, 0);
   obj = edje_edit_object_add(ecore_evas_get(ecore_evas));
   edje_object_file_set(obj, project->saved_edj, eina_list_data_get(list));
   edje_edit_string_list_free(list);

   /* check if images exist */
   list = edje_edit_images_list_get(obj);
   EINA_LIST_FOREACH(list, l, data)
     {
        if (!strcmp(data, EFLETE_DUMMY_IMAGE_NAME))
          {
             edje_edit_string_list_free(list);
             return last_error;
          }
     }
   edje_edit_string_list_free(list);

   snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_IMAGE_NAME, ap.path.image_path);
   you_shall_not_pass_editor_signals(NULL);
   if (!editor_image_add(obj, buf, false))
     {
        CRIT("Could not add eflete dummy image");
        last_error = PM_PROJECT_ADD_SPEC_IMAGE_FAILED;
     }
   you_shall_pass_editor_signals(NULL);

   ecore_evas_free(ecore_evas);

   return last_error;
}

static PM_Project_Result
_project_edj_file_copy(Project_Process_Data *ppd)
{
   DBG("Copy the .edj file to project folder.");
   if (eina_file_copy(ppd->edj, ppd->project->saved_edj,
                      EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                      NULL, NULL))
     last_error = PM_PROJECT_SUCCESS;
   else
     last_error = PM_PROJECT_COPY_FILE_FAILED;

   return last_error;
}

static PM_Project_Result
_copy_meta_data_to_pro(Project_Process_Data *ppd)
{
   Eet_File *ef;
   char *name, *authors, *version, *license, *comment;
   last_error = PM_PROJECT_SUCCESS;

   ef = eet_open(ppd->edj, EET_FILE_MODE_READ_WRITE);

   name = strdup(ppd->name);
   authors = eet_read(ef, PROJECT_KEY_AUTHORS, NULL);
   version = eet_read(ef, PROJECT_KEY_FILE_VERSION, NULL);
   license = eet_read(ef, PROJECT_KEY_LICENSE, NULL);
   comment = eet_read(ef, PROJECT_KEY_COMMENT, NULL);
   eet_close(ef);

   last_error = pm_project_meta_data_set(ppd->project, name, authors, version, license, comment);

   if (name) free(name);
   if (authors) free(authors);
   if (version) free(version);
   if (license) free(license);
   if (comment) free(comment);

   return last_error;
}

static Eina_Bool
_project_dev_file_create(Project *pro)
{
   DBG("Create the .dev file.");
   return ecore_file_cp(pro->saved_edj, pro->dev);
}

static void
_end_send(void *data)
{
   PM_Project_End_Cb func;
   Project *project;
   void *udata;
   Project_Process_Data *ppd = data;

   /** Copy the links to callback and meesage, to fast release worker resource. */
   func = ppd->func_end;
   udata = ppd->data;
   project = ppd->project;

   _project_process_data_cleanup(ppd);
   func(udata, last_error, project);
}

static Eina_Bool
_project_lock(Project *project)
{
   char path[PATH_MAX];
   char buf[BUFF_MAX];
   char *dir;
   int pid;

   assert(project != NULL);

   pid = getpid();

   dir = ecore_file_dir_get(project->pro_path);
   snprintf(path, sizeof(path), "%s/"LOCK_FILE, dir);
   free(dir);
   project->fd_lock = open(path, O_RDWR | O_CREAT, S_IROTH | S_IWOTH);
   if (!project->fd_lock)
     {
        ERR("%s: %s\n", path, strerror(errno));
        return false;
     }

#ifdef _WIN32
   hMutex = CreateMutex(NULL, FALSE, PACKAGE_NAME);

   HANDLE handle = CreateFile(path, GENERIC_READ, NULL, NULL, CREATE_NEW,
                              FILE_FLAG_DELETE_ON_CLOSE, 0);
   if (INVALID_HANDLE_VALUE == handle)
     {
        ERR("Failed to open file \"%s\"", path);
        return false;
     }
#else
   struct flock fl;
   fl.l_type = F_WRLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 0;
   fl.l_pid = pid;
   if (fcntl(project->fd_lock, F_SETLK, &fl) == -1)
     {
        ERR("Unable to lock project file '%s'\n", path);
        close(project->fd_lock);
        return false;
     }

   snprintf(buf, sizeof(buf), "%d\n", pid);
   if (!write(project->fd_lock, buf, strlen(buf)))
     {
        close(project->fd_lock);
        return false;
     }
#endif /* _WIN32 */

   return true;
}

static Eina_Bool
_project_unlock(Project *project)
{
   char path[PATH_MAX];
   char *dir;

   assert(project != NULL);

   dir = ecore_file_dir_get(project->pro_path);
   snprintf(path, sizeof(path), "%s/"LOCK_FILE, dir);
   free(dir);

#ifdef _WIN32
   if (hMutex)
     {
        CloseHandle(hMutex);
        hMutex = NULL;
     }
#else
   struct flock fl;
   fl.l_type = F_UNLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 0;
   fl.l_pid = getpid();
   if (fcntl(project->fd_lock, F_SETLK, &fl) == -1)
     {
        ERR("Unable to unlock file '%s'\n", path);
        return false;
     }
#endif /* _WIN32 */

   close(project->fd_lock);
   if (!ecore_file_unlink(path))
     ERR("unlink %s: %s\n", path, strerror(errno));

   return true;
}

static Eina_Bool
_project_trylock(const char *pro_path)
{
   int fd;
   char *dir;
   char path[PATH_MAX];
   Eina_Bool ret = true;

   assert(path != NULL);

   dir = ecore_file_dir_get(pro_path);
   snprintf(path, sizeof(path), "%s/"LOCK_FILE, dir);
   free(dir);
   if (!ecore_file_exists(path))
     return true;

   fd = open(path, O_RDWR);
   if (fd < 1)
     {
        ERR(" %s\n", strerror(errno));
        return false;
     }

#ifdef _WIN32
   hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, PACKAGE_NAME);
   if (hMutex)
     return false;
#else
   struct flock fl;
   fl.l_type = F_UNLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 0;
   fl.l_pid = 0;
   if (fcntl(fd, F_GETLK, &fl) != -1)
     {
        if (fl.l_type != F_UNLCK)
          ret = false;
     }
   else
     ret = false;
#endif /* _WIN32 */

   close(fd);
   return ret;
}

static Eina_Bool
_exe_output_handler(void *data,
                    int type __UNUSED__,
                    void *event_info)
{
   int i;
   Ecore_Exe_Event_Data *exe_out_msg = (Ecore_Exe_Event_Data *)event_info;
   Project_Process_Data *ppd = data;

   if (!exe_out_msg) return ECORE_CALLBACK_DONE;

   for (i = 0; exe_out_msg->lines[i].line != NULL; i++)
     ppd->func_progress(NULL, exe_out_msg->lines[i].line);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_exporter_finish_handler(void *data,
                         int type __UNUSED__,
                         void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;
   Project *project = (Project *) ppd->project;
   Ecore_Exe_Event_Del *exporter_exit = (Ecore_Exe_Event_Del *)event_info;

   _ecore_event_handlers_del(ppd);

   if (exporter_exit->exit_code != 0)
     {
        last_error = exporter_exit->exit_code;
        _end_send(ppd);
        return ECORE_CALLBACK_DONE;
     }

   resource_manager_init(project);

   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

static void
_project_close_internal(Project *project)
{
   _project_unlock(project);

   if (project->global_object)
     evas_object_del(project->global_object);

   if (project->ecore_evas)
     ecore_evas_free(project->ecore_evas);

   if (project->mmap_file)
     eina_file_close(project->mmap_file);

   if (project->name)
     eina_stringshare_del(project->name);

   if (project->dev)
     eina_stringshare_del(project->dev);
   if (project->develop_path)
     eina_stringshare_del(project->develop_path);
   if (project->pro_path)
     eina_stringshare_del(project->pro_path);


#ifdef HAVE_ENVENTOR
   if (enventor_object_project_unload(project))
     free(project->enventor);
#endif /* HAVE_ENVENTOR */

   if (project->ef)
     eet_close(project->ef);
}

static PM_Project_Result
_project_open_internal(Project_Process_Data *ppd)
{
   char buf[PATH_MAX];
   char *file_dir;

   Eet_File *ef;
   char *tmp;
   int tmp_len;
   Edje_Load_Error error;

   last_error = PM_PROJECT_SUCCESS;

   edje_file_cache_flush();
   ef = eet_open(ppd->path, EET_FILE_MODE_READ_WRITE);
   if (!ef)
     {
        ERR("Failed to open project file '%s'", ppd->path);
        last_error = PM_PROJECT_OPEN_PRO_FAILED;
        return last_error;
     }

   _project_descriptor_init(ppd);
   ppd->project = eet_data_read(ef, ppd->eed_project, PROJECT_FILE_KEY);
   _pm_project_descriptor_shutdown(ppd);
   if (!ppd->project)
     {
        ERR("Failed to load project data from '%s'", ppd->path);
        last_error = PM_PROJECT_READ_PRO_FAILED;
        return last_error;
     }

   ppd->project->ef = ef;
   ppd->project->pro_path = eina_stringshare_add(ppd->path);

   file_dir = ecore_file_dir_get(ppd->path);
   ppd->project->dev = eina_stringshare_printf("%s/%s.dev", file_dir, ppd->name);
   ppd->project->saved_edj = eina_stringshare_printf("%s/%s.edj", file_dir, ppd->name);
   ppd->project->develop_path = eina_stringshare_printf("%s/develop", file_dir);

   snprintf(buf, sizeof(buf), "%s/images", ppd->project->develop_path);
   ecore_file_mkdir(buf);
   snprintf(buf, sizeof(buf), "%s/sounds", ppd->project->develop_path);
   ecore_file_mkdir(buf);
   snprintf(buf, sizeof(buf), "%s/fonts", ppd->project->develop_path);
   ecore_file_mkdir(buf);

   _project_lock(ppd->project);

   /* updating .dev file path */
   tmp = strdup(ppd->path);
   tmp_len = strlen(tmp);
   tmp[tmp_len - 3] = 'd';
   tmp[tmp_len - 2] = 'e';
   tmp[tmp_len - 1] = 'v';
   eina_stringshare_replace(&ppd->project->dev, tmp);
   free(tmp);
   /* updating .edj file path */
   tmp = strdup(ppd->path);
   tmp[tmp_len - 3] = 'e';
   tmp[tmp_len - 2] = 'd';
   tmp[tmp_len - 1] = 'j';
   eina_stringshare_replace(&ppd->project->saved_edj, tmp);
   free(tmp);

   //message = eina_stringshare_add(_("Checking project version"));

   /* checking for older project versions and upgrading them version-by-version */
   if (ppd->project->version < 2) /* upgrade to version 2 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 2"));
        INFO("Updating project files to version 2");
        ecore_file_mv(ppd->project->dev, ppd->project->saved_edj);
        ppd->project->version = 2;
     }
   if (ppd->project->version < 3) /* upgrade to version 3 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 3"));
        INFO("Updating project files to version 3");
        if (PM_PROJECT_SUCCESS != _project_special_group_add(ppd->project))
          {
             CRIT("Update failed: Spec group is not added");
             return last_error;
          }
        ppd->project->version = 3;
     }
   if (ppd->project->version < 4) /* upgrade to version 4 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 4"));
        INFO("Updating project files to version 4");
        if (PM_PROJECT_SUCCESS != _project_dummy_image_add(ppd->project))
          {
             CRIT("Update failed: Spec image is not added");
             return last_error;
          }
        ppd->project->version = 4;
     }
   if (ppd->project->version < 5) /* upgrade to version 4 */
     {
        //message = eina_stringshare_add(_("Updating project files to version 5"));
        INFO("Updating project files to version 5");
        if (PM_PROJECT_SUCCESS != _project_dummy_sample_add(ppd->project))
          {
             CRIT("Update failed: Spec sample is not added");
             return last_error;
          }
        ppd->project->version = 5;
     }

   TODO("Add crash recovery prompt here");
   TODO("Add project integrity check here");

   _project_dev_file_create(ppd->project);
   ppd->project->mmap_file = eina_file_open(ppd->project->dev, false);

   ppd->project->changed = false;
   ppd->project->close_request = false;
   ppd->project->ecore_evas = ecore_evas_buffer_new(0, 0);
   ppd->project->global_object = edje_edit_object_add(ecore_evas_get(ppd->project->ecore_evas));
   edje_object_mmap_set(ppd->project->global_object, ppd->project->mmap_file, EFLETE_INTERNAL_GROUP_NAME);
   error = edje_object_load_error_get(ppd->project->global_object);
   if (EDJE_LOAD_ERROR_NONE != error)
     {
        CRIT("Could not load internal object: %s\n", edje_load_error_str(error));
        last_error = PM_PROJECT_LOAD_INTERNAL_OBJ_FAILED;
        return last_error;
     }

   pm_project_meta_data_get(ppd->project, &ppd->project->name, NULL, NULL, NULL, NULL);
   if (!ppd->project->name)
     ppd->project->name = eina_stringshare_add(ecore_file_strip_ext(ecore_file_file_get(ppd->path)));


/******************************************************************************/
   snprintf(buf, sizeof(buf),
            "%s --edj %s --path %s/develop", ap.path.exporter, ppd->project->saved_edj, file_dir);

   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _exporter_finish_handler, ppd);

   free(file_dir);

   return last_error;
}

PM_Project_Result
pm_project_open(const char *path,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   Project_Process_Data *ppd;
   char *spath;

   assert(path != NULL);

   spath = eina_file_path_sanitize(path);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(spath);
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;

   if (PM_PROJECT_SUCCESS != _project_open_internal(ppd))
     {
        Project *project = ppd->project;
        _project_process_data_cleanup(ppd);
        _project_close_internal(project);
     }

   free(spath);
   return last_error;
}

static Eina_Bool
_edje_pick_finish_handler(void *data,
                          int type __UNUSED__,
                          void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   /* reset path variable for open project. _project_open_internal use this
    * var for path to .pro file */
   eina_stringshare_del(ppd->path);
   ppd->path = eina_stringshare_ref(ppd->project->pro_path);

   _ecore_event_handlers_del(ppd);

   ecore_file_cp(ppd->edj, ppd->project->saved_edj);
   if (PM_PROJECT_SUCCESS != _project_special_group_add(ppd->project) ||
       PM_PROJECT_SUCCESS != _project_dummy_image_add(ppd->project) ||
       PM_PROJECT_SUCCESS != _project_dummy_sample_add(ppd->project))
     return ECORE_CALLBACK_CANCEL;

   if (PM_PROJECT_SUCCESS != _project_open_internal(ppd))
     {
        _project_close_internal(ppd->project);
        return ECORE_CALLBACK_CANCEL;
     }
   else
     return ECORE_CALLBACK_DONE;
}

static PM_Project_Result
_project_import_edj(Project_Process_Data *ppd)
{
   Eina_Stringshare *edj_in, *edj_out;
   Eina_List *l, *groups;
   Eina_Stringshare *group;
   Evas_Object *obj = NULL;
   Eina_Strbuf *strbuf;
   char buf[PATH_MAX];
   unsigned int count;

   //Eina_Stringshare *msg = eina_stringshare_printf(_("Start import '%s' file as new project"), ptd->edj);
   snprintf(buf, sizeof(buf), "Start import '%s' file as new project", ppd->edj);
   if (ppd->func_progress) ppd->func_progress(NULL, buf);
   //eina_stringshare_del(msg);

   ppd->project = _project_create(ppd);
   if (!ppd->project)
     return last_error;

   groups = edje_file_collection_list(ppd->edj);
   count = eina_list_count(groups);
   edje_edit_string_list_free(groups);
   if (ppd->widgets && (count != eina_list_count(ppd->widgets)))
     {
        //msg = eina_stringshare_printf(_("Merging groups from choosen file"));
        snprintf(buf, sizeof(buf), "Merging groups from choosen file");
        if (ppd->func_progress) ppd->func_progress(NULL, buf);
        //eina_stringshare_del(msg);

        eina_file_mkdtemp("eflete_build_XXXXXX", &ppd->tmp_dirname);
        edj_in = eina_stringshare_printf("%s/in.edj", ppd->tmp_dirname);
        edj_out = eina_stringshare_printf("%s/out.edj", ppd->tmp_dirname);
        ecore_file_cp(ppd->edj, edj_in);

        /* prepare the cmd string for run edje_pick */
        strbuf = eina_strbuf_new();
        eina_strbuf_append_printf(strbuf, "edje_pick -o %s -i %s", edj_out, edj_in);

        obj = edje_edit_object_add(evas_object_evas_get(ap.win));
        EINA_LIST_FOREACH(ppd->widgets, l, group)
          {
             if ((group[0] == 'c') &&
                 (group[1] == 'p') &&
                 (group[2] == '*') &&
                 (group[3] == '*') &&
                 (group[4] == '*'))
               {
                  char **arr = eina_str_split(group, "***", 0);
                  you_shall_not_pass_editor_signals(NULL);
                  /* load any group for coping */
                  if (!edje_object_file_set(obj, edj_in, arr[1]))
                    {
                       CRIT("Can't load object");
                       free(arr[0]);
                       free(arr);
                       last_error = PM_PROJECT_LOAD_GROUP_FAILED;
                       return last_error;
                    }
                  if (!editor_group_copy(obj, arr[1], arr[2], false))
                    {
                       CRIT("Can not copy group %s, to %s", arr[1], arr[2]);
                       free(arr[0]);
                       free(arr);
                       last_error = PM_PROJECT_COPY_GROUP_FAILED;
                       return last_error;
                    }
                  you_shall_pass_editor_signals(NULL);
                  eina_strbuf_append_printf(strbuf, " -g %s", arr[2]);
                  free(arr[0]);
                  free(arr);
               }
             else
               eina_strbuf_append_printf(strbuf, " -g %s", group);
          }
        evas_object_del(obj);

        eina_stringshare_del(ppd->edj);
        ppd->edj = eina_stringshare_ref(edj_out);
        ppd->source_edj = eina_stringshare_ref(edj_in);
#ifndef _WIN32
        char *str = string_backslash_insert(eina_strbuf_string_get(strbuf), '&');
        ecore_exe_pipe_run(str, FLAGS, NULL);
        free(str);
#else
        ecore_exe_pipe_run(eina_strbuf_string_get(strbuf), FLAGS, NULL);
        eina_strbuf_free(strbuf);
#endif /* _WIN32 */

        ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
        ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _edje_pick_finish_handler, ppd);
     }
   else
     {
        /* reset path variable for open project. _project_open_internal use this
         * var for path to .pro file */
        eina_stringshare_del(ppd->path);
        ppd->path = eina_stringshare_ref(ppd->project->pro_path);

        if (PM_PROJECT_SUCCESS != _project_edj_file_copy(ppd))
          return last_error;
        if (PM_PROJECT_SUCCESS != _copy_meta_data_to_pro(ppd))
          return last_error;
        if (PM_PROJECT_SUCCESS != _project_special_group_add(ppd->project))
          return last_error;
        if (PM_PROJECT_SUCCESS != _project_dummy_image_add(ppd->project))
          return last_error;
        if (PM_PROJECT_SUCCESS != _project_dummy_sample_add(ppd->project))
          return last_error;
        if (PM_PROJECT_SUCCESS != _project_open_internal(ppd))
          return last_error;
     }
   return last_error;
}

PM_Project_Result
pm_project_import_edj(const char *name,
                      const char *path,
                      const char *edj,
                      Eina_List *list,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end ,
                      const void *data)
{
   Project_Process_Data *ppd;
   Eina_Stringshare *pro_path;

   assert(name != NULL);
   assert(path != NULL);
   assert(edj != NULL);

   char *spath = eina_file_path_sanitize(path);
   char *sedj = eina_file_path_sanitize(edj);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->name = eina_stringshare_add(name);
   ppd->path = eina_stringshare_add(spath);
   ppd->edj = eina_stringshare_add(sedj);
   ppd->widgets = list;

   if (PM_PROJECT_SUCCESS != _project_import_edj(ppd))
     {
        Project *project = ppd->project;
        _project_process_data_cleanup(ppd);
        _project_close_internal(project);
        pro_path = eina_stringshare_printf("%s/%s", spath, name);
        ecore_file_recursive_rm(pro_path);
        eina_stringshare_del(pro_path);
     }

   free(spath);
   free(sedj);

   return last_error;
}

static Eina_Bool
_finish_from_edje_cc(void *data,
                     int type __UNUSED__,
                     void *event_info)
{
   Project_Process_Data *ppd = data;
   Ecore_Exe_Event_Del *edje_cc_exit = (Ecore_Exe_Event_Del *)event_info;
   char buf[PATH_MAX];

   if (edje_cc_exit->exit_code != 0)
     {
        last_error = PM_PROJECT_BUILD_SOURCE_EDC_FAILED;
        _end_send(ppd);
        return ECORE_CALLBACK_DONE;
     }

   if (ppd->func_progress)
     {
        snprintf(buf, sizeof(buf), "Data for importing prepare");
        if (ppd->func_progress) ppd->func_progress(NULL, buf);
     }
   _ecore_event_handlers_del(ppd);

   _project_import_edj(ppd);

   return ECORE_CALLBACK_DONE;
}

static PM_Project_Result
_project_import_edc(void *data)
{
   Project_Process_Data *ppd = data;
   char buf[PATH_MAX];

   assert(ppd != NULL);

   last_error = PM_PROJECT_SUCCESS;
   snprintf(buf, sizeof(buf), "Start import '%s' file as new project", ppd->edc);
   if (ppd->func_progress) ppd->func_progress(NULL, buf);

   eina_file_mkdtemp("eflete_build_XXXXXX", &ppd->tmp_dirname);
   ppd->edj = eina_stringshare_printf("%s/out.edj", ppd->tmp_dirname);
   snprintf(buf, sizeof(buf),
            "edje_cc -v %s %s %s", ppd->edc, ppd->edj, ppd->build_options);
   DBG("Run command for compile: %s\n", buf);
   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _finish_from_edje_cc, ppd);

   return last_error;
}

PM_Project_Result
pm_project_import_edc(const char *name,
                      const char *path,
                      const char *edc,
                      const char *import_options,
                      PM_Project_Progress_Cb func_progress,
                      PM_Project_End_Cb func_end,
                      const void *data)
{
   Project_Process_Data *ppd;

   assert(name != NULL);
   assert(path != NULL);
   assert(edc != NULL);

   char *spath = eina_file_path_sanitize(path);
   char *sedc = eina_file_path_sanitize(edc);

   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->name = eina_stringshare_add(name);
   ppd->path = eina_stringshare_add(spath);
   ppd->edc = eina_stringshare_add(sedc);
   ppd->build_options = eina_stringshare_add(import_options);

   if (PM_PROJECT_SUCCESS != _project_import_edc(ppd))
     {
        Project *project = ppd->project;
        _project_process_data_cleanup(ppd);
        _project_close_internal(project);
        ecore_file_recursive_rm(spath);
     }
   free(spath);
   free(sedc);

   return last_error;
}

PM_Project_Result
pm_project_close(Project *project)
{
   char buf[PATH_MAX];

   assert(project != NULL);

   last_error = PM_PROJECT_SUCCESS;
   snprintf(buf, sizeof(buf),
            "%s.backup", project->dev);
   ecore_file_unlink(buf);
   snprintf(buf, sizeof(buf),
            "%s/images", project->develop_path);
   ecore_file_recursive_rm(buf);
   snprintf(buf, sizeof(buf),
            "%s/sounds", project->develop_path);
   ecore_file_recursive_rm(buf);
   snprintf(buf, sizeof(buf),
            "%s/fonts", project->develop_path);
   ecore_file_recursive_rm(buf);

   ecore_file_unlink(project->dev);
   _project_close_internal(project);

   resource_manager_shutdown(project);

   evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_CLOSED, NULL);
   free(project);

   return last_error;
}

static Eina_Bool
_copy_progress(void *data, unsigned long long done, unsigned long long total)
{
   Project_Process_Data * ppd = data;

   if (done == total)
     {
        last_error = PM_PROJECT_SUCCESS;
        _end_send(ppd);
     }
   return true;
}

PM_Project_Result
pm_project_save(Project *project,
                PM_Project_Progress_Cb func_progress,
                PM_Project_End_Cb func_end,
                const void *data)
{
   assert(project != NULL);

   last_error = PM_PROJECT_SUCCESS;
   Project_Process_Data *ppd;
   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;
   ppd->project = project;
   if (!editor_save_all(ppd->project->global_object))
     {
        ERR("Failed to save project.");
        last_error = PM_PROJECT_ERROR;
        _end_send(ppd);
     }
   else if (!eina_file_copy(ppd->project->dev, ppd->project->saved_edj,
                            EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                            _copy_progress, ppd))
     last_error = PM_PROJECT_COPY_FILE_FAILED;

   return last_error;
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

PM_Project_Result
pm_project_meta_data_set(Project *project,
                         const char *name,
                         const char *authors,
                         const char *version,
                         const char *license,
                         const char *comment)
{
   int bytes, size;
   Eet_File *ef;

   assert(project != NULL);

   last_error = PM_PROJECT_SUCCESS;
   ef = eet_open(project->pro_path, EET_FILE_MODE_READ_WRITE);
#define DATA_WRITE(DATA, KEY) \
   if (DATA) \
     { \
        size = (strlen(DATA) + 1) * sizeof(char); \
        bytes = eet_write(ef, KEY, DATA, size, compess_level); \
        if (bytes <= 0 && size > 0) \
          { \
             CRIT("Could not write data '"#DATA"' size %i write %i\n", size, bytes);   \
             last_error = PM_PROJECT_WRITE_PRO_FAILED; \
          }\
     }

   DATA_WRITE(name,    PROJECT_KEY_NAME);
   DATA_WRITE(authors, PROJECT_KEY_AUTHORS);
   DATA_WRITE(version, PROJECT_KEY_FILE_VERSION);
   DATA_WRITE(license, PROJECT_KEY_LICENSE);
   DATA_WRITE(comment, PROJECT_KEY_COMMENT);
   eet_close(ef);

   return last_error;
}

static Eina_Bool
_group_export_finish_handler(void *data,
                             int type __UNUSED__,
                             void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   last_error = PM_PROJECT_SUCCESS;
   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

PM_Project_Result
pm_group_source_code_export(Project *project,
                            Eina_List *groups,
                            const char *path,
                            PM_Project_Progress_Cb func_progress,
                            PM_Project_End_Cb func_end,
                            const void *data)
{
   Project_Process_Data *ppd;
   Group2 *group;
   Eina_Strbuf *cmd;
   Eina_List *g, *l;

   assert(project != NULL);
   assert(path != NULL);

   last_error = PM_PROJECT_SUCCESS;
   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;

   cmd = eina_strbuf_new();
   eina_strbuf_append_printf(cmd, "%s --edj %s --path %s -s -u", ap.path.exporter, project->saved_edj, path);

   g = groups ? groups : project->groups;
   EINA_LIST_FOREACH(g, l, group)
      eina_strbuf_append_printf(cmd, " -g %s", group->common.name);

   ecore_exe_pipe_run(eina_strbuf_string_get(cmd), FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _group_export_finish_handler, ppd);

   eina_strbuf_free(cmd);
   return last_error;
}

PM_Project_Result
pm_project_source_code_export(Project *project,
                              const char *path,
                              PM_Project_Progress_Cb func_progress,
                              PM_Project_End_Cb func_end,
                              const void *data)
{
   Project_Process_Data *ppd;
   char buf[PATH_MAX];

   assert(project != NULL);
   assert(path != NULL);

   last_error = PM_PROJECT_SUCCESS;
   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;

   snprintf(buf, sizeof(buf),
            "%s --edj %s --path %s -s", ap.path.exporter, project->saved_edj, path);

   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _group_export_finish_handler, ppd);

   return last_error;
}

static Eina_Bool
_develop_export_finish_handler(void *data,
                               int type __UNUSED__,
                               void *event_info)
{
   Project_Process_Data *ppd = data;
   Ecore_Exe_Event_Del *edje_pick_exit = (Ecore_Exe_Event_Del *)event_info;
   char buf[PATH_MAX];

   _ecore_event_handlers_del(ppd);

   last_error = PM_PROJECT_SUCCESS;
   if (edje_pick_exit->exit_code != 0)
     {
        last_error = PM_PROJECT_EXPORT_DEVELOP_EDJ_FAILED;
        _end_send(ppd);
        return ECORE_CALLBACK_DONE;
     }

   if (ppd->tmp_dirname)
     {
        ecore_file_unlink(ppd->path);
        ecore_file_mv(ppd->tmp_dirname, ppd->path);
        eina_tmpstr_del(ppd->tmp_dirname);
        ppd->tmp_dirname = NULL;
     }
   snprintf(buf, sizeof(buf), _("Export to file '%s' successfull"), ppd->path);
   if (ppd->func_progress) ppd->func_progress(NULL, buf);

  _end_send(ppd);

  return ECORE_CALLBACK_DONE;
}

PM_Project_Result
pm_project_develop_export(Project *project,
                          const char *path,
                          Eina_List *groups,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   Project_Process_Data *ppd;
   Eina_Strbuf *cmd;
   Eina_List *l;
   Group2 *group;

   assert(project != NULL);
   assert(path != NULL);

   last_error = PM_PROJECT_SUCCESS;
   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(path);
   ppd->project = project;
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->data = (void *)data;

   CRIT_ON_FAIL(editor_save_all(project->global_object));

   if (groups)
     {
        cmd = eina_strbuf_new();
        if (!ecore_file_exists(ppd->path))
          eina_strbuf_append_printf(cmd, "edje_pick -o %s", path);
        else
          {
             int fd = eina_file_mkstemp("eflete_export_XXXXXX", &ppd->tmp_dirname);
             /* special for Windows. Window does not like the opened handlers
                we need close it for write file in future. */
             close(fd);
             eina_strbuf_append_printf(cmd, "edje_pick -o %s", ppd->tmp_dirname);
             eina_strbuf_append_printf(cmd, " -a %s", path);
          }
        eina_strbuf_append_printf(cmd, " -i %s", project->dev);

        EINA_LIST_FOREACH(groups, l, group)
           eina_strbuf_append_printf(cmd, " -g %s", group->common.name);

        DBG("Run command for export: %s", eina_strbuf_string_get(cmd));
        ecore_exe_pipe_run(eina_strbuf_string_get(cmd), FLAGS, NULL);

        ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
        ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _develop_export_finish_handler, ppd);
     }
   else
     eina_file_copy(ppd->project->dev, path,
                            EINA_FILE_COPY_PERMISSION | EINA_FILE_COPY_XATTR,
                            _copy_progress, ppd);


   return last_error;
}

static Eina_Bool
_release_export_build_finish_handler(void *data,
                                     int type __UNUSED__,
                                     void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;

   if (!ecore_file_exists(ppd->edj))
     last_error = PM_PROJECT_EXPORT_RELEASE_EDJ_FAILED;
   else
     last_error = PM_PROJECT_SUCCESS;
   _end_send(ppd);
   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_release_export_finish_handler(void *data,
                             int type __UNUSED__,
                             void *event_info __UNUSED__)
{
   Project_Process_Data *ppd = data;
   Eina_Strbuf *buf;
   char folder[PATH_MAX];

   _ecore_event_handlers_del(ppd);

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "edje_cc -v %s %s", ppd->edc, ppd->edj);
   snprintf(folder, sizeof(folder), "%s/images", ppd->tmp_dirname);
   if (ecore_file_exists(folder))
     eina_strbuf_append_printf(buf, " -id %s", folder);
   snprintf(folder, sizeof(folder), "%s/sounds", ppd->tmp_dirname);
   if (ecore_file_exists(folder))
     eina_strbuf_append_printf(buf, " -sd %s", folder);
   snprintf(folder, sizeof(folder), "%s/fonts", ppd->tmp_dirname);
   if (ecore_file_exists(folder))
     eina_strbuf_append_printf(buf, " -fd %s", folder);
   ecore_exe_pipe_run(eina_strbuf_string_get(buf), FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _release_export_build_finish_handler, ppd);

   eina_strbuf_free(buf);
   return ECORE_CALLBACK_DONE;
}

PM_Project_Result
pm_project_release_export(Project *project,
                          const char *path,
                          PM_Project_Progress_Cb func_progress,
                          PM_Project_End_Cb func_end,
                          const void *data)
{
   Project_Process_Data *ppd;
   char buf[PATH_MAX];

   assert(project != NULL);
   assert(path != NULL);

   last_error = PM_PROJECT_SUCCESS;
   ppd = mem_calloc(1, sizeof(Project_Process_Data));
   ppd->path = eina_stringshare_add(path);
   ppd->project = project;
   ppd->func_progress = func_progress;
   ppd->func_end = func_end;
   ppd->edj = eina_stringshare_add(path);
   ppd->data = (void *)data;

   CRIT_ON_FAIL(editor_save_all(project->global_object));

   eina_file_mkdtemp("eflete_export_XXXXXX", &ppd->tmp_dirname);
   snprintf(buf, sizeof(buf),
            "%s --edj %s --path %s -s", ap.path.exporter, ppd->project->saved_edj, ppd->tmp_dirname);
   ppd->edc = eina_stringshare_printf("%s/generated.edc", ppd->tmp_dirname);
   ecore_exe_pipe_run(buf, FLAGS, NULL);

   ppd->data_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_output_handler, ppd);
   ppd->del_handler = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _release_export_finish_handler, ppd);

   return last_error;
}

Eina_Bool
pm_lock_check(const char *path)
{
   if (ecore_file_exists(path) == false)
     return true;
   return _project_trylock(path);
}

const char *
pm_project_result_string_get(PM_Project_Result result)
{
   switch (result)
     {
      case PM_PROJECT_SUCCESS:
         return "Project job is success";
      case PM_PROJECT_CANCEL:
         return "User cancel project job";
      case PM_PROJECT_LOCKED:
         return "Given pro file is locked";
      case PM_PROJECT_CREATE_PRO_FAILED:
         return "Could not to create new pro file";
      case PM_PROJECT_OPEN_PRO_FAILED:
         return "Could not to open given pro file";
      case PM_PROJECT_READ_PRO_FAILED:
         return "Could not read given pro file";
      case PM_PROJECT_WRITE_PRO_FAILED:
         return "Could not write given pro file";
      case PM_PROJECT_ADD_SPEC_GROUP_FAILED:
         return "Could not add special group to given project";
      case PM_PROJECT_ADD_SPEC_IMAGE_FAILED:
         return "Could not add special image to given project";
      case PM_PROJECT_ADD_SPEC_SAMPLE_FAILED:
         return "Could not add special sample to given project";
      case PM_PROJECT_LOAD_INTERNAL_OBJ_FAILED:
         return "Could not load internal object";
      case PM_PROJECT_LOAD_GROUP_FAILED:
         return "Could not load group to object";
      case PM_PROJECT_COPY_GROUP_FAILED:
         return "Could not copy group";
      case PM_PROJECT_COPY_FILE_FAILED:
         return "Could not copy file";
      case  PM_PROJECT_EXPORT_CREATE_PATH_FAILED:
         return "Could not create a path";
      case  PM_PROJECT_EXPORT_COPY_FILE_FAILED:
         return "Could not copy file";
      case  PM_PROJECT_EXPORT_SAVE_IMAGE_FAILED:
         return "Could not save image";
      case  PM_PROJECT_EXPORT_WRONG_IMAGE_ID:
         return "Image have wrong id";
      case  PM_PROJECT_EXPORT_CREATE_FILE_FAILED:
         return "Could not create a file";
      case  PM_PROJECT_EXPORT_SAVE_SAMPLE_FAILED:
         return "Could not save sample sound";
      case  PM_PROJECT_EXPORT_READ_EDJ_FONT_FAILED:
         return "Could not read given edj file";
      case  PM_PROJECT_EXPORT_SAVE_FONT_FAILED:
         return "Could not save font";
      case  PM_PROJECT_EXPORT_CREATE_OBJECT_FAILED:
         return "Could not create a object";
      case  PM_PROJECT_EXPORT_DEVELOP_EDJ_FAILED:
         return "Could not save develop edj file";
      case  PM_PROJECT_EXPORT_RELEASE_EDJ_FAILED:
         return "Could not save release edj file";
      case  PM_PROJECT_BUILD_SOURCE_EDC_FAILED:
         return "Could not build edj file from edc";
      case PM_PROJECT_LAST:
      default:
         return "Unknown error";
     }
}

void
pm_project_file_reload(Project *project)
{
   eina_file_close(project->mmap_file);
   project->mmap_file = eina_file_open(project->dev, false);
   edje_object_mmap_set(project->global_object, project->mmap_file, EFLETE_INTERNAL_GROUP_NAME);
}
