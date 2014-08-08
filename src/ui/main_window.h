/**
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
#include "ui_workspace.h"
#include "live_view.h"
#include "ui_block.h"
#include "colorclass_editor.h"
#include "notify.h"
#include "open_file_dialog.h"
#include "string_macro.h"

#include "part_dialog.h"
#include "state_dialog.h"
#include "style_dialog.h"
#include "colorsel.h"

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
 * Adds toolbar with menu and buttons to the given Elementary layout.
 *
 * @param ap The App_Data structure pointer.
 *
 * @return menu Evas_Object if successful, or NULL elthewhere.
 *
 * @ingroup Window
 */
Evas_Object *
ui_menu_add(App_Data *ap);

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
 * @return The Evas_Object pointer. Widget list object.
 *
 * @ingroup Window
 */
Evas_Object *
ui_edj_load_done(App_Data* ap, const char *selected_file);

/**
 * Delete selected state from current part.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param ap The App_Data structure pointer.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_part_state_delete(App_Data *ap);

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
 * Disable or enable menu item by it's name.'
 *
 * If flag is EINA_TRUE - an item will be disabled, othervise - enabled, so it
 * can be used, clicked or anything like that.
 *
 * @param ap The App_Data structure pointer.
 * @param name Menu item's title.
 * @param flag for disabling - EINA_TRUE, for enabling - EINA_FALSE.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 */
Eina_Bool
ui_menu_disable_set(Eina_Hash *menu_hash, const char *name, Eina_Bool flag);

/**
 * Disable or enable base menus (Editors, Saves, Separate..)
 *
 * @param ap The App_Data structure pointer.
 * @param flag for disabling - EINA_TRUE, for enabling - EINA_FALSE.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 */
Eina_Bool
ui_menu_base_disabled_set(Eina_Hash *menu_hash, Eina_Bool flag);

/**
 * Disable or enable all menus.
 *
 * @param ap The App_Data structure pointer.
 * @param flag for disabling - EINA_TRUE, for enabling - EINA_FALSE.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 */
Eina_Bool
ui_menu_locked_set(Eina_Hash *menu_hash, Eina_Bool flag);

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

#endif /* UI_MAIN_WINDOW_H */
