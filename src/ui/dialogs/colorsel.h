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

/**
 * @defgroup Colorsel Color selector
 *
 * A ColorSelector is a color selection widget. It allows application to set a
 * series of colors.It also allows to load/save colors from/to config with a
 * unique identifier, by default, the colors are loaded/saved from/to config
 * using "default" identifier. The colors can be picked by user from the color
 * set by clicking on individual color item on the palette or by selecting it
 * from selector.
 *
 * This widget is combined of ctxpopup and colorselector widgets. This widget
 * can be added ONLY to window.
 *
 * This widget emits the following signals, besides the ones sent from Layout:
 * @li "dissmised"
 * @li "color,changed" - When the color value changes on selector event_info is NULL.
 * @li "palette,item,selected" - When user clicks on color item. The event_info
 *      parameter of the callback will be the selected color item.
 * @li "palette,item,longpressed" - When user long presses on color item. The
 *      event info parameter of the callback contains selected color item.
 *
 */

#include "eflete.h"

/**
 * Add a new colorselector to the parent to application window.
 *
 * @param parent The parent object.
 *
 * @return The new object or NULL if it cannot be created.
 *
 * @note Only the window can be parent to colorselector, in the another case
 *       behavior is undefined.
 *
 * @ingroup Colorsel
 */
Evas_Object *
colorselector_add(Evas_Object *parent);

/**
 * Set color to colorselector.
 *
 * @param obj Colorselector object,
 * @param r r-value of color,
 * @param g g-value of color,
 * @param b b-value of color,
 * @param a a-value of color.
 *
 * @ingroup Colorsel
 */
void
colorselector_color_set(Evas_Object *obj, int r, int g, int b, int a);

/**
 * Get current color from colorselector.
 *
 * @param obj Colorselector object,
 * @param r integer pointer for r-value of color,
 * @param g integer pointer for g-value of color,
 * @param b integer pointer for b-value of color,
 * @param a integer pointer for a-value of color.
 *
 * @ingroup Colorsel
 */
void
colorselector_color_get(Evas_Object *obj, int *r, int *g, int *b, int *a);

/**
 * @warning NOT IMPLEMENTED
 * Get a list of saved paletts name.
 *
 * @param obj Colorselector object.
 *
 * @return The list of paletts names or NULL, if not saved pallets.
 *
 * @ingroup Colorsel
 */
Eina_List *
colorsector_palettes_get(Evas_Object *obj);

/**
 * @warning NOT IMPLEMENTED
 * Save the current palett colors.
 *
 * @param obj Colorselector object.
 * @param name The name of saved palette.
 *
 * @return EINA_TRUE if the palatte be saved, EINA_FALSE otherwise.
 *
 * @ingroup Colorsel
 */
Eina_Bool
colorselector_palette_save(Evas_Object *obj, const char *name);

/**
 * @warning NOT IMPLEMENTED
 * Load the palette by name and set the pallet to colorselector.
 *
 * @param obj Colorselector object,
 * @param name The name of palette, that will be loaded.
 *
 * @return EINA_TRUE if the palatte be loaded, EINA_FALSE otherwise.
 *
 * @ingroup Colorsel
 */
Eina_Bool
colorselector_palette_load(Evas_Object *obj, const char *name);
