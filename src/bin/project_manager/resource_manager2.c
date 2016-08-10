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
//   int im_total, im_proc;

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
//   im_total = eina_list_count(images);
//   im_proc = 0;
//   PROGRESS_SEND(_("Start image processing, total %d:"), im_total);
   EINA_LIST_FOREACH(images, l, image_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!image_name) continue;

//        im_proc++;
//        PROGRESS_SEND(_("image processing (%d/%d): %s"),
//                      im_proc, im_total, image_name);

        res = mem_calloc(1, sizeof(Image2));
        res->common.type = RESOURCE_TYPE_IMAGE;
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
//             THREAD_CONTEXT_SWITCH_BEGIN;
             im = evas_object_image_add(e);
             res->common.id = edje_edit_image_id_get(project->global_object, image_name);
             res->is_set = false;
             res->is_used = false;
             source_file = eina_stringshare_printf("edje/images/%i", res->common.id);
             evas_object_image_file_set(im, project->dev, source_file);
//             THREAD_CONTEXT_SWITCH_END;
             evas_object_image_save(im, res->source, NULL, NULL);
//             THREAD_CONTEXT_SWITCH_BEGIN;
             evas_object_del(im);
             eina_stringshare_del(source_file);
//             THREAD_CONTEXT_SWITCH_END;
          }
     }

   edje_edit_string_list_free(images);
   eina_stringshare_del(resource_folder);
   return true;
}

static Eina_Bool
_sound_resources_load(Project *project __UNUSED__)
{
   return false;
}

static Eina_Bool
_font_resources_load(Project *project __UNUSED__)
{
   return false;
}

static Eina_Bool
_tones_resources_load(Project *project __UNUSED__)
{
   return false;
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
