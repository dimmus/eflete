/**
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include <Evas.h>

Eina_List *
_object_images_get(Evas_Object *obj);

Eina_List *
_object_samples_get(Evas_Object *obj);

Eina_List *
_object_fonts_get(Evas_Object *obj);

Eina_List *
_object_groups_get(Evas_Object *obj);

Eina_List *
_strings_list_duplicates_del(Eina_List *list);
