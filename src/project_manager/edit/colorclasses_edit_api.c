/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "image_edit_api.h"

Eina_List *
colorclasses_edit_list_get(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_classes_list_get(edje_obj);
}

EAPI Eina_Bool
colorclasses_edit_colors_get (Project *project,
   const char *class_name, int *r, int *g, int *b, int *a,
   int *r2, int *g2, int *b2, int *a2, int *r3, int *g3, int *b3, int *a3)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_colors_get (edje_obj, class_name,
      r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3);
}

EAPI Eina_Bool
colorclasses_edit_colors_set(Project *project,
const char *class_name, int	r, int g, int b, int a, int r2, int g2, int b2,
int a2, int r3, int g3, int b3, int a3)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_colors_set (edje_obj, class_name,
      r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3);
}

EAPI Eina_Bool
colorclass_edit_add (Project *project, const char *name)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_add(edje_obj,name);
}

EAPI Eina_Bool
colorclass_edit_del (Project *project, const char *name)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_del(edje_obj,name);
}

