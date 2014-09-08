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

#ifndef SOUND_EDITOR_DIALOG_H
#define SOUND_EDITOR_DIALOG_H

/**
 * @defgroup Sound_Editor Sound_Editor
 * @ingroup Dialogs
 *
 * Show sounds and tones, which are used in project.
 */

#include "eflete.h"
#include "modal_window.h"
#include "widget_macro.h"
#include "string_macro.h"

typedef enum {
   SOUND_EDITOR_SINGLE,
   SOUND_EDITOR_MULTIPLE
} Sound_Editor_Mode;


/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @param project currently opened project
 * @param mode its enumerate: SOUND_EDITOR_SINGLE if single-selection mode,
 *             SOUND_EDITOR_MULTIPLE if multi-selection mode.
 *
 * @return the inwin object that contain sound viewer
 *
 * @ingroup Sound_Editor
 */
Evas_Object *
sound_editor_window_add(Project *project, Sound_Editor_Mode mode);

#endif  /* SOUND_EDITOR_DIALOG_H */
