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

struct _Meta_Data_Controls
{
   Evas_Object *version;
   Evas_Object *authors;
   Evas_Object *licenses;
   Evas_Object *comment;
};
typedef struct _Meta_Data_Controls Meta_Data_Controls;

void
meta_controls_add(Evas_Object *layout, Meta_Data_Controls *meta);

/* save data from controls to project file */
void
meta_controls_data_save(Meta_Data_Controls *meta);

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

Evas_Object *
_tab_project_info_add(void);

Eina_Bool
progress_print(void *data, Eina_Stringshare *progress_string);

void
progress_end(void *data, PM_Project_Result result);

void
elipsis_btn_add(Evas_Object *entry, Evas_Smart_Cb cb_func, void *data);

void
exist_permission_check(const char *path, const char *name, const char *title);

void
entry_path_set(void *data, Evas_Object *obj, void *event_info);


#endif /* TABS_PRIVATE */
