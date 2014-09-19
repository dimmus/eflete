/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
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

#include "string_macro.h"
#include "colorsel.h"

static Elm_Entry_Filter_Accept_Set accept_prop = {
   .accepted = NULL,
   .rejected = PART_NAME_BANNED_SYMBOLS
};

static void
_on_state_color_class_change(void *data,
                             Evas_Object *obj,
                             void *event_info __UNUSED__);
static void
_on_state_text_style_change(void *data,
                             Evas_Object *obj,
                             void *event_info __UNUSED__);
#define ITEM1 "item1"
#define ITEM2 "item2"

#define IMAGE_NAME_REGEX "^([a-z0-9\\._\\-]*\\.(png|jpg|jpeg|gif|tiff|tga|jp2k|bmp|svg))*$"
#define IMAGE_BORDER_REGEX "^([0-9]+( [0-9]+){3}){0,1}$"

#define ITEM_CONTEINER_1LABEL_ADD(PARENT, ITEM, TEXT1) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, 1.0, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(ITEM, EFLETE_EDJ, "eflete/prop/container/1label"); \
   elm_object_part_text_set(ITEM, "eflete.text.start", TEXT1); \
   evas_object_show(ITEM);

#define ITEM_CONTEINER_2LABEL_ADD(PARENT, ITEM, TEXT1, TEXT2) \
   ITEM = elm_layout_add(PARENT); \
   evas_object_size_hint_weight_set(ITEM, 1.0, 0.0); \
   evas_object_size_hint_align_set(ITEM, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(ITEM, EFLETE_EDJ, "eflete/prop/container/2label"); \
   elm_object_part_text_set(ITEM, "eflete.text.start", TEXT1); \
   elm_object_part_text_set(ITEM, "eflete.text.end", TEXT2); \
   evas_object_show(ITEM);

#define ITEM_SPINNER_INT_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   int value = (int)elm_spinner_value_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, value)) \
     return; \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

/* group */
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
   Evas_Object *item, *box, *layout, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text, "eflete/property/item/default") \
   BOX_ADD(item, box, true, false); \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, "w:", "px"); \
   SPINNER_ADD(layout, spinner1, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner1, "%.0f"); \
   elm_spinner_value_set(spinner1, edje_edit_##SUB##_##VALUE1##_get(pd->style->obj)); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   elm_object_part_content_set(layout, "eflete.content", spinner1); \
   elm_box_pack_end(box, layout); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, "h:", "px"); \
   SPINNER_ADD(layout, spinner2, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_value_set(spinner2, edje_edit_##SUB##_##VALUE2##_get(pd->style->obj)); \
   elm_spinner_label_format_set(spinner2, "%.0f"); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   elm_object_part_content_set(layout, "eflete.content", spinner2); \
   elm_box_pack_end(box, layout); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   evas_object_data_set(item, ITEM1, spinner1); \
   evas_object_data_set(item, ITEM2, spinner2); \
   return item; \
}

#define ITEM_2SPINNER_GROUP_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, Prop_Data *pd) \
{ \
   Evas_Object *spinner1, *spinner2; \
   spinner1 = evas_object_data_get(item, ITEM1); \
   elm_spinner_value_set(spinner1, edje_edit_##SUB##_##VALUE1##_get(pd->style->obj)); \
   evas_object_smart_callback_del_full(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   spinner2 = evas_object_data_get(item, ITEM2); \
   elm_spinner_value_set(spinner2, edje_edit_##SUB##_##VALUE2##_get(pd->style->obj)); \
   evas_object_smart_callback_del_full(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   pm_project_changed(app_data_get()->project); \
}

/* part */
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
   pm_project_changed(app_data_get()->project); \
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
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

/* combobox */
#define ITEM_1COMBOBOX_PART_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                              Evas_Object *obj EINA_UNUSED, \
                              void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   if (item->index != 0) edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, item->title); \
   else edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, NULL); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_1COMBOBOX_PART_ADD(TYPE, TEXT, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *combobox; \
   Part *part; \
   Edje_Part_Type type; \
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   EWE_COMBOBOX_ADD(parent, combobox) \
   const char *value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("None")); \
   ewe_combobox_item_add(combobox, _("None")); \
   EINA_INLIST_FOREACH(pd->style->parts, part) \
     { \
        type = edje_edit_part_type_get(pd->style->obj, part->name); \
        switch(TYPE) \
          { \
             case CLIP_TO: \
                { \
                   if (!strcmp(wm_part_type_get(type), "RECTANGLE") && (strcmp(pd->part->name, part->name))) \
                      ewe_combobox_item_add(combobox, part->name); \
                } \
                break; \
             case FORWARD_EVENTS: \
                { \
                   if ((edje_edit_part_drag_x_get(pd->style->obj, part->name) || edje_edit_part_drag_y_get(pd->style->obj, part->name)) && (strcmp(pd->part->name, part->name))) \
                      ewe_combobox_item_add(combobox, part->name); \
                } \
                break; \
             case SOURCE: \
                { \
                   Eina_List *collections, *l; \
                   char *group; \
                   App_Data *ap = app_data_get(); \
                   collections = edje_file_collection_list(ap->project->dev); \
                   EINA_LIST_FOREACH(collections, l, group) \
                   { \
                      ewe_combobox_item_add(combobox, group); \
                   } \
                } \
                break; \
             default: \
                if (strcmp(pd->part->name, part->name)) \
                   ewe_combobox_item_add(combobox, part->name); \
          } \
     } \
   elm_object_tooltip_text_set(combobox, tooltip); \
   evas_object_smart_callback_add(combobox, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", combobox); \
   evas_object_data_set(item, ITEM1, combobox); \
   return item; \
}

#define ITEM_1COMBOBOX_PART_UPDATE(TYPE, text, SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *combobox; \
   Part *part; \
   Edje_Part_Type type; \
   combobox = evas_object_data_get(item, ITEM1); \
   ewe_combobox_items_list_free(combobox, true); \
   const char *value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("None")); \
   ewe_combobox_item_add(combobox, _("None")); \
   EINA_INLIST_FOREACH(pd->style->parts, part) \
     { \
        type = edje_edit_part_type_get(pd->style->obj, part->name); \
        switch(TYPE) \
          { \
             case CLIP_TO: \
                { \
                   if (!strcmp(wm_part_type_get(type), "RECTANGLE") && (strcmp(pd->part->name, part->name))) \
                      ewe_combobox_item_add(combobox, part->name); \
                } \
                break; \
             case FORWARD_EVENTS: \
                { \
                   if ((edje_edit_part_drag_x_get(pd->style->obj, part->name) || edje_edit_part_drag_y_get(pd->style->obj, part->name)) && (strcmp(pd->part->name, part->name))) \
                      ewe_combobox_item_add(combobox, part->name); \
                } \
                break; \
             case SOURCE: \
                { \
                   Eina_List *collections, *l; \
                   char *group; \
                   App_Data *ap = app_data_get(); \
                   collections = edje_file_collection_list(ap->project->dev); \
                   EINA_LIST_FOREACH(collections, l, group) \
                   { \
                      ewe_combobox_item_add(combobox, group); \
                   } \
                } \
                break; \
             default: \
                if (strcmp(pd->part->name, part->name)) \
                   ewe_combobox_item_add(combobox, part->name); \
          } \
     } \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_1COMBOBOX_PART_TEXTBLOCK_CALLBACK(SUB, VALUE, TYPE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                              Evas_Object *obj EINA_UNUSED, \
                              void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, (TYPE)item->index)) \
     return; \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_1COMBOBOX_PART_TEXTBLOCK_ADD(TEXT, SUB, VALUE, TYPE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip, \
                                const char **entries) \
{ \
   Evas_Object *item, *combobox; \
   TYPE value; \
   int i = 0; \
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   EWE_COMBOBOX_ADD(parent, combobox) \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name); \
   while (entries[i]) \
    { \
       ewe_combobox_item_add(combobox, entries[i]); \
       i++; \
    } \
   ewe_combobox_select_item_set(combobox, value); \
   elm_object_tooltip_text_set(combobox, tooltip); \
   evas_object_smart_callback_add(combobox, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", combobox); \
   return item; \
}

#define ITEM_1COMBOBOX_PART_TEXTBLOCK_UPDATE(text, SUB, VALUE, TYPE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *combobox = elm_object_part_content_get(item, "elm.swallow.content"); \
   TYPE value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name); \
   ewe_combobox_select_item_set(combobox, value); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_1CHEACK_PART_ADD(text, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *layout, *check; \
   ITEM_ADD(parent, item, text, "eflete/property/item/default") \
   ITEM_CONTEINER_1LABEL_ADD(item, layout, NULL); \
   CHECK_ADD(layout, check, DEFAULT_STYLE) \
   elm_check_state_set(check, edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name)); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", check); \
   elm_object_part_content_set(item, "elm.swallow.content", layout); \
   evas_object_data_set(item, ITEM1, check); \
   return item; \
}

#define ITEM_1CHEACK_PART_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *check; \
   check = evas_object_data_get(item, ITEM1); \
   elm_check_state_set(check, edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name)); \
   evas_object_smart_callback_del_full(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   pm_project_changed(app_data_get()->project); \
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
   Evas_Object *item, *box, *layout, *check, *spinner; \
   int ch_value, st_value; \
   ITEM_ADD(parent, item, text, "eflete/property/item/default") \
   BOX_ADD(item, box, true, true) \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, "turn", NULL); \
   CHECK_ADD(layout, check, "eflete/toggle") \
   ch_value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, pd->part->name); \
   elm_check_state_set(check, ch_value); \
   elm_object_tooltip_text_set(check, tooltip1); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", check); \
   elm_box_pack_end(box, layout); \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, NULL, "px"); \
   SPINNER_ADD(box, spinner, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner, fmt); \
   st_value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, pd->part->name); \
   elm_spinner_value_set(spinner, st_value); \
   elm_object_tooltip_text_set(spinner, tooltip2); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", spinner); \
   elm_box_pack_end(box, layout); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   evas_object_data_set(item, ITEM1, check); \
   evas_object_data_set(item, ITEM2, spinner); \
   return item; \
}

#define ITEM_DRAG_PART_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *check, *spinner; \
   int ch_value, st_value; \
   check = evas_object_data_get(item, ITEM1); \
   ch_value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, pd->part->name); \
   elm_check_state_set(check, ch_value); \
   evas_object_smart_callback_del_full(check, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   spinner = evas_object_data_get(item, ITEM2); \
   st_value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, pd->part->name); \
   elm_spinner_value_set(spinner, st_value); \
   evas_object_smart_callback_del_full(spinner, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   pm_project_changed(app_data_get()->project); \
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
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value, \
                                        value)) \
     { \
        NOTIFY_INFO(5, "Wrong input value for "#VALUE" field."); \
        return; \
     } \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

/* combobox */
#define ITEM_1COMBOBOX_STATE_PART_CALLBACK(SUB, VALUE, TYPE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj EINA_UNUSED, \
                             void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
     pd->part->curr_state, pd->part->curr_state_value, (TYPE)item->index); \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_1COMBOBOX_STATE_PART_ADD(TEXT, SUB, VALUE, TYPE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip, \
                                const char **entries) \
{ \
   Evas_Object *item, *combobox; \
   TYPE value; \
   int i = 0; \
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   EWE_COMBOBOX_ADD(parent, combobox) \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
             pd->part->curr_state, pd->part->curr_state_value); \
   while (entries[i]) \
    { \
       ewe_combobox_item_add(combobox, entries[i]); \
       i++; \
    } \
   ewe_combobox_select_item_set(combobox, value); \
   elm_object_tooltip_text_set(combobox, tooltip); \
   evas_object_smart_callback_add(combobox, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", combobox); \
   return item; \
}

#define ITEM_1COMBOBOX_STATE_PART_UPDATE(TEXT, SUB, VALUE, TYPE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *combobox = elm_object_part_content_get(item, "elm.swallow.content"); \
   TYPE value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, pd->part->name, \
                  pd->part->curr_state, pd->part->curr_state_value); \
   ewe_combobox_select_item_set(combobox, value); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_COMBOBOX_STATE_CALLBACK(TYPE, TEXT, SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj EINA_UNUSED, \
                             void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   if (item->index != 0) edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value, \
                                        item->title); \
   else edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                                        pd->part->curr_state, pd->part->curr_state_value, \
                                        NULL); \
   if (TYPE != TEXT_STYLE) \
     prop_item_state_text_update(pd->prop_state_text.text, pd); \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_SPINNER_STATE_INT_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   int value = (int)elm_spinner_value_get(obj); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name,\
                                        pd->part->curr_state, \
                                        pd->part->curr_state_value, \
                                        value)) \
     return; \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_SPINNER_STATE_DOUBLE_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   double value = elm_spinner_value_get(obj); \
   value /= 100; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name,\
                                        pd->part->curr_state, \
                                        pd->part->curr_state_value, \
                                        value)) \
     return; \
   pm_project_changed(app_data_get()->project); \
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
   edje_edit_state_color_class_set(pd->style->obj, pd->part->name, \
                                   pd->part->curr_state, \
                                   pd->part->curr_state_value, NULL); \
   prop_item_state_color_class_update(pd->prop_state.color_class, pd); \
   pm_project_changed(app_data_get()->project); \
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
   pm_project_changed(app_data_get()->project); \
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
   char **c = NULL; \
   Prop_Data *pd = (Prop_Data *)data; \
   const char *value = elm_entry_entry_get(obj); \
   if (!value || !strcmp(value, "")) \
     { \
        elm_object_part_text_set(obj, "elm.guide", "left right top bottom"); \
        edje_edit_##SUB##_##VALUE##_set(pd->style->obj, pd->part->name, \
                           pd->part->curr_state, pd->part->curr_state_value, \
                           0, 0, 0, 0); \
     } \
   else \
     { \
        c = eina_str_split_full (value, " ", 4, &tok_elm); \
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
     } \
   pm_project_changed(app_data_get()->project); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->style->isModify = true; \
}

#define ITEM_2SPINNER_STATE_ADD(text, SUB, VALUE1, VALUE2, STYLE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            double min, \
                                            double max, \
                                            double step, \
                                            const char *fmt, \
                                            const char *sp1_lb_start, \
                                            const char *sp1_lb_end, \
                                            const char *sp2_lb_start, \
                                            const char *sp2_lb_end, \
                                            const char *tooltip1, \
                                            const char *tooltip2, \
                                            Eina_Bool to_percent) \
{ \
   Evas_Object *item, *box, *layout, *spinner1, *spinner2; \
   double value; \
   ITEM_ADD(parent, item, text, STYLE) \
   BOX_ADD(item, box, true, true) \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, sp1_lb_start, sp1_lb_end); \
   SPINNER_ADD(layout, spinner1, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner1, fmt); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   if (to_percent) value *= 100; \
   elm_spinner_value_set(spinner1, value); \
   elm_object_part_content_set(layout, "eflete.content", spinner1); \
   elm_box_pack_end(box, layout); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, sp2_lb_start, sp2_lb_end); \
   SPINNER_ADD(layout, spinner2, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner2, fmt); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   if (to_percent) value *= 100; \
   elm_spinner_value_set(spinner2, value); \
   elm_object_part_content_set(layout, "eflete.content", spinner2); \
   elm_box_pack_end(box, layout); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  _on_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   evas_object_data_set(item, ITEM1, spinner1); \
   evas_object_data_set(item, ITEM2, spinner2); \
   return item;\
}

#define ITEM_2SPINNER_STATE_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd, \
                                               Eina_Bool to_percent) \
{ \
   Evas_Object *spinner1, *spinner2; \
   double value; \
   spinner1 = evas_object_data_get(item, ITEM1); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   if (to_percent) value *= 100; \
   elm_spinner_value_set(spinner1, value); \
   evas_object_smart_callback_del_full(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(spinner1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   spinner2 = evas_object_data_get(item, ITEM2); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   if (to_percent) value *= 100; \
   elm_spinner_value_set(spinner2, value); \
   evas_object_smart_callback_del_full(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(spinner2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_1ENTRY_STATE_ADD(TEXT, SUB, VALUE, FILTER) \
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
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   EWE_ENTRY_ADD(item, entry, true, DEFAULT_STYLE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, FILTER); \
   if (btn_func_cb) \
     { \
        btn = elm_button_add(parent); \
        elm_object_style_set(btn, "eflete/elipsis"); \
        evas_object_show(btn); \
        evas_object_smart_callback_add(btn, "clicked", btn_func_cb, pd); \
        elm_object_part_content_set(entry, "elm.swallow.end", btn); \
        elm_object_tooltip_text_set(btn, btn_tooltip); \
     } \
   ewe_entry_entry_set(entry, value); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
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
   ewe_entry_entry_set(entry, value); \
   evas_object_smart_callback_del_full(entry, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(entry, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_1COMBOBOX_STATE_ADD(TYPE, TEXT, SUB, VALUE, LIST) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                void *func_cb, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *combobox; \
   Part *part; \
   Edje_Part_Type type; \
   Eina_List *list = NULL; \
   const char *ccname = NULL; \
   Eina_Stringshare *value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   EWE_COMBOBOX_ADD(item, combobox) \
   elm_object_tooltip_text_set(combobox, tooltip); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("None")); \
   ewe_combobox_item_add(combobox, _("None")); \
   if ((TYPE != COLOR_CLASS) && (TYPE != TEXT_STYLE)) \
     { \
        EINA_INLIST_FOREACH(pd->style->parts, part) \
          { \
             type = edje_edit_part_type_get(pd->style->obj, part->name); \
             if (!strcmp(wm_part_type_get(type), "TEXT") && (strcmp(pd->part->name, part->name))) \
               ewe_combobox_item_add(combobox, part->name); \
             evas_object_smart_callback_add(combobox, "selected", _on_##SUB##_##VALUE##_change, pd); \
          } \
     } \
   else \
     { \
        Eina_List *l = edje_edit_##LIST##_list_get(pd->style->obj); \
        EINA_LIST_FOREACH(l, list, ccname) \
          ewe_combobox_item_add(combobox, ccname); \
        evas_object_smart_callback_add(combobox, "selected", func_cb, pd); \
     } \
   elm_object_part_content_set(item, "elm.swallow.content", combobox); \
   return item; \
}

#define ITEM_1COMBOBOX_STATE_UPDATE(TYPE, TEXT, SUB, VALUE, LIST) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *combobox; \
   Part *part; \
   Edje_Part_Type type; \
   Eina_Stringshare *value; \
   Eina_List *list = NULL; \
   const char *ccname = NULL; \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   combobox = elm_object_part_content_get(item, "elm.swallow.content"); \
   ewe_combobox_items_list_free(combobox, true); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("None")); \
   ewe_combobox_item_add(combobox, _("None")); \
   if ((TYPE != COLOR_CLASS) && (TYPE != TEXT_STYLE)) \
     { \
        EINA_INLIST_FOREACH(pd->style->parts, part) \
          { \
             type = edje_edit_part_type_get(pd->style->obj, part->name); \
             if (!strcmp(wm_part_type_get(type), "TEXT") && (strcmp(pd->part->name, part->name))) \
               ewe_combobox_item_add(combobox, part->name); \
          } \
     } \
   else \
     { \
        Eina_List *cclist = edje_edit_##LIST##_list_get(pd->style->obj); \
        EINA_LIST_FOREACH(cclist, list, ccname) \
          ewe_combobox_item_add(combobox, ccname); \
     } \
   pm_project_changed(app_data_get()->project); \
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
   ITEM_ADD(parent, item, text, "eflete/property/item/default") \
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
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_2COMBOBOX_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *layout, *combobox; \
   Part *part; \
   Eina_Stringshare *value; \
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/relative_to") \
   BOX_ADD(item, box, false, true) \
   elm_box_padding_set(box, 0, 6); \
   ITEM_CONTEINER_1LABEL_ADD(box, layout, _("x:")); \
   EWE_COMBOBOX_ADD(layout, combobox) \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("Layout")); \
   ewe_combobox_item_add(combobox, _("Layout")); \
   EINA_INLIST_FOREACH(pd->style->parts, part) \
     { \
        if (strcmp(pd->part->name, part->name)) \
          ewe_combobox_item_add(combobox, part->name); \
     } \
   elm_object_tooltip_text_set(combobox, tooltip1); \
   elm_object_part_content_set(layout, "eflete.content", combobox); \
   elm_box_pack_end(box, layout); \
   edje_edit_string_free(value); \
   evas_object_smart_callback_add(combobox, "selected", _on_combobox_##SUB##_##VALUE1##_change, pd); \
   evas_object_data_set(item, ITEM1, combobox); \
   ITEM_CONTEINER_1LABEL_ADD(box, layout, _("y:")); \
   EWE_COMBOBOX_ADD(layout, combobox) \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("Layout")); \
   ewe_combobox_item_add(combobox, _("Layout")); \
   EINA_INLIST_FOREACH(pd->style->parts, part) \
     { \
        if (strcmp(pd->part->name, part->name)) \
          ewe_combobox_item_add(combobox, part->name); \
     } \
   elm_object_tooltip_text_set(combobox, tooltip2); \
   evas_object_smart_callback_add(combobox, "selected", _on_combobox_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", combobox); \
   elm_box_pack_end(box, layout); \
   edje_edit_string_free(value); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   evas_object_data_set(item, ITEM2, combobox); \
   return item; \
}

#define ITEM_2COMBOBOX_STATE_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *combobox; \
   Eina_Stringshare *value; \
   Part *part; \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   combobox = evas_object_data_get(item, ITEM1); \
   ewe_combobox_items_list_free(combobox, true); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("Layout")); \
   ewe_combobox_item_add(combobox, _("Layout")); \
   EINA_INLIST_FOREACH(pd->style->parts, part) \
     { \
        if (strcmp(pd->part->name, part->name)) \
          ewe_combobox_item_add(combobox, part->name); \
     } \
   edje_edit_string_free(value); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   combobox = evas_object_data_get(item, ITEM2); \
   ewe_combobox_items_list_free(combobox, true); \
   if (value) \
     ewe_combobox_text_set(combobox, value); \
   else \
     ewe_combobox_text_set(combobox, _("Layout")); \
   ewe_combobox_item_add(combobox, _("Layout")); \
   EINA_INLIST_FOREACH(pd->style->parts, part) \
     { \
        if (strcmp(pd->part->name, part->name)) \
          ewe_combobox_item_add(combobox, part->name); \
     } \
   pm_project_changed(app_data_get()->project); \
   edje_edit_string_free(value); \
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
   Evas_Object *item, *layout, *spinner; \
   double value; \
   ITEM_ADD(parent, item, text , "eflete/property/item/default") \
   ITEM_CONTEINER_2LABEL_ADD(item, layout, NULL, NULL); \
   SPINNER_ADD(layout, spinner, min, max, step, true, DEFAULT_STYLE) \
   elm_spinner_label_format_set(spinner, fmt); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_spinner_value_set(spinner, value); \
   elm_object_tooltip_text_set(spinner, tooltip); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", spinner); \
   elm_object_part_content_set(item, "elm.swallow.content", layout); \
   evas_object_data_set(item, ITEM1, spinner); \
   return item;\
}

#define ITEM_1SPINNER_STATE_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *spinner; \
   double value; \
   spinner = evas_object_data_get(item, ITEM1); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_spinner_value_set(spinner, value); \
   evas_object_smart_callback_del_full(spinner, "changed", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(spinner, "changed", _on_##SUB##_##VALUE##_change, pd); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_1CHEACK_STATE_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_item_##SUB##_##VALUE##_add(Evas_Object *parent, \
                                Prop_Data *pd, \
                                const char *tooltip) \
{ \
   Evas_Object *item, *layout, *check; \
   Eina_Bool value; \
   ITEM_ADD(parent, item, TEXT , "eflete/property/item/default") \
   ITEM_CONTEINER_2LABEL_ADD(item, layout, NULL, NULL); \
   CHECK_ADD(layout, check, "eflete/toggle") \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_check_state_set(check, value); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", check); \
   elm_object_part_content_set(item, "elm.swallow.content", layout); \
   evas_object_data_set(item, ITEM1, check); \
   return item; \
}

#define ITEM_1CHEACK_STATE_UPDATE(SUB, VALUE) \
static void \
prop_item_##SUB##_##VALUE##_update(Evas_Object *item, \
                                   Prop_Data *pd) \
{ \
   Evas_Object *check; \
   Eina_Bool value; \
   check = evas_object_data_get(item, ITEM1); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->style->obj, \
                                           pd->part->name, \
                                           pd->part->curr_state, \
                                           pd->part->curr_state_value); \
   elm_check_state_set(check, value); \
   evas_object_smart_callback_del_full(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(check, "changed", _on_##SUB##_##VALUE##_change, pd); \
   pm_project_changed(app_data_get()->project); \
}

#define ITEM_2CHEACK_STATE_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_item_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                            Prop_Data *pd, \
                                            const char *tooltip1, \
                                            const char *tooltip2) \
{ \
   Evas_Object *item, *box, *layout, *check1, *check2; \
   Eina_Bool value; \
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   BOX_ADD(item, box, true, true) \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, NULL, NULL); \
   CHECK_ADD(layout, check1, "eflete/toggle") \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check1, value); \
   elm_object_tooltip_text_set(check1, tooltip1); \
   evas_object_smart_callback_add(check1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", check1); \
   elm_box_pack_end(box, layout); \
   ITEM_CONTEINER_2LABEL_ADD(box, layout, NULL, NULL); \
   CHECK_ADD(layout, check2, "eflete/toggle") \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check2, value); \
   elm_object_tooltip_text_set(check2, tooltip2); \
   evas_object_smart_callback_add(check2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(layout, "eflete.content", check2); \
   elm_box_pack_end(box, layout); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   evas_object_data_set(item, ITEM1, check1); \
   evas_object_data_set(item, ITEM2, check2); \
   return item; \
}

#define ITEM_2CHEACK_STATE_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_item_##SUB##_##VALUE1##_##VALUE2##_update(Evas_Object *item, \
                                               Prop_Data *pd) \
{ \
   Evas_Object *check1, *check2; \
   Eina_Bool value; \
   check1 = evas_object_data_get(item, ITEM1); \
   value = edje_edit_##SUB##_##VALUE1##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check1, value); \
   evas_object_smart_callback_del_full(check1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(check1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   check2 = evas_object_data_get(item, ITEM2); \
   value = edje_edit_##SUB##_##VALUE2##_get(pd->style->obj, \
                                            pd->part->name, \
                                            pd->part->curr_state, \
                                            pd->part->curr_state_value); \
   elm_check_state_set(check2, value); \
   evas_object_smart_callback_del_full(check2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(check2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   pm_project_changed(app_data_get()->project); \
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
   ITEM_ADD(parent, item, TEXT, "eflete/property/item/default") \
   EWE_ENTRY_ADD(item, entry, true, DEFAULT_STYLE) \
   elm_object_part_text_set(entry, "elm.guide", "left right top bottom"); \
   if (!l && !r && !t && !b) \
     ewe_entry_entry_set(entry, NULL); \
   else \
     { \
        snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
        ewe_entry_entry_set(entry, buff); \
     } \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
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
   if (!l && !r && !t && !b) \
     ewe_entry_entry_set(entry, NULL); \
   else \
     { \
        snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
        ewe_entry_entry_set(entry, buff); \
     } \
   evas_object_smart_callback_del_full(entry, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(entry, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
   pm_project_changed(app_data_get()->project); \
   return item; \
}
