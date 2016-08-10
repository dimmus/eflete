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

#include "resource_manager2.h"
#include "project_manager.h"

static Eina_Bool __UNUSED__
_resource_usage_resource_add(Resource2 *origin __UNUSED__, Resource2 *used __UNUSED__)
{
   return false;
}


static Eina_Bool __UNUSED__
_resource_usage_resource_del(Resource2 *origin __UNUSED__, Resource2 *used __UNUSED__)
{
   return false;
}

/*********************************************/

static Eina_Bool
_image_resources_load(Project *project)
{
   Image2 *res;
   Eina_List *images;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *image_name, *source_file;
   Eina_List *l;
   Evas *e;
   Evas_Object *im;
   char *file_dir;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export images", resource_folder);
        eina_stringshare_del(resource_folder);
        return false;
     }

   images = edje_edit_images_list_get(project->global_object);

   e = ecore_evas_get(project->ecore_evas);
   EINA_LIST_FOREACH(images, l, image_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!image_name) continue;

        res = mem_calloc(1, sizeof(Image2));
        res->common.type = RESOURCE2_TYPE_IMAGE;
        res->common.name = eina_stringshare_add(image_name);
        res->comp_type = edje_edit_image_compression_type_get(project->global_object,
                                                              res->common.name);
        if (res->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
          res->source = eina_stringshare_add(image_name);
        else
          res->source = eina_stringshare_printf("%s/%s", resource_folder, image_name);

        project->images = eina_list_append(project->images, res);

        if (!ecore_file_exists(res->source))
          {
             file_dir = ecore_file_dir_get(res->source);
             ecore_file_mkpath(file_dir);
             free(file_dir);
             im = evas_object_image_add(e);
             res->common.id = edje_edit_image_id_get(project->global_object, image_name);
             res->is_set = false;
             res->is_used = false;
             source_file = eina_stringshare_printf("edje/images/%i", res->common.id);
             evas_object_image_file_set(im, project->dev, source_file);
             evas_object_image_save(im, res->source, NULL, NULL);
             evas_object_del(im);
             eina_stringshare_del(source_file);
          }
     }

   edje_edit_string_list_free(images);
   eina_stringshare_del(resource_folder);
   return true;
}

static Eina_Bool
_sound_resources_load(Project *project __UNUSED__)
{
   Sound2 *res;
   Eina_List *sounds;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *sound_name, *sound_file;
   Eina_List *l;
   Eina_Binbuf *sound_bin;
   FILE *f;
   char *file_dir;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export sounds", resource_folder);
        eina_stringshare_del(resource_folder);
        return false;
     }

   sounds = edje_edit_sound_samples_list_get(project->global_object);
   EINA_LIST_FOREACH(sounds, l, sound_name)
     {
        sound_file = edje_edit_sound_samplesource_get(project->global_object, sound_name);

        res = mem_calloc(1, sizeof(Sound2));
        res->common.type = RESOURCE2_TYPE_SAMPLE;
        res->common.name = eina_stringshare_add(sound_name);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, sound_file);

        project->sounds = eina_list_append(project->sounds, res);

        if (!ecore_file_exists(res->source))
          {
             file_dir = ecore_file_dir_get(res->source);
             ecore_file_mkpath(file_dir);
             free(file_dir);
             sound_bin = edje_edit_sound_samplebuffer_get(project->global_object, sound_name);
             if (!(f = fopen(res->source, "wb")))
               {
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

   edje_edit_string_list_free(sounds);
   eina_stringshare_del(resource_folder);
   return true;
}

static Eina_Bool
_font_resources_load(Project *project __UNUSED__)
{
   Font2 *res;
   Eina_List *fonts;
   Eina_Stringshare *resource_folder;
   Eet_File *ef;
   Eina_List *l;
   Eina_Stringshare *font_name, *font_file;
   void *font;
   FILE *f;
   int size;

   Eina_Bool result = true;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/fonts", project->develop_path);
   ecore_file_recursive_rm(resource_folder);

   fonts = edje_edit_fonts_list_get(project->global_object);

   ef = eet_open(project->dev, EET_FILE_MODE_READ);
   if (eina_list_count(fonts) == 0)
     {
        res = false;
        goto cleanup;
     }

   if (!ecore_file_mkpath(resource_folder))
     {
        ERR("Failed create path %s for export fonts", resource_folder);
        res = false;
        goto cleanup;
     }

   EINA_LIST_FOREACH(fonts, l, font_name)
     {
        font_file = edje_edit_font_path_get(project->global_object, font_name);

        res = mem_calloc(1, sizeof(Font2));
        res->common.type = RESOURCE2_TYPE_FONT;
        res->common.name = eina_stringshare_add(font_name);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, font_file);

        project->fonts = eina_list_append(project->fonts, res);

        if (!ecore_file_exists(res->source))
          {
             edje_edit_string_free(font_file);
             font_file = eina_stringshare_printf("edje/fonts/%s", font_name);
             font = eet_read(ef, font_file, &size);
             if (!font) continue;
             if (!(f = fopen(res->source, "wb")))
               {
                  ERR("Could not open file: %s", res->source);
                  continue;
               }
             if (fwrite(font, size, 1, f) != 1)
               ERR("Could not write font: %s", strerror(errno));
             fclose(f);
             free(font);
             eina_stringshare_del(font_file);
          }
     }

cleanup:
   eet_close(ef);
   edje_edit_string_list_free(fonts);
   eina_stringshare_del(resource_folder);
   return result;
}

static Eina_Bool
_tones_resources_load(Project *project __UNUSED__)
{
   Eina_List *tones, *l;
   Tone2 *res;
   Eina_Stringshare *name;

   assert(project != NULL);

   tones = edje_edit_sound_tones_list_get(project->global_object);

   if (eina_list_count(tones) == 0)
     {
        edje_edit_string_list_free(tones);
        return false;
     }

   EINA_LIST_FOREACH(tones, l, name)
     {
        res = mem_calloc(1, sizeof(Tone2));
        res->common.type = RESOURCE2_TYPE_TONE;
        res->common.name = eina_stringshare_add(name);
        res->freq = edje_edit_sound_tone_frequency_get(project->global_object, name);
        project->tones = eina_list_append(project->tones, res);
     }
    edje_edit_string_list_free(tones);

   return true;
}

static Eina_Bool
_colorclasses_resources_load(Project *project __UNUSED__)
{
   return false;
}

static Eina_Bool
_styles_resources_load(Project *project __UNUSED__)
{
   return false;
}

/******************* public API ********************/

Eina_Bool
resource_manager_init(Project *project)
{
   /* loading resources are in here */
   _image_resources_load(project);
   _sound_resources_load(project);
   _font_resources_load(project);
   _tones_resources_load(project);
   _colorclasses_resources_load(project);
   _styles_resources_load(project);

//   gm_groups_load(project);

   return false;
}

Eina_Bool
resource_manager_shutdown(Project *project __UNUSED__)
{
   return false;
}

Resource2 *
resource_manager_find(const Eina_List *list __UNUSED__, Eina_Stringshare *name __UNUSED__)
{
   return NULL;
}

Resource2 *
resource_manager_v_find(const Eina_List *list __UNUSED__, Eina_Stringshare *name __UNUSED__, double value __UNUSED__)
{
   return NULL;
}
