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

#ifdef HAVE_AUDIO
TODO("Check pulse_audio on configure and add COREAUDIO support")
   #define HAVE_PULSE 1
   #include <Ecore_Audio.h>
#endif

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

typedef struct
{
   Eina_Stringshare *sound_name;
   Eina_Stringshare *src;
   Eina_Stringshare *format;
   Edje_Edit_Sound_Comp comp;
   double rate;
   int tone_frq;
} Item;

typedef struct {
   Evas_Object *markup;
   Evas_Object *gengrid;
   Sound_Type sound_type;
   Elm_Object_Item *tone;
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

#endif  /* SOUND_EDITOR_DIALOG_H */
