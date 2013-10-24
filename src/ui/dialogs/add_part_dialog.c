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

#include "add_part_dialog.h"

#define APD_GS_KEY "entry_gs"
static Evas_Object *entry;

static void
_cancel_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}

static void
_swallow_add_on_click(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,swallow,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}

static void
_txtblock_add_on_click(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,txtblock,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}

static void
_group_add_on_click(void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,group,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}

static void
_spacer_add_on_click(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,spacer,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}


static void
_txt_add_on_click(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,txt,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}



static void
_rect_add_on_click(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,rect,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}

static void
_img_add_on_click(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   evas_object_smart_callback_del(gs, "gs,image,choosed", _img_add_on_click);
   char *char_arr[2];
   char_arr[0] = (char *)event_info;
   char_arr[1] = (char *)elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,img,add", char_arr);
   evas_object_del((Evas_Object *)data);
}

static void
_on_image_editor_done(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei)
{
   Evas_Object *gs = (Evas_Object *)data;
   char *selected = (char *)ei;
   if (!selected)
     {
        evas_object_smart_callback_del(gs, "gs,image,choosed", _img_add_on_click);
        return;
     }
   evas_object_smart_callback_call(gs, "gs,image,choosed", selected);
}

static void
_on_state_image_choose(void *data __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Evas_Object *img_edit;
   App_Data *ap = app_create();
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   img_edit = image_editor_window_add(ap->win, SINGLE);
   image_editor_init(img_edit, ap->project);
   image_editor_callback_add(img_edit, _on_image_editor_done, gs);
}

static void
_on_delete_popup(void *data,
                 Evas *evas __UNUSED__,
                 Evas_Object *o __UNUSED__,
                 void *einfo __UNUSED__)
{
   Evas_Object *gs = (Evas_Object *)data;
   evas_object_smart_callback_del(gs, "gs,image,choosed", _img_add_on_click);
}


Evas_Object *
new_part_dialog_add(Evas_Object *parent, Evas_Object  *groupspace)
{
   Evas_Object *box, *button;
   Evas_Object *popup, *bt_no;

   popup = elm_popup_add(parent);
   elm_object_style_set(popup, "eflete/popup");
   elm_object_part_text_set(popup, "title,text", "Add new part:");

   BOX_ADD(popup, box, EINA_FALSE, EINA_TRUE);
   ENTRY_ADD(popup, entry, EINA_TRUE, DEFAULT_STYLE);
   elm_entry_entry_set(entry, "New_part_name");
   evas_object_show(entry);
   elm_box_pack_end(box, entry);
   evas_object_data_set(entry, APD_GS_KEY, groupspace);

   BUTTON_ADD(box, button, "Rectangle");
   evas_object_smart_callback_add (button, "clicked",_rect_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, "Text");
   evas_object_smart_callback_add (button, "clicked",_txt_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, "Image");
   evas_object_smart_callback_add (button, "clicked", _on_state_image_choose, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, "Swallow");
   evas_object_smart_callback_add (button, "clicked",_swallow_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, "Textblock");
   evas_object_smart_callback_add (button, "clicked",_txtblock_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, "Group");
   evas_object_smart_callback_add (button, "clicked",_group_add_on_click, popup);
   elm_box_pack_end(box, button);
   BUTTON_ADD(box, button, "Spacer");
   evas_object_smart_callback_add (button, "clicked",_spacer_add_on_click, popup);
   elm_box_pack_end(box, button);

   elm_object_content_set(popup, box);
   evas_object_smart_callback_add(groupspace, "gs,image,choosed",
                                  _img_add_on_click, popup);

   BUTTON_ADD(box, bt_no, "Cancel");
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, popup);
   elm_object_part_content_set(popup, "button1", bt_no);
   evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, _on_delete_popup,
                                  groupspace);
   evas_object_show(popup);
   return popup;
}
