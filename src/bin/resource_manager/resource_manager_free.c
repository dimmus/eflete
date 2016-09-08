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

/****** TOP BLOCK DELETION **********/
void
_resource_image_set_free(Image_Set2 *res_image_set __UNUSED__)
{
}

void
_resource_image_free(Image2 *res_image __UNUSED__)
{
}

void
_resource_tone_free(Tone2 *res_tone __UNUSED__)
{
}

void
_resource_sound_free(Sound2 *res_sound __UNUSED__)
{
}

void
_resource_colorclasses_free(Colorclass2 *res_colorclass __UNUSED__)
{
}

void
_resource_styles_free(Style2 *res_style __UNUSED__)
{
}

void
_resource_font_free(Font2 *res_font __UNUSED__)
{
}

void
_resource_data_free(Global_Data2 *res_data __UNUSED__)
{
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
_resource_state_del(Group2 *group, Part2 *part, State2 *state, Change *change)
{
   Eina_List *l;
   Resource2 *res;

   EINA_LIST_FOREACH(state->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          CRIT_ON_FAIL(editor_program_filter_state_set(group->edit_object, //check this one
                                                       change,
                                                       false,
                                                       true,
                                                       res->common.name,
                                                       NULL));
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
_resource_part_del(Group2 *group, Part2 *part, Change *change)
{
   Eina_List *l;
   Resource2 *res;
   State2 *state;
   Part_Item2 *item;

   EINA_LIST_FOREACH(part->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_STATE)
          {
             state = (State2 *)res;
             if (state->part->type == EDJE_PART_TYPE_PROXY)
               CRIT_ON_FAIL(editor_state_proxy_source_set(group->edit_object,
                                                          change,
                                                          false,
                                                          true,
                                                          state->part->common.name,
                                                          state->common.name,
                                                          state->val,
                                                          NULL));
             else if (state->part->type == EDJE_PART_TYPE_TEXT)
               CRIT_ON_FAIL(editor_state_text_text_source_set(group->edit_object,
                                                              change,
                                                              false,
                                                              true,
                                                              part->common.name,
                                                              state->common.name,
                                                              state->val,
                                                              NULL));
          }
        else if (res->common.type == RESOURCE2_TYPE_PROGRAM)
          CRIT_ON_FAIL(editor_program_filter_part_set(group->edit_object,
                                                      change,
                                                      false,
                                                      true,
                                                      res->common.name,
                                                      NULL));
     }

   /* kill all dependencies of part's states,
      since they will be deleted together with part */
   EINA_LIST_FOREACH(part->states, l, state)
     {
        _resource_state_del(group, part, state, change);
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
_resource_program_del(Group2 *group, Program2 *program)
{
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
   eina_stringshare_del(group->common.name);
   eina_list_free(group->programs);
   eina_list_free(group->parts);
   eina_list_free(group->limits);
   eina_list_free(group->data_items);
   eina_list_free(group->aliases);

   /* 3. cleanup common */
   pro->groups = eina_list_remove(pro->RM.groups, group);

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

   EINA_LIST_FOREACH(group->common.used_in, l, res)
     {
        if (res->common.type == RESOURCE2_TYPE_PART)
          {
             part = (Part2 *)res;
             if (part->type == EDJE_PART_TYPE_GROUP)
               CRIT_ON_FAIL(editor_part_group_source_set(group->edit_object,
                                                         NULL,
                                                         false,
                                                         true,
                                                         part->common.name,
                                                         NULL));
             else if (part->type == EDJE_PART_TYPE_TEXTBLOCK)
               {
                  CRIT_ON_FAIL(editor_part_textblock_selection_under_set(group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_selection_over_set( group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_cursor_under_set(   group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_cursor_over_set(    group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_anchors_under_set(  group->edit_object, NULL, false, true, part->common.name, NULL));
                  CRIT_ON_FAIL(editor_part_textblock_anchors_over_set(   group->edit_object, NULL, false, true, part->common.name, NULL));
               }
          }
        else if (res->common.type == RESOURCE2_TYPE_ITEM)
          {
             item = (Part_Item2 *)res;
             CRIT_ON_FAIL(editor_part_item_source_set(group->edit_object,
                                                      NULL,
                                                      false,
                                                      true,
                                                      item->part->common.name,
                                                      item->common.name,
                                                      EFLETE_INTERNAL_GROUP_NAME));
          }
     }

   EINA_LIST_FOREACH(group->programs, l, program)
     {
        _resource_program_del(group, program);
     }
   EINA_LIST_FOREACH(group->data_items, l, data)
     {
        _resource_group_data_del(group, data);
     }
   EINA_LIST_FOREACH(group->parts, l, part)
     {
        _resource_part_del(group, part, NULL);
     }

   _resource_group_free(pro, group);
}
