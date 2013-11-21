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

#define OBJ_DATA "group_data"

static Elm_Entry_Filter_Accept_Set accept_color = {
     .accepted = "0123456789 ",
     .rejected = NULL
};
static Elm_Entry_Filter_Limit_Size limit_color = {
     .max_char_count = 15,
     .max_byte_count = 0
};

static Elm_Entry_Filter_Accept_Set accept_prop = {
   .accepted = NULL,
   .rejected = PROP_BANNED_SYMBOLS
};

/* group */
#define ITEM_SPINNER_CALLBACK(sub, value, type) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Group *group = (Group *)data; \
   type value = (type)elm_spinner_value_get(obj); \
   edje_edit_##sub##_##value##_set(group->obj, value); \
   group->isModify = true; \
}

#define ITEM_2SPINNER_GROUP_ADD(text, sub, value1, value2) \
Evas_Object * \
prop_item_##sub##_##value1##_##value2##_add(Evas_Object *parent, \
                                            Group *group, \
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
   elm_spinner_label_format_set(spinner1, "%.0f"); \
   elm_spinner_value_set(spinner1, edje_edit_##sub##_##value1##_get(group->obj)); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   elm_box_pack_end(box, spinner1); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##sub##_##value1##_change, group); \
   SPINNER_ADD(box, spinner2, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_value_set(spinner2, edje_edit_##sub##_##value2##_get(group->obj)); \
   elm_spinner_label_format_set(spinner2, "%.0f"); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   elm_box_pack_end(box, spinner2); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##sub##_##value2##_change, group); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2SPINNER_GROUP_UPDATE(sub, value1, value2) \
void prop_item_##sub##_##value1##_##value2##_update(Evas_Object *item, Group *group) \
{ \
   Evas_Object *box, *spinner1, *spinner2; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   spinner1 = eina_list_nth(nodes, 0); \
   elm_spinner_value_set(spinner1, edje_edit_##sub##_##value1##_get(group->obj)); \
   evas_object_smart_callback_del(spinner1, "changed", _on_##sub##_##value1##_change); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##sub##_##value1##_change, group); \
   spinner2 = eina_list_nth(nodes, 1); \
   elm_spinner_value_set(spinner2, edje_edit_##sub##_##value2##_get(group->obj)); \
   evas_object_smart_callback_del(spinner2, "changed", _on_##sub##_##value2##_change); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##sub##_##value2##_change, group); \
   eina_list_free(nodes); \
}

/* part */
#define ITEM_STRING_PART_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   if (strcmp(value, "") == 0) value = NULL; \
     if (!edje_edit_##sub##_##value##_set(group->obj, part->name, value)) \
       { \
         NOTIFY_INFO(5, "Wrong input value for"# value" field"); \
         return; \
       } \
   edje_edit_##sub##_##value##_set(group->obj, part->name, value); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part); \
}

#define ITEM_CHECK_PART_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   Eina_Bool value = elm_check_state_get(obj); \
   edje_edit_##sub##_##value##_set(group->obj, part->name, value); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part); \
}

#define ITEM_INT_PART_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   int drag = elm_spinner_value_get(obj); \
   edje_edit_##sub##_##value##_set(group->obj, part->name, drag); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part); \
}

#define ITEM_1ENTRY_PART_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_prop); \
   elm_entry_entry_set(entry, edje_edit_##sub##_##value##_get(group->obj, part->name)); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1ENTRY_PART_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *entry; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   elm_entry_entry_set(entry, edje_edit_##sub##_##value##_get(group->obj, part->name)); \
   evas_object_data_del(entry, OBJ_DATA); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   evas_object_smart_callback_del(entry, "activated", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
}

#define ITEM_1CHEACK_PART_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(parent, check, DEFAULT_STYLE) \
   elm_check_state_set(check, edje_edit_##sub##_##value##_get(group->obj, part->name)); \
   evas_object_data_set(check, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", _on_##sub##_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1CHEACK_PART_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *check; \
   check = elm_object_part_content_get(item, "elm.swallow.content"); \
   elm_check_state_set(check, edje_edit_##sub##_##value##_get(group->obj, part->name)); \
   evas_object_data_del(check, OBJ_DATA); \
   evas_object_data_set(check, OBJ_DATA, group); \
   evas_object_smart_callback_del(check, "changed", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(check, "changed", _on_##sub##_##value##_change, part); \
}

#define ITEM_DRAG_PART_ADD(text, sub, value1, value2) \
Evas_Object * \
prop_item_##sub##_##value1##_##value2##_add(Evas_Object *parent, \
                                            Group *group, \
                                            Part *part, \
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
   CHECK_ADD(box, check, DEFAULT_STYLE) \
   ch_value = edje_edit_##sub##_##value1##_get(group->obj, part->name); \
   elm_check_state_set(check, ch_value); \
   evas_object_data_set(check, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check, tooltip1); \
   evas_object_smart_callback_add(check, "changed", _on_##sub##_##value1##_change, part); \
   elm_box_pack_end(box, check); \
   SPINNER_ADD(box, spinner, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner, fmt); \
   st_value = edje_edit_##sub##_##value2##_get(group->obj, part->name); \
   elm_spinner_value_set(spinner, st_value); \
   evas_object_data_set(spinner, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner, tooltip2); \
   evas_object_smart_callback_add(spinner, "changed", _on_##sub##_##value2##_change, part); \
   elm_box_pack_end(box, spinner); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_DRAG_PART_UPDATE(sub, value1, value2) \
void \
prop_item_##sub##_##value1##_##value2##_update(Evas_Object *item, \
                                               Group *group, \
                                               Part *part) \
{ \
   Evas_Object *box, *check, *spinner; \
   int ch_value, st_value; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   check = eina_list_nth(nodes, 0); \
   ch_value = edje_edit_##sub##_##value1##_get(group->obj, part->name); \
   elm_check_state_set(check, ch_value); \
   evas_object_data_del(check, OBJ_DATA); \
   evas_object_data_set(check, OBJ_DATA, group); \
   evas_object_smart_callback_del(check, "changed", _on_##sub##_##value1##_change); \
   evas_object_smart_callback_add(check, "changed", _on_##sub##_##value1##_change, part); \
   spinner = eina_list_nth(nodes, 1); \
   st_value = edje_edit_##sub##_##value2##_get(group->obj, part->name); \
   elm_spinner_value_set(spinner, st_value); \
   evas_object_data_del(spinner, OBJ_DATA); \
   evas_object_data_set(spinner, OBJ_DATA, group); \
   evas_object_smart_callback_del(spinner, "changed", _on_##sub##_##value2##_change); \
   evas_object_smart_callback_add(spinner, "changed", _on_##sub##_##value2##_change, part); \
   eina_list_free(nodes); \
}

/* state */
#define ITEM_STRING_STATE_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   Eina_Bool res; \
   if (strcmp(value, "") == 0) value = NULL; \
   res = edje_edit_##sub##_##value##_set(group->obj, part->name, \
                                         part->curr_state, part->curr_state_value, \
                                         value); \
   if (!res) \
     { \
        NOTIFY_INFO(5, "Wrong input value for"# value" field"); \
           return; \
     } \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part);\
}

#define ITEM_SPINNER_STATE_CALLBACK(sub, value, type) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   type value = (type)elm_spinner_value_get(obj); \
   edje_edit_##sub##_##value##_set(group->obj, part->name,\
                                   part->curr_state, part->curr_state_value, \
                                   value); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part);\
}

#define ITEM_COLOR_STATE_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                              Evas_Object *obj, \
                              void *ei __UNUSED__) \
{ \
   int r, g, b, a; \
   unsigned int tok_elm; \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   char **c = eina_str_split_full (value, " ", 4, &tok_elm); \
   if (tok_elm < 4) \
     { \
        free(c[0]); \
        free(c); \
        NOTIFY_ERROR ("Please input correct color data:\n r g b a") \
        return; \
     } \
   Evas_Object *box, *image; \
   Eina_List *nodes = NULL; \
   r = atoi(c[0]); g = atoi(c[1]); b = atoi(c[2]); a = atoi(c[3]); \
   edje_edit_##sub##_##value##_set(group->obj, part->name, \
                                   part->curr_state, part->curr_state_value, \
                                   r, g, b, a); \
   evas_object_color_set(part->obj, r*a/255, g*a/255, b*a/255, a); \
   box = elm_object_parent_widget_get(obj); \
   nodes = elm_box_children_get(box); \
   image = eina_list_nth(nodes, 1); \
   evas_object_color_set(image, r*a/255, g*a/255, b*a/255, a); \
   free(c[0]); \
   free(c); \
   eina_list_free(nodes); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part); \
}

#define ITEM_CHECK_STATE_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   Eina_Bool value = elm_check_state_get(obj); \
   edje_edit_##sub##_##value##_set(group->obj, part->name, \
                                   part->curr_state, part->curr_state_value, \
                                   value); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part);\
}

#define ITEM_IM_BORDER_STATE_CALLBACK(sub, value) \
static void \
_on_##sub##_##value##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   unsigned int tok_elm; \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   char **c = eina_str_split_full (value, " ", 4, &tok_elm); \
   if (tok_elm < 4) \
    { \
       free(c[0]); \
       free(c); \
       NOTIFY_ERROR ("Please input correct border data: l r t b, \
       where l - left, r - right, t - top, b - bottom borders") \
       return; \
    } \
   edje_edit_##sub##_##value##_set(group->obj, part->name, \
                                part->curr_state, part->curr_state_value, \
                                atoi(c[0]), atoi(c[1]), atoi(c[2]), atoi(c[3])); \
   free(c[0]); \
   free(c); \
   group->isModify = true; \
   evas_object_smart_callback_call(group->obj, "group,update", part);\
}

#define ITEM_2SPINNER_STATE_ADD(text, sub, value1, value2) \
Evas_Object * \
prop_item_##sub##_##value1##_##value2##_add(Evas_Object *parent, \
                                            Group *group, \
                                            Part *part, \
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
   value = edje_edit_##sub##_##value1##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_spinner_value_set(spinner1, value); \
   elm_box_pack_end(box, spinner1); \
   evas_object_data_set(spinner1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##sub##_##value1##_change, part); \
   SPINNER_ADD(box, spinner2, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner2, fmt); \
   value = edje_edit_##sub##_##value2##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_spinner_value_set(spinner2, value); \
   elm_box_pack_end(box, spinner2); \
   evas_object_data_set(spinner2, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  _on_##sub##_##value2##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item;\
}

#define ITEM_2SPINNER_STATE_UPDATE(sub, value1, value2) \
void \
prop_item_##sub##_##value1##_##value2##_update(Evas_Object *item, \
                                               Group *group, \
                                               Part *part) \
{ \
   Evas_Object *box, *spinner1, *spinner2; \
   double value; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   spinner1 = eina_list_nth(nodes, 0); \
   value = edje_edit_##sub##_##value1##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_spinner_value_set(spinner1, value); \
   evas_object_data_del(spinner1, OBJ_DATA); \
   evas_object_data_set(spinner1, OBJ_DATA, group); \
   evas_object_smart_callback_del(spinner1, "changed", _on_##sub##_##value1##_change); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##sub##_##value1##_change, part); \
   spinner2 = eina_list_nth(nodes, 1); \
   value = edje_edit_##sub##_##value2##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_spinner_value_set(spinner2, value); \
   evas_object_data_del(spinner2, OBJ_DATA); \
   evas_object_data_set(spinner2, OBJ_DATA, group); \
   evas_object_smart_callback_del(spinner2, "changed", _on_##sub##_##value2##_change); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##sub##_##value2##_change, part); \
   eina_list_free(nodes); \
}

#define ITEM_1ENTRY_STATE_ADD(text, sub, value, func) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
                                const char *tooltip, \
                                const char *btn_tooltip) \
{ \
   Evas_Object *item, *entry, *box, *btn; \
   const char *value; \
   value = edje_edit_##sub##_##value##_get(group->obj, part->name, \
                                           part->curr_state, \
                                           part->curr_state_value); \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, true, false) \
   ENTRY_ADD(parent, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_prop); \
   elm_box_pack_end(box, entry); \
   btn = elm_button_add(parent); \
   elm_object_text_set(btn, "..."); \
   elm_object_style_set(btn, DEFAULT_STYLE); \
   evas_object_show(btn); \
   elm_box_pack_end(box, btn); \
   evas_object_smart_callback_add(btn, "clicked", func, entry); \
   elm_entry_entry_set(entry, value); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   elm_object_tooltip_text_set(btn, btn_tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_1ENTRY_STATE_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *entry, *box; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   entry = eina_list_nth(nodes, 0); \
   const char *value = edje_edit_##sub##_##value##_get(group->obj, part->name, \
                                                       part->curr_state, \
                                                       part->curr_state_value); \
   elm_entry_entry_set(entry, value); \
   evas_object_data_del(entry, OBJ_DATA); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   evas_object_smart_callback_del(entry, "activated", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   eina_list_free(nodes); \
}

#define ITEM_COLOR_STATE_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *box, *entry, *image; \
   char buff[BUFF_MAX]; \
   int r, g, b, a; \
   edje_edit_##sub##_##value##_get(group->obj, part->name, \
                                   part->curr_state, part->curr_state_value, \
                                   &r, &g, &b, &a); \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, true, false) \
   ENTRY_ADD(box, entry, true, DEFAULT_STYLE) \
   IMAGE_ADD(box, image, TET_IMG_PATH"bg_demo.png"); \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_color); \
   elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limit_color); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", r, g, b, a); \
   elm_entry_entry_set(entry, buff); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   evas_object_color_set(image, r*a/255, g*a/255, b*a/255, a); \
   elm_box_pack_end(box, entry); \
   elm_box_pack_end(box, image); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_COLOR_STATE_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *box, *entry, *image; \
   char buff[BUFF_MAX]; \
   int r, g, b, a; \
   Eina_List *nodes; \
   edje_edit_##sub##_##value##_get(group->obj, part->name, \
                                   part->curr_state, part->curr_state_value, \
                                   &r, &g, &b, &a); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", r, g, b, a); \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   entry = eina_list_nth(nodes, 0); \
   elm_entry_entry_set(entry, buff); \
   evas_object_data_del(entry, OBJ_DATA); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   evas_object_smart_callback_del(entry, "activated", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   image = eina_list_nth(nodes, 1); \
   evas_object_color_set(image, r*a/255, g*a/255, b*a/255, a); \
   eina_list_free(nodes); \
}

#define ITEM_2ENTRY_STATE_ADD(text, sub, value1, value2) \
Evas_Object * \
prop_item_##sub##_##value1##_##value2##_add(Evas_Object *parent, \
                                            Group *group, \
                                            Part *part, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *entry1, *entry2; \
   Eina_Stringshare *value; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, true, true) \
   elm_box_padding_set(box, 5, 0); \
   ENTRY_ADD(box, entry1, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry1, elm_entry_filter_accept_set, &accept_prop); \
   ENTRY_ADD(box, entry2, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry2, elm_entry_filter_accept_set, &accept_prop); \
   value = edje_edit_##sub##_##value1##_get(group->obj, \
                                          part->name, \
                                          part->curr_state, \
                                          part->curr_state_value); \
   elm_entry_entry_set(entry1, value); \
   evas_object_data_set(entry1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry1, tooltip1); \
   evas_object_smart_callback_add(entry1, "activated", _on_##sub##_##value1##_change, part); \
   elm_box_pack_end(box, entry1); \
   edje_edit_string_free(value); \
   value = edje_edit_##sub##_##value2##_get(group->obj, \
                                          part->name, \
                                          part->curr_state, \
                                          part->curr_state_value); \
   elm_entry_entry_set(entry2, value); \
   evas_object_data_set(entry2, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry2, tooltip2); \
   evas_object_smart_callback_add(entry2, "activated", _on_##sub##_##value2##_change, part); \
   elm_box_pack_end(box, entry2); \
   edje_edit_string_free(value); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2ENTRY_STATE_UPDATE(sub, value1, value2) \
void \
prop_item_##sub##_##value1##_##value2##_update(Evas_Object *item, \
                                               Group *group, \
                                               Part *part) \
{ \
   Evas_Object *box, *entry1, *entry2; \
   Eina_Stringshare *value; \
   Eina_List *nodes; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   value = edje_edit_##sub##_##value1##_get(group->obj, \
                                          part->name, \
                                          part->curr_state, \
                                          part->curr_state_value); \
   entry1 = eina_list_nth(nodes, 0); \
   elm_entry_entry_set(entry1, value); \
   evas_object_data_del(entry1, OBJ_DATA); \
   evas_object_data_set(entry1, OBJ_DATA, group); \
   evas_object_smart_callback_del(entry1, "activated", _on_##sub##_##value1##_change); \
   evas_object_smart_callback_add(entry1, "activated", _on_##sub##_##value1##_change, part); \
   edje_edit_string_free(value); \
   value = edje_edit_##sub##_##value2##_get(group->obj, \
                                          part->name, \
                                          part->curr_state, \
                                          part->curr_state_value); \
   entry2 = eina_list_nth(nodes, 1); \
   elm_entry_entry_set(entry2, value); \
   evas_object_data_del(entry2, OBJ_DATA); \
   evas_object_data_set(entry2, OBJ_DATA, group); \
   evas_object_smart_callback_del(entry2, "activated", _on_##sub##_##value2##_change); \
   evas_object_smart_callback_add(entry2, "activated", _on_##sub##_##value2##_change, part); \
   edje_edit_string_free(value); \
   eina_list_free(nodes); \
}

#define ITEM_1SPINNER_STATE_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
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
   value = edje_edit_##sub##_##value##_get(group->obj, \
                                           part->name, \
                                           part->curr_state, \
                                           part->curr_state_value); \
   elm_spinner_value_set(spinner, value); \
   evas_object_data_set(spinner, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner, tooltip); \
   evas_object_smart_callback_add(spinner, "changed", _on_##sub##_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", spinner); \
   return item;\
}

#define ITEM_1SPINNER_STATE_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *spinner; \
   double value; \
   spinner = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##sub##_##value##_get(group->obj, \
                                           part->name, \
                                           part->curr_state, \
                                           part->curr_state_value); \
   elm_spinner_value_set(spinner, value); \
   evas_object_data_del(spinner, OBJ_DATA); \
   evas_object_data_set(spinner, OBJ_DATA, group); \
   evas_object_smart_callback_del(spinner, "changed", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(spinner, "changed", _on_##sub##_##value##_change, part); \
}

#define ITEM_1CHEACK_STATE_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *check; \
   Eina_Bool value; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(item, check, DEFAULT_STYLE) \
   value = edje_edit_##sub##_##value##_get(group->obj, \
                                           part->name, \
                                           part->curr_state, \
                                           part->curr_state_value); \
   elm_check_state_set(check, value); \
   evas_object_data_set(check, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", _on_##sub##_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1CHEACK_STATE_UPDATE(sub, value) \
void \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *check; \
   Eina_Bool value; \
   check = elm_object_part_content_get(item, "elm.swallow.content"); \
   value = edje_edit_##sub##_##value##_get(group->obj, \
                                           part->name, \
                                           part->curr_state, \
                                           part->curr_state_value); \
   elm_check_state_set(check, value); \
   evas_object_data_del(check, OBJ_DATA); \
   evas_object_data_set(check, OBJ_DATA, group); \
   evas_object_smart_callback_del(check, "changed", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(check, "changed", _on_##sub##_##value##_change, part); \
}

#define ITEM_2CHEACK_STATE_ADD(text, sub, value1, value2) \
Evas_Object * \
prop_item_##sub##_##value1##_##value2##_add(Evas_Object *parent, \
                                            Group *group, \
                                            Part *part, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *check1, *check2; \
   Eina_Bool value; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, true, true) \
   CHECK_ADD(box, check1, DEFAULT_STYLE) \
   CHECK_ADD(box, check2, DEFAULT_STYLE) \
   value = edje_edit_##sub##_##value1##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_check_state_set(check1, value); \
   evas_object_data_set(check1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check1, tooltip1); \
   evas_object_smart_callback_add(check1, "changed", _on_##sub##_##value1##_change, part); \
   elm_box_pack_end(box, check1); \
   value = edje_edit_##sub##_##value2##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_check_state_set(check2, value); \
   evas_object_data_set(check2, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check2, tooltip2); \
   evas_object_smart_callback_add(check2, "changed", _on_##sub##_##value2##_change, part); \
   elm_box_pack_end(box, check2); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2CHEACK_STATE_UPDATE(sub, value1, value2) \
void \
prop_item_##sub##_##value1##_##value2##_update(Evas_Object *item, \
                                               Group *group, \
                                               Part *part) \
{ \
   Evas_Object *box, *check1, *check2; \
   Eina_List *nodes; \
   Eina_Bool value; \
   box = elm_object_part_content_get(item, "elm.swallow.content"); \
   nodes = elm_box_children_get(box); \
   check1 = eina_list_nth(nodes, 0); \
   value = edje_edit_##sub##_##value1##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_check_state_set(check1, value); \
   evas_object_data_del(check1, OBJ_DATA); \
   evas_object_data_set(check1, OBJ_DATA, group); \
   evas_object_smart_callback_del(check1, "changed", _on_##sub##_##value1##_change); \
   evas_object_smart_callback_add(check1, "changed", _on_##sub##_##value1##_change, part); \
   check2 = eina_list_nth(nodes, 1); \
   value = edje_edit_##sub##_##value2##_get(group->obj, \
                                            part->name, \
                                            part->curr_state, \
                                            part->curr_state_value); \
   elm_check_state_set(check2, value); \
   evas_object_data_del(check2, OBJ_DATA); \
   evas_object_data_set(check2, OBJ_DATA, group); \
   evas_object_smart_callback_del(check2, "changed", _on_##sub##_##value2##_change); \
   evas_object_smart_callback_add(check2, "changed", _on_##sub##_##value2##_change, part); \
   eina_list_free(nodes); \
}

#define ITEM_IM_BORDER_STATE_ADD(text, sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_add(Evas_Object *parent, \
                                Group *group, \
                                Part *part, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   char buff[BUFF_MAX]; \
   int l, r, t, b; \
   edje_edit_##sub##_##value##_get(group->obj, part->name, \
                                   part->curr_state, part->curr_state_value, \
                                   &l, &r, &t, &b); \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(item, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_color); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
   elm_entry_entry_set(entry, buff); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_IM_BORDER_STATE_UPDATE(sub, value) \
Evas_Object * \
prop_item_##sub##_##value##_update(Evas_Object *item, \
                                   Group *group, \
                                   Part *part) \
{ \
   Evas_Object *entry; \
   char buff[BUFF_MAX]; \
   int l, r, t, b; \
   entry = elm_object_part_content_get(item, "elm.swallow.content"); \
   edje_edit_##sub##_##value##_get(group->obj, part->name, \
                                   part->curr_state, part->curr_state_value, \
                                   &l, &r, &t, &b); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
   elm_entry_entry_set(entry, buff); \
   evas_object_data_del(entry, OBJ_DATA); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   evas_object_smart_callback_del(entry, "activated", _on_##sub##_##value##_change); \
   evas_object_smart_callback_add(entry, "activated", _on_##sub##_##value##_change, part); \
   return item; \
}
