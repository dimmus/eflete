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
#include "project_manager.h"

#ifdef HAVE_AUDIO
TODO("Check pulse_audio on configure and add COREAUDIO support")
   #define HAVE_PULSE 1
   #include <Ecore_Audio.h>
#endif

typedef enum {
   SOUND_TYPE_SAMPLE,
   SOUND_TYPE_TONE
} Sound_Type;

struct _Sound_Data {
   Sound_Type type;
   Resource *resource; /* for SAMPLE it's External_Resource, TONE - Tone_Resource */
   Eina_Stringshare *name;
   Eina_Stringshare *type_label;
};
typedef struct _Sound_Data Sound_Data;

/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @return the inwin object that contain sound viewer
 *
 * @ingroup Sound_Editor
 */
Evas_Object *
sound_editor_window_add(void);

#endif  /* SOUND_EDITOR_DIALOG_H */
