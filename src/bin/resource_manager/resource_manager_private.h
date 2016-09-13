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

#ifndef RESOURCE_MANAGER_PRIVATE_H
#define RESOURCE_MANAGER_PRIVATE_H

#include "eflete.h"

/* INTERNAL API TO WORK WITH USAGE */

Eina_Bool
_resource_usage_resource_del(Resource2 *origin, Resource2 *used);

Eina_Bool
_resource_usage_resource_add(Resource2 *origin, Resource2 *used);

Eina_Bool
_resource_usage_dependency_cleanup(Resource2 *origin);

/* RESOURCE LOADING */

Eina_Bool
_image_resources_load(Project *project);

Eina_Bool
_image_set_resources_load(Project *project);

Eina_Bool
_sound_resources_load(Project *project);

Eina_Bool
_font_resources_load(Project *project);

Eina_Bool
_tones_resources_load(Project *project);

Eina_Bool
_colorclasses_resources_load(Project *project);

Eina_Bool
_styles_tag_resources_load(Project *pro, Eina_Stringshare *name, Style2 *style);

Eina_Bool
_styles_resources_load(Project *project);

Eina_Bool
_global_data_resources_load(Project *project);

State2 *
_gm_state_add(Project *pro,
              Group2 *group,
              Part2 *part,
              const char *state_name,
              double state_value);

Part_Item2 *
_gm_part_item_add(Part2 *part,
                  Eina_Stringshare *item_name,
                  unsigned int i);

Part2 *
_gm_part_add(Project *project, Group2 *group, const char *part_name);

Group_Data2 *
_gm_group_data_add(Project *pro,
                   Group2 *group,
                   Eina_Stringshare *group_data_name);

Program2 *
_program_load(Group2 *group, Eina_Stringshare *program_name);

Group2 *
_gm_group_add(Project *pro, Eina_Stringshare *group_name);

void
_group_load(Project *pro, Group2 *group);

void
_gm_groups_load(Project *project);

void
_resource_group_edit_object_load(Project *pro, Group2 *group, Evas *e);

void
_resource_group_edit_object_unload(Group2 *group);

/* DEPENDENCY BUILD */

void
_item_dependency_load(Project *pro, Group2 *group, Part2 *part, Part_Item2 *item);

void
_state_dependency_load(Project *pro, Group2 *group, Part2 *part, State2 *state);

void
_part_dependency_load(Project *pro, Group2 *group, Part2 *part);

void
_program_dependency_load(Project *pro, Group2 *group, Program2 *program);

void
_group_dependency_load(Project *pro, Group2 *group);

void
_resource_dependency_load(Project *pro);

void
_style_dependency_load(Project *pro);

/* REACTION TO EDITOR AND EDJE EDIT */

void
_resource_callbacks_register(Project *project);

void
_resource_callbacks_unregister(Project *project);

/* RESOURCES DELETION AND FREE */

void
_resource_state_free(Part2 *part, State2 *state);

void
_resource_state_del(Part2 *part, State2 *state, Change *change);

void
_resource_part_item_free(Part2 *part, Part_Item2 *item);

void
_resource_part_item_del(Part2 *part, Part_Item2 *item);

void
_resource_part_free(Group2 *group, Part2 *part);

void
_resource_part_del(Group2 *group, Part2 *part, Change *change);

void
_resource_program_free(Group2 *group, Program2 *program);

void
_resource_program_del(Group2 *group, Program2 *program, Change *change);

void
_resource_group_data_free(Group2 *group, Group_Data2 *data);

void
_resource_group_data_del(Group2 *group, Group_Data2 *data);

void
_resource_group_free(Project *pro, Group2 *group);

void
_resource_group_del(Project *pro, Group2 *group);

/* TOP LEVEL RESOURCES DELETION AND FREE */

void
_resource_image_free(Project *pro, Image2 *res);

void
_resource_image_del(Project *pro, Image2 *res_image);

void
_resource_tone_free(Project *pro, Tone2 *res);

void
_resource_tone_del(Project *pro, Tone2 *res_tone);

void
_resource_sound_free(Project *pro, Sound2 *res);

void
_resource_sound_del(Project *pro, Sound2 *res_sound);

void
_resource_colorclass_free(Project *pro, Colorclass2 *res);

void
_resource_colorclass_del(Project *pro, Colorclass2 *res_colorclass);

void
_resource_style_tag_free(Style_Tag2 *res);

void
_resource_style_free(Project *pro, Style2 *res);

void
_resource_style_del(Project *pro, Style2 *res_style);

/***** NOT IMPLEMENTED YET BUT NEED FREEING ***********/

void
_resource_image_set_free(Project *pro, Image_Set2 *res);

void
_resource_font_free(Project *pro, Font2 *res);

void
_resource_data_free(Project *pro, Global_Data2 *res);

#endif /* RESOURCE_MANAGER_PRIVATE_H */
