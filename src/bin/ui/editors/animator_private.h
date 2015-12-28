/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#define PLAY_CB "play_cb"
#define PAUSE_CB "pause_cb"
#define NAME_CHANGED_CB "name_changed_cb"
#define SCROLL_CB "scroll_cb"

#include "eflete.h"

/* program sequence */
Evas_Object *prog_sequence_add(Evas_Object *parent, Group *group, Evas_Object *live);

Eina_Bool prog_sequence_program_set(Evas_Object *obj, const char* program_name, Eina_List *program_list);

void prog_sequence_program_reset(Evas_Object *obj);

void prog_sequence_program_play(Evas_Object *obj);

void prog_sequence_program_stop(Evas_Object *obj);

void prog_sequence_cycled_set(Evas_Object *obj, Eina_Bool cycled);

/* program editor */
Evas_Object * program_editor_add(Evas_Object *parent, Group *group, Evas_Object *live);

Eina_Bool prog_editor_program_set(Evas_Object *obj, const char* program_name);

void program_editor_program_reset(Evas_Object *obj);

void program_editor_program_play(Evas_Object *obj);

void program_editor_program_stop(Evas_Object *obj);

void program_editor_cycled_set(Evas_Object *obj, Eina_Bool cycled);

void program_editor_free(Evas_Object * obj);
