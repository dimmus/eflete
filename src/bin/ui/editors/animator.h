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

#ifndef PROGRAM_EDITOR_H
#define PROGRAM_EDITOR_H

#define TRANSITIONS_COUNT 11
#define ACTIONS_COUNT 11
#define TARGET_NAME_KEY "target.name.key"
#define TARGET_ITEM_KEY "target.item.key"
#define AFTER_NAME_KEY "after.name.key"
#define AFTER_ITEM_KEY "after.item.key"
#define DEL_BUTTON_KEY "del.button.key"

/**
 * @defgroup ProgramEditor Program Editor
 *
 * Program editor is subwindow that contain information about
 * all the programs used by the project.
 */

#include "eflete.h"

/**
 * Add new program editor inwin object.
 *
 * @param project A project that was early loaded.
 *
 * @return Pointer to inwin object, which contain some program properties and
 * list of programs.
 *
 * @ingroup ProgramEditor
 */
Evas_Object *
animator_window_add(Project *project);

#endif /* PROGRAM_EDITOR_H */
