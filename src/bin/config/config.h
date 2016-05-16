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

/**
 * @struct _Config
 *
 * Contain some values and settings of eflete window.
 *
 * @ingroup Config
 */
struct _Config
{
   struct {
      int x; /**< X position of window. */
      int y; /**< Y position of window. */
      int w; /**< width of window. */
      int h; /**< height of window. */
   } window;
   struct { /* take and save left panes size */
      double left; /**< position of left panes */
      double right; /**< position of right panes */
      double left_ver; /**< position of panes between the project and group navi */
   } panes;
   Eina_List *recents; /**< The list of recent projects */
   const char *profile; /**< profile name */
};
typedef struct _Config Config;

/**
 * @struct _Recent
 *
 * Containt data about early opened project.
 * 
 * @ingroup Config
 */
struct _Recent
{
   char *name;
   char *path;
};
typedef struct _Recent Recent;

/**
 * @enum _COLORS
 * @ingroup Config
 */
enum _COLORS
{
   WORKSPACE = 0,
   GROUPEDIT,
   LIVEVIEW,
   HIGHLIGHT,
   LAST
};
/**
 * @typedef COLORS
 * @ingroup Config
 */
typedef enum _COLORS COLORS;

/**
 * @struct _Profile_Color
 *
 * Set of colors.
 *
 * @ingroup Config
 */
struct _Profile_Color
{
   unsigned char r; /**< red color */
   unsigned char g; /**< green color */
   unsigned char b; /**< blue color */
   unsigned char a; /**< alpha */
};
/**
 * @typedef Profile_Color
 * @ingroup Config
 */
typedef struct _Profile_Color Profile_Color;

/**
 * @enum _Ruler_Mode
 * @ingroup Config
 */
enum _Ruler_Mode {
   ABSOLUTE_SCALE,
   RELATIVE_SCALE,
   ABS_REL_SCALE
};
/**
 * @typedef Ruler_Mode
 * @ingroup Config
 */
typedef enum _Ruler_Mode Ruler_Mode;

/**
 * @struct _Profile
 *
 * User's profile containing some information about shortcuts, ruler's mode,
 * colors etc.
 *
 * @ingroup Config
 */
struct _Profile
{
   int        version;
   struct {
      const char     *projects_folder;
      Eina_Bool      save_ui;
      Eina_Bool      save_win_pos;
      struct {
         Eina_Bool      autosave; /**< shows if autosave is enabled or not */
         unsigned int   period; /**< autosave period in seconds */
      } autosave;
   } general;
   struct {
      const char     *bg_image; /**< background image of workspace */
      unsigned int   groupedit_handler_size; /**< size of groupedit handlers */
      struct {
         Eina_Bool      visible; /**< shows if rulers are visible or not */
         Ruler_Mode     mode; /**< mode of rulers */
      } rulers;
   } workspace;
   struct {
      const char  *bg_image; /**< background image of live view */
      Eina_Bool   auto_fill_text; /**< shows if live view automatically fills
                                       all text fields in live object */
      Eina_Bool   auto_fill_content; /**< shows if live view automatically fills
                                          all contents in live object */
   } liveview;
   struct {
      double scale; /**< scale of the edc source code in enventor mode*/
   } enventor;
   Profile_Color colors[LAST]; /**< colors */
   Eina_List     *shortcuts; /**< user defined or default shortcuts */
};

/**
 * @struct _Shortcuts
 *
 * Contains all information about current shortcut.
 *
 * @ingroup Config
 */
struct _Shortcuts
{
   const char       *keyname; /**< key name of the shortcut (important to have
                                   for showing in prefference/settings). */
   unsigned int      keycode; /**< for multilanguage purpose. Keycode of clicked
                                   keyboard button. */
   unsigned int      modifiers; /**< Pressed modifiers. (SHIFT is 1, CTRL is 2,
                                     ALT is 4). For example, if modifiers = 5,
                                     then ALT and SHIFT are pressed at same
                                     time. */
   const char       *description; /**< description of the shortcut. According to
                                       description there is a function will be
                                       called after using shortcut. */
   Eina_Bool        holdable; /**< Shows if shortcut function working only when
                                  key is being holded. */
};

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
 * @return EINA_TRUE if init complete successful. EINA_FALSE overwise.
 *
 * @ingroup Config
 */
Eina_Bool
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
 *                   save data to disk.
 *
 * @ingroup Config
 */
Eina_Bool
config_save(void);

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
 * Update data about sizes of Panes in configuration WITHOUT saving the Eflete config.
 *
 * @return EINA_TRUE if succeed, EINA_FALSE otherwise.
 *
 * @ingroup Config
 */
Eina_Bool
config_panes_sizes_data_update(void);

/**
 * Add new project data to recent list.
 *
 * @param name The project name;
 * @param path The project path.
 *
 * @ingroup Config
 */
void
config_recent_add(const char *name, const char *path);

/**
 * Clear the recent list.
 *
 * @ingroup Config
 */
void
config_recent_list_clear(void);

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
