/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
 * along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
 */

#ifndef CURSOR_H
#define CURSOR_H

/**
 * @defgroup Cursor Cursor
 * @ingroup EFLETE
 *
 * Module is manipulate the pointer in the Eflete.
 *
 */

#include "eflete.h"

/**
 * Define the cursor type that can setted to given object.
 */
enum _Cursor_Type
{
   CURSOR_UNKNOWN = -1,
   CURSOR_SYSTEM = 0,
   CURSOR_ARROW ,
   CURSOR_BOTTOM_LEFT_CORNER,
   CURSOR_BOTTOM_RIGHT_CORNER,
   CURSOR_TOP_LEFT_CORNER,
   CURSOR_TOP_RIGHT_CORNER,
   CURSOR_CROSSHAIR,
   CURSOR_FLEUR,
   CURSOR_SIZING,
   CURSOR_HAND1,
   CURSOR_LAST
};
typedef enum _Cursor_Type Cursor_Type;

/**
 * Set the style for all Eflete cursors. If in the theme some cursor is missing
 * will be used default.
 *
 * @param theme The cursors theme will be loaded and setted.
 *
 * @return EINA_TRUE, if style is loaded or EINA_FALSE overwise.
 *
 * @ingroup Cursor
 */
Eina_Bool
cursor_theme_set(const char *theme);

/**
 * Get the cursors theme.
 *
 * @return the theme name or NULL if the system cursors is used.
 *
 * @ingroup Cursor
 */
const char *
cursor_theme_get(void);

/**
 * Set the Eflete main cursor, it means that cursor will be shown in all Eflete
 * parts, except objects that setted cursor personaly, using
 * @ref cursor_style_set ["cursor_style_set()"]
 *
 * @param type The cursor type
 *
 * @return EINA_TRUE if cursor setted or EINA_FALSE overwise.
 *
 * @note Use it for set the cursor for some global events, for example
 * application try to open the file, while file is not opened set the approprite
 * cursor.
 *
 * @ingroup Cursor
 */
Eina_Bool
cursor_main_set(Cursor_Type type);

/**
 * Get the currset main cursor type.
 *
 * @return The cursor type.
 *
 * @ingroup Cursor
 */
Cursor_Type
cursor_main_get(void);

/**
 * Set the cursor to be shown when the mouse is over the object.
 *
 * @param obj The object where will be setted new cursor.
 * @param type The cursor type.
 *
 * @return EINA_TRUE if cursor is setted, or EINA_FALSE overwise.
 *
 * @ingroup Cursor
 */
Eina_Bool
cursor_style_set(Evas_Object *obj, Cursor_Type type);

/**
 * Get the cursor name which was setted to given object.
 *
 * @return The cursor type.
 *
 * @ingroup Cursor
 */
Cursor_Type
cursor_style_get(Evas_Object *obj);

#endif /* CURSOR_H */
