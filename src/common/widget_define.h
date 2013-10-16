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

#ifndef WIDGET_DEFINE_H
#define WIDGET_DEFINE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Elementary.h>

#define ITEM_ADD(parent, item, text) \
   item = elm_layout_add(parent); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(item, TET_EDJ, "property/item/default"); \
   elm_object_text_set(item, text); \
   evas_object_show(item);

#define BOX_ADD(parent, box, isHorizontal, isHomogeneos) \
   box = elm_box_add(parent); \
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0); \
   elm_box_horizontal_set(box, isHorizontal); \
   elm_box_homogeneous_set(box, isHomogeneos); \
   evas_object_show(box);

#define SCROLLER_ADD(parent, scroller) \
   scroller = elm_scroller_add(parent); \
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_style_set(scroller, "eflete/default"); \
   evas_object_show(scroller);

#define LABEL_ADD(parent, label, text) \
   label = elm_label_add(parent); \
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_text_set(label, text); \
   evas_object_show(label);

#define FRAME_ADD(parent, frame, autocollapse, title) \
   frame = elm_frame_add(parent); \
   elm_frame_autocollapse_set(frame, autocollapse); \
   evas_object_size_hint_fill_set(frame, EVAS_HINT_FILL, 0.0); \
   elm_object_text_set(frame, title); \
   elm_object_style_set(frame, "eflete/default"); \
   evas_object_show(frame);

#define SPINNER_ADD(parent, spinner, min, max, step, isEdit, style) \
   spinner = elm_spinner_add(parent); \
   evas_object_size_hint_weight_set(spinner, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(spinner, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_spinner_min_max_set(spinner, min, max); \
   elm_spinner_step_set(spinner, step); \
   elm_spinner_editable_set(spinner, isEdit); \
   elm_object_style_set(spinner, style); \
   evas_object_show(spinner);

#define ENTRY_ADD(parent, entry, isSingleLine) \
   entry = elm_entry_add(parent); \
   elm_entry_single_line_set(entry, EINA_TRUE); \
   evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_entry_single_line_set(entry, isSingleLine); \
   elm_entry_scrollable_set(entry, EINA_TRUE); \
   elm_entry_scrollbar_policy_set(entry, ELM_SCROLLER_POLICY_OFF, \
                                  ELM_SCROLLER_POLICY_OFF); \
   evas_object_show(entry);

#define CHECK_ADD(parent, check, style) \
   check = elm_check_add(parent); \
   elm_object_style_set(check, style); \
   evas_object_size_hint_align_set(check, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(check, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(check);

#define IMAGE_ADD(parent, image, path) \
   image = elm_image_add(parent); \
   elm_image_file_set(image, path, NULL); \
   evas_object_size_hint_align_set(image, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(image);

#define BUTTON_ADD(parent, btn, text) \
   btn = elm_button_add(parent); \
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_object_text_set(btn, text); \
   evas_object_show(btn);

#define ICON_STANDARD_ADD(parent, icon, noscale, icon_name) \
   icon = elm_icon_add (parent);\
   elm_icon_standard_set(icon, icon_name);\
   elm_image_no_scale_set(icon, noscale);\
   evas_object_show(icon);

#define HOVERSEL_ADD(parent, hoversel, isHorizontal) \
   hoversel = elm_hoversel_add(parent); \
   evas_object_size_hint_align_set(hoversel, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(hoversel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_hoversel_horizontal_set(hoversel, isHorizontal); \
   evas_object_show(hoversel);

#define OPEN_DIALOG_ADD(inwin, box, fs, dialogTitle) \
   box = elm_box_add(inwin); \
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_win_inwin_content_set(inwin, box); \
   evas_object_show(box); \
   mw_title_set(inwin, dialogTitle); \
   evas_object_focus_set(inwin, EINA_TRUE); \
   fs = elm_fileselector_add(box); \
   elm_box_pack_end(box, fs); \
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_fileselector_path_set(fs, getenv("HOME")); \
   elm_fileselector_buttons_ok_cancel_set(fs, EINA_TRUE); \
   elm_fileselector_expandable_set(fs, EINA_FALSE); \
   elm_fileselector_mode_set(fs, ELM_FILESELECTOR_LIST); \
   evas_object_show(fs);


#endif /* WIDGET_DEFINE_H */
