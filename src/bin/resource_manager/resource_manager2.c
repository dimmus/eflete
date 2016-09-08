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

Resource2 *
resource_manager_id_find(const Eina_List *list, const unsigned int id)
{
   Resource2 *res = NULL, *data;
   const Eina_List *l;
   EINA_LIST_FOREACH(list, l, data)
     {
        if (data->common.id == id)
          {
             res = data;
             break;
          }
     }

   return res;
}

Resource2 *
resource_manager_v_find(const Eina_List *list, Eina_Stringshare *name, double value)
{
   State2 *res = NULL, *data;
   const Eina_List *l;
   EINA_LIST_FOREACH(list, l, data)
     {
        if ((data->common.name == name) && (data->val == value))
          {
             res = data;
             break;
          }
     }

   return (Resource2 *)res;
}

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

   _resource_callbacks_register(project);

   return false;
}

Eina_Bool
resource_manager_shutdown(Project *pro)
{
   Image_Set2 *res_image_set;
   Image2 *res_image;
   Sound2 *res_sound;
   Font2 *res_font;
   Tone2 *res_tone;
   Global_Data2 *res_data;
   Style2 *res_style;
   Colorclass2 *res_colorclass;

   State2 *state;
   Part_Item2 *item;
   Part2 *part;
   Program2 *program;
   Group_Data2 *data;

   Group2 *group;

   /* image_set */
   EINA_LIST_FREE(pro->RM.image_sets, res_image_set)
     _resource_image_set_free(res_image_set);
   EINA_LIST_FREE(pro->RM.images, res_image)
     _resource_image_free(res_image);

   EINA_LIST_FREE(pro->RM.tones, res_tone)
     _resource_tone_free(res_tone);
   EINA_LIST_FREE(pro->RM.sounds, res_sound)
     _resource_sound_free(res_sound);

   EINA_LIST_FREE(pro->RM.colorclasses, res_colorclass)
     _resource_colorclasses_free(res_colorclass);
   EINA_LIST_FREE(pro->RM.styles, res_style)
     _resource_styles_free(res_style);

   EINA_LIST_FREE(pro->RM.fonts, res_font)
      _resource_font_free(res_font);
   EINA_LIST_FREE(pro->RM.global_data, res_data)
      _resource_data_free(res_data);

   EINA_LIST_FREE(pro->RM.groups, group)
     {
        /* free parts */
        EINA_LIST_FREE(group->parts, part)
          {
             /* free states */
             EINA_LIST_FREE(part->states, state)
               {
                  _resource_state_free(part, state);
               }
             /* free part items */
             EINA_LIST_FREE(part->items, item)
               {
                  _resource_part_item_free(part, item);
               }
             _resource_part_free(group, part);
          }
        /* free programs */
        EINA_LIST_FREE(group->programs, program)
          {
             _resource_program_free(group, program);
          }
        /* free group data*/
        EINA_LIST_FREE(group->data_items, data)
          {
             _resource_group_data_free(group, data);
          }
        _resource_group_free(pro, group);
     }

   _resource_callbacks_unregister(pro);

   return true;
}
