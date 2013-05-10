#ifndef __UI_MAIN_WINDOW_HEADER__
#define __UI_MAIN_WINDOW_HEADER__

#include <Elementary.h>
#include <Eet.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <ui.h>

/**
 * Adds main window object for Edje tool development.
 * out: 
 * in 
 */
Eina_Bool
ui_main_window_add (UI_Data *ud);

/**
 * Delete main window object for Edje tool development.
 */
void
ui_main_window_del (void);

/**
 * Adds marked panes to the given Elementary layout.
 * in: 
 * out: EINA_TRUE if panes created normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_panes_add (UI_Data *ud);

/**
 * Start process load settings for panes.
 */
void
ui_panes_settings_load (Evas_Object *win);

/**
 * Start process save settings for panes.
 */
void
ui_panes_settings_save ();

/**
 * Delete pannes and contain object.
 * out: EINA_TRUE if panes delete normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_panes_del (void);

/**
 * Adds toolbar with menu and buttons to the given Elementary layout.
 * in: 
 * out: EINA_TRUE if menu created normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_menu_add (UI_Data *ud);

/**
 * Delete toolbar and menu objects.
 * out: EINA_TRUE if toolbar and menu delete normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_menu_del (void);

/**
 * 
 */
void
ui_resize_pans(int w, int h);
#endif	// __UI_MAIN_WINDOW_HEADER__

