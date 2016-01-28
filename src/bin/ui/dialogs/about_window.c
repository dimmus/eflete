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

#include "main_window.h"
#include "modal_window.h"

static void
_on_mwin_del(void *data __UNUSED__,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

static void
_on_button_close_clicked_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   assert(data != NULL);
   evas_object_del(data);
}

Evas_Object *
about_window_add()
{
   Evas_Object *button;
   Evas_Object *mwin = mw_add("about_window", NULL, NULL);

   assert(mwin != NULL);

   mw_title_set(mwin, _("About"));
   Evas_Object *label;

   label = elm_label_add(mwin);
   elm_object_text_set(label,
     "<color=#b6b6b6>"
     "<b><align=center>"PACKAGE_NAME" v."VERSION"</align></b><br>"
     "This application was written for Enlightenment project.<br>"
     "It is designed to create and modify styles of Elementary widgets.<br>"
     "<br>"
     "Copyright (C) 2013 - 2015 Samsung Electronics.<br>"
     "<br>"
     "<align=center><b>Authors:</b><br>"
     "Vyacheslav \"rimmed\" Reutskiy (v.reutskiy@samsung.com)<br>"
     "Mykyta Biliavskyi (m.biliavskyi@samsung.com)<br>"
     "Vitalii Vorobiov (vi.vorobiov@samsung.com)<br>"
     "Andrii Kroitor (an.kroitor@samsung.com)<br>"
     "Kateryna Fesyna (fesyna1@gmail.com)<br>"
     "Maksym Volodin (mac9.ua@gmail.com)<br>"
     "Igor Gala (igor.gala89@gmail.com)<br>"
     "<br>"
     "Olga Kolesnik (o.kolesnik@samsung.com)<br>"
     "<br>"
     "Oleg Dotsenko (o.dotsenko@samsung.com)<br>"
     "Yurii Tsivun (y.tsivun@samsung.com)<br>"
     "Dmitriy Samoylov (dm.samoylov@samsung.com)<br>"
     "</align>");

   elm_object_style_set(label, "slide_about");
   elm_win_inwin_content_set(mwin, label);
   elm_layout_signal_emit(label, "elm,state,slide,start", "elm");
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(mwin, EVAS_CALLBACK_DEL, _on_mwin_del, NULL);

   BUTTON_ADD(mwin, button, _("Close"));
   evas_object_smart_callback_add(button, "clicked", _on_button_close_clicked_cb, mwin);
   elm_object_part_content_set(mwin, "eflete.swallow.btn_close", button);

   evas_object_show(mwin);
   return mwin;
}
