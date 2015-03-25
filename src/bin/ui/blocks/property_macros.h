/*
 * Edje Theme Editor
 * Copyright (C) 2015 Samsung Electronics.
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

/** @privatesection  @{ */

/**
 * @defgroup Property_Macro Property Macro
 * @ingroup Property
 *
 * This a helper module. Here defined macro-helpers for modules like property.
 * Wherever necessary list of item with label and controls this module will
 * be used.
 */

/**
 * Create new layout with style "elm/layout/property/STYLE".
 * Macro develop for property items, can use in simular cases.
 * Also macro declare the evas object 'item'
 *
 * @param PARENT The parent elm widget for property item;
 * @param NAME This text will be setted to part 'elm.text' of
 *             created layout;
 * @param STYLE The layout style.
 *
 * @ingroup Property_Macro
 */
#define PROPERTY_ITEM_ADD(PARENT, NAME, STYLE) \
   Evas_Object *item; \
   item = elm_layout_add(PARENT); \
   elm_layout_theme_set(item, "layout", "property", STYLE); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_text_set(item, NULL, NAME); \
   evas_object_show(item);

/*****************************************************************************/
/*                         GROUP 2 CHECK CONTROL                             */
/*****************************************************************************/
/**
 * Macro for functions that create the tem with label and 2 spinners for group
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of group attribute
 *            (example: min for min_x attribute)
 * @param VALUE1 The first value of group attribute (example: x for min_x attribute)
 * @param VALUE2 The second value of group attribute (example: y for min_y attribute)
 *
 * @ingroup Property_Macro
 */
#define GROUP_ATTR_2SPINNER_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_group_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                             Prop_Data *pd, \
                                             const char *tooltip1, \
                                             const char *tooltip2) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   elm_object_part_text_set(item, "label.swallow1.start", "w:"); \
   elm_object_part_text_set(item, "label.swallow1.end", "px"); \
   SPINNER_ADD(item, pd->group.SUB##_##VALUE1, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->group.SUB##_##VALUE1, "%.0f"); \
   elm_object_tooltip_text_set(pd->group.SUB##_##VALUE1, tooltip1); \
   evas_object_smart_callback_add(pd->group.SUB##_##VALUE1, "changed", _on_group_##SUB##_##VALUE1##_change, pd); \
   elm_object_part_content_set(item, "swallow.content1", pd->group.SUB##_##VALUE1); \
   evas_object_event_callback_priority_add(pd->group.SUB##_##VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   elm_object_part_text_set(item, "label.swallow2.start", "h:"); \
   elm_object_part_text_set(item, "label.swallow2.end", "px"); \
   SPINNER_ADD(item, pd->group.SUB##_##VALUE2, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->group.SUB##_##VALUE2, "%.0f"); \
   elm_object_tooltip_text_set(pd->group.SUB##_##VALUE2, tooltip2); \
   evas_object_smart_callback_add(pd->group.SUB##_##VALUE2, "changed", _on_group_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "swallow.content2", pd->group.SUB##_##VALUE2); \
   evas_object_event_callback_priority_add(pd->group.SUB##_##VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   prop_group_##SUB##_##VALUE1##_##VALUE2##_update(pd); \
   return item; \
}

/**
 * Macro for function that update the value of controls of the group attibute.
 *
 * @paramram SUB The prefix of main parameter of group attribute
 *               (example: min for min_x attribute)
 * @paramram VALUE1 The first value of group attribute (example: x for min_x attribute)
 * @paramram VALUE2 The second value of group attribute (example: y for min_y attribute)
 *
 * @ingroup Property_Macro
 */
#define GROUP_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_group_##SUB##_##VALUE1##_##VALUE2##_update(Prop_Data *pd) \
{ \
   elm_spinner_value_set(pd->group.SUB##_##VALUE1, edje_edit_group_##SUB##_##VALUE1##_get(pd->wm_style->obj)); \
   elm_spinner_value_set(pd->group.SUB##_##VALUE2, edje_edit_group_##SUB##_##VALUE2##_get(pd->wm_style->obj)); \
}

/**
 * This macro is used to generate callback and static functions for creating,
 * updating and other stuff of group's attributes called 'min' and 'max'
 *
 * The behaviour of these attributes (which are being represented by spinners)
 * is next: when we change 'min' value and it is becoming higher than 'max' then
 * value 'max' should be changed also (min can't be higher than max).
 *
 * That's why we have to check if 'min' is higher (>) than 'max' in all callbacks
 * for 'min', for both w and h parameter.
 * Second macro require to check if 'max' is lower then 'min' (because 'max'
 * can't be lower than 'min', so we need update is so 'min' would be equal to
 * 'max').
 * So we need to check if 'max' is lower (<) than 'min'.
 *
 * @param SUB1 The prefix of main parameter of group attribute
 *             (example: min for min_x attribute)
 * @param SUB2 The prefix of minor parameter of group attribute
 *             (example: min for min_x attribute)
 * @param VALUE1 The value of group attribute (example: x for min_x attribute)
 * @param CHECK shows the way SUB1.VALUE and SUB2.VALUE argument will be compared.
 *        If Min compared to Max, then it is >, if Max compared to Min, then it is <
 *
 * @ingroup Property_Macro
 */
#define GROUP_ATTR_2SPINNER_CALLBACK(SUB1, SUB2, VALUE, CHECK) \
static void \
_on_group_##SUB1##_##VALUE##_change(void *data, \
                                    Evas_Object *obj, \
                                    void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   int value = (int)elm_spinner_value_get(obj); \
   int value_##SUB2##_##VALUE = edje_edit_group_##SUB2##_##VALUE##_get(pd->wm_style->obj); \
   int old_value_##SUB2##_##VALUE = value_##SUB2##_##VALUE; \
   int old_value_##SUB1##_##VALUE = edje_edit_group_##SUB1##_##VALUE##_get(pd->wm_style->obj); \
   if (value CHECK value_##SUB2##_##VALUE) \
     { \
        if (!edje_edit_group_##SUB2##_##VALUE##_set(pd->wm_style->obj, value)) \
          return; \
        elm_spinner_value_set(pd->group.SUB2##_##VALUE, value); \
        value_##SUB2##_##VALUE = value; \
     } \
   if (!edje_edit_group_##SUB1##_##VALUE##_set(pd->wm_style->obj, value)) \
     return; \
   if (!strcmp("min", #SUB1)) \
     { \
       history_diff_add(pd->wm_style->obj, PROPERTY, CONTAINER, VAL_GROUP, old_value_##SUB1##_##VALUE, value, \
                        old_value_##SUB2##_##VALUE, value_##SUB2##_##VALUE,  \
                        (void*)edje_edit_group_##SUB1##_##VALUE##_set, pd->wm_style->full_group_name, \
                        (void*)edje_edit_group_##SUB2##_##VALUE##_set,  "group_"#VALUE, \
                        NULL, NULL, 0); \
     } \
   else \
    { \
       history_diff_add(pd->wm_style->obj, PROPERTY, CONTAINER, VAL_GROUP, old_value_##SUB2##_##VALUE,\
                        value_##SUB2##_##VALUE, old_value_##SUB1##_##VALUE, value,  \
                        (void*)edje_edit_group_##SUB2##_##VALUE##_set, pd->wm_style->full_group_name, \
                        (void*)edje_edit_group_##SUB1##_##VALUE##_set,  "group_"#VALUE, \
                        NULL, NULL, 0); \
    } \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                         PART 1 CHECK CONTROL                              */
/*****************************************************************************/
/**
 * Macro defines a functions that create a item with label and 1 check for part
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1CHECK_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Prop_Data *pd, \
                           const char *tooltip) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   CHECK_ADD(item, pd->SUB.VALUE) \
   elm_object_style_set(pd->SUB.VALUE, "toggle"); \
   elm_check_state_set(pd->SUB.VALUE, edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name)); \
   elm_object_tooltip_text_set(pd->SUB.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "changed", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->SUB.VALUE); \
   return item; \
}

/**
 * Macro defines a callback for PART_ATTR_1CHECK_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute;
 * @param VALUE The value of part attribute.
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1CHECK_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Bool old_value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, \
                                                      pd->wm_part->name);\
   if (!edje_edit_part_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, value)) \
     return; \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, NULL, 0.0); \
   workspace_edit_object_recalc(pd->workspace); \
   project_changed(); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                       PART 1 COMBOBOX CONTROL                             */
/*****************************************************************************/
/**
 * Macro for functions that create the tem with label and 1 combobox for part
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, \
                              Prop_Data *pd, \
                              const char *tooltip) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(item, pd->MEMBER.VALUE) \
   prop_##MEMBER##_##VALUE##_update(pd, pd->MEMBER.VALUE); \
   elm_object_tooltip_text_set(pd->MEMBER.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE, "selected", _on_##MEMBER##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->MEMBER.VALUE); \
   return item; \
}

/**
 * Macro define the callback for PART_ATTR_1COMBOBOX_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   const char *old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                                           pd->wm_part->name);\
   const char *value = NULL; \
   if (item->index != 0) \
     { \
        value = item->title; \
        if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, \
                                             pd->wm_part->name, item->title)) \
          { \
             ewe_combobox_select_item_set(obj, pd->part.previous_source); \
             NOTIFY_ERROR(_("This source value will <br>" \
                            "lead to Recursive Reference. <br>" \
                            "Previous value restored.")); \
             return; \
          } \
        pd->part.previous_source = item->index; \
     } \
   else \
     { \
        edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, NULL); \
        pd->part.previous_source = 0; \
     } \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_STRING, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, NULL, 0.0); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                     PART 1 COMBOBOX LIST CONTROL                          */
/*****************************************************************************/
/**
 * Macro defines functions that create a item with label and 1 combobox for part
 * attribute. A predefined list fill the combobox.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, TYPE) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                        Prop_Data *pd, \
                        const char *tooltip, \
                        const char **list) \
{ \
   TYPE value; \
   int i; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(parent, pd->SUB.VALUE) \
   value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name); \
   for (i = 0; list[i]; ewe_combobox_item_add(pd->SUB.VALUE, list[i]), i++) ; \
   ewe_combobox_select_item_set(pd->SUB.VALUE, value); \
   elm_object_tooltip_text_set(pd->SUB.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->SUB.VALUE); \
   return item; \
}

/**
 * Macro defines a function for update control by PART_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, TYPE) \
static void \
prop_##SUB##_##VALUE##_update(Prop_Data *pd) \
{ \
   TYPE value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name); \
   ewe_combobox_select_item_set(pd->SUB.VALUE, value); \
}

/**
 * Macro defines a callback for PART_ATTR_1COMBOBOX_ADD.
 *
 * @param TEXT The attribute name, for error message
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                          Evas_Object *obj __UNUSED__, \
                          void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   int old_value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name); \
   int value = item->index; \
   if (!edje_edit_part_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, (TYPE)item->index)) \
     { \
        NOTIFY_WARNING(_("Cann't apply value '%s' to attribute "#TEXT". Restore previous value"), item->title); \
        ewe_combobox_select_item_set(obj, old_value); \
        return; \
     } \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, NULL, 0.0); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                          PART 1CHECK 1SPINNER                             */
/*****************************************************************************/
#define PART_ATTR_DRAG_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_part_drag_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                       Prop_Data *pd, \
                                       const char *tooltip1, \
                                       const char *tooltip2 ) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   CHECK_ADD(item, pd->SUB.VALUE1) \
   elm_object_style_set(pd->SUB.VALUE1, "toggle"); \
   elm_object_tooltip_text_set(pd->SUB.VALUE1, tooltip1); \
   evas_object_smart_callback_add(pd->SUB.VALUE1, "changed", _on_part_drag_##VALUE1##_change, pd); \
   elm_layout_content_set(item, "swallow.content1", pd->SUB.VALUE1); \
   SPINNER_ADD(item, pd->SUB.VALUE2, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->SUB.VALUE2, N_("%.0f")); \
   elm_object_tooltip_text_set(pd->SUB.VALUE2, tooltip2); \
   evas_object_smart_callback_add(pd->SUB.VALUE2, "changed", _on_part_drag_##VALUE2##_change, pd); \
   evas_object_event_callback_priority_add(pd->SUB.VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   elm_layout_text_set(item, "label.swallow2.start", _("step")); \
   elm_layout_text_set(item, "label.swallow2.end", _("px")); \
   elm_layout_content_set(item, "swallow.content2", pd->SUB.VALUE2); \
   prop_##SUB##_##VALUE1##_##VALUE2##_update(pd); \
   return item; \
}

#define PART_ATTR_DRAG_UPDATE(SUB, VALUE1, VALUE2) \
static void \
prop_part_drag_##VALUE1##_##VALUE2##_update(Prop_Data *pd) \
{ \
   Eina_Bool ch_value; int st_value; \
   ch_value = edje_edit_part_drag_##VALUE1##_get(pd->wm_style->obj, pd->wm_part->name); \
   elm_check_state_set(pd->SUB.VALUE1, ch_value); \
   st_value = edje_edit_part_drag_##VALUE2##_get(pd->wm_style->obj, pd->wm_part->name); \
   elm_spinner_value_set(pd->SUB.VALUE2, st_value); \
}

#define PART_ATTR_DRAG_CALLBACK(SUB, VALUE1, VALUE2) \
static void \
_on_part_drag_##VALUE1##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Bool old_value = edje_edit_part_drag_##VALUE1##_get(pd->wm_style->obj, \
                                                            pd->wm_part->name);\
   edje_edit_part_drag_##VALUE1##_set(pd->wm_style->obj, pd->wm_part->name, value); \
   prop_part_drag_control_disable_set(pd, false); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_drag_##VALUE1##_set, #SUB"_"#VALUE1, \
                     pd->wm_part->name, NULL, 0.0); \
   workspace_edit_object_recalc(pd->workspace); \
   project_changed(); \
   pd->wm_style->isModify = true; \
} \
static void \
_on_part_drag_##VALUE2##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Bool old_value = edje_edit_part_drag_##VALUE2##_get(pd->wm_style->obj, \
                                                         pd->wm_part->name);\
   edje_edit_part_drag_##VALUE2##_set(pd->wm_style->obj, pd->wm_part->name, value); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_drag_##VALUE2##_set, #SUB"_"#VALUE2, \
                     pd->wm_part->name, NULL, 0.0); \
   workspace_edit_object_recalc(pd->workspace); \
   project_changed(); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                     UPDATE THE ATTR WITH LIST OF PARTS                    */
/*****************************************************************************/
#define PART_ATTR_PARTS_LIST(SUB, VALUE, MEMBER) \
static void \
prop_##MEMBER##_##VALUE##_update(Prop_Data *pd, Evas_Object *obj __UNUSED__) \
{ \
   Part *part; \
   Eina_Inlist *list_n = NULL; \
   Eina_Stringshare *value; \
   ewe_combobox_items_list_free(pd->MEMBER.VALUE, true); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name); \
   if (value) ewe_combobox_text_set(pd->MEMBER.VALUE, value); \
   else ewe_combobox_text_set(pd->MEMBER.VALUE, _("None")); \
   ewe_combobox_item_add(pd->MEMBER.VALUE, _("None")); \
   EINA_INLIST_FOREACH_SAFE(pd->wm_style->parts, list_n, part) \
     { \
        if ((part != pd->wm_part) && (part->type != EDJE_PART_TYPE_SPACER)) \
           ewe_combobox_item_add(pd->MEMBER.VALUE, part->name); \
     } \
   edje_edit_string_free(value); \
}

/** @} privatesection */
