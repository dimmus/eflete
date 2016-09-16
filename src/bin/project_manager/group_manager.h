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

#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H

#include "eflete.h"

void
gm_group_edit_object_load(Project *pro, Group *group, Evas *e);

void
gm_group_edit_object_reload(Project *pro, Group *group);

void
gm_group_edit_object_unload(Group *group);

Eina_List *
gm_group_used_groups_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_images_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_sample_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_color_classes_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_styles_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_fonts_edj_get(Evas_Object *obj);

#endif /* GROUP_MANAGER_H */
