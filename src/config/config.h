/*I{
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
 * }
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 * @defgroup Config Config
 * @ingroup Eflete
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

enum _COLORS
{
   WORKSPACE = 0,
   GROUPEDIT,
   LIVEVIEW,
   HIGHLIGHT,
   LAST
};
typedef enum _COLORS COLORS;

struct _Profile_Color
{
   unsigned char r, g, b, a;
};
typedef struct _Profile_Color Profile_Color;

enum _Ruler_Mode {
   ABSOLUTE_SCALE,
   RELATIVE_SCALE,
   ABS_REL_SCALE
};
typedef enum _Ruler_Mode Ruler_Mode;

struct _Profile
{
   int        version;
   struct {
      const char     *home_folder;
      const char     *swap_folder;
      Eina_Bool      save_ui;
      Eina_Bool      save_win_pos;
      struct {
         Eina_Bool      autosave;
         unsigned int   period; /* autosave period in seconds */
      } autosave;
   } general;
   struct {
      const char     *bg_image;
      unsigned int   groupedit_handler_size;
      struct {
         Eina_Bool      visible;
         Ruler_Mode     mode;
      } rulers;
   } workspace;
   struct {
      const char  *bg_image;
      Eina_Bool   auto_fill_text;
      Eina_Bool   auto_fill_content;
   } liveview;
   Profile_Color colors[LAST];
};
typedef struct _Profile Profile;

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

/**
 * Load the Eflete profile by name.
 *
 * @param name The name of profile.
 *
 * This funtion load a profile by name. Profile will be searched in Eflete
 * settings folder, if file not found, will be created a default profile.
 *
 * @ingroup Config
 */
void
profile_load(const char *name);

/**
 * Save the laoded profile with new name.
 *
 * @param name The name with been saved the profile.
 *
 * @ingroup Config
 */
Eina_Bool
profile_save(const char *name);

/**
 * Get the loaded profile.
 *
 * @return The loaded profile.
 *
 * @ingroup Config
 */
Profile *
profile_get(void);

/**
 * Get the list of Eflete profiles.
 *
 * @return The list of profile names of Eflete.
 *
 * @note for delete Profiles listyou can use macro EINA_LIST_STRINGSHARE_FREE
 *       or union of EINA_LIST_FREE and eina_stringshare_del.
 *
 * @ingroup Config
 */
Eina_List *
profiles_get(void);

#endif /* CONFIG_H */
