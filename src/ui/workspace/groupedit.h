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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

struct _Geom{
   int x, y, w, h;
};

typedef struct _Geom Groupedit_Geom;

Evas_Object *
groupedit_add(Evas_Object *parent);

Eina_Bool
groupedit_handler_size_set(Evas_Object *obj, int htl_w, int htl_h, int hrb_w, int hrb_h);

Eina_Bool
groupedit_handler_size_get(Evas_Object *obj, int *htl_w, int *htl_h, int *hrb_w, int *hrb_h);

/**
 * if w or h < 0, set 0
 */
Eina_Bool
groupedit_min_size_set(Evas_Object *obj, int w, int h);

/**
 * if w or h < 0, set -1
 */
Eina_Bool
groupedit_max_size_set(Evas_Object *obj, int w, int h);

/**
 * if w > max.w then set max.w
 * if h > max.h then set max.h
 * if w < min.w then set min.w
 * if h < min.h then set min.h
 */
Eina_Bool
groupedit_container_size_set(Evas_Object *obj, int w, int h);

void
groupedit_container_size_get(Evas_Object *obj, int *w, int *h);

/*TODO: aspect for groupedit */
//Eina_Bool
//ws_groupedit_aspect_set(Evas_Object *obj, double x, double y);


/* if the style discription missing for some groupedit parts (contariner,
   handlers), will be used default style. */
Eina_Bool
groupedit_style_set(Evas_Object *obj, const char *style);

const char *
groupedit_style_get(Evas_Object *obj);



/******************************************************************************/
/******************************************************************************/
Eina_Bool
groupedit_edit_object_set(Evas_Object *obj,
                              Evas_Object *edit_obj,
                              const char *file);

Evas_Object *
groupedit_edit_object_unset(Evas_Object *obj);

void
groupedit_edit_object_recalc_all(Evas_Object *obj);

Eina_Bool
groupedit_edit_object_part_add(Evas_Object *obj, const char *part,
                               Edje_Part_Type type, const char *data);

Eina_Bool
groupedit_edit_object_part_del(Evas_Object *obj, const char *part);

Eina_Bool
groupedit_edit_object_part_state_set(Evas_Object *obj, const char *part,
                                     const char *state, double value);

Eina_Bool
groupedit_edit_object_part_state_add(Evas_Object *obj, const char *part,
                                     const char *state, double value);

Eina_Bool
groupedit_edit_object_part_state_del(Evas_Object *obj, const char *part,
                                     const char *state, double value);

void
groupedit_part_object_area_set(Evas_Object *obj, const char *part);

void
groupedit_part_object_area_visible_set(Evas_Object *obj, Eina_Bool visible);

Eina_Bool
groupedit_part_object_area_visible_get(Evas_Object *obj);
