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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @defgroup Config Config
 * @ingroup EFLTET
 *
 * A Config module respons for save current sizes of window, panes and user
 * profiles.
 * Also stored window position on descktop.
 */

#include "eflete.h"

struct _Config
{
   struct {
      int x, y, w, h;
   } window;
   struct { /* take and save left panes size */
      double left;
      double right;
      double left_hor;
      double right_hor;
      double center;
      double center_down;
   } panes;
   const char *profile;
};
typedef struct _Config Config;

/**
 * Initialisation config module.
 *
 * @return EINA_TRUE if init complete successful. EINA_FALSE overwise.
 *
 * @ingroup Config
 */
Eina_Bool
config_init(void);

/**
 * Release the config module.
 *
 * @ingroup Config
 */
void
config_shutdown(void);

/**
 * Load the Eflete config.
 *
 * @ingroup Config
 */
void
config_load(void);

/**
 * Save the Eflete config.
 *
 * @return EINA_TRUE if save complete successful. EINA_FALSE if fail
 * save data to disk.
 *
 * @ingroup Config
 */
Eina_Bool
config_save(App_Data *ap);

/**
 * Get the Eflete config.
 *
 * @return The Eflete config, or NULL if it not loaded.
 *
 * @ingroup Config
 */
Config *
config_get(void);

#endif /* CONFIG_H */
