/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
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
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WIDGET_DEFINE_H
#define WIDGET_DEFINE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Elementary.h>

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

#define ICON_STANDARD_ADD(parent, icon, noscale, icon_name) \
   icon = elm_icon_add (parent);\
   elm_icon_standard_set(icon, icon_name);\
   elm_image_no_scale_set(icon, noscale);\
   evas_object_show(icon);

#endif /* WIDGET_DEFINE_H */
