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

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "eflete.h"

typedef struct _Resource_Name_Validator Resource_Name_Validator;

void
resource_name_validator_list_set(Resource_Name_Validator *validator, Eina_List **list, Eina_Bool sorted);
void
resource_name_validator_resource_set(Resource_Name_Validator *validator, Resource *resource);
Resource_Name_Validator *
resource_name_validator_new(const char *pattern, const char *sig);
void
resource_name_validator_free(Resource_Name_Validator *validator);
Elm_Regexp_Status
resource_name_validator_status_get(Resource_Name_Validator *validator);
void
resource_name_validator_helper(void *data, const Eo_Event *event);

#endif /* VALIDATOR_H */
