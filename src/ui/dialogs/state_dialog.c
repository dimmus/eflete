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
* along with this program; If not, see http://www.gnu.org/licenses/gpl-2.0.html.
*/

#include "state_dialog.h"
#include "common_macro.h"
#include "string_macro.h"


static Evas_Object *entry_name;
static Evas_Object *entry_value;
static Evas_Object *hover_dup_state;
static Eina_Bool to_close;

static Elm_Entry_Filter_Accept_Set accept_value = {
   .accepted = "0123456789.",
   .rejected = NULL
};

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static void
_ok_clicked(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *workspace = ap->workspace;
   Evas_Object *glist = ui_block_state_list_get(ap);
   Part *part = ui_state_list_part_get(glist);
   const char *str_name, *str_value;
   Eina_Stringshare *state;

   if (elm_entry_is_empty(entry_name))
     {
        NOTIFY_WARNING(_("State name can not be empty!"))
        to_close = false;
        return;
     }

   str_name = elm_entry_entry_get(entry_name);
   str_value = elm_entry_entry_get(entry_value);
   if (workspace_edit_object_part_state_add(workspace, part->name,
                                            str_name, atof(str_value)))
     {
        state = eina_stringshare_printf("%s %.2f", str_name, atof(str_value));
        ui_state_list_state_add(glist, state);
        eina_stringshare_del(state);
     }
   to_close = true;
}

static void
_ok_close_clicked(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;

   if (!to_close) return;
   evas_object_del(popup);
}

static void
_cancel_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}


Evas_Object *
state_dialog_add(App_Data *ap)
{
   Evas_Object *popup, *box, *bt_yes, *bt_no;
   Evas_Object *item_name, *item_value, *item_dup;
   Evas_Object *glist = NULL;
   Part *part = NULL;
   Eina_Stringshare *title = NULL;

   if ((!ap) && (!ap->workspace))
     {
        ERR("Failed create state dialog.");
        return NULL;
     }

   glist = ui_block_state_list_get(ap);
   part = ui_state_list_part_get(glist);
   if (!part)
     {
        NOTIFY_INFO(3, _("Please select part"));
        return NULL;
     }

   popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(popup, "eflete");
   title = eina_stringshare_printf(_("Add new state to part \"%s\""), part->name);
   elm_object_part_text_set(popup, "title,text", title);
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(popup, box, false, false);

   ITEM_ADD(box, item_name, _("Name:"))
   ENTRY_ADD(item_name, entry_name, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_name, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry_name, "guide", _("Type a new state name."));
   elm_object_part_content_set(item_name, "elm.swallow.content", entry_name);

   ITEM_ADD(box, item_value, _("Value:"))
   ENTRY_ADD(item_name, entry_value, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_value, elm_entry_filter_accept_set, &accept_value);
   elm_object_part_text_set(entry_value, "guide", _("Type a state value (0.0 - 1.0)."));
   elm_object_part_content_set(item_value, "elm.swallow.content", entry_value);

   ITEM_ADD(box, item_dup, _("Duplicate state:"))
   HOVERSEL_ADD(item_dup, hover_dup_state, false)
   elm_object_text_set(hover_dup_state, _("None"));
   elm_object_disabled_set(hover_dup_state, true);
   elm_object_part_content_set(item_dup, "elm.swallow.content", hover_dup_state);

   elm_box_pack_end(box, item_name);
   elm_box_pack_end(box, item_value);
   elm_box_pack_end(box, item_dup);
   elm_object_content_set(popup, box);

   BUTTON_ADD(popup, bt_yes, _("Add"));
   evas_object_smart_callback_add (bt_yes, "pressed", _ok_clicked, ap);
   evas_object_smart_callback_add (bt_yes, "unpressed", _ok_close_clicked, popup);
   elm_object_part_content_set(popup, "button1", bt_yes);

   BUTTON_ADD(popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, popup);
   elm_object_part_content_set(popup, "button2", bt_no);

   evas_object_show(popup);
   eina_stringshare_del(title);
   return popup;
}
