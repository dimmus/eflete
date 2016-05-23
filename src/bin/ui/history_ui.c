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
} History_New_UI_data;

static History_New_UI_data hd;

static void
_list_update(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *ei __UNUSED__)
{
   Change *change = NULL;
   Eina_List *l = NULL;

   /* removing all reverted changes because the were deleted from history */
   ewe_combobox_items_list_free(hd.undo_cmbx, EINA_FALSE);
   ewe_combobox_items_list_free(hd.redo_cmbx, EINA_FALSE);

   Eina_List *reverse_redo = NULL;
   EINA_LIST_REVERSE_FOREACH(hd.history->changes, l, change)
     {
        if (!change->reverted)
          ewe_combobox_item_add(hd.undo_cmbx, change->description);
        else
          reverse_redo = eina_list_append(reverse_redo, change);
     }

   EINA_LIST_REVERSE_FOREACH(reverse_redo, l, change)
     {
        ewe_combobox_item_add(hd.redo_cmbx, change->description);
     }
}

static void
_undo_item_selected(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *ei)
{
   Ewe_Combobox_Item *it = (Ewe_Combobox_Item *)ei;
   Change *cur_change = NULL;

   while (true)
     {
        cur_change = eina_list_data_get(hd.history->current_change);
        if (!cur_change || cur_change->description == it->title)
          {
             history_undo(hd.history);
             break;
          }

        if (!history_undo(hd.history))
          {
             ERR("Can't undo change. Something is wrong with object");
             TODO("Add error handling here");
             abort();
          }
     }
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   _list_update(data, obj, ei);
}

static void
_redo_item_selected(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *ei)
{
   Ewe_Combobox_Item *it = (Ewe_Combobox_Item *)ei;
   Change *cur_change = NULL;

   do
     {
       if (!history_redo(hd.history))
          {
             ERR("Can't redo change. Something is wrong with object");
             TODO("Add error handling here");
             abort();
          }
        cur_change = eina_list_data_get(hd.history->current_change);
      }
   while (cur_change->description != it->title);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   _list_update(data, obj, ei);
}

static void
_on_change_undo(void *data, Evas_Object *obj, void *ei)
{
   history_undo(hd.history);
   _list_update(data, obj, ei);
}

static void
_on_change_redo(void *data, Evas_Object *obj, void *ei)
{
   history_redo(hd.history);
   _list_update(data, obj, ei);
}

static void
_history_set_new(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei)
{
   Group *group = ei;

   ewe_combobox_items_list_free(hd.undo_cmbx, EINA_FALSE);
   ewe_combobox_items_list_free(hd.redo_cmbx, EINA_FALSE);

   hd.history = (group) ? group->history : NULL;
   hd.group = group;
}

Evas_Object *
history_ui_add(void)
{
   assert(ap.win != NULL);

   hd.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(hd.layout, "layout", "history", "combobox");

   EWE_COMBOBOX_ADD(hd.layout, hd.undo_cmbx);
   ewe_combobox_style_set(hd.undo_cmbx, "undo");
   elm_object_part_content_set(hd.layout, "undo", hd.undo_cmbx);
   evas_object_smart_callback_add(hd.undo_cmbx, "selected", _undo_item_selected, NULL);

   EWE_COMBOBOX_ADD(hd.layout, hd.redo_cmbx);
   ewe_combobox_style_set(hd.redo_cmbx, "redo");
   elm_object_part_content_set(hd.layout, "redo", hd.redo_cmbx);
   evas_object_smart_callback_add(hd.redo_cmbx, "selected", _redo_item_selected, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_HISTORY_CHANGE_ADDED, _list_update, strdup("Add"));
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _history_set_new, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_UNDO, _on_change_undo, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_REDO, _on_change_redo, NULL);

   return hd.layout;
}
