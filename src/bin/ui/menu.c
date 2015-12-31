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
#include "style_editor.h"
#include "image_editor.h"
#include "sound_editor.h"
#include "animator.h"
#include "about_window.h"
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
   MENU_VIEW_RULERS,
   MENU_VIEW_RULERS_SHOW,
   MENU_VIEW_RULERS_ABS,
   MENU_VIEW_RULERS_REL,
   MENU_VIEW_RULERS_BOTH,
   MENU_VIEW_WORKSPACE_OBJECT_AREA,
   MENU_EDITORS_IMAGE,
   MENU_EDITORS_SOUND,
   MENU_EDITORS_COLORCLASS,
   MENU_EDITORS_TEXT_STYLE,

   MENU_NULL
};

int MENU_ITEMS_LIST_STYLE_ONLY[] = {
   MENU_EDITORS_ANIMATOR,
   MENU_VIEW_WORKSPACE,
   /* MENU_FILE_EXPORT_EDC_GROUP, */

   MENU_NULL
};

int MENU_ITEMS_LIST_MAIN[] = {
   MENU_FILE,
   MENU_VIEW,
   /*MENU_EDIT,*/
   MENU_EDITORS,
   MENU_HELP,

   MENU_NULL
};

static int sad_callback_data[MENU_ITEMS_COUNT];

struct _Menu
{
   Elm_Object_Item *menu_items[MENU_ITEMS_COUNT];
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
         tabs_menu_tab_open(TAB_HOME_NEW_PROJECT);
         break;
      case MENU_FILE_OPEN_PROJECT:
         tabs_menu_tab_open(TAB_HOME_OPEN_PROJECT);
         break;
      case MENU_FILE_IMPORT_EDJ:
         tabs_menu_tab_open(TAB_HOME_IMPORT_EDJ);
         break;
      case MENU_FILE_IMPORT_EDC:
         tabs_menu_tab_open(TAB_HOME_IMPORT_EDC);
         break;
      case MENU_FILE_SAVE:
         project_save();
         break;
      case MENU_FILE_EXPORT_EDC_GROUP:
         TODO("Implement me")
         /* project_export_edc_group(); */
         break;
      case MENU_FILE_EXPORT_EDC_PROJECT:
         project_export_edc_project();
         break;
      case MENU_FILE_EXPORT_DEVELOP:
         project_export_develop();
         break;
      case MENU_FILE_CLOSE_PROJECT:
           {
              if (!project_close()) break;
              tabs_menu_tab_open(TAB_HOME_OPEN_PROJECT);
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
           {
              double current_factor = workspace_zoom_factor_get(ap.workspace);
              workspace_zoom_factor_set(ap.workspace, current_factor + 0.1);
           }
         break;
      case MENU_VIEW_WORKSPACE_ZOOM_OUT:
           {
              double current_factor = workspace_zoom_factor_get(ap.workspace);
              workspace_zoom_factor_set(ap.workspace, current_factor - 0.1);
           }
         break;
      case MENU_VIEW_WORKSPACE_SEPARATE:
           {
              Eina_Bool sep = workspace_separate_mode_get(ap.workspace);
              workspace_separate_mode_set(ap.workspace, !sep);
           }
         break;
      case MENU_VIEW_RULERS_SHOW:
         evas_object_smart_callback_call(ap.workspace, "ruler,toggle", strdup("rulers"));
         break;
      case MENU_VIEW_RULERS_ABS:
         evas_object_smart_callback_call(ap.workspace, "ruler,toggle", strdup("abs"));
         break;
      case MENU_VIEW_RULERS_REL:
         evas_object_smart_callback_call(ap.workspace, "ruler,toggle", strdup("rel"));
         break;
      case MENU_VIEW_RULERS_BOTH:
         evas_object_smart_callback_call(ap.workspace, "ruler,toggle", strdup("abs&rel"));
         break;
      case MENU_VIEW_WORKSPACE_OBJECT_AREA:
         evas_object_smart_callback_call(ap.workspace, "highlight,visible", NULL);
         break;
      case MENU_EDITORS_ANIMATOR:
           {
              if (!ap.project)
                NOTIFY_WARNING(_("Please open the widget style for editing style programs!"))
 /*             else
                animator_window_add(ap.project);*/
           }
         break;
      case MENU_EDITORS_IMAGE:
         tabs_menu_tab_open(TAB_IMAGE_EDITOR);
         break;
      case MENU_EDITORS_SOUND:
         tabs_menu_tab_open(TAB_SOUND_EDITOR);
         break;
      case MENU_EDITORS_COLORCLASS:
         tabs_menu_tab_open(TAB_COLORCLASS_EDITOR);
         break;
      case MENU_EDITORS_TEXT_STYLE:
         tabs_menu_tab_open(TAB_STYLE_EDITOR);
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
   Evas_Object *window_menu, *items_obj;
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


#define ITEM_MENU_ADD(PARENT_ID, ID, ICON, LABEL) \
   menu->menu_items[ID] = elm_menu_item_add(window_menu, menu->menu_items[PARENT_ID], ICON, LABEL, _delay_menu_cb, &sad_callback_data[ID]);

   ITEM_MENU_ADD(MENU_NULL, MENU_FILE, NULL, _("File"))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_NEW_PROJECT, "file", _("New project"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_FILE_NEW_PROJECT]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-N");
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_OPEN_PROJECT, "folder", _("Open project"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_FILE_OPEN_PROJECT]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-O");
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDJ, NULL, _("Import edj-file"))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDC, NULL, _("Import edc-file"))
      elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE]);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_SAVE, EFLETE_IMG_PATH"icon-save.png", _("Save"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_FILE_SAVE]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-S");
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT_EDC, NULL, _("Export as edc"))
         ITEM_MENU_ADD(MENU_FILE_EXPORT_EDC, MENU_FILE_EXPORT_EDC_GROUP, NULL, _("Group"))
         ITEM_MENU_ADD(MENU_FILE_EXPORT_EDC, MENU_FILE_EXPORT_EDC_PROJECT, NULL, _("Project"))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT, NULL, _("Export as edj"))
         ITEM_MENU_ADD(MENU_FILE_EXPORT, MENU_FILE_EXPORT_DEVELOP, NULL, _("Develop"))
         ITEM_MENU_ADD(MENU_FILE_EXPORT, MENU_FILE_EXPORT_RELEASE, NULL, _("Release"))
      elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE]);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_CLOSE_PROJECT, NULL, _("Close project"))
      elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE]);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXIT, NULL, _("Quit"))
         items_obj = elm_menu_item_object_get(menu->menu_items[MENU_FILE_EXIT]);
         elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-Q");
/* ITEM_MENU_ADD(NULL, MENU_EDIT, NULL, _("Edit"))
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PREFERENCE, NULL, _("Preference..."))*/
   ITEM_MENU_ADD(MENU_NULL, MENU_VIEW, NULL, _("View"))
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE, NULL, _("Workspace"))
         ITEM_MENU_ADD(MENU_VIEW_WORKSPACE, MENU_VIEW_WORKSPACE_ZOOM_IN, NULL, _("Zoom in"))
         ITEM_MENU_ADD(MENU_VIEW_WORKSPACE, MENU_VIEW_WORKSPACE_ZOOM_OUT, NULL, _("Zoom out"))
         elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_VIEW_WORKSPACE]);
         ITEM_MENU_ADD(MENU_VIEW_WORKSPACE, MENU_VIEW_WORKSPACE_SEPARATE, NULL, _("Separate"))
         ITEM_MENU_ADD(MENU_VIEW_WORKSPACE, MENU_VIEW_WORKSPACE_OBJECT_AREA, NULL, _("Show/Hide object area"))
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_RULERS, NULL, _("Rulers"))
         ITEM_MENU_ADD(MENU_VIEW_RULERS, MENU_VIEW_RULERS_SHOW, NULL, _("Show/Hide rulers"))
         elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_VIEW_RULERS]);
         ITEM_MENU_ADD(MENU_VIEW_RULERS, MENU_VIEW_RULERS_ABS, NULL, _("Absolute scale"))
         ITEM_MENU_ADD(MENU_VIEW_RULERS, MENU_VIEW_RULERS_REL, NULL, _("Relative scale"))
         ITEM_MENU_ADD(MENU_VIEW_RULERS, MENU_VIEW_RULERS_BOTH, NULL, _("Both"))
   ITEM_MENU_ADD(MENU_NULL, MENU_EDITORS, NULL, _("Edit"))
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_ANIMATOR, "animator", _("Animator"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_EDITORS_ANIMATOR]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-1");
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_IMAGE, "image", _("Image manager"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_EDITORS_IMAGE]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-2");
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_SOUND, "sound", _("Sound manager"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_EDITORS_SOUND]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-3");
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_COLORCLASS, "color", _("Color class manager"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_EDITORS_COLORCLASS]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-4");
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_TEXT_STYLE, "text", _("Textblock styles manager"))
      items_obj = elm_menu_item_object_get(menu->menu_items[MENU_EDITORS_TEXT_STYLE]);
      elm_object_part_text_set(items_obj, "elm.shortcut", "Ctrl-5");
   ITEM_MENU_ADD(MENU_NULL, MENU_HELP, NULL, _("Help"))
      ITEM_MENU_ADD(MENU_HELP, MENU_HELP_ABOUT, NULL, _("About"))

   elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE_IMPORT_EDC]);
#undef ITEM_MENU_ADD

   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

   TODO("remove both lines after implementation this features")
   ui_menu_disable_set(menu, MENU_FILE_EXPORT_RELEASE, true);
   ui_menu_disable_set(menu, MENU_FILE_EXPORT_EDC_GROUP, true);

   menu->window_menu = window_menu;

   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _project_changed, NULL);

   return menu;
}

Eina_Bool
ui_menu_disable_set(Menu *menu, int mid, Eina_Bool flag)
{
   assert(menu != NULL);
   assert((mid > MENU_NULL) && (mid < MENU_ITEMS_COUNT));

   elm_object_item_disabled_set(menu->menu_items[mid], flag);

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
