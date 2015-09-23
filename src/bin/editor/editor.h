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
   ATTRIBUTE_STATE_MIN_W,
   ATTRIBUTE_STATE_MIN_H,
   ATTRIBUTE_STATE_MAX_W,
   ATTRIBUTE_STATE_MAX_H,
   ATTRIBUTE_STATE_ALIGN_X,
   ATTRIBUTE_STATE_ALIGN_Y,
   ATTRIBUTE_STATE_REL1_RELATIVE_X,
   ATTRIBUTE_STATE_REL1_RELATIVE_Y,
   ATTRIBUTE_STATE_REL2_RELATIVE_X,
   ATTRIBUTE_STATE_REL2_RELATIVE_Y,
   ATTRIBUTE_STATE_REL1_OFFSET_X,
   ATTRIBUTE_STATE_REL1_OFFSET_Y,
   ATTRIBUTE_STATE_REL2_OFFSET_X,
   ATTRIBUTE_STATE_REL2_OFFSET_Y,
   ATTRIBUTE_STATE_ASPECT_MIN,
   ATTRIBUTE_STATE_ASPECT_MAX,
   ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_X,
   ATTRIBUTE_STATE_FILL_ORIGIN_RELATIVE_Y,
   ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_X,
   ATTRIBUTE_STATE_FILL_ORIGIN_OFFSET_Y,
   ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_X,
   ATTRIBUTE_STATE_FILL_SIZE_RELATIVE_Y,
   ATTRIBUTE_STATE_FILL_SIZE_OFFSET_X,
   ATTRIBUTE_STATE_FILL_SIZE_OFFSET_Y,
   ATTRIBUTE_STATE_TEXT_ALIGN_X,
   ATTRIBUTE_STATE_TEXT_ALIGN_Y,
   ATTRIBUTE_STATE_TEXT_ELIPSIS,
   ATTRIBUTE_STATE_TEXT_SIZE,
   ATTRIBUTE_STATE_TEXT_FIT_X,
   ATTRIBUTE_STATE_TEXT_FIT_Y,
   ATTRIBUTE_STATE_TEXT_MAX_X,
   ATTRIBUTE_STATE_TEXT_MAX_Y,
   ATTRIBUTE_STATE_TEXT_MIN_X,
   ATTRIBUTE_STATE_TEXT_MIN_Y,
   ATTRIBUTE_STATE_FIXED_H,
   ATTRIBUTE_STATE_FIXED_W,
   ATTRIBUTE_STATE_FILL_SMOOTH,
   ATTRIBUTE_STATE_VISIBLE,
   ATTRIBUTE_STATE_IMAGE,
   ATTRIBUTE_STATE_COLOR_CLASS,
   ATTRIBUTE_STATE_REL1_TO_X,
   ATTRIBUTE_STATE_REL1_TO_Y,
   ATTRIBUTE_STATE_REL2_TO_X,
   ATTRIBUTE_STATE_REL2_TO_Y,
   ATTRIBUTE_STATE_TEXT_SOURCE,
   ATTRIBUTE_STATE_TEXT,
   ATTRIBUTE_STATE_FONT,
   ATTRIBUTE_STATE_TEXT_STYLE,
   ATTRIBUTE_STATE_COLOR,
   ATTRIBUTE_STATE_COLOR2,
   ATTRIBUTE_STATE_COLOR3,
   ATTRIBUTE_STATE_IMAGE_BORDER,
   ATTRIBUTE_STATE_IMAGE_BORDER_FILL,
   ATTRIBUTE_STATE_ASPECT_PREF,
   ATTRIBUTE_PART_EFFECT,
   ATTRIBUTE_PART_IGNORE_FLAGS,
   ATTRIBUTE_PART_MOUSE_EVENTS,
   ATTRIBUTE_PART_REPEAT_EVENTS,
   ATTRIBUTE_PART_SCALE,
   ATTRIBUTE_PART_DRAG_COUNT_X,
   ATTRIBUTE_PART_DRAG_COUNT_Y,
   ATTRIBUTE_PART_DRAG_X,
   ATTRIBUTE_PART_DRAG_Y,
   ATTRIBUTE_PART_DRAG_STEP_X,
   ATTRIBUTE_PART_DRAG_STEP_Y,
   ATTRIBUTE_PART_CLIP_TO,
   ATTRIBUTE_PART_DRAG_CONFINE,
   ATTRIBUTE_PART_DRAG_THRESHOLD,
   ATTRIBUTE_PART_DRAG_EVENT,
   ATTRIBUTE_PART_NAME,
   ATTRIBUTE_PART_SOURCE,
   ATTRIBUTE_GROUP_NAME,
   ATTRIBUTE_PART_ITEM_ASPECT_MODE,
   ATTRIBUTE_PART_ITEM_ALIGN_X,
   ATTRIBUTE_PART_ITEM_ALIGN_Y,
   ATTRIBUTE_PART_ITEM_WEIGHT_X,
   ATTRIBUTE_PART_ITEM_WEIGHT_Y,
   ATTRIBUTE_PART_ITEM_ASPECT_H,
   ATTRIBUTE_PART_ITEM_ASPECT_W,
   ATTRIBUTE_PART_ITEM_MAX_H,
   ATTRIBUTE_PART_ITEM_MAX_W,
   ATTRIBUTE_PART_ITEM_MIN_H,
   ATTRIBUTE_PART_ITEM_MIN_W,
   ATTRIBUTE_PART_ITEM_PREFER_H,
   ATTRIBUTE_PART_ITEM_PREFER_W,
   ATTRIBUTE_PART_ITEM_SPREAD_H,
   ATTRIBUTE_PART_ITEM_SPREAD_W,
   ATTRIBUTE_PART_ITEM_SPAN_COL,
   ATTRIBUTE_PART_ITEM_SPAN_ROW,
   ATTRIBUTE_PART_ITEM_POSITION_COL,
   ATTRIBUTE_PART_ITEM_POSITION_ROW,
   ATTRIBUTE_PART_ITEM_SOURCE,
   ATTRIBUTE_PART_ITEM_PADDING,
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

Eina_Bool
editor_group_add(Evas_Object *obj, const char *name);

Eina_Bool
editor_group_copy(Evas_Object *obj, const char *group_src, const char *group_dest);

Eina_Bool
editor_group_alias_add(Evas_Object *obj, const char *group_src, const char *group_alias);

Eina_Bool
editor_group_del(Evas_Object *obj, const char *name);

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
