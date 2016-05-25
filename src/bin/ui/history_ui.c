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

   assert(hd != NULL);

   /* removing all reverted changes because the were deleted from history */
   ewe_combobox_items_list_free(hd->undo_cmbx, EINA_FALSE);
   ewe_combobox_items_list_free(hd->redo_cmbx, EINA_FALSE);

   hd->to_undo = 0;
   hd->to_redo = 0;
   Eina_List *reverse_redo = NULL;
   EINA_LIST_REVERSE_FOREACH(hd->history->changes, l, change)
     {
        if (!change->reverted)
          {
             hd->to_undo++;
             ewe_combobox_item_add(hd->undo_cmbx, change->description);
          }
        else
          {
             hd->to_redo++;
             reverse_redo = eina_list_append(reverse_redo, change);
          }
     }

   EINA_LIST_REVERSE_FOREACH(reverse_redo, l, change)
     {
        ewe_combobox_item_add(hd->redo_cmbx, change->description);
     }
}

static void
_undo_item_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *ei)
{
   History_New_UI_data *hd = data;
   Ewe_Combobox_Item *it = (Ewe_Combobox_Item *)ei;
   unsigned int i;

   assert(hd != NULL);

   for (i = 0; i <= it->index; i++)
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
   Ewe_Combobox_Item *it = (Ewe_Combobox_Item *)ei;
   unsigned int i;

   assert(hd != NULL);

   for (i = 0; i <= it->index; i++)
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

   EWE_COMBOBOX_ADD(hd->layout, hd->undo_cmbx);
   ewe_combobox_style_set(hd->undo_cmbx, "undo");
   elm_object_part_content_set(hd->layout, "undo", hd->undo_cmbx);
   evas_object_smart_callback_add(hd->undo_cmbx, "selected", _undo_item_selected, hd);

   EWE_COMBOBOX_ADD(hd->layout, hd->redo_cmbx);
   ewe_combobox_style_set(hd->redo_cmbx, "redo");
   elm_object_part_content_set(hd->layout, "redo", hd->redo_cmbx);
   evas_object_smart_callback_add(hd->redo_cmbx, "selected", _redo_item_selected, hd);

   return hd->layout;
}
