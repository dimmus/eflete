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
/* --------------- Common functions ---------------------*/



void
_resources_export_feedback_cb(void *data __UNUSED__,
                             Ecore_Thread *th __UNUSED__,
                             void *msg_data)
{
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Eina_Stringshare *message = (Eina_Stringshare *)msg_data;
   assert(message != NULL);
   assert(ptd != NULL);

   ptd->func_progress(NULL, message);
   eina_stringshare_del(message);
}

void
_resources_export_cancel_cb(void *data __UNUSED__, Ecore_Thread *th __UNUSED__)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   ecore_thread_global_data_del("ptd");
   ptd->result = PM_PROJECT_ERROR;
   ERR("Failed to export resources.");
   ptd->result = PM_PROJECT_ERROR;
   free(ftd);
   _end_send(ptd);
}

void
_resources_export_end_cb(void *data __UNUSED__, Ecore_Thread *th __UNUSED__)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;

   ftd->queue--;
   if (ftd->queue != 0)
     return;

   ecore_thread_feedback_run(_gm_group_load_feedback_job, _resources_export_feedback_cb,
                             _gm_group_load_end_cb, _gm_group_load_cancel_cb, ftd,
                             true);
}

/* --------------- Export images ---------------------*/
typedef struct
{
   Evas *e;
   Evas_Object *im;
   const char *id;
   const char *dev;
   const char *source;
   Eina_Lock mutex;
} Image_Data_Save;

static void*
_image_save_routine(void *data)
{
   Image_Data_Save *ids = (Image_Data_Save *)data;
   eina_lock_take(&ids->mutex);

   ids->im = evas_object_image_add(ids->e);
   evas_object_image_file_set(ids->im, ids->dev, ids->id);
   evas_object_image_save(ids->im, ids->source, NULL, NULL);
   evas_object_del(ids->im);
   eina_lock_release(&ids->mutex);
   return NULL;
}

void
_image_resources_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Project *project = (Project *) ptd->project;
   External_Resource *res;
   Eina_List *images;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *image_name, *source_file;
   Eina_List *l;
   Evas *e;
   int id;
   char *file_dir;
   int im_total, im_proc;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export images", resource_folder);
        eina_stringshare_del(resource_folder);
        eina_lock_release(&ftd->mutex);
        ecore_thread_cancel(th);
        return;
     }

   images = edje_edit_images_list_get(project->global_object);

   e = ecore_evas_get(project->ecore_evas);
   im_total = eina_list_count(images);
   im_proc = 0;
   Edje_Edit_Image_Comp comp_type;

   Eina_Stringshare *message = eina_stringshare_printf(_("Start image processing, total %d:"), im_total);
   ecore_thread_feedback(th, message);

   Image_Data_Save *ids = mem_calloc(1, sizeof(Image_Data_Save));
   ids->e = e;
   ids->dev = project->dev;
   eina_lock_new(&ids->mutex);
   eina_lock_take(&ids->mutex);
   EINA_LIST_FOREACH(images, l, image_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!image_name) continue;

        im_proc++;

        message = eina_stringshare_printf(_("image processing (%d/%d): %s"), im_proc, im_total, image_name);
        ecore_thread_feedback(th, message);

        res = (External_Resource *) resource_add(image_name, RESOURCE_TYPE_IMAGE);
        comp_type = edje_edit_image_compression_type_get(project->global_object,
                                                         res->name);
        if (comp_type == EDJE_EDIT_IMAGE_COMP_USER)
          res->source = eina_stringshare_add(image_name);
        else
          res->source = eina_stringshare_printf("%s/%s", resource_folder, image_name);
        resource_insert(&project->images, (Resource *)res);

        if (!ecore_file_exists(res->source))
          {
             file_dir = ecore_file_dir_get(res->source);
             ecore_file_mkpath(file_dir);
             free(file_dir);
             id = edje_edit_image_id_get(project->global_object, image_name);
             if (id < 0)
               {
                  message = eina_stringshare_printf(_("Image %s coudn't be exported"), image_name);
                  ecore_thread_feedback(th, message);
                  WARN("Image %s coudn't be exported", image_name);
                  sleep(2);
                  continue;
               }
             source_file = eina_stringshare_printf("edje/images/%i", id);
             ids->id = source_file;
             ids->im = NULL;
             ids->source = res->source;
             eina_lock_release(&ids->mutex);
             ecore_main_loop_thread_safe_call_sync(_image_save_routine, ids);
             eina_lock_take(&ids->mutex);
             eina_stringshare_del(source_file);
          }
     }
   eina_lock_release(&ids->mutex);
   eina_lock_free(&ids->mutex);
   free(ids);

   edje_edit_string_list_free(images);
   eina_stringshare_del(resource_folder);
   eina_lock_release(&ftd->mutex);
}


/* --------------- Export sounds ---------------------*/
void
_sound_resources_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Project *project = (Project *) ptd->project;
   External_Resource *res;
   Eina_List *sounds;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *sound_name, *sound_file;
   Eina_List *l;
   Eina_Binbuf *sound_bin;
   FILE *f;
   char *file_dir;
   int snd_total, snd_proc;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export sounds", resource_folder);
        eina_stringshare_del(resource_folder);
        eina_lock_release(&ftd->mutex);
        ecore_thread_cancel(th);
        return;
     }

   sounds = edje_edit_sound_samples_list_get(project->global_object);

   snd_total = eina_list_count(sounds);
   snd_proc = 0;
   Eina_Stringshare *message = eina_stringshare_printf(_("Start sound processing: total %d:"), snd_total);
   ecore_thread_feedback(th, message);


   EINA_LIST_FOREACH(sounds, l, sound_name)
     {
        sound_file = edje_edit_sound_samplesource_get(project->global_object, sound_name);
        snd_proc++;

        message = eina_stringshare_printf(_("sound processing (%d/%d): %s"), snd_proc, snd_total, sound_file);
        ecore_thread_feedback(th, message);

        res = (External_Resource*)resource_add(sound_name, RESOURCE_TYPE_SOUND);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, sound_file);
        resource_insert(&project->sounds, (Resource *)res);

        if (!ecore_file_exists(res->source))
          {
             file_dir = ecore_file_dir_get(res->source);
             ecore_file_mkpath(file_dir);
             free(file_dir);
             sound_bin = edje_edit_sound_samplebuffer_get(project->global_object, sound_name);
             if (!(f = fopen(res->source, "wb")))
               {
                  message = eina_stringshare_printf(_("Could not open file: %s"), res->source);
                  ecore_thread_feedback(th, message);
                  ERR("Could not open file: %s", res->source);
                  sleep(2);
                  continue;
               }
             if (fwrite(eina_binbuf_string_get(sound_bin),
                        eina_binbuf_length_get(sound_bin), 1, f) != 1)
               ERR("Could not write sound: %s", strerror(errno));
             if (f) fclose(f);
             eina_binbuf_free(sound_bin);
          }
        edje_edit_string_free(sound_file);
     }
   eina_lock_release(&ftd->mutex);

   edje_edit_string_list_free(sounds);
   eina_stringshare_del(resource_folder);
   return;
}

/* --------------- Export fonts ---------------------*/
void
_font_resources_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Project *project = (Project *) ptd->project;
   External_Resource *res;
   Eina_List *fonts;
   Eina_Stringshare *resource_folder;
   Eet_File *ef;
   Eina_List *l;
   Eina_Stringshare *font_name, *font_file;
   void *font;
   FILE *f;
   int size, fnt_total, fnt_proc;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/fonts", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   fonts = edje_edit_fonts_list_get(project->global_object);

   ef = eet_open(project->dev, EET_FILE_MODE_READ);
   fnt_total = eina_list_count(fonts);
   if (fnt_total == 0)
     {
        eina_condition_signal(&ftd->condition);
        eina_lock_release(&ftd->mutex);
        eet_close(ef);
        edje_edit_string_list_free(fonts);
        eina_stringshare_del(resource_folder);
        return;
     }

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export fonts", resource_folder);
        eina_stringshare_del(resource_folder);
        eina_lock_release(&ftd->mutex);
        ecore_thread_cancel(th);
       return;
     }

   fnt_proc = 0;
   Eina_Stringshare *message = eina_stringshare_printf(_("Start font processing, total %d:"), fnt_total);
   ecore_thread_feedback(th, message);

   EINA_LIST_FOREACH(fonts, l, font_name)
     {
        font_file = edje_edit_font_path_get(project->global_object, font_name);
        fnt_proc++;
        message = eina_stringshare_printf(_("font processing (%d/%d): %s"), fnt_proc, fnt_total, font_file);
        ecore_thread_feedback(th, message);

        res = (External_Resource *)resource_add(font_file, RESOURCE_TYPE_FONT);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, font_file);
        resource_insert(&project->fonts, (Resource *)res);

        if (!ecore_file_exists(res->source))
          {
             edje_edit_string_free(font_file);
             font_file = eina_stringshare_printf("edje/fonts/%s", font_name);
             font = eet_read(ef, font_file, &size);
             if (!font) continue;
             if (!(f = fopen(res->source, "wb")))
               {
                  message = eina_stringshare_printf(_("Could not open file: %s"), res->source);
                  ecore_thread_feedback(th, message);
                  ERR("Could not open file: %s", res->source);
                  sleep(2);
                  continue;
               }
             if (fwrite(font, size, 1, f) != 1)
               ERR("Could not write font: %s", strerror(errno));
             if (f) fclose(f);
             free(font);
             eina_stringshare_del(font_file);
          }
     }
   eet_close(ef);
   edje_edit_string_list_free(fonts);
   eina_stringshare_del(resource_folder);
   eina_lock_release(&ftd->mutex);
   return;
}

/* --------------- Export tones ---------------------*/
void
_tones_resources_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Project *project = (Project *) ptd->project;
   Eina_List *tones, *l;
   Tone_Resource *res;
   Eina_Stringshare *name;
   int tones_total, tones_proc = 0;

   assert(project != NULL);

   tones = edje_edit_sound_tones_list_get(project->global_object);
   tones_total = eina_list_count(tones);

   if (tones_total == 0)
     {
        eina_lock_release(&ftd->mutex);
        edje_edit_string_list_free(tones);
        return;
     }

   Eina_Stringshare *message = eina_stringshare_printf(_("Start tone processing, total %d:"), tones_total);
   ecore_thread_feedback(th, message);

   EINA_LIST_FOREACH(tones, l, name)
     {
        message = eina_stringshare_printf(_("tone processing (%d/%d): %s"), ++tones_proc, tones_total, name);
        ecore_thread_feedback(th, message);

        res = (Tone_Resource *)resource_add(name, RESOURCE_TYPE_TONE);
        res->freq = edje_edit_sound_tone_frequency_get(project->global_object, name);
        resource_insert(&project->tones, (Resource *)res);
     }
    eina_lock_release(&ftd->mutex);
    edje_edit_string_list_free(tones);
    return;
}

/* --------------- Export colorclasses ---------------------*/
void
_colorclasses_resources_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Project *project = (Project *) ptd->project;
   Eina_List *colorclasses, *l;
   Colorclass_Resource *res;
   Eina_Stringshare *name;
   int cc_total, cc_proc = 0;

   assert(project != NULL);

   colorclasses = edje_edit_color_classes_list_get(project->global_object);
   cc_total = eina_list_count(colorclasses);

   if (cc_total == 0)
     {
        eina_lock_release(&ftd->mutex);
        edje_edit_string_list_free(colorclasses);
        return;
     }

   Eina_Stringshare *message = eina_stringshare_printf(_("Start colorclass processing, total %d:"), cc_total);
   ecore_thread_feedback(th, message);

   EINA_LIST_FOREACH(colorclasses, l, name)
     {
        message = eina_stringshare_printf(_("colorclass processing (%d/%d): %s"), ++cc_proc, cc_total, name);
        ecore_thread_feedback(th, message);

        res = (Colorclass_Resource *)resource_add(name, RESOURCE_TYPE_COLORCLASS);

        if (!edje_edit_color_class_colors_get(project->global_object, name,
                                              &res->color1.r, &res->color1.g, &res->color1.b, &res->color1.a,
                                              &res->color2.r, &res->color2.g, &res->color2.b, &res->color2.a,
                                              &res->color3.r, &res->color3.g, &res->color3.b, &res->color3.a))
          {
             eina_stringshare_del(res->name);
             resource_free((Resource *)res);
          }
        else
          resource_insert(&project->colorclasses, (Resource *)res);
     }

   edje_edit_string_list_free(colorclasses);
   eina_lock_release(&ftd->mutex);
   return;
}

/* --------------- Export styles ---------------------*/
void
_styles_resources_feedback_job(void *data, Ecore_Thread *th)
{
   Feedback_Thread_Data *ftd = (Feedback_Thread_Data *)data;
   if (!eina_lock_take(&ftd->mutex))
     {
       ERR("Failed access data");
       ecore_thread_cancel(th);
       return;
     }
   Project_Thread *ptd = ecore_thread_global_data_find("ptd");
   Project *project = (Project *) ptd->project;

   Eina_List *styles, *l;
   Resource *res;
   Eina_Stringshare *name;
   int styles_total, styles_proc = 0;

   assert(project != NULL);

   styles = edje_edit_styles_list_get(project->global_object);
   styles_total = eina_list_count(styles);

   if (styles_total == 0)
     {
        eina_lock_release(&ftd->mutex);
        edje_edit_string_list_free(styles);
        return;
     }

   Eina_Stringshare *message = eina_stringshare_printf(_("Start style processing, total %d:"), styles_total);
   ecore_thread_feedback(th, message);

   EINA_LIST_FOREACH(styles, l, name)
     {
        message = eina_stringshare_printf(_("style processing (%d/%d): %s"), ++styles_proc, styles_total, name);
        ecore_thread_feedback(th, message);

        res = resource_add(name, RESOURCE_TYPE_STYLE);
        resource_insert(&project->styles, res);
     }

   eina_lock_release(&ftd->mutex);
   edje_edit_string_list_free(styles);
   return;
}

