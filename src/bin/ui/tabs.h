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
   TABS_NEW_PROJECT = 0,
   TABS_IMPORT_EDJ,
   TABS_IMPORT_EDC,
   TABS_OPEN_PROJECT,
   TABS_LAST
} Tabs_View;

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
 * Show home tab with given view
 *
 * @ingroup Tabs
 */
void
tabs_tab_home_open(Tabs_View view);

#endif /* TABS_H */
