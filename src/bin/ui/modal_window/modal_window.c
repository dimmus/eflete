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

#include "modal_window.h"
#include "widget_macro.h"
#include "main_window.h"
#include "shortcuts.h"

#define SIGNAL_DONE "done"
#define SIGNAL_CANCEL "cancel"

static void
_response_cb(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   assert(data != NULL);

   evas_object_del(data);
}

static void
_mw_info(void *data,
         Evas_Object *obj,
         void *event_info __UNUSED__)
{
   Evas_Object *popup, *content, *bt;
   char *info = (char *)data;

   assert(info != NULL);

   Evas_Object *mwin = elm_object_parent_widget_get(obj);

   popup = elm_popup_add(mwin);
   elm_popup_orient_set(popup, ELM_POPUP_ORIENT_CENTER);
   elm_object_part_text_set(popup, "title,text", _("Title will be deleted"));
   LABEL_ADD(popup, content, info);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_content_set(popup, content);

   BUTTON_ADD(popup, bt, _("Ok"));
   elm_object_part_content_set(popup, "button1", bt);
   evas_object_smart_callback_add(bt, "clicked", _response_cb, popup);
   evas_object_show(popup);
}

/* PLEASE, see comment below
static void
_on_key_down(void *data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;
   Evas_Object *btn_close = (Evas_Object *) data;
   if (strcmp(ev->keyname, "Escape") == 0)
     evas_object_smart_callback_call(btn_close, "clicked", NULL);
}
*/

static void
_anim_show_finish(void *data,
                  Evas_Object *obj,
                  const char *emission __UNUSED__,
                  const char *source __UNUSED__)
{
   Evas_Object *img, *mw;

   mw = (Evas_Object *)data;

   assert(mw != NULL);

   evas_object_smart_callback_call(mw, "show,anim,finished", "eflete");

   img = elm_layout_content_get(obj, "elm.swallow.content");
   evas_object_image_source_visible_set(img, true);
   evas_object_del(obj);
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
}

static void
_anim_hide_finish(void *data,
                  Evas_Object *obj,
                  const char *emission __UNUSED__,
                  const char *source __UNUSED__)
{
   Evas_Object *img, *mw;

   mw = (Evas_Object *)data;

   assert(mw != NULL);

   evas_object_smart_callback_call(mw, "hide,anim,finished", "eflete");

   img = elm_layout_content_get(obj, "elm.swallow.content");
   evas_object_image_source_visible_set(img, true);
   evas_object_del(obj);
   shortcuts_object_check_pop(mw);
   evas_object_hide(mw);
}

#define ANIM_ACTION(NAME) \
static void \
_anim_##NAME(void *data, \
             Evas *e, \
             Evas_Object *obj, \
             void *event_info __UNUSED__) \
{ \
   Evas_Object *parent, *helper, *img; \
   parent = (Evas_Object *)data; \
   assert(parent != NULL); \
   helper = elm_layout_add(parent); \
   elm_layout_theme_set(helper, "layout", "inwin", "anim/helper"); \
   evas_object_size_hint_weight_set(helper, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(helper, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_win_resize_object_add(parent, helper); \
   evas_object_show(helper); \
   elm_layout_signal_callback_add(helper, #NAME",anim,finished", \
                                  "eflete", _anim_##NAME##_finish, obj); \
   img = evas_object_image_filled_add(e); \
   evas_object_image_source_set(img, obj); \
   evas_object_image_source_visible_set(img, false); \
   evas_object_size_hint_weight_set(img, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(img, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_show(img); \
   elm_layout_content_set(helper, "elm.swallow.content", img); \
   elm_layout_signal_emit(helper, #NAME, "eflete"); \
}

ANIM_ACTION(show)
ANIM_ACTION(hide)

static void
_mw_close(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *mw = data;

   assert(mw != NULL);

   _anim_hide(elm_object_parent_widget_get(mw), evas_object_evas_get(mw), mw, NULL);
   evas_object_smart_callback_call(mw, SIGNAL_CANCEL, NULL);
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_mw_done(void *data,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   Evas_Object *mw = data;

   assert(mw != NULL);

   _anim_hide(elm_object_parent_widget_get(mw), evas_object_evas_get(mw), mw, NULL);
   evas_object_smart_callback_call(mw, SIGNAL_DONE, NULL);
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

void
mw_del(Evas_Object *mw)
{
   assert(mw != NULL);

   _anim_hide(elm_object_parent_widget_get(mw),
              evas_object_evas_get(mw), mw, NULL);
}

Evas_Object *
mw_add(void)
{
   Evas_Object *btn;
   static Evas_Object *mw = NULL;

   if (!mw)
     {
        mw = elm_win_inwin_add(ap.win);
        elm_object_style_set(mw, "modal_window");
        evas_object_event_callback_add(mw, EVAS_CALLBACK_SHOW, _anim_show, ap.win);

        evas_object_focus_set(mw, true);

        BUTTON_ADD(mw, btn, NULL);
        elm_object_style_set(btn, "close");
        evas_object_smart_callback_add(btn, "clicked", _mw_close, mw);
        elm_object_part_content_set(mw, "elm.swallow.close", btn);

        BUTTON_ADD(mw, btn, _("Cancel"))
           evas_object_smart_callback_add(btn, "clicked", _mw_close, mw);
        elm_object_part_content_set(mw, "eflete.swallow.btn_close", btn);

        BUTTON_ADD(mw, btn, _("Ok"))
           evas_object_smart_callback_add(btn, "clicked", _mw_done, mw);
        elm_object_part_content_set(mw, "eflete.swallow.btn_ok", btn);
     }

   shortcuts_object_push(mw);

   return mw;
}

Eina_Bool
mw_title_set(Evas_Object *object, const char *title)
{
   assert(object != NULL);
   assert(title != NULL);

   elm_object_part_text_set(object, "elm.text.title", title);
   return true;
}

Eina_Bool
mw_icon_set(Evas_Object *object, Evas_Object *icon)
{
   assert(object != NULL);
   assert(icon != NULL);

   elm_layout_content_set(object, "elm.swallow.icon", icon);
   return true;
}

Eina_Bool
mw_info_text_set(Evas_Object *object, const char *text)
{
   Evas_Object *ic_info, *bt_info;

   assert(object != NULL);
   assert(text != NULL);

   bt_info = elm_button_add(object);
   evas_object_size_hint_align_set(bt_info, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(bt_info, "clicked", _mw_info, text);
   ICON_STANDARD_ADD(bt_info, ic_info, false, "info");
   elm_layout_content_set(bt_info, "icon", ic_info);
   elm_layout_content_set(object, "elm.swallow.info", bt_info);
   evas_object_show(bt_info);
   return true;
}
