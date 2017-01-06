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
#include "tabs.h"
#include "string_common.h"

/* CALLBACK FUNCTIONS */

static void
_property_resource_attribute_changed(void *data,
                                     Evas_Object *obj __UNUSED__,
                                     void *event_info)
{
   Colorclass2 *cc_res;
   Style2 *style_res;
   Style_Tag2 *style_tag;

   Editor_Attribute_Resource_Change *change = event_info;
   Project *pro = (Project *)data;

   RM_Attribute_Resources editor_resource = change->attribute;
   switch (editor_resource)
     {
      case RM_ATTRIBUTE_RESOURCES_COLORCLASS_DESCRIPTION:
         cc_res = (Colorclass2 *)resource_manager_find(pro->RM.colorclasses,
                                                       change->color_class_name);
         eina_stringshare_del(cc_res->description);
         cc_res->description = eina_stringshare_add(change->color_class_description);
         break;
      case RM_ATTRIBUTE_RESOURCES_COLORCLASS_COLORS:
         cc_res = (Colorclass2 *)resource_manager_find(pro->RM.colorclasses,
                                                       change->color_class_name);
         cc_res->color1.r = change->r;
         cc_res->color1.g = change->g;
         cc_res->color1.b = change->b;
         cc_res->color1.a = change->a;
         cc_res->color2.r = change->r2;
         cc_res->color2.g = change->g2;
         cc_res->color2.b = change->b2;
         cc_res->color2.a = change->a2;
         cc_res->color3.r = change->r3;
         cc_res->color3.g = change->g3;
         cc_res->color3.b = change->b3;
         cc_res->color3.a = change->a3;
         break;
      case RM_ATTRIBUTE_RESOURCES_STYLE_TAG_ADDED:
         style_res = (Style2 *)resource_manager_find(pro->RM.styles,
                                                          change->style_name);
         style_tag = mem_calloc(1, sizeof(Style_Tag2));
         style_tag->common.type = RESOURCE2_TYPE_STYLE_TAG;
         style_tag->common.name = eina_stringshare_add(change->tag_name);
         style_tag->style = style_res;

         style_res->tags = eina_list_append(style_res->tags, style_tag);
         break;
      case RM_ATTRIBUTE_RESOURCES_STYLE_TAG_DELETED:
         style_res = (Style2 *)resource_manager_find(pro->RM.styles,
                                                          change->style_name);
         style_tag = (Style_Tag2 *)resource_manager_find(style_res->tags,
                                                     change->tag_name);
         if (style_tag)
           _resource_style_tag_free(style_tag);
         break;
      default:
         break;
     }
}

static void
_property_attribute_changed(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   TODO("Update editor and resource manager (read comment below)")
   /***********************************************************************
    ***********************************************************************
    **                          IMPORTANT NOTE:                          **
    ***********************************************************************
    ** This function is still WIP. That means that it will be            **
    ** fully usable when edje-edit will be fully moved to Model.         **
    ** So every edje_edit changes after editor will come in here         **
    ** to update part, state and all other structures.                   **
    **                                                                   **
    ** Right now only few attributes could be usable,                    **
    ** for example those who changes resource usage (color_class,        **
    ** sound, etc).                                                      **
    **                                                                   **
    ** TODO:                                                             **
    ** > edje-edit to model, expand structures to save entire data       **
    ***********************************************************************
    ***********************************************************************/
   Resource2 *part, *state, *source, *old_source, *item, *program, *group_data;

   Editor_Attribute_Change *change = (Editor_Attribute_Change *)event_info;
   Attribute editor_resource = (int)change->attribute;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();

   /* do not update if change was called from resource_manager_free */
   if ((!group) || (group->edit_object != change->edit_object))
     return;

   switch ((int)change->attribute)
     {
      case RM_ATTRIBUTE_GROUP_MIN_W:
      case RM_ATTRIBUTE_GROUP_MIN_H:
      case RM_ATTRIBUTE_GROUP_MAX_W:
      case RM_ATTRIBUTE_GROUP_MAX_H:
      case RM_ATTRIBUTE_STATE_MIN_W:
      case RM_ATTRIBUTE_STATE_MIN_H:
      case RM_ATTRIBUTE_STATE_MAX_W:
      case RM_ATTRIBUTE_STATE_MAX_H:
      case RM_ATTRIBUTE_STATE_ALIGN_X:
      case RM_ATTRIBUTE_STATE_ALIGN_Y:
      case RM_ATTRIBUTE_STATE_REL1_RELATIVE_X:
      case RM_ATTRIBUTE_STATE_REL1_RELATIVE_Y:
      case RM_ATTRIBUTE_STATE_REL2_RELATIVE_X:
      case RM_ATTRIBUTE_STATE_REL2_RELATIVE_Y:
      case RM_ATTRIBUTE_STATE_REL1_OFFSET_X:
      case RM_ATTRIBUTE_STATE_REL1_OFFSET_Y:
      case RM_ATTRIBUTE_STATE_REL2_OFFSET_X:
      case RM_ATTRIBUTE_STATE_REL2_OFFSET_Y:
      case RM_ATTRIBUTE_STATE_ASPECT_MIN:
      case RM_ATTRIBUTE_STATE_ASPECT_MAX:
      case RM_ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_X:
      case RM_ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_Y:
      case RM_ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_X:
      case RM_ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_Y:
      case RM_ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_X:
      case RM_ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_Y:
      case RM_ATTRIBUTE_STATE_FILL_SIZE_OFFSET_X:
      case RM_ATTRIBUTE_STATE_FILL_SIZE_OFFSET_Y:
      case RM_ATTRIBUTE_STATE_TEXT_ALIGN_X:
      case RM_ATTRIBUTE_STATE_TEXT_ALIGN_Y:
      case RM_ATTRIBUTE_STATE_TEXT_ELIPSIS:
      case RM_ATTRIBUTE_STATE_TEXT_SIZE:
      case RM_ATTRIBUTE_STATE_TEXT_FIT_X:
      case RM_ATTRIBUTE_STATE_TEXT_FIT_Y:
      case RM_ATTRIBUTE_STATE_TEXT_MAX_X:
      case RM_ATTRIBUTE_STATE_TEXT_MAX_Y:
      case RM_ATTRIBUTE_STATE_TEXT_MIN_X:
      case RM_ATTRIBUTE_STATE_TEXT_MIN_Y:
      case RM_ATTRIBUTE_STATE_FIXED_H:
      case RM_ATTRIBUTE_STATE_FIXED_W:
      case RM_ATTRIBUTE_STATE_REL1_TO_X:
      case RM_ATTRIBUTE_STATE_REL1_TO_Y:
      case RM_ATTRIBUTE_STATE_REL2_TO_X:
      case RM_ATTRIBUTE_STATE_REL2_TO_Y:
      case RM_ATTRIBUTE_STATE_TEXT:
      case RM_ATTRIBUTE_STATE_FONT:
      case RM_ATTRIBUTE_STATE_ASPECT_PREF:
      case RM_ATTRIBUTE_PART_TEXT_EFFECT:
      case RM_ATTRIBUTE_PART_TEXT_SHADOW_DIRECTION:
      case RM_ATTRIBUTE_PART_CLIP_TO:
      case RM_ATTRIBUTE_PART_DRAG_CONFINE:
      case RM_ATTRIBUTE_STATE_CONTAINER_ALIGN_X:
      case RM_ATTRIBUTE_STATE_CONTAINER_ALIGN_Y:
      case RM_ATTRIBUTE_STATE_CONTAINER_MIN_H:
      case RM_ATTRIBUTE_STATE_CONTAINER_MIN_V:
      case RM_ATTRIBUTE_STATE_TABLE_HOMOGENEOUS:
      case RM_ATTRIBUTE_STATE_CONTAINER_PADING_X:
      case RM_ATTRIBUTE_STATE_CONTAINER_PADING_Y:
      case RM_ATTRIBUTE_STATE_MINMUL_H:
      case RM_ATTRIBUTE_STATE_MINMUL_W:
      case RM_ATTRIBUTE_PART_MULTILINE:
      case RM_ATTRIBUTE_PART_ENTRY_MODE:
      case RM_ATTRIBUTE_STATE_FILL_TYPE:
      case RM_ATTRIBUTE_STATE_COLOR:
      case RM_ATTRIBUTE_STATE_OUTLINE_COLOR:
      case RM_ATTRIBUTE_STATE_SHADOW_COLOR:
      case RM_ATTRIBUTE_STATE_MAP_ROTATION_CENTER:
      case RM_ATTRIBUTE_STATE_MAP_ROTATION_X:
      case RM_ATTRIBUTE_STATE_MAP_ROTATION_Y:
      case RM_ATTRIBUTE_STATE_MAP_ROTATION_Z:
      case RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_1:
      case RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_2:
      case RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_3:
      case RM_ATTRIBUTE_STATE_MAP_POINT_COLOR_4:
      case RM_ATTRIBUTE_STATE_FILL_SMOOTH:
      case RM_ATTRIBUTE_STATE_VISIBLE:
      case RM_ATTRIBUTE_STATE_IMAGE_BORDER_TOP:
      case RM_ATTRIBUTE_STATE_IMAGE_BORDER_BOTTOM:
      case RM_ATTRIBUTE_STATE_IMAGE_BORDER_LEFT:
      case RM_ATTRIBUTE_STATE_IMAGE_BORDER_RIGHT:
      case RM_ATTRIBUTE_STATE_IMAGE_BORDER_FILL:
      case RM_ATTRIBUTE_STATE_MAP_ON:
      case RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE_ON:
      case RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE:
      case RM_ATTRIBUTE_STATE_MAP_LIGHT:
      case RM_ATTRIBUTE_STATE_MAP_SMOOTH:
      case RM_ATTRIBUTE_STATE_MAP_ALPHA:
      case RM_ATTRIBUTE_STATE_MAP_BACKFACE_CULL:
      case RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE_FOCAL:
      case RM_ATTRIBUTE_STATE_MAP_PERSPECTIVE_ZPLANE:
      case RM_ATTRIBUTE_PART_ITEM_ASPECT_MODE:
      case RM_ATTRIBUTE_PART_ITEM_ALIGN_X:
      case RM_ATTRIBUTE_PART_ITEM_ALIGN_Y:
      case RM_ATTRIBUTE_PART_ITEM_WEIGHT_X:
      case RM_ATTRIBUTE_PART_ITEM_WEIGHT_Y:
      case RM_ATTRIBUTE_PART_ITEM_ASPECT_H:
      case RM_ATTRIBUTE_PART_ITEM_ASPECT_W:
      case RM_ATTRIBUTE_PART_ITEM_MAX_H:
      case RM_ATTRIBUTE_PART_ITEM_MAX_W:
      case RM_ATTRIBUTE_PART_ITEM_MIN_H:
      case RM_ATTRIBUTE_PART_ITEM_MIN_W:
      case RM_ATTRIBUTE_PART_ITEM_PREFER_H:
      case RM_ATTRIBUTE_PART_ITEM_PREFER_W:
      case RM_ATTRIBUTE_PART_ITEM_SPREAD_H:
      case RM_ATTRIBUTE_PART_ITEM_SPREAD_W:
      case RM_ATTRIBUTE_PART_ITEM_SPAN_COL:
      case RM_ATTRIBUTE_PART_ITEM_SPAN_ROW:
      case RM_ATTRIBUTE_PART_ITEM_POSITION_COL:
      case RM_ATTRIBUTE_PART_ITEM_POSITION_ROW:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_RIGHT:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_LEFT:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_TOP:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_BOTTOM:
         break;
      case RM_ATTRIBUTE_PROGRAM_AFTER:
         program = resource_manager_find(group->programs, change->program_name);

         /* if old value - its delete after */
         if (change->old_value)
           {
              old_source = resource_manager_find(group->programs, change->old_value);
              _resource_usage_resource_del(program, old_source);
              ((Program2 *)program)->afters = eina_list_remove(((Program2 *)program)->afters, old_source);
           }

         /* if value - its add after */
         if (change->value)
           {
              source = resource_manager_find(group->programs, change->value);
              _resource_usage_resource_add(program, source);
              ((Program2 *)program)->afters = eina_list_append(((Program2 *)program)->afters, source);
           }
         break;
      case RM_ATTRIBUTE_PROGRAM_TARGET:
         program = resource_manager_find(group->programs, change->program_name);

         /* if old value - its delete target */
         if (change->old_value)
           {
              if (((Program2 *)program)->type == EDJE_ACTION_TYPE_ACTION_STOP)
                old_source = resource_manager_find(group->programs, change->old_value);
              else
                old_source = resource_manager_find(group->parts, change->old_value);

              _resource_usage_resource_del(program, old_source);
              ((Program2 *)program)->targets = eina_list_remove(((Program2 *)program)->targets, old_source);
           }

         /* if value - its add after */
         if (change->value)
           {
              if (((Program2 *)program)->type == EDJE_ACTION_TYPE_ACTION_STOP)
                source = resource_manager_find(group->programs, change->value);
              else
                source = resource_manager_find(group->parts, change->value);

              _resource_usage_resource_add(program, source);
              ((Program2 *)program)->targets = eina_list_append(((Program2 *)program)->targets, source);
           }
         break;
      case RM_ATTRIBUTE_STATE_IMAGE:
         part = resource_manager_find(group->parts, change->part_name);
         state = resource_manager_v_find(((Part2 *)part)->states, change->state_name, change->state_value);

         if (change->old_value && strcmp(change->old_value, EFLETE_DUMMY_IMAGE_NAME))
           {
              old_source = resource_manager_find(pro->RM.images, change->old_value);
              /* in case if not implemented image set was set up as tween, need
                 to remove it and never come back (until image sets will be
                 implemented of course) */
              if (!old_source)
                old_source = resource_manager_find(pro->RM.image_sets, change->old_value);
              _resource_usage_resource_del(state, old_source);
           }
         TODO("Support some image sets here");

         if (change->value && strcmp(change->value, EFLETE_DUMMY_IMAGE_NAME))
           {
              source = resource_manager_find(pro->RM.images, change->value);
              if (!source)
                source = resource_manager_find(pro->RM.image_sets, change->value);
              _resource_usage_resource_add(state, source);
           }
         eina_stringshare_del(((State2 *)state)->normal);
         ((State2 *)state)->normal = eina_stringshare_add(change->value);
         break;
      case RM_ATTRIBUTE_STATE_IMAGE_TWEEN:
         part = resource_manager_find(group->parts, change->part_name);
         state = resource_manager_v_find(((Part2 *)part)->states, change->state_name, change->state_value);

         if (change->old_value)
           {
              old_source = resource_manager_find(pro->RM.images, change->old_value);
              /* in case if not implemented image set was set up as tween, need
                 to remove it and never come back (until image sets will be
                 implemented of course) */
              if (!old_source)
                old_source = resource_manager_find(pro->RM.image_sets, change->old_value);
              _resource_usage_resource_del(state, old_source);
              ((State2 *)state)->tweens = eina_list_remove(((State2 *)state)->tweens, old_source);
           }
         if (change->value)
           {
              source = resource_manager_find(pro->RM.images, change->value);
              if (!source)
                source = resource_manager_find(pro->RM.image_sets, change->value);
              _resource_usage_resource_add(state, source);
              ((State2 *)state)->tweens = eina_list_append(((State2 *)state)->tweens, source);
           }
         break;
      case RM_ATTRIBUTE_PROGRAM_SAMPLE_NAME:
         program = resource_manager_find(group->programs, change->program_name);

         if (change->old_value && strcmp(change->old_value, EFLETE_DUMMY_SAMPLE_NAME))
           {
              old_source = resource_manager_find(pro->RM.sounds, change->old_value);
              _resource_usage_resource_del(program, old_source);
           }

         if (change->value && strcmp(change->value, EFLETE_DUMMY_SAMPLE_NAME))
           {
              source = resource_manager_find(pro->RM.sounds, change->value);
              _resource_usage_resource_add(program, source);
           }
         break;
      case RM_ATTRIBUTE_PROGRAM_TONE_NAME:
         program = resource_manager_find(group->programs, change->program_name);

         if (change->old_value && strcmp(change->old_value, ""))
           {
              old_source = resource_manager_find(pro->RM.tones, change->old_value);
              _resource_usage_resource_del(program, old_source);
           }

         if (change->value && strcmp(change->value, ""))
           {
              source = resource_manager_find(pro->RM.tones, change->value);
              _resource_usage_resource_add(program, source);
           }
         break;
      case RM_ATTRIBUTE_STATE_TEXT_STYLE:
         part = resource_manager_find(group->parts, change->part_name);
         state = resource_manager_v_find(((Part2 *)part)->states, change->state_name, change->state_value);

         if (change->old_value)
           {
              old_source = resource_manager_find(pro->RM.styles, change->old_value);
              _resource_usage_resource_del(state, old_source);
           }

         if (change->value)
           {
              source = resource_manager_find(pro->RM.styles, change->value);
              _resource_usage_resource_add(state, source);
           }
         break;
      case RM_ATTRIBUTE_STATE_COLOR_CLASS:
         part = resource_manager_find(group->parts, change->part_name);
         state = resource_manager_v_find(((Part2 *)part)->states, change->state_name, change->state_value);

         if (change->old_value)
           {
              old_source = resource_manager_find(pro->RM.colorclasses, change->old_value);
              _resource_usage_resource_del(state, old_source);
           }

         if (change->value)
           {
              source = resource_manager_find(pro->RM.colorclasses, change->value);
              _resource_usage_resource_add(state, source);
           }
         break;
      case RM_ATTRIBUTE_PROGRAM_FILTER_STATE:
         program = resource_manager_find(group->programs, change->program_name);
         part = resource_manager_find(group->parts, ((Program2 *)program)->filter_part);

         if (change->old_value)
           {
              old_source = resource_manager_find(((Part2 *)part)->states, change->old_value);
              _resource_usage_resource_del(program, old_source);
           }

         if (change->value)
           {
              source = resource_manager_find(((Part2 *)part)->states, change->value);
              _resource_usage_resource_add(program, source);
           }
         break;
      case RM_ATTRIBUTE_PROGRAM_FILTER_PART:
         program = resource_manager_find(group->programs, change->program_name);

         if (change->old_value)
           {
              old_source = resource_manager_find(group->parts, change->old_value);
              _resource_usage_resource_del(program, old_source);
              eina_stringshare_del(((Program2 *)program)->filter_part);
              ((Program2 *)program)->filter_part = NULL;
           }

         if (change->value)
           {
              source = resource_manager_find(group->parts, change->value);
              _resource_usage_resource_add(program, source);
              ((Program2 *)program)->filter_part = eina_stringshare_add(change->value);
           }
         break;
      case RM_ATTRIBUTE_STATE_PROXY_SOURCE:
      case RM_ATTRIBUTE_STATE_TEXT_SOURCE:
      case RM_ATTRIBUTE_STATE_TEXT_TEXT_SOURCE:
         part = resource_manager_find(group->parts, change->part_name);
         state = resource_manager_v_find(((Part2 *)part)->states, change->state_name, change->state_value);

         if (change->old_value)
           {
              old_source = resource_manager_find(group->parts, change->old_value);
              _resource_usage_resource_del(state, old_source);
           }

         if (change->value)
           {
              source = resource_manager_find(group->parts, change->value);
              _resource_usage_resource_add(state, source);
           }
         break;
      case RM_ATTRIBUTE_PART_ITEM_SOURCE:
         part = resource_manager_find(group->parts, change->part_name);
         item = resource_manager_find(((Part2 *)part)->items, change->item_name);

         /* if old_valuye wasn't null and wasn't compared to EFLETE_INTERNAL_GROUP_NAME */
         if (change->old_value && strcmp(change->old_value, EFLETE_INTERNAL_GROUP_NAME))
           {
              old_source = resource_manager_find(pro->RM.groups, change->old_value);
              _resource_usage_resource_del(item, old_source);
           }

         /* if new value is not null and not internal group */
         if (change->value && strcmp(change->value, EFLETE_INTERNAL_GROUP_NAME))
           {
              source = resource_manager_find(pro->RM.groups, change->value);
              _resource_usage_resource_add(item, source);
           }
         break;
      case RM_ATTRIBUTE_PART_GROUP_SOURCE:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_SELECTION_UNDER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_SELECTION_OVER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_CURSOR_UNDER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_CURSOR_OVER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_UNDER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_OVER:
         part = resource_manager_find(group->parts, change->part_name);

         if (change->old_value)
           {
              old_source = resource_manager_find(pro->RM.groups, change->old_value);
              _resource_usage_resource_del(part, old_source);
           }

         if (change->value)
           {
              source = resource_manager_find(pro->RM.groups, change->value);
              _resource_usage_resource_add(part, source);
           }
         break;
      case RM_ATTRIBUTE_PART_NAME:
         part = resource_manager_find(group->parts, change->old_value);
         eina_stringshare_del(part->common.name);
         part->common.name = eina_stringshare_add(change->value);
         break;
      case RM_ATTRIBUTE_GROUP_DATA_NAME:
         group_data = resource_manager_find(group->data_items, change->old_value);
         eina_stringshare_del(group_data->common.name);
         group_data->common.name = eina_stringshare_add(change->value);
         break;
      default:
         break;
     }
   evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_ATTRIBUTE_CHANGED, &editor_resource);
}

static void
_colorclass_added(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *ei)
{
   Colorclass2 *res;
   const char *name = (const char *)ei;
   Project *project = (Project *)data;

   res = mem_calloc(1, sizeof(Colorclass2));
   res->common.type = RESOURCE2_TYPE_COLORCLASS;
   res->common.name = eina_stringshare_add(name);
   project->RM.colorclasses = eina_list_append(project->RM.colorclasses, res);
}

static void
_colorclass_deleted(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *ei)
{
   const char *name = (const char *)ei;
   Project *pro = (Project *)data;
   Colorclass2 *res_colorclass;

   res_colorclass = (Colorclass2 *)resource_manager_find(pro->RM.colorclasses,
                                                         name);
   _resource_colorclass_del(pro, res_colorclass);
}

static void
_sound_added(void *data,
             Evas_Object *obj __UNUSED__,
             void *ei)
{
   Sound2 *res;
   Eina_Stringshare *resource_folder, *sound_file;
   const char *name = (const char *)ei;
   Project *project = (Project *)data;

   sound_file = edje_edit_sound_samplesource_get(project->global_object, name);
   resource_folder = eina_stringshare_printf("%s/sounds", project->develop_path);

   res = mem_calloc(1, sizeof(Sound2));
   res->common.type = RESOURCE2_TYPE_SAMPLE;
   res->common.name = eina_stringshare_add(name);
   res->source = eina_stringshare_printf("%s/%s", resource_folder, sound_file);

   project->RM.sounds = eina_list_append(project->RM.sounds, res);
   edje_edit_string_free(sound_file);
}

static void
_sound_deleted(void *data,
               Evas_Object *obj __UNUSED__,
               void *ei)
{
   const char *name = (const char *)ei;
   Project *pro = (Project *)data;
   Sound2 *res_sound;

   res_sound = (Sound2 *)resource_manager_find(pro->RM.sounds, name);
   _resource_sound_del(pro, res_sound);
}

static void
_tone_added(void *data,
            Evas_Object *obj __UNUSED__,
            void *ei)
{
   Tone2 *res;
   const char *name = (const char *)ei;
   Project *project = (Project *)data;

   res = mem_calloc(1, sizeof(Tone2));
   res->common.type = RESOURCE2_TYPE_TONE;
   res->common.name = eina_stringshare_add(name);
   res->freq = edje_edit_sound_tone_frequency_get(project->global_object, name);
   project->RM.tones = eina_list_append(project->RM.tones, res);
}

static void
_tone_deleted(void *data,
               Evas_Object *obj __UNUSED__,
               void *ei)
{
   const char *name = (const char *)ei;
   Project *pro = (Project *)data;
   Tone2 *res_tone;

   res_tone = (Tone2 *)resource_manager_find(pro->RM.tones, name);
   _resource_tone_del(pro, res_tone);
}

static void
_image_added(void *data,
             Evas_Object *obj __UNUSED__,
             void *ei)
{
   Image2 *res;
   const char *name = (const char *)ei;
   Project *project = (Project *)data;
   Eina_Stringshare *resource_folder;

   resource_folder = eina_stringshare_printf("%s/images", project->develop_path);

   res = mem_calloc(1, sizeof(Image2));
   res->common.type = RESOURCE2_TYPE_IMAGE;
   res->common.name = eina_stringshare_add(ecore_file_file_get(name));
   res->comp_type = edje_edit_image_compression_type_get(project->global_object,
                                                         res->common.name);
   if (res->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
     res->source = eina_stringshare_add(name);
   else
     res->source = eina_stringshare_printf("%s/%s", resource_folder, ecore_file_file_get(name));

   project->RM.images = eina_list_append(project->RM.images, res);
   eina_stringshare_del(resource_folder);
}

static void
image_deleted(void *data,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   const char *name = (const char *)ei;
   Project *pro = (Project *)data;
   Image2 *res_image;

   res_image = (Image2 *)resource_manager_find(pro->RM.images, name);
   _resource_image_del(pro, res_image);
}

static void
_style_added(void *data,
             Evas_Object *obj __UNUSED__,
             void *ei)
{
   Style2 *res;
   const char *name = (const char *)ei;
   Project *project = (Project *)data;

   res = mem_calloc(1, sizeof(Style2));
   res->common.type = RESOURCE2_TYPE_STYLE;
   res->common.name = eina_stringshare_add(name);
   project->RM.styles = eina_list_append(project->RM.styles, res);
}

static void
_style_deleted(void *data,
               Evas_Object *obj __UNUSED__,
               void *ei)
{
   const char *name = (const char *)ei;
   Project *pro = (Project *)data;
   Style2 *res_style;

   res_style = (Style2 *)resource_manager_find(pro->RM.styles, name);
   _resource_style_del(pro, res_style);
}

static void
_style_changed(void *data,
               Evas_Object *obj __UNUSED__,
               void *ei)
{
   Font_Change *font_change = (Font_Change *)ei;
   Project *pro = (Project *)data;
   Style2 *res_style;
   Style_Tag2 *res_tag;
   Resource2 *old_source, *source;

   res_style = (Style2 *)resource_manager_find(pro->RM.styles, font_change->style_name);
   res_tag = (Style_Tag2 *)resource_manager_find(res_style->tags,
                                                 font_change->tag_name);
   if (res_tag->font)
     {
        old_source = resource_manager_find(pro->RM.fonts, res_tag->font);
        if (old_source)
          _resource_usage_resource_del((Resource2 *)res_tag, old_source);
        eina_stringshare_del(res_tag->font);
        res_tag->font = NULL;
     }
   if (font_change->value)
     {
        res_tag->font = eina_stringshare_add(font_change->value);
        source = resource_manager_find(pro->RM.fonts, res_tag->font);
        if (source)
          _resource_usage_resource_add((Resource2 *)res_tag, source);
     }
}

static void
_editor_part_added_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Eina_Stringshare *part_name = event_info;
   Project *pro = (Project *)data;

   Group2 *group = tabs_current_group_get();
   Part2 *part = _part_add(pro, group, part_name);
   _part_dependency_load(pro, group, part);
}

static void
_editor_part_deleted_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   const Editor_Part *editor_part = event_info;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_part->part_name);

   _resource_part_del(pro, group, part, editor_part->change);
}

static void
_editor_program_added_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Eina_Stringshare *program_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();
   Program2 *program = _program_load(group, program_name);

   _program_dependency_load(pro, group, program);
}

static void
_editor_program_deleted_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   const Editor_Program *editor_part = event_info;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();
   Program2 *program = (Program2 *)resource_manager_find(group->programs,
                                                         editor_part->program_name);

   _resource_program_del(pro, group, program, editor_part->change);
}

static void
_editor_group_data_added_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Eina_Stringshare *group_data_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();

   _group_data_add(pro, group, group_data_name);
}

static void
_editor_group_data_deleted_cb(void *data __UNUSED__,
                              Evas_Object *obj __UNUSED__,
                              void *event_info)
{
   Eina_Stringshare *group_data_name = event_info;
   Group2 *group = tabs_current_group_get();
   Group_Data2 *group_data = (Group_Data2 *)resource_manager_find(group->data_items, group_data_name);

   _resource_group_data_del(group, group_data);
}

static void
_editor_part_item_added_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   const Editor_Item *editor_item = event_info;
   Resource2 *used;
   Part_Item2 *item;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_item->part_name);

   /* we can use _item_dependency_load here, but let's avoid using edje edit */
   item = _part_item_add(part, editor_item->item_name, editor_item->item_index);
   used = resource_manager_find(pro->RM.groups, editor_item->source);
   if (used)
     _resource_usage_resource_add((Resource2 *)item, used);
}

static void
_editor_part_item_deleted_cb(void *data __UNUSED__,
                             Evas_Object *obj __UNUSED__,
                             void *event_info)
{
   const Editor_Item *editor_item = event_info;
   Group2 *group = tabs_current_group_get();
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_item->part_name);
   Part_Item2 *item = (Part_Item2 *)resource_manager_id_find(part->items, editor_item->item_index);

   _resource_part_item_del(part, item);
}

static void
_editor_state_added_cb(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   const Editor_State *editor_state = event_info;
   Project *pro = (Project *)data;
   Part2 *part;
   State2 *state;
   Group2 *group = tabs_current_group_get();

   part = (Part2 *)resource_manager_find(group->parts, editor_state->part_name);
   state = _state_add(pro, group, part, editor_state->state_name, editor_state->state_value);
   _state_dependency_load(pro, group, part, state);
}

static void
_editor_state_deleted_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   const Editor_State *editor_state = event_info;
   Project *pro = (Project *)data;
   Group2 *group = tabs_current_group_get();
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_state->part_name);
   State2 *state = (State2 *)resource_manager_v_find(part->states, editor_state->state_name, editor_state->state_value);

   _resource_state_del(pro, part, state, editor_state->change);
}

static void
_editor_part_restacked_cb(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   const Editor_Part_Restack *editor_part_restack = event_info;
   Group2 *group = tabs_current_group_get();
   Part2 *part, *rel_part = NULL;
   Eina_List *rel_l;

   part = (Part2 *)resource_manager_find(group->parts,
                                         editor_part_restack->part_name);
   if (editor_part_restack->relative_part_name)
     rel_part = (Part2 *)resource_manager_find(group->parts,
                                               editor_part_restack->relative_part_name);
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

static void
_editor_part_item_restacked_cb(void *data __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info)
{
   const Editor_Part_Item_Restack *editor_part_item_restack = event_info;
   Group2 *group = tabs_current_group_get();
   Part_Item2 *part_item, *relative_part_item;
   Part2 *part = (Part2 *)resource_manager_find(group->parts,
                                                editor_part_item_restack->part_name);

   part_item = (Part_Item2 *)resource_manager_id_find(part->items,
                                                      editor_part_item_restack->item_index);
   if (editor_part_item_restack->item_move_up)
     {
        relative_part_item = (Part_Item2 *)resource_manager_id_find(part->items,
                                                                    editor_part_item_restack->item_index - 1);
        part->items = eina_list_remove(part->items, part_item);
        part->items = eina_list_prepend_relative(part->items,
                                                 part_item,
                                                 relative_part_item);
        relative_part_item->common.id++;
        part_item->common.id--;
     }
   else
     {
        relative_part_item = (Part_Item2 *)resource_manager_id_find(part->items,
                                                                    editor_part_item_restack->item_index + 1);
        part->items = eina_list_remove(part->items, part_item);
        part->items = eina_list_append_relative(part->items,
                                                part_item,
                                                relative_part_item);
        relative_part_item->common.id--;
        part_item->common.id++;
     }
}

static void
_editor_group_add_cb(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Eina_Stringshare *group_name = (Eina_Stringshare *)event_info;
   Project *pro = (Project *)data;
   Group2 *group;

   group = _group_add(pro, group_name);
   _group_load(pro, group);
   _group_dependency_load(pro, group);

   /* to send info that group was succesfully added and UI need to update */
   evas_object_smart_callback_call(ap.win, SIGNAL_GROUP_ADDED, group);
}

static void
_editor_group_del_cb(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Eina_Stringshare *group_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = (Group2 *)resource_manager_find(pro->RM.groups, group_name);

   _resource_group_del(pro, group);
}

/* INITIAL FUNCTIONS */

void
_resource_callbacks_register(Project *project)
{
   TODO("Those signals and their edje_edit API need to be implemented through editor")
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_COLORCLASS_ADDED, _colorclass_added, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_COLORCLASS_DELETED, _colorclass_deleted, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_SOUND_ADDED, _sound_added, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_SOUND_DELETED, _sound_deleted, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_TONE_ADDED, _tone_added, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_TONE_DELETED, _tone_deleted, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_IMAGE_ADDED, _image_added, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_IMAGE_DELETED, image_deleted, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_STYLE_ADDED, _style_added, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_STYLE_DELETED, _style_deleted, project);
   evas_object_smart_callback_add(ap.win,  SIGNAL_EDITOR_STYLE_TAG_CHANGED, _style_changed, project);

   /* already implemented stack of editor changes */
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ADDED, _editor_part_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_DELETED, _editor_part_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_RESTACKED, _editor_part_restacked_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_ADDED, _editor_part_item_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_DELETED, _editor_part_item_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_RESTACKED, _editor_part_item_restacked_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_ADDED, _editor_state_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_DELETED, _editor_state_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_ADDED, _editor_program_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_DELETED, _editor_program_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_GROUP_DATA_ADDED, _editor_group_data_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_GROUP_DATA_DELETED, _editor_group_data_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, _property_attribute_changed, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, _property_resource_attribute_changed, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_GROUP_ADDED, _editor_group_add_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_GROUP_DELETED, _editor_group_del_cb, project);
}

void
_resource_callbacks_unregister(Project *project)
{
   TODO("Those signals and their edje_edit API need to be implemented through editor")
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_COLORCLASS_ADDED, _colorclass_added, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_COLORCLASS_DELETED, _colorclass_deleted, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_SOUND_ADDED, _sound_added, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_SOUND_DELETED, _sound_deleted, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_TONE_ADDED, _tone_added, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_TONE_DELETED, _tone_deleted, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_IMAGE_ADDED, _image_added, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_IMAGE_DELETED, image_deleted, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_STYLE_ADDED, _style_added, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_STYLE_DELETED, _style_deleted, project);
   evas_object_smart_callback_del_full(ap.win,  SIGNAL_EDITOR_STYLE_TAG_CHANGED, _style_changed, project);

   /* already implemented stack of editor changes */
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PART_ADDED, _editor_part_added_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PART_DELETED, _editor_part_deleted_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PART_RESTACKED, _editor_part_restacked_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PART_ITEM_ADDED, _editor_part_item_added_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PART_ITEM_DELETED, _editor_part_item_deleted_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PART_ITEM_RESTACKED, _editor_part_item_restacked_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_STATE_ADDED, _editor_state_added_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_STATE_DELETED, _editor_state_deleted_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PROGRAM_ADDED, _editor_program_added_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_PROGRAM_DELETED, _editor_program_deleted_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_GROUP_DATA_ADDED, _editor_group_data_added_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_GROUP_DATA_DELETED, _editor_group_data_deleted_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_RM_ATTRIBUTE_CHANGED, _property_attribute_changed, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_RM_RESOURCE_ATTRIBUTE_CHANGED, _property_resource_attribute_changed, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_GROUP_ADDED, _editor_group_add_cb, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_EDITOR_GROUP_DELETED, _editor_group_del_cb, project);
}
