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

#define SIG_SOUND_SELECTED "sound_selected"

typedef enum {
   SOUND_EDITOR_EDIT,
   SOUND_EDITOR_SAMPLE_SELECT,
   SOUND_EDITOR_TONE_SELECT
} Sound_Editor_Mode;

typedef enum {
   SOUND_TYPE_SAMPLE,
   SOUND_TYPE_TONE
} Sound_Type;

typedef struct {
   Sound_Type sound_type;
   Eina_Stringshare *file_name;
   double duration;
   Eina_Stringshare *snd_src;
   Eina_Stringshare *format;
   int length;
   Edje_Edit_Sound_Comp compression_type;
   double quality;
   int tone_frq;
} Selected_Sound_Data;

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
 * @param mode one of the following values:
 * @li SOUND_EDITOR_EDIT editor mode;
 * @li SOUND_EDITOR_SAMPLE_SELECT sample selector mode;
 * @li SOUND_EDITOR_TONE_SELECT tone selector mode.
 *
 * @return the inwin object that contain sound viewer
 *
 * @ingroup Sound_Editor
 */
Evas_Object *
sound_editor_window_add(Sound_Editor_Mode mode);

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
