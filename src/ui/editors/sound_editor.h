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

#include "modal_window.h"

typedef enum {
   SOUND_EDITOR_SINGLE,
   SOUND_EDITOR_MULTIPLE
} Sound_Editor_Mode;

/**
 * Delete added sounds from list
 *
 * @param add_snd list with data
 *
 * @ingroup Sound_Editor
 */
void
sound_editor_added_sounds_free(Eina_List *add_snd);

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

/**
 * This function will select the sound item by it's name. It is very useful
 * with sound property (setting another sound).
 *
 * @param win Pointer to inwin object, which was created with
 * sound_editor_window_add function.
 * @param selected Name of selected sound. If selected param is NULL, this
 * function do nothing.
 *
 * @return EINA_TRUE if successful or EINA_FALSE otherwise.
 *
 * @ingroup Sound_Editor
 */
Eina_Bool
sound_editor_file_choose(Evas_Object *win, const char *selected);

#endif  /* SOUND_EDITOR_DIALOG_H */
