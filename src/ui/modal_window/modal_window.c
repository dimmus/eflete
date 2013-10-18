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

#include "modal_window.h"

static void
_mw_close(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *mwin = (Evas_Object *)data;
   evas_object_del(mwin);
}

static void
_response_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   evas_object_del(data);
}

static void
_mw_info(void *data,
         Evas_Object *obj,
         void *event_info __UNUSED__)
{
   Evas_Object *popup, *content, *bt;
   char *info = (char *)data;
   Evas_Object *mwin = elm_object_parent_widget_get(obj);

   popup = elm_popup_add(mwin);
   elm_object_style_set(popup, "eflete/popup");
   elm_object_part_text_set(popup, "title,text", "Title will be deleted");
   content = elm_label_add(popup);
   elm_object_text_set(content, info);
   elm_object_content_set(popup, content);

   BUTTON_ADD(popup, bt, "OK");
   elm_object_part_content_set(popup, "button1", bt);
   evas_object_smart_callback_add(bt, "clicked", _response_cb, popup);
   evas_object_show(popup);
}

static void
_on_key_down(void *data __UNUSED__,
             Evas *e __UNUSED__,
             Evas_Object *obj,
             void *event_info)
{
   Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;
   if (strcmp(ev->keyname, "Escape") == 0)
     evas_object_del(obj);
}

Evas_Object *
_mw_create(Evas_Object *parent, const char *style_name)
{
   Evas_Object *mw, *bt_close, *ic_close;

   mw = elm_win_inwin_add(parent);
   elm_object_style_set(mw, style_name);

   evas_object_focus_set(mw, EINA_TRUE);
   evas_object_event_callback_add(mw, EVAS_CALLBACK_KEY_DOWN,
                                  _on_key_down, NULL);

   BUTTON_ADD(mw, bt_close, "");
   evas_object_smart_callback_add(bt_close, "clicked", _mw_close, mw);
   ic_close = elm_icon_add(bt_close);
   elm_image_file_set(ic_close, TET_IMG_PATH"mw_button_close.png", NULL);
   elm_layout_content_set(bt_close, "icon", ic_close);
   elm_layout_content_set(mw, "elm.swallow.close", bt_close);

   return mw;
}

Evas_Object *
mw_add(Evas_Object *parent)
{
   if (!parent)
     {
        ERR("Parent evas_object is NULL.");
        return NULL;
     }

   return _mw_create(parent, "eflete/default");
}

Evas_Object *
mw_about_add(Evas_Object *parent)
{
   if (!parent)
     {
        ERR("Parent evas_object is NULL.");
        return NULL;
     }

   return _mw_create(parent, "about_window");
}

void
mw_title_set(Evas_Object *object, const char *title)
{
   if ((!object) || (!title))
     {
        ERR("Object or title is NULL.");
        return;
     }

   elm_object_part_text_set(object, "elm.text.title", title);
}

void
mw_icon_set(Evas_Object *object, Evas_Object *icon)
{
   if ((!object) || (!icon))
     {
        ERR("Evas_object or icon is NULL.");
        return;
     }

   elm_layout_content_set(object, "elm.swallow.icon", icon);
}

void
mw_info_text_set(Evas_Object *object, const char *text)
{
   Evas_Object *ic_info, *bt_info;

   if ((!object) || (!text))
     {
        ERR("Evas_object or text is NULL.");
        return;
     }

   bt_info = elm_button_add(object);
   evas_object_size_hint_align_set(bt_info, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt_info, "clicked", _mw_info, text);
   ic_info = elm_icon_add(bt_info);
   elm_image_file_set(ic_info, TET_IMG_PATH"mw_button_info.png", NULL);
   elm_layout_content_set(bt_info, "icon", ic_info);
   elm_layout_content_set(object, "elm.swallow.info", bt_info);
   evas_object_show(bt_info);
}
