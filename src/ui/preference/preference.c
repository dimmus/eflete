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

#include "preference.h"
#include "main_window.h"

typedef struct _Preferences Preferences;

struct _Preferences
{
   Project *pr;
   Evas_Object *mwin;
};

static void
_on_edit_preferences_exit(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Preferences *preference = (Preferences *)data;
   evas_object_del(preference->mwin);
}

static void
_on_preferences_close(void *data,
                      Evas *e __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Preferences *preference = (Preferences *)data;
   free(preference);
}

static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_locked_set(ap->menu_hash, false);
}

Evas_Object *
preferences_window_add(Project *project)
{
   Evas_Object *panes;
   Evas_Object *window_layout, *button_box, *btn;
   Preferences *preference = NULL;

   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   preference = (Preferences *)mem_calloc(1, sizeof(Preferences));

   preference->pr = project;
   preference->mwin = mw_add(_on_edit_preferences_exit, preference);
   mw_title_set(preference->mwin, _("Preferences"));
   evas_object_event_callback_add(preference->mwin, EVAS_CALLBACK_FREE,
                                  _on_preferences_close, preference);
   window_layout = elm_layout_add(preference->mwin);
   elm_layout_file_set(window_layout, EFLETE_EDJ, "eflete/editor/default");
   elm_win_inwin_content_set(preference->mwin, window_layout);

   panes = elm_panes_add(preference->mwin);
   elm_object_style_set(panes, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.2);
   elm_object_part_content_set(window_layout, "eflete.swallow.content", panes);
   evas_object_show(panes);

   BOX_ADD(window_layout, button_box, true, false)
   elm_box_align_set(button_box, 1.0, 0.5);

   BUTTON_ADD(preference->mwin, btn, _("Close"));
   evas_object_smart_callback_add(btn, "clicked", _on_edit_preferences_exit, preference);
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_size_hint_min_set(btn, 100, 30);
   evas_object_show(btn);
   elm_box_pack_end(button_box, btn);
   elm_object_part_content_set(window_layout, "eflete.swallow.button_box", button_box);

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_event_callback_add(preference->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(preference->mwin);
   return preference->mwin;
}
