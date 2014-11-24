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

#include "wizard.h"

struct _Wizard_Import_Edj_Win
{
   Evas_Object *win;
   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edj;
   Evas_Object *meta_name;
   Evas_Object *meta_version;
   Evas_Object *meta_authors;
   Evas_Object *meta_licenses;
   Evas_Object *meta_comment;
};

typedef struct _Wizard_Import_Edj_Win Wizard_Import_Edj_Win;

Evas_Object *
wizard_import_edj_add(App_Data *ap __UNUSED__)
{
   Evas_Object *mwin, *layout;
   Evas_Object *bt;
   Wizard_Import_Edj_Win *wiew;

   mwin = mw_add(NULL, NULL);
   if (!mwin) return NULL;
   mw_title_set(mwin, _("Wizard: import edj"));

   wiew = (Wizard_Import_Edj_Win *)mem_malloc(sizeof(Wizard_Import_Edj_Win));
   wiew->win = mwin;

   layout = elm_layout_add(mwin);
   elm_layout_file_set(layout, EFLETE_EDJ, "elm/layout/wizard/import");
   elm_object_content_set(mwin, layout);
   evas_object_show(layout);

   BUTTON_ADD(layout, bt, _("Apply"))
   elm_object_part_content_set(layout, "swallow.button1", bt);
   BUTTON_ADD(layout, bt, _("Cancel"))
   elm_object_part_content_set(layout, "swallow.button2", bt);

   //label.name
   elm_object_part_text_set(layout, "label.name", _("Project name:"));
   ENTRY_ADD(layout, wiew->name, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.name", wiew->name);
   //label.path
   elm_object_part_text_set(layout, "label.path", _("Path to project:"));
   ENTRY_ADD(layout, wiew->path, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.path", wiew->path);
   //label.edj
   elm_object_part_text_set(layout, "label.edj", _("Path to edj file:"));
   ENTRY_ADD(layout, wiew->edj, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.edj", wiew->edj);

   //label.meta_name
   elm_object_part_text_set(layout, "label.meta_name", _("Project name:"));
   ENTRY_ADD(layout, wiew->meta_name, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_name", wiew->meta_name);
   //label.meta_version
   elm_object_part_text_set(layout, "label.meta_version", _("Version of file:"));
   ENTRY_ADD(layout, wiew->meta_version, true, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_version", wiew->meta_version);
   //label.meta_authors
   elm_object_part_text_set(layout, "label.meta_author", _("Authors:"));
   ENTRY_ADD(layout, wiew->meta_authors, false, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_authors", wiew->meta_authors);
   //label.meta_licenses
   elm_object_part_text_set(layout, "label.meta_licenses", _("Licenses:"));
   ENTRY_ADD(layout, wiew->meta_licenses, false, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_licenses", wiew->meta_licenses);
   //label.meta_comment
   elm_object_part_text_set(layout, "label.meta_comment", _("Comment:"));
   ENTRY_ADD(layout, wiew->meta_comment, false, DEFAULT_STYLE)
   elm_object_part_content_set(layout, "swallow.meta_comment", wiew->meta_comment);

   evas_object_show(mwin);
   return mwin;
}
