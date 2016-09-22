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
#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "eflete.h"
#include "project_manager2.h"
#include "main_window.h"

void
sound_player_sound_set(Sound_Data *sound);

void
sound_player_sound_unset(void);

Evas_Object *
sound_player_add(Evas_Object *parent);

#endif /* SOUND_PLAYER_H */
