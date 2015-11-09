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
#include "signals.h"

typedef struct {
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *btn_undo_all;
   Evas_Object *btn_clean;
   History_ *history;
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
             if (!history_undo_(hd.history))
               {
                  ERR("Can't undo change. Something is wrong with object");
                  TODO("Add error handling here");
                  abort();
               }
             hd.active_item = elm_genlist_item_prev_get(hd.active_item);
          }
     }
   else
     {
        /* redo */
        while (eina_list_data_get(hd.history->current_change) != change)
          {
             if (!history_redo_(hd.history))
               {
                  ERR("Can't redo change. Something is wrong with object");
                  TODO("Add error handling here");
                  abort();
               }
             hd.active_item = (hd.active_item != NULL)?
                elm_genlist_item_next_get(hd.active_item):
                elm_genlist_first_item_get(hd.genlist);
          }
     }
   elm_genlist_realized_items_update(hd.genlist);
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

   hd.btn_clean = elm_button_add(hd.layout);
   ICON_STANDARD_ADD(hd.btn_clean, ic, true, "delete");
   elm_object_part_content_set(hd.btn_clean, NULL, ic);
   hd.btn_undo_all = elm_button_add(hd.layout);
   elm_object_text_set(hd.btn_undo_all, _("Discard"));

   elm_layout_content_set(hd.layout, NULL, hd.genlist);
   elm_layout_content_set(hd.layout, "elm.swallow.btn_clean", hd.btn_clean);
   elm_layout_content_set(hd.layout, "elm.swallow.btn_discard", hd.btn_undo_all);

   evas_object_smart_callback_add(ap.win, SIGNAL_HISTORY_CHANGE_ADDED, _on_change_added, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_TAB_CHANGED, _history_set, NULL);

   TODO("Add clean-up callbacks here")
   return hd.layout;
}
