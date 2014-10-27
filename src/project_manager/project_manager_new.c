/*I{
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
 *}
 */

#include "project_manager_new.h"
#include "alloc.h"

Project *
pm_project_import_edj(const char *name __UNUSED__,
                      const char *path __UNUSED__,
                      const char *edj __UNUSED__)
{
   return NULL;
}

Project *
pm_project_import_edc(const char *name __UNUSED__,
                      const char *path __UNUSED__,
                      const char *edc __UNUSED__,
                      const char *image_directory __UNUSED__,
                      const char *sound_directory __UNUSED__,
                      const char *font_directory __UNUSED__,
                      const char *data_directory __UNUSED__,
                      const char *vibration_directory __UNUSED__);

Project *
pm_project_open(const char *path __UNUSED__)
{
   return NULL;
}

Eina_Bool
pm_project_save(Project *project __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_style_save_as(Project *project __UNUSED__, const char *file __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_build(Project *project __UNUSED__, Build build_profile __UNUSED__)
{
   return false;
}

Eina_Bool
pm_project_close(Project *project __UNUSED__)
{
   return false;
}

void
pm_project_meta_data_get(Project *project __UNUSED__,
                         char *name __UNUSED__,
                         char *authors __UNUSED__,
                         char *version __UNUSED__,
                         char *license __UNUSED__,
                         char *comment __UNUSED__)
{

}

Eina_Bool
pm_project_meta_data_set(Project *project __UNUSED__,
                         const char *name __UNUSED__,
                         const char *authors __UNUSED__,
                         const char *version __UNUSED__,
                         const char *license __UNUSED__,
                         const char *comment __UNUSED__)
{
   return false;
}
