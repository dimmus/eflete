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

void
_resource_free(Resource2 *res)
{
   eina_stringshare_del(res->common.name);
   eina_list_free(res->common.used_in);
   eina_list_free(res->common.uses___);
   free(res);
}

void
_resource_part_free(Part2 *res)
{
   Resource2 *part_res;
   State2 *state;
   EINA_LIST_FREE(res->states, state)
     {
        eina_stringshare_del(state->normal);
        eina_list_free(state->tweens);
        _resource_free((Resource2 *)state);
     }
   EINA_LIST_FREE(res->items, part_res)
      _resource_free(part_res);
   _resource_free((Resource2 *)res);
}

void
_resource_group_free(Group2 *res)
{
   Part2 *part;
   Program2 *program;
   Group_Data2 *group_data;

   EINA_LIST_FREE(res->parts, part)
      _resource_part_free(part);

   EINA_LIST_FREE(res->programs, program)
     {
        eina_list_free(program->targets);
        eina_list_free(program->afters);
        _resource_free((Resource2 *)program);
     }
   EINA_LIST_FREE(res->data_items, group_data)
     {
        eina_stringshare_del(group_data->source);
        _resource_free((Resource2 *)group_data);
     }
   _resource_free((Resource2 *)res);
}

Eina_Bool
resource_manager_shutdown(Project *pro)
{
   Resource2 *res;
   Image2 *res_image;
   Sound2 *res_sound;
   Font2 *res_font;
   Global_Data2 *res_data;

   Group2 *group;

   /* image_set */
   EINA_LIST_FREE(pro->image_sets, res)
     _resource_free(res);
   EINA_LIST_FREE(pro->tones, res)
     _resource_free(res);
   EINA_LIST_FREE(pro->colorclasses, res)
     _resource_free(res);
   EINA_LIST_FREE(pro->styles, res)
     _resource_free(res);

   EINA_LIST_FREE(pro->images, res_image)
     {
        eina_stringshare_del(res_image->source);
        _resource_free((Resource2 *)res_image);
     }
   EINA_LIST_FREE(pro->sounds, res_sound)
     {
        eina_stringshare_del(res_sound->source);
        _resource_free((Resource2 *)res_sound);
     }
   EINA_LIST_FREE(pro->fonts, res_font)
     {
        eina_stringshare_del(res_font->source);
        _resource_free((Resource2 *)res_font);
     }
   EINA_LIST_FREE(pro->global_data, res_data)
     {
        eina_stringshare_del(res_data->source);
        _resource_free((Resource2 *)res_data);
     }

   EINA_LIST_FREE(pro->groups2, group)
      _resource_group_free(group);

   _resource_callbacks_unregister(pro);

   return true;
}
