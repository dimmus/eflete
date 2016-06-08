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

#include "project_manager.h"
#include "widget_list.h"

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
   if (!edje_object_mmap_set(group->edit_object, pro->mmap_file, group->name))
     {
        ERR("Can't set mmap object");
        abort();
     }
}

void
gm_group_edit_object_reload(Project *pro, Group *group)
{
   Part *part;
   Eina_List *l;

   assert(pro != NULL);
   assert(group != NULL);
   assert(group->edit_object != NULL);

   if (!edje_object_mmap_set(group->edit_object, pro->mmap_file, group->name))
     {
        ERR("Can't set mmap object");
        abort();
     }

   EINA_LIST_FOREACH(group->parts, l, part)
      edje_edit_part_selected_state_set(group->edit_object,
                                        part->name,
                                        part->current_state->parsed_name,
                                        part->current_state->parsed_val);
}

void
gm_group_edit_object_unload(Group *group)
{
   assert(group != NULL);
   assert(group->edit_object != NULL);

   evas_object_del(group->edit_object);
   group->edit_object = NULL;
}

Eina_List *
_strings_list_duplicates_del(Eina_List *list)
{
   Eina_List *l, *l2;
   Eina_Stringshare *str, *str2;

   EINA_LIST_FOREACH(list, l, str)
     {
        EINA_LIST_FOREACH(eina_list_next(l), l2, str2)
          {
             if ((str == str2) || !strcmp(str, str2))
               {
                  list = eina_list_remove_list(list, l2);
                  eina_stringshare_del(str2);
               }
          }
     }
   return list;
}

#define SOURCE_GET(FUNC) \
do { \
   source = FUNC(obj, part); \
   if (source) \
     { \
        groups = eina_list_sorted_insert(groups, sort_cb, eina_stringshare_add(source)); \
        eina_stringshare_del(source); \
     } \
   } \
while (0);

Eina_List *
gm_group_used_groups_edj_get(const char *edj, const char *group)
{
   Eina_List *groups = NULL;
   Eina_List *parts, *l1;
   Eina_Stringshare *part, *source;
   Evas *e;
   Evas_Object *obj, *win;
   Edje_Part_Type type;
   int items_count, i;

   ecore_thread_main_loop_begin();
   win = elm_win_add(NULL, "eflete_group_groups_list_get", ELM_WIN_BASIC);
   elm_win_norender_push(win);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, edj, group);

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        type = edje_edit_part_type_get(obj, part);
        if ((EDJE_PART_TYPE_GROUP != type) ||
            (EDJE_PART_TYPE_TEXTBLOCK != type) ||
            (EDJE_PART_TYPE_BOX != type) ||
            (EDJE_PART_TYPE_TABLE != type))
          continue;
        SOURCE_GET(edje_edit_part_source_get)
        SOURCE_GET(edje_edit_part_source2_get)
        SOURCE_GET(edje_edit_part_source3_get)
        SOURCE_GET(edje_edit_part_source4_get)
        SOURCE_GET(edje_edit_part_source5_get)
        SOURCE_GET(edje_edit_part_source6_get)

        items_count = edje_edit_part_items_count_get(obj, part);
        for (i = 0; i < items_count; i++)
          {
             source = edje_edit_part_item_index_source_get(obj, part, i);
             if (source)
               {
                  groups = eina_list_sorted_insert(groups, sort_cb, eina_stringshare_add(source));
                  eina_stringshare_del(source);
               }
          }
     }
   edje_edit_string_list_free(parts);
   evas_object_del(win);
   ecore_thread_main_loop_end();

   return _strings_list_duplicates_del(groups);
}
#undef SOURCE_GET

Eina_List *
gm_group_used_images_edj_get(const char *edj, const char *group)
{
   Eina_List *images = NULL;
   Eina_List *parts, *l1, *states, *l2, *tweens;
   Eina_Stringshare *part, *state, *img, *pstate;
   double pvalue;
   Evas *e;
   Evas_Object *obj, *win;

   ecore_thread_main_loop_begin();
   win = elm_win_add(NULL, "eflete_group_images_list_get", ELM_WIN_BASIC);
   elm_win_norender_push(win);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, edj, group);

   parts = edje_edit_parts_list_get(obj);
   EINA_LIST_FOREACH(parts, l1, part)
     {
        if (EDJE_PART_TYPE_IMAGE != edje_edit_part_type_get(obj, part)) continue;
        states = edje_edit_part_states_list_get(obj, part);
        EINA_LIST_FOREACH(states, l2, state)
          {
             state_name_split(state, &pstate, &pvalue);
             img = edje_edit_state_image_get(obj, part, pstate, pvalue);
             images = eina_list_sorted_insert(images, sort_cb, eina_stringshare_add(img));
             eina_stringshare_del(img);
             eina_stringshare_del(pstate);

             tweens = edje_edit_state_tweens_list_get(obj, part, pstate, pvalue);
             images = eina_list_sorted_merge(images, tweens, sort_cb);
          }
     }
   edje_edit_string_list_free(parts);
   evas_object_del(win);
   ecore_thread_main_loop_end();

   return _strings_list_duplicates_del(images);
}

Eina_List *
gm_group_used_sample_edj_get(const char *edj, const char *group)
{
   Eina_List *samples = NULL;
   Eina_List *programs, *l1;
   Evas *e;
   Evas_Object *obj, *win;
   Eina_Stringshare *program, *sample;

   ecore_thread_main_loop_begin();
   win = elm_win_add(NULL, "eflete_group_samples_list_get", ELM_WIN_BASIC);
   elm_win_norender_push(win);
   e = evas_object_evas_get(win);
   obj = edje_edit_object_add(e);
   edje_object_file_set(obj, edj, group);

   programs = edje_edit_programs_list_get(obj);
   EINA_LIST_FOREACH(programs, l1, program)
     {
        if (EDJE_ACTION_TYPE_SOUND_SAMPLE != edje_edit_program_action_get(obj, program))
          continue;

        sample = edje_edit_program_sample_name_get(obj, program);
        if (sample)
          {
             samples = eina_list_sorted_insert(samples, sort_cb, eina_stringshare_add(sample));
             eina_stringshare_del(sample);
          }
     }
   edje_edit_string_list_free(programs);
   evas_object_del(win);
   ecore_thread_main_loop_end();

   return _strings_list_duplicates_del(samples);
}

State *
gm_state_add(Project *pro, Part *part, const char *state_name)
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

Part *
gm_part_add(Project *pro, Group *group, const char *part_name)
{
   Part *part;
   Eina_List *states, *l;
   Eina_Stringshare *state_name, *group_name, *item_name;

   assert(pro != NULL);
   assert(group != NULL);
   assert(part_name != NULL);

   part = mem_calloc(1, sizeof(Part));
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
   Eina_List *parts, *l, *lt, *programs, *targets, *datas;
   Eina_Stringshare *part_name, *program_name, *target_name, *state_name,
                    *state_full_name, *sample_name, *tone_name, *group_data_name;
   double state_val;
   Part *part;
   State *state;
   Program *program;

   assert(pro != NULL);
   assert(group != NULL);

   _group_name_parse(group);

   ecore_thread_main_loop_begin();
   gm_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
   if (edje_edit_group_alias_is(group->edit_object, group->name))
     {
        main_group_name = edje_edit_group_aliased_get(group->edit_object, group->name);
        group->main_group = pm_resource_get(pro->groups, main_group_name);
        if (!(group->main_group))
          {
             group->main_group = mem_calloc(1, sizeof(Group));
             group->main_group->name = eina_stringshare_add(main_group_name);
             pro->groups = eina_list_append(pro->groups, group->main_group);
          }
        group->main_group->aliases = eina_list_sorted_insert(group->main_group->aliases, (Eina_Compare_Cb)resource_cmp, group);
        edje_edit_string_free(main_group_name);
     }
   else
     {
        parts = edje_edit_parts_list_get(group->edit_object);
        EINA_LIST_FOREACH(parts, l, part_name)
           gm_part_add(pro, group, part_name);
        edje_edit_string_list_free(parts);

        datas = edje_edit_group_data_list_get(group->edit_object);
        EINA_LIST_FOREACH(datas, l, group_data_name)
           gm_group_data_add(pro, group, group_data_name);

        programs = edje_edit_programs_list_get(group->edit_object);
        EINA_LIST_FOREACH(programs, l, program_name)
          {
            program = mem_calloc(1, sizeof(Program));
            program->name = eina_stringshare_add(program_name);
            program->type = edje_edit_program_action_get(group->edit_object, program_name);
            group->programs = eina_list_sorted_insert(group->programs, (Eina_Compare_Cb)resource_cmp, program);
          }
        edje_edit_string_list_free(programs);
        EINA_LIST_FOREACH(group->programs, l, program)
          {
             switch (program->type)
               {
                case EDJE_ACTION_TYPE_STATE_SET:
                   targets = edje_edit_program_targets_get(group->edit_object, program->name);
                   state_name = edje_edit_program_state_get(group->edit_object, program->name);
                   state_val = edje_edit_program_value_get(group->edit_object, program->name);
                   state_full_name = eina_stringshare_printf("%s %.2f", state_name, state_val);
                   eina_stringshare_del(state_name);
                   EINA_LIST_FOREACH(targets, lt, target_name)
                     {
                        part = (Part *) pm_resource_unsorted_get(group->parts, target_name);
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
                   targets = edje_edit_program_targets_get(group->edit_object, program->name);
                   EINA_LIST_FOREACH(targets, lt, target_name)
                      pm_resource_usage_unsorted_add(group->parts, target_name, (void *) program);
                   edje_edit_string_list_free(targets);
                   break;
                case EDJE_ACTION_TYPE_SOUND_SAMPLE:
                   sample_name = edje_edit_program_sample_name_get(group->edit_object, program->name);
                   pm_resource_usage_add(pro->sounds, sample_name, (void *) program);
                   eina_stringshare_del(sample_name);
                   break;
                case EDJE_ACTION_TYPE_SOUND_TONE:
                   tone_name = edje_edit_program_tone_name_get(group->edit_object, program->name);
                   pm_resource_usage_add(pro->tones, tone_name, (void *) program);
                   eina_stringshare_del(tone_name);
                   break;
                default:
                   TODO("Handle other action types");
                   break;
               }
          }
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
   Part *part;
   Resource *program;
   Eina_List *l, *ln;

   assert(pro != NULL);
   assert(group != NULL);
   assert(group->edit_object == NULL);

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

Eina_Bool
_is_checked(Eina_List *widgets)
{
   Eina_List *l, *ll, *lll;
   Tree_Item_Data *widget, *style;
   End_Item_Data *item_style;

   EINA_LIST_FOREACH(widgets, l, widget)
     {
        if (widget->check) return EINA_TRUE;
        EINA_LIST_FOREACH(widget->list, ll, style)
          {
             if (style->check) return EINA_TRUE;
             EINA_LIST_FOREACH(style->list, lll, item_style)
               if (item_style->check) return EINA_TRUE;
          }
     }

   return EINA_FALSE;
}

void
gm_groups_load(Project *pro)
{
   Eina_List *collections, *l, *wl, *wll, *wlll;
   Eina_Stringshare *group_name;
   const char *widget_name;
   Tree_Item_Data *widget, *style;
   End_Item_Data *item_style;
   Group *group;
   Eina_Bool check, is_checked;

   assert(pro != NULL);
   assert(pro->dev != NULL);
   assert(pro->groups == NULL);

   collections = edje_file_collection_list(pro->dev);

   assert(collections != NULL);

   is_checked = _is_checked(pro->widgets);

   collections = eina_list_sort(collections, eina_list_count(collections), (Eina_Compare_Cb) strcmp);
   EINA_LIST_FOREACH(collections, l, group_name)
     {
        check = false;
        if (!strcmp(group_name, EFLETE_INTERNAL_GROUP_NAME)) continue;

        if (pro->widgets && is_checked)
          {
             widget_name = widget_name_get(group_name);
             if (!widget_name) continue;
             EINA_LIST_FOREACH(pro->widgets, wl, widget)
               {
                  EINA_LIST_FOREACH(widget->list, wll, style)
                    {
                       if (style->check)
                         {
                            if (!strcmp(widget->name, widget_name) &&
                                style_name_check(group_name, style->name))
                              {
                                 check = true;
                                 break;
                              }
                         }
                       EINA_LIST_FOREACH(style->list, wlll, item_style)
                         {
                            if (item_style->check)
                              {
                                 if (!strcmp(widget->name, widget_name) &&
                                     item_style_name_check(group_name, item_style->name, widget->list))
                                   {
                                      check = true;
                                      break;
                                   }
                              }
                         }
                    }
               }
             if (!check) continue;
          }
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
   Part *part;
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
//   Eina_Stringshare *name, *image_name;
//   Eina_List *tween_list, *l;

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
TODO("fix usage adding on properties change before using this code")
#if 0
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
#endif
   state->used_in = eina_list_free(state->used_in);
   state->part->states = eina_list_remove(state->part->states, state);
   eina_stringshare_del(state->parsed_name);
   eina_stringshare_del(state->name);
   free(state);

#undef USAGE_DEL
}

void
gm_part_item_add(Project *pro, Part *part, Eina_Stringshare *item_name)
{
   assert(pro != NULL);
   assert(part != NULL);
   assert(item_name != NULL);
   assert((part->type ==  EDJE_PART_TYPE_BOX) ||
          (part->type ==  EDJE_PART_TYPE_TABLE));

   part->items = eina_list_append(part->items, eina_stringshare_ref(item_name));
}

void
gm_part_item_del(Project *pro, Part *part, Eina_Stringshare *item_name)
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
gm_part_del(Project *pro, Part* part)
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
gm_part_rename(Part* part, const char *new_part_name)
{
   assert(part != NULL);
   assert(new_part_name != NULL);

   eina_stringshare_del(part->name);
   part->name = eina_stringshare_add(new_part_name);
}

void
gm_part_restack(Part *part, Part *rel_part)
{
   Eina_List *rel_l;

   assert(part != NULL);

   part->group->parts = eina_list_remove(part->group->parts, part);

   if (rel_part)
     {
        rel_l = eina_list_data_find_list(part->group->parts, rel_part);
        assert (rel_l != NULL);
        part->group->parts = eina_list_prepend_relative_list(part->group->parts, part, rel_l);
     }
   else
     part->group->parts = eina_list_append(part->group->parts, part);
}

void
gm_part_item_restack(Part *part, Eina_Stringshare *part_item, Eina_Stringshare *relative_part_item)
{
   assert(part != NULL);
   assert(part_item != NULL);

   part->items = eina_list_remove(part->items, part_item);
   if (relative_part_item)
     part->items = eina_list_prepend_relative(part->items,
                                              part_item,
                                              relative_part_item);
   else
     part->items = eina_list_append(part->items, part_item);
}

void
gm_program_add(Project *pro, Group *group, Eina_Stringshare *program_name)
{
   Program *program;

   assert(pro != NULL);
   assert(program_name != NULL);
   assert(group != NULL);

   program = mem_calloc(1, sizeof(Program));
   program->name = eina_stringshare_add(program_name);
   program->type = edje_edit_program_action_get(group->edit_object, program_name);
   group->programs = eina_list_sorted_insert(group->programs, (Eina_Compare_Cb)resource_cmp, program);
}

void
gm_program_del(Project *pro, Group *group, Eina_Stringshare *program_name)
{
   Eina_List *l;
   Resource *program;

   assert(pro != NULL);
   assert(program_name != NULL);
   assert(group != NULL);

   program = pm_resource_get(group->programs, program_name);

   assert(program != NULL);

   l = eina_list_data_find_list(group->programs, program);

   assert(l != NULL);

   group->programs = eina_list_remove_list(group->programs, l);
   eina_stringshare_del(program->name);
   eina_list_free(program->used_in);
   free(program);
}

void
gm_group_data_add(Project *pro, Group *group, Eina_Stringshare *group_data_name)
{
   Resource *group_data;

   assert(pro != NULL);
   assert(group_data_name != NULL);
   assert(group != NULL);

   group_data = mem_calloc(1, sizeof(Program));
   group_data->name = eina_stringshare_add(group_data_name);
   group->data_items = eina_list_sorted_insert(group->data_items, (Eina_Compare_Cb)resource_cmp, group_data);
}

void
gm_group_data_del(Project *pro, Group *group, Eina_Stringshare *group_data_name)
{
   Eina_List *l;
   Resource *group_data;

   assert(pro != NULL);
   assert(group_data_name != NULL);
   assert(group != NULL);

   group_data = pm_resource_get(group->data_items, group_data_name);

   assert(group_data != NULL);

   l = eina_list_data_find_list(group->data_items, group_data);

   assert(l != NULL);

   group->data_items = eina_list_remove_list(group->data_items, l);
   eina_stringshare_del(group_data->name);
   eina_list_free(group_data->used_in);
   free(group_data);
}

void
gm_group_data_rename(Project *pro, Group *group, Resource* group_data, const char *new_data_name)
{
   Eina_List *l;

   assert(pro != NULL);
   assert(group != NULL);
   assert(group_data != NULL);
   assert(new_data_name != NULL);

   l = eina_list_data_find_list(group->data_items, group_data);

   assert(l != NULL);

   group->data_items = eina_list_remove_list(group->data_items, l);
   eina_stringshare_del(group_data->name);
   group_data->name = eina_stringshare_add(new_data_name);
   group->data_items = eina_list_sorted_insert(group->data_items, (Eina_Compare_Cb)resource_cmp, group_data);
}

/**
 * ref http://docs.enlightenment.org/auto/edje/group__Edje__Object__Part.html
 */
static char *part_types[] = {
     "NONE",
     "RECTANGLE",
     "TEXT",
     "IMAGE",
     "SWALLOW",
     "TEXTBLOCK",
     "GRADIENT",
     "GROUP",
     "BOX",
     "TABLE",
     "EXTERNAL",
     "PROXY",
     "SPACER",
     "MESH NODE",
     "LIGHT",
     "CAMERA",
     "SNAPSHOT"
};
static unsigned int part_types_count = 16;

const char *
gm_part_type_text_get(Edje_Part_Type part_type)
{
   assert(part_type <= part_types_count);

   return part_types[part_type];
}
