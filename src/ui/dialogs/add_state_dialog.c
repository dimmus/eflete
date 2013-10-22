/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#include "add_state_dialog.h"

#define STADD_LIST_KEY "state_list_key"

static Evas_Object *entry_name;
static Evas_Object *entry_value;

static void
_ok_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *groupspace = (Evas_Object *)data;
   Evas_Object *glist = evas_object_data_del(obj, STADD_LIST_KEY);
   Part *part = ui_state_list_part_get(glist);
   Group *group = ui_groupspace_group_get(groupspace);
   double value = 0;
   char state_name[BUFF_MAX];
   snprintf(state_name, sizeof(state_name), "%s", elm_entry_entry_get(entry_name));

   const char *str_value = elm_entry_entry_get(entry_value);

   value = atof(str_value);
   if(ui_groupspace_part_state_add(groupspace, part, state_name, value))
     ui_states_list_data_set(glist, group, part);
}

static void
_cancel_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *mwin = (Evas_Object *)data;
   evas_object_del(mwin);
}

Evas_Object *
new_state_dialog_add(App_Data *ap)
{
   Evas_Object *popup, *box, *bt_yes, *bt_no;
   Evas_Object *style_box, *class_box, *label;
   Evas_Object *glist = ui_block_state_list_get(ap);
   Evas_Object *groupspace = ap->ws->groupspace;

   if (!ap)
     {
        ERR("Failed create state dialog.");
        return NULL;
     }

   if (!ui_state_list_part_get(glist))
     {
        NOTIFY_INFO(3, "Please select part");
        return NULL;
     }


   popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(popup, "eflete/popup");
   elm_object_part_text_set(popup, "title,text", "Add new state:");
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(popup, box, EINA_FALSE, EINA_FALSE);

   BOX_ADD(box, style_box, EINA_TRUE, EINA_FALSE);
   LABEL_ADD(style_box, label, "Name: ");
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(style_box, label);
   ENTRY_ADD(style_box, entry_name, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_entry_set(entry_name, "new_state");
   elm_box_pack_end(style_box, entry_name);

   BOX_ADD(box, class_box, EINA_TRUE, EINA_FALSE);
   LABEL_ADD(class_box, label, "Value: ");
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(class_box, label);
   ENTRY_ADD(class_box, entry_value, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_entry_set(entry_value, "0.0");
   elm_box_pack_end(class_box, entry_value);

   elm_box_pack_end(box, style_box);
   elm_box_pack_end(box, class_box);
   elm_object_content_set(popup, box);

   BUTTON_ADD(popup, bt_yes, "Add");
   evas_object_data_set(bt_yes, STADD_LIST_KEY, glist);
   evas_object_smart_callback_add (bt_yes, "pressed", _ok_clicked, groupspace);
   evas_object_smart_callback_add (bt_yes, "unpressed", _cancel_clicked, popup);
   elm_object_part_content_set(popup, "button1", bt_yes);

   BUTTON_ADD(popup, bt_no, "Cancel");
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, popup);
   elm_object_part_content_set(popup, "button2", bt_no);

   evas_object_show(popup);
   return popup;
}

