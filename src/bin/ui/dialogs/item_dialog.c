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

#include "item_dialog.h"

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static void
_job_popup_close(void *data)
{
   App_Data *ap = (App_Data *)data;

   evas_object_del(ap->popup);
   ap->popup = NULL;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_on_button_add_clicked(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   /*TODO: not implemented yet*/
}

static void
_on_button_cancel_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   ecore_job_add(_job_popup_close, data);
}

static void
_on_item_source_change(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   /*TODO: not implemented yet */
}

Evas_Object *
item_dialog_add(App_Data *ap, Part *part)
{
   Evas_Object *box, *item, *button;
   Eina_List *collections = NULL, *l = NULL;
   Eina_Stringshare *group = NULL;
   Evas_Object *combobox_source;
   Evas_Object *entry;
   Evas_Object *markup = NULL;

   if ((!ap) || (!ap->workspace) || (!part))
     {
        ERR("Wrong input params: ap[%p] workspace[%p] part[%p]", ap, ap?ap->workspace:NULL, part)
        return NULL;
     }

   ap->popup = elm_popup_add(ap->win_layout);
   elm_object_part_text_set(ap->popup, "title,text", _("Add new item"));
   elm_popup_orient_set(ap->popup, ELM_POPUP_ORIENT_CENTER);

   markup = elm_layout_add(ap->popup);
   elm_layout_theme_set(markup, "layout", "dialog", "item");
   evas_object_size_hint_weight_set(markup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(markup);

   BOX_ADD(ap->popup, box, false, false);

   ITEM_ADD(box, item, _("Name:"), "eflete/property/item/small_label")
   EWE_ENTRY_ADD(item, entry, true);
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry, "guide", _("Type a item name."));
   elm_object_part_content_set(item, "elm.swallow.content", entry);
   elm_box_pack_end(box, item);

   ITEM_ADD(box, item, _("Source:"), "eflete/property/item/small_label")
   EWE_COMBOBOX_ADD(item, combobox_source)

   ewe_combobox_item_add(combobox_source, _("None"));
   ewe_combobox_select_item_set(combobox_source, 0);

   collections = edje_file_collection_list(ap->project->dev);
   EINA_LIST_FOREACH(collections, l, group)
   {
      if (group != ap->project->current_style->full_group_name)
        ewe_combobox_item_add(combobox_source, group);
   }
   edje_file_collection_list_free(collections);

   elm_object_part_content_set(item, "elm.swallow.content", combobox_source);
   evas_object_smart_callback_add(combobox_source, "selected",
                                  _on_item_source_change, markup);
   elm_box_pack_end(box, item);
   elm_object_part_content_set(markup, "items.swallow", box);
   elm_object_content_set(ap->popup, markup);

   BUTTON_ADD(ap->popup, button, _("Add"));
   evas_object_data_set(ap->popup, "ENTRY", entry);
   evas_object_data_set(ap->popup, "PART", part);
   evas_object_data_set(ap->popup, "COMBOBOX", combobox_source);
   evas_object_smart_callback_add (button, "clicked", _on_button_add_clicked, ap);
   elm_object_part_content_set(ap->popup, "button1", button);

   BUTTON_ADD(ap->popup, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked, ap);
   elm_object_part_content_set(ap->popup, "button2", button);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(ap->popup);
   return ap->popup;
}
