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
#include "string_common.h"

static Eina_Bool
_resource_usage_resource_add(Resource2 *origin, Resource2 *used)
{
   used->common.used_in = eina_list_append(used->common.used_in, origin);
   origin->common.uses___ = eina_list_append(origin->common.uses___, used);
   return true;
}


static Eina_Bool __UNUSED__
_resource_usage_resource_del(Resource2 *origin, Resource2 *used)
{
   used->common.used_in = eina_list_remove(used->common.used_in, origin);
   origin->common.uses___ = eina_list_remove(origin->common.uses___, used);
   return true;
}

Resource2 *
resource_manager_find(const Eina_List *list, Eina_Stringshare *name)
{
   Resource2 *res = NULL, *data;
   const Eina_List *l;
   EINA_LIST_FOREACH(list, l, data)
     {
        if (data->common.name == name)
          {
             res = data;
             break;
          }
     }

   return res;
}

/*********************************************/
static Eina_Bool
_global_data_resources_load(Project *project)
{
   Global_Data2 *res;
   Eina_List *data;
   Eina_Stringshare *key;
   Eina_List *l;

   assert(project != NULL);

   data = edje_edit_data_list_get(project->global_object);

   EINA_LIST_FOREACH(data, l, key)
     {
        res = mem_calloc(1, sizeof(Global_Data2));
        res->common.type = RESOURCE2_TYPE_DATA_GLOBAL;
        res->common.name = eina_stringshare_add(key);
        res->source = edje_edit_data_value_get(project->global_object, key);
        project->global_data = eina_list_append(project->global_data, res);
     }

   edje_edit_string_list_free(data);
   return true;
}

static Eina_Bool
_image_set_resources_load(Project *project)
{
   Image_Set2 *res;
   Eina_List *images;
   Eina_Stringshare *image_name;
   Eina_List *l;

   assert(project != NULL);

   images = edje_edit_image_set_list_get(project->global_object);

   EINA_LIST_FOREACH(images, l, image_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!image_name) continue;

        res = mem_calloc(1, sizeof(Image_Set2));
        res->common.type = RESOURCE2_TYPE_IMAGE_SET;
        res->common.name = eina_stringshare_add(image_name);

        project->image_sets = eina_list_append(project->image_sets, res);

        res->common.id = edje_edit_image_set_id_get(project->global_object, image_name);
        res->is_used = false;
     }

   edje_edit_string_list_free(images);
   return true;
}

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
   Eina_List *colorclasses, *l;
   Colorclass2 *res;
   Eina_Stringshare *name;

   assert(project != NULL);

   colorclasses = edje_edit_color_classes_list_get(project->global_object);
   if (eina_list_count(colorclasses) == 0)
     {
        edje_edit_string_list_free(colorclasses);
        return false;
     }

   EINA_LIST_FOREACH(colorclasses, l, name)
     {
        res = mem_calloc(1, sizeof(Colorclass2));
        res->common.type = RESOURCE2_TYPE_COLORCLASS;
        res->common.name = eina_stringshare_add(name);

        if (!edje_edit_color_class_colors_get(project->global_object, name,
                                              &res->color1.r, &res->color1.g, &res->color1.b, &res->color1.a,
                                              &res->color2.r, &res->color2.g, &res->color2.b, &res->color2.a,
                                              &res->color3.r, &res->color3.g, &res->color3.b, &res->color3.a))
          {
             eina_stringshare_del(res->common.name);
             free(res);
          }
        else
          project->colorclasses = eina_list_append(project->colorclasses, res);
     }

   edje_edit_string_list_free(colorclasses);
   return true;
}

static Eina_Bool
_styles_resources_load(Project *project __UNUSED__)
{
   Eina_List *styles, *l;
   Style2 *res;
   Eina_Stringshare *name;

   assert(project != NULL);

   styles = edje_edit_styles_list_get(project->global_object);
   if (eina_list_count(styles) == 0)
     {
        edje_edit_string_list_free(styles);
        return false;
     }
   EINA_LIST_FOREACH(styles, l, name)
     {
        res = mem_calloc(1, sizeof(Style2));
        res->common.type = RESOURCE2_TYPE_STYLE;
        res->common.name = eina_stringshare_add(name);
        project->styles = eina_list_append(project->styles, res);

        TODO("parse all values and find dependencies in here like that:");
        /*
            STYLES uses FONT, COLOR_CLASS?, VIBRO?
         */
     }
   edje_edit_string_list_free(styles);
   return true;
}

/****************************************************/
static void
_group_name_parse(Group2 *group)
{
   char **c;
   unsigned int count;

   assert(group != NULL);

   if (!eina_str_has_prefix(group->common.name, "elm")) return;

   c = eina_str_split_full(group->common.name, "/", 4, &count);

   TODO("move here complicated class/style parsing from live_view");
   if (count == 4)
     {
        group->widget = eina_stringshare_add(c[1]);
        group->class = eina_stringshare_add(c[2]);
        group->style = eina_stringshare_add(c[3]);
     }

   free(c[0]);
   free(c);
}

static void
_resource_group_edit_object_load(Project *pro, Group2 *group, Evas *e)
{
   assert(pro != NULL);
   assert(group != NULL);
   assert(group->edit_object == NULL);

   group->edit_object = edje_edit_object_add(e);
   if (!edje_object_mmap_set(group->edit_object, pro->mmap_file, group->common.name))
     {
        ERR("Can't set mmap object");
        abort();
     }
}

void
_resource_group_edit_object_unload(Group2 *group)
{
   assert(group != NULL);
   assert(group->edit_object != NULL);

   evas_object_del(group->edit_object);
   group->edit_object = NULL;
}

State2 *
_gm_state_add(Project *pro, Part2 *part, const char *state_name, double state_value)
{
   State2 *state;

   assert(pro != NULL);
   assert(part != NULL);
   assert(state_name != NULL);

   state = mem_calloc(1, sizeof(State2));
   state->common.type = RESOURCE2_TYPE_STATE;
   state->common.name = eina_stringshare_add(state_name);
   state->val = state_value;
   state->part = part;
   part->states = eina_list_append(part->states, state);

   /* default 0.0 should be allways first state */
   if (part->current_state == NULL)
     part->current_state = state;

   return state;
}

Part2 *
_gm_part_add(Project *pro, Group2 *group, const char *part_name)
{
   Part_Item2 *item;
   Part2 *part;
   Eina_List *states, *items, *l;
   Eina_Stringshare *state_name, *parsed_state_name, *item_name;
   double val;

   assert(pro != NULL);
   assert(group != NULL);
   assert(part_name != NULL);

   part = mem_calloc(1, sizeof(Part2));
   part->common.type = RESOURCE2_TYPE_PART;
   part->common.name = eina_stringshare_add(part_name);
   part->type = edje_edit_part_type_get(group->edit_object, part_name);
   part->visible = true;
   part->group = group;
   group->parts = eina_list_append(group->parts, part);

   states = edje_edit_part_states_list_get(group->edit_object, part_name);
   EINA_LIST_FOREACH(states, l, state_name)
     {
        state_name_split(state_name, &parsed_state_name, &val);
        _gm_state_add(pro, part, parsed_state_name, val);
        eina_stringshare_del(parsed_state_name);
     }
   edje_edit_string_list_free(states);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     {
        items = edje_edit_part_items_list_get(group->edit_object, part_name);
        EINA_LIST_FOREACH(items, l, item_name)
          {
             TODO("Remove this after switching to index API for items");
             if (!item_name) continue;
             item = mem_calloc(1, sizeof(Part_Item2));
             item->common.type = RESOURCE2_TYPE_ITEM;
             item->common.name = eina_stringshare_add(item_name);
             item->part = part;
             part->items = eina_list_append(part->items, part);
          }
        edje_edit_string_list_free(items);
     }

   return part;
}

void
_gm_group_data_add(Project *pro, Group2 *group, Eina_Stringshare *group_data_name)
{
   Group_Data2 *group_data;

   assert(pro != NULL);
   assert(group_data_name != NULL);
   assert(group != NULL);

   group_data = mem_calloc(1, sizeof(Program2));
   group_data->common.type = RESOURCE2_TYPE_DATA_GROUP;
   group_data->common.name = eina_stringshare_add(group_data_name);
   group_data->source = edje_edit_group_data_value_get(group->edit_object,
                                                       group_data_name);
   group->data_items = eina_list_append(group->data_items, group_data);
}

static void
_group_load(Project *pro, Group2 *group)
{
   Eina_List *parts, *l, *programs, *datas;
   Eina_Stringshare *part_name, *program_name, *group_data_name;
   Program2 *program;

   assert(pro != NULL);
   assert(group != NULL);

   _group_name_parse(group);

   _resource_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
   if (!edje_edit_group_alias_is(group->edit_object, group->common.name))
     {
        parts = edje_edit_parts_list_get(group->edit_object);
        EINA_LIST_FOREACH(parts, l, part_name)
           _gm_part_add(pro, group, part_name);
        edje_edit_string_list_free(parts);

        datas = edje_edit_group_data_list_get(group->edit_object);
        EINA_LIST_FOREACH(datas, l, group_data_name)
           _gm_group_data_add(pro, group, group_data_name);

        programs = edje_edit_programs_list_get(group->edit_object);
        EINA_LIST_FOREACH(programs, l, program_name)
          {
             program  = mem_calloc(1, sizeof(Program2));
             program->common.type = RESOURCE2_TYPE_PROGRAM;
             program->common.name = eina_stringshare_add(program_name);
             program->type = edje_edit_program_action_get(group->edit_object,
                                                          program_name);
             group->programs = eina_list_append(group->programs, program);
          }
        edje_edit_string_list_free(programs);
     }

   _resource_group_edit_object_unload(group);
}

void
_gm_groups_load(Project *pro)
{
   Eina_List *collections, *l;
   Eina_Stringshare *group_name;
   Group2 *res;

   assert(pro != NULL);
   assert(pro->dev != NULL);
   assert(pro->groups == NULL);

   collections = edje_file_collection_list(pro->dev);

   assert(collections != NULL);

   collections = eina_list_sort(collections, eina_list_count(collections), (Eina_Compare_Cb) strcmp);
   EINA_LIST_FOREACH(collections, l, group_name)
     {
        if (!strcmp(group_name, EFLETE_INTERNAL_GROUP_NAME)) continue;
        res = mem_calloc(1, sizeof(Group2));
        res->common.type = RESOURCE2_TYPE_GROUP;
        res->common.name = eina_stringshare_add(group_name);
        pro->groups = eina_list_append(pro->groups, res);
     }
   edje_file_collection_list_free(collections);

   EINA_LIST_FOREACH(pro->groups, l, res)
     _group_load(pro, res);
}

/************************ dependency load ***********************************/
void
_item_dependency_load(Project *pro __UNUSED__, Group2 *group __UNUSED__, Part2 *part __UNUSED__)
{
}

void
_state_dependency_load(Project *pro, Group2 *group, Part2 *part)
{
   Eina_List *l2, *l, *tween_list;
   State2 *state;
   Resource2 *res;
   Eina_Stringshare *source, *image_name, *tween_name;
   Eina_Stringshare *font_name, *color_class, *style_name;

   EINA_LIST_FOREACH(part->states, l, state)
     {
        if (part->type == EDJE_PART_TYPE_PROXY)
          {
             source = edje_edit_state_proxy_source_get(group->edit_object,
                                                       part->common.name,
                                                       state->common.name,
                                                       state->val);
             res = resource_manager_find(group->parts, source);
             if (res)
               _resource_usage_resource_add((Resource2 *)state, res);
             edje_edit_string_free(source);
          }

        if (part->type == EDJE_PART_TYPE_IMAGE)
          {
             image_name = edje_edit_state_image_get(group->edit_object,
                                                    part->common.name,
                                                    state->common.name,
                                                    state->val);

             if (strcmp(image_name, EFLETE_DUMMY_IMAGE_NAME))
               {
                  if (edje_edit_image_set_exists(group->edit_object, image_name))
                    res = resource_manager_find(pro->image_sets, image_name);
                  else
                    res = resource_manager_find(pro->images, image_name);
                  _resource_usage_resource_add((Resource2 *)state, res);
               }
             edje_edit_string_free(image_name);

             tween_list = edje_edit_state_tweens_list_get(group->edit_object,
                                                          part->common.name,
                                                          state->common.name,
                                                          state->val);
             EINA_LIST_FOREACH(tween_list, l2, tween_name)
               {
                  if (edje_edit_image_set_exists(group->edit_object, tween_name))
                    res = resource_manager_find(pro->image_sets, tween_name);
                  else
                    res = resource_manager_find(pro->images, tween_name);
                  _resource_usage_resource_add((Resource2 *)state, res);
               }
             edje_edit_string_list_free(tween_list);
          }

        color_class = edje_edit_state_color_class_get(group->edit_object,
                                                      part->common.name,
                                                      state->common.name,
                                                      state->val);
        res = resource_manager_find(pro->colorclasses, color_class);
        if (res)
          _resource_usage_resource_add((Resource2 *)state, res);
        edje_edit_string_free(color_class);

        if (part->type == EDJE_PART_TYPE_TEXT)
          {
             font_name = edje_edit_state_font_get(group->edit_object,
                                                  part->common.name,
                                                  state->common.name,
                                                  state->val);
             res = resource_manager_find(pro->fonts, font_name);
             if (res)
               _resource_usage_resource_add((Resource2 *)state, res);
             edje_edit_string_free(font_name);

             source = edje_edit_state_text_source_get(group->edit_object,
                                                      part->common.name,
                                                      state->common.name,
                                                      state->val);
             res = resource_manager_find(group->parts, source);
             if (res)
               _resource_usage_resource_add((Resource2 *)state, res);
             edje_edit_string_free(source);

             source = edje_edit_state_text_text_source_get(group->edit_object,
                                                           part->common.name,
                                                           state->common.name,
                                                           state->val);
             res = resource_manager_find(group->parts, source);
             if (res)
               _resource_usage_resource_add((Resource2 *)state, res);
             edje_edit_string_free(source);
          }

        if (part->type == EDJE_PART_TYPE_TEXTBLOCK)
          {
             style_name = edje_edit_state_text_style_get(group->edit_object,
                                                         part->common.name,
                                                         state->common.name,
                                                         state->val);
             res = resource_manager_find(pro->styles, style_name);
             _resource_usage_resource_add((Resource2 *)state, res);
             edje_edit_string_free(style_name);
          }
        TODO("Implement size_class and text_class stuff")
           /*
              Relationships uses - used:
              STATE - (unimplemented yet) SIZE_CLASS, TEXT_CLASS
            */
     }
}

void
_part_dependency_load(Project *pro, Group2 *group)
{
   Part2 *part;
   Eina_Stringshare *source;
   Resource2 *res;
   Eina_List *l;

   EINA_LIST_FOREACH(group->parts, l, part)
     {
        source = edje_edit_part_source_get(group->edit_object, part->common.name);
        res = resource_manager_find(pro->groups, source);
        if (res)
          _resource_usage_resource_add((Resource2 *)part, res);
        edje_edit_string_free(source);

        _state_dependency_load(pro, group, part);
        _item_dependency_load(pro, group, part);
     }
}

void
_program_dependency_load(Project *pro __UNUSED__, Group2 *group)
{
   Eina_List *l;
   Program2 *program __UNUSED__;

   EINA_LIST_FOREACH(group->programs, l, program)
     {
        TODO("Next usage and dependencies described below: ");
        /*
           GROUP - GROUP relationships (alias)
           PROGRAM - PART, STATE, PROGRAM, LIMIT?, SAMPLE, TONE
         */
     }
}

void
_resource_dependency_load(Project *pro)
{
   Group2 *group;
   Resource2 *res, *used;
   Eina_Stringshare *main_group_name, *set_image_name;
   Eina_List *set_images;
   Eina_List *l1, *l2;

   /* image_set */
   EINA_LIST_FOREACH(pro->image_sets, l1, res)
     {
        set_images = edje_edit_image_set_images_list_get(pro->global_object, res->common.name);
        EINA_LIST_FOREACH(set_images, l2, set_image_name)
          {
             used = resource_manager_find(pro->images, set_image_name);
             _resource_usage_resource_add(res, used);
          }
        edje_edit_string_list_free(set_images);
     }

   /* groups */
   EINA_LIST_FOREACH(pro->groups, l1, group)
     {
        _resource_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
        if (edje_edit_group_alias_is(group->edit_object, group->common.name))
          {
             main_group_name = edje_edit_group_aliased_get(group->edit_object, group->common.name);
             TODO("Add aliased groups as resource");
             //resource_insert(&group->main_group->aliases, (Resource *)group);
             edje_edit_string_free(main_group_name);
          }
        else
          {
             _part_dependency_load(pro, group);
             _program_dependency_load(pro, group);
          }
        _resource_group_edit_object_unload(group);
     }
}

/******************* public API ********************/

Eina_Bool
resource_manager_init(Project *project)
{
   /* loading resources are in here */
   _image_resources_load(project);
   _image_set_resources_load(project);
   _sound_resources_load(project);
   _font_resources_load(project);
   _tones_resources_load(project);
   _colorclasses_resources_load(project);
   _styles_resources_load(project);
   _global_data_resources_load(project);

   _gm_groups_load(project);

   _resource_dependency_load(project);

   return false;
}

Eina_Bool
resource_manager_shutdown(Project *project __UNUSED__)
{
   return false;
}

Resource2 *
resource_manager_v_find(const Eina_List *list __UNUSED__, Eina_Stringshare *name __UNUSED__, double value __UNUSED__)
{
   return NULL;
}
