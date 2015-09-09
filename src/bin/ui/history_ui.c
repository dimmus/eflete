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

typedef struct {
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *btn_undo_all;
   Evas_Object *btn_clean;
   History_ *history;
   Elm_Genlist_Item_Class *itc_change;
   Elm_Genlist_Item_Class *itc_change_reverted;
} History_UI_data;

static History_UI_data hd;

Evas_Object *
history_ui_add(void)
{
   Evas_Object *ic;

   assert(ap.win != NULL);

   hd.itc_change = elm_genlist_item_class_new();
   hd.itc_change->item_style = "history";
   hd.itc_change->func.text_get = NULL;
   hd.itc_change->func.content_get = NULL;
   hd.itc_change->func.del = NULL;

   hd.itc_change_reverted = elm_genlist_item_class_new();
   hd.itc_change_reverted->item_style = "history_inactive";
   hd.itc_change_reverted->func.text_get = NULL;
   hd.itc_change_reverted->func.content_get = NULL;
   hd.itc_change_reverted->func.del = NULL;

   hd.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(hd.layout, "layout", "history", "default");

   hd.genlist = elm_genlist_add(hd.layout);

   hd.btn_clean = elm_button_add(hd.layout);
   ICON_STANDARD_ADD(hd.btn_clean, ic, true, "delete");
   elm_object_part_content_set(hd.btn_clean, NULL, ic);
   hd.btn_undo_all = elm_button_add(hd.layout);
   elm_object_text_set(hd.btn_undo_all, _("Discard"));

   elm_layout_content_set(hd.layout, NULL, hd.genlist);
   elm_layout_content_set(hd.layout, "elm.swallow.btn_clean", hd.btn_clean);
   elm_layout_content_set(hd.layout, "elm.swallow.btn_discard", hd.btn_undo_all);

   TODO("Add clean-up callbacks here")
   evas_object_show(hd.layout);
   return hd.layout;
}
