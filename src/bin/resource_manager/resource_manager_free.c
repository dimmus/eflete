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
#include "project_manager2.h"

/*****************************************************************************
 *****************************************************************************
 ********************** IMPORTANT ABOUT GROUP_LOAD/UNLOAD ********************
 *****************************************************************************
 *****************************************************************************
 ** in case if group is already loaded as tab, we dont need to              **
 ** load that case again and then close, that's why it's                    **
 ** important to check if group is already loaded. In this case             **
 ** ALWAYS use those API pair this way:                                     **
 *****************************************************************************
 ** void                                                                    **
 ** _something_del()                                                        **
 ** {                                                                       **
 **    <stuff>                                                              **
 **    Eina_Bool is_opened = false;                                         **
 **    <more stuff>                                                         **
 **    ...                                                                  **
 **    if (!state->part->group->edit_object)                                **
 **      {                                                                  **
 **         is_opened = true;                                               **
 **         resource_group_edit_object_load(Project *,                      **
 **                                         Group *,                        **
 **                                         evas_object_evas_get(ap.win));  **
 **      }                                                                  **
 **    ...                                                                  **
 **    <work with dependecies and edje_edit>                                **
 **    ...                                                                  **
 **    if (is_opened)                                                       **
 **      resource_group_edit_object_unload(Group *);                        **
 **    is_opened = false;                                                   **
 ** }                                                                       **
 *****************************************************************************
 *****************************************************************************/
void
_resource_image_free(Project *pro, Image2 *res)
{
   pro->RM.images = eina_list_remove(pro->RM.images, res);

   eina_stringshare_del(res->common.name);
   eina_stringshare_del(res->source);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_image_del(Project *pro, Image2 *res_image)
{
   Eina_List *l, *images;
   Resource2 *res;
   State2 *state;
   Image_Set2 *image_set;
   int idx;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(res_image->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_STATE)
          {
             state = (State2 *)res;
             if (res_image->common.name == state->normal)
               {
                  if (!state->part->group->edit_object)
                    {
                       is_opened = true;
                       resource_group_edit_object_load(pro,
                                                       state->part->group,
                                                       evas_object_evas_get(ap.win));
                    }
                  CRIT_ON_FAIL(editor_state_image_set(state->part->group->edit_object,
                                                      NULL,
                                                      false,
                                                      true,
                                                      state->part->common.name,
                                                      state->common.name,
                                                      state->val,
                                                      EFLETE_DUMMY_IMAGE_NAME));
                  if (is_opened)
                    resource_group_edit_object_unload(state->part->group);
                  eina_stringshare_del(state->normal);
                  state->normal = eina_stringshare_add(EFLETE_DUMMY_IMAGE_NAME);
               }
             else
               {
                  /* NOT WORKING FOR NOW SINCE TWEEN ALREADY DELETED SOMEHOW
                  CRIT_ON_FAIL(editor_state_tween_del(state->part->group->edit_object,
                                                      NULL,
                                                      false,
                                                      true,
                                                      state->part->common.name,
                                                      state->common.name,
                                                      state->val,
                                                      res_image->common.name));
                   */
                  state->tweens = eina_list_remove(state->tweens, res_image);
               }
          }
        else if (res->common.type == RESOURCE2_TYPE_IMAGE_SET)
          {
             image_set = (Image_Set2 *)res;

             TODO("Make editor_image_set_image_del");
             images = edje_edit_image_set_images_list_get(pro->global_object,
                                                          image_set->common.name);
             idx = eina_list_data_idx(images, (void *)res_image->common.name);
             edje_edit_image_set_image_del(pro->global_object,
                                           image_set->common.name,
                                           idx);
             edje_edit_string_list_free(images);
          }
     }
   _resource_image_free(pro, res_image);
}

void
_resource_tone_free(Project *pro, Tone2 *res)
{
   pro->RM.tones = eina_list_remove(pro->RM.tones, res);

   eina_stringshare_del(res->common.name);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_tone_del(Project *pro, Tone2 *res_tone)
{
   Eina_List *l;
   Resource2 *res;
   Program2 *program;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(res_tone->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          {
             program = (Program2 *)res;
             if (!program->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  program->group,
                                                  evas_object_evas_get(ap.win));
               }
             CRIT_ON_FAIL(editor_program_tone_name_set(program->group->edit_object,
                                                       NULL,
                                                       false,
                                                       true,
                                                       program->common.name,
                                                       ""));
             if (is_opened)
               resource_group_edit_object_unload(program->group);
          }
     }

   _resource_tone_free(pro, res_tone);
}

void
_resource_sound_free(Project *pro, Sound2 *res)
{
   pro->RM.sounds = eina_list_remove(pro->RM.sounds, res);

   eina_stringshare_del(res->common.name);
   eina_stringshare_del(res->source);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_sound_del(Project *pro, Sound2 *res_sound)
{
   Eina_List *l;
   Resource2 *res;
   Program2 *program;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(res_sound->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          {
             program = (Program2 *)res;
             if (!program->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  program->group,
                                                  evas_object_evas_get(ap.win));
               }
             CRIT_ON_FAIL(editor_program_sample_name_set(program->group->edit_object,
                                                         NULL,
                                                         false,
                                                         true,
                                                         program->common.name,
                                                         EFLETE_DUMMY_SAMPLE_NAME));
             if (is_opened)
               resource_group_edit_object_unload(program->group);
          }
     }

   _resource_sound_free(pro, res_sound);
}

void
_resource_colorclass_free(Project *pro, Colorclass2 *res)
{
   pro->RM.colorclasses = eina_list_remove(pro->RM.colorclasses, res);

   eina_stringshare_del(res->common.name);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_colorclass_del(Project *pro, Colorclass2 *res_colorclass)
{
   Eina_List *l;
   Resource2 *res;
   State2 *state;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(res_colorclass->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_STATE)
          {
             state = (State2 *)res;
             if (!state->part->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  state->part->group,
                                                  evas_object_evas_get(ap.win));
               }
             CRIT_ON_FAIL(editor_state_color_class_set(state->part->group->edit_object,
                                                       NULL,
                                                       false,
                                                       true,
                                                       state->part->common.name,
                                                       state->common.name,
                                                       state->val,
                                                       NULL));
             if (is_opened)
               resource_group_edit_object_unload(state->part->group);
          }
     }
   _resource_colorclass_free(pro, res_colorclass);
}

void
_resource_style_tag_free(Style_Tag2 *res)
{
   _resource_usage_dependency_cleanup((Resource2 *)res);

   res->style->tags = eina_list_remove(res->style->tags, res);

   eina_stringshare_del(res->common.name);
   eina_stringshare_del(res->font);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_style_free(Project *pro, Style2 *res)
{
   Style_Tag2 *tag;
   EINA_LIST_FREE(res->tags, tag)
     {
        _resource_style_tag_free(tag);
     }

   pro->RM.styles = eina_list_remove(pro->RM.styles, res);

   eina_stringshare_del(res->common.name);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_style_del(Project *pro, Style2 *res_style)
{
   Eina_List *l;
   Resource2 *res;
   State2 *state;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(res_style->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_STATE)
          {
             state = (State2 *)res;
             if (!state->part->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  state->part->group,
                                                  evas_object_evas_get(ap.win));
               }
             CRIT_ON_FAIL(editor_state_text_style_set(state->part->group->edit_object,
                                                      NULL,
                                                      false,
                                                      true,
                                                      state->part->common.name,
                                                      state->common.name,
                                                      state->val,
                                                      NULL));
             if (is_opened)
               resource_group_edit_object_unload(state->part->group);
          }
     }

   _resource_style_free(pro, res_style);
}

void
_resource_image_set_free(Project *pro, Image_Set2 *res)
{
   pro->RM.image_sets = eina_list_remove(pro->RM.image_sets, res);

   eina_stringshare_del(res->common.name);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_font_free(Project *pro, Font2 *res)
{
   pro->RM.fonts = eina_list_remove(pro->RM.fonts, res);

   eina_stringshare_del(res->common.name);
   eina_stringshare_del(res->source);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_data_free(Project *pro, Global_Data2 *res)
{
   pro->RM.global_data = eina_list_remove(pro->RM.global_data, res);

   eina_stringshare_del(res->common.name);
   eina_stringshare_del(res->source);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

/****** GROUP DELETION **********/

void
_resource_state_free(Part2 *part, State2 *state)
{
   /* 1. remove each state from all "used_in" and "uses___" and cleanup */
   _resource_usage_dependency_cleanup((Resource2 *)state);

   /* 2. cleanup custom fields */
   eina_list_free(state->tweens);
   eina_stringshare_del(state->normal);

   /* 3. cleanup common */
   part->states = eina_list_remove(part->states, state);

   eina_stringshare_del(state->common.name);
   eina_list_free(state->common.used_in);
   eina_list_free(state->common.uses___);
   free(state);
}

void
_resource_state_del(Project *pro, Part2 *part, State2 *state, Change *change)
{
   Eina_List *l;
   Resource2 *res;
   Program2 *program;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(state->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          {
             program = (Program2 *)res;
             if (!program->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  program->group,
                                                  evas_object_evas_get(ap.win));
               }
             CRIT_ON_FAIL(editor_program_filter_state_set(program->group->edit_object,
                                                          change,
                                                          false,
                                                          true,
                                                          res->common.name,
                                                          NULL));
             if (is_opened)
               resource_group_edit_object_unload(program->group);
          }
     }
   _resource_state_free(part, state);
}

void
_resource_part_item_free(Part2 *part, Part_Item2 *item)
{
   /* 1. remove each item from all "used_in" and "uses___" and cleanup */
   _resource_usage_dependency_cleanup((Resource2 *)item);
   /* 2. cleanup common */
   part->items = eina_list_remove(part->items, item);

   eina_stringshare_del(item->common.name);
   eina_list_free(item->common.used_in);
   eina_list_free(item->common.uses___);
   free(item);
}

void
_resource_part_item_del(Part2 *part, Part_Item2 *item)
{
   /* item is not used anywhere at all */
   _resource_part_item_free(part, item);
}

void
_resource_part_free(Group2 *group, Part2 *part)
{
   /* step by step */
   /* 1. remove part from all "used_in" and "uses___" and cleanup */
   _resource_usage_dependency_cleanup((Resource2 *)part);
   /* 2. cleanup items and state list */
   eina_list_free(part->states);
   eina_list_free(part->items);
   eina_stringshare_del(part->current_item_name);
   /* 3. remove part from group->parts */
   group->parts = eina_list_remove(group->parts, part);

   eina_stringshare_del(part->common.name);
   eina_list_free(part->common.used_in);
   eina_list_free(part->common.uses___);
   free(part);
}

TODO("Apply more complex work (with warning and error maybe?)"
     "with parts which are used by other resources later")
void
_resource_part_del(Project *pro, Group2 *group, Part2 *part, Change *change)
{
   Eina_List *l;
   Resource2 *res;
   State2 *state;
   Part_Item2 *item;
   Program2 *program;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(part->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_STATE)
          {
             state = (State2 *)res;
             if (!state->part->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  state->part->group,
                                                  evas_object_evas_get(ap.win));
               }
             if (state->part->type == EDJE_PART_TYPE_PROXY)
               CRIT_ON_FAIL(editor_state_proxy_source_set(state->part->group->edit_object,
                                                          change,
                                                          false,
                                                          true,
                                                          state->part->common.name,
                                                          state->common.name,
                                                          state->val,
                                                          NULL));
             else if (state->part->type == EDJE_PART_TYPE_TEXT)
               {
                  CRIT_ON_FAIL(editor_state_text_text_source_set(state->part->group->edit_object,
                                                                 change,
                                                                 false,
                                                                 true,
                                                                 part->common.name,
                                                                 state->common.name,
                                                                 state->val,
                                                                 NULL));
                  CRIT_ON_FAIL(editor_state_text_source_set(state->part->group->edit_object,
                                                            change,
                                                            false,
                                                            true,
                                                            part->common.name,
                                                            state->common.name,
                                                            state->val,
                                                            NULL));
               }
             if (is_opened)
               resource_group_edit_object_unload(state->part->group);
             else
               resource_group_edit_object_reload(pro, state->part->group);
          }
        else if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          {
             program = (Program2 *)res;
             if (!program->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  program->group,
                                                  evas_object_evas_get(ap.win));
               }
             /* if it is actually filter */
             if (program->filter_part == part->common.name)
               {
                  CRIT_ON_FAIL(editor_program_filter_part_set(program->group->edit_object,
                                                              change,
                                                              false,
                                                              true,
                                                              res->common.name,
                                                              NULL));
                  eina_stringshare_del(program->filter_part);
                  program->filter_part = NULL;
               }
             else
               {
                  /* if not its probably target part */
                  CRIT_ON_FAIL(editor_program_target_del(program->group->edit_object,
                                                         change,
                                                         false,
                                                         true,
                                                         res->common.name,
                                                         part->common.name));
                  program->targets = eina_list_remove(program->targets, part);
               }
             if (is_opened)
               resource_group_edit_object_unload(program->group);
          }
        is_opened = false;
     }

   /* kill all dependencies of part's states,
      since they will be deleted together with part */
   EINA_LIST_FOREACH(part->states, l, state)
     {
        _resource_state_del(pro, part, state, change);
     }
   EINA_LIST_FOREACH(part->items, l, item)
     {
        _resource_part_item_del(part, item);
     }

   _resource_part_free(group, part);
}

void
_resource_program_free(Group2 *group, Program2 *program)
{
   /* 1. remove each state from all "used_in" and "uses___" and cleanup */
   _resource_usage_dependency_cleanup((Resource2 *)program);

   /* 2. cleanup custom fields */
   eina_list_free(program->afters);
   eina_list_free(program->targets);

   /* 3. cleanup common */
   group->programs = eina_list_remove(group->programs, program);

   eina_stringshare_del(program->common.name);
   eina_list_free(program->common.used_in);
   eina_list_free(program->common.uses___);
   free(program);
}

void
_resource_program_del(Project *pro, Group2 *group, Program2 *program, Change *change)
{
   Eina_List *l;
   Resource2 *res;
   Program2 *res_program;

   Eina_Bool is_opened = false;

   EINA_LIST_FOREACH(program->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          {
             res_program = (Program2 *)res;

             /* if its inside of targets */
             while (eina_list_data_find_list(res_program->targets, program))
               {
                  /* if not its probably target part */
                  if (!program->group->edit_object)
                    {
                       is_opened = true;
                       resource_group_edit_object_load(pro,
                                                       program->group,
                                                       evas_object_evas_get(ap.win));
                    }
                  CRIT_ON_FAIL(editor_program_target_del(program->group->edit_object,
                                                         change,
                                                         false,
                                                         true,
                                                         res->common.name,
                                                         program->common.name));
                  if (is_opened)
                    resource_group_edit_object_unload(program->group);
                  res_program->targets = eina_list_remove(res_program->targets, program);
               }
             /* if its inside of afters */
             while (eina_list_data_find_list(res_program->afters, program))
               {
                  /* if not its probably target part */
                  if (!program->group->edit_object)
                    {
                       is_opened = true;
                       resource_group_edit_object_load(pro,
                                                       program->group,
                                                       evas_object_evas_get(ap.win));
                    }
                  CRIT_ON_FAIL(editor_program_after_del(program->group->edit_object,
                                                        change,
                                                        false,
                                                        true,
                                                        res->common.name,
                                                        program->common.name));
                  if (is_opened)
                    resource_group_edit_object_unload(program->group);
                  res_program->afters = eina_list_remove(res_program->afters, program);
               }
          }
        is_opened = false;
     }

   /* item is not used anywhere at all */
   _resource_program_free(group, program);
}

void
_resource_group_data_free(Group2 *group, Group_Data2 *data)
{
   /* 1. remove each state from all "used_in" and "uses___" and cleanup */
   _resource_usage_dependency_cleanup((Resource2 *)data);

   /* 2. cleanup custom fields */
   eina_stringshare_del(data->source);

   /* 3. cleanup common */
   group->data_items = eina_list_remove(group->data_items, data);

   eina_stringshare_del(data->common.name);
   eina_list_free(data->common.used_in);
   eina_list_free(data->common.uses___);
   free(data);
}

void
_resource_group_data_del(Group2 *group, Group_Data2 *data)
{
   /* item is not used anywhere at all */
   _resource_group_data_free(group, data);
}

void
_resource_group_free(Project *pro, Group2 *group)
{
   /* 1. remove each state from all "used_in" and "uses___" and cleanup */
   _resource_usage_dependency_cleanup((Resource2 *)group);

   /* 2. cleanup custom fields */
   eina_stringshare_del(group->widget);
   eina_stringshare_del(group->class);
   eina_stringshare_del(group->style);
   eina_list_free(group->programs);
   eina_list_free(group->parts);
   eina_list_free(group->limits);
   eina_list_free(group->data_items);
   eina_list_free(group->aliases);

   /* 3. cleanup common */
   pro->RM.groups = eina_list_remove(pro->RM.groups, group);

   eina_stringshare_del(group->common.name);
   eina_list_free(group->common.used_in);
   eina_list_free(group->common.uses___);
   free(group);
}

TODO("Apply more complex work (with warning and error maybe?)"
     "with parts which are used by other resources later")
void
_resource_group_del(Project *pro, Group2 *group)
{
   Eina_List *l;
   Resource2 *res;
   Program2 *program;
   Part2 *part;
   Part_Item2 *item;
   Group_Data2 *data;
   Group2 *alias_group;

   Eina_Bool is_opened = false;

   /* If this group is alias, then delete it from aliased */
   if (group->main_group)
     group->main_group->aliases = eina_list_remove(group->main_group->aliases, group);

   /* IMPORTANT
      Since edje edit together with group deletion also delete all it's aliases
      it's important to update dependencies and recursively remove deps for
      every aliased group. */
   EINA_LIST_FOREACH(group->aliases, l, alias_group)
     {
        CRIT_ON_FAIL(editor_group_del(ap.project->global_object, alias_group->common.name, true));
     }

   EINA_LIST_FOREACH(group->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PART)
          {
             part = (Part2 *)res;
             if (!part->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  part->group,
                                                  evas_object_evas_get(ap.win));
               }
             if (part->type == EDJE_PART_TYPE_GROUP)
               CRIT_ON_FAIL(editor_part_group_source_set(part->group->edit_object,
                                                         NULL,
                                                         false,
                                                         true,
                                                         part->common.name,
                                                         NULL));
             else if (part->type == EDJE_PART_TYPE_TEXTBLOCK)
               {
                  CRIT_ON_FAIL(editor_part_textblock_selection_under_set(part->group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_selection_over_set( part->group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_cursor_under_set(   part->group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_cursor_over_set(    part->group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_anchors_under_set(  part->group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_anchors_over_set(   part->group->edit_object, NULL, false, true, part->common.name, NULL));
               }
             if (is_opened)
               resource_group_edit_object_unload(part->group);
          }
        else if (res->common.type == RESOURCE2_TYPE_ITEM)
          {
             item = (Part_Item2 *)res;
             if (!item->part->group->edit_object)
               {
                  is_opened = true;
                  resource_group_edit_object_load(pro,
                                                  item->part->group,
                                                  evas_object_evas_get(ap.win));
               }
             CRIT_ON_FAIL(editor_part_item_source_set(item->part->group->edit_object,
                                                      NULL,
                                                      false,
                                                      true,
                                                      item->part->common.name,
                                                      item->common.name,
                                                      EFLETE_INTERNAL_GROUP_NAME));
             if (is_opened)
               resource_group_edit_object_unload(item->part->group);
          }
        is_opened = false;
     }

   EINA_LIST_FOREACH(group->programs, l, program)
     {
        _resource_program_del(pro, group, program, NULL);
     }
   EINA_LIST_FOREACH(group->data_items, l, data)
     {
        _resource_group_data_del(group, data);
     }
   EINA_LIST_FOREACH(group->parts, l, part)
     {
        _resource_part_del(pro, group, part, NULL);
     }

   _resource_group_free(pro, group);
}
