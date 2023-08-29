#include "resource_manager2.h"
#include "resource_manager_private.h"
#include "project_manager2.h"
#include "string_common.h"

Eina_Bool
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
        project->RM.global_data = eina_list_append(project->RM.global_data, res);
     }

   edje_edit_string_list_free(data);
   return true;
}

Eina_Bool
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

        project->RM.image_sets = eina_list_append(project->RM.image_sets, res);

        res->common.id = edje_edit_image_set_id_get(project->global_object, image_name);
        res->is_used = false;
     }

   edje_edit_string_list_free(images);
   return true;
}

Eina_Bool
_image_resources_load(Project *project)
{
   Image2 *res;
   Eina_List *images;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *image_name;
   Eina_List *l;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);
   images = edje_edit_images_list_get(project->global_object);
   EINA_LIST_FOREACH(images, l, image_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        if (!image_name) continue;
        if (!strcmp(image_name, EFLETE_DUMMY_IMAGE_NAME))
          continue;

        res = mem_calloc(1, sizeof(Image2));
        res->common.type = RESOURCE2_TYPE_IMAGE;
        res->common.name = eina_stringshare_add(image_name);
        res->comp_type = edje_edit_image_compression_type_get(project->global_object,
                                                              res->common.name);
        res->common.id = edje_edit_image_id_get(project->global_object, image_name);
        res->is_used = false;

        if (res->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
          res->source = eina_stringshare_add(image_name);
        else
          res->source = eina_stringshare_printf("%s/%s", resource_folder, image_name);

        project->RM.images = eina_list_append(project->RM.images, res);
     }

   edje_edit_string_list_free(images);
   eina_stringshare_del(resource_folder);
   return true;
}

Eina_Bool
_vector_resources_load(Project *project)
{
   Image2 *res;
   Eina_List *vectors;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *vector_name;
   Eina_List *l;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/vectors", project->develop_path);
   vectors = edje_edit_vectors_list_get(project->global_object);
   EINA_LIST_FOREACH(vectors, l, vector_name)
     {
        /* for supporting old themes, which were compilled
         * with edje_cc version less than 1.10 */
        res = mem_calloc(1, sizeof(Vector2));
        res->common.type = RESOURCE2_TYPE_VECTOR;
        res->common.name = eina_stringshare_add(vector_name);
        res->common.id = edje_edit_vector_id_get(project->global_object, vector_name);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, vector_name);

        project->RM.vectors = eina_list_append(project->RM.vectors, res);
     }

   edje_edit_string_list_free(vectors);
   eina_stringshare_del(resource_folder);
   return true;
}

Eina_Bool
_sound_resources_load(Project *project)
{
   Sound2 *res;
   Eina_List *sounds;
   Eina_Stringshare *resource_folder;
   Eina_Stringshare *sound_name, *sound_file;
   Eina_List *l;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);

   sounds = edje_edit_sound_samples_list_get(project->global_object);
   EINA_LIST_FOREACH(sounds, l, sound_name)
     {
        if (!strcmp(sound_name, EFLETE_DUMMY_SAMPLE_NAME))
          continue;

        sound_file = edje_edit_sound_samplesource_get(project->global_object, sound_name);

        res = mem_calloc(1, sizeof(Sound2));
        res->common.type = RESOURCE2_TYPE_SAMPLE;
        res->common.name = eina_stringshare_add(sound_name);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, sound_file);

        project->RM.sounds = eina_list_append(project->RM.sounds, res);

        edje_edit_string_free(sound_file);
     }

   edje_edit_string_list_free(sounds);
   eina_stringshare_del(resource_folder);
   return true;
}

Eina_Bool
_font_resources_load(Project *project)
{
   Font2 *res;
   Eina_List *fonts;
   Eina_Stringshare *resource_folder;
   Eina_List *l;
   Eina_Stringshare *font_name, *font_file;

   Eina_Bool result = true;

   assert(project != NULL);

   resource_folder = eina_stringshare_printf("%s/fonts", project->develop_path);

   fonts = edje_edit_fonts_list_get(project->global_object);

   if (eina_list_count(fonts) == 0)
     {
        res = NULL;
        goto cleanup;
     }

   EINA_LIST_FOREACH(fonts, l, font_name)
     {
        font_file = edje_edit_font_path_get(project->global_object, font_name);

        res = mem_calloc(1, sizeof(Font2));
        res->common.type = RESOURCE2_TYPE_FONT;
        res->common.name = eina_stringshare_add(font_name);
        res->source = eina_stringshare_printf("%s/%s", resource_folder, font_file);

        project->RM.fonts = eina_list_append(project->RM.fonts, res);
     }

cleanup:
   edje_edit_string_list_free(fonts);
   eina_stringshare_del(resource_folder);
   return result;
}

Eina_Bool
_tones_resources_load(Project *project)
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
        project->RM.tones = eina_list_append(project->RM.tones, res);
     }
    edje_edit_string_list_free(tones);

   return true;
}

Eina_Bool
_colorclasses_resources_load(Project *project)
{
   Eina_List *colorclasses, *l;
   Colorclass2 *res;
   Eina_Stringshare *name, *description;

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
          {
             description = edje_edit_color_class_description_get(project->global_object, name);
             res->description = eina_stringshare_add(description);
             project->RM.colorclasses = eina_list_append(project->RM.colorclasses, res);
          }
     }

   edje_edit_string_list_free(colorclasses);
   return true;
}

Eina_Bool
_styles_tag_resources_load(Project *pro, Eina_Stringshare *name, Style2 *style)
{
   char *pch, *tok, *data;
   Eina_Stringshare *value;
   Style_Tag2 *res;

   value = edje_edit_style_tag_value_get(pro->global_object,
                                         style->common.name,
                                         name);
   data = strdup(value);
   edje_edit_string_free(value);
   pch = strstr(data, "font");
   if (!pch)
     {
        free(data);
        return false;
     }
   pch += strlen_safe("font");
   tok = strtok(pch, " =");
   if (pch)
     {
        res = mem_calloc(1, sizeof(Style_Tag2));
        res->common.type = RESOURCE2_TYPE_STYLE_TAG;
        res->common.name = eina_stringshare_add(name);
        res->font = eina_stringshare_add(tok);
        style->tags = eina_list_append(style->tags, res);
        res->style = style;
     }
   free(data);
   return true;
}

Eina_Bool
_styles_resources_load(Project *project)
{
   Eina_List *styles, *tags, *l2, *l1;
   Style2 *res;
   Eina_Stringshare *name, *tag_value;

   assert(project != NULL);

   styles = edje_edit_styles_list_get(project->global_object);
   if (eina_list_count(styles) == 0)
     {
        edje_edit_string_list_free(styles);
        return false;
     }
   EINA_LIST_FOREACH(styles, l1, name)
     {
        res = mem_calloc(1, sizeof(Style2));
        res->common.type = RESOURCE2_TYPE_STYLE;
        res->common.name = eina_stringshare_add(name);

        tags = edje_edit_style_tags_list_get(project->global_object, name);
        EINA_LIST_FOREACH(tags, l2, tag_value)
          {
             _styles_tag_resources_load(project, tag_value, res);
          }
        project->RM.styles = eina_list_append(project->RM.styles, res);
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

State2 *
_state_add(Project *pro, Group2 *group, Part2 *part, const char *state_name, double state_value)
{
   State2 *state;
   Eina_Stringshare *image_name, *vector_name;
   Eina_List *tween_list, *l;
   Resource2 *res;

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

   if (part->type == EDJE_PART_TYPE_IMAGE)
     {
        image_name = edje_edit_state_image_get(group->edit_object,
                                               part->common.name,
                                               state->common.name,
                                               state->val);
        if (image_name && strcmp(image_name, EFLETE_DUMMY_IMAGE_NAME))
          state->normal = eina_stringshare_add(image_name);
        edje_edit_string_free(image_name);

        tween_list = edje_edit_state_tweens_list_get(group->edit_object,
                                                     part->common.name,
                                                     state->common.name,
                                                     state->val);
        EINA_LIST_FOREACH(tween_list, l, image_name)
          {
             if (edje_edit_image_set_exists(group->edit_object, image_name))
               res = resource_manager_find(pro->RM.image_sets, image_name);
             else
               res = resource_manager_find(pro->RM.images, image_name);
             state->tweens = eina_list_append(state->tweens, res);
          }
        edje_edit_string_list_free(tween_list);
     }

   if (part->type == EDJE_PART_TYPE_VECTOR)
     {
        vector_name = edje_edit_state_vector_get(group->edit_object,
                                                part->common.name,
                                                state->common.name,
                                                state->val);
        if (vector_name)
          state->normal = eina_stringshare_add(vector_name);
        edje_edit_string_free(vector_name);
     }

   return state;
}

Part_Item2 *
_part_item_add(Part2 *part, Eina_Stringshare *item_name, unsigned int i)
{
   Eina_List *l, *item_list;
   Part_Item2 *item, *item_data;

   item = mem_calloc(1, sizeof(Part_Item2));
   item->common.id = i;
   item->common.type = RESOURCE2_TYPE_ITEM;
   if (item_name)
     {
        item->common.name = eina_stringshare_add(item_name);
        edje_edit_string_free(item_name);
     }
   item->part = part;

   if (i != eina_list_count(part->items))
     {
        l = eina_list_nth_list(part->items, i);
        part->items = eina_list_prepend_relative_list(part->items, item, l);
        item_list = eina_list_data_find_list(part->items, item);
        EINA_LIST_FOREACH(eina_list_next(item_list), l, item_data)
          {
             item_data->common.id++;
          }
     }
   else
     {
        part->items = eina_list_append(part->items, item);
     }

   return item;
}

Part2 *
_part_add(Project *pro, Group2 *group, const char *part_name)
{
   Part2 *part;
   Eina_List *states, *l;
   Eina_Stringshare *state_name, *parsed_state_name, *item_name;
   double val;
   unsigned int items_count, i;

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
        _state_add(pro, group, part, parsed_state_name, val);
        eina_stringshare_del(parsed_state_name);
     }
   edje_edit_string_list_free(states);

   if ((part->type == EDJE_PART_TYPE_TABLE) ||
       (part->type == EDJE_PART_TYPE_BOX))
     {
        items_count = edje_edit_part_items_count_get(group->edit_object, part_name);
        for (i = 0; i < items_count; i++)
          {
             item_name = edje_edit_part_item_index_name_get(group->edit_object, part_name, i);
             _part_item_add(part, item_name, i);
          }
     }

   return part;
}

Group_Data2 *
_group_data_add(Project *pro, Group2 *group, Eina_Stringshare *group_data_name)
{
   Group_Data2 *group_data;

   assert(pro != NULL);
   assert(group_data_name != NULL);
   assert(group != NULL);

   group_data = mem_calloc(1, sizeof(Group_Data2));
   group_data->common.type = RESOURCE2_TYPE_DATA_GROUP;
   group_data->common.name = eina_stringshare_add(group_data_name);
   group_data->source = edje_edit_group_data_value_get(group->edit_object,
                                                       group_data_name);
   group->data_items = eina_list_append(group->data_items, group_data);

   return group_data;
}

Program2 *
_program_load(Group2 *group, Eina_Stringshare *program_name)
{
   Program2 *program;
   Eina_Stringshare *name;
   Eina_List *targets, *afters, *l2;
   Resource2 *res;

   program = mem_calloc(1, sizeof(Program2));
   program->common.type = RESOURCE2_TYPE_PROGRAM;
   program->common.name = eina_stringshare_add(program_name);
   program->type = edje_edit_program_action_get(group->edit_object,
                                                program_name);
   program->group = group;

   targets = edje_edit_program_targets_get(group->edit_object, program->common.name);
   EINA_LIST_FOREACH(targets, l2, name)
     {
        if (program->type == EDJE_ACTION_TYPE_ACTION_STOP)
          res = resource_manager_find(group->programs, name);
        else
          res = resource_manager_find(group->parts, name);
        if (res)
          program->targets = eina_list_append(program->targets, res);
     }
   edje_edit_string_list_free(targets);

   afters = edje_edit_program_afters_get(group->edit_object, program->common.name);
   EINA_LIST_FOREACH(afters, l2, name)
     {
        res = resource_manager_find(group->programs, name);
        if (res)
          program->afters = eina_list_append(program->afters, res);
     }
   edje_edit_string_list_free(afters);

   group->programs = eina_list_append(group->programs, program);

   return program;
}

void
_group_load(Project *pro, Group2 *group)
{
   Eina_List *parts, *l, *programs, *datas;
   Eina_Stringshare *part_name, *program_name, *group_data_name;

   assert(pro != NULL);
   assert(group != NULL);

   _group_name_parse(group);

   resource_group_edit_object_load(pro, group, evas_object_evas_get(pro->global_object));
   if (!edje_edit_group_alias_is(group->edit_object, group->common.name))
     {
        parts = edje_edit_parts_list_get(group->edit_object);
        EINA_LIST_FOREACH(parts, l, part_name)
           _part_add(pro, group, part_name);
        edje_edit_string_list_free(parts);

        datas = edje_edit_group_data_list_get(group->edit_object);
        EINA_LIST_FOREACH(datas, l, group_data_name)
           _group_data_add(pro, group, group_data_name);

        programs = edje_edit_programs_list_get(group->edit_object);
        EINA_LIST_FOREACH(programs, l, program_name)
          {
             _program_load(group, program_name);
          }
        edje_edit_string_list_free(programs);
     }

   resource_group_edit_object_unload(group);
}

static int
groupcmp(const void *d1, const void *d2)
{
   Group2 *g1 = (Group2 *)d1;
   Group2 *g2 = (Group2 *)d2;

   return strcmp(g1->common.name, g2->common.name);
}

Group2 *
_group_add(Project *pro, Eina_Stringshare *group_name)
{
   Group2 *res;

   res = mem_calloc(1, sizeof(Group2));
   res->common.type = RESOURCE2_TYPE_GROUP;
   res->common.name = eina_stringshare_ref(group_name);
   pro->RM.groups = eina_list_sorted_insert(pro->RM.groups, groupcmp,  res);

   return res;
}

void
_groups_load(Project *pro)
{
   Eina_List *collections, *l;
   Eina_Stringshare *group_name;
   Group2 *res;

   assert(pro != NULL);
   assert(pro->dev != NULL);
   assert(pro->RM.groups == NULL);

   collections = edje_file_collection_list(pro->dev);

   assert(collections != NULL);

   EINA_LIST_FREE(collections, group_name)
     {
        if (!strcmp(group_name, EFLETE_INTERNAL_GROUP_NAME)) continue;
        _group_add(pro, group_name);
        eina_stringshare_del(group_name);
     }

   EINA_LIST_FOREACH(pro->RM.groups, l, res)
     _group_load(pro, res);
}
