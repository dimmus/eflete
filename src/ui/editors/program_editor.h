/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#ifndef PROGRAM_EDITOR_H
#define PROGRAM_EDITOR_H

#define TRANSITIONS_COUNT 14
#define ACTIONS_COUNT 11

/**
 * @defgroup ProgramEditor Program Editor
 *
 * Program editor is subwindow that contain information about
 * all the programs used by the project.
 */

#include "eflete.h"
#include "modal_window.h"
#include "widget_macro.h"
#include "string_macro.h"

/**
 * Add new program editor inwin object.
 *
 * @param style A @Style structure data.
 *
 * @return Pointer to inwin object, which contain some program properties and
 * list of programs.
 *
 * @ingroup ProgramEditor
 */
Evas_Object *
program_editor_window_add(Style *style);

#endif /* PROGRAM_EDITOR_H */
