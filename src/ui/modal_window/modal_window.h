/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#ifndef MODAL_WINDOW_H
#define MADAL_WINDOW_H

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
#include "widget_macro.h"

/**
 * Adds an modal window to the window.
 *
 * The obj used as parent MUST be an Elementary Window. Never call this function
 * with anything other than the top-most window as its parameter, unless you are
 * fond of undefined behavior.
 *
 * @param parent The parent object.
 * @return The new modal window or NULL if it cannot be created.
 *
 * @ingroup ModalWindow
 */
Evas_Object *
mw_add(Evas_Object *parent);

/**
 * Adds an "About" modal window to the window.
 *
 * The obj used as parent MUST be an Elementary Window. Never call this function
 * with anything other than the top-most window as its parameter, unless you are
 * fond of undefined behavior.
 *
 * @param parent The parent object.
 * @return The new modal window or NULL if it cannot be created.
 *
 * @ingroup ModalWindow
 */
Evas_Object *
mw_about_add(Evas_Object *parent);

/**
 * Set the title of the modal window.
 *
 * @param object The modal window object.
 * @param title The title to set.
 *
 * @ingroup ModalWindow
 */
void
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
 *
 * @ingroup ModalWindow
 */
void
mw_icon_set(Evas_Object *object, Evas_Object *icon);

/**
 * Set the info text to a modal window.
 *
 * This set an info text tobe used as the help info for the given window.
 *
 * @param object The modal window object to set an info.
 * @param text The text to use for an info.
 *
 * @ingroup ModalWindow
 */
void
mw_info_text_set(Evas_Object *object, const char *text);

#endif /* MODAL_WINDOW_H */
