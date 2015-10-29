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

#include "default.h"

#define EDITOR_STATE_ARGS_PROTO const char *part_name, const char *state_name, double state_val
#define EDITOR_STATE_ARGS part_name, state_name, state_val

#define EDITOR_DOUBLE_DEFAULT_CHECK(FUNC, PROTO_ARGS, ARGS, DEF_VAL) \
Eina_Bool \
editor_##FUNC##_default_is(Evas_Object *edit_object, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   double val = edje_edit_##FUNC##_get(edit_object, ARGS); \
   return !(fabs(val - DEF_VAL) > DBL_EPSILON); \
}

#define EDITOR_DOUBLE_RESET(FUNC, PROTO_ARGS, ARGS, RESET_VAL) \
Eina_Bool \
editor_##FUNC##_reset(Evas_Object *edit_object, Change *change, PROTO_ARGS) \
{ \
   assert(edit_object != NULL); \
   if (!editor_##FUNC##_default_is(edit_object, ARGS)) return true; \
   return editor_##FUNC##_set(edit_object, change, false, ARGS, RESET_VAL); \
}

#define EDITOR_STATE_DOUBLE_RESET(FUNC, DEF_VAL, RESET_VAL) \
EDITOR_DOUBLE_DEFAULT_CHECK(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, DEF_VAL) \
EDITOR_DOUBLE_RESET(state_##FUNC, EDITOR_STATE_ARGS_PROTO, EDITOR_STATE_ARGS, RESET_VAL)

EDITOR_STATE_DOUBLE_RESET(rel1_relative_x, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(rel2_relative_x, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(rel1_relative_y, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(rel2_relative_y, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(align_x, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(align_y, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(aspect_min, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(aspect_max, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_origin_relative_x, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_origin_relative_y, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(fill_size_relative_x, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(fill_size_relative_y, 1.0, 1.0)
EDITOR_STATE_DOUBLE_RESET(container_align_x, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(container_align_y, 0.5, 0.5)
EDITOR_STATE_DOUBLE_RESET(minmul_h, 0.0, 0.0)
EDITOR_STATE_DOUBLE_RESET(minmul_w, 0.0, 0.0)

