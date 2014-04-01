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
 * @defgroup Settings Settings
 * @ingroup EFL_TET
 *
 * A Settings module respons for save current sizes of window and panes.
 * Also stored window position on descktop.
 */

#include "ui_main_window.h"

#define CONFIGFILE EFLETE_SETT_PATH"eflete.cfg"

struct _Config
{
   Evas_Object *panes_left;
   Evas_Object *panes_left_hor;
   Evas_Object *panes_right;
   Evas_Object *panes_right_hor;
   Evas_Object *panes_center;
   Evas_Object *panes_center_down;
   Evas_Object *window;
};
typedef struct _Config Config;

/**
 * Initialisation fields of Window_Config struct.
 *
 * @return pointer of Config, if it succes init, or NULL if fail.
 *
 * @ingroup Settings
 */
Config *
config_init(void);

/**
 * Save the Eflete config.
 *
 * @return EINA_TRUE if save complete successful. EINA_FALSE if fail
 * save data to disk.
 *
 * @ingroup Settings
 */
Eina_Bool
config_save(void);

/**
 * Load the Eflete config.
 *
 * @return EINA_TRUE if load complete successful. EINA_FALSE if fail
 * to load stored data.
 *
 * @ingroup Settings
 */
Eina_Bool
config_load(void);

#endif /* CONFIG_H */
