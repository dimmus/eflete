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

/**
 * Defines the types of window that can be created
 *
 * These are hints set on the window so that a running Window Manager knows
 * how the window should be handled and/or what kind of decorations it
 * should have.
 *
 * Currently, only the X11 backed engines use them.
 */

#ifndef EWE_WIN_COMMON_H
#define EWE_WIN_COMMON_H

typedef enum
{
   EWE_WIN_UNKNOWN = -1,
   EWE_WIN_BASIC, /**< A normal window. Indicates a normal, top-level
                     window. Almost every window will be created with this
                     type. */
   EWE_WIN_DIALOG_BASIC, /**< Used for simple dialog windows/ */
   EWE_WIN_DESKTOP, /**< For special desktop windows, like a background
                       window holding desktop icons. */
   EWE_WIN_DOCK, /**< The window is used as a dock or panel. Usually would
                    be kept on top of any other window by the Window
                    Manager. */
   EWE_WIN_TOOLBAR, /**< The window is used to hold a floating toolbar, or
                       similar. */
   EWE_WIN_MENU, /**< Similar to #ELM_WIN_TOOLBAR. */
   EWE_WIN_UTILITY, /**< A persistent utility window, like a toolbox or
                       palette. */
   EWE_WIN_SPLASH, /**< Splash window for a starting up application. */
   EWE_WIN_DROPDOWN_MENU, /**< The window is a dropdown menu, as when an
                             entry in a menubar is clicked. Typically used
                             with elm_win_override_set(). This hint exists
                             for completion only, as the EFL way of
                             implementing a menu would not normally use a
                             separate window for its contents. */
   EWE_WIN_POPUP_MENU, /**< Like #ELM_WIN_DROPDOWN_MENU, but for the menu
                          triggered by right-clicking an object. */
   EWE_WIN_TOOLTIP, /**< The window is a tooltip. A short piece of
                       explanatory text that typically appear after the
                       mouse cursor hovers over an object for a while.
                       Typically used with elm_win_override_set() and also
                       not very commonly used in the EFL. */
   EWE_WIN_NOTIFICATION, /**< A notification window, like a warning about
                            battery life or a new E-Mail received. */
   EWE_WIN_COMBO, /**< A window holding the contents of a combo box. Not
                     usually used in the EFL. */
   EWE_WIN_DND, /**< Used to indicate the window is a representation of an
                   object being dragged across different windows, or even
                   applications. Typically used with
                   elm_win_override_set(). */
   EWE_WIN_INLINED_IMAGE, /**< The window is rendered onto an image
                             buffer. No actual window is created for this
                             type, instead the window and all of its
                             contents will be rendered to an image buffer.
                             This allows to have children window inside a
                             parent one just like any other object would
                             be, and do other things like applying @c
                             Evas_Map effects to it. This is the only type
                             of window that requires the @c parent
                             parameter of elm_win_add() to be a valid @c
                             Evas_Object. */
   EWE_WIN_SOCKET_IMAGE, /**< The window is rendered onto an image buffer
                             and can be shown other process's plug image object.
                             No actual window is created for this type,
                             instead the window and all of its contents will be
                             rendered to an image buffer and can be shown
                             other process's plug image object*/
} Ewe_Win_Type;

#endif /* EWE_WIN_COMMON_H */
