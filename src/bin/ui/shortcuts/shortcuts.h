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
shortcuts_init();

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
shortcuts_shutdown();

#endif /* SHORTCUTS_H */
