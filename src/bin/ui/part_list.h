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

#ifndef PART_LIST_H
#define PART_LIST_H

#include "eflete.h"
#include "group_manager.h"

#define SIGNAL_PART_LIST_PART_SELECTED "SIGNAL_PART_LIST_PART_SELECTED"
#define SIGNAL_PART_LIST_PART_STATE_SELECTED "SIGNAL_PART_LIST_PART_STATE_SELECTED"

Evas_Object *
part_list_add(Group *group);

void
part_list_part_select(Evas_Object *obj, Part_ *part);
#endif /* PART_LIST_H */
