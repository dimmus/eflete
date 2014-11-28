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

#include "part_dialog.h"
#include "string_macro.h"

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
_job_popup_del(void *data)
{
   App_Data *ap = (App_Data *)data;
   evas_object_del(ap->popup);
   ap->popup = NULL;
   ui_menu_locked_set(ap->menu_hash, false);
}

static void
_cancel_clicked(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   ecore_job_add(_job_popup_del, data);
}

#define WORKSPACE_PART_ADD(TYPE, DATA) \
   App_Data *ap = (App_Data *)data; \
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
   evas_object_del(ap->popup); \
   ap->popup = NULL; \
   ui_menu_locked_set(ap->menu_hash, false); \
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

/*
Not implemented yet
static void
_group_add_on_click(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_GROUP, NULL)
}
*/
static void
_spacer_add_on_click(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_SPACER, NULL)
}

static void
_proxy_add_on_click(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   WORKSPACE_PART_ADD(EDJE_PART_TYPE_PROXY, NULL)
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
   App_Data *ap = (App_Data *)data;
   ENTRY_IS_EMPTY
   evas_object_hide(ap->popup); /* popup is delete in WORKSPACE_PART_ADD */
   img_edit = image_editor_window_add(ap->project, SINGLE);
   evas_object_smart_callback_add(img_edit, SIG_IMAGE_SELECTED, _on_image_editor_done, ap);
}

Evas_Object *
part_dialog_add(App_Data *ap)
{
   Evas_Object *win, *workspace, *widget_list;
   Evas_Object *box, *button, *bt_no;
   Eina_Stringshare *title;
   Style *style;

   if ((!ap) || (!ap->project)) return NULL;
   win = ap->win;
   workspace = ap->workspace;
   widget_list = ui_block_widget_list_get(ap);
   if ((!win) || (!workspace) || (!widget_list)) return NULL;

   style = workspace_edit_object_get(workspace);
   ap->popup = elm_popup_add(ap->win);
   elm_object_style_set(ap->popup, "eflete");
   title = eina_stringshare_printf(_("Add new part to group \"%s\""), style->name);
   elm_object_part_text_set(ap->popup, "title,text", title);

   BOX_ADD(ap->popup, box, EINA_FALSE, EINA_TRUE);
   EWE_ENTRY_ADD(box, entry, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_name);
   elm_object_part_text_set(entry, "guide", _("Type the new part new."));
   evas_object_show(entry);
   elm_box_pack_end(box, entry);

   BUTTON_ADD(box, button, _("Rectangle"));
   evas_object_smart_callback_add(button, "clicked", _rect_add_on_click, ap);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Text"));
   evas_object_smart_callback_add(button, "clicked", _txt_add_on_click, ap);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Image"));
   evas_object_smart_callback_add(button, "clicked", _on_state_image_choose, ap);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Swallow"));
   evas_object_smart_callback_add(button, "clicked", _swallow_add_on_click, ap);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, _("Textblock"));
   evas_object_smart_callback_add(button, "clicked", _txtblock_add_on_click, ap);
   elm_box_pack_end(box, button);
/* Not implemented yet
   BUTTON_ADD(box, button, _("Group"));
   evas_object_smart_callback_add(button, "clicked", _group_add_on_click, ap);
   elm_box_pack_end(box, button);
*/
   BUTTON_ADD(box, button, _("Spacer"));
   evas_object_smart_callback_add(button, "clicked", _spacer_add_on_click, ap);
   elm_box_pack_end(box, button);

   BUTTON_ADD(box, button, _("Proxy"));
   evas_object_smart_callback_add(button, "clicked", _proxy_add_on_click, ap);
   elm_box_pack_end(box, button);

   elm_object_content_set(ap->popup, box);
   BUTTON_ADD(box, bt_no, _("Cancel"));
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, ap);
   elm_object_part_content_set(ap->popup, "button1", bt_no);

   ui_menu_locked_set(ap->menu_hash, true);

   evas_object_show(ap->popup);
   eina_stringshare_del(title);
   return ap->popup;
}
#undef ENTRY_IS_EMPTY
