/*
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

#include "eflete.h"
#ifdef HAVE_TIZEN

static int _spinner_changed_from_code = 0;
static void
_tizen_spinner_changed_hack(void *data __UNUSED__, Evas_Object *obj, void *ei)
{
   /* don't call callback if we are setting value to spinner from code */
   if (!_spinner_changed_from_code)
      evas_object_smart_callback_call(obj, signals.elm.spinner.changed_user, ei);
}

void
tizen_hack_spinner_value_set(Evas_Object *spinner, double val)
{
   assert(spinner != NULL);

   _spinner_changed_from_code++;
   _elm_spinner_value_set(spinner, val);
   _spinner_changed_from_code--;
}

Evas_Object *
tizen_hack_spinner_add(Evas_Object *parent)
{
   Evas_Object *ret = _elm_spinner_add(parent);
   evas_object_smart_callback_add(ret, signals.elm.spinner.changed, _tizen_spinner_changed_hack, NULL);
   return ret;
}
#endif
