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

#include "style_editor.h"

Evas_Object *
about_window_add()
{
   Evas_Object *mwin = mw_about_add(NULL, NULL);
   mw_title_set(mwin, _("About"));
   Evas_Object *label, *box;

   box = elm_box_add(mwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(box, 1.0, 0.4);
   evas_object_show(box);

   LABEL_ADD(mwin, label,
     "<color=#b6b6b6>"
     "<b><align=center>"ETE_PACKAGE" (Eflete) v."VERSION"</align></b><br>"
     "This application was written for Enlightenment, to use EFL<br>"
     "and design to a create and modify a Elementary widgets style.<br>"
     "<br>"
     "Copyright (C) 2013 Samsung Electronics.<br>"
     "<br>"
     "<align=center><b>Authors:</b><br>"
     "Vyacheslav \"rimmed\" Reutskiy (v.reutskiy@samsung.com))<br>"
     "Mykyta Biliavskyi (m.biliavskyi@samsung.com)<br>"
     "Vitalii Vorobiov (vi.vorobiov@samsung.com)<br>"
     "Andrii Kroitor (an.kroitor@samsung.com)<br>"
     "Ievgen Bugai (i.bugai@samsung.com)<br>"
     "Artem Popov (artem.popov@samsung.com)<br>"
     "<br>"
     "Oleg Dotsenko (o.dotsenko@samsung.com)<br>"
     "Dmitriy Samoylov (dm.samoylov@samsung.com)<br>"
     "<br>"
     "<b>Special thanks for the help:</b><br>"
     "Sergey Osadchy (se.osadchy@samsung.com)<br>"
     "Kateryna Fesyna (k.fesyna@samsung.com)<br>"
     "Maksym Volodin (m.volodin@samsung.com)<br>"
     "</align>");
   elm_box_pack_end(box, label);
   elm_win_inwin_content_set(mwin, box);

   evas_object_show(mwin);
   return mwin;
}

#undef FONT_SIZE
