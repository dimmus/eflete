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

#ifndef SHORTCUTS_H
#define SHORTCUTS_H

/**
 * @defgroup Shortcuts Shortcuts
 * @ingroup Window
 *
 * Shortcuts is a module for keyboard shortcuts of different modules and main
 * window. Currently shortcuts are available for different windows such as main
 * window, opened project (groupedit/workspace) and editors (sound, image,
 * colorclasses, styles and programs).
 *
 * How to use it.
 * 1.) Call shortcuts_init();
 *     When that function is being called it generates hash table, where
 *     key is string (description of function) and data is pointer to function.
 *     For example string "new_theme" is key used for getting pointer to function
 *     that will create new theme.
 * 2.) Load user's profile.
 *     User's Profile that contain Eina_List of user's Shortcuts.
 * 3.) Call shortcuts_profile_load(App_Data *ap, Profile *profile);
 *     This function will create hash table. Key for that hash table is a
 *     structure with modifiers and keycode (two keys). According to those two
 *     value it will return pointer to function.
 */

#include "eflete.h"

#define MOD_NONE 0
#define MOD_SHIFT 1
#define MOD_CTRL 2
#define MOD_ALT 4
#define MOD_SUPER  8

typedef enum {
   SHORTCUT_TYPE_NONE,

   SHORTCUT_TYPE_REDO,
   SHORTCUT_TYPE_UNDO,
   SHORTCUT_TYPE_SAVE,
   SHORTCUT_TYPE_ADD_GROUP,
   SHORTCUT_TYPE_ADD_PART,
   SHORTCUT_TYPE_ADD_STATE,
   SHORTCUT_TYPE_ADD_ITEM,
   SHORTCUT_TYPE_ADD_PROGRAM,
   SHORTCUT_TYPE_ADD_DATA_ITEM,
   SHORTCUT_TYPE_DEL,
   SHORTCUT_TYPE_TAB_NUM1,
   SHORTCUT_TYPE_TAB_NUM2,
   SHORTCUT_TYPE_TAB_NUM3,
   SHORTCUT_TYPE_TAB_NUM4,
   SHORTCUT_TYPE_TAB_NUM5,
   SHORTCUT_TYPE_TAB_NUM6,
   SHORTCUT_TYPE_TAB_NUM7,
   SHORTCUT_TYPE_TAB_NUM8,
   SHORTCUT_TYPE_TAB_NUM9,
   SHORTCUT_TYPE_TAB_NUM10,
   SHORTCUT_TYPE_TAB_NEXT,
   SHORTCUT_TYPE_TAB_PREV,
   SHORTCUT_TYPE_TAB_CLOSE,
   SHORTCUT_TYPE_TAB_IMAGE_MANAGER,
   SHORTCUT_TYPE_TAB_SOUND_MANAGER,
   SHORTCUT_TYPE_TAB_COLOR_CLASS_MANAGER,
   SHORTCUT_TYPE_TAB_STYLE_MANAGER,
   SHORTCUT_TYPE_QUIT,
   SHORTCUT_TYPE_MODE_NORMAL,
   SHORTCUT_TYPE_MODE_CODE,
   SHORTCUT_TYPE_MODE_DEMO,
   SHORTCUT_TYPE_STATE_NEXT,
   SHORTCUT_TYPE_PART_NEXT,
   SHORTCUT_TYPE_PART_PREV,
   SHORTCUT_TYPE_PART_SHOWHIDE,
   SHORTCUT_TYPE_ALL_PARTS_SHOWHIDE,
   SHORTCUT_TYPE_ZOOM_IN,
   SHORTCUT_TYPE_ZOOM_OUT,
   SHORTCUT_TYPE_ZOOM_RESET,
   SHORTCUT_TYPE_OBJECT_AREA,
   SHORTCUT_TYPE_DONE,
   SHORTCUT_TYPE_CANCEL,

   SHORTCUT_TYPE_LAST,
} Shortcut_Type;

/**
 * Setting shortcuts from user's profile.
 *
 * @param ap Application Data.
 * @param profile Currently loaded user's profile.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Shortcuts
 */
Eina_Bool
shortcuts_profile_load(Profile *profile);

/**
 * Initialize shortcut module.
 *
 * This function will prepare module to be used. For example, it will create
 * hash of functions that can be used for finding according to pressed keys.
 *
 * @param ap App Data containing important information.
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Shortcuts
 */
Eina_Bool
shortcuts_init(void);

/**
 * Initialize shortcut module.
 *
 * This function will free all created structures and hashes inside of this
 * module.
 *
 * @param ap App Data containing important information.
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Shortcuts
 */
Eina_Bool
shortcuts_shutdown(void);

/**
 * Push an object on stack of shortcut handlers.
 * This object will recieve all shortcuts except SAVE and QUIT as long as it will
 * be on top of handlers stack.
 *
 * @param obj New handler object
 *
 * @ingroup Shortcuts
 */
void
shortcuts_object_push(Evas_Object *obj);

/**
 * Pop an object from stack of shortcut handlers.
 *
 * @param obj Handler object. This object should be on top of stack, otherwise
 * program will be terminated to avoid undefined behaviour.
 *
 * @ingroup Shortcuts
 */
void
shortcuts_object_check_pop(Evas_Object *obj);

#endif /* SHORTCUTS_H */
