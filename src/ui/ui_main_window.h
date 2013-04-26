#ifndef __UI_MAIN_WINDOW_HEADER__
#define __UI_MAIN_WINDOW_HEADER__

#include <Elementary.h>
#include <Eet.h>

/**
 * Adds main window object for Edje tool development.
 * out: The created object, or NULL on failrue.
 */
Evas_Object *
ui_main_window_add (void);

/**
 * Delete main window object for Edje tool development. 
 */
void
ui_main_window_del (void);

/**
 * Adds marked panes to the given Elementary layout.
 * in: Layout container.
 * out: EINA_TRUE if panes created normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_panes_add (Evas_Object *layout);

void 
ui_panes_settings_load (void);

void
ui_panes_settings_save (void);

Eina_Bool
_panes_settings_save (Eina_List *ui_list_panes);

Eina_Bool
_panes_settings_load (Eina_List *ui_list_panes);

/**
 * Delete pannes and contain object.
 * out: EINA_TRUE if panes delete normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_panes_del (void);

/**
 * Adds toolbar with menu and buttons to the given Elementary layout.
 * in: Layout container.
 * out: EINA_TRUE if menu created normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_menu_add (Evas_Object *layout);

/**
 * Delete toolbar and menu objects.
 * out: EINA_TRUE if toolbar and menu delete normal, EINA_FALSE on failrue.
 */
Eina_Bool
ui_menu_del (void);

#endif	// __UI_MAIN_WINDOW_HEADER__

