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

#ifndef EDITOR_H
#define EDITOR_H

#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "eflete.h"
#include "change.h"
#include "signals.h"

typedef enum {
   ATTRIBUTE_GROUP_MIN_W,
   ATTRIBUTE_GROUP_MIN_H,
   ATTRIBUTE_GROUP_MAX_W,
   ATTRIBUTE_GROUP_MAX_H,
   ATTRIBUTE_STATE_MAX_W,
   ATTRIBUTE_STATE_MAX_H,
   ATTRIBUTE_STATE_ALIGN_X,
   ATTRIBUTE_STATE_ALIGN_Y
} Attribute;

static inline void
_editor_project_changed()
{
   ap.project->changed = true;
   evas_object_smart_callback_call(ap.win, SIGNAL_PROJECT_CHANGED, NULL);
}

/* General */
Eina_Bool
editor_save(Evas_Object *edit_object);

Eina_Bool
editor_save_all(Evas_Object *edit_object);

Eina_Bool
editor_internal_group_add(Evas_Object *edit_object);

/* Group */
Eina_Bool
editor_group_min_w_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

Eina_Bool
editor_group_min_h_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

Eina_Bool
editor_group_max_w_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);

Eina_Bool
editor_group_max_h_set(Evas_Object *obj, Change *change, Eina_Bool merge,
                       int new_val);
#endif /* EDITOR_H */
