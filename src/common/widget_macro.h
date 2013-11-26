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

#ifndef WIDGET_MACRO_H
#define WIDGET_MACRO_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Elementary.h>

/* TODO: (refactoring) Style param in every widget define is needed. */
#define DEFAULT_STYLE "eflete/default"

#define FS_INWIN "fs_inwin"
#define FS_TITLE "fs_title"

#define ITEM_ADD(PARENT, ITEM, TEXT) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(ITEM, TET_EDJ, "property/item/default"); \
   elm_object_part_text_set(ITEM, "elm.text", TEXT); \
   evas_object_show(ITEM);

#define BOX_ADD(PARENT, BOX, ISHORIZONTAL, isHomogeneos) \
   BOX = elm_box_add(PARENT); \
   evas_object_size_hint_weight_set(BOX, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(BOX, EVAS_HINT_FILL, 0.0); \
   elm_box_horizontal_set(BOX, ISHORIZONTAL); \
   elm_box_homogeneous_set(BOX, isHomogeneos); \
   evas_object_show(BOX);

#define SCROLLER_ADD(PARENT, SCROLLER) \
   SCROLLER = elm_scroller_add(PARENT); \
   evas_object_size_hint_weight_set(SCROLLER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SCROLLER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_style_set(SCROLLER, DEFAULT_STYLE); \
   elm_scroller_policy_set(SCROLLER, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF); \
   elm_scroller_policy_set(SCROLLER, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO); \
   evas_object_show(SCROLLER);

#define LABEL_ADD(PARENT, LABEL, TEXT) \
   LABEL = elm_label_add(PARENT); \
   elm_object_style_set(LABEL, DEFAULT_STYLE); \
   elm_object_text_set(LABEL, TEXT); \
   evas_object_show(LABEL);

#define FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
   FRAME = elm_frame_add(PARENT); \
   elm_frame_autocollapse_set(FRAME, AUTOCOLLAPSE); \
   evas_object_size_hint_fill_set(FRAME, EVAS_HINT_FILL, 0.0); \
   elm_object_text_set(FRAME, TITLE); \
   elm_object_style_set(FRAME, DEFAULT_STYLE); \
   evas_object_show(FRAME);

#define SPINNER_ADD(PARENT, SPINNER, MIN, MAX, STEP, ISEDIT, STYLE) \
   SPINNER = elm_spinner_add(PARENT); \
   evas_object_size_hint_weight_set(SPINNER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SPINNER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_spinner_min_max_set(SPINNER, MIN, MAX); \
   elm_spinner_step_set(SPINNER, STEP); \
   elm_spinner_editable_set(SPINNER, ISEDIT); \
   elm_object_style_set(SPINNER, STYLE); \
   evas_object_show(SPINNER);

#define ENTRY_ADD(PARENT, ENTRY, ISSINGLELINE, STYLE) \
   ENTRY = elm_entry_add(PARENT); \
   elm_object_style_set(ENTRY, STYLE); \
   evas_object_size_hint_weight_set(ENTRY, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(ENTRY, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_entry_single_line_set(ENTRY, ISSINGLELINE); \
   elm_entry_scrollable_set(ENTRY, EINA_TRUE); \
   elm_scroller_policy_set(ENTRY, ELM_SCROLLER_POLICY_OFF, \
                                  ELM_SCROLLER_POLICY_OFF); \
   evas_object_show(ENTRY);

#define CHECK_ADD(PARENT, CHECK, STYLE) \
   CHECK = elm_check_add(PARENT); \
   elm_object_style_set(CHECK, STYLE); \
   evas_object_size_hint_align_set(CHECK, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(CHECK, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(CHECK);

#define IMAGE_ADD(PARENT, IMAGE, PATH) \
   IMAGE = elm_image_add(PARENT); \
   elm_image_file_set(IMAGE, PATH, NULL); \
   evas_object_size_hint_align_set(IMAGE, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(IMAGE, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(IMAGE);

#define BUTTON_ADD(PARENT, BUTTON, TEXT) \
   BUTTON = elm_button_add(PARENT); \
   evas_object_size_hint_align_set(BUTTON, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(BUTTON, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_object_text_set(BUTTON, TEXT); \
   elm_object_style_set(BUTTON, DEFAULT_STYLE); \
   evas_object_show(BUTTON);

#define ICON_STANDARD_ADD(PARENT, ICON, NOSCALE, ICON_NAME) \
   ICON = elm_icon_add (PARENT);\
   elm_icon_standard_set(ICON, ICON_NAME);\
   elm_image_no_scale_set(ICON, NOSCALE);\
   evas_object_show(ICON);

#define HOVERSEL_ADD(PARENT, HOVERSEL, ISHORIZONTAL) \
   HOVERSEL = elm_hoversel_add(PARENT); \
   evas_object_size_hint_align_set(HOVERSEL, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(HOVERSEL, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_hoversel_horizontal_set(HOVERSEL, ISHORIZONTAL); \
   evas_object_show(HOVERSEL);

#define OPEN_DIALOG_ADD(INWIN, FILESELECTOR, DIALOGTITLE) \
   mw_title_set(INWIN, DIALOGTITLE); \
   evas_object_focus_set(INWIN, EINA_TRUE); \
   FILESELECTOR = elm_fileselector_add(INWIN); \
   evas_object_data_set(FILESELECTOR, FS_INWIN, INWIN); \
   evas_object_size_hint_weight_set(FILESELECTOR, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(FILESELECTOR, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_fileselector_path_set(FILESELECTOR, getenv("HOME")); \
   elm_fileselector_buttons_ok_cancel_set(FILESELECTOR, EINA_TRUE); \
   elm_fileselector_expandable_set(FILESELECTOR, EINA_FALSE); \
   elm_fileselector_mode_set(FILESELECTOR, ELM_FILESELECTOR_LIST); \
   elm_win_inwin_content_set(INWIN, FILESELECTOR); \
   evas_object_show(FILESELECTOR);

#endif /* WIDGET_MACRO_H */
