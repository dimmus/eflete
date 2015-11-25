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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "item_dialog.h"

static Evas_Object *entry, *combobox, *btn_add;
static Elm_Validator_Regexp *name_validator = NULL;

static void
_job_popup_close(void *data __UNUSED__)
{
   assert(name_validator != NULL);

   Evas_Object *markup = elm_object_content_get(ap.popup);
   Evas_Object *proxy_preview = elm_object_part_content_unset(markup, "preview.swallow");
   Evas_Object *group_preview = elm_object_part_content_unset(markup, "origin.swallow");

   elm_validator_regexp_free(name_validator);
   name_validator = NULL;

   eina_file_map_free(ap.project->mmap_file, group_preview);
   evas_object_del(group_preview);
   evas_object_del(proxy_preview);
   evas_object_del(ap.popup);

   ap.popup = NULL;
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_on_button_add_clicked(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{

   Part *part = evas_object_data_get(ap.popup, "PART");

   assert(entry != NULL);
   assert(combobox != NULL);
   assert(part != NULL);

   Ewe_Combobox_Item *item = NULL;

   if (elm_entry_is_empty(entry))
     {
        NOTIFY_WARNING(_("Item name can not be empty!"))
        return;
     }

   item = ewe_combobox_select_item_get(combobox);

   if (item->index == 0)
     {
        NOTIFY_WARNING(_("Source is not selected! Please select source for item."))
        return;
     }

   edje_edit_string_list_free(part->items);
   part->items = edje_edit_part_items_list_get(ap.project->current_style->obj, part->name);
   ui_widget_list_part_items_refresh(ui_block_widget_list_get(), part, true);

   workspace_edit_object_recalc(ap.workspace);
   //project_changed(false);
   ecore_job_add(_job_popup_close, NULL);
}

static void
_on_button_cancel_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   ecore_job_add(_job_popup_close, data);
}

static void
_on_item_source_change(void *data,
                       Evas_Object *obj,
                       void *event_info)
{
   Evas_Object *markup = (Evas_Object *)data;
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info;

   assert(markup != NULL);

   Evas_Object *proxy_preview = elm_object_part_content_unset(markup, "preview.swallow");
   Evas_Object *group_preview = elm_object_part_content_unset(markup, "origin.swallow");

   if (!group_preview)
     group_preview =  edje_object_add(evas_object_evas_get(obj));
   else
     eina_file_map_free(ap.project->mmap_file, group_preview);

   edje_object_mmap_set(group_preview, ap.project->mmap_file, item->title);
   evas_object_size_hint_min_set(group_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(group_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_align_set(group_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_max_set(group_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(markup, "origin.swallow", group_preview);
   evas_object_show(group_preview);
   if (!proxy_preview)
     {
        proxy_preview = evas_object_image_filled_add(evas_object_evas_get(obj));
        evas_object_size_hint_min_set(proxy_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_weight_set(proxy_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_size_hint_max_set(proxy_preview, EVAS_HINT_FILL, EVAS_HINT_FILL);
        evas_object_image_scale_hint_set(proxy_preview, EVAS_IMAGE_SCALE_HINT_STATIC);
        evas_object_image_content_hint_set(proxy_preview, EVAS_IMAGE_CONTENT_HINT_STATIC);
        elm_object_part_content_set(markup, "preview.swallow", proxy_preview);
        evas_object_show(proxy_preview);
     }
   evas_object_image_source_set(proxy_preview, group_preview);
   evas_object_image_smooth_scale_set(proxy_preview, true);
   evas_object_image_source_visible_set(proxy_preview, false);
   evas_object_image_source_clip_set(proxy_preview, false);

   return;
}

static void
_validation(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   if ((elm_validator_regexp_status_get(name_validator) != ELM_REG_NOERROR) ||
       (ewe_combobox_select_item_get(combobox)->index == 0))
     elm_object_disabled_set(btn_add, true);
   else
     elm_object_disabled_set(btn_add, false);
}

Evas_Object *
item_dialog_add(Part *part)
{
   Evas_Object *box, *item, *button;
   Eina_List *collections = NULL, *l = NULL;
   Eina_Stringshare *group = NULL;
   Evas_Object *markup = NULL;

   assert(ap.workspace != NULL);
   assert(part != NULL);
   assert(name_validator == NULL);

   name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   ap.popup = elm_popup_add(ap.win_layout);
   elm_object_part_text_set(ap.popup, "title,text", _("Add new item"));
   elm_popup_orient_set(ap.popup, ELM_POPUP_ORIENT_CENTER);

   markup = elm_layout_add(ap.popup);
   elm_layout_theme_set(markup, "layout", "dialog", "item");
   evas_object_size_hint_weight_set(markup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(markup);

   BOX_ADD(ap.popup, box, false, false);

   LAYOUT_PROP_ADD(box, _("Name:"), "property", "1swallow")
   ENTRY_ADD(item, entry, true);
   eo_do(entry, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, name_validator));
   evas_object_smart_callback_add(entry, "changed", _validation, NULL);
   elm_object_part_text_set(entry, "guide", _("Type a item name."));
   elm_object_part_content_set(item, "elm.swallow.content", entry);
   elm_box_pack_end(box, item);

   LAYOUT_PROP_ADD(box, _("Source:"), "property", "1swallow")
   EWE_COMBOBOX_ADD(item, combobox)
   ewe_combobox_item_add(combobox, _("None"));
   ewe_combobox_select_item_set(combobox, 0);

   collections = edje_file_collection_list(ap.project->dev);
   collections = eina_list_sort(collections, eina_list_count(collections), sort_cb);
   EINA_LIST_FOREACH(collections, l, group)
     {
        if (group != ap.project->current_style->full_group_name)
          ewe_combobox_item_add(combobox, group);
     }
   edje_file_collection_list_free(collections);

   elm_object_part_content_set(item, "elm.swallow.content", combobox);
   evas_object_smart_callback_add(combobox, "selected", _on_item_source_change, markup);
   evas_object_smart_callback_add(combobox, "selected", _validation, NULL);
   elm_box_pack_end(box, item);
   elm_object_part_content_set(markup, "items.swallow", box);
   elm_object_content_set(ap.popup, markup);

   evas_object_data_set(ap.popup, "PART", part);

   BUTTON_ADD(ap.popup, btn_add, _("Add"));
   evas_object_smart_callback_add (btn_add, "clicked", _on_button_add_clicked, NULL);
   elm_object_part_content_set(ap.popup, "button1", btn_add);
   elm_object_disabled_set(btn_add, true);

   BUTTON_ADD(ap.popup, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked, NULL);
   elm_object_part_content_set(ap.popup, "button2", button);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_show(ap.popup);
   elm_object_focus_set(entry, true);
   return ap.popup;
}
