/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#ifndef TABS_H
#define TABS_H

/**
 * @defgroup Tabs
 * @ingroup Main_Window
 *
 * Tabs provide a open more the one layout to edit. Have a 'home' tab, where
 * user can create new, open, or import Eflete project.
 */
#include "eflete.h"

typedef enum {
   TAB_HOME_PROJECT_INFO = 0,
   TAB_HOME_NEW_PROJECT,
   TAB_HOME_IMPORT_EDJ,
   TAB_HOME_IMPORT_EDC,
   TAB_HOME_OPEN_PROJECT,
   TAB_IMAGE_EDITOR,
   TAB_SOUND_EDITOR,
   TAB_COLORCLASS_EDITOR,
   TAB_STYLE_EDITOR,
   TAB_LAST
} Tabs_Menu;

/**
 * Create a tabs.
 *
 * @param parent The parent object
 *
 * @ingroup Tabs
 */
Evas_Object *
tabs_add(void);

/**
 * Show tab according to given option
 *
 * @ingroup Tabs
 */
void
tabs_menu_tab_open(Tabs_Menu view);

/**
 * Fill the import edj tab fields
 *
 * @ingroup Tabs
 */
void
tabs_menu_import_edj_data_set(const char *name, const char *path, const char *edj, const Eina_List *widgets);

/**
 * Fill the import edc tab fields
 *
 * @ingroup Tabs
 */
void
tabs_menu_import_edc_data_set(const char *name, const char *path, const char *edc,
                              const Eina_List *img, const Eina_List *snd, const Eina_List *fnt, const Eina_List *dd);

/**
 * Fill the new project tab fields
 *
 * @ingroup Tabs
 */
void
tabs_menu_new_data_set(const char *name, const char *path, const Eina_List *widgets);

/**
 * Add new tab. This tab consist a workspace and a liveview.
 *
 * @param group The Group object
 *
 * @ingroup Tabs
 */
void
tabs_tab_add(Group *group);

/**
 * Add home tab.
 *
 * @ingroup Tabs
 */
void
tabs_home_tab_add(Tabs_Menu view);

/**
 * Close all tabs
 *
 * @ingroup Tabs
 */
void
tabs_clean(void);

/**
 * Close the selected tab.
 *
 * @ingroup Tabs
 */
void
tabs_current_tab_close(void);

/**
 * Get pointer to active workspace
 *
 * @ingroup Tabs
 */
Evas_Object *
tabs_current_workspace_get(void);

/**
 * Get pointer to group that is related to currently opened tab
 *
 * @ingroup Tabs
 */
Group *
tabs_current_group_get(void);

#endif /* TABS_H */
