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

#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "ui_states_list.h"
#include "string_common.h"

typedef struct {
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *btn_add;
   Evas_Object *btn_del;
   Part *part; /* selected part in the widget list */
   struct {
      Evas_Object *popup;
      Evas_Object *entry_name;
      Evas_Object *spinner_value;
      Evas_Object *combobox_dup;
      Evas_Object *btn_add;
      Elm_Validator_Regexp *name_validator;
   } popup;
} State_List;

static State_List sl;
static State_Data sd;
static Elm_Genlist_Item_Class *_itc_state = NULL;

static void
_popup_close(void)
{
   App_Data *ap = app_data_get();
   if (sl.popup.name_validator)
     {
        elm_validator_regexp_free(sl.popup.name_validator);
        sl.popup.name_validator = NULL;
     }
   evas_object_del(sl.popup.popup);
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_close_cb(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   _popup_close();
}

static void
_del_cb(void *data __UNUSED__,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   Elm_Object_Item *eoi, *next_eoi;

   eoi = elm_genlist_selected_item_get(sl.genlist);
   assert(eoi != NULL);

   sd.part = sl.part;
   sd.state = elm_object_item_data_get(eoi);
   sd.value = -1;
   sd.duplicate_state = NULL;
   evas_object_smart_callback_call(sl.layout, "stl,state,del", &sd);
   next_eoi = elm_genlist_item_next_get(sl.genlist);
   if (!next_eoi) next_eoi = elm_genlist_item_prev_get(eoi);
   elm_genlist_item_selected_set(next_eoi, true);

   elm_object_item_del(eoi);
   _popup_close();
}

static void
_btn_del_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Evas_Object *bt_no;
   Eina_Stringshare *message;
   App_Data *ap;

   ap = app_data_get();
   assert(ap != NULL);

   sl.popup.popup = elm_popup_add(ap->win);
   elm_object_part_text_set(sl.popup.popup, "title,text", _("Delete state"));

   message = eina_stringshare_printf(_("Do you want to delete the state \"%s %f from part \"%s\"?"),
                                     sl.part->curr_state, sl.part->curr_state_value, sl.part->name);
   LABEL_ADD(sl.popup.popup, sl.popup.entry_name, message)
   evas_object_size_hint_weight_set(sl.popup.entry_name, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(sl.popup.entry_name, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_label_line_wrap_set(sl.popup.entry_name, ELM_WRAP_CHAR);
   elm_object_content_set(sl.popup.popup, sl.popup.entry_name);

   BUTTON_ADD(sl.popup.popup, sl.popup.btn_add, _("Delete"));
   evas_object_smart_callback_add (sl.popup.btn_add, "clicked", _del_cb, NULL);
   elm_object_part_content_set(sl.popup.popup, "button1", sl.popup.btn_add);

   BUTTON_ADD(sl.popup.popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _close_cb, NULL);
   elm_object_part_content_set(sl.popup.popup, "button2", bt_no);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(sl.popup.popup);
   elm_object_focus_set(sl.popup.entry_name, true);
}

static void
_add_cb(void *data __UNUSED__,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   Ewe_Combobox_Item *item;
   Elm_Object_Item *eoi;
   Eina_Stringshare *state_str;

   sd.part = sl.part;
   sd.state = elm_entry_entry_get(sl.popup.entry_name);
   sd.value = elm_spinner_value_get(sl.popup.spinner_value);
   item = ewe_combobox_select_item_get(sl.popup.combobox_dup);
   if (!strcmp(item->title, _("None")))
     sd.duplicate_state = NULL;
   else
     sd.duplicate_state = item->title;

   state_str = eina_stringshare_printf("%s %1.2f", sd.state, sd.value);
   eoi = elm_genlist_item_append(sl.genlist, _itc_state, state_str, NULL,
                                 ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_object_item_data_set(eoi, (void *)state_str);

   evas_object_smart_callback_call(sl.layout, "stl,state,add", &sd);
   /* select item after it created, because on select event called signal to
    * chage the selected state on workspace */
   elm_genlist_item_selected_set(eoi, true);
   _popup_close();
}

static void
_validate(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   if ((elm_validator_regexp_status_get(sl.popup.name_validator) != ELM_REG_NOERROR))
     elm_object_disabled_set(sl.popup.btn_add, true);
   else
     elm_object_disabled_set(sl.popup.btn_add, false);
}

static void
_btn_add_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Evas_Object *box, *item, *bt_no;
   Eina_Stringshare *title;
   Eina_List *states = NULL, *l = NULL;
   const char *state_name = NULL;
   App_Data *ap;

   ap = app_data_get();
   assert(ap != NULL);

   sl.popup.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   sl.popup.popup = elm_popup_add(ap->win);
   title = eina_stringshare_printf(_("Add new state to part \"%s\""), sl.part->name);
   elm_object_part_text_set(sl.popup.popup, "title,text", title);

   BOX_ADD(sl.popup.popup, box, false, false);
   LAYOUT_PROP_ADD(box, _("Name:"), "property", "1swallow")
   ENTRY_ADD(item, sl.popup.entry_name, true);
   eo_do(sl.popup.entry_name, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE,
         elm_validator_regexp_helper, sl.popup.name_validator));
   evas_object_smart_callback_add(sl.popup.entry_name, "changed", _validate, NULL);
   elm_object_part_text_set(sl.popup.entry_name, "guide", _("Type a new state name"));
   elm_object_part_content_set(item, "elm.swallow.content", sl.popup.entry_name);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Value:"), "property", "1swallow_subtext")
   SPINNER_ADD(item, sl.popup.spinner_value, 0.0, 1.0, 0.1, true);
   elm_spinner_label_format_set(sl.popup.spinner_value, "%1.2f");
   elm_object_part_text_set(item, "elm.subtext", _("Available values: 0.0 - 1.0"));
   evas_object_smart_callback_add(sl.popup.spinner_value, "changed", _validate, NULL);
   elm_object_part_content_set(item, "elm.swallow.content", sl.popup.spinner_value);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Duplicate state:"), "property", "1swallow")
   EWE_COMBOBOX_ADD(item, sl.popup.combobox_dup)
   states = edje_edit_part_states_list_get(ap->project->current_style->obj, sl.part->name);

   ewe_combobox_item_add(sl.popup.combobox_dup, _("None"));
   ewe_combobox_select_item_set(sl.popup.combobox_dup, 0);
   EINA_LIST_FOREACH(states, l, state_name)
     {
       ewe_combobox_item_add(sl.popup.combobox_dup, state_name);
     }
   elm_object_part_content_set(item, "elm.swallow.content", sl.popup.combobox_dup);
   edje_edit_string_list_free(states);

   elm_box_pack_end(box, item);
   elm_object_content_set(sl.popup.popup, box);

   BUTTON_ADD(sl.popup.popup, sl.popup.btn_add, _("Add"));
   evas_object_smart_callback_add (sl.popup.btn_add, "clicked", _add_cb, NULL);
   elm_object_part_content_set(sl.popup.popup, "button1", sl.popup.btn_add);
   elm_object_disabled_set(sl.popup.btn_add, true);

   BUTTON_ADD(sl.popup.popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _close_cb, NULL);
   elm_object_part_content_set(sl.popup.popup, "button2", bt_no);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(sl.popup.popup);
   elm_object_focus_set(sl.popup.entry_name, true);

   eina_stringshare_del(title);
}

static char *
_item_state_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   return strdup(data);
}

static void
_on_state_selected(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *eoi = (Elm_Object_Item *)event_info;

   sd.part = sl.part;
   sd.state = elm_object_item_data_get(eoi);
   sd.value = -1;
   sd.duplicate_state = NULL;
   evas_object_smart_callback_call(sl.layout, "stl,state,select", &sd);
   if (!strncmp(sd.state, "default 0.00", strlen("default 0.00")))
     elm_object_disabled_set(sl.btn_del, true);
   else
     elm_object_disabled_set(sl.btn_del, false);
}

Evas_Object *
ui_states_list_add(Evas_Object *parent)
{
   Evas_Object *ic;

   assert(parent != NULL);
   TODO("add delete data on EVAS_OBJECT_DEL event")

   sl.layout = elm_layout_add(parent);
   elm_layout_theme_set(sl.layout, "layout", "state_list", "layout");
   evas_object_size_hint_align_set(sl.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(sl.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(sl.layout);

   sl.genlist = elm_genlist_add(sl.layout);
   evas_object_size_hint_align_set(sl.genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(sl.genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(sl.genlist, "selected", _on_state_selected, NULL);
   elm_object_disabled_set(sl.genlist, true);
   evas_object_show(sl.genlist);

   if (!_itc_state)
     {
        _itc_state = elm_genlist_item_class_new();
        _itc_state->item_style = "default";
        _itc_state->func.text_get = _item_state_label_get;
     }

   sl.btn_add = elm_button_add(sl.layout);
   elm_object_style_set(sl.btn_add, "anchor");
   elm_object_disabled_set(sl.btn_add, true);
   evas_object_smart_callback_add(sl.btn_add, "clicked", _btn_add_cb, NULL);
   ICON_STANDARD_ADD(sl.btn_add, ic, true, "plus");
   elm_object_part_content_set(sl.btn_add, NULL, ic);
   elm_object_disabled_set(sl.btn_add, true);
   evas_object_show(sl.btn_add);

   sl.btn_del = elm_button_add(sl.layout);
   elm_object_style_set(sl.btn_del, "anchor");
   elm_object_disabled_set(sl.btn_del, true);
   evas_object_smart_callback_add(sl.btn_del, "clicked", _btn_del_cb, NULL);
   ICON_STANDARD_ADD(sl.btn_del, ic, true, "minus");
   elm_object_part_content_set(sl.btn_del, NULL, ic);
   evas_object_show(sl.btn_add);

   elm_layout_content_set(sl.layout, NULL, sl.genlist);
   elm_layout_content_set(sl.layout, "elm.swallow.btn_add", sl.btn_add);
   elm_layout_content_set(sl.layout, "elm.swallow.btn_del", sl.btn_del);
   return sl.layout;
}

static void
_item_del(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   Elm_Object_Item *item = (Elm_Object_Item *)event_info;
   Eina_Stringshare *state = elm_object_item_data_get(item);
   eina_stringshare_del(state);
}

Eina_Bool
ui_states_list_data_set(Evas_Object *obj, Style *style, Part *part)
{
   Eina_List *states, *l;
   const char *state_name;
   Eina_Stringshare *state, *curr_state;
   Elm_Object_Item *stit;

   /* We must be sure that only one State list is exist */
   assert(obj == sl.layout);
   assert(style != NULL);
   assert(part != NULL);

   elm_genlist_clear(sl.genlist);
   elm_object_disabled_set(sl.genlist, false);
   evas_object_show(sl.layout);
   elm_object_disabled_set(sl.btn_add, false);
   elm_object_disabled_set(sl.btn_del, false);

   states = edje_edit_part_states_list_get(style->obj, part->name);
   sl.part = part;
   curr_state = eina_stringshare_printf("%s %1.2f", part->curr_state, part->curr_state_value);
   EINA_LIST_FOREACH(states, l, state_name)
     {
        state = eina_stringshare_add(state_name);
        stit = elm_genlist_item_append(sl.genlist, _itc_state, state, NULL,
                                       ELM_GENLIST_ITEM_NONE, NULL, NULL);
        elm_object_item_data_set(stit, (void *)state);
        elm_object_item_del_cb_set(stit, _item_del);
        if (strcmp(state, curr_state) == 0)
          elm_genlist_item_selected_set(stit, true);
     }
   edje_edit_string_list_free(states);

   eina_stringshare_del(curr_state);
   return true;
}

Eina_Bool
ui_states_list_data_unset(Evas_Object *obj)
{
   assert(obj == sl.layout);

   elm_object_disabled_set(sl.btn_add, true);
   elm_object_disabled_set(sl.btn_del, true);
   elm_genlist_clear(sl.genlist);
   elm_object_disabled_set(sl.genlist, true);
   sl.part = NULL;

   return true;
}

Part *
ui_states_list_part_get(Evas_Object *obj)
{
   assert(obj != sl.layout);
   return sl.part;
}

Eina_Stringshare *
ui_states_list_selected_state_get(Evas_Object *obj)
{
   Evas_Object *gl;
   Elm_Object_Item *eoi;
   Eina_Stringshare *state;

   assert(obj != NULL);

   gl = evas_object_data_get(obj, "st_gl");
   eoi = elm_genlist_selected_item_get(gl);

   if (!eoi) return NULL;
   state = eina_stringshare_add(elm_object_item_data_get(eoi));

   return state;
}
