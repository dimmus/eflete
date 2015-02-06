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

#ifndef WIDGET_MACRO_H
#define WIDGET_MACRO_H

#include <Elementary.h>

#define FS_TITLE "fs_title"

#define ITEM_ADD(PARENT, ITEM, TEXT, STYLE) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(ITEM, EFLETE_EDJ, STYLE); \
   elm_object_part_text_set(ITEM, "elm.text", TEXT); \
   evas_object_show(ITEM);

#define BOX_ADD(PARENT, BOX, ISHORIZONTAL, ISHOMOGENEOS) \
   BOX = elm_box_add(PARENT); \
   evas_object_size_hint_weight_set(BOX, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(BOX, EVAS_HINT_FILL, 0.0); \
   elm_box_horizontal_set(BOX, ISHORIZONTAL); \
   elm_box_homogeneous_set(BOX, ISHOMOGENEOS); \
   evas_object_show(BOX);

#define SCROLLER_ADD(PARENT, SCROLLER) \
   SCROLLER = elm_scroller_add(PARENT); \
   evas_object_size_hint_weight_set(SCROLLER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SCROLLER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_scroller_policy_set(SCROLLER, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF); \
   elm_scroller_policy_set(SCROLLER, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO); \
   evas_object_show(SCROLLER);

#define LABEL_ADD(PARENT, LABEL, TEXT) \
   LABEL = elm_label_add(PARENT); \
   elm_object_text_set(LABEL, TEXT); \
   evas_object_show(LABEL);

#define FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
   FRAME = elm_frame_add(PARENT); \
   elm_frame_autocollapse_set(FRAME, AUTOCOLLAPSE); \
   evas_object_size_hint_fill_set(FRAME, EVAS_HINT_FILL, 0.0); \
   elm_object_text_set(FRAME, TITLE); \
   evas_object_show(FRAME);

#define SPINNER_ADD(PARENT, SPINNER, MIN, MAX, STEP, ISEDIT) \
   SPINNER = elm_spinner_add(PARENT); \
   evas_object_size_hint_weight_set(SPINNER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SPINNER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_spinner_min_max_set(SPINNER, MIN, MAX); \
   elm_spinner_step_set(SPINNER, STEP); \
   elm_spinner_editable_set(SPINNER, ISEDIT); \
   evas_object_show(SPINNER);

#define ENTRY_ADD(PARENT, ENTRY, ISSINGLELINE) \
   ENTRY = elm_entry_add(PARENT); \
   evas_object_size_hint_weight_set(ENTRY, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(ENTRY, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_entry_single_line_set(ENTRY, ISSINGLELINE); \
   elm_entry_scrollable_set(ENTRY, EINA_TRUE); \
   elm_scroller_policy_set(ENTRY, ELM_SCROLLER_POLICY_OFF, \
                                  ELM_SCROLLER_POLICY_OFF); \
   evas_object_show(ENTRY);

#define EWE_ENTRY_ADD(PARENT, ENTRY, ISSINGLELINE) \
   ENTRY = ewe_entry_add(PARENT); \
   evas_object_size_hint_weight_set(ENTRY, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(ENTRY, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_entry_single_line_set(ENTRY, ISSINGLELINE); \
   elm_entry_scrollable_set(ENTRY, EINA_TRUE); \
   elm_scroller_policy_set(ENTRY, ELM_SCROLLER_POLICY_OFF, \
                                  ELM_SCROLLER_POLICY_OFF); \
   evas_object_show(ENTRY);

#define EWE_COMBOBOX_ADD(PARENT, COMBOBOX) \
   COMBOBOX = ewe_combobox_add(PARENT); \
   evas_object_size_hint_weight_set(COMBOBOX, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(COMBOBOX, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_show(COMBOBOX);

#define SEGMENT_CONTROL_ADD(PARENT, SC) \
   SC = elm_segment_control_add(PARENT); \
   evas_object_size_hint_weight_set(SC, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SC, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_show(SC);

#define CHECK_ADD(PARENT, CHECK) \
   CHECK = elm_check_add(PARENT); \
   evas_object_size_hint_align_set(CHECK, 0.0, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(CHECK, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(CHECK);

#define IMAGE_ADD(PARENT, IMAGE, NAME) \
   GET_IMAGE(IMAGE, PARENT, NAME); \
   evas_object_size_hint_align_set(IMAGE, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(IMAGE, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(IMAGE);

#define BUTTON_ADD(PARENT, BUTTON, TEXT) \
   BUTTON = elm_button_add(PARENT); \
   evas_object_size_hint_align_set(BUTTON, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(BUTTON, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_object_text_set(BUTTON, TEXT); \
   evas_object_show(BUTTON);

#define ICON_STANDARD_ADD(PARENT, ICON, NOSCALE, ICON_NAME) \
   ICON = elm_icon_add (PARENT);\
   elm_icon_standard_set(ICON, ICON_NAME);\
   elm_image_no_scale_set(ICON, NOSCALE);\
   evas_object_show(ICON);

#define ICON_ADD(PARENT, ICON, NOSCALE, NAME) \
   GET_IMAGE(ICON, PARENT, NAME);\
   evas_object_show(ICON);

#define HOVERSEL_ADD(PARENT, HOVERSEL, ISHORIZONTAL) \
   HOVERSEL = elm_hoversel_add(PARENT); \
   evas_object_size_hint_align_set(HOVERSEL, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(HOVERSEL, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_hoversel_horizontal_set(HOVERSEL, ISHORIZONTAL); \
   evas_object_show(HOVERSEL);

#define MODAL_WINDOW_ADD(WIN, PARENT, TITLE, DEL_CB, CB_DATA) \
   WIN = elm_win_add(PARENT, NULL, ELM_WIN_DIALOG_BASIC); \
   elm_win_modal_set(WIN, true); \
   elm_win_title_set(WIN, TITLE); \
   evas_object_resize(WIN, 800, 600); \
   evas_object_smart_callback_add(WIN, "delete,request", DEL_CB, CB_DATA); \
   evas_object_show(WIN);

#define FILESELECTOR_ADD(FILESELECTOR, PARENT, CALLBACK, DATA) \
   FILESELECTOR = elm_fileselector_add(PARENT); \
   evas_object_size_hint_weight_set(FILESELECTOR, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_fileselector_path_set(FILESELECTOR, profile_get()->general.projects_folder); \
   elm_fileselector_buttons_ok_cancel_set(FILESELECTOR, true); \
   elm_fileselector_expandable_set(FILESELECTOR, false); \
   elm_fileselector_mode_set(FILESELECTOR, ELM_FILESELECTOR_LIST); \
   evas_object_smart_callback_add(FILESELECTOR, "done", CALLBACK, DATA); \
   evas_object_smart_callback_add(FILESELECTOR, "activated", CALLBACK, DATA); \
   evas_object_show(FILESELECTOR);

#define RADIO_ADD(PARENT, RADIO, STATE, TEXT) \
   RADIO = elm_radio_add(PARENT); \
   evas_object_size_hint_weight_set(RADIO, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_radio_state_value_set(RADIO, STATE); \
   elm_object_text_set(RADIO, TEXT); \
   evas_object_show(RADIO);

#endif /* WIDGET_MACRO_H */
