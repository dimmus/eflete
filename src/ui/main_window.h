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

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

/**
 * @defgroup Window Window
 * @ingroup EFLETE
 *
 * Main window header provide API to create and manipulate main window object.
 * Create panes markup for differents block's, like workspace, widget list,
 * state list, etc...
 */

#include "eflete.h"
#include "config.h"
#include "common_macro.h"
#include "ui_widget_list.h"
#include "ui_signal_list.h"
#include "ui_block.h"
#include "notify.h"
#include "string_macro.h"

#include "part_dialog.h"
#include "state_dialog.h"
#include "style_dialog.h"
#include "colorsel.h"
#include "colorclass_editor.h"

enum Menu_Item
{
   /* Needed as parent for top level menu items and as end mark in items lists */
   MENU_NULL,

   MENU_FILE,
      MENU_FILE_NEW_PROJECT,
      MENU_FILE_OPEN_PROJECT,
      MENU_FILE_IMPORT_EDJ,
      MENU_FILE_IMPORT_EDC,
      MENU_FILE_SAVE,
      MENU_FILE_EXPORT_EDC,
      MENU_FILE_EXPORT,
         MENU_FILE_EXPORT_DEVELOP,
         MENU_FILE_EXPORT_RELEASE,
      MENU_FILE_CLOSE_PROJECT,
      MENU_FILE_EXIT,
   MENU_EDIT,
      MENU_EDIT_PREFERENCE,
   MENU_VIEW,
      MENU_VIEW_WORKSPACE,
         MENU_VIEW_WORKSPACE_ZOOM_IN,
         MENU_VIEW_WORKSPACE_ZOOM_OUT,
         MENU_VIEW_WORKSPACE_SEPARATE,
         MENU_VIEW_WORKSPACE_OBJECT_AREA,
      MENU_VIEW_RULERS,
         MENU_VIEW_RULERS_SHOW,
         MENU_VIEW_RULERS_ABS,
         MENU_VIEW_RULERS_REL,
         MENU_VIEW_RULERS_BOTH,
   MENU_EDITORS,
      MENU_EDITORS_ANIMATOR,
      MENU_EDITORS_IMAGE,
      MENU_EDITORS_SOUND,
      MENU_EDITORS_COLORCLASS,
      MENU_EDITORS_TEXT_STYLE,
   MENU_HELP,
      MENU_HELP_ABOUT,

   /* Needed for menu array init */
   MENU_ITEMS_COUNT
};

extern int MENU_ITEMS_LIST_BASE[];
extern int MENU_ITEMS_LIST_STYLE_ONLY[];
extern int MENU_ITEMS_LIST_MAIN[];

/**
 * Adds toolbar with menu and buttons to the given Elementary layout.
 *
 * @param ap The App_Data structure pointer.
 *
 * @return menu Menu object if successful, or NULL elthewhere.
 *
 * @ingroup Window
 */
Menu *
ui_menu_add(App_Data *ap);

/**
 * Disable or enable menu item
 *
 * If flag is EINA_TRUE - an item will be disabled, othervise - enabled, so it
 * can be used, clicked or anything like that.
 *
 * @param menu object
 * @param mid Menu item's ID.
 * @param flag for disabling - EINA_TRUE, for enabling - EINA_FALSE.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 */
Eina_Bool
ui_menu_disable_set(Menu *menu, int mid, Eina_Bool flag);

/**
 * Disable or enable list of menu items
 *
 * If flag is EINA_TRUE - an item will be disabled, othervise - enabled, so it
 * can be used, clicked or anything like that.
 *
 * @param menu object
 * @param list Array of menu items ID. Last item should always be MENU_NULL.
 * see default lists (MENU_ITEMS_LIST_BASE, MENU_ITEMS_LIST_STYLE_ONLY,
 * MENU_ITEMS_LIST_MAIN)
 * @param flag for disabling - EINA_TRUE, for enabling - EINA_FALSE.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 */
Eina_Bool
ui_menu_items_list_disable_set(Menu *menu, int *list, Eina_Bool flag);

typedef Eina_Bool
(* Splash_Cb)(void *data);

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
 * @return EINA_TRUE if successful, or EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_main_window_del(App_Data *ap) EINA_ARG_NONNULL(1);

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
 * Toggles min sizes for left Pane.
 *
 * @param ap The App_Data structure pointer.
 * @param is_on value to toggle min sizes for left Pane ON/OFF.
 * @return EINA_TRUE if succeed, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_panes_left_panes_min_size_toggle(App_Data *ap, Eina_Bool is_on);

/**
 * Show panes element on main window
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 * @return EINA_TRUE if successful, or EINA_FALSE otherwise.
 */
Eina_Bool
ui_panes_show(App_Data *ap);

/**
 * Hide panes element on main window
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 * @return EINA_TRUE if successful, or EINA_FALSE otherwise.
 */
Eina_Bool
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
 * This function will be called when back from style list button is clicked.
 *
 * @param ap The App_Data structure pointer.
 *
 * @ingroup Window
 */
void
ui_style_back(App_Data *ap);

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
 * View style objects on workspace, load signals in list. Show, or update
 * information about group parameters.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param style The @Style structure object.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_style_clicked(App_Data *ap, Style *style);

/**
 * Load project data to App_Data structure. Turn to work state for application.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @param selected_file String with opened project file name.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_edj_load(App_Data* ap, const char *selected_file);

/**
 * Delete selected style/class/layout from current widget
 *
 * @param ap The App_Data structure pointer.
 * @param group_type type of group to be deleted.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_group_delete(App_Data *ap, Type group_type);

/**
 * Open new theme from template file.
 *
 * @param ap The App_Data structure pointer.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
new_theme_create(App_Data *ap);

/**
 * Ask user if he wants to close project
 *
 * @param ap The App_Data structure pointer.
 * @param msg The explanation text that will be shown to user. NULL for default message.
 * @return EINA_TRUE if roject is saved or user wants to discard changes, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_close_project_request(App_Data *ap, const char *msg);

/**
 * Get data of widget user currently works with.
 *
 * @param ap The App_Data structure pointer.
 * @return Widget data structure.
 */
Widget *
ui_widget_from_ap_get(App_Data *ap);

/**
 * Get data of class user currently works with.
 *
 * @param ap The App_Data structure pointer.
 * @return Class data structure.
 */
Class *
ui_class_from_ap_get(App_Data *ap);

/* FIXME: Add comments */
Eina_Bool
register_callbacks(App_Data *ap);

/**
 * Add callbacks to widget list. Callbacks are using next signals:
 * "wl,group,select"
 * "wl,part,select",
 * "wl,part,back",
 * "wl,group,back",
 *
 * @param wd_list A pointer to widget list object.
 * @param ap The Apllication data.
 * @return EINA_TRUE if succeed, EINA_FALSE otherwise.
 */
Eina_Bool
add_callbacks_wd(Evas_Object *wd_list, App_Data *ap);

/**
 * Switch code editing mode ON or OFF.
 * The Code Editing mode means that Workspace, States, History and Signals
 * blocks are hidden and only Widget List, Life Wiew and Code tab are available
 * for user.
 *
 * @param ap The App_Data structure pointer.
 * @param is_on value to toggle Code Editing mode ON/OFF.
 * @return EINA_TRUE if succeed, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
code_edit_mode_switch(App_Data *ap, Eina_Bool is_on);

/**
 * The splash window with animation and info line.
 *
 * @param parent The parent widget, MUST be a window;
 * @param setup Callback will be be called on splash window show;
 * @param teardown Callback will be called on "Cancel" button click;
 * @param data User data.
 *
 * @return The splash window object.
 *
 * @ingroup Window
 */
Evas_Object *
splash_add(Evas_Object *parent, Splash_Cb setup,
           Splash_Cb teardown, void *data) EINA_ARG_NONNULL(1, 2) EINA_WARN_UNUSED_RESULT;

/**
 * Delete the splash window. Before delete will be played the close animation.
 *
 * @param obj The splash object.
 *
 * @ingroup Window
 */
void
splash_del(Evas_Object *obj);

/**
 * Show the main layout blocks.
 *
 * @param ap The Eflete App_Data.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup Window
 */
Eina_Bool
blocks_show(App_Data *ap);

/**
 * Hide the main layout blocks.
 *
 * @param ap The Eflete App_Data.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup Window
 */
Eina_Bool
blocks_hide(App_Data *ap);

/**
 * Unset data from all ui blocks.
 *
 * @param ap The Eflete App_Data.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup Window
 */
Eina_Bool
blocks_data_unset(App_Data *ap);

/**
 * Open existing project.
 *
 * @ingroup Window
 */
void
project_open();

/**
 * Save opened project.
 *
 * @ingroup Window
 */
void
project_save();

/**
 * Mark the opened project as changed, and activate "Save project" button.
 *
 * @ingroup Window
 */
void
project_changed(void);

/**
 * Dialog with qustion what do with openned project.
 *
 * @param ap The Eflete App_Data.
 * @param msg The message.
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup Window
 */
Eina_Bool
project_close_request(App_Data *ap, const char *msg);

/**
 * Export project as develop edj file.
 *
 * @ingroup Window
 */
void
project_export_develop(void);

/**
 * Update Statusbar field that contains time of last save of current
 * project file.
 *
 * @param ap The App_Data structure pointer.
 * @param is_autosave flag to inform if the function is called by
 *                    'autosave' functionality.
 * @ingroup Window
 */
void
save_time_info_update(App_Data *ap, Eina_Bool is_autosave);

/**
 * Call dialog window for request
 *
 * @param parent Parent object
 * @param msg Request message.
 * @return EINA_FALSE if user has rejected request or EINA_TRUE if user has accepted request
 *
 * @ingroup Window
 */
Eina_Bool
export_replace_request(Evas_Object *parent, const char *msg);

#endif /* UI_MAIN_WINDOW_H */
