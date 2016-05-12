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

#include "main_window.h"
#include "preference.h"
#include "tabs.h"
#include "workspace.h"

static int _menu_delayed_event = 0;

int MENU_ITEMS_LIST_BASE[] = {
   MENU_FILE_SAVE,
   MENU_FILE_EXPORT_EDC,
   MENU_FILE_EXPORT_EDC_PROJECT,
   MENU_FILE_EXPORT,
   MENU_FILE_EXPORT_DEVELOP,
/* MENU_FILE_EXPORT_RELEASE,*/
   MENU_FILE_CLOSE_PROJECT,
   MENU_WINDOW_MANAGER_IMAGE,
   MENU_WINDOW_MANAGER_SOUND,
   MENU_WINDOW_MANAGER_COLORCLASS,
   MENU_WINDOW_MANAGER_TEXT_STYLE,
   MENU_EDIT_GROUP_ADD,

   MENU_NULL
};

int MENU_ITEMS_LIST_STYLE_ONLY[] = {
   MENU_EDIT_UNDO,
   MENU_EDIT_REDO,
   MENU_VIEW_WORKSPACE_ZOOM_IN,
   MENU_VIEW_WORKSPACE_ZOOM_OUT,
   MENU_VIEW_WORKSPACE_ZOOM_RESET,
   MENU_VIEW_WORKSPACE_FIT,
   MENU_VIEW_WORKSPACE_FILL,
   MENU_VIEW_WORKSPACE_OBJECT_AREA,
   MENU_VIEW_RULERS_SHOW,
   MENU_FILE_EXPORT_EDC_GROUP,
   MENU_EDIT_PART_ADD,
   MENU_EDIT_STATE_ADD,
   MENU_EDIT_ITEM_ADD,
   MENU_EDIT_PROGRAM_ADD,

   MENU_NULL
};

int MENU_ITEMS_LIST_MAIN[] = {
   MENU_FILE,
   MENU_VIEW,
   MENU_EDIT,
   MENU_HELP,

   MENU_NULL
};

static int sad_callback_data[MENU_ITEMS_COUNT];

struct _Menu
{
   Elm_Object_Item *items[MENU_ITEMS_COUNT];
   Evas_Object *window_menu;
};

struct _menu_event
{
   int mid;
};
typedef struct _menu_event Menu_Event;

static Eina_Bool
_menu_cb(void *data __UNUSED__,
         int type __UNUSED__,
         void *event)
{
   Menu_Event *menu_event = (Menu_Event *)event;

   switch (menu_event->mid)
     {
      case MENU_FILE_NEW_PROJECT:
         //tabs_menu_tab_open(TAB_HOME_NEW_PROJECT);
         break;
      case MENU_FILE_OPEN_PROJECT:
         //tabs_menu_tab_open(TAB_HOME_OPEN_PROJECT);
         break;
      case MENU_FILE_IMPORT_EDJ:
         //tabs_menu_tab_open(TAB_HOME_IMPORT_EDJ);
         break;
      case MENU_FILE_IMPORT_EDC:
         //tabs_menu_tab_open(TAB_HOME_IMPORT_EDC);
         break;
      case MENU_FILE_SAVE:
         project_save();
         break;
      case MENU_FILE_EXPORT_EDC_GROUP:
         project_export_edc_group();
         break;
      case MENU_FILE_EXPORT_EDC_PROJECT:
         project_export_edc_project();
         break;
      case MENU_FILE_EXPORT_DEVELOP:
         project_export_develop();
         break;
      case MENU_FILE_EXPORT_RELEASE:
         project_export_release();
         break;
      case MENU_FILE_CLOSE_PROJECT:
           {
              if (!project_close()) break;
              //tabs_menu_tab_open(TAB_HOME_OPEN_PROJECT);
              ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_BASE, true);
              ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
              ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, true);
           }
         break;
      case MENU_FILE_EXIT:
         ui_main_window_del();
         break;
      case MENU_EDIT_PREFERENCE:
         /* preferences_window_add(ap.project); */
         break;
      case MENU_VIEW_WORKSPACE_ZOOM_IN:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ZOOM_IN, NULL);
         break;
      case MENU_VIEW_WORKSPACE_ZOOM_OUT:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ZOOM_OUT, NULL);
        break;
      case MENU_VIEW_WORKSPACE_ZOOM_RESET:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ZOOM_RESET, NULL);
         break;
      case MENU_VIEW_WORKSPACE_FIT:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_FIT, NULL);
         break;
      case MENU_VIEW_WORKSPACE_FILL:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_FILL, NULL);
         break;
      case MENU_VIEW_RULERS_SHOW:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_RULERS_VISIBLED, NULL);
         break;
      case MENU_VIEW_WORKSPACE_OBJECT_AREA:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_OBJECT_AREA, NULL);
         break;
      case MENU_EDIT_UNDO:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_UNDO, NULL);
         break;
      case MENU_EDIT_REDO:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_REDO, NULL);
         break;
      case MENU_EDIT_GROUP_ADD:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ADD_GROUP, NULL);
         break;
      case MENU_EDIT_PART_ADD:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ADD_PART, NULL);
         break;
      case MENU_EDIT_STATE_ADD:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ADD_STATE, NULL);
         break;
      case MENU_EDIT_ITEM_ADD:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ADD_ITEM, NULL);
         break;
      case MENU_EDIT_PROGRAM_ADD:
         evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_ADD_PROGRAM, NULL);
         break;

      case MENU_WINDOW_TAB_HOME:
         tabs_home_tab_add(TAB_HOME_OPEN_PROJECT);
         break;
      case MENU_WINDOW_MANAGER_IMAGE:
         //tabs_menu_tab_open(TAB_IMAGE_EDITOR);
         break;
      case MENU_WINDOW_MANAGER_SOUND:
         //tabs_menu_tab_open(TAB_SOUND_EDITOR);
         break;
      case MENU_WINDOW_MANAGER_COLORCLASS:
         //tabs_menu_tab_open(TAB_COLORCLASS_EDITOR);
         break;
      case MENU_WINDOW_MANAGER_TEXT_STYLE:
         //tabs_menu_tab_open(TAB_STYLE_EDITOR);
         break;
      case MENU_HELP_ABOUT:
         about_window_add();
         break;
      default:
         ERR("unknown menu id: %d", menu_event->mid);
         break;
     }
   return ECORE_CALLBACK_DONE;
}

static void
_delay_menu_cb(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Menu_Event *menu_event = mem_malloc(sizeof(Menu_Event));

   assert(data != NULL);

   menu_event->mid = * (int *)data;
   ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
}

static void
_project_changed(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, false);
}

Menu *
ui_menu_add(void)
{
   Evas_Object *window_menu;
   Menu *menu;
   int i = 0;

   assert(ap.win != NULL);
   assert(ap.win_layout != NULL);

   _menu_delayed_event = ecore_event_type_new();

   ecore_event_handler_add(_menu_delayed_event, _menu_cb, NULL);

   menu = mem_calloc(1, sizeof(Menu));

   /* this array is needed to pass integers as data to callback */
   for (i = 0; i < MENU_ITEMS_COUNT; i++)
     sad_callback_data[i] = i;

   window_menu = elm_win_main_menu_get(ap.win);


#define ITEM_MENU_ADD(PARENT_ID, ID, ICON, LABEL, SHORTCUT) \
   do \
     { \
        menu->items[ID] = elm_menu_item_add(window_menu, menu->items[PARENT_ID], ICON, LABEL, _delay_menu_cb, &sad_callback_data[ID]); \
        if (SHORTCUT) \
          { \
             Evas_Object *item_obj = elm_menu_item_object_get(menu->items[ID]);\
             elm_object_part_text_set(item_obj, "elm.shortcut", SHORTCUT); \
          } \
     } \
   while (0);

#define ___(PARENT_ID) \
   elm_menu_item_separator_add(window_menu, menu->items[PARENT_ID]);

   ITEM_MENU_ADD(MENU_NULL, MENU_FILE, NULL, _("File"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_NEW_PROJECT, "file", _("New project"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_OPEN_PROJECT, "folder", _("Open project"), "Ctrl-O")
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDJ, NULL, _("Import edj-file"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDC, NULL, _("Import edc-file"), NULL)
      ___(MENU_FILE);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_SAVE, EFLETE_IMG_PATH"icon-save.png", _("Save"), "Ctrl-S")
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT_EDC, NULL, _("Export as edc"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT_EDC, MENU_FILE_EXPORT_EDC_GROUP, NULL, _("Group"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT_EDC, MENU_FILE_EXPORT_EDC_PROJECT, NULL, _("Project"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT, NULL, _("Export as edj"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT, MENU_FILE_EXPORT_DEVELOP, NULL, _("Develop"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT, MENU_FILE_EXPORT_RELEASE, NULL, _("Release"), NULL)
      ___(MENU_FILE);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_CLOSE_PROJECT, NULL, _("Close project"), NULL)
      ___(MENU_FILE);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXIT, NULL, _("Quit"), "Ctrl-Q")

   ITEM_MENU_ADD(MENU_NULL, MENU_EDIT, NULL, _("Edit"), NULL)
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_UNDO, NULL, _("Undo"), "Ctrl-Z")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_REDO, NULL, _("Redo"), "Ctrl-Y")
      ___(MENU_EDIT);
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_GROUP_ADD, NULL, _("Add group"), "Ctrl-N")
      ___(MENU_EDIT);
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PART_ADD, NULL, _("Add part"), "q")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_STATE_ADD, NULL, _("Add state"), "w")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_ITEM_ADD, NULL, _("Add item"), "e")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PROGRAM_ADD, NULL, _("Add program"), "r")
      /* ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PREFERENCE, NULL, _("Preference..."))*/

   ITEM_MENU_ADD(MENU_NULL, MENU_VIEW, NULL, _("View"), NULL)
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_ZOOM_IN, NULL, _("Zoom in"), "+")
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_ZOOM_OUT, NULL, _("Zoom out"), "-")
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_ZOOM_RESET, NULL, _("Reset zoom"), "/")
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_FIT, NULL, _("Fit container in Workspace"), NULL)
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_FILL, NULL, _("Fill workspace"), NULL)
      ___(MENU_VIEW);
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_OBJECT_AREA, NULL, _("Show object area"), "o")
      ___(MENU_VIEW);
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_RULERS_SHOW, NULL, _("Show rulers"), NULL)

   ITEM_MENU_ADD(MENU_NULL, MENU_WINDOW, NULL, _("Window"), NULL)
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_TAB_HOME, NULL, _("Home"), NULL)
      ___(MENU_WINDOW);
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_IMAGE, "image2", _("Image manager"), "F7")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_SOUND, "sound2", _("Sound manager"), "F8")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_TEXT_STYLE, "text2", _("Textblock styles manager"), "F9")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_COLORCLASS, "color", _("Color class manager"), "F10")

   ITEM_MENU_ADD(MENU_NULL, MENU_HELP, NULL, _("Help"), NULL)
      ITEM_MENU_ADD(MENU_HELP, MENU_HELP_ABOUT, NULL, _("About"), NULL)

   elm_menu_item_separator_add(window_menu, menu->items[MENU_FILE_IMPORT_EDC]);

   /* uncomment it, when release export will be implemented */
   ui_menu_disable_set(menu, MENU_FILE_EXPORT_RELEASE, true);

   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

   menu->window_menu = window_menu;

   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _project_changed, NULL);

   return menu;
}

Eina_Bool
ui_menu_disable_set(Menu *menu, int mid, Eina_Bool flag)
{
   assert(menu != NULL);
   assert((mid > MENU_NULL) && (mid < MENU_ITEMS_COUNT));

   elm_object_item_disabled_set(menu->items[mid], flag);

   return true;
}

Eina_Bool
ui_menu_items_list_disable_set(Menu *menu, int *list, Eina_Bool flag)
{
   assert(menu != NULL);
   assert(list != NULL);

   Eina_Bool result = true;
   int i = 0;
   while (list[i] != 0)
     {
        result &= ui_menu_disable_set(menu, list[i], flag);
        i++;
     }
   return result;
}
