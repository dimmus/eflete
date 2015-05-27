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
   LAYOUT_PROP_ADD(PARENT, NAME, "property", STYLE)

#define PART_ARGS , pd->wm_part->name
#define PART_ARGS_DIFF , pd->wm_part->name, NULL, 0.0
#define PART_ITEM_ARGS , pd->wm_part->name, pd->item_name
#define STATE_ARGS , pd->wm_part->name, pd->wm_part->curr_state, pd->wm_part->curr_state_value

/*****************************************************************************/
/*                      COMMON ATTRIBUTE CONTOLS MACRO                       */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 spinners.
 *
 * @param PREFIX The attribute prefix (STATE, PART, etc), used for define the
 *        update function
 * @param TEXT The label text
 * @param SUB The prefix of the attribute
 * @param VALUE1 The first value of the attribute
 * @param VALUE2 The second value of the attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MIN The min value of spinner
 * @param MAX The max value of spinner
 * @param STEP The step to increment or decrement the spinner value
 * @param FMT The format string of the displayed label
 * @param L1_START The text of label before first swallow
 * @param L1_END The text of label after first swallow
 * @param L2_START The text of label before second swallow
 * @param L2_END The text of label after second swallow
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_2SPINNER_ADD(PREFIX, TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                            MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                            TOOLTIP1, TOOLTIP2, MULTIPLIER) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, STYLE) \
   SPINNER_ADD(item, pd->MEMBER.VALUE1, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->MEMBER.VALUE1, FMT); \
   elm_layout_content_set(item, "swallow.content1", pd->MEMBER.VALUE1); \
   elm_layout_text_set(item, "label.swallow1.start", L1_START); \
   elm_layout_text_set(item, "label.swallow1.end", L1_END); \
   if (TOOLTIP1) elm_object_tooltip_text_set(pd->MEMBER.VALUE1, TOOLTIP1); \
   evas_object_event_callback_priority_add(pd->MEMBER.VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE1, "changed", _on_##MEMBER##_##VALUE1##_change, pd); \
   SPINNER_ADD(item, pd->MEMBER.VALUE2, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->MEMBER.VALUE2, FMT); \
   elm_layout_content_set(item, "swallow.content2", pd->MEMBER.VALUE2); \
   elm_layout_text_set(item, "label.swallow2.start", L2_START); \
   elm_layout_text_set(item, "label.swallow2.end", L2_END); \
   if (TOOLTIP2) elm_object_tooltip_text_set(pd->MEMBER.VALUE2, TOOLTIP2); \
   evas_object_event_callback_priority_add(pd->MEMBER.VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE2, "changed", _on_##MEMBER##_##VALUE2##_change, pd); \
   PREFIX##_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER); \
   return item; \
}

/**
 * Macro defines a function call with arguments that updates control by
 * COMMON_1(2)SPINNER_ADD macro.
 *
 * @param SUB The prefix of the attribute
 * @param VALUE The first value of the attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, ARGS) \
   elm_spinner_value_set(pd->MEMBER.VALUE, \
                         MULTIPLIER * edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS)); \

/**
 * Macro defines a COMMON_1(2)SPINNER_ADD.
 *
 * @param SUB The prefix of the attribute
 * @param VALUE The value of the attribute
 * @param TYPE The spinner value type: int, double
 * @param HISTORY_TYPE The history value type: VAL_INT, VAL_DOUBLE
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, HISTORY_TYPE, MULTIPLIER, ARGS) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   TYPE value = elm_spinner_value_get(obj); \
   TYPE old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS); \
   value /= MULTIPLIER; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj ARGS, value)) \
     return; \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, HISTORY_TYPE, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/**
 * Macro defines functions that create an item with label and 1 combobox for
 * a attribute item. A predefined list fill the combobox.
 *
 * @param PREFIX The attribute prefix (STATE, PART, etc), used for define the
 *        update function
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param LIST The predefined strings list
 * @param TOOLTIP The combobox tooltip
 * @param ARGS
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_ADD(PREFIX, TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, ARGS) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, Prop_Data *pd) \
{ \
   int i; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(parent, pd->MEMBER.VALUE) \
   for (i = 0; LIST[i]; ewe_combobox_item_add(pd->MEMBER.VALUE, LIST[i]), i++) ; \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->MEMBER.VALUE); \
   PREFIX##_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   return item; \
}

/**
 * Macro defines a function that updates control by COMMON_COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param ARGS
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER, ARGS) \
   ewe_combobox_select_item_set(pd->MEMBER.VALUE, (int)edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS));

/**
 * Macro defines a callback for COMMON_COMBOBOX_ADD.
 *
 * @param TEXT The attribute name, for error message
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, ARGS, ARGS_DIFF) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                          Evas_Object *obj __UNUSED__, \
                          void *event_info) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info; \
   int old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS); \
   int value = item->index; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj ARGS, (TYPE)item->index)) \
     { \
        NOTIFY_WARNING(_("Cann't apply value '%s' for attribute '"#TEXT"'. Restore previous value"), item->title); \
        ewe_combobox_select_item_set(obj, old_value); \
        return; \
     } \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE ARGS_DIFF); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/**
 * Macro defines a callback for attribute that controled by 2 spinners and
 * edje edit functions returned, as arguments, two values at hte same time.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE1 The first value of attribute
 * @param VALUE2 The second value of attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param TYPE The type of given attribute
 * @param MULTIPLIER The multiplier to convert the value to percent
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_2SPINNER_DOUBLEVAL_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER, ARGS) \
static void \
_on_##MEMBER##_##VALUE1##_change(void *data, \
                                 Evas_Object *obj __UNUSED__, \
                                 void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   edje_edit_##SUB##_##VALUE1##_set(pd->wm_style->obj ARGS, \
                                    (TYPE)elm_spinner_value_get(pd->MEMBER.VALUE1) / MULTIPLIER, \
                                    (TYPE)elm_spinner_value_get(pd->MEMBER.VALUE2) / MULTIPLIER); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
} \
static void \
_on_##MEMBER##_##VALUE2##_change(void *data, \
                                 Evas_Object *obj __UNUSED__, \
                                 void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   edje_edit_##SUB##_##VALUE1##_set(pd->wm_style->obj ARGS, \
                                    (TYPE)elm_spinner_value_get(pd->MEMBER.VALUE1) / MULTIPLIER, \
                                    (TYPE)elm_spinner_value_get(pd->MEMBER.VALUE2) / MULTIPLIER); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/**
 * Macro defines a update function for attribute that controled by 2 spinners
 * and edje edit functions returned, as arguments, two values at hte same time.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE1 The first value of attribute
 * @param VALUE2 The second value of attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param TYPE The type of given attribute
 * @param MULTIPLIER The multiplier to convert the value to percent
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_2SPINNER_DOUBLEVAL_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER, ARGS) \
   TYPE VALUE1, VALUE2; \
   edje_edit_##SUB##_##VALUE1##_get(pd->wm_style->obj ARGS, &VALUE1, &VALUE2); \
   elm_spinner_value_set(pd->MEMBER.VALUE1, VALUE1 * MULTIPLIER); \
   elm_spinner_value_set(pd->MEMBER.VALUE2, VALUE2 * MULTIPLIER);

/**
 * Macro defines functions that create an item with label and 1 check for part
 * attribute.
 *
 * @param PREFIX The attribute prefix (STATE, PART, etc), used for define the
 *        update function
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param LIST The predefined strings list
 * @param TOOLTIP The combobox tooltip
 * @param ARGS The edje edit function arguments
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_CHECK_ADD(PREFIX, TEXT, SUB, VALUE, MEMBER, TOOLTIP, ARGS) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   CHECK_ADD(item, pd->MEMBER.VALUE) \
   elm_object_style_set(pd->MEMBER.VALUE, "toggle"); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE, "changed", _on_##MEMBER##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->MEMBER.VALUE); \
   PREFIX##_ATTR_1CHECK_UPDATE(SUB, VALUE, MEMBER) \
   return item; \
}

/**
 * Macro defines a functions that create an item with label and 2 check for
 * state attribute.
 *
 * @param PREFIX The attribute prefix (STATE, PART, etc), used for define the
 *        update function
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The check member from Prop_Data structure
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 *
 * @ingroup Property_Macro
 */
#define COMMON_2CHECK_ADD(PREFIX, TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                          L1_START, L1_END, L2_START, L2_END, \
                          TOOLTIP1, TOOLTIP2) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   CHECK_ADD(item, pd->MEMBER.VALUE1) \
   elm_object_style_set(pd->MEMBER.VALUE1, "toggle"); \
   if (TOOLTIP1) elm_object_tooltip_text_set(pd->MEMBER.VALUE1, TOOLTIP1); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE1, "changed", _on_##MEMBER##_##VALUE1##_change, pd); \
   elm_layout_content_set(item, "swallow.content1", pd->MEMBER.VALUE1); \
   elm_layout_text_set(item, "label.swallow1.start", L1_START); \
   elm_layout_text_set(item, "label.swallow1.end", L1_END); \
   CHECK_ADD(item, pd->MEMBER.VALUE2) \
   elm_object_style_set(pd->MEMBER.VALUE2, "toggle"); \
   if (TOOLTIP2) elm_object_tooltip_text_set(pd->MEMBER.VALUE2, TOOLTIP2); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE2, "changed", _on_##MEMBER##_##VALUE2##_change, pd); \
   elm_layout_content_set(item, "swallow.content2", pd->MEMBER.VALUE2); \
   elm_layout_text_set(item, "label.swallow2.start", L2_START); \
   elm_layout_text_set(item, "label.swallow2.end", L2_END); \
   PREFIX##_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2, MEMBER) \
   return item; \
}

/* Macro defines a function that updates control by COMMON_CHECK_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_CHECK_UPDATE(SUB, VALUE, MEMBER, ARGS) \
   elm_check_state_set(pd->MEMBER.VALUE, edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS));

/**
 * Macro defines a callback for attribute that controled by check.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The check member from Prop_Data structure
 * @param ARGS The edje edit function arguments
 * @param ARGS_DIFF The edje edit function arguments for history
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, ARGS, ARGS_DIFF) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Bool old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS);\
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj ARGS, value)) \
     return; \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE ARGS_DIFF); \
   workspace_edit_object_recalc(pd->workspace); \
   project_changed(); \
   pd->wm_style->isModify = true; \
}

/**
 * Macro defines a update function for attribute that controled by 2 checks
 * and edje edit functions returned, as arguments, two values at hte same time.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE1 The first value of attribute
 * @param VALUE2 The second value of attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_2CHECK_DOUBLEVAL_UPDATE(SUB, VALUE1, VALUE2, MEMBER, ARGS) \
   Eina_Bool VALUE1, VALUE2; \
   edje_edit_##SUB##_##VALUE1##_get(pd->wm_style->obj ARGS, &VALUE1, &VALUE2); \
   elm_spinner_value_set(pd->MEMBER.VALUE1, VALUE1); \
   elm_spinner_value_set(pd->MEMBER.VALUE2, VALUE2);

/**
 * Macro defines a callback for attribute that controled by 2 checks and
 * edje edit functions returned, as arguments, two values at hte same time.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE1 The first value of attribute
 * @param VALUE2 The second value of attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_2CHECK_DOUBLEVAL_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, ARGS) \
static void \
_on_##MEMBER##_##VALUE1##_change(void *data, \
                                 Evas_Object *obj __UNUSED__, \
                                 void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   edje_edit_##SUB##_##VALUE1##_set(pd->wm_style->obj ARGS, \
                                    elm_check_state_get(pd->MEMBER.VALUE1), \
                                    elm_check_state_get(pd->MEMBER.VALUE2)); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
} \
static void \
_on_##MEMBER##_##VALUE2##_change(void *data, \
                                 Evas_Object *obj __UNUSED__, \
                                 void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   edje_edit_##SUB##_##VALUE1##_set(pd->wm_style->obj ARGS, \
                                    elm_check_state_get(pd->MEMBER.VALUE1), \
                                    elm_check_state_get(pd->MEMBER.VALUE2)); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/**
 * Macro defines a function that updates control by COMMON_COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_STRSHARE_UPDATE(SUB, VALUE, MEMBER, ARGS) \
   Eina_Stringshare *value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS); \
   ewe_combobox_text_set(pd->MEMBER.VALUE, value ? value : _("None")); \
   edje_edit_string_free(value);


/**
 * Macro defines a callback for COMMON_COMBOBOX_ADD.
 *
 * @param TEXT The attribute name, for error message
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 * @param ARGS The edje edit function arguments
 * @param ARGS_DIFF The edje edit function arguments for history 
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_STRSHARE_CALLBACK(SUB, VALUE, MEMBER, ARGS, ARGS_DIFF) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *event_info) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info; \
   Eina_Stringshare *old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj ARGS); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj ARGS, (char *)item->title)) \
     return; \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_STRING, old_value, \
                    item->title, pd->wm_style->full_group_name, \
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE ARGS); \
   eina_stringshare_del(old_value); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                         GROUP 2 CHECK CONTROL                             */
/*****************************************************************************/
/**
 * Macro for functions that create an item with label and 2 spinners for group
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
   if (tooltip1) elm_object_tooltip_text_set(pd->group.SUB##_##VALUE1, tooltip1); \
   evas_object_smart_callback_add(pd->group.SUB##_##VALUE1, "changed", _on_group_##SUB##_##VALUE1##_change, pd); \
   elm_object_part_content_set(item, "swallow.content1", pd->group.SUB##_##VALUE1); \
   evas_object_event_callback_priority_add(pd->group.SUB##_##VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   elm_object_part_text_set(item, "label.swallow2.start", "h:"); \
   elm_object_part_text_set(item, "label.swallow2.end", "px"); \
   SPINNER_ADD(item, pd->group.SUB##_##VALUE2, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->group.SUB##_##VALUE2, "%.0f"); \
   if (tooltip2) elm_object_tooltip_text_set(pd->group.SUB##_##VALUE2, tooltip2); \
   evas_object_smart_callback_add(pd->group.SUB##_##VALUE2, "changed", _on_group_##SUB##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "swallow.content2", pd->group.SUB##_##VALUE2); \
   evas_object_event_callback_priority_add(pd->group.SUB##_##VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   prop_group_##SUB##_##VALUE1##_##VALUE2##_update(pd); \
   return item; \
}

/**
 * Macro for function that updates a value of controls of the group attibute.
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
 * Macro defines functions that create an item with label and 1 check for part
 * attribute.
 *
 * @see COMMON_CHECK_ADD
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1CHECK_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   COMMON_CHECK_ADD(PART, TEXT, SUB, VALUE, MEMBER, TOOLTIP, PART_ARGS)

/**
 * Macro for function that updates a value of controls of the PART_ATTR_1CHECK_ADD macro.
 *
 * @paramram SUB The prefix of main name of part attribute
 * @paramram VALUE The value(name) of part attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1CHECK_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_CHECK_UPDATE(SUB, VALUE, MEMBER, PART_ARGS)

/**
 * Macro defines a callback for PART_ATTR_1CHECK_ADD.
 *
 * @see COMMON_CHECK_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1CHECK_CALLBACK(SUB, VALUE, MEMBER) \
   COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, PART_ARGS, PART_ARGS_DIFF) \

/*****************************************************************************/
/*                       PART 1 COMBOBOX CONTROL                             */
/*****************************************************************************/
/**
 * Macro for functions that create an item with label and 1 combobox for part
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, \
                              Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(item, pd->MEMBER.VALUE) \
   prop_##MEMBER##_##VALUE##_update(pd); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE, "selected", _on_##MEMBER##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->MEMBER.VALUE); \
   return item; \
}

/**
 * Macro defines the callback for PART_ATTR_1COMBOBOX_ADD.
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
   App_Data *ap = app_data_get(); \
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
   pm_save_to_dev(ap->project, pd->wm_style); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                     PART 1 COMBOBOX LIST CONTROL                          */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 combobox for part
 * attribute. A predefined list fill the combobox.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param LIST The predefined strings list
 * @param TOOLTIP The combobox tooltip
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP) \
   COMMON_COMBOBOX_LIST_ADD(PART, TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, PART_ARGS)

/**
 * Macro defines a function that updates control by PART_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER, PART_ARGS)

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
   COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, PART_ARGS, PART_ARGS_DIFF)

/*****************************************************************************/
/*                       PART 1COMBOBOX SOURCE UPDATE                        */
/*****************************************************************************/
/**
 * Macro defines a function that updates any 'source' attribute control macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE The 'source' value (source, source2 etc)
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_SOURCE_UPDATE(MEMBER, VALUE) \
static void \
prop_##MEMBER##_##VALUE##_update(Prop_Data *pd) \
{ \
   Eina_List *collections, *l; \
   const char *group, *value; \
   App_Data *ap = app_data_get(); \
   unsigned int i = 0; \
   ewe_combobox_items_list_free(pd->MEMBER.VALUE, true); \
   value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name); \
   if (value) ewe_combobox_text_set(pd->MEMBER.VALUE, value); \
   else ewe_combobox_text_set(pd->MEMBER.VALUE, _("None")); \
   ewe_combobox_item_add(pd->MEMBER.VALUE, _("None")); \
   collections = edje_mmap_collection_list(ap->project->mmap_file); \
   collections = eina_list_sort(collections, eina_list_count(collections), sort_cb); \
   EINA_LIST_FOREACH(collections, l, group) \
     { \
        if (group != pd->wm_style->full_group_name) \
          ewe_combobox_item_add(pd->MEMBER.VALUE, group); \
        if (group == value) \
          pd->part.previous_source = i; \
        i++; \
     } \
   edje_edit_string_free(value); \
   edje_mmap_collection_list_free(collections); \
}

/*****************************************************************************/
/*                        PART 1CHECK 1SPINNER DRAG                          */
/*****************************************************************************/
/**
 * Macro for dragable part attribute. An item that is created by this defined
 * function consists a toggle, to turn on/off a drag of the part, and spinner
 * to set a drag step.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE1 The value of first drag parametr (turn on/off)
 * @param VALUE2 The value of second drag parametr (drag step)
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_DRAG_ADD(TEXT, SUB, VALUE1, VALUE2) \
static Evas_Object * \
prop_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                       Prop_Data *pd, \
                                       const char *tooltip1, \
                                       const char *tooltip2 ) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   CHECK_ADD(item, pd->SUB.VALUE1) \
   elm_object_style_set(pd->SUB.VALUE1, "toggle"); \
   if (tooltip1) elm_object_tooltip_text_set(pd->SUB.VALUE1, tooltip1); \
   evas_object_smart_callback_add(pd->SUB.VALUE1, "changed", _on_part_drag_##VALUE1##_change, pd); \
   elm_layout_content_set(item, "swallow.content1", pd->SUB.VALUE1); \
   SPINNER_ADD(item, pd->SUB.VALUE2, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->SUB.VALUE2, N_("%.0f")); \
   if (tooltip2) elm_object_tooltip_text_set(pd->SUB.VALUE2, tooltip2); \
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

/**
 * Macro defines a function that updates control by PART_ATTR_DRAG_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE1 The value of first drag paramenetr (turn on/off)
 * @param VALUE2 The value of second drag paramenetr (drag step)
 *
 * @ingroup Property_Macro
 */
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

/**
 * Macro defines a callbacks for PART_ATTR_DRAG_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE1 The value of first drag paramenetr (turn on/off)
 * @param VALUE2 The value of second drag paramenetr (drag step)
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_DRAG_CALLBACK(SUB, VALUE1, VALUE2) \
static void \
_on_part_drag_##VALUE1##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   App_Data *ap = app_data_get(); \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Bool old_value = edje_edit_part_drag_##VALUE1##_get(pd->wm_style->obj, \
                                                            pd->wm_part->name);\
   edje_edit_part_drag_##VALUE1##_set(pd->wm_style->obj, pd->wm_part->name, value); \
   prop_part_drag_control_disable_set(pd, false); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_drag_##VALUE1##_set, #SUB"_"#VALUE1, \
                     pd->wm_part->name, NULL, 0.0); \
   pm_save_to_dev(ap->project, pd->wm_style); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
} \
static void \
_on_part_drag_##VALUE2##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   App_Data *ap = app_data_get(); \
   int value = elm_spinner_value_get(obj); \
   int old_value = edje_edit_part_drag_##VALUE2##_get(pd->wm_style->obj, \
                                                         pd->wm_part->name);\
   edje_edit_part_drag_##VALUE2##_set(pd->wm_style->obj, pd->wm_part->name, value); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_drag_##VALUE2##_set, #SUB"_"#VALUE2, \
                     pd->wm_part->name, NULL, 0.0); \
   pm_save_to_dev(ap->project, pd->wm_style); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                     UPDATE THE ATTR WITH LIST OF PARTS                    */
/*****************************************************************************/
/**
 * Macro defines a function that updates combobox list of attributes, which
 * depends on parts name
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_PARTS_LIST(SUB, VALUE, MEMBER) \
static void \
prop_##MEMBER##_##VALUE##_update(Prop_Data *pd) \
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

/*****************************************************************************/
/*                     PART ITEM 1 COMBOBOX CONTROL                          */
/*****************************************************************************/
/**
 * Macro for functions that create an item with label and 1 combobox for part
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 * @param TOOLTIP The combobox tooltip
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \

/**
 * Macro defines the callback for PART_ITEM_ATTR_1COMBOBOX_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                   pd->item_name, item->title); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                    PART ITEM 2 SPINNER CONTROLS                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 spinners for
 * part item attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MIN The min value of spinner
 * @param MAX The max value of spinner
 * @param STEP The step to increment or decrement the spinner value
 * @param FMT The format string of the displayed label
 * @param L1_START The text of label before first swallow
 * @param L1_END The text of label after first swallow
 * @param L2_START The text of label before second swallow
 * @param L2_END The text of label after second swallow
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_2SPINNER_ADD(TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                                    MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                                    TOOLTIP1, TOOLTIP2, MULTIPLIER) \
COMMON_2SPINNER_ADD(PART_ITEM, TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                    MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                    TOOLTIP1, TOOLTIP2, MULTIPLIER)

/**
 * Macro defines a function that updates control by PART_ITEM_ATTR_2SPINNER_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE1, MEMBER, TYPE,  MULTIPLIER, PART_ITEM_ARGS) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE2, MEMBER, TYPE,  MULTIPLIER, PART_ITEM_ARGS) \

/**
 * Macro defines a callback for PART_ITEM2SPINNER_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 * @param TYPE The spinner value type: int, double
 * @param HISTORY_TYPE The history value type: VAL_INT, VAL_DOUBLE
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @ingroup Property_Macro
 */
TODO("Add support PART_ITEM attributes to history")
#define PART_ITEM_ATTR_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, HISTORY_TYPE, MULTIPLIER) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   TYPE value = elm_spinner_value_get(obj); \
   value /= MULTIPLIER; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj PART_ITEM_ARGS, value)) \
     return; \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                    PART ITEM 1 COMBOBOX LIST CONTROL                      */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 combobox for part
 * item attribute. A predefined list fill the combobox.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param LIST The predefined strings list
 * @param TOOLTIP The combobox tooltip
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP) \
   COMMON_COMBOBOX_LIST_ADD(PART_ITEM, TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, PART_ITEM_ARGS)

/**
 * Macro defines a function that updates control by PART_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER, PART_ITEM_ARGS)

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
#define PART_ITEM_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                          Evas_Object *obj __UNUSED__, \
                          void *event_info) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info; \
   edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj PART_ITEM_ARGS, (TYPE)item->index); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                 PART ITEM 2 SPINNERS DOUBLEVAL CONTROL                    */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 spinners for
 * part item attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MIN The min value of spinner
 * @param MAX The max value of spinner
 * @param STEP The step to increment or decrement the spinner value
 * @param FMT The format string of the displayed label
 * @param L1_START The text of label before first swallow
 * @param L1_END The text of label after first swallow
 * @param L2_START The text of label before second swallow
 * @param L2_END The text of label after second swallow
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_DOUBLEVAL_ATTR_2SPINNER_ADD(TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                                              MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                                              TOOLTIP1, TOOLTIP2, MULTIPLIER) \
COMMON_2SPINNER_ADD(PART_ITEM_DOUBLEVAL, TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE,\
                    MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                    TOOLTIP1, TOOLTIP2, MULTIPLIER)

/**
 * Macro defines a function that updates control by PART_ITEM_ATTR_2SPINNER_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_DOUBLEVAL_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER) \
   COMMON_2SPINNER_DOUBLEVAL_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER, PART_ITEM_ARGS)

/**
 * Macro defines a callback for PART_ITEM2SPINNER_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 * @param TYPE The spinner value type: int, double
 * @param HISTORY_TYPE The history value type: VAL_INT, VAL_DOUBLE
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_DOUBLEVAL_ATTR_SPINNER_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, TYPE, HISTORY_TYPE, MULTIPLIER) \
   COMMON_2SPINNER_DOUBLEVAL_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER, PART_ITEM_ARGS)

/*****************************************************************************/
/*                           STATE 1 CHECK CONTROL                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 1 check for
 * state attribute.
 *
 * @see COMMON_CHECK_ADD
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1CHECK_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   COMMON_CHECK_ADD(STATE, TEXT, SUB, VALUE, MEMBER, TOOLTIP, STATE_ARGS)

/**
 * Macro defines a function that updates control by STATE_ATTR_1CHECK_ADD macro.
 *
 * @see COMMON_CHECK_UPDATE
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1CHECK_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_CHECK_UPDATE(SUB, VALUE, MEMBER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_ATTR_1CHEACK_ADD.
 *
 * @see COMMON_CHECK_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE, MEMBER) \
   COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, STATE_ARGS, STATE_ARGS)

/*****************************************************************************/
/*                        STATE 1 SPINNER CONTROLS                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 1 spinners for
 * state attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE The value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MIN The min value of spinner
 * @param MAX The max value of spinner
 * @param STEP The step to increment or decrement the spinner value
 * @param FMT The format string of the displayed label
 * @param L_START The text of label before first swallow
 * @param L_END The text of label after first swallow
 * @param TOOLTIP The first spinner tooltip
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1SPINNER_ADD(TEXT, SUB, VALUE, MEMBER, \
                                MIN, MAX, STEP, FMT, \
                                L_START, L_END, \
                                TOOLTIP, MULTIPLIER) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, \
                              Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   SPINNER_ADD(item, pd->MEMBER.VALUE, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->MEMBER.VALUE, FMT); \
   elm_layout_content_set(item, "swallow.content1", pd->MEMBER.VALUE); \
   elm_layout_text_set(item, "label.swallow1.start", L_START); \
   elm_layout_text_set(item, "label.swallow1.end", L_END); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   evas_object_event_callback_priority_add(pd->MEMBER.VALUE, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE, "changed", _on_##MEMBER##_##VALUE##_change, pd); \
   COMMON_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, STATE_ARGS) \
   return item; \
}

/**
 * Macro defines a callback for STATE_ATTR_1(2)SPINNER_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 * @param TYPE The spinner value type: int, double
 * @param HISTORY_TYPE The history value type: VAL_INT, VAL_DOUBLE
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, HISTORY_TYPE, MULTIPLIER) \
   COMMON_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, HISTORY_TYPE, MULTIPLIER, STATE_ARGS)

/**
 * Macro defines a callback for callbacks of min and max changes.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 * @param TYPE The spinner value type: int, double
 * @param HISTORY_TYPE The history value type: VAL_INT, VAL_DOUBLE
 * @param MULTIPLIER The multiplier to convert the value to percent
 * @param DIF_VALUE opposite to VALUE attribute
 * @param CHECK function that compire DIF_VALUE and VALUE attribute
 *
 * @ingroup Property_Macro
 */
TODO("merge STATE_MINMAX_... with GROUP_ATTR_2SPINNER... because they are really same")
#define STATE_MINMAX_ATTR_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, HISTORY_TYPE, MULTIPLIER, DIF_VALUE, CHECK) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   TYPE value = elm_spinner_value_get(obj); \
   TYPE old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj STATE_ARGS); \
   TYPE opposite_value = edje_edit_##SUB##_##DIF_VALUE##_get(pd->wm_style->obj STATE_ARGS); \
   value /= MULTIPLIER; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj STATE_ARGS, value)) \
     return; \
   if ((value CHECK opposite_value) && (opposite_value != -1)) \
     { \
        if (!edje_edit_##SUB##_##DIF_VALUE##_set(pd->wm_style->obj STATE_ARGS, value)) \
          return; \
        elm_spinner_value_set(pd->state.DIF_VALUE, value); \
     } \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, HISTORY_TYPE, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                        STATE 2 SPINNER CONTROLS                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 spinners for
 * state attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MIN The min value of spinner
 * @param MAX The max value of spinner
 * @param STEP The step to increment or decrement the spinner value
 * @param FMT The format string of the displayed label
 * @param L1_START The text of label before first swallow
 * @param L1_END The text of label after first swallow
 * @param L2_START The text of label before second swallow
 * @param L2_END The text of label after second swallow
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2SPINNER_ADD(TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                                MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                                TOOLTIP1, TOOLTIP2, MULTIPLIER) \
COMMON_2SPINNER_ADD(STATE, TEXT, STYLE, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                    MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                    TOOLTIP1, TOOLTIP2, MULTIPLIER)

/**
 * Macro defines a function that updates control by STATE_ATTR_2SPINNER_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE1, MEMBER, TYPE,  MULTIPLIER, STATE_ARGS) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE2, MEMBER, TYPE,  MULTIPLIER, STATE_ARGS) \

/*****************************************************************************/
/*                          STATE 2 CHECK CONTROLS                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 check for
 * state attribute.
 *
 * @see COMMON_2CHECK_ADD
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2CHECK_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                              L1_START, L1_END, L2_START, L2_END, \
                              TOOLTIP1, TOOLTIP2) \
   COMMON_2CHECK_ADD(STATE, TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                     L1_START, L1_END, L2_START, L2_END, \
                     TOOLTIP1, TOOLTIP2)

/**
 * Macro defines a function that updates control by STATE_ATTR_2CHECK_ADD macro.
 *
 * @see COMMON_CHECK_UPDATE
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2, MEMBER) \
   COMMON_CHECK_UPDATE(SUB, VALUE1, MEMBER, STATE_ARGS) \
   COMMON_CHECK_UPDATE(SUB, VALUE2, MEMBER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_ATTR_2CHECK_ADD.
 *
 * @see COMMON_CHECK_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2CHECK_CALLBACK(SUB, VALUE1, VALUE2, MEMBER) \
   COMMON_CHECK_CALLBACK(SUB, VALUE1, MEMBER, STATE_ARGS, STATE_ARGS) \
   COMMON_CHECK_CALLBACK(SUB, VALUE2, MEMBER, STATE_ARGS, STATE_ARGS)

/*****************************************************************************/
/*                    STATE 1 COMBOBOX LIST CONTROL                          */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 combobox for
 * state attribute. A predefined list fill the combobox.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 * @param LIST The pointer to array of strings
 * @param TOOLTIP The tooltip for combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP) \
   COMMON_COMBOBOX_LIST_ADD(STATE, TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, STATE_ARGS)

/**
 * Macro defines a function that updates control by STATE_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_ATTR_1COMBOBOX_ADD.
 *
 * @param TEXT The attribute name, for error message
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE) \
   COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, STATE_ARGS, STATE_ARGS)

/*****************************************************************************/
/*                STATE 1 COMBOBOX STRSHARE LIST CONTROL                     */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 combobox for
 * state attribute. A predefined list fill the combobox.
 *
 * @see COMMON_COMBOBOX_LIST_ADD
 *
 * @ingroup Property_Macro
 */
#define STATE_STRSHARE_ATR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP) \
   COMMON_COMBOBOX_LIST_ADD(STATE_STRSHARE, TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, STATE_ARGS)

/**
 * Macro defines a function that updates control by STATE_STRSHARE_ATR_1COMBOBOX_LIST_ADD macro.
 *
 * @see COMMON_COMBOBOX_LIST_STRSHARE_UPDATE
 *
 * @ingroup Property_Macro
 */
#define STATE_STRSHARE_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_COMBOBOX_LIST_STRSHARE_UPDATE(SUB, VALUE, MEMBER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_STRSHARE_ATR_1COMBOBOX_LIST_ADD.
 *
 * @see COMMON_COMBOBOX_LIST_STRSHARE_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define STATE_STRSHARE_ATR_1COMBOBOX_LIST_CALLBACK(SUB, VALUE, MEMBER) \
   COMMON_COMBOBOX_LIST_STRSHARE_CALLBACK(SUB, VALUE, MEMBER, STATE_ARGS, STATE_ARGS)

/*****************************************************************************/
/*                          STATE 1 COLOR CONTROL                            */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 entry for
 * state color attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The color member from Prop_Data structure
 * @param TOOLTIP The tooltip for combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_COLOR_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   pd->MEMBER.VALUE = elm_layout_add(item); \
   elm_layout_theme_set(pd->MEMBER.VALUE, "layout", "property", "color"); \
   evas_object_size_hint_weight_set(pd->MEMBER.VALUE,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(pd->MEMBER.VALUE, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   pd->MEMBER.VALUE##_obj = elm_layout_add(parent); \
   elm_layout_theme_set(pd->MEMBER.VALUE##_obj, "image", "color", "color_set"); \
   evas_object_event_callback_add(pd->MEMBER.VALUE, EVAS_CALLBACK_MOUSE_DOWN, \
                                  _on_##MEMBER##_##VALUE##_clicked, pd); \
   elm_layout_content_set(pd->MEMBER.VALUE, NULL, pd->MEMBER.VALUE##_obj); \
   elm_layout_content_set(item, NULL, pd->MEMBER.VALUE); \
   prop_##SUB##_##VALUE##_update(pd); \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_COLOR_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The color member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_COLOR_LIST_UPDATE(SUB, VALUE, MEMBER) \
static void \
prop_##SUB##_##VALUE##_update(Prop_Data *pd) \
{ \
   int r, g, b, a; \
   edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name, \
                                   pd->wm_part->curr_state, pd->wm_part->curr_state_value, \
                                   &r, &g, &b, &a); \
   evas_object_color_set(pd->MEMBER.VALUE##_obj, r*a/255, g*a/255, b*a/255, a); \
}

/**
 * Macro defines a callback for STATE_ATTR_COLOR_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The color member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_COLOR_CALLBACK(SUB, VALUE, MEMBER) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   int r, g, b, a; \
   int old_r, old_g, old_b, old_a; \
   Prop_Data *pd = (Prop_Data *)data; \
   edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name, \
                                   pd->wm_part->curr_state, \
                                   pd->wm_part->curr_state_value, \
                                   &old_r, &old_g, &old_b, &old_a); \
   colorselector_color_get(obj, &r, &g, &b, &a); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                        pd->wm_part->curr_state, \
                                        pd->wm_part->curr_state_value, \
                                        r, g, b, a))\
     return; \
   evas_object_color_set(pd->MEMBER.VALUE##_obj, r*a/255, g*a/255, b*a/255, a); \
   edje_edit_state_color_class_set(pd->wm_style->obj, pd->wm_part->name, \
                                   pd->wm_part->curr_state, \
                                   pd->wm_part->curr_state_value, NULL); \
   if ((r != old_r) || (g != old_g) || (b != old_b) || (a != old_a)) \
     history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_FOUR, old_r, old_g, old_b, \
                      old_a, r, g, b, a, pd->wm_style->full_group_name, \
                      (void*)edje_edit_##SUB##_##VALUE##_set, #SUB"_"#VALUE, \
                      pd->wm_part->name, pd->wm_part->curr_state, \
                      pd->wm_part->curr_state_value); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
} \
static void \
_on_##MEMBER##_##VALUE##_dismissed(void *data, \
                                   Evas_Object *obj, \
                                   void *event_info __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   evas_object_smart_callback_del_full(obj, "color,changed", \
                                       _on_##MEMBER##_##VALUE##_change, pd); \
   evas_object_smart_callback_del_full(obj, "palette,item,selected", \
                                       _on_##MEMBER##_##VALUE##_change, pd); \
   evas_object_smart_callback_del_full(obj, "dismissed", \
                                       _on_##MEMBER##_##VALUE##_dismissed, pd); \
   evas_object_hide(obj); \
} \
static void \
_on_##MEMBER##_##VALUE##_clicked(void *data, \
                                 Evas *e __UNUSED__, \
                                 Evas_Object *obj, \
                                 void *event_info __UNUSED__) \
{ \
   int x, y; \
   int r, g, b, a; \
   Evas_Object *colorsel; \
   Prop_Data *pd = (Prop_Data *)data; \
   colorsel = colorselector_get(); \
   evas_object_color_get(pd->MEMBER.VALUE##_obj, &r, &g, &b, &a); \
   colorselector_color_set(colorsel, r, g, b, a); \
   evas_object_smart_callback_add(colorsel, "color,changed", \
                                  _on_##MEMBER##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(colorsel, "palette,item,selected", \
                                  _on_##MEMBER##_##VALUE##_change, pd); \
   evas_object_smart_callback_add(colorsel, "dismissed", \
                                  _on_##MEMBER##_##VALUE##_dismissed, pd); \
   evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y); \
   evas_object_move(colorsel, x, y); \
   evas_object_show(colorsel); \
}

/*****************************************************************************/
/*                       STATE 1 COMBOBOX CONTROL                            */
/*****************************************************************************/
/**
 * Macro for functions that create an item with label and 1 combobox for state
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE The value of state attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP)

/**
 * Macro defines the callback for STATE_ATTR_1COMBOBOX_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE The value of state attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   App_Data *ap = app_data_get(); \
   Ewe_Combobox_Item *item = ei; \
   const char *old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                                           pd->wm_part->name, \
                                                           pd->wm_part->curr_state, \
                                                           pd->wm_part->curr_state_value);\
   edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                   pd->wm_part->curr_state, pd->wm_part->curr_state_value, \
                                   !strcmp(item->title, _("None")) ? NULL : item->title); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_STRING, old_value, \
                    item->title, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   pm_save_to_dev(ap->project, pd->wm_style); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
   edje_edit_string_free(old_value); \
}

/*****************************************************************************/
/*                       STATE 2 COMBOBOX CONTROL                            */
/*****************************************************************************/
/**
 * Macro for functions that create an item with label and 1 combobox for state
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMEBER The combobox member from Prop_Data structure
 * @paramram TOOLTIP1 The tooltip for first combobox
 * @paramram TOOLTIP2 The tooltip for second combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow_vertical_pad") \
   elm_object_part_text_set(item, "label.swallow1.start", _("x:")); \
   EWE_COMBOBOX_ADD(item, pd->MEMBER.VALUE1) \
   if (TOOLTIP1) elm_object_tooltip_text_set(pd->MEMBER.VALUE1, TOOLTIP1); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE1, "selected", \
                                  _on_##MEMBER##_##VALUE1##_change, pd); \
   elm_object_part_content_set(item, "swallow.content1", pd->MEMBER.VALUE1); \
   elm_object_part_text_set(item, "label.swallow2.start", _("y:")); \
   EWE_COMBOBOX_ADD(item, pd->MEMBER.VALUE2) \
   if (TOOLTIP2) elm_object_tooltip_text_set(pd->MEMBER.VALUE2, TOOLTIP2); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE2, "selected", \
                                  _on_##MEMBER##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "swallow.content2", pd->MEMBER.VALUE2); \
   prop_##MEMBER##_##VALUE1##_update(pd); \
   prop_##MEMBER##_##VALUE2##_update(pd); \
   return item; \
}

/*****************************************************************************/
/*                       STATE 1COMBOBOX SOURCE UPDATE                       */
/*****************************************************************************/
/**
 * Macro defines a function that updates any 'source' attribute control macro.
 *
 * @param SUB The prefix of main parameter
 * @param VALUE The 'source' value (ex: text_source)
 * @param MEMBER The entry member from Prop_Data structure
 * @param PART_TYPE The part type that be excluded from dropdown list or
 *        only includeted to dropdown list, depending on the argument COMPARE
 * @param COMPARE The compare argument
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_SOURCE_UPDATE(SUB, VALUE, MEMBER, EXCLUDE_PART, COMPARE) \
static void \
prop_##MEMBER##_##VALUE##_update(Prop_Data *pd) \
{ \
   Part *part; \
   Eina_Inlist *list_n = NULL; \
   Eina_Stringshare *value; \
   ewe_combobox_items_list_free(pd->MEMBER.VALUE, true); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                           pd->wm_part->name, \
                                           pd->wm_part->curr_state, \
                                           pd->wm_part->curr_state_value); \
   ewe_combobox_item_add(pd->MEMBER.VALUE, _("None")); \
   ewe_combobox_text_set(pd->MEMBER.VALUE, value ? value : _("None")); \
   EINA_INLIST_FOREACH_SAFE(pd->wm_style->parts, list_n, part) \
     { \
        if ((part != pd->wm_part) && (part->type COMPARE EXCLUDE_PART)) \
          ewe_combobox_item_add(pd->MEMBER.VALUE, part->name); \
     } \
   edje_edit_string_free(value); \
}

/*****************************************************************************/
/*                          STATE 1 ENTRY CONTROL                            */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 entry for state
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The entry member from Prop_Data structure
 * @param REGEX The regex for validation input string
 * @param TOOLTIP The tooltip for combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, REGEX, TOOLTIP) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Prop_Data *pd, \
                           void *btn_func_cb) \
{ \
   Evas_Object *btn; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_ENTRY_ADD(item, pd->MEMBER.VALUE, true) \
   if (btn_func_cb) \
     { \
        btn = elm_button_add(parent); \
        elm_object_style_set(btn, "elipsis"); \
        evas_object_smart_callback_add(btn, "clicked", btn_func_cb, pd); \
        evas_object_smart_callback_add(pd->MEMBER.VALUE, "clicked", btn_func_cb, pd); \
        elm_object_part_content_set(pd->MEMBER.VALUE, "elm.swallow.end", btn); \
        elm_entry_editable_set(pd->MEMBER.VALUE, false); \
        evas_object_show(btn); \
     } \
   else \
     evas_object_smart_callback_add(pd->MEMBER.VALUE, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
   ewe_entry_regex_set(pd->MEMBER.VALUE, REGEX, EWE_REG_EXTENDED); \
   ewe_entry_regex_autocheck_set(pd->MEMBER.VALUE, true); \
   ewe_entry_regex_glow_set(pd->MEMBER.VALUE, true); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   elm_layout_content_set(item, NULL, pd->MEMBER.VALUE); \
   prop_##SUB##_##VALUE##_update(pd); \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_1ENTRY_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @paramram MEMBER The entry member from Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
static void \
prop_##SUB##_##VALUE##_update(Prop_Data *pd) \
{ \
   const char *value; \
   value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name, \
                                           pd->wm_part->curr_state, \
                                           pd->wm_part->curr_state_value); \
   char *text = elm_entry_utf8_to_markup(value); \
   ewe_entry_entry_set(pd->MEMBER.VALUE, text); \
   edje_edit_string_free(value); \
   free(text); \
}

/**
 * Macro defines a callback for STATE_ATTR_1ENTRY_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1ENTRY_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   if (ewe_entry_regex_error_get(obj)) return; \
   const char *text = elm_entry_entry_get(obj); \
   char *value = elm_entry_markup_to_utf8(text); \
   const char *old_value =  edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                               pd->wm_part->name, pd->wm_part->curr_state, \
                               pd->wm_part->curr_state_value); \
   edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                   pd->wm_part->curr_state, \
                                   pd->wm_part->curr_state_value, value); \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_STRING, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, pd->wm_part->curr_state, \
                    pd->wm_part->curr_state_value); \
   elm_object_focus_set(obj, true); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
   eina_stringshare_del(old_value); \
   free(value); \
}

/*****************************************************************************/
/*              STATE CONTAINER 2 SPINNERS DOUBLEVAL CONTROL                 */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 spinners for
 * state container attribute.
 *
 * @see COMMON_2SPINNER_ADD
 *
 * @ingroup Property_Macro
 */
#define STATE_DOUBLEVAL_ATTR_2SPINNER_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TYPE, \
                                          MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                                          TOOLTIP1, TOOLTIP2, MULTIPLIER) \
   COMMON_2SPINNER_ADD(STATE_DOUBLEVAL, TEXT, "2swallow", SUB, VALUE1, VALUE2, MEMBER, TYPE,\
                       MIN, MAX, STEP, FMT, L1_START, L1_END, L2_START, L2_END, \
                       TOOLTIP1, TOOLTIP2, MULTIPLIER)

/**
 * Macro defines a function that updates control by STATE_CONTAINER_DOUBLEVAL_ATTR_2SPINNER_ADD macro.
 *
 * @see COMMON_2SPINNER_DOUBLEVAL_UPDATE
 *
 * @ingroup Property_Macro
 */
#define STATE_DOUBLEVAL_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER) \
   COMMON_2SPINNER_DOUBLEVAL_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_CONTAINER_DOUBLEVAL_ATTR_2SPINNER_ADD.
 *
 * @see COMMON_2SPINNER_DOUBLEVAL_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define STATE_DOUBLEVAL_ATTR_2SPINNER_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER) \
   COMMON_2SPINNER_DOUBLEVAL_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER, STATE_ARGS)

/*****************************************************************************/
/*               STATE CONTAINER 2 CHECK DOUBLEVAL CONTROL                   */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 check for
 * state container attribute.
 *
 * @see COMMON_2CHECK_ADD
 *
 * @ingroup Property_Macro
 */
#define STATE_DOUBLEVAL_ATTR_2CHECK_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                                        L1_START, L1_END, L2_START, L2_END, \
                                        TOOLTIP1, TOOLTIP2) \
   COMMON_2CHECK_ADD(STATE_DOUBLEVAL, TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                     L1_START, L1_END, L2_START, L2_END, TOOLTIP1, TOOLTIP2)

/**
 * Macro defines a function that updates control by STATE_CONTAINER_DOUBLEVAL_ATTR_2CHECK_ADD macro.
 *
 * @see COMMON_2CHECK_DOUBLEVAL_UPDATE
 *
 * @ingroup Property_Macro
 */
#define STATE_DOUBLEVAL_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2, MEMBER) \
   COMMON_2CHECK_DOUBLEVAL_UPDATE(SUB, VALUE1, VALUE2, MEMBER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_CONTAINER_DOUBLEVAL_ATTR_2CHECK_ADD.
 *
 * @see COMMON_2CHECK_DOUBLEVAL_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define STATE_DOUBLEVAL_ATTR_2CHECK_CALLBACK(SUB, VALUE1, VALUE2, MEMBER) \
   COMMON_2CHECK_DOUBLEVAL_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, STATE_ARGS)


/** @} privatesection */
