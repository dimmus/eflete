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

#include "style_editor.h"

Evas_Object *
about_window_add(Evas_Object *parent)
{
   Evas_Object *mwin = mw_about_add(parent);
   mw_title_set(mwin, "About");
   const char* labels_text[7] = { "Vyacheslav \"rimmed\" Reutskiy (v.reutskiy@samsung.com)",
                               "Mykyta Biliavskyi (m.biliavskyi@samsung.com)",
                               "Vitalii Vorobiov (vi.vorobiov@samsung.com)",
                               "Dmytro Dadyka (d.dadyka@samsung.com)",
                               "Sergey Osadchy (se.osadchy@samsung.com)",
                               "Andrii Kroitor (an.kroitor@samsung.com)",
                               "Kateryna Fesyna (k.fesyna@samsung.com)"};
   Evas_Object *label, *box;
   int i;

   box = elm_box_add(mwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set( box, 1.0, 0.4);
   evas_object_show(box);

   label = elm_label_add(mwin);
   elm_object_text_set(label, "Theme-Editor-Tool authors:");
   elm_object_style_set(label, "marker");
   evas_object_color_set(label, 0, 0, 0, 255);
   elm_box_pack_end(box, label);
   evas_object_show(label);

   Evas_Object * separator = elm_separator_add (mwin);
   elm_separator_horizontal_set(separator, EINA_TRUE);
   elm_box_pack_end(box, separator);
   evas_object_show(separator);

   for (i = 0; i < 7; i++)
   {
       label = elm_label_add(mwin);
       elm_object_text_set(label, labels_text[i]);
       elm_box_pack_end(box, label);
       evas_object_show(label);
   }

   elm_win_inwin_content_set(mwin, box);

   evas_object_show(mwin);
   return mwin;
}

#undef FONT_SIZE
