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
#include "widget_macro.h"

#ifdef HAVE_AUDIO
TODO("Check pulse_audio on configure and add COREAUDIO support")
   #define HAVE_PULSE 1
   #include <Ecore_Audio.h>
#endif

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
         MENU_FILE_EXPORT_EDC_PROJECT,
         MENU_FILE_EXPORT_EDC_GROUP,
      MENU_FILE_EXPORT,
         MENU_FILE_EXPORT_DEVELOP,
         MENU_FILE_EXPORT_RELEASE,
      MENU_FILE_CLOSE_PROJECT,
      MENU_FILE_EXIT,
   MENU_EDIT,
      MENU_EDIT_UNDO,
      MENU_EDIT_REDO,
      MENU_EDIT_GROUP_ADD,
      MENU_EDIT_PART_ADD,
      MENU_EDIT_STATE_ADD,
      MENU_EDIT_ITEM_ADD,
      MENU_EDIT_PROGRAM_ADD,

      MENU_EDIT_PREFERENCE,
   MENU_VIEW,
      MENU_VIEW_WORKSPACE_ZOOM_IN,
      MENU_VIEW_WORKSPACE_ZOOM_OUT,
      MENU_VIEW_WORKSPACE_ZOOM_RESET,
      MENU_VIEW_WORKSPACE_FIT,
      MENU_VIEW_WORKSPACE_FILL,
      MENU_VIEW_WORKSPACE_OBJECT_AREA,
      MENU_VIEW_RULERS_SHOW,
   MENU_WINDOW,
      MENU_WINDOW_TAB_HOME,
      MENU_WINDOW_MANAGER_IMAGE,
      MENU_WINDOW_MANAGER_SOUND,
      MENU_WINDOW_MANAGER_COLORCLASS,
      MENU_WINDOW_MANAGER_TEXT_STYLE,

   MENU_HELP,
      MENU_HELP_ABOUT,

   /* Needed for menu array init */
   MENU_ITEMS_COUNT
};

extern int MENU_ITEMS_LIST_BASE[];
extern int MENU_ITEMS_LIST_STYLE_ONLY[];
extern int MENU_ITEMS_LIST_MAIN[];
extern int MENU_ITEMS_LIST_TEMPORARY[];

typedef enum
{
   BG_PREVIEW_WHITE = 1,
   BG_PREVIEW_TILE,
   BG_PREVIEW_BLACK
} Bg_Preview;

/**
 * The validation func for popup action.
 *
 * @param data The user data.
 *
 * @ingroup Window
 */
typedef Eina_Bool(* Popup_Validator_Func)(void *data);

/**
 * The fileselector helper callback.
 *
 * @param data The user data;
 * @param obj The fileselector from helper;
 * @param event_info The eina_list with the selected paths.
 *
 * @return EINA_TRUE if all good and you happy, overwise all bad EINA_FALSE.
 *
 * @ingroup Window
 */
typedef Eina_Bool(* Helper_Done_Cb)(void *data, Evas_Object *obj, void *event_info);

#define POPUP_BUTTON_MASK 0xf
typedef enum _Popup_Button
{
   BTN_NONE       = 0,
   BTN_OK         = (1 << 0),
   BTN_CANCEL     = (1 << 1),
   BTN_SAVE       = (1 << 2),
   BTN_DONT_SAVE  = (1 << 3),
   BTN_REPLACE    = (1 << 4)
} Popup_Button;

/**
 * Adds toolbar with menu and buttons to the given Elementary layout.
 *
 * @return menu Menu object if successful, or NULL elthewhere.
 *
 * @ingroup Window
 */
Menu *
ui_menu_add(void);

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

enum _Splash_Status
{
   SPLASH_NONE,
   SPLASH_SUCCESS,
   SPLASH_CANCEL,
   SPLASH_LAST
};
typedef enum _Splash_Status Splash_Status;

typedef Eina_Bool
(* Splash_Cb)(void *data, Splash_Status status);

/**
 * Adds main window object for Edje tool development.
 *
 * @return EINA_TRUE if window and subblocks created correct, EINA_FALSE if fail.
 *
 * @ingroup Window
 */
Eina_Bool
ui_main_window_add(void);

/**
 * Delete main window object for Edje tool development.
 *
 * @return EINA_TRUE if successful, or EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_main_window_del(void);

/**
 * Update displayed information about current part state. Update part object
 * on workspace with new state parameters.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param obj The Evas_Object pointer, wich pointed on states genlist.
 * @param state Name of the state to be set.
 *
 * @ingroup Window
 */
void
ui_state_select(Evas_Object *obj, Eina_Stringshare *state);

/**
 * Load project data to App_Data structure. Turn to work state for application.
 * Moved to own method for the separation of the interaction between the blocks.
 *
 * @param selected_file String with opened project file name.
 *
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_edj_load(const char *selected_file);

/**
 * Ask user if he wants to close project
 *
 * @param msg The explanation text that will be shown to user. NULL for default message.
 * @return EINA_TRUE if roject is saved or user wants to discard changes, EINA_FALSE otherwise.
 *
 * @ingroup Window
 */
Eina_Bool
ui_close_project_request(const char *msg);

/**
 * The splash window with animation and info line.
 *
 * @param parent The parent widget, MUST be a window;
 * @param setup Callback will be be called on splash window show;
 * @param teardown Callback will be called on splash delete event;
 * @param cancel Callback will be called on "Cancel" button click;
 * @param data User data.
 *
 * @return The splash window object.
 *
 * @ingroup Window
 */
Evas_Object *
splash_add(Evas_Object *parent,
           Splash_Cb setup,
           Splash_Cb teardown,
           Splash_Cb cancel,
           void *data) EINA_ARG_NONNULL(1, 2) EINA_WARN_UNUSED_RESULT;

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
 * Save opened project.
 *
 * @ingroup Window
 */
void
project_save(void);

/**
 * Requesting to change project (need to close it, to hide blocks, unset data,
 * etc).
 * This function will ask user what to do with opened project
 * (if it is changed).
 *
 * @return EINA_TRUE on success, otherwise EINA_FALSE.
 *
 * @ingroup Window
 */
Eina_Bool
project_close(void);

/**
 * Create and show popup with given title and content. Only one - Evas_Object or
 * text will be setted as content. And you can set define the buttons what you
 * want to use in the popup. Func have a blocked behavior, while the popup button
 * not clicked, popup blocked the code runing, used ecore main loop iteration.
 * Vlidator called only click on the positive buttons like: BTN_OK, BTN_SAVE and
 * BTN_REPLACE. Another buttons are negative.
 *
 * @note Popup have not delete the setted content. User must delete it manualy.
 * @note The focused object should be in the popup content.
 *
 * @param title The Popup title;
 * @param msg The Popup message, formated text;
 * @param content The user Evas_Object seted as content to popup;
 * @param to_focus The object what be focused after popup show;
 * @param p_btns The flags for set the popup buttons;
 * @param func The validation func, if returned EINA_FALSE popup not be closed;
 * @param data The user data for validation func.
 *
 * @ingroup Window
 */
Popup_Button
popup_want_action(const char *title,
                  const char *msg,
                  Evas_Object *content,
                  Evas_Object *to_focus,
                  Popup_Button p_btns,
                  Popup_Validator_Func func,
                  void *data);
/**
 * Disable the popup buttons.
 *
 * @param p_btns The flags for set the popup buttons;
 * @param disabled The button disabled state.
 *
 * @ingroup Window
 */
void
popup_buttons_disabled_set(Popup_Button p_btns, Eina_Bool disabled);

void
popup_fileselector_folder_helper(const char *title, Evas_Object *follow_up, const char *path,
                                 Helper_Done_Cb func, void *data,
                                 Eina_Bool multi, Eina_Bool is_save);

void
popup_fileselector_edj_helper(const char *title, Evas_Object *follow_up, const char *path,
                              Helper_Done_Cb func, void *data,
                              Eina_Bool multi, Eina_Bool is_save);

void
popup_fileselector_edc_helper(const char *title, Evas_Object *follow_up, const char *path,
                              Helper_Done_Cb func, void *data,
                              Eina_Bool multi, Eina_Bool is_save);

void
popup_fileselector_image_helper(const char *title, Evas_Object *follow_up, const char *path,
                                Helper_Done_Cb func, void *data,
                                Eina_Bool multi, Eina_Bool is_save);

void
popup_fileselector_sound_helper(const char *title, Evas_Object *follow_up, const char *path,
                                Helper_Done_Cb func, void *data,
                                Eina_Bool multi, Eina_Bool is_save);

void
popup_fileselector_file_set(const char *file);

void
popup_log_message_helper(const char *msg);

void
popup_gengrid_image_helper(const char *title, Evas_Object *follow_up,
                           Helper_Done_Cb func, void *data,
                           Eina_Bool multi);

void
popup_gengrid_helper_item_select(const char *item_title);

void
popup_colorselector_helper(Evas_Object *follow_up,
                           Helper_Done_Cb func, Evas_Smart_Cb func_change,
                           void *data, int r, int g, int b, int a);

/**
 * Export project as develop edj file.
 *
 * @ingroup Window
 */
void
project_export_develop(void);

/**
 * Export project as release edj file.
 *
 * @ingroup Window
 */
void
project_export_release(void);

/**
 * Update Statusbar field that contains time of last save of current
 * project file.
 *
 * @param is_autosave flag to inform if the function is called by
 *                    'autosave' functionality.
 * @ingroup Window
 */
void
save_time_info_update(Eina_Bool is_autosave);

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

/**
 * Export project as edc.
 *
 * @ingroup Window
 */
void
project_export_edc_project(void);


/**
 * Export group as edc.
 *
 * @ingroup Window
 */
void
project_export_edc_group(void);

/**
 * Add new image editor layout for setting into tab.
 *
 * @return Pointer to layout object, which contain image grid and buttons.
 *
 * @ingroup Window
 */
Evas_Object *
image_manager_add(void);

typedef struct _Colorclass_Item Colorclass_Item;
struct _Colorclass_Item
{
   Eina_Stringshare *name;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
};

struct _ColorClassData
{
   Colorclass_Item *current_ccl;
   Evas_Object *edje_preview;
};

/**
 * Add new colorclass manager layout object.
 *
 * @return Pointer to layout object, which contain list of colorclasses,
 * control buttons, etc.
 *
 * @ingroup Window
 */
Evas_Object *
colorclass_manager_add();

struct _Style_Data
{
   const char *st_name;
   const char *st_tag;
   Eina_Stringshare *stvalue;
   Eina_Strbuf *style;
   Evas_Object *textblock_style;
   Eina_Bool is_parent_item;
};
typedef struct _Style_Data Style_Data;

/**
 * Add new style manager layout object.
 *
 * @return Pointer to layout object, which contain list of styles,
 * control buttons, etc.
 *
 * @ingroup Window
 */
Evas_Object *
style_manager_add();

typedef enum {
   SOUND_TYPE_SAMPLE,
   SOUND_TYPE_TONE
} Sound_Type;

struct _Sound_Data {
   Sound_Type type;
   Resource *resource; /* for SAMPLE it's External_Resource, TONE - Tone_Resource */
   Eina_Stringshare *name;
   Eina_Stringshare *type_label;
};
typedef struct _Sound_Data Sound_Data;

/**
 * Add new sound manager layout object.
 *
 * @return Pointer to layout object, which contain list of sounds,
 * control buttons, etc.
 *
 * @ingroup Window
 */
Evas_Object *
sound_manager_add(void);

/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @return the inwin object that contain style viewer
 *
 * @ingroup Window
 */
Evas_Object *
about_window_add(void);

#endif /* UI_MAIN_WINDOW_H */
