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

#include "style_editor.h"
#include "main_window.h"

static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
   ap->modal_editor--;
}

Evas_Object *
about_window_add()
{
   Evas_Object *mwin = mw_about_add(NULL, NULL);
   mw_title_set(mwin, _("About"));
   Evas_Object *label;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   label = elm_label_add(mwin);
   elm_object_text_set(label,
     "<color=#b6b6b6>"
     "<b><align=center>"PACKAGE_NAME" v."VERSION"</align></b><br>"
     "This application was written for Enlightenment, to use EFL<br>"
     "and design to a create and modify a Elementary widgets style.<br>"
     "<br>"
     "Copyright (C) 2013 - 2014 Samsung Electronics.<br>"
     "<br>"
     "<align=center><b>Authors:</b><br>"
     "Vyacheslav \"rimmed\" Reutskiy (v.reutskiy@samsung.com))<br>"
     "Mykyta Biliavskyi (m.biliavskyi@samsung.com)<br>"
     "Vitalii Vorobiov (vi.vorobiov@samsung.com)<br>"
     "Andrii Kroitor (an.kroitor@samsung.com)<br>"
     "Igor Gala (i.gala@samsung.com)<br>"
     "Kateryna Fesyna (k.fesyna@samsung.com)<br>"
     "Maksym Volodin (m.volodin@samsung.com)<br>"
     "Artem Popov (artem.popov@samsung.com)<br>"
     "<br>"
     "Ievgen Bugai (i.bugai@samsung.com)<br>"
     "Olga Kolesnik (o.kolesnik@samsung.com)<br>"
     "<br>"
     "Oleg Dotsenko (o.dotsenko@samsung.com)<br>"
     "Dmitriy Samoylov (dm.samoylov@samsung.com)<br>"
     "<br>"
     "<b>Special thanks for the help:</b><br>"
     "Sergey Osadchy (se.osadchy@samsung.com)<br>"
     "</align>");

   elm_object_style_set(label, "slide_about");
   elm_win_inwin_content_set(mwin, label);
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(mwin);
   ap->modal_editor++;
   return mwin;
}
