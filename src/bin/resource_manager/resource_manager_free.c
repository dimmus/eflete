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
   Part_Item2 *part_res;
   State2 *state;
   EINA_LIST_FREE(res->states, state)
     {
        eina_stringshare_del(state->normal);
        eina_list_free(state->tweens);
        _resource_free((Resource2 *)state);
     }
   EINA_LIST_FREE(res->items, part_res)
     {
        if (part_res->source)
          eina_stringshare_del(part_res->source);
        _resource_free((Resource2 *)part_res);
     }
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
