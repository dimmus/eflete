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
   elm_box_align_set(box, 0.0, 0.5); \
   elm_box_horizontal_set(box, isHorizontal); \
   elm_box_homogeneous_set(box, isHomogeneos); \
   evas_object_show(box);

#define LABEL_ADD(parent, label, text) \
   label = elm_label_add(parent); \
   evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_text_set(label, text); \
   evas_object_show(label);

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
   check = elm_check_add(item); \
   elm_object_style_set(check, style); \
   evas_object_size_hint_align_set(check, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(check, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(check);

#define IMAGE_ADD(parent, image, path) \
   image = elm_image_add(item); \
   elm_image_file_set(image, path, NULL); \
   evas_object_size_hint_align_set(image, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(image);


#endif /* WIDGET_DEFINE_H */
