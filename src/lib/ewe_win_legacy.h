/**
 * Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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

#ifndef EWE_WIN_LEGACY_H
#define EWE_WIN_LEGACY_H

/**
 * Adds a window onject.
 * If this is the first window created, pass NULL as parent.
 *
 * @param  parent Parent object to add the window to, or NULL
 * @param name The name of the window
 * @param type	The window type, one of Ewe_Win_Type.
 * @return The created object, or NULL on failure
 *
 * @ingroup Win
 */
EAPI Evas_Object *
ewe_win_add(Evas_Object *parent, const char *name, Ewe_Win_Type type);

/**
 * Set the floating mode of a window.
 *
 * @param obj The window object
 * @param floating If true, the window is floating mode
 *
 * @ingroup Win
 */
EAPI void
ewe_win_floating_mode_set(Evas_Object *obj, Eina_Bool floating);

/**
 * Get the floating mode of a window.
 *
 * @param obj The window object
 *
 * @ingroup Win
 */
EAPI Eina_Bool
ewe_win_floating_mode_get(const Evas_Object *obj);

/**
 * This manually asks evas to render the window now.
 *
 * @param obj The window object
 *
 * @ingroup Win
 */
EAPI void ewe_win_render(Evas_Object *obj);

#endif /* EWE_WIN_LEGACY_H */
