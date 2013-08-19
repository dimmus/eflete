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
#include "colorclass_editor.h"
#include "font_viewer_dialog.h"
#include "notify.h"
/**
 * Adds main window object for Edje tool development.
 *
 * @param ap The App_Data structure pointer.
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
ui_main_window_del(void);

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

/**
 * Clear groupspace with highlight. Clear items from states, signals
 * groups and parts genlists.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 */
void
ui_part_back(App_Data *ap);

/**
 * Can be used in future API!!!!
 * This function will be called when back from group list button is clicked.
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 */
void
ui_group_back(App_Data *ap);

/**
 * Update displayed information about current part state. Update part object
 * on workspace with new state parameters.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param obj The Evas_Object pointer, wich pointed on states genlist.
 * @param state Name of the state to be set.
 *
 * @ingroup Window
 */
void
ui_state_select(App_Data *ap, Evas_Object *obj, Eina_Stringshare *state);

/**
 * Show information about properties of part. Highlight part object
 * on workspace.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param part The Part pointer.
 *
 * @return Evas_object pointer. States gen list object.
 * @ingroup Window
 */
Evas_Object *
ui_part_select(App_Data *ap, Part* part);

/**
 * View group objects on workspace, load signals in list. Show, or update
 * information about group parameters.1
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param group The Group pointer.
 *
 * @ingroup Window
 */
void
ui_group_clicked(App_Data *ap, Group *group);

/**
 * Load project data to App_Data structure. Turn to work state for application.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param obj The Evas_Object pointer, wich pointed to fileselector widget.
 * @param selected String with opened project file name.
 *
 * @return The Evas_Object pointer. Widget list object.
 *
 * @ingroup Window
 */
Evas_Object *
ui_edj_load_done(App_Data* ap, Evas_Object* obj, const char *selected);

/**
 * Load project data to App_Data structure. Turn to work state for application.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param obj The Evas_Object pointer, wich pointed to fileselector widget.
 * @param selected String with opened project file name.
 *
 * @ingroup Window
 */
Evas_Object *
ui_edc_load_done(App_Data* ap, const char *project_name, const char *path_edc, const char *path_id, const char *path_sd, const char *path_fd);

#endif	/* UI_MAIN_WINDOW_HEADER_H */

