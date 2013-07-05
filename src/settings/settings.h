#ifndef SETTINGS_HEADER_H
#define SETTINGS_HEADER_H

/**
 * @defgroup Settings Settings
 * @ingroup EFL_TET
 *
 * A Settings module respons for save current sizes of window and panes.
 * Also stored window position on descktop.
 */

#include "ui_main_window.h"

#define SETTINGSFILE TET_SETT_PATH".efltet"

struct _UI_Elements_Settings
{
	Evas_Object *panes_left;
	Evas_Object *panes_left_hor;
	Evas_Object *panes_right;
	Evas_Object *panes_right_hor;
	Evas_Object *panes_center;
	Evas_Object *panes_center_down;
	Evas_Object *window;
};
typedef struct _UI_Elements_Settings UI_Elements_Settings;

/**
 * Initialisation fields of UI_Elements_Settings struct.
 *
 * @return pointer of UI_Elements_Setting if it succes init, or NULL if fail.
 *
 * @ingroup Settings
 */
UI_Elements_Settings *
ui_element_settings_init(void);

/**
 * Save ui elements settings (position and size of main window, size of panes)
 * to "ui.set" file. Key in file "settings".
 *
 * @return EINA_TRUE if save complete successful. EINA_FALSE if fail
 * save data to disk.
 *
 * @ingroup Settings
 */
Eina_Bool
ui_settings_save(void);

/**
 * Load settings from "ui.set" file. Key in file "settings"
 * Load position and size of main window, and left and right sizes of panes.
 *
 * @return EINA_TRUE if load complete successful. EINA_FALSE if fail
 * to load stored data.
 *
 * @ingroup Settings
 */
Eina_Bool
ui_settings_load(void);

#endif /* SETTINGS_HEADER_H */
