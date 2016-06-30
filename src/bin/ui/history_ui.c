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

#include "history_ui.h"
#include "change.h"
#include "project_manager.h"
#include "history.h"
#include "widget_macro.h"
#include "main_window.h"

typedef struct {
   Evas_Object *layout;
   Evas_Object *undo_cmbx;
   Evas_Object *redo_cmbx;
   History *history;
   Group *group;
   int to_undo;
   int to_redo;
} History_New_UI_data;

#define HISTORY_DATA "history_data"
#define HISTORY_DATA_GET(OBJ) \
   History_New_UI_data *hd = evas_object_data_get(OBJ, HISTORY_DATA); \
   assert(hd != NULL);

static void
_list_update(History_New_UI_data *hd)
{
   Change *change = NULL;
   Eina_List *l = NULL;
   unsigned int i = 0;
   Elm_Object_Item *hoversel_it;

   assert(hd != NULL);

   /* removing all reverted changes because the were deleted from history */
   elm_hoversel_clear(hd->undo_cmbx);
   elm_hoversel_clear(hd->redo_cmbx);

   hd->to_undo = 0;
   hd->to_redo = 0;
   Eina_List *reverse_redo = NULL;
   EINA_LIST_REVERSE_FOREACH(hd->history->changes, l, change)
     {
        if (!change->reverted)
          {
             hd->to_undo++;
             hoversel_it = elm_hoversel_item_add(hd->undo_cmbx, change->description, NULL, ELM_ICON_NONE, NULL, NULL);
             evas_object_data_set(hoversel_it, "number", (void*)(uintptr_t)i);
             printf("UNDO\n");
             i++;
          }
        else
          {
             hd->to_redo++;
             reverse_redo = eina_list_append(reverse_redo, change);
          }
     }

   i = 0;
   EINA_LIST_REVERSE_FOREACH(reverse_redo, l, change)
     {
        hoversel_it = elm_hoversel_item_add(hd->redo_cmbx, change->description, NULL, ELM_ICON_NONE, NULL, NULL);
        evas_object_data_set(hoversel_it, "number", (void*)(uintptr_t)i);
             printf("REDO\n");
        i++;
     }
}

static void
_undo_item_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *ei)
{
   History_New_UI_data *hd = data;
   int i;
   int index = (int)(uintptr_t) (evas_object_data_get(ei, "number"));

   assert(hd != NULL);

   for (i = 0; i <= index; i++)
     CRIT_ON_FAIL(history_undo(hd->history));

   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   _list_update(hd);
}

static void
_redo_item_selected(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *ei)
{
   History_New_UI_data *hd = data;
   int i;
   int index = (int)(uintptr_t) (evas_object_data_get(ei, "number"));

   assert(hd != NULL);

   for (i = 0; i <= index; i++)
     CRIT_ON_FAIL(history_redo(hd->history));

   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   _list_update(hd);
}

void
history_ui_undo(Evas_Object *obj)
{
   HISTORY_DATA_GET(obj);
   if (hd->to_undo == 0) return;
   history_undo(hd->history);
   _list_update(hd);
}

void
history_ui_redo(Evas_Object *obj)
{
   HISTORY_DATA_GET(obj);
   if (hd->to_redo == 0) return;
   history_redo(hd->history);
   _list_update(hd);
}

void
history_ui_update(Evas_Object *obj)
{
   HISTORY_DATA_GET(obj);
   _list_update(hd);
}

static void
_btn_undo_cb(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *eo __UNUSED__)
{
   History_New_UI_data *hd = (History_New_UI_data *) data;
   history_ui_undo(hd->layout);
}

static void
_btn_redo_cb(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *eo __UNUSED__)
{
   History_New_UI_data *hd = (History_New_UI_data *) data;
   history_ui_redo(hd->layout);
}


Evas_Object *
history_ui_add(Evas_Object *parent, History *history)
{
   assert(parent != NULL);
   assert(history != NULL);

   History_New_UI_data *hd = mem_calloc(1, sizeof(History_New_UI_data));
   hd->layout = elm_layout_add(parent);
   evas_object_data_set(hd->layout, HISTORY_DATA, hd);
   hd->history = history;
   elm_layout_theme_set(hd->layout, "layout", "history", "combobox");
   evas_object_show(hd->layout);

   Evas_Object *btn = elm_button_add(hd->layout);
   elm_object_style_set(btn, "undo");
   evas_object_show(btn);
   evas_object_smart_callback_add(btn, "clicked", _btn_undo_cb, hd);

   Evas_Object *undo_layout = elm_layout_add(hd->layout);
   elm_layout_theme_set(undo_layout, "layout", "history", "control");
   evas_object_show(undo_layout);
   elm_object_part_content_set(undo_layout, "button", btn);

   hd->undo_cmbx = elm_hoversel_add(hd->layout);
   elm_hoversel_hover_parent_set(hd->undo_cmbx, ap.win);
   evas_object_size_hint_weight_set(hd->undo_cmbx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(hd->undo_cmbx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_style_set(hd->undo_cmbx, "history");
   evas_object_smart_callback_add(hd->undo_cmbx, "selected", _undo_item_selected, hd);
   evas_object_show(hd->undo_cmbx);
   elm_object_part_content_set(undo_layout, "arrow", hd->undo_cmbx);
   elm_object_part_content_set(hd->layout, "undo", undo_layout);

   btn = elm_button_add(hd->layout);
   elm_object_style_set(btn, "redo");
   evas_object_show(btn);
   evas_object_smart_callback_add(btn, "clicked", _btn_redo_cb, hd);

   Evas_Object *redo_layout = elm_layout_add(hd->layout);
   elm_layout_theme_set(redo_layout, "layout", "history", "control");
   evas_object_show(redo_layout);
   elm_object_part_content_set(redo_layout, "button", btn);

   hd->redo_cmbx = elm_hoversel_add(hd->layout);
   elm_hoversel_hover_parent_set(hd->redo_cmbx, ap.win);
   evas_object_size_hint_weight_set(hd->redo_cmbx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(hd->redo_cmbx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_style_set(hd->redo_cmbx, "history");
   evas_object_smart_callback_add(hd->redo_cmbx, "selected", _redo_item_selected, hd);
   evas_object_show(hd->redo_cmbx);
   elm_object_part_content_set(redo_layout, "arrow", hd->redo_cmbx);
   elm_object_part_content_set(hd->layout, "redo", redo_layout);

   return hd->layout;
}
