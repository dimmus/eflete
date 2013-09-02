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

#include "style_edit.h"

Eina_List *
style_edit_styles_list_get(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_styles_list_get(edje_obj);
}

Eina_Bool
style_edit_style_add(Project *project, const char *style)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    return edje_edit_style_add(edje_obj, style);
}

void
style_edit_style_del(Project *project, const char *style)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    edje_edit_style_del(edje_obj, style);
}

Eina_List *
style_edit_style_tags_list_get(Project *project, const char *style)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_style_tags_list_get(edje_obj, style);
}

Eina_Bool
style_edit_style_tag_add(Project *project,
                         const char *style,
                         const char *tag_name,
                         const char *tag_value)
{
    Evas_Object *edje_obj;
    Eina_Bool res;
    GET_OBJ(project, edje_obj);

    res = edje_edit_style_tag_add(edje_obj, style, tag_name);
    if (res) edje_edit_style_tag_value_set(edje_obj, style, tag_name, tag_value);
    return res;
}

void
style_edit_style_tag_del(Project *project,
                         const char *style,
                         const char *tag_name)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    edje_edit_style_tag_del(edje_obj, style, tag_name);
}

const char*
style_edit_style_tag_value_get(Project *project,
                               const char *style,
                               const char *tag_name)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    return edje_edit_style_tag_value_get(edje_obj, style, tag_name);
}

void
style_edit_style_tag_value_set(Project *project,
                               const char* style,
                               const char* tag_name,
                               const char* new_value)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    edje_edit_style_tag_value_set(edje_obj, style, tag_name, new_value);
}

Eina_Bool
style_edit_save(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_save(edje_obj);
}
