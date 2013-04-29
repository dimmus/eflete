#ifndef __UI_SETTINGS_HEADER__
#define __UI_SETTINGS_HEADER__

#define SETTINGSFILE "config.set"


/**
 * Save panes settings to "config.set" file. Key in file "panes_settings".
 * Saves left and right sizes of panes,and fixed state.
 * in: Eina_List that contain pointers to all panes.
 * out: EINA_TRUE if save complete successful. EINA_FALSE if fail
 * save data to disk.
 */
Eina_Bool
_panes_settings_save (Eina_List *ui_list_panes);

/**
 * Load settings from "config.set" file. Key in file "panes_settings"
 * Load left and right sizes of panes,and fixed state.
 * in: Eina_List that contain pointers to all panes.
 * out: EINA_TRUE if load complete successful. EINA_FALSE if fail
 * to load stored data.
 */
Eina_Bool
_panes_settings_load (Eina_List *ui_list_panes);

#endif // __UI_SETTINGS_HEADER__

