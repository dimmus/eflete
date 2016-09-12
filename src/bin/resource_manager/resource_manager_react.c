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
#include "tabs.h"
#include "string_common.h"

/* TEMPORARY FUNCTION WHICH SHOULD BE DELETED AFTER RESOURCE_MANAGER2 IMPLEMENTED */
Group2 *
_get_current_group2(Project *pro)
{
   /*******************************************************/
   /******** THIS BEHAVIOUR SHOULD BE CHANGED *************/
   /*******************************************************/
   /******* BECAUSE GROUP2 DIFFERENT FROM GROUP ***********/
   /******* AND EFLETE DOESN'T WORK WITH IT YET ***********/
   /*******************************************************/
   Group *group = tabs_current_group_get();
   return (Group2 *)resource_manager_find(pro->RM.groups, group->name);
   /*******************************************************/
   /*******************************************************/
   /*******************************************************/
   /*******************************************************/
   /*******************************************************/
   /*******************************************************/
}

/* CALLBACK FUNCTIONS */

static void
_property_resource_attribute_changed(void *data __UNUSED__,
                                     Evas_Object *obj __UNUSED__,
                                     void *event_info)
{
   Attribute *attr = event_info;
   Attribute_Resource editor_resource = (int)*attr;
   printf("Some resource attribute was changed [%d][%d] \n", (int)*attr, editor_resource);
   switch ((int)*attr)
     {
      case RM_ATTRIBUTE_RESOURCES_COLORCLASS_DESCRIPTION:
      case RM_ATTRIBUTE_RESOURCES_COLORCLASS_COLORS:
      case RM_ATTRIBUTE_RESOURCES_STYLE_TAG_ADDED:
      case RM_ATTRIBUTE_RESOURCES_STYLE_TAG_DELETED:
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
    ** > expand editor (top blocks like image, sound, etc) not supported **
    ***********************************************************************
    ***********************************************************************/
   Resource2 *part, *source, *old_source;

   Editor_Attribute_Change *change = (Editor_Attribute_Change *)event_info;
   Attribute editor_resource = (int)change->attribute;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);

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
      case RM_ATTRIBUTE_STATE_IMAGE:
      case RM_ATTRIBUTE_STATE_IMAGE_TWEEN:
      case RM_ATTRIBUTE_STATE_REL1_TO_X:
      case RM_ATTRIBUTE_STATE_REL1_TO_Y:
      case RM_ATTRIBUTE_STATE_REL2_TO_X:
      case RM_ATTRIBUTE_STATE_REL2_TO_Y:
      case RM_ATTRIBUTE_STATE_TEXT_SOURCE:
      case RM_ATTRIBUTE_STATE_TEXT_TEXT_SOURCE:
      case RM_ATTRIBUTE_STATE_TEXT:
      case RM_ATTRIBUTE_STATE_FONT:
      case RM_ATTRIBUTE_STATE_TEXT_STYLE:
      case RM_ATTRIBUTE_STATE_ASPECT_PREF:
      case RM_ATTRIBUTE_PART_TEXT_EFFECT:
      case RM_ATTRIBUTE_PART_TEXT_SHADOW_DIRECTION:
      case RM_ATTRIBUTE_PART_CLIP_TO:
      case RM_ATTRIBUTE_PART_DRAG_CONFINE:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_SELECTION_UNDER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_SELECTION_OVER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_CURSOR_UNDER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_CURSOR_OVER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_UNDER:
      case RM_ATTRIBUTE_PART_TEXTBLOCK_ANCHORS_OVER:
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
      case RM_ATTRIBUTE_STATE_COLOR_CLASS:
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
      case RM_ATTRIBUTE_PART_ITEM_SOURCE:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_RIGHT:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_LEFT:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_TOP:
      case RM_ATTRIBUTE_PART_ITEM_PADDING_BOTTOM:
         break;
      case RM_ATTRIBUTE_PART_GROUP_SOURCE:
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
   res->common.name = eina_stringshare_add(name);
   res->comp_type = edje_edit_image_compression_type_get(project->global_object,
                                                         res->common.name);
   if (res->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
     res->source = eina_stringshare_add(name);
   else
     res->source = eina_stringshare_printf("%s/%s", resource_folder, name);

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
_part_renamed(void *data,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Rename *ren = ei;
   Part2 *current_part;
   Project *pro = (Project *)data;

   Group2 *group = _get_current_group2(pro);

   current_part = (Part2 *)resource_manager_find(group->parts, ren->old_name);
   eina_stringshare_del(current_part->common.name);
   current_part->common.name = eina_stringshare_add(ren->new_name);
}

static void
_group_data_renamed(void *data,
              Evas_Object *obj __UNUSED__,
              void *ei)
{
   Rename *ren = ei;
   Group_Data2 *group_data;
   Project *pro = (Project *)data;

   Group2 *group = _get_current_group2(pro);
   group_data = (Group_Data2 *)resource_manager_find(group->data_items, ren->old_name);

   eina_stringshare_del(group_data->common.name);
   group_data->common.name = eina_stringshare_add(ren->new_name);
}
static void
_editor_part_added_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Eina_Stringshare *part_name = event_info;
   Project *pro = (Project *)data;

   Group2 *group = _get_current_group2(pro);
   Part2 *part = _gm_part_add(pro, group, part_name);
   _part_dependency_load(pro, group, part);
}

static void
_editor_part_deleted_cb(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   const Editor_Part *editor_part = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_part->part_name);

   _resource_part_del(group, part, editor_part->change);
}

static void
_editor_program_added_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Eina_Stringshare *program_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
   Program2 *program = _program_load(group, program_name);

   _program_dependency_load(pro, group, program);
}

static void
_editor_program_deleted_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Eina_Stringshare *program_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
   Program2 *program = (Program2 *)resource_manager_find(group->programs, program_name);

   _resource_program_del(group, program);
}

static void
_editor_group_data_added_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Eina_Stringshare *group_data_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);

   _gm_group_data_add(pro, group, group_data_name);
}

static void
_editor_group_data_deleted_cb(void *data,
                              Evas_Object *obj __UNUSED__,
                              void *event_info)
{
   Eina_Stringshare *group_data_name = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
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
   Group2 *group = _get_current_group2(pro);
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_item->part_name);
   unsigned int count = eina_list_count(part->items);

   /* we can use _item_dependency_load here, but let's avoid using edje edit */
   item = _gm_part_item_add(part, editor_item->item_name, count);
   used = resource_manager_find(pro->RM.groups, editor_item->source);
   if (used)
     _resource_usage_resource_add((Resource2 *)item, used);
}

static void
_editor_part_item_deleted_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info)
{
   const Editor_Item *editor_item = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_item->part_name);
   Part_Item2 *item = (Part_Item2 *)resource_manager_find(part->items, editor_item->item_name);

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
   Group2 *group = _get_current_group2(pro);

   part = (Part2 *)resource_manager_find(group->parts, editor_state->part_name);
   state = _gm_state_add(pro, group, part, editor_state->state_name, editor_state->state_value);
   _state_dependency_load(pro, group, part, state);
}

static void
_editor_state_deleted_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   const Editor_State *editor_state = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
   Part2 *part = (Part2 *)resource_manager_find(group->parts, editor_state->part_name);
   State2 *state = (State2 *)resource_manager_v_find(part->states, editor_state->state_name, editor_state->state_value);

   _resource_state_del(part, state, editor_state->change);
}

static void
_editor_part_restacked_cb(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   const Editor_Part_Restack *editor_part_restack = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
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
_editor_part_item_restacked_cb(void *data,
                               Evas_Object *obj __UNUSED__,
                               void *event_info)
{
   const Editor_Part_Item_Restack *editor_part_item_restack = event_info;
   Project *pro = (Project *)data;
   Group2 *group = _get_current_group2(pro);
   Part_Item2 *part_item, *relative_part_item;
   Part2 *part = (Part2 *)resource_manager_find(group->parts,
                                                editor_part_item_restack->part_name);
   part_item = (Part_Item2 *)resource_manager_find(part->items,
                                                   editor_part_item_restack->part_item);
   relative_part_item = (Part_Item2 *)resource_manager_find(part->items,
                                                            editor_part_item_restack->relative_part_item);

   part->items = eina_list_remove(part->items, part_item);

   if (relative_part_item)
     part->items = eina_list_prepend_relative(part->items,
                                              part_item,
                                              relative_part_item);
   else
     part->items = eina_list_append(part->items, part_item);
}

static void
_editor_group_add_cb(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Eina_Stringshare *group_name = (Eina_Stringshare *)event_info;
   Project *pro = (Project *)data;
   Group2 *group;

   group = _gm_group_add(pro, group_name);
   _group_load(pro, group);
   _group_dependency_load(pro, group);
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

   /* already implemented stack of editor changes */
   evas_object_smart_callback_add(ap.win, SIGNAL_PART_RENAMED, _part_renamed, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_DATA_RENAMED, _group_data_renamed, project);
   TODO("PART COPY - check if it is working after integration")
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ADDED, _editor_part_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_DELETED, _editor_part_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_RESTACKED, _editor_part_restacked_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_ADDED, _editor_part_item_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_DELETED, _editor_part_item_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PART_ITEM_RESTACKED, _editor_part_item_restacked_cb, project);
   TODO("STATE COPY - check if it is working after integration")
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_ADDED, _editor_state_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_STATE_DELETED, _editor_state_deleted_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_ADDED, _editor_program_added_cb, project);
   evas_object_smart_callback_add(ap.win, SIGNAL_EDITOR_PROGRAM_DELETED, _editor_program_deleted_cb, project);
   TODO("add afters and targets addition and changes")
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

   /* already implemented stack of editor changes */
   evas_object_smart_callback_del_full(ap.win, SIGNAL_PART_RENAMED, _part_renamed, project);
   evas_object_smart_callback_del_full(ap.win, SIGNAL_GROUP_DATA_RENAMED, _group_data_renamed, project);
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
