#ifndef UI_MAIN_WINDOW_HEADER_H
#define UI_MAIN_WINDOW_HEADER_H

/**
 * @defgroup Window Window
 * @ingroup EFL_TET
 *
 * Main window header provide API to create and manipulate main window object.
 * Create panes markup for differents block's, like workspace, widget list,
 * state list, etc...
 */

#include "efl_tet.h"
#include "ui_widget_list.h"
#include "ui_signal_list.h"
#include "ui_workspace.h"
#include "ui_block.h"
#include "image_viewer_dialog.h"
#include "colorclass_viewer_dialog.h"

/**
 * Adds main window object for Edje tool development.
 *
 * @param ap The App_Data structure pointer.
 *
 * @return EINA_TRUE if window and subblocks created correct, EINA_FALSE if fail.
 *
 * @ingroup Window
 */
Eina_Bool
ui_main_window_add(App_Data *ap);

/**
 * Delete main window object for Edje tool development.
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 */
void
ui_main_window_del(App_Data *ap);

/**
 * Adds marked panes to the given Elementary layout.
 *
 * @param ap The App_Data structure pointer.
 * @return EINA_TRUE if all panes created and positioned correctly, EINA_FALSE
 * if fail create any one of panes.
 *
 * @ingroup Window
 */
Eina_Bool
ui_panes_add(App_Data *ap);

/**
 * Start process load settings for panes.
 *
 * @ingroup Window
 */
void
ui_panes_settings_load();

/**
 * Start process save settings for panes.
 *
 * @ingroup Window
 */
void
ui_panes_settings_save();

/**
 * Adds toolbar with menu and buttons to the given Elementary layout.
 *
 * @param ap The App_Data structure pointer.
 * @returnEINA_TRUE if menu created normal, EINA_FALSE on failrue.
 *
 * @ingroup Window
 */
Eina_Bool
ui_menu_add(App_Data *ap);

/**
 * Resize all panes on main window according to window new size.
 *
 * @param w A main window new width value.
 * @param h A main window new height value
 *
 * @ingroup Window
 */
void
ui_resize_pans(int w, int h);

/**
 * Show panes element on main window
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 */
void
ui_panes_show(App_Data *ap);

/**
 * Hide panes element on main window
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 */
void
ui_panes_hide(App_Data *ap);

#endif	/* UI_MAIN_WINDOW_HEADER_H */

