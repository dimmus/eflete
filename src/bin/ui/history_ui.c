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
   Evas_Object *genlist;
   Evas_Object *btn_undo_all;
   Evas_Object *btn_clean;
   History *history;
   Group *group;
   Elm_Genlist_Item_Class *itc_change;
   Elm_Genlist_Item *active_item;
} History_UI_data;

static History_UI_data hd;

static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part)
{
   Change * change = data;
   if (!strcmp(part, "elm.text.message"))
     return strdup(change->description);
   return NULL;
}

static Eina_Bool
_item_state_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *state)
{
   Change *change = data;
   Change *current_change;

   assert(change != NULL);

   if ((change->reverted == true) && ((!strcmp(state, "reverted"))))
     return true;
   if (change->reverted == false)
     {
        current_change = eina_list_data_get(hd.history->current_change);
        if ((change != current_change) && (!strcmp(state, "unselected")))
          return true;
        if ((change == current_change) && (!strcmp(state, "selected")))
          return true;
     }
   return false;
}

static void
_item_selected(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *ei)
{
   Elm_Genlist_Item *glit = ei;
   Change *change = elm_object_item_data_get(glit);

   assert(change != NULL);

   if (glit == hd.active_item)
     return;

   if (!change->reverted)
     {
        /* undo */
        while (eina_list_data_get(hd.history->current_change) != change)
          {
             if (!history_undo(hd.history))
               {
                  ERR("Can't undo change. Something is wrong with object");
                  TODO("Add error handling here");
                  abort();
               }
             elm_genlist_item_fields_update(hd.active_item, "reverted", ELM_GENLIST_ITEM_FIELD_STATE);
             hd.active_item = elm_genlist_item_prev_get(hd.active_item);
          }
     }
   else
     {
        /* redo */
        while (eina_list_data_get(hd.history->current_change) != change)
          {
             if (!history_redo(hd.history))
               {
                  ERR("Can't redo change. Something is wrong with object");
                  TODO("Add error handling here");
                  abort();
               }
             elm_genlist_item_fields_update(hd.active_item, "unselected", ELM_GENLIST_ITEM_FIELD_STATE);
             hd.active_item = (hd.active_item != NULL)?
                elm_genlist_item_next_get(hd.active_item):
                elm_genlist_first_item_get(hd.genlist);
          }
     }
   elm_genlist_item_fields_update(hd.active_item, "selected", ELM_GENLIST_ITEM_FIELD_STATE);
   TODO("Add update workspace callback here");

   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
}

static void
_on_change_added(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *ei)
{
   Elm_Genlist_Item *glit, *glit2;
   Change *change = ei;

   /* removing all reverted changes because the were deleted from history */
   glit = elm_genlist_last_item_get(hd.genlist);
   while (glit != hd.active_item)
     {
        glit2 = elm_genlist_item_prev_get(glit);
        elm_object_item_del(glit);
        glit = glit2;
     }

   glit = elm_genlist_item_append(hd.genlist,
                                  hd.itc_change,
                                  change,
                                  NULL,
                                  ELM_GENLIST_ITEM_NONE,
                                  _item_selected,
                                  NULL);

   /* unselect active item */
   elm_genlist_item_fields_update(hd.active_item, "unselected", ELM_GENLIST_ITEM_FIELD_STATE);
   /* making item active before selection allows to skip callback */
   hd.active_item = glit;
   elm_genlist_item_selected_set(glit, true);
}

static void
_history_set(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void * ei)
{
   Group *group = ei;
   Eina_List *l = NULL;
   Change *change;
   Elm_Genlist_Item *it;

   elm_genlist_clear(hd.genlist);
   hd.active_item = NULL;

   hd.history = (group) ? group->history : NULL;
   hd.group = group;
   if (!hd.history)
     return;

   /* */
   EINA_LIST_FOREACH(hd.history->changes, l, change)
     {
        it = elm_genlist_item_append(hd.genlist,
                                     hd.itc_change,
                                     change,
                                     NULL,
                                     ELM_GENLIST_ITEM_NONE,
                                     _item_selected,
                                     NULL);
        if (l == hd.history->current_change)
          {
             /* making item active before selection allows to skip callback */
             hd.active_item = it;
             elm_genlist_item_selected_set(it, true);
          }
     }
}

static void
_undo_shortcut(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void * ei __UNUSED__)
{
   Elm_Genlist_Item *glit;

   if (!hd.history)
     return;

   assert(hd.active_item != NULL);

   glit = elm_genlist_item_prev_get(hd.active_item);
   if (glit)
     elm_genlist_item_selected_set(glit, true);
}

static void
_redo_shortcut(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void * ei __UNUSED__)
{
   Elm_Genlist_Item *glit;

   if (!hd.history)
     return;

   assert(hd.active_item != NULL);

   glit = elm_genlist_item_next_get(hd.active_item);
   if (glit)
     elm_genlist_item_selected_set(glit, true);
}

static void
_on_undo_all(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *ei __UNUSED__)
{
   elm_genlist_item_selected_set(elm_genlist_first_item_get(hd.genlist), true);
}

static void
_on_clean(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *ei __UNUSED__)
{
   Popup_Button btn_res;
   Change *change;

   btn_res = popup_want_action(_("Confirm history clean-up"),
                               _("Are you sure you want to clean history?<br>"
                                 "This action can't be undone."),
                               NULL, NULL, BTN_OK|BTN_CANCEL, NULL, NULL);
   if (BTN_CANCEL == btn_res) return;

   history_del(hd.group->history);
   hd.group->history = history_add(hd.group);

   change = change_add(_("history cleaned"));
   history_change_add(hd.group->history, change);

   _history_set(NULL, NULL, hd.group);
}

Evas_Object *
history_ui_add(void)
{
   Evas_Object *ic;

   assert(ap.win != NULL);

   hd.itc_change = elm_genlist_item_class_new();
   hd.itc_change->item_style = "history";
   hd.itc_change->func.text_get = _item_label_get;
   hd.itc_change->func.state_get = _item_state_get;
   hd.itc_change->func.content_get = NULL;
   hd.itc_change->func.del = NULL;

   hd.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(hd.layout, "layout", "history", "default");

   hd.genlist = elm_genlist_add(hd.layout);
   elm_genlist_mode_set(hd.genlist, ELM_LIST_COMPRESS);
   elm_genlist_homogeneous_set(hd.genlist, true);

   hd.btn_clean = elm_button_add(hd.layout);
   ICON_STANDARD_ADD(hd.btn_clean, ic, true, "delete");
   elm_object_part_content_set(hd.btn_clean, NULL, ic);
   evas_object_smart_callback_add(hd.btn_clean, "clicked", _on_clean, NULL);
   hd.btn_undo_all = elm_button_add(hd.layout);
   elm_object_text_set(hd.btn_undo_all, _("Discard"));
   evas_object_smart_callback_add(hd.btn_undo_all, "clicked", _on_undo_all, NULL);

   elm_layout_content_set(hd.layout, NULL, hd.genlist);
   elm_layout_content_set(hd.layout, "elm.swallow.btn_clean", hd.btn_clean);
   elm_layout_content_set(hd.layout, "elm.swallow.btn_discard", hd.btn_undo_all);

   evas_object_smart_callback_add(ap.win, SIGNAL_HISTORY_CHANGE_ADDED, _on_change_added, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _history_set, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_UNDO, _undo_shortcut, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_REDO, _redo_shortcut, NULL);

   TODO("Add clean-up callbacks here")
   return hd.layout;
}

/* New UI for history module. Based on comboboxes  */
typedef struct {
   Evas_Object *layout;
   Evas_Object *undo_cmbx;
   Evas_Object *redo_cmbx;
   History *history;
   Group *group;
} History_New_UI_data;

static History_New_UI_data hd_new;

static void
_list_update(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *ei __UNUSED__)
{
   Change *change = NULL;//ei;
   Eina_List *l = NULL;

   /* removing all reverted changes because the were deleted from history */
   ewe_combobox_items_list_free(hd_new.undo_cmbx, EINA_FALSE);
   ewe_combobox_items_list_free(hd_new.redo_cmbx, EINA_FALSE);

   char *action = NULL;
   if (data != NULL) action = (char *)data;
   printf("\n%s:%s[%d]: %s List of changes: \n", __FILE__, __func__, __LINE__, action != NULL ? action : "");
   Eina_List *reverse_redo = NULL;
   Change *cur_change = eina_list_data_get(hd_new.history->current_change);
   EINA_LIST_REVERSE_FOREACH(hd_new.history->changes, l, change)
     {
        printf("\t\t%s\"%s\" is %sreverted\n", (change == cur_change) ? "-->" : "",
               change->description, (change->reverted == EINA_FALSE) ? "non" : "");
        if (!change->reverted)
          ewe_combobox_item_add(hd_new.undo_cmbx, change->description);
        else
          reverse_redo = eina_list_append(reverse_redo, change);
     }

   EINA_LIST_REVERSE_FOREACH(reverse_redo, l, change)
     {
        ewe_combobox_item_add(hd_new.redo_cmbx, change->description);
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
        cur_change = eina_list_data_get(hd_new.history->current_change);
        if (!cur_change || cur_change->description == it->title)
          {
             history_undo(hd_new.history);
             break;
          }

        if (!history_undo(hd_new.history))
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
       if (!history_redo(hd_new.history))
          {
             ERR("Can't redo change. Something is wrong with object");
             TODO("Add error handling here");
             abort();
          }
        cur_change = eina_list_data_get(hd_new.history->current_change);
      }
   while (cur_change->description != it->title);
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL);
   _list_update(data, obj, ei);
}

static void
_on_change_undo(void *data, Evas_Object *obj, void *ei)
{
   history_undo(hd_new.history);
   _list_update(data, obj, ei);
}

static void
_on_change_redo(void *data, Evas_Object *obj, void *ei)
{
   history_redo(hd_new.history);
   _list_update(data, obj, ei);
}

static void
_history_set_new(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei)
{
   Group *group = ei;

   ewe_combobox_items_list_free(hd_new.undo_cmbx, EINA_FALSE);
   ewe_combobox_items_list_free(hd_new.redo_cmbx, EINA_FALSE);

   hd_new.history = (group) ? group->history : NULL;
   hd_new.group = group;
}



Evas_Object *
history_ui_add_2(void)
{
   assert(ap.win != NULL);

   hd_new.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(hd_new.layout, "layout", "history", "new");

   EWE_COMBOBOX_ADD(hd_new.layout, hd_new.undo_cmbx);
   ewe_combobox_style_set(hd_new.undo_cmbx, "undo");

   elm_object_part_content_set(hd_new.layout, "undo", hd_new.undo_cmbx);
   evas_object_smart_callback_add(hd_new.undo_cmbx, "selected", _undo_item_selected, NULL);

   EWE_COMBOBOX_ADD(hd_new.layout, hd_new.redo_cmbx);
   ewe_combobox_text_set(hd_new.redo_cmbx, "R");
   elm_object_part_content_set(hd_new.layout, "redo", hd_new.redo_cmbx);
   evas_object_smart_callback_add(hd_new.redo_cmbx, "selected", _redo_item_selected, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_HISTORY_CHANGE_ADDED, _list_update, strdup("Add"));
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _history_set_new, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_UNDO, _on_change_undo, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_REDO, _on_change_redo, NULL);

   return hd_new.layout;
}
