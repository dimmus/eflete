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

Eina_Bool
_resource_usage_dependency_cleanup(Resource2 *origin)
{
   Eina_List *l;
   Resource2 *res;

   EINA_LIST_FOREACH(origin->common.used_in, l, res)
     {
        _resource_usage_resource_del(res, origin);
     }
   EINA_LIST_FOREACH(origin->common.uses___, l, res)
     {
        _resource_usage_resource_del(origin, res);
     }
   eina_list_free(origin->common.used_in);
   eina_list_free(origin->common.uses___);

   return true;
}

/***********************************************/

void
_item_dependency_load(Project* pro, Group2 *group, Part2 *part, Part_Item2 *item)
{
   Resource2 *used;
   Eina_Stringshare *source = edje_edit_part_item_source_get(group->edit_object,
                                                             part->common.name,
                                                             item->common.name);
   item->source = eina_stringshare_add(source);
   used = resource_manager_find(pro->RM.groups, source);
   if (used)
     _resource_usage_resource_add((Resource2 *)item, used);
   edje_edit_string_free(source);
}

void
_state_dependency_load(Project *pro, Group2 *group, Part2 *part, State2 *state)
{
   Eina_List *l2;
   Resource2 *res;
   Colorclass2 *res_colorclass;
   Eina_Stringshare *source;
   Eina_Stringshare *font_name, *color_class, *style_name;

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
               res = resource_manager_find(pro->RM.image_sets, state->normal);
             else
               res = resource_manager_find(pro->RM.images, state->normal);
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
   res = resource_manager_find(pro->RM.colorclasses, color_class);
   if (res)
     _resource_usage_resource_add((Resource2 *)state, res);
   else
     {
        /* Colorclass can be specified but not defined in edc.
           If colorclass don't exist yet adding it */
        CRIT_ON_FAIL(editor_color_class_add(group->edit_object,
                                            color_class,
                                            false));
        res_colorclass = mem_calloc(1, sizeof(Colorclass2));
        res_colorclass->common.type = RESOURCE2_TYPE_COLORCLASS;
        res_colorclass->common.name = eina_stringshare_add(color_class);
        pro->RM.colorclasses = eina_list_append(pro->RM.colorclasses,
                                                res_colorclass);
        _resource_usage_resource_add((Resource2 *)state,
                                     (Resource2 *)res_colorclass);
     }
   edje_edit_string_free(color_class);

   if (part->type == EDJE_PART_TYPE_TEXT)
     {
        font_name = edje_edit_state_font_get(group->edit_object,
                                             part->common.name,
                                             state->common.name,
                                             state->val);
        res = resource_manager_find(pro->RM.fonts, font_name);
        if (res)
          _resource_usage_resource_add((Resource2 *)state, res);
        edje_edit_string_free(font_name);

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
        res = resource_manager_find(pro->RM.styles, style_name);
        _resource_usage_resource_add((Resource2 *)state, res);
        edje_edit_string_free(style_name);
     }
   TODO("Implement size_class and text_class stuff")
      /*
         Relationships uses - used:
         STATE - (unimplemented yet) SIZE_CLASS, TEXT_CLASS
       */
}

void
_part_dependency_load(Project *pro, Group2 *group, Part2 *part)
{
   Part_Item2 *item;
   State2 *state;
   Eina_Stringshare *source, *group_name;
   Resource2 *res;
   Eina_List *l;

   source = edje_edit_part_source_get(group->edit_object, part->common.name);
   res = resource_manager_find(pro->RM.groups, source);
   if (res)
     _resource_usage_resource_add((Resource2 *)part, res);
   edje_edit_string_free(source);

#define TEXT_RESOURCE_USES(FUNC) \
   group_name = FUNC(group->edit_object, part->common.name); \
   res = resource_manager_find(pro->RM.groups, group_name); \
   _resource_usage_resource_add((Resource2 *)part, res); \
   edje_edit_string_free(group_name);

   if (part->type == EDJE_PART_TYPE_TEXTBLOCK)
     {
        TEXT_RESOURCE_USES(edje_edit_part_source_get);
        TEXT_RESOURCE_USES(edje_edit_part_source2_get);
        TEXT_RESOURCE_USES(edje_edit_part_source3_get);
        TEXT_RESOURCE_USES(edje_edit_part_source4_get);
        TEXT_RESOURCE_USES(edje_edit_part_source5_get);
        TEXT_RESOURCE_USES(edje_edit_part_source6_get);
     }

#undef TEXT_RESOURCE_USES

   EINA_LIST_FOREACH(part->states, l, state)
     {
        _state_dependency_load(pro, group, part, state);
     }

   EINA_LIST_FOREACH(part->items, l, item)
     {
        _item_dependency_load(pro, group, part, item);
     }
}

void
_program_dependency_load(Project *pro, Group2 *group, Program2 *program)
{
   Eina_List *l2;
   Resource2 *res, *res_state;
   Eina_Stringshare *name, *state;

   if (program->type == EDJE_ACTION_TYPE_SOUND_SAMPLE)
     {
        name = edje_edit_program_sample_name_get(group->edit_object, program->common.name);
        res = resource_manager_find(pro->RM.sounds, name);
        if (res)
          _resource_usage_resource_add((Resource2 *)program, res);
        edje_edit_string_free(name);
     }
   if (program->type == EDJE_ACTION_TYPE_SOUND_TONE)
     {
        name = edje_edit_program_tone_name_get(group->edit_object, program->common.name);
        res = resource_manager_find(pro->RM.tones, name);
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

void
_group_dependency_load(Project *pro, Group2 *group)
{
   Resource2 *used;
   Part2 *part;
   Program2 *program;
   Eina_Stringshare *main_group_name;
   Eina_List *l;

   _resource_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
   if (edje_edit_group_alias_is(group->edit_object, group->common.name))
     {
        main_group_name = edje_edit_group_aliased_get(group->edit_object, group->common.name);
        used = resource_manager_find(pro->RM.groups, main_group_name);
        _resource_usage_resource_add((Resource2 *)group, used);

        edje_edit_string_free(main_group_name);

        group->main_group = (Group2 *)used;
        group->main_group->aliases = eina_list_append(group->main_group->aliases, group);
     }
   else
     {
        EINA_LIST_FOREACH(group->parts, l, part)
          {
             _part_dependency_load(pro, group, part);
          }
        EINA_LIST_FOREACH(group->programs, l, program)
          {
             _program_dependency_load(pro, group, program);
          }
     }
   _resource_group_edit_object_unload(group);
}

void
_resource_dependency_load(Project *pro)
{
   Group2 *group;
   Resource2 *res, *used;
   Eina_Stringshare *set_image_name;
   Eina_List *set_images;
   Eina_List *l1, *l2;

   /* image_set */
   EINA_LIST_FOREACH(pro->RM.image_sets, l1, res)
     {
        set_images = edje_edit_image_set_images_list_get(pro->global_object, res->common.name);
        EINA_LIST_FOREACH(set_images, l2, set_image_name)
          {
             used = resource_manager_find(pro->RM.images, set_image_name);
             _resource_usage_resource_add(res, used);
          }
        edje_edit_string_list_free(set_images);
     }

   /* groups */
   EINA_LIST_FOREACH(pro->RM.groups, l1, group)
     {
        _group_dependency_load(pro, group);
     }
}
