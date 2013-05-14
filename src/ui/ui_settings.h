#ifndef __UI_SETTINGS_HEADER__
#define __UI_SETTINGS_HEADER__

#include "ui_main_window.h"

#define UISETTINGSFILE "ui.set"

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
 * out: pointer of UI_Elements_Setting if it succes init, or NULL if fail.
 */
UI_Elements_Settings *
ui_element_settings_init (void);

/**
 * Save ui elements settings (position and size of main window, size of panes)
 * to "ui.set" file. Key in file "settings".
 * out: EINA_TRUE if save complete successful. EINA_FALSE if fail
 * save data to disk.
 */
Eina_Bool
ui_settings_save ();

/**
 * Load settings from "ui.set" file. Key in file "settings"
 * Load position and size of main window, and left and right sizes of panes.
 * out: EINA_TRUE if load complete successful. EINA_FALSE if fail
 * to load stored data.
 */
Eina_Bool
ui_settings_load ();

#endif // __UI_SETTINGS_HEADER__

