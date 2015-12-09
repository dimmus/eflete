/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "group_manager.h"
#include "alloc.h"
#include "signals.h"
#include "string_common.h"

static void
_group_name_parse(Group *group)
{
   char **c;
   unsigned int count;

   assert(group != NULL);

   c = eina_str_split_full(group->name, "/", 4, &count);

   TODO("move here complicated class/style parsing from live_view");
   if ((count == 4) && (!strcmp(c[0], "elm")))
     {
        group->widget = eina_stringshare_add(c[1]);
        group->class = eina_stringshare_add(c[2]);
        group->style = eina_stringshare_add(c[3]);
     }

   free(c[0]);
   free(c);
}

void
gm_group_edit_object_load(Project *pro, Group *group, Evas *e)
{
   assert(pro != NULL);
   assert(group != NULL);
   assert(group->edit_object == NULL);

   group->edit_object = edje_edit_object_add(e);
   edje_object_freeze(group->edit_object);
   evas_object_freeze_events_set(group->edit_object, true);

   if (!edje_object_mmap_set(group->edit_object, pro->mmap_file, group->name))
     {
        ERR("Can't set mmap object");
        abort();
     }
}

void
gm_group_edit_object_unload(Group *group)
{
   assert(group != NULL);
   assert(group->edit_object != NULL);

   evas_object_del(group->edit_object);
   group->edit_object = NULL;
}

State *
gm_state_add(Project *pro, Part_ *part, const char *state_name)
{
   State *state;
   Eina_Stringshare *image_name, *name;
   Eina_List *tween_list, *l;

   assert(pro != NULL);
   assert(part != NULL);
   assert(state_name != NULL);

   state = mem_calloc(1, sizeof(State));
   state->name = eina_stringshare_add(state_name);
   state->part = part;

   part->states = eina_list_sorted_insert(part->states, (Eina_Compare_Cb) resource_cmp, state);

   /* default 0.0 should be allways first state */
   if (part->current_state == NULL)
     part->current_state = state;

   state_name_split(state_name, &state->parsed_name, &state->parsed_val);

   #define USAGE_ADD(TYPE, USAGE_LIST) \
   name = edje_edit_state_ ## TYPE ## _get(part->group->edit_object, \
                                           part->name, \
                                           state->parsed_name, \
                                           state->parsed_val); \
   if (name) \
     { \
        pm_resource_usage_add(USAGE_LIST, name, state); \
        edje_edit_string_free(name); \
     }

   #define COLORCLASS_USAGE_ADD() \
   name = edje_edit_state_color_class_get(part->group->edit_object, \
                                          part->name, \
                                          state->parsed_name, \
                                          state->parsed_val); \
   if (name) \
     { \
        if (!pm_resource_usage_add(pro->colorclasses, name, state)) \
          { \
             /* Colorclass can be specified but not defined in edc.
                If colorclass don't exist yet adding it */ \
             TODO("move this code to colorclass resource manager"); \
             edje_edit_color_class_add(pro->global_object, name); \
             Colorclass_Resource *res = mem_calloc(1, sizeof(Colorclass_Resource)); \
             res->name = eina_stringshare_add(name); \
             res->color1.r = res->color1.g = res->color1.b = res->color1.a = 255; \
             res->color2.r = res->color2.g = res->color2.b = res->color2.a = 255; \
             res->color3.r = res->color3.g = res->color3.b = res->color3.a = 255; \
             pro->colorclasses = eina_list_sorted_insert(pro->colorclasses, (Eina_Compare_Cb) resource_cmp, res); \
             if (!pm_resource_usage_add(pro->colorclasses, name, state)) \
               { \
                  ERR("Err: resource not found: %s", name); \
                  abort(); \
               }; \
          } \
        edje_edit_string_free(name); \
     }


   switch (part->type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
      case EDJE_PART_TYPE_PROXY:
      case EDJE_PART_TYPE_BOX:
      case EDJE_PART_TYPE_TABLE:
         COLORCLASS_USAGE_ADD();
         break;
      case EDJE_PART_TYPE_IMAGE:
         COLORCLASS_USAGE_ADD();

         USAGE_ADD(image, pro->images);

         tween_list = edje_edit_state_tweens_list_get(part->group->edit_object,
                                                      part->name,
                                                      state->parsed_name,
                                                      state->parsed_val);
         EINA_LIST_FOREACH(tween_list, l, image_name)
            pm_resource_usage_add(pro->images, image_name, state);
         edje_edit_string_list_free(tween_list);

         break;
      case EDJE_PART_TYPE_TEXT:
         COLORCLASS_USAGE_ADD();
         USAGE_ADD(font, pro->fonts);
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         USAGE_ADD(text_style, pro->styles);
         break;
      default:
         break;
     }
   #undef COLORCLASS_USAGE_ADD
   #undef USAGE_ADD
   return state;
}

Part_ *
gm_part_add(Project *pro, Group *group, const char *part_name)
{
   Part_ *part;
   Eina_List *states, *l;
   Eina_Stringshare *state_name, *group_name, *item_name;

   assert(pro != NULL);
   assert(group != NULL);
   assert(part_name != NULL);

   part = mem_calloc(1, sizeof(Part_));
   part->name = eina_stringshare_add(part_name);
   part->group = group;
   part->type = edje_edit_part_type_get(group->edit_object, part_name);
   part->visible = true;
   group->parts = eina_list_append(group->parts, part);
   states = edje_edit_part_states_list_get(group->edit_object, part_name);
   EINA_LIST_FOREACH(states, l, state_name)
     gm_state_add(pro, part, state_name);
   edje_edit_string_list_free(states);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     part->items = edje_edit_part_items_list_get(group->edit_object, part_name);

   #define GROUP_USAGE_ADD(TYPE) \
   group_name = edje_edit_part_ ## TYPE ## _get(part->group->edit_object, \
                                                part->name); \
   if (group_name) \
     { \
        pm_resource_usage_add(pro->groups, group_name, part); \
        edje_edit_string_free(group_name); \
     }

   switch (part->type)
     {
      case EDJE_PART_TYPE_BOX:
      case EDJE_PART_TYPE_TABLE:
         EINA_LIST_FOREACH(part->items, l, item_name)
           {
              group_name = edje_edit_part_item_source_get(group->edit_object,
                                                          part->name,
                                                          item_name);
              if (group_name)
                {
                   pm_resource_usage_add(pro->groups, group_name, part);
                   edje_edit_string_free(group_name);
                }
           }
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         GROUP_USAGE_ADD(source);
         GROUP_USAGE_ADD(source2);
         GROUP_USAGE_ADD(source3);
         GROUP_USAGE_ADD(source4);
         GROUP_USAGE_ADD(source5);
         GROUP_USAGE_ADD(source6);
         break;
      case EDJE_PART_TYPE_GROUP:
         GROUP_USAGE_ADD(source);
         break;
      default:
         break;
     }
   #undef GROUP_USAGE_ADD
   return part;
}

static void
_group_load(Project *pro, Group *group)
{
   Eina_Stringshare *main_group_name;
   Eina_List *parts, *l, *lt, *programs, *targets;
   Eina_Stringshare *part_name, *program_name, *target_name, *state_name,
                    *state_full_name, *sample_name, *tone_name;
   Edje_Action_Type act;
   double state_val;
   Part_ *part;
   State *state;
   Resource *program;

   assert(pro != NULL);
   assert(group != NULL);

   _group_name_parse(group);

   ecore_thread_main_loop_begin();
   gm_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
   if (edje_edit_group_alias_is(group->edit_object, group->name))
     {
        main_group_name = edje_edit_group_aliased_get(group->edit_object, group->name);
        group->main_group = pm_resource_get(pro->groups, main_group_name);
        group->main_group->aliases = eina_list_sorted_insert(group->main_group->aliases, (Eina_Compare_Cb)resource_cmp, group);
        edje_edit_string_free(main_group_name);
     }
   else
     {
        parts = edje_edit_parts_list_get(group->edit_object);
        EINA_LIST_FOREACH(parts, l, part_name)
           gm_part_add(pro, group, part_name);
        edje_edit_string_list_free(parts);

        programs = edje_edit_programs_list_get(group->edit_object);
        EINA_LIST_FOREACH(programs, l, program_name)
          {
            program = mem_calloc(1, sizeof(Resource));
            program->name = eina_stringshare_add(program_name);
            group->programs = eina_list_sorted_insert(group->programs, (Eina_Compare_Cb)resource_cmp, program);
          }
        EINA_LIST_FOREACH(programs, l, program_name)
          {
             program = pm_resource_get(group->programs, program_name);
             act = edje_edit_program_action_get(group->edit_object, program_name);
             switch (act)
               {
                case EDJE_ACTION_TYPE_STATE_SET:
                   targets = edje_edit_program_targets_get(group->edit_object, program_name);
                   state_name = edje_edit_program_state_get(group->edit_object, program_name);
                   state_val = edje_edit_program_value_get(group->edit_object, program_name);
                   state_full_name = eina_stringshare_printf("%s %.2f", state_name, state_val);
                   eina_stringshare_del(state_name);
                   EINA_LIST_FOREACH(targets, lt, target_name)
                     {
                        part = (Part_ *) pm_resource_unsorted_get(group->parts, target_name);
                        state = (State *) pm_resource_get(part->states, state_full_name);
                        if (state)
                          state->used_in = eina_list_sorted_insert(state->used_in,
                                                                   (Eina_Compare_Cb) resource_cmp,
                                                                   program);
                        else
                          {
                             TODO("Handle programs with states that don't exist");
                          }

                     }
                   eina_stringshare_del(state_full_name);
                   edje_edit_string_list_free(targets);
                   break;
                case EDJE_ACTION_TYPE_DRAG_VAL_SET:
                case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
                case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
                   targets = edje_edit_program_targets_get(group->edit_object, program_name);
                   EINA_LIST_FOREACH(targets, lt, target_name)
                      pm_resource_usage_unsorted_add(group->parts, target_name, (void *) program);
                   edje_edit_string_list_free(targets);
                   break;
                case EDJE_ACTION_TYPE_SOUND_SAMPLE:
                   sample_name = edje_edit_program_sample_name_get(group->edit_object, program_name);
                   pm_resource_usage_add(pro->sounds, sample_name, (void *) program);
                   eina_stringshare_del(sample_name);
                   break;
                case EDJE_ACTION_TYPE_SOUND_TONE:
                   tone_name = edje_edit_program_tone_name_get(group->edit_object, program_name);
                   pm_resource_usage_add(pro->tones, tone_name, (void *) program);
                   eina_stringshare_del(tone_name);
                   break;
                default:
                   TODO("Handle other action types");
                   break;
               }
          }
        edje_edit_string_list_free(programs);
     }

   gm_group_edit_object_unload(group);
   ecore_thread_main_loop_end();
}

Group *
gm_group_add(Project *pro, const char *group_name)
{
   Group *group;

   assert(pro != NULL);
   assert(group_name != NULL);

   group = mem_calloc(1, sizeof(Group));
   group->name = eina_stringshare_add(group_name);
   pro->groups = eina_list_sorted_insert(pro->groups, (Eina_Compare_Cb) resource_cmp, group);

   _group_load(pro, group);
   evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_ADDED, (void *)group);
   return group;
}

void
gm_group_del(Project *pro, Group *group)
{
   Group *alias;
   Part_ *part;
   Resource *program;
   Eina_List *l, *ln;

   assert(pro != NULL);
   assert(group != NULL);
   assert(group->edit_object == NULL);
   assert(group->history == NULL);

   /* Don't free the list data (Part), because data owners are another groups */
   group->used_in = eina_list_free(group->used_in);
   if (group->main_group)
     group->main_group->aliases = eina_list_remove(group->main_group->aliases, group);
   EINA_LIST_FREE(group->aliases, alias)
     {
        gm_group_del(pro, alias);
     }
   EINA_LIST_FOREACH_SAFE(group->parts, l, ln, part)
     {
        gm_part_del(pro, part);
     }
   EINA_LIST_FREE(group->programs, program)
     {
        eina_stringshare_del(program->name);
        assert(program->used_in == NULL);
        free(program);
     }
   pro->groups = eina_list_remove(pro->groups, group);
   eina_stringshare_del(group->widget);
   eina_stringshare_del(group->class);
   eina_stringshare_del(group->style);
   evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_DELETED, (void *)group->name);
   /* delete group name after call signal, because the group name need in the
    * callbacks */
   eina_stringshare_del(group->name);
   free(group);
}

void
gm_groups_load(Project *pro)
{
   Eina_List *collections, *l;
   Eina_Stringshare *group_name;
   Group *group;

   assert(pro != NULL);
   assert(pro->dev != NULL);
   assert(pro->groups == NULL);

   collections = edje_file_collection_list(pro->dev);

   assert(collections != NULL);

   collections = eina_list_sort(collections, eina_list_count(collections), (Eina_Compare_Cb) strcmp);
   EINA_LIST_FOREACH(collections, l, group_name)
     {
        if (!strcmp(group_name, EFLETE_INTERNAL_GROUP_NAME)) continue;

        group = mem_calloc(1, sizeof(Group));
        group->name = eina_stringshare_add(group_name);
        pro->groups = eina_list_append(pro->groups, group);
     }
   edje_file_collection_list_free(collections);

   EINA_LIST_FOREACH(pro->groups, l, group)
     _group_load(pro, group);
}

void
gm_groups_free(Project *pro)
{
   Group *group;
   Resource *program;
   Part_ *part;
   State *state;
   Eina_Stringshare *item_name;

   assert(pro != NULL);

   EINA_LIST_FREE(pro->groups, group)
     {
        eina_stringshare_del(group->name);
        eina_list_free(group->used_in);
        eina_stringshare_del(group->widget);
        eina_stringshare_del(group->class);
        eina_stringshare_del(group->style);
        eina_list_free(group->aliases);

        EINA_LIST_FREE(group->parts, part)
          {
             eina_stringshare_del(part->name);
             eina_list_free(part->used_in);
             EINA_LIST_FREE(part->states, state)
               {
                  eina_stringshare_del(state->name);
                  eina_stringshare_del(state->parsed_name);
                  eina_list_free(state->used_in);
                  free(state);
               }
            EINA_LIST_FREE(part->items, item_name)
               eina_stringshare_del(item_name);
            free(part);
          }
        EINA_LIST_FREE(group->programs, program)
          {
             eina_stringshare_del(program->name);
             eina_list_free(program->used_in);
             free(program);
          }
        /* object should be deleted before freeing groups list*/
        assert(group->edit_object == NULL);
        free(group);
     }
}

void
gm_state_del(Project *pro, State *state)
{
   Eina_Stringshare *name, *image_name;
   Eina_List *tween_list, *l;

   assert(pro != NULL);
   assert(state != NULL);

#define USAGE_DEL(TYPE, USAGE_LIST) \
   name = edje_edit_state_ ## TYPE ## _get(state->part->group->edit_object, \
                                           state->part->name, \
                                           state->parsed_name, \
                                           state->parsed_val); \
   if (name) \
     { \
        pm_resource_usage_del(USAGE_LIST, name, state); \
        edje_edit_string_free(name); \
     }
   switch (state->part->type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
      case EDJE_PART_TYPE_PROXY:
      case EDJE_PART_TYPE_BOX:
      case EDJE_PART_TYPE_TABLE:
         USAGE_DEL(color_class, pro->colorclasses);
         break;
      case EDJE_PART_TYPE_IMAGE:
         USAGE_DEL(color_class, pro->colorclasses);

         USAGE_DEL(image, pro->images);

         tween_list = edje_edit_state_tweens_list_get(state->part->group->edit_object,
                                                      state->part->name,
                                                      state->parsed_name,
                                                      state->parsed_val);
         EINA_LIST_FOREACH(tween_list, l, image_name)
            pm_resource_usage_del(pro->images, image_name, state);
         edje_edit_string_list_free(tween_list);

         break;
      case EDJE_PART_TYPE_TEXT:
         USAGE_DEL(color_class, pro->colorclasses);
         TODO("Uncomment this when font editor would be implemented,"
              "or else it caused eflete to be aborted on part deletion");

         /* USAGE_DEL(font, pro->fonts); */
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         USAGE_DEL(text_style, pro->styles);
         break;
      default:
         break;
     }
   state->used_in = eina_list_free(state->used_in);
   state->part->states = eina_list_remove(state->part->states, state);
   eina_stringshare_del(state->parsed_name);
   eina_stringshare_del(state->name);
   free(state);

#undef USAGE_DEL
}

void
gm_part_item_del(Project *pro, Part_ *part, Eina_Stringshare *item_name)
{
   Eina_List *l;

   assert(pro != NULL);
   assert(part != NULL);
   assert(item_name != NULL);
   assert((part->type ==  EDJE_PART_TYPE_BOX) ||
          (part->type ==  EDJE_PART_TYPE_TABLE));

   l = eina_list_data_find_list(part->items, item_name);

   assert(l != NULL);

   part->items = eina_list_remove_list(part->items, l);
   eina_stringshare_del(item_name);
}

void
gm_part_del(Project *pro, Part_* part)
{
   State *state;
   const char *group_name, *item_name;
   Eina_List *l;

   assert(pro != NULL);
   assert(part != NULL);

   #define GROUP_USAGE_DEL(TYPE) \
   group_name = edje_edit_part_ ## TYPE ## _get(part->group->edit_object, \
                                                part->name); \
   if (group_name) \
     { \
        pm_resource_usage_del(pro->groups, group_name, part); \
        edje_edit_string_free(group_name); \
     }

   switch (part->type)
     {
      case EDJE_PART_TYPE_BOX:
      case EDJE_PART_TYPE_TABLE:
         EINA_LIST_FOREACH(part->items, l, item_name)
           {
              group_name = edje_edit_part_item_source_get(part->group->edit_object,
                                                          part->name,
                                                          item_name);
              if (group_name)
                {
                   pm_resource_usage_del(pro->groups, group_name, part);
                   edje_edit_string_free(group_name);
                }
           }
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         GROUP_USAGE_DEL(source);
         GROUP_USAGE_DEL(source2);
         GROUP_USAGE_DEL(source3);
         GROUP_USAGE_DEL(source4);
         GROUP_USAGE_DEL(source5);
         GROUP_USAGE_DEL(source6);
         break;
      case EDJE_PART_TYPE_GROUP:
         GROUP_USAGE_DEL(source);
         break;
      default:
         break;
     }
   #undef GROUP_USAGE_DEL

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     edje_edit_string_list_free(part->items);

   EINA_LIST_FREE(part->states, state)
      gm_state_del(pro, state);

   part->used_in = eina_list_free(part->used_in);
   part->group->parts = eina_list_remove(part->group->parts, part);
   eina_stringshare_del(part->name);
   free(part);
}

void
gm_part_rename(Part_* part, const char *new_part_name)
{
   assert(part != NULL);
   assert(new_part_name != NULL);

   eina_stringshare_del(part->name);
   part->name = eina_stringshare_add(new_part_name);
}
