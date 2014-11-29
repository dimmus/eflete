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

#include "main_window.h"

struct _Splash_Data
{
   Evas_Object *win;
   Splash_Cb setup;
   Splash_Cb teardown;
   void *data;
};

typedef struct _Splash_Data Splash_Data;

Splash_Data sdata;

static void
_on_teardown(void *data __UNUSED__,
             Evas_Object *obj,
             const char *emission __UNUSED__,
             const char *source __UNUSED__)
{
   if (sdata.teardown) sdata.teardown(sdata.data);
   evas_object_del(obj);
}

static void
_on_splash_close(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   elm_layout_signal_emit(sdata.win, "end", "eflete");
}

static void
_on_setup(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          const char *emission __UNUSED__,
          const char *source __UNUSED__)
{
   if (sdata.setup) sdata.setup(sdata.data);
}

Evas_Object *
splash_add(Evas_Object *parent, Splash_Cb setup, Splash_Cb teardown, void *data)
{
   Evas_Object *bt;

   sdata.setup = setup;
   sdata.teardown = teardown;
   sdata.data = data;

   sdata.win = elm_win_inwin_add(parent);
   elm_object_style_set(sdata.win, "splash");

   BUTTON_ADD(sdata.win, bt, "Cancel");
   elm_object_content_set(sdata.win, bt);
   evas_object_smart_callback_add(bt, "clicked", _on_splash_close, NULL);

   elm_layout_signal_callback_add(sdata.win, "setup", "eflete", _on_setup, NULL);
   elm_layout_signal_callback_add(sdata.win, "teardown", "eflete", _on_teardown, NULL);

   return sdata.win;
}

void
splash_del(Evas_Object *obj)
{
   elm_layout_signal_emit(obj, "end", "eflete");
}
