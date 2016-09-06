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

#define ALLOW_DIRECT_EDJE_EDIT_CALLS
#include "editor.h"
#include "editor_macro.h"
#include "change.h"
#include "diff.h"

extern int _editor_signals_blocked;

Eina_Bool
editor_image_add(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_image_add(obj, name));

   if (!editor_save(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_ADDED, (void *)name);
   return true;
}

Eina_Bool
editor_image_del(Evas_Object *obj, const char *name, Eina_Bool notify)
{
   assert(obj != NULL);
   assert(name != NULL);

   CRIT_ON_FAIL(edje_edit_image_del(obj, name));

   if (!editor_save(obj))
     return false; /* i hope it will never happen */
   _editor_project_changed();
   if (notify)
     evas_object_smart_callback_call(ap.win, SIGNAL_EDITOR_IMAGE_DELETED, (void *)name);
   return true;
}
