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
#include "resource_manager_private.h"
#include "project_manager.h"
#include "string_common.h"

/***********************************************/

Eina_Bool
_resource_usage_resource_add(Resource2 *origin, Resource2 *used)
{
   used->common.used_in = eina_list_append(used->common.used_in, origin);
   origin->common.uses___ = eina_list_append(origin->common.uses___, used);
   return true;
}

Eina_Bool
_resource_usage_resource_del(Resource2 *origin, Resource2 *used)
{
   used->common.used_in = eina_list_remove(used->common.used_in, origin);
   origin->common.uses___ = eina_list_remove(origin->common.uses___, used);
   return true;
}

/***********************************************/

void
_item_dependency_load(Project* pro, Group2 *group, Part2 *part)
{
   Part_Item2 *item;
   Resource2 *used;
   Eina_List *l;
   Eina_Stringshare *source;

   EINA_LIST_FOREACH(part->items, l, item)
     {
        source = edje_edit_part_item_source_get(group->edit_object,
                                                part->common.name,
                                                item->common.name);
        used = resource_manager_find(pro->groups2, source);
        if (used)
          _resource_usage_resource_add((Resource2 *)item, used);
        edje_edit_string_free(source);
     }
}

void
_state_dependency_load(Project *pro, Group2 *group, Part2 *part)
{
   Eina_List *l2, *l;
   State2 *state;
   Resource2 *res;
   Eina_Stringshare *source;
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
             if (strcmp(state->normal, EFLETE_DUMMY_IMAGE_NAME))
               {
                  if (edje_edit_image_set_exists(group->edit_object, state->normal))
                    res = resource_manager_find(pro->image_sets, state->normal);
                  else
                    res = resource_manager_find(pro->images, state->normal);
                  _resource_usage_resource_add((Resource2 *)state, res);
               }

             EINA_LIST_FOREACH(state->tweens, l2, res)
               {
                  _resource_usage_resource_add((Resource2 *)state, res);
               }
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
        res = resource_manager_find(pro->groups2, source);
        if (res)
          _resource_usage_resource_add((Resource2 *)part, res);
        edje_edit_string_free(source);

        _state_dependency_load(pro, group, part);
        _item_dependency_load(pro, group, part);
     }
}

void
_program_dependency_load(Project *pro, Group2 *group)
{
   Eina_List *l, *l2;
   Program2 *program;
   Resource2 *res, *res_state;
   Eina_Stringshare *name, *state;

   EINA_LIST_FOREACH(group->programs, l, program)
     {
        if (program->type == EDJE_ACTION_TYPE_SOUND_SAMPLE)
          {
             name = edje_edit_program_sample_name_get(group->edit_object, program->common.name);
             res = resource_manager_find(pro->sounds, name);
             if (res)
               _resource_usage_resource_add((Resource2 *)program, res);
             edje_edit_string_free(name);
          }
        if (program->type == EDJE_ACTION_TYPE_SOUND_TONE)
          {
             name = edje_edit_program_tone_name_get(group->edit_object, program->common.name);
             res = resource_manager_find(pro->tones, name);
             if (res)
               _resource_usage_resource_add((Resource2 *)program, res);
             edje_edit_string_free(name);
          }
        name = edje_edit_program_filter_part_get(group->edit_object, program->common.name);
        res = resource_manager_find(group->parts, name);
        if (res)
          {
             _resource_usage_resource_add((Resource2 *)program, res);
             state = edje_edit_program_filter_state_get(group->edit_object, program->common.name);
             res_state = resource_manager_find(((Part2 *)res)->states, state);
             if (res_state)
               _resource_usage_resource_add((Resource2 *)program, res_state);
             edje_edit_string_free(state);
          }
        edje_edit_string_free(name);
        EINA_LIST_FOREACH(program->afters, l2, res)
          {
             _resource_usage_resource_add((Resource2 *)program, res);
          }

        EINA_LIST_FOREACH(program->targets, l2, res)
          {
             _resource_usage_resource_add((Resource2 *)program, res);
          }
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
   EINA_LIST_FOREACH(pro->groups2, l1, group)
     {
        _resource_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
        if (edje_edit_group_alias_is(group->edit_object, group->common.name))
          {
             main_group_name = edje_edit_group_aliased_get(group->edit_object, group->common.name);
             used = resource_manager_find(pro->groups2, main_group_name);
             _resource_usage_resource_add((Resource2 *)group, used);

             edje_edit_string_free(main_group_name);

             group->main_group = (Group2 *)used;
             group->main_group->aliases = eina_list_append(group->main_group->aliases, group);
          }
        else
          {
             _part_dependency_load(pro, group);
             _program_dependency_load(pro, group);
          }
        _resource_group_edit_object_unload(group);
     }
}
