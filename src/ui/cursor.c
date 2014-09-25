/**
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

#include "cursor.h"

#define CURSOR_KEY "eflete_cursor"

struct _Cursor
{
   Cursor_Type type;
   Ecore_Evas *ee;
};
typedef struct _Cursor Cursor;

static const char *cursor_string[CURSOR_LAST] = {
   NULL,
   "arrow",
   "bottom_left_corner",
   "bottom_right_corner",
   "top_left_corner",
   "top_right_corner",
   "crosshair",
   "fluer",
   "sizing",
   "hand1"
};

static Evas_Object *
_cursor_object_get(Evas_Object *obj, Cursor_Type type)
{
   Evas *e;
   Eina_Stringshare *cur_name;
   Evas_Object *cur_obj;
   int x, y;

   e = evas_object_evas_get(obj);

   cur_name = eina_stringshare_printf("eflete/cursor/%s/default",
                                      cursor_string[type]);
   cur_obj = edje_object_add(e);
   edje_object_file_set(cur_obj, EFLETE_EDJ, cur_name);
   edje_object_size_min_get(cur_obj, &x, &y);
   edje_object_size_min_restricted_calc(cur_obj, &x, &y, x, y);
   evas_object_resize(cur_obj, x, y);

   eina_stringshare_del(cur_name);
   return cur_obj;
}

static void
_ecore_evas_cursor_set(Ecore_Evas *ee, Evas_Object *cursor)
{
   int x, y;
   edje_object_part_geometry_get(cursor, "hotspot", &x, &y, NULL, NULL);
   ecore_evas_object_cursor_set(ee, cursor, ELM_OBJECT_LAYER_CURSOR, x, y);
}

static void
_eflete_cursor_mouse_in(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj,
                        void *event_info)
{
   Evas_Event_Mouse_In *ev;
   Cursor *cursor;
   Evas_Object *cur_obj;

   ev = (Evas_Event_Mouse_In *)event_info;
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) return;

   cursor = evas_object_data_get(obj, CURSOR_KEY);
   cur_obj = _cursor_object_get(obj, cursor->type);
   _ecore_evas_cursor_set(cursor->ee, cur_obj);
}

static void
_eflete_cursor_mouse_out(void *data __UNUSED__,
                         Evas *e __UNUSED__,
                         Evas_Object *obj,
                         void *event_info)
{
   Evas_Event_Mouse_Out *ev;
   Cursor *cursor;
   Evas_Object *cur_obj;

   ev = (Evas_Event_Mouse_Out *)event_info;
   if (ev->event_flags & EVAS_EVENT_FLAG_ON_HOLD) return;

   cursor = evas_object_data_get(obj, CURSOR_KEY);
   cursor = ecore_evas_data_get(cursor->ee, CURSOR_KEY);
   cur_obj = _cursor_object_get(obj, cursor->type);

   _ecore_evas_cursor_set(cursor->ee, cur_obj);
}

static void
_eflete_object_cursor_del(void *data,
                          Evas *e __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Cursor *cursor = (Cursor *)data;
   free(cursor);
}


Eina_Bool
cursor_main_set(Evas_Object *win, Cursor_Type type)
{
   const char *obj_name;
   Evas *e;
   Ecore_Evas *ee;
   Evas_Object *cur_obj;
   Cursor *cursor;

   if ((type <= CURSOR_UNKNOWN) || (type >= CURSOR_LAST)) return false;
   if (!win) return false;
   obj_name = evas_object_type_get(win);
   if (strcmp(obj_name, "elm_win")) return false;

   e = evas_object_evas_get(win);
   ee = ecore_evas_ecore_evas_get(e);
   cur_obj = _cursor_object_get(win, type);
   _ecore_evas_cursor_set(ee, cur_obj);

   cursor = ecore_evas_data_get(ee, CURSOR_KEY);
   if (!cursor) cursor = mem_malloc(sizeof(Cursor));
   cursor->type = type;
   cursor->ee = ee;
   ecore_evas_data_set(ee, CURSOR_KEY, cursor);

   return true;
}

Cursor_Type
cursor_main_get(Evas_Object *win)
{
   Evas* e;
   Ecore_Evas *ee;
   Cursor *cursor;

   if (!win) return CURSOR_UNKNOWN;
   e = evas_object_evas_get(win);
   ee = ecore_evas_ecore_evas_get(e);
   cursor = ecore_evas_data_get(ee, CURSOR_KEY);

   if (!cursor)
     return CURSOR_SYSTEM;
   else
     return cursor->type;
}

Eina_Bool
cursor_type_set(Evas_Object *obj, Cursor_Type type)
{
   Evas *e;
   Ecore_Evas *ee;
   Cursor *cursor;

   if ((type <= CURSOR_UNKNOWN) || (type >= CURSOR_LAST)) return false;
   if (!obj) return false;

   cursor = evas_object_data_get(obj, CURSOR_KEY);
   if (!cursor) cursor = mem_malloc(sizeof(Cursor));

   e = evas_object_evas_get(obj);
   ee = ecore_evas_ecore_evas_get(e);

   cursor->type = type;
   cursor->ee = ee;
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_IN,
                                  _eflete_cursor_mouse_in, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_MOUSE_OUT,
                                  _eflete_cursor_mouse_out, NULL);
   evas_object_event_callback_add(obj, EVAS_CALLBACK_DEL,
                                  _eflete_object_cursor_del, cursor);
   evas_object_data_set(obj, CURSOR_KEY, cursor);

   return true;
}

Cursor_Type
cursor_type_get(Evas_Object *obj)
{
   Cursor *cursor;

   if (!obj) return CURSOR_UNKNOWN;
   cursor = evas_object_data_get(obj, CURSOR_KEY);

   return cursor->type;
}
