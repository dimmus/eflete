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
 */

#include "eflete.h"

/**
 * Set up shortcuts for main window.
 *
 * @param ap Application Data.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Shortcuts
 */
Eina_Bool
shortcuts_main_add(App_Data *ap);

/**
 * Unset shortcuts from main window.
 *
 * @param ap Application Data.
 *
 * @return EINA_TRUE on success or EINA_FALSE, on errors.
 *
 * @ingroup Shortcuts
 */
Eina_Bool
shortcuts_main_del(App_Data *ap);


#endif /* SHORTCUTS_H */
