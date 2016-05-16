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

#ifndef MODAL_WINDOW_H
#define MODAL_WINDOW_H

/**
 * @defgroup ModalWindow Modal Window
 *
 * In user interface design, a modal window is a child window that requires
 * users to interact with it before they can return to operating the parent
 * application, thus preventing the workflow on the application main window.
 *
 * It window have a title, icon and two buttons. The modal window have two
 * buttons: close and info. Close button - closed the window. Info button
 * serves for view information about current modal window.
 */

#include "eflete.h"

/**
 * Adds an modal window to the window.
 *
 * The obj used as parent MUST be an Elementary Window. Never call this function
 * with anything other than the top-most window as its parameter, unless you are
 * fond of undefined behavior.
 *
 * @return The new modal window or NULL if it cannot be created.
 *
 * @note Do not use evas_object_del for this object if you want to play hide
 * animation.
 *
 * @ingroup ModalWindow
 */
Evas_Object *
mw_add(void);

/**
 * Set the title of the modal window.
 *
 * @param object The modal window object.
 * @param title The title to set.
 * @return EINA_FALSE on error, EINA_TRUE otherwise
 *
 * @ingroup ModalWindow
 */
Eina_Bool
mw_title_set(Evas_Object *object, const char *title);

/**
 * Set a modal window object's icon
 *
 * This sets an image to be used as the icon for the given window. The exact
 * pixel dimensions of the object (not object size) will be used, and the image
 * pixels will be used as-is when this function is called. If any icon was
 * already set as a content object in the same part, previous object will be
 * deleted automatically.
 *
 * @param object The modal window object to set an icon.
 * @param icon The image object to use for an icon.
 * @return EINA_FALSE on error, EINA_TRUE otherwise
 *
 * @ingroup ModalWindow
 */
Eina_Bool
mw_icon_set(Evas_Object *object, Evas_Object *icon);

void
mw_del(Evas_Object *mw);

#endif /* MODAL_WINDOW_H */
