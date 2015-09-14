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

#ifndef TABS_PRIVATE
#define TABS_PRIVATE

#include "main_window.h"
#include "navigator.h"

Evas_Object *
_tab_open_project_add(void);

Evas_Object *
_tab_new_project_add(void);

void
_tab_open_project_recents_update();

Evas_Object *
_tab_import_edj_add(void);

Evas_Object *
_tab_import_edc_add(void);

Eina_Bool
progress_print(void *data, Eina_Stringshare *progress_string);

void
progress_end(void *data, PM_Project_Result result);

void
elipsis_btn_add(Evas_Object *entry, Evas_Smart_Cb cb_func, void *data);

void
exist_permission_check(const char *path, const char *name, const char *title);

#endif /* TABS_PRIVATE */
