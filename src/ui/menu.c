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
#include "wizard.h"
#include "preference.h"
#include "style_editor.h"
#include "image_editor.h"
#include "sound_editor.h"
#include "animator.h"
#include "about_window.h"

int MENU_ITEMS_LIST_BASE[] = {
   MENU_FILE_SAVE,
   MENU_FILE_SAVE_AS,
   MENU_FILE_EXPORT_EDC,
   MENU_VIEW_WORKSPACE,
   MENU_VIEW_WORKSPACE_ZOOM_IN,
   MENU_VIEW_WORKSPACE_ZOOM_OUT,
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
   MENU_VIEW_WORKSPACE_SEPARATE,
   MENU_VIEW_WORKSPACE_OBJECT_AREA,

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
   Elm_Object_Item *toolbar_items[MENU_ITEMS_COUNT];

   Evas_Object *window_menu;
   Evas_Object *toolbar;
};

static void
_menu_cb(void *data,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   int mid = * (int *)data;
   App_Data *ap = app_data_get();

   switch (mid)
   {
      case MENU_FILE_NEW_PROJECT:
         wizard_new_project_add(ap);
      break;
      case MENU_FILE_OPEN_PROJECT:
         project_open();
      break;
      case MENU_FILE_IMPORT_EDJ:
         wizard_import_edj_add(ap);
      break;
      case MENU_FILE_IMPORT_EDC:
         wizard_import_edc_add(ap);
      break;
      case MENU_FILE_SAVE:
         project_save();
      break;
      case MENU_FILE_SAVE_AS:
         /* TODO: add implementation here */
      break;
      case MENU_FILE_EXPORT_EDC:
         /* TODO: add implementation here */
      break;
      case MENU_FILE_CLOSE_PROJECT:
         {
            if (!project_close_request(ap,
                                       _("You want to close project. <br/>"
                                         "If you dont save opened project<br/>"
                                         "all your changes will be lost!")))

            pm_project_close(ap->project);
            STATUSBAR_PROJECT_PATH(ap, _("No project opened"));
            blocks_hide(ap);
            ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_BASE, true);
            ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
            ui_menu_disable_set(ap->menu, MENU_FILE_SAVE, true);
            ui_menu_disable_set(ap->menu, MENU_FILE_CLOSE_PROJECT, true);
         }
      break;
      case MENU_FILE_EXIT:
         ui_main_window_del(ap);
      break;
      case MENU_EDIT_PREFERENCE:
         /* preferences_window_add(ap->project); */
      break;
      case MENU_VIEW_WORKSPACE_ZOOM_IN:
         {
            double current_factor = workspace_zoom_factor_get(ap->workspace);
            workspace_zoom_factor_set(ap->workspace, current_factor + 0.1);
         }
      break;
      case MENU_VIEW_WORKSPACE_ZOOM_OUT:
         {
            double current_factor = workspace_zoom_factor_get(ap->workspace);
            workspace_zoom_factor_set(ap->workspace, current_factor - 0.1);
         }
      break;
      case MENU_VIEW_WORKSPACE_SEPARATE:
         {
            Eina_Bool sep = workspace_separate_mode_get(ap->workspace);
            workspace_separate_mode_set(ap->workspace, !sep);
         }
      break;
      case MENU_VIEW_RULERS_SHOW:
         evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("rulers"));
      break;
      case MENU_VIEW_RULERS_ABS:
         evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("abs"));
      break;
      case MENU_VIEW_RULERS_REL:
         evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("rel"));
      break;
      case MENU_VIEW_RULERS_BOTH:
         evas_object_smart_callback_call(ap->workspace, "ruler,toggle", strdup("abs&rel"));
      break;
      case MENU_VIEW_WORKSPACE_OBJECT_AREA:
         evas_object_smart_callback_call(ap->workspace, "highlight,visible", NULL);
      break;
      case MENU_EDITORS_ANIMATOR:
         {
            if (!ap->project->current_style)
              NOTIFY_WARNING(_("Please open the widget style for editing style programs!"))
            else
              animator_window_add(ap->project->current_style);
         }
      break;
      case MENU_EDITORS_IMAGE:
         image_editor_window_add(ap->project, MULTIPLE);
      break;
      case MENU_EDITORS_SOUND:
         sound_editor_window_add(ap->project, SOUND_EDITOR_SINGLE);
      break;
      case MENU_EDITORS_COLORCLASS:
         colorclass_viewer_add(ap->project);
      break;
      case MENU_EDITORS_TEXT_STYLE:
         style_editor_window_add(ap->project);
      break;
      case MENU_HELP_ABOUT:
         about_window_add();
      break;
      default:
         ERR("unknown menu id");
      break;
   }
}

Menu *
ui_menu_add(App_Data *ap)
{
   Evas_Object *window_menu, *toolbar;
   Menu *menu;
   int i = 0;
   if (!ap)
     {
        ERR("ap is NULL");
        return NULL;
     }
   if ((!ap->win) || (!ap->win_layout)) return NULL;

   menu = mem_calloc(1, sizeof(Menu));

   /* this array is needed to pass integers as data to callback */
   for (i = 0; i < MENU_ITEMS_COUNT; i++)
     sad_callback_data[i] = i;

   window_menu = elm_win_main_menu_get(ap->win);
   elm_object_style_set(window_menu, DEFAULT_STYLE);


#define ITEM_MENU_ADD(PARENT_ID, ID, ICON, LABEL) \
   menu->menu_items[ID] = elm_menu_item_add(window_menu, menu->menu_items[PARENT_ID], ICON, LABEL, _menu_cb, &sad_callback_data[ID]);

   ITEM_MENU_ADD(MENU_NULL, MENU_FILE, NULL, _("File"))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_NEW_PROJECT, NULL, _("New project..."))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_OPEN_PROJECT, NULL, _("Open project..."))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDJ, NULL, _("Import edj-file..."))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDC, NULL, _("Import edc-file..."))
      elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE]);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_SAVE, NULL, _("Save"))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_SAVE_AS, NULL, _("Save as..."))
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT_EDC, NULL, _("Export to edc..."))
      elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE]);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_CLOSE_PROJECT, NULL, _("Close project"))
      elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE]);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXIT, NULL, _("Exit"))
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
   ITEM_MENU_ADD(MENU_NULL, MENU_EDITORS, NULL, _("Editors"))
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_ANIMATOR, NULL, _("Animator"))
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_IMAGE, NULL, _("Images"))
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_SOUND, NULL, _("Sound"))
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_COLORCLASS, NULL, _("Colorclasses"))
      ITEM_MENU_ADD(MENU_EDITORS, MENU_EDITORS_TEXT_STYLE, NULL, _("Text styles"))
   ITEM_MENU_ADD(MENU_NULL, MENU_HELP, NULL, _("Help"))
      ITEM_MENU_ADD(MENU_HELP, MENU_HELP_ABOUT, NULL, _("About"))

   elm_menu_item_separator_add(window_menu, menu->menu_items[MENU_FILE_IMPORT_EDC]);
#undef ITEM_MENU_ADD

   toolbar = elm_toolbar_add(ap->win);
   elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_MENU);
   /*TODO: for this select mode need new style */
   elm_toolbar_select_mode_set(toolbar, ELM_OBJECT_SELECT_MODE_NONE);
   elm_object_style_set(toolbar, DEFAULT_STYLE);
   elm_toolbar_align_set(toolbar, 0.0);
   evas_object_size_hint_weight_set(toolbar, 0.0, 0.0);
   evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, 0.0);
   elm_object_part_content_set(ap->win_layout, "eflete.swallow.toolbar", toolbar);
   evas_object_show(toolbar);

#define ITEM_TB_ADD(ID, ICON, LABEL) \
   menu->toolbar_items[ID] = elm_toolbar_item_append(toolbar, ICON, LABEL, _menu_cb, &sad_callback_data[ID]);

   ITEM_TB_ADD(MENU_FILE_NEW_PROJECT, EFLETE_IMG_PATH"icon-new_project.png", _("New project"));
   ITEM_TB_ADD(MENU_FILE_OPEN_PROJECT, EFLETE_IMG_PATH"icon-open_project.png", _("Open project"));
   ITEM_TB_ADD(MENU_FILE_SAVE, EFLETE_IMG_PATH"icon-save.png", _("Save project"));
   ITEM_TB_ADD(MENU_EDITORS_ANIMATOR, EFLETE_IMG_PATH"icon-animator.png", _("Animator"));
   ITEM_TB_ADD(MENU_EDITORS_IMAGE, EFLETE_IMG_PATH"icon-image.png", _("Image editor"));
   ITEM_TB_ADD(MENU_EDITORS_SOUND, EFLETE_IMG_PATH"icon-sound.png", _("Sound editor"));
   ITEM_TB_ADD(MENU_EDITORS_COLORCLASS, EFLETE_IMG_PATH"icon-color.png", _("Color class editor"));
   ITEM_TB_ADD(MENU_EDITORS_TEXT_STYLE, EFLETE_IMG_PATH"icon-text.png", _("Textblock style editor"));

#undef ITEM_TB_ADD
   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_STYLE_ONLY, true);

   ui_menu_disable_set(menu, MENU_FILE_CLOSE_PROJECT, true);

   menu->window_menu = window_menu;
   menu->toolbar = toolbar;
   return menu;
}

Eina_Bool
ui_menu_disable_set(Menu *menu, int mid, Eina_Bool flag)
{
   if (!menu)
     {
        ERR("Menu is NULL");
        return false;
     }
   if ((mid <= MENU_NULL) || (mid >= MENU_ITEMS_COUNT))
     {
        ERR("Wrond menu item id: %d", mid);
        return false;
     }
   elm_object_item_disabled_set(menu->menu_items[mid], flag);
   elm_object_item_disabled_set(menu->toolbar_items[mid], flag);

   return true;
}

Eina_Bool
ui_menu_items_list_disable_set(Menu *menu, int *list, Eina_Bool flag)
{
   if ((!menu) || (!list)) return false;

   Eina_Bool result = true;
   int i = 0;
   while (list[i] != 0)
     {
        result &= ui_menu_disable_set(menu, list[i], flag);
        i++;
     }
   return result;
}
