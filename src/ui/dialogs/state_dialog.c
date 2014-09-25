/**
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

#include "state_dialog.h"
#include "common_macro.h"
#include "string_macro.h"
#include "main_window.h"


static Evas_Object *entry_name;
static Evas_Object *entry_value;
static Eina_Bool to_close;

static const char *state_from;
static double value_from;
static Eina_Bool state_copy;



static Elm_Entry_Filter_Accept_Set accept_value = {
   .accepted = "0123456789.",
   .rejected = NULL
};

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static void
_add_ok_clicked(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *workspace = ap->workspace;
   Evas_Object *glist = ui_block_state_list_get(ap);
   Part *part = ui_states_list_part_get(glist);
   const char *str_name, *str_value;
   Eina_Stringshare *state;
   Eina_Bool result = false;

   if (elm_entry_is_empty(entry_name))
     {
        NOTIFY_WARNING(_("State name can not be empty!"))
        to_close = false;
        return;
     }
   if (!ewe_entry_regex_check(entry_value))
     {
        NOTIFY_WARNING(_("State value is not correct!"));
        to_close = false;
        return;
     }

   str_name = elm_entry_entry_get(entry_name);
   str_value = elm_entry_entry_get(entry_value);
   if (edje_edit_state_exist(ap->project->current_style->obj, part->name,
                             str_name, atof(str_value)))
     {
        NOTIFY_WARNING(_("State is exist!"))
        to_close = false;
        return;
     }

   if (!state_copy)
     result = workspace_edit_object_part_state_add(workspace, part->name,
                                                   str_name, atof(str_value));
   else
     result =  workspace_edit_object_part_state_copy(workspace, part->name,
                                                     state_from, value_from,
                                                     str_name, atof(str_value));

   if (result)
     {
        ap->project->current_style->isModify = true;
        state = eina_stringshare_printf("%s %.2f", str_name, atof(str_value));
        ui_states_list_state_add(glist, state);
        eina_stringshare_del(state);
     }
   to_close = true;
}

static void
_add_ok_close_clicked(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   if (!to_close) return;
   evas_object_del(ap->popup);
   ui_menu_locked_set(ap->menu_hash, false);
}

static void
_cancel_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   evas_object_del(ap->popup);
   ui_menu_locked_set(ap->menu_hash, false);
}

static void
_on_dup_state_change(void *data,
                     Evas_Object *obj EINA_UNUSED,
                     void *ei)
{
   char **split = NULL;
   Part *part = (Part *) data;
   Ewe_Combobox_Item *it = ei;
   const char *state = it->title;
   if (!it->index)
     {
        state_copy = false;
        return;
     }

   split = eina_str_split(state, " ", 2);
   eina_stringshare_del(part->curr_state);
   state_from = eina_stringshare_add(split[0]);
   value_from = atof(split[1]);
   free(split[0]);
   free(split);
   state_copy = true;
}

Evas_Object *
state_dialog_state_add(App_Data *ap)
{
   Evas_Object *box, *item, *bt_yes, *bt_no;
   Evas_Object *item_dup;
   Evas_Object *glist = NULL;
   Part *part = NULL;
   Eina_Stringshare *title = NULL;
   Eina_List *states = NULL, *l = NULL;
   const char *state_name = NULL;
   Evas_Object *combobox_dup_state;

   if ((!ap) || (!ap->workspace))
     {
        ERR("Failed create a add state dialog.");
        return NULL;
     }

   glist = ui_block_state_list_get(ap);
   part = ui_states_list_part_get(glist);
   if (!part) return NULL;

   state_copy = false;

   ap->popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(ap->popup, "eflete");
   title = eina_stringshare_printf(_("Add new state to part \"%s\""), part->name);
   elm_object_part_text_set(ap->popup, "title,text", title);
   elm_popup_orient_set(ap->popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(ap->popup, box, false, false);

   ITEM_ADD(box, item, _("Name:"), "eflete/property/item/default")
   EWE_ENTRY_ADD(item, entry_name, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_name, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry_name, "guide", _("Type a new state name."));
   elm_object_part_content_set(item, "elm.swallow.content", entry_name);
   elm_box_pack_end(box, item);

   ITEM_ADD(box, item, _("Value:"), "eflete/property/item/default")
   EWE_ENTRY_ADD(item, entry_value, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry_value, elm_entry_filter_accept_set, &accept_value);
   elm_object_part_text_set(entry_value, "guide", _("Type a state value (0.0 - 1.0)."));
   ewe_entry_regex_set(entry_value, FLOAT_NUMBER_0_1_REGEX, EWE_REG_EXTENDED);
   ewe_entry_regex_autocheck_set(entry_value, true);
   ewe_entry_regex_glow_set(entry_value, true);
   elm_object_part_content_set(item, "elm.swallow.content", entry_value);
   elm_box_pack_end(box, item);

   ITEM_ADD(box, item_dup, _("Duplicate state:"), "eflete/property/item/default")
   EWE_COMBOBOX_ADD(item_dup, combobox_dup_state)

   states = edje_edit_part_states_list_get(ap->project->current_style->obj, part->name);

   ewe_combobox_item_add(combobox_dup_state, _("None"));
   ewe_combobox_select_item_set(combobox_dup_state, 0);
   EINA_LIST_FOREACH(states, l, state_name)
     {
       ewe_combobox_item_add(combobox_dup_state, state_name);
     }
   elm_object_part_content_set(item_dup, "elm.swallow.content", combobox_dup_state);
   evas_object_smart_callback_add(combobox_dup_state, "selected",
                                  _on_dup_state_change, part);
   edje_edit_string_list_free(states);

   elm_box_pack_end(box, item_dup);
   elm_object_content_set(ap->popup, box);

   BUTTON_ADD(ap->popup, bt_yes, _("Add"));
   evas_object_smart_callback_add (bt_yes, "pressed", _add_ok_clicked, ap);
   evas_object_smart_callback_add (bt_yes, "unpressed", _add_ok_close_clicked, ap);
   elm_object_part_content_set(ap->popup, "button1", bt_yes);

   BUTTON_ADD(ap->popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, ap);
   elm_object_part_content_set(ap->popup, "button2", bt_no);

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_show(ap->popup);
   eina_stringshare_del(title);
   return ap->popup;
}

static void
_del_ok_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   Evas_Object *state_list, *workspace;
   Part *part;
   Eina_Stringshare *state;

   state_list = ui_block_state_list_get(ap);
   workspace = ap->workspace;
   part = ui_states_list_part_get(state_list);
   state = ui_states_list_selected_state_get(state_list);

   char **arr = eina_str_split(state, " ", 2);

   /*TODO: need to check the program, some program can use the given state.
     If given state used in programs - show dialog window with the question:
     'This state used in the program(s). Are you sure you want to delete
     %state name%' and delete the programs or some params from the program */
   if (workspace_edit_object_part_state_del(workspace, part->name, arr[0], atof(arr[1])))
     {
        ap->project->current_style->isModify = true;
        ui_states_list_selected_state_del(state_list);
     }

   ui_menu_locked_set(ap->menu_hash, false);
   free(arr[0]);
   free(arr);
}

Evas_Object *
state_dialog_state_del(App_Data *ap)
{
   Evas_Object *label, *bt_yes, *bt_no;
   Evas_Object *state_list;
   Part *part;
   Eina_Stringshare *state, *title, *message;

   if ((!ap) || (!ap->workspace))
     {
        ERR("Failed create an add state dialog.");
        return NULL;
     }

   state_list = ui_block_state_list_get(ap);
   part = ui_states_list_part_get(state_list);
   if (!part) return NULL;
   state = ui_states_list_selected_state_get(state_list);

   if (!strcmp(state, "default 0.00"))
     {
        NOTIFY_WARNING(_("Can't delete the default state."));
        return NULL;
     }

   ap->popup = elm_popup_add(ap->win_layout);
   elm_object_style_set(ap->popup, "eflete");
   title = eina_stringshare_printf(_("Delete the state from part"));
   elm_object_part_text_set(ap->popup, "title,text", title);
   elm_popup_orient_set(ap->popup, ELM_POPUP_ORIENT_CENTER);

   message = eina_stringshare_printf(_("Do you want to delete the state \"%s\" "
                                       "from part \"%s\"."), state, part->name);
   LABEL_ADD(ap->popup, label, message)
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_label_line_wrap_set(label, ELM_WRAP_CHAR);
   elm_object_content_set(ap->popup, label);

   BUTTON_ADD(ap->popup, bt_yes, _("Delete"));
   evas_object_smart_callback_add (bt_yes, "pressed", _del_ok_clicked, ap);
   evas_object_smart_callback_add (bt_yes, "unpressed", _cancel_clicked, ap);
   elm_object_part_content_set(ap->popup, "button1", bt_yes);

   BUTTON_ADD(ap->popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, ap);
   elm_object_part_content_set(ap->popup, "button2", bt_no);

   ui_menu_locked_set(ap->menu_hash, true);

   evas_object_show(ap->popup);
   eina_stringshare_del(state);
   eina_stringshare_del(message);

   return ap->popup;
}
