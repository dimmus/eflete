/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
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
* along with this program; If not, see .
*/

#include "image_edit_api.h"

Eina_List *
image_edit_images_list_get(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_images_list_get(edje_obj);
}

EAPI int
image_edit_image_id_get (Project *project, const char *image_name)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_image_id_get(edje_obj,image_name);
}

EAPI Edje_Edit_Image_Comp
image_edit_image_compression_type_get (Project *project, const char *image)
{
   Evas_Object *edje_obj;
   GET_OBJ(project,edje_obj);

   return edje_edit_image_compression_type_get (edje_obj, image);
}

EAPI int
image_edit_image_compression_rate_get  (  Project *project, const char *   image )
{
   Evas_Object *edje_obj;
   GET_OBJ(project,edje_obj);

   return edje_edit_image_compression_rate_get(edje_obj,image);
}
