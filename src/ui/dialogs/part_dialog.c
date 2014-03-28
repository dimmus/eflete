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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "part_dialog.h"
#include "string_macro.h"

#define AP_KEY "app_data"

#define ENTRY_IS_EMPTY \
   const char *str = elm_entry_entry_get(entry); \
   if (elm_entry_is_empty(entry)) \
     { \
        NOTIFY_WARNING(_("Part name can not be empty")) \
        return; \
     } \
   if (edje_edit_part_exist(ap->project->current_style->obj, str)) \
     { \
        NOTIFY_WARNING(_("Can't add the part %s, part with the same name already exists"), str) \
        return; \
     }

static Evas_Object *entry;

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = PART_NAME_BANNED_SYMBOLS
};

static void
_cancel_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}

#define WORKSPACE_PART_ADD(TYPE, DATA) \
   App_Data *ap = evas_object_data_get(entry, AP_KEY); \
   Evas_Object *workspace = ap->workspace; \
   Evas_Object *widget_list = ui_block_widget_list_get(ap); \
   Style *style = workspace_edit_object_get(workspace); \
   ENTRY_IS_EMPTY \
   const char *name = elm_entry_entry_get(entry); \
   if (workspace_edit_object_part_add(workspace, name, TYPE, DATA)) \
     { \
       ui_widget_list_part_add(widget_list, style, name); \
       style->isModify = true; \
     } \
   evas_object_del((Evas_Object *)data); \
   live_view_widget_style_set(ap->live_view, ap->project, style);

static void
_swallow_add_on_click(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_SWALLOW, NULL)
}

static void
_txtblock_add_on_click(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_TEXTBLOCK, NULL)
}

static void
_group_add_on_click(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_GROUP, NULL)
}

static void
_spacer_add_on_click(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_SPACER, NULL)
}


static void
_txt_add_on_click(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_TEXT, NULL)
}

static void
_rect_add_on_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_RECTANGLE, NULL)
}

static void
_on_image_editor_done(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   char *selected = (char *)event_info;
   if (!selected) return;
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_IMAGE, selected)
}

static void
_on_state_image_choose(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Evas_Object *img_edit;
   Evas_Object *popup = (Evas_Object *)data;
   App_Data *ap = evas_object_data_get(entry, AP_KEY);
   ENTRY_IS_EMPTY
   evas_object_hide(popup);
   img_edit = image_editor_window_add(ap->project, SINGLE);
   image_editor_callback_add(img_edit, _on_image_editor_done, NULL);
}

Evas_Object *
part_dialog_add(App_Data *ap)
{
   Evas_Object *win, *workspace, *widget_list;
   Evas_Object *box, *button;
   Evas_Object *popup, *bt_no;
   Eina_Stringshare *title;
   Style *style;

   if ((!ap) || (!ap->project)) return NULL;
   win = ap->win;
   workspace = ap->workspace;
   widget_list = ui_block_widget_list_get(ap);
   if ((!win) || (!workspace) || (!widget_list)) return NULL;

   style = workspace_edit_object_get(workspace);
   popup = elm_popup_add(ap->win);
   elm_object_style_set(popup, "eflete");
   title = eina_stringshare_printf(_("Add new part to group \"%s\""), style->name);
   elm_object_part_text_set(popup, "title,text", title);

   BOX_ADD(popup, box, EINA_FALSE, EINA_TRUE);
   ENTRY_ADD(popup, entry, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry, "guide", _("Type the new part new."));
   evas_object_show(entry);
   elm_box_pack_end(box, entry);
   evas_object_data_set(entry, AP_KEY, ap);

   BUTTON_ADD(box, button, _("Rectangle"));
   evas_object_smart_callback_add(button, "clicked", _rect_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Text"));
   evas_object_smart_callback_add(button, "clicked", _txt_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Image"));
   evas_object_smart_callback_add(button, "clicked", _on_state_image_choose, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Swallow"));
   evas_object_smart_callback_add(button, "clicked", _swallow_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Textblock"));
   evas_object_smart_callback_add(button, "clicked", _txtblock_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Group"));
   evas_object_smart_callback_add(button, "clicked", _group_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Spacer"));
   evas_object_smart_callback_add(button, "clicked", _spacer_add_on_click, popup);
   elm_box_pack_end(box, button);

   elm_object_content_set(popup, box);
   BUTTON_ADD(box, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, popup);
   elm_object_part_content_set(popup, "button1", bt_no);
   evas_object_show(popup);
   eina_stringshare_del(title);
   return popup;
}
#undef ENTRY_IS_EMPTY
