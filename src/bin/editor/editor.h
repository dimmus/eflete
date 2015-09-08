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

#ifndef BANNED_EDJE_EDIT_API_H
#define BANNED_EDJE_EDIT_API_H

#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "eflete.h"
#include "change.h"

Eina_Bool
editor_group_min_w_set(Evas_Object *obj, Change *change, int new_val);

Eina_Bool
editor_group_min_h_set(Evas_Object *obj, Change *change, int new_val);

Eina_Bool
editor_group_max_w_set(Evas_Object *obj, Change *change, int new_val);

Eina_Bool
editor_group_max_h_set(Evas_Object *obj, Change *change, int new_val);
#endif /* BANNED_EDJE_EDIT_API_H */
