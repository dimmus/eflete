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

#include "string_macro.h"
#include "colorsel.h"

static Elm_Entry_Filter_Accept_Set accept_color = {
   .accepted = "0123456789 ",
   .rejected = NULL
};

static Elm_Entry_Filter_Accept_Set accept_prop = {
   .accepted = NULL,
   .rejected = PART_NAME_BANNED_SYMBOLS
};

/* group */
#define ITEM_SPINNER_CALLBACK(SUB, VALUE, type) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   type value = (type)elm_spinner_value_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, value)) \
     return; \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_2SPINNER_GROUP_ADD(text, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            double min, \
                                            double max, \
                                            double step, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, true, true); \
   elm_box_padding_set(box, 5, 0); \
   SPINNER_ADD(box, spinner1, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner1, "%.0f px"); \
   elm_spinner_value_set(spinner1, edje_edit_##SUB##_##VALUE1##_get(pd->style->obj)); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   elm_box_pack_end(box, spinner1); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   SPINNER_ADD(box, spinner2, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_value_set(spinner2, edje_edit_##SUB##_##VALUE2##_get(pd->style->obj)); \
   elm_spinner_label_format_set(spinner2, "%.0f px"); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   elm_box_pack_end(box, spinner2); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2SPINNER_GROUP_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, Prop_Data *pd) \
{ \
   Evas_Object *box, *spinner1, *spinner2; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   spinner1 = eina_list_nth(nodes, 0); \
   elm_spinner_value_set(spinner1, edje_edit_##SUB##_##VALUE1##_get(pd->style->obj)); \
   evas_object_smart_callback_del_full(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   spinner2 = eina_list_nth(nodes, 1); \
   elm_spinner_value_set(spinner2, edje_edit_##SUB##_##VALUE2##_get(pd->style->obj)); \
   evas_object_smart_callback_del_full(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   eina_list_free(nodes); \
}

/* part */
#define ITEM_STRING_PART_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   const char *value = elm_entry_entry_get(obj); \
   if (strcmp(value, "") == 0) value = NULL; \
     if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, value)) \
       { \
         NOTIFY_INFO(5, "Wrong input value for "#VALUE" field"); \
         return; \
       } \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_CHECK_PART_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, value)) \
     return; \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_INT_PART_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   int drag = elm_spinner_value_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, drag)) \
     return; \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_1ENTRY_PART_ADD(text, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_prop); \
   elm_entry_entry_set(entry, edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name)); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1ENTRY_PART_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *entry; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   elm_entry_entry_set(entry, edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name)); \
   evas_object_smart_callback_del_full(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
}

#define ITEM_1CHEACK_PART_ADD(text, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(parent, check, DEFAULT_STYLE) \
   elm_check_state_set(check, edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name)); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1CHEACK_PART_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *check; \
   check = elm_object_part_content_get(item, "elm.swallow.content"); \
   elm_check_state_set(check, edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name)); \
   evas_object_smart_callback_del_full(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
}

#define ITEM_DRAG_PART_ADD(text, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            double min, \
                                            double max, \
                                            double step, \
                                            const char *fmt, \
                                            const char *tooltip1, \
                                            const char *tooltip2 ) \
{ \
   Evas_Object *item, *box, *check, *spinner; \
   int ch_value, st_value; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, true, true) \
   CHECK_ADD(box, check, "eflete/toggle") \
   ch_value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, pd->part->name); \
   elm_check_state_set(check, ch_value); \
   elm_object_tooltip_text_set(check, tooltip1); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   elm_box_pack_end(box, check); \
   SPINNER_ADD(box, spinner, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner, fmt); \
   st_value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, pd->part->name); \
   elm_spinner_value_set(spinner, st_value); \
   elm_object_tooltip_text_set(spinner, tooltip2); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_box_pack_end(box, spinner); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_DRAG_PART_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *box, *check, *spinner; \
   int ch_value, st_value; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   check = eina_list_nth(nodes, 0); \
   ch_value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, pd->part->name); \
   elm_check_state_set(check, ch_value); \
   evas_object_smart_callback_del_full(check, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   spinner = eina_list_nth(nodes, 1); \
   st_value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, pd->part->name); \
   elm_spinner_value_set(spinner, st_value); \
   evas_object_smart_callback_del_full(spinner, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   eina_list_free(nodes); \
}

/* state */
#define ITEM_STRING_STATE_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   const char *value = elm_entry_entry_get(obj); \
   if (strcmp(value, "") == 0) value = NULL; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value, \
                                        value)) \
     { \
        NOTIFY_INFO(5, "Wrong input value for "#VALUE" field."); \
        return; \
     } \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_SPINNER_STATE_CALLBACK(SUB, VALUE, type) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   type value = (type)elm_spinner_value_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name,\
                                        pd->part->curr_state, \
                                        pd->part->curr_state_value, \
                                        value)) \
     return; \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_COLOR_STATE_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   int r, g, b, a; \
   Prop_Data *pd = (Prop_Data *)data; \
   Evas_Object *color, *rect; \
   color = evas_object_data_get(obj, "color"); \
   colorselector_color_get(obj, &r, &g, &b, &a); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, \
                                        pd->part->curr_state_value, \
                                        r, g, b, a))\
     return; \
   rect = elm_object_part_content_get(color, "elm.swallow.content"); \
   evas_object_color_set(rect, r*a/255, g*a/255, b*a/255, a); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
} \
static void \
_on_##SUB##_##VALUE##_dismissed(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   evas_object_smart_callback_del_full(obj, "color,changed", \
                                      _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_del_full(obj, "palette,item,selected", \
                                       _on_##SUB##_##VALUE##_change, pd); \
   evas_object_data_del(obj, "color"); \
   evas_object_hide(obj); \
} \
static void \
_on_##SUB##_##VALUE##_clicked(void *data, \
                              Evas *e __UNUSED__, \
                              Evas_Object *obj, \
                              void *event_info __UNUSED__) \
{ \
   int x, y; \
   int r, g, b, a; \
   Evas_Object *colorsel, *rect; \
   Prop_Data *pd = (Prop_Data *)data; \
   colorsel = colorselector_get(); \
   evas_object_data_set(colorsel, "color", obj); \
   rect = elm_object_part_content_get(obj, "elm.swallow.content"); \
   evas_object_color_get(rect, &r, &g, &b, &a); \
   colorselector_color_set(colorsel, r, g, b, a); \
   evas_object_smart_callback_add(colorsel, "color,changed", \
                                  _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(colorsel, "palette,item,selected", \
                                  _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(colorsel, "dismissed", \
                                  _on_##SUB##_##VALUE##_dismissed, pd); \
   evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y); \
   evas_object_move(colorsel, x, y); \
   evas_object_show(colorsel); \
}

#define ITEM_CHECK_STATE_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, \
                                        pd->part->curr_state_value, \
                                        value)) \
     return; \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_IM_BORDER_STATE_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   unsigned int tok_elm; \
   Prop_Data *pd = (Prop_Data *)data; \
   const char *value = elm_entry_entry_get(obj); \
   char **c = eina_str_split_full (value, " ", 4, &tok_elm); \
   if (tok_elm < 4) \
     { \
        free(c[0]); \
        free(c); \
        NOTIFY_ERROR ("Please input correct border data: l r t b, " \
                      "where l - left, r - right, t - top, b - bottom borders") \
        return; \
     } \
   edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                   pd->part->curr_state, pd->part->curr_state_value, \
                                   atoi(c[0]), atoi(c[1]), atoi(c[2]), atoi(c[3])); \
   free(c[0]); \
   free(c); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_2SPINNER_STATE_ADD(text, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            double min, \
                                            double max, \
                                            double step, \
                                            const char *fmt, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   double value; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, true, true) \
   elm_box_padding_set(box, 5, 0); \
   SPINNER_ADD(box, spinner1, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner1, fmt); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_spinner_value_set(spinner1, value); \
   elm_box_pack_end(box, spinner1); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   SPINNER_ADD(box, spinner2, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner2, fmt); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_spinner_value_set(spinner2, value); \
   elm_box_pack_end(box, spinner2); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  _on_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item;\
}

#define ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *box, *spinner1, *spinner2; \
   double value; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   spinner1 = eina_list_nth(nodes, 0); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_spinner_value_set(spinner1, value); \
   evas_object_smart_callback_del_full(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   spinner2 = eina_list_nth(nodes, 1); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_spinner_value_set(spinner2, value); \
   evas_object_smart_callback_del_full(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   eina_list_free(nodes); \
}

#define ITEM_1ENTRY_STATE_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                void *btn_func_cb, \
                                const char *tooltip, \
                                const char *btn_tooltip) \
{ \
   Evas_Object *item, *entry, *btn; \
   const char *value; \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   ITEM_ADD(parent, item, TEXT) \
   ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_prop); \
   if (btn_func_cb) \
     { \
        btn = elm_button_add(parent); \
        elm_object_style_set(btn, "eflete/elipsis"); \
        evas_object_show(btn); \
        evas_object_smart_callback_add(btn, "clicked", btn_func_cb, entry); \
        elm_object_part_content_set(entry, "elm.swallow.end", btn); \
        elm_object_tooltip_text_set(btn, btn_tooltip); \
     } \
   elm_entry_entry_set(entry, value); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1ENTRY_STATE_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *entry; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   const char *value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                                                       pd->part->curr_state, \
                                                       pd->part->curr_state_value); \
   elm_entry_entry_set(entry, value); \
   evas_object_smart_callback_del_full(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
}

#define ITEM_COLOR_STATE_ADD(text, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *color, *rect; \
   int r, g, b, a; \
   edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                                   pd->part->curr_state, pd->part->curr_state_value, \
                                   &r, &g, &b, &a); \
   ITEM_ADD(parent, item, text) \
   color = elm_layout_add(item); \
   elm_layout_file_set(color, EFLETE_EDJ, "eflete/prop/color"); \
   evas_object_size_hint_weight_set(color, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(color, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_tooltip_text_set(color, tooltip); \
   rect = evas_object_rectangle_add(evas_object_evas_get(parent)); \
   evas_object_event_callback_add(color, EVAS_CALLBACK_MOUSE_DOWN, \
                                  _on_##SUB##_##VALUE##_clicked, pd); \
   evas_object_color_set(rect, r*a/255, g*a/255, b*a/255, a); \
   elm_object_part_content_set(color, "elm.swallow.content", rect); \
   elm_object_part_content_set(item, "elm.swallow.content", color); \
   return item; \
}

#define ITEM_COLOR_STATE_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *color, *rect; \
   int r, g, b, a; \
   edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                                   pd->part->curr_state, pd->part->curr_state_value, \
                                   &r, &g, &b, &a); \
   color = elm_object_part_content_get(item, "elm.swallow.content"); \
   rect = elm_object_part_content_get(color, "elm.swallow.content"); \
   evas_object_event_callback_del_full(color, EVAS_CALLBACK_MOUSE_DOWN, \
                                       _on_##SUB##_##VALUE##_clicked, pd); \
   evas_object_event_callback_add(color, EVAS_CALLBACK_MOUSE_DOWN, \
                                  _on_##SUB##_##VALUE##_clicked, pd); \
   evas_object_color_set(rect, r*a/255, g*a/255, b*a/255, a); \
}

#define ITEM_2ENTRY_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *entry1, *entry2; \
   Eina_Stringshare *value; \
   ITEM_ADD(parent, item, TEXT) \
   BOX_ADD(item, box, true, true) \
   elm_box_padding_set(box, 5, 0); \
   ENTRY_ADD(box, entry1, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry1, elm_entry_filter_accept_set, &accept_prop); \
   ENTRY_ADD(box, entry2, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry2, elm_entry_filter_accept_set, &accept_prop); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_entry_entry_set(entry1, value); \
   elm_object_tooltip_text_set(entry1, tooltip1); \
   evas_object_smart_callback_add(entry1, "activated", _on_##SUB##_##VALUE1##_change, pd); \
   elm_box_pack_end(box, entry1); \
   edje_edit_string_free(value); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_entry_entry_set(entry2, value); \
   elm_object_tooltip_text_set(entry2, tooltip2); \
   evas_object_smart_callback_add(entry2, "activated", _on_##SUB##_##VALUE2##_change, pd); \
   elm_box_pack_end(box, entry2); \
   edje_edit_string_free(value); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2ENTRY_STATE_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *box, *entry1, *entry2; \
   Eina_Stringshare *value; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   entry1 = eina_list_nth(nodes, 0); \
   elm_entry_entry_set(entry1, value); \
   evas_object_smart_callback_del_full(entry1, "activated", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(entry1, "activated", _on_##SUB##_##VALUE1##_change, pd); \
   edje_edit_string_free(value); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   entry2 = eina_list_nth(nodes, 1); \
   elm_entry_entry_set(entry2, value); \
   evas_object_smart_callback_del_full(entry2, "activated", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(entry2, "activated", _on_##SUB##_##VALUE2##_change, pd); \
   edje_edit_string_free(value); \
   eina_list_free(nodes); \
}

#define ITEM_1SPINNER_STATE_ADD(text, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                double min, \
                                double max, \
                                double step, \
                                const char *fmt, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *spinner; \
   double value; \
   ITEM_ADD(parent, item, text) \
   SPINNER_ADD(item, spinner, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner, fmt); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_spinner_value_set(spinner, value); \
   elm_object_tooltip_text_set(spinner, tooltip); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", spinner); \
   return item;\
}

#define ITEM_1SPINNER_STATE_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *spinner; \
   double value; \
   spinner = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_spinner_value_set(spinner, value); \
   evas_object_smart_callback_del_full(spinner, "changed", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE##_change, pd); \
}

#define ITEM_1CHEACK_STATE_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *check; \
   Eina_Bool value; \
   ITEM_ADD(parent, item, TEXT) \
   CHECK_ADD(item, check, "eflete/toggle") \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_check_state_set(check, value); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1CHEACK_STATE_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *check; \
   Eina_Bool value; \
   check = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_check_state_set(check, value); \
   evas_object_smart_callback_del_full(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
}

#define ITEM_2CHEACK_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *check1, *check2; \
   Eina_Bool value; \
   ITEM_ADD(parent, item, TEXT) \
   BOX_ADD(item, box, true, true) \
   CHECK_ADD(box, check1, "eflete/toggle") \
   CHECK_ADD(box, check2, "eflete/toggle") \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check1, value); \
   elm_object_tooltip_text_set(check1, tooltip1); \
   evas_object_smart_callback_add(check1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   elm_box_pack_end(box, check1); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check2, value); \
   elm_object_tooltip_text_set(check2, tooltip2); \
   evas_object_smart_callback_add(check2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_box_pack_end(box, check2); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2CHEACK_STATE_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *box, *check1, *check2; \
   Eina_List *nodes; \
   Eina_Bool value; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   check1 = eina_list_nth(nodes, 0); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check1, value); \
   evas_object_smart_callback_del_full(check1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(check1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   check2 = eina_list_nth(nodes, 1); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check2, value); \
   evas_object_smart_callback_del_full(check2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(check2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   eina_list_free(nodes); \
}

#define ITEM_IM_BORDER_STATE_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   char buff[BUFF_MAX]; \
   int l, r, t, b; \
   edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                                   pd->part->curr_state, pd->part->curr_state_value, \
                                   &l, &r, &t, &b); \
   ITEM_ADD(parent, item, TEXT) \
   ENTRY_ADD(item, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_color); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
   elm_entry_entry_set(entry, buff); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_IM_BORDER_STATE_UPDATE(SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *entry; \
   char buff[BUFF_MAX]; \
   int l, r, t, b; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                                   pd->part->curr_state, pd->part->curr_state_value, \
                                   &l, &r, &t, &b); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
   elm_entry_entry_set(entry, buff); \
   evas_object_smart_callback_del_full(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(entry, "activated", _on_##SUB##_##VALUE##_change, pd); \
   return item; \
}
