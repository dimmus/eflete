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

static char *
_item_part_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   char *style_label = (char *)data;
   if (!style_label)
     {
        ERR("It impossible, but it is occurred, part name is missing!");
        return NULL;
     }
   return strdup(style_label);
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
_img_add_on_click(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Evas_Object *gs = evas_object_data_get(entry, APD_GS_KEY);
   const char *name = elm_entry_entry_get(entry);
   evas_object_smart_callback_call(gs, "gs,img,add", strdup(name));
   evas_object_del((Evas_Object *)data);
}

Evas_Object *
new_part_dialog_add(Evas_Object *parent, Evas_Object  *groupspace)
{
   Evas_Object *box, *button;
   Evas_Object *popup, *bt_no;

   popup = elm_popup_add(parent);
   elm_object_part_text_set(popup, "title,text", "Add new part:");
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);

   BOX_ADD(popup, box, EINA_FALSE, EINA_FALSE);
   ENTRY_ADD(popup, entry, EINA_TRUE);
   elm_entry_entry_set(entry, "New_part_name");
   evas_object_show(entry);
   elm_box_pack_end(box, entry);
   evas_object_data_set(entry, APD_GS_KEY, groupspace);

#define ADD_BUTTON(text, callback) \
   button = elm_button_add(box); \
   evas_object_smart_callback_add (button, "clicked",_##callback##_add_on_click, popup); \
   evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(button, EVAS_HINT_FILL, 0.0); \
   elm_object_text_set(button, text); \
   evas_object_show(button); \
   elm_box_pack_end(box, button);

   ADD_BUTTON("Rectangle", rect);
   ADD_BUTTON("Text", txt);
   ADD_BUTTON("Image", img);
   ADD_BUTTON("Swallow", swallow);
   ADD_BUTTON("Textblock", txtblock);
   ADD_BUTTON("Group", group);
   ADD_BUTTON("Spacer", spacer);

   elm_object_content_set(popup, box);

   bt_no = elm_button_add(popup);
   elm_object_text_set(bt_no, "Cancel");
   evas_object_smart_callback_add (bt_no, "clicked", _cancel_clicked, popup);
   elm_object_part_content_set(popup, "button2", bt_no);
   evas_object_show(bt_no);

   evas_object_show(popup);
#undef ADD_BUTTON
   return popup;
}
