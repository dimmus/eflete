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

#define PROP_ITEM_UNSET(BOX, ITEM) \
   if (ITEM) \
     {\
        evas_object_smart_callback_del(ITEM, "clicked", _on_frame_click); \
        elm_box_unpack(BOX, ITEM); \
        evas_object_del(ITEM); \
        ITEM = NULL; \
     }
/*
 * Callback is added for frames at property box to correct scroller
 * work while each frame would be expanded/collapsed
 * TODO: Documentation
 */
#define FRAME_PROPERTY_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

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
   assert(PARENT != NULL); \
   LAYOUT_PROP_ADD(PARENT, NAME, "property", STYLE)

#define GROUP_ARGS
#define PART_ARGS , pd->part->name
#define PART_ITEM_ARGS , pd->part->name, pd->item_name
#define STATE_ARGS , pd->part->name, pd->part->current_state->parsed_name, pd->part->current_state->parsed_val
#define PROGRAM_ARGS , pd->attributes.program.program

/*****************************************************************************/
/*                      COMMON ATTRIBUTE CONTOLS MACRO                       */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 1 spinners.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE The value of state attribute
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
#define COMMON_1SPINNER_ADD(PREFIX, TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                            L_START, L_END, TOOLTIP, MULTIPLIER) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   SPINNER_ADD(item, pd->attributes.MEMBER.VALUE, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->attributes.MEMBER.VALUE, FMT); \
   elm_layout_content_set(item, "swallow.content1", pd->attributes.MEMBER.VALUE); \
   elm_layout_text_set(item, "label.swallow1.start", L_START); \
   elm_layout_text_set(item, "label.swallow1.end", L_END); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE, TOOLTIP); \
   evas_object_event_callback_priority_add(pd->attributes.MEMBER.VALUE, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "spinner,drag,start", _on_##MEMBER##_##VALUE##_start, pd); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "spinner,drag,stop", _on_##MEMBER##_##VALUE##_stop, pd); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "changed", _on_##MEMBER##_##VALUE##_change, pd); \
   PREFIX##_ATTR_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE, MULTIPLIER) \
   return item; \
}
/**
 * Macro defines a functions that create an item with label and 2 spinners.
 *
 * @param PREFIX The attribute prefix (STATE, PART, etc), used for define the
 *        update function
 * @param TEXT The label text
 * @param SUB The prefix of the attribute
 * @param VALUE1 The first value of the attribute
 * @param VALUE2 The second value of the attribute
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, STYLE) \
   SPINNER_ADD(item, pd->attributes.MEMBER.VALUE1, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->attributes.MEMBER.VALUE1, FMT); \
   elm_layout_content_set(item, "swallow.content1", pd->attributes.MEMBER.VALUE1); \
   elm_layout_text_set(item, "label.swallow1.start", L1_START); \
   elm_layout_text_set(item, "label.swallow1.end", L1_END); \
   if (TOOLTIP1) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE1, TOOLTIP1); \
   evas_object_event_callback_priority_add(pd->attributes.MEMBER.VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE1, "spinner,drag,start", _on_##MEMBER##_##VALUE1##_start, pd); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE1, "spinner,drag,stop", _on_##MEMBER##_##VALUE1##_stop, pd); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE1, "changed", _on_##MEMBER##_##VALUE1##_change, pd); \
   SPINNER_ADD(item, pd->attributes.MEMBER.VALUE2, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->attributes.MEMBER.VALUE2, FMT); \
   elm_layout_content_set(item, "swallow.content2", pd->attributes.MEMBER.VALUE2); \
   elm_layout_text_set(item, "label.swallow2.start", L2_START); \
   elm_layout_text_set(item, "label.swallow2.end", L2_END); \
   if (TOOLTIP2) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE2, TOOLTIP2); \
   evas_object_event_callback_priority_add(pd->attributes.MEMBER.VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE2, "spinner,drag,start", _on_##MEMBER##_##VALUE2##_start, pd); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE2, "spinner,drag,stop", _on_##MEMBER##_##VALUE2##_stop, pd); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE2, "changed", _on_##MEMBER##_##VALUE2##_change, pd); \
   PREFIX##_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER); \
   return item; \
}

/**
 * Macro defines a function call with arguments that updates control by
 * COMMON_1(2)SPINNER_ADD macro.
 *
 * @param SUB The prefix of the attribute
 * @param VALUE The first value of the attribute
 * @param MEMBER The spinner member from Group_Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, ARGS) \
   elm_spinner_value_set(pd->attributes.MEMBER.VALUE, \
                         MULTIPLIER * edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS)); \

/**
 * Macro defines a COMMON_1(2)SPINNER_ADD.
 *
 * @param SUB The prefix of the attribute
 * @param VALUE The value of the attribute
 * @param TYPE The spinner value type: int, double
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, ARGS, DESCRIPTION) \
static void \
_on_##MEMBER##_##VALUE##_start(void *data, \
                               Evas_Object *obj __UNUSED__, \
                               void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   elm_object_focus_set(obj, true); /* there are problems with unfocusing entry.
                              elementary is too lazy to change focus in time */ \
   assert(pd->change == NULL); \
   pd->change = change_add(NULL); \
   pd->old_##TYPE##_val = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
} \
static void \
_on_##MEMBER##_##VALUE##_stop(void *data, \
                              Evas_Object *obj __UNUSED__, \
                              void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Eina_Stringshare *msg; \
   assert(pd->change != NULL); \
   TYPE new_val = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
   if (new_val != pd->old_##TYPE##_val) \
     { \
        msg = eina_stringshare_printf(DESCRIPTION, pd->old_##TYPE##_val, new_val); \
        change_description_set(pd->change, msg); \
        eina_stringshare_del(msg); \
        history_change_add(pd->group->history, pd->change); \
     } \
   else \
     change_free(pd->change); \
   pd->change = NULL; \
} \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   TYPE value = elm_spinner_value_get(obj); \
   value /= MULTIPLIER; \
   if (pd->change) \
     { \
        if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, pd->change, true ARGS, value)) \
        { \
           ERR("editor_group_"#SUB"_"#VALUE"_set failed"); \
           abort(); \
        } \
     } \
   else \
     { \
        _on_##MEMBER##_##VALUE##_start(data, obj, ei); \
        if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, pd->change, true ARGS, value)) \
        { \
           ERR("editor_"#SUB"_"#VALUE"_set failed"); \
           abort(); \
        } \
        _on_##MEMBER##_##VALUE##_stop(data, obj, ei); \
     } \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
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
 * @param MEMBER The combobox member from Group_Prop_Data structure
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
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   int i; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(parent, pd->attributes.MEMBER.VALUE) \
   for (i = 0; LIST[i]; ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, LIST[i]), i++) ; \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.MEMBER.VALUE); \
   PREFIX##_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   return item; \
}

/**
 * Macro defines a function that updates control by COMMON_COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Group_Prop_Data structure
 * @param ARGS
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER, ARGS) \
   ewe_combobox_select_item_set(pd->attributes.MEMBER.VALUE, (int)edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS));

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
#define COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, ARGS, DESCRIPTION) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                          Evas_Object *obj __UNUSED__, \
                          void *event_info) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info; \
   TYPE old_val = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
   if (item->index == old_val) \
     return; \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION, item->title); \
   Change *change = change_add(msg); \
   eina_stringshare_del(msg); \
   if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, change, false ARGS, (TYPE)item->index)) \
     { \
        ERR("Cann't apply value '%s' for attribute '"#TEXT"'.", item->title); \
        abort(); \
     } \
   history_change_add(pd->group->history, change); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
}

/**
 * Macro defines functions that create an item with label and 1 check for part
 * attribute.
 *
 * @param PREFIX The attribute prefix (STATE, PART, etc), used for define the
 *        update function
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Group_Prop_Data structure
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
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow_subtext") \
   CHECK_ADD(item, pd->attributes.MEMBER.VALUE) \
   elm_object_style_set(pd->attributes.MEMBER.VALUE, "toggle"); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "changed", _on_##MEMBER##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.MEMBER.VALUE); \
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
 * @param MEMBER The check member from Group_Prop_Data structure
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 *
 * @ingroup Property_Macro
 */
#define COMMON_2CHECK_ADD(PREFIX, TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                          L1_START, L1_END, L2_START, L2_END, \
                          TOOLTIP1, TOOLTIP2) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   CHECK_ADD(item, pd->attributes.MEMBER.VALUE1) \
   elm_object_style_set(pd->attributes.MEMBER.VALUE1, "toggle"); \
   if (TOOLTIP1) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE1, TOOLTIP1); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE1, "changed", _on_##MEMBER##_##VALUE1##_change, pd); \
   elm_layout_content_set(item, "swallow.content1", pd->attributes.MEMBER.VALUE1); \
   elm_layout_text_set(item, "label.swallow1.start", L1_START); \
   elm_layout_text_set(item, "label.swallow1.end", L1_END); \
   CHECK_ADD(item, pd->attributes.MEMBER.VALUE2) \
   elm_object_style_set(pd->attributes.MEMBER.VALUE2, "toggle"); \
   if (TOOLTIP2) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE2, TOOLTIP2); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE2, "changed", _on_##MEMBER##_##VALUE2##_change, pd); \
   elm_layout_content_set(item, "swallow.content2", pd->attributes.MEMBER.VALUE2); \
   elm_layout_text_set(item, "label.swallow2.start", L2_START); \
   elm_layout_text_set(item, "label.swallow2.end", L2_END); \
   PREFIX##_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2, MEMBER) \
   return item; \
}

/* Macro defines a function that updates control by COMMON_CHECK_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Group_Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_CHECK_UPDATE(SUB, VALUE, MEMBER, ARGS) \
   elm_check_state_set(pd->attributes.MEMBER.VALUE, edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS));

/**
 * Macro defines a callback for attribute that controled by check.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The check member from Group_Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @note for internal usage in property_macros.h
 *
 * @ingroup Property_Macro
 */
#define COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, ARGS, DESCRIPTION) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION, value?_("true"):_("false")); \
   Change *change = change_add(msg); \
   eina_stringshare_del(msg); \
   if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, change, false ARGS, value)) \
     { \
       ERR("editor_"#SUB"_"#VALUE"_set failed"); \
       abort(); \
     } \
   history_change_add(pd->group->history, change); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
}

/**
 * Macro defines a function that updates control by COMMON_COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Group_Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_STRSHARE_UPDATE(SUB, VALUE, MEMBER, ARGS) \
   Eina_Stringshare *value = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
   ewe_combobox_text_set(pd->attributes.MEMBER.VALUE, value ? value : _("None")); \
   edje_edit_string_free(value);


/**
 * Macro defines a callback for COMMON_COMBOBOX_ADD.
 *
 * @param TEXT The attribute name, for error message
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_COMBOBOX_LIST_STRSHARE_CALLBACK(SUB, VALUE, MEMBER, ARGS) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *event_info) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->group->edit_object ARGS, (char *)item->title)) \
     { \
       ERR("edje_edit_"#SUB"_"#VALUE"_set failed"); \
       abort(); \
     } \
   /*project_changed(false);*/ \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
}

/**
 * Macro defines functions that create an item with label and 1 entry.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The entry member from Group_Prop_Data structure
 * @param VALIDATOR The entry validator
 * @param TOOLTIP The tooltip for combobox
 *
 * @ingroup Property_Macro
 */
#define COMMON_ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Group_Prop_Data *pd, \
                           void *btn_func_cb) \
{ \
   Evas_Object *btn; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   ENTRY_ADD(item, pd->attributes.MEMBER.VALUE, true) \
   if (btn_func_cb) \
     { \
        btn = elm_button_add(parent); \
        elm_object_style_set(btn, "elipsis"); \
        evas_object_smart_callback_add(btn, "clicked", btn_func_cb, pd); \
        evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "clicked", btn_func_cb, pd); \
        elm_object_part_content_set(pd->attributes.MEMBER.VALUE, "elm.swallow.elipsis", btn); \
        elm_entry_editable_set(pd->attributes.MEMBER.VALUE, false); \
        evas_object_show(btn); \
     } \
   else \
     { \
       evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "changed,user", _on_##SUB##_##VALUE##_change, pd); \
       evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "activated", _on_##SUB##_##VALUE##_activated, pd); \
       evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "unfocused", _on_##SUB##_##VALUE##_activated, pd); \
     } \
   if (VALIDATOR) \
      eo_do(pd->attributes.MEMBER.VALUE, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, VALIDATOR)); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE, TOOLTIP); \
   elm_layout_content_set(item, NULL, pd->attributes.MEMBER.VALUE); \
   prop_##SUB##_##VALUE##_update(pd); \
   return item; \
}

/**
 * Macro defines a function that updates control by COMMON_ENTRY_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The entry member from Group_Prop_Data structure
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_ENTRY_UPDATE(SUB, VALUE, MEMBER, ARGS) \
static void \
prop_##SUB##_##VALUE##_update(Group_Prop_Data *pd) \
{ \
   const char *value; \
   value = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
   char *text = elm_entry_utf8_to_markup(value); \
   if (strcmp(text, elm_entry_entry_get(pd->attributes.MEMBER.VALUE))) \
     elm_entry_entry_set(pd->attributes.MEMBER.VALUE, text); \
   edje_edit_string_free(value); \
   free(text); \
}

/**
 * Macro defines a callback for COMMON_ENTRY_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param VALIDATOR The entry validator
 * @param ARGS The edje edit function arguments
 *
 * @ingroup Property_Macro
 */
#define COMMON_ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, ARGS, DESCRIPTION) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   if (VALIDATOR && (elm_validator_regexp_status_get(VALIDATOR)) != ELM_REG_NOERROR) \
     return; \
   if (!pd->change) pd->change = change_add(NULL); \
   const char *text = elm_entry_entry_get(obj); \
   char *value = elm_entry_markup_to_utf8(text); \
   editor_##SUB##_##VALUE##_set(pd->group->edit_object, pd->change, true ARGS, value); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
   free(value); \
} \
static void \
_on_##SUB##_##VALUE##_activated(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   if (VALIDATOR && (elm_validator_regexp_status_get(VALIDATOR)) != ELM_REG_NOERROR) \
     return; \
   if (!pd->change) \
     return; \
   Eina_Stringshare *val = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION, val); \
   change_description_set(pd->change, msg); \
   history_change_add(pd->group->history, pd->change); \
   pd->change = NULL; \
   prop_##SUB##_##VALUE##_update(pd); \
   eina_stringshare_del(msg); \
   eina_stringshare_del(val); \
}

/**
 * Macro defines a creating of COMMON_ATTR_2COMBOBOX.
 *
 * @param TEXT Text of attribute mix
 * @param SUB The prefix of main parameter of attribute
 * @param VALUE1 The first value of attribute
 * @param VALUE2 The second value of attribute
 * @param MEMBER position of attribute controls in struct
 * @param TOOLTIP1 tooltip for first control
 * @param TOOLTIP2 tooltip for second control
 * @param LABEL1 label for first control
 * @param LABEL2 label for second control
 *
 * @ingroup Property_Macro
 */
#define COMMON_ATTR_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, LABEL1, LABEL2) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow_vertical_pad") \
   elm_object_part_text_set(item, "label.swallow1.start", LABEL1); \
   EWE_COMBOBOX_ADD(item, pd->attributes.MEMBER.VALUE1) \
   if (TOOLTIP1) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE1, TOOLTIP1); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE1, "selected", \
                                  _on_##MEMBER##_##VALUE1##_change, pd); \
   elm_object_part_content_set(item, "swallow.content1", pd->attributes.MEMBER.VALUE1); \
   elm_object_part_text_set(item, "label.swallow2.start", LABEL2); \
   EWE_COMBOBOX_ADD(item, pd->attributes.MEMBER.VALUE2) \
   if (TOOLTIP2) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE2, TOOLTIP2); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE2, "selected", \
                                  _on_##MEMBER##_##VALUE2##_change, pd); \
   elm_object_part_content_set(item, "swallow.content2", pd->attributes.MEMBER.VALUE2); \
   prop_##MEMBER##_##VALUE1##_update(pd); \
   prop_##MEMBER##_##VALUE2##_update(pd); \
   return item; \
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
                                             Group_Prop_Data *pd, \
                                             const char *tooltip1, \
                                             const char *tooltip2) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   elm_object_part_text_set(item, "label.swallow1.start", "w:"); \
   elm_object_part_text_set(item, "label.swallow1.end", "px"); \
   SPINNER_ADD(item, pd->attributes.group.SUB##_##VALUE1, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->attributes.group.SUB##_##VALUE1, "%.0f"); \
   if (tooltip1) elm_object_tooltip_text_set(pd->attributes.group.SUB##_##VALUE1, tooltip1); \
   evas_object_smart_callback_add(pd->attributes.group.SUB##_##VALUE1, "changed", _on_group_##SUB##_##VALUE1##_change, pd); \
   evas_object_smart_callback_add(pd->attributes.group.SUB##_##VALUE1, "spinner,drag,start", _on_group_##SUB##_##VALUE1##_start, pd); \
   evas_object_smart_callback_add(pd->attributes.group.SUB##_##VALUE1, "spinner,drag,stop", _on_group_##SUB##_##VALUE1##_stop, pd); \
   elm_object_part_content_set(item, "swallow.content1", pd->attributes.group.SUB##_##VALUE1); \
   evas_object_event_callback_priority_add(pd->attributes.group.SUB##_##VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   elm_object_part_text_set(item, "label.swallow2.start", "h:"); \
   elm_object_part_text_set(item, "label.swallow2.end", "px"); \
   SPINNER_ADD(item, pd->attributes.group.SUB##_##VALUE2, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->attributes.group.SUB##_##VALUE2, "%.0f"); \
   if (tooltip2) elm_object_tooltip_text_set(pd->attributes.group.SUB##_##VALUE2, tooltip2); \
   evas_object_smart_callback_add(pd->attributes.group.SUB##_##VALUE2, "changed", _on_group_##SUB##_##VALUE2##_change, pd); \
   evas_object_smart_callback_add(pd->attributes.group.SUB##_##VALUE2, "spinner,drag,start", _on_group_##SUB##_##VALUE2##_start, pd); \
   evas_object_smart_callback_add(pd->attributes.group.SUB##_##VALUE2, "spinner,drag,stop", _on_group_##SUB##_##VALUE2##_stop, pd); \
   elm_object_part_content_set(item, "swallow.content2", pd->attributes.group.SUB##_##VALUE2); \
   evas_object_event_callback_priority_add(pd->attributes.group.SUB##_##VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
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
prop_group_##SUB##_##VALUE1##_##VALUE2##_update(Group_Prop_Data *pd) \
{ \
   elm_spinner_value_set(pd->attributes.group.SUB##_##VALUE1, edje_edit_group_##SUB##_##VALUE1##_get(pd->group->edit_object)); \
   elm_spinner_value_set(pd->attributes.group.SUB##_##VALUE2, edje_edit_group_##SUB##_##VALUE2##_get(pd->group->edit_object)); \
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
#define GROUP_ATTR_2SPINNER_CALLBACK(SUB1, SUB2, VALUE, DESCRIPTION) \
static void \
_on_group_##SUB1##_##VALUE##_start(void *data, \
                                   Evas_Object *obj __UNUSED__, \
                                   void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   elm_object_focus_set(obj, true); /* there are problems with unfocusing entry.
                              elementary is too lazy to change focus in time */ \
   assert(pd->change == NULL); \
   pd->change = change_add(DESCRIPTION);\
   pd->old_int_val = edje_edit_group_##SUB1##_##VALUE##_get(pd->group->edit_object); \
} \
static void \
_on_group_##SUB1##_##VALUE##_stop(void *data, \
                                  Evas_Object *obj __UNUSED__, \
                                  void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Eina_Stringshare *msg; \
   assert(pd->change != NULL); \
   int new_int_val = edje_edit_group_##SUB1##_##VALUE##_get(pd->group->edit_object); \
   if (new_int_val != pd->old_int_val) \
     { \
        msg = eina_stringshare_printf(DESCRIPTION, pd->old_int_val, new_int_val); \
        change_description_set(pd->change, msg); \
        eina_stringshare_del(msg); \
        history_change_add(pd->group->history, pd->change); \
     } \
   else \
     change_free(pd->change); \
   pd->change = NULL; \
} \
static void \
_on_group_##SUB1##_##VALUE##_change(void *data, \
                                    Evas_Object *obj, \
                                    void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   int value = (int)elm_spinner_value_get(obj); \
   if (pd->change) \
     { \
        if (!editor_group_##SUB1##_##VALUE##_set(pd->group->edit_object, pd->change, true, value)) \
        { \
           ERR("editor_group_"#SUB1"_"#VALUE"_set failed"); \
           abort(); \
        } \
     } \
   else \
     { \
        _on_group_##SUB1##_##VALUE##_start(data, obj, ei); \
        if (!editor_group_##SUB1##_##VALUE##_set(pd->group->edit_object, pd->change, true, value)) \
        { \
           ERR("editor_group_"#SUB1"_"#VALUE"_set failed"); \
           abort(); \
        } \
        _on_group_##SUB1##_##VALUE##_stop(data, obj, ei); \
     } \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
}

/*****************************************************************************/
/*                           GROUP 1 ENTRY CONTROL                            */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 entry for group
 * attribute.
 *
 * @see COMMON_ENTRY_ADD
 *
 * @ingroup Property_Macro
 */
#define GROUP_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP) \
   COMMON_ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP)

/**
 * Macro defines a function that updates control by STATE_ATTR_1ENTRY_ADD macro.
 *
 * @see COMMON_ENTRY_UPDATE
 *
 * @ingroup Property_Macro
 */
#define GROUP_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_ENTRY_UPDATE(SUB, VALUE, MEMBER, GROUP_ARGS) \

/**
 * Macro defines a callback for STATE_ATTR_1ENTRY_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 *
 * @ingroup Property_Macro
 */
#define GROUP_ATTR_1ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, DESCRIPTION) \
   COMMON_ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, GROUP_ARGS, DESCRIPTION) \

/*****************************************************************************/
/*                        PROGRAM 1 ENTRY CONTROL                            */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 entry for program
 * attribute.
 *
 * @see COMMON_ENTRY_ADD
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP) \
   COMMON_ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP)

/**
 * Macro defines a function that updates control by PROGRAM_ATTR_1ENTRY_ADD macro.
 *
 * @see COMMON_ENTRY_UPDATE
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_ENTRY_UPDATE(SUB, VALUE, MEMBER, PROGRAM_ARGS) \

/**
 * Macro defines a callback for STATE_ATTR_1ENTRY_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, DESCRIPTION) \
   COMMON_ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, PROGRAM_ARGS, DESCRIPTION) \

/*****************************************************************************/
/*                   PROGRAM 1 COMBOBOX LIST CONTROL                         */
/*****************************************************************************/
/**
 * Macro defines functions that create an item with label and 1 combobox for program
 * attribute. A predefined list fill the combobox.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The combobox member from Group_Prop_Data structure
 * @param LIST The predefined strings list
 * @param TOOLTIP The combobox tooltip
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP) \
   COMMON_COMBOBOX_LIST_ADD(PROGRAM, TEXT, SUB, VALUE, MEMBER, LIST, TOOLTIP, PROGRAM_ARGS)

/**
 * Macro defines a function that updates control by PROGRAM_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_COMBOBOX_LIST_UPDATE(SUB, VALUE, MEMBER, PROGRAM_ARGS)

/**
 * Macro defines a callback for PROGRAM_ATTR_1COMBOBOX_ADD.
 *
 * @param TEXT The attribute name, for error message
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, PROGRAM_ARGS, DESCRIPTION)

/*****************************************************************************/
/*                   PROGRAM 1 NORMAL COMBOBOX                               */
/*****************************************************************************/

/**
 * Macro for functions that create an item with label and 1 combobox for state
 * attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 * @paramram TOOLTIP1 The tooltip for first combobox
 * @paramram TOOLTIP2 The tooltip for second combobox
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, LABEL1, LABEL2) \
   COMMON_ATTR_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, LABEL1, LABEL2)

/*****************************************************************************/
/*                         PROGRAM 1 SPINNER CONTROL                         */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 1 spinners.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE The value of state attribute
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
#define PROGRAM_ATTR_1SPINNER_ADD(TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                                  L_START, L_END, TOOLTIP, MULTIPLIER) \
COMMON_1SPINNER_ADD(PROGRAM, TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                    L_START, L_END, TOOLTIP, MULTIPLIER) \

#define PROGRAM_ATTR_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE, MULTIPLIER) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE,  MULTIPLIER, PROGRAM_ARGS)

/**
 * Macro defines a callback for STATE_ATTR_1(2)SPINNER_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 * @param TYPE The spinner value type: int, double
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, DESCRIPTION) \
   COMMON_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, PROGRAM_ARGS, DESCRIPTION)

#define PROGRAM_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, TYPE, MULTIPLIER) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE1, MEMBER, TYPE,  MULTIPLIER, PROGRAM_ARGS) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE2, MEMBER, TYPE,  MULTIPLIER, PROGRAM_ARGS)

/*****************************************************************************/
/*                       PROGRAM 1 CHECK CONTROL                             */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 1 check
 *
 * @see COMMON_2CHECK_ADD
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_1CHECK_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
   COMMON_CHECK_ADD(PROGRAM, TEXT, SUB, VALUE, MEMBER, TOOLTIP, PROGRAM_ARGS)

/**
 * Macro defines a callback for attribute that controled by check.
 *
 * @see COMMON_CHECK_CALLBACK
 *
 * @ingroup Property_Macro
 */
#define PROGRAM_ATTR_CHECK_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, PROGRAM_ARGS, DESCRIPTION)

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
#define PART_ATTR_1CHECK_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, PART_ARGS, DESCRIPTION)

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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
static Evas_Object * \
prop_##MEMBER##_##VALUE##_add(Evas_Object *parent, \
                              Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(item, pd->attributes.MEMBER.VALUE) \
   prop_##MEMBER##_##VALUE##_update(pd); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->attributes.MEMBER.VALUE, "selected", _on_##MEMBER##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->attributes.MEMBER.VALUE); \
   return item; \
}

/**
 * Macro defines the callback for PART_ATTR_1COMBOBOX_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, ARGS, DESCRIPTION) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *ei) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   Eina_Stringshare *old_val = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object ARGS); \
   if (((item->index != 0) && (item->title == old_val)) /*stringshares*/ || \
       ((item->index == 0) && (old_val == NULL))) \
     { \
       eina_stringshare_del(old_val); \
       return; \
     } \
   eina_stringshare_del(old_val); \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION, item->title); \
   Change *change = change_add(msg); \
   eina_stringshare_del(msg); \
   if (item->index != 0) \
     { \
        if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, change, false \
                                          ARGS, item->title)) \
          { \
             ewe_combobox_select_item_set(obj, pd->attributes.part.previous_source); \
             change_free(change); \
             return; \
          } \
        pd->attributes.part.previous_source = item->index; \
     } \
   else \
     { \
        editor_##SUB##_##VALUE##_set(pd->group->edit_object, change, false ARGS, NULL); \
        pd->attributes.part.previous_source = 0; \
     } \
   history_change_add(pd->group->history, change); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
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
 * @param MEMBER The combobox member from Group_Prop_Data structure
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
#define PART_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, PART_ARGS, DESCRIPTION)

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
prop_##MEMBER##_##VALUE##_update(Group_Prop_Data *pd) \
{ \
   Eina_List *collections, *l; \
   const char *group, *value; \
   unsigned int i = 0; \
   ewe_combobox_items_list_free(pd->attributes.MEMBER.VALUE, true); \
   value = edje_edit_part_##VALUE##_get(pd->group->edit_object, pd->part->name); \
   if (value) ewe_combobox_text_set(pd->attributes.MEMBER.VALUE, value); \
   else ewe_combobox_text_set(pd->attributes.MEMBER.VALUE, _("None")); \
   ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, _("None")); \
   collections = edje_mmap_collection_list(ap.project->mmap_file); \
   collections = eina_list_sort(collections, eina_list_count(collections), sort_cb); \
   EINA_LIST_FOREACH(collections, l, group) \
     { \
        if (group != pd->group->name) \
          ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, group); \
        if (group == value) \
          pd->attributes.part.previous_source = i; \
        i++; \
     } \
   edje_edit_string_free(value); \
   edje_mmap_collection_list_free(collections); \
}

/*****************************************************************************/
/*                      PROGRAM MULTIPLE COMBOBOX                            */
/*****************************************************************************/
/**
 * Macro for afters and list. An list of items that is created by this defined
 * function consists a combobox, to pick program or part as target/after and
 * buttons to add new combobox, and remove old
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE1 The value of first drag parametr (turn on/off)
 * @param VALUE2 The value of second drag parametr (drag step)
 *
 * @ingroup Property_Macro
 */

#define PROGRAM_MULTIPLE_COMBOBOX(PARAM, COMBOBOX_PREVIOUS, TOOLTIP, ISNOTAFTER) \
static void \
_add_##PARAM(void *data, Evas_Object *obj, void *event_info); \
static void \
_del_##PARAM(void *data, \
             Evas_Object *obj __UNUSED__, \
             void *event_info __UNUSED__) \
{ \
   Evas_Object *item = (Evas_Object *)data; \
   Evas_Object *ic, *button, *new_item, *combo, *first_item; \
   Group_Prop_Data *pd = evas_object_data_get(item, GROUP_PROP_DATA); \
   Eina_List *items = elm_box_children_get(pd->attributes.program.PARAM##_box); \
   Eina_Stringshare *current_title; \
   /* User clicked on Delete button while there are only one target. \
    * In here we should just disable button and clear Combobox into NULL */ \
   if (eina_list_count(items) == 1) \
     { \
        first_item = eina_list_data_get(items); \
        combo = elm_layout_content_get(first_item, NULL); \
        ewe_combobox_text_set(combo, NULL); \
        button = elm_layout_content_get(first_item, "swallow.button_del"); \
        elm_object_disabled_set(button, true); \
     } \
   /* check the first item, if deleted object the first in the list, we need to \
    * set the label to next item and move btn_add */ \
   if ((eina_list_data_get(items) == item) && (eina_list_count(items) != 1)) \
     { \
        new_item = eina_list_data_get(eina_list_next(items)); \
        button = elm_button_add(new_item); \
        ic = elm_icon_add(button); \
        elm_icon_standard_set(ic, "plus"); \
        elm_object_part_content_set(button, NULL, ic); \
        elm_layout_content_set(item, "swallow.button_add", button); \
        evas_object_smart_callback_add(button, "clicked", _add_##PARAM, pd); \
        elm_layout_content_set(new_item, "swallow.button_add", button); \
     } \
   combo = elm_layout_content_get(item, NULL); \
   Eina_Stringshare *value = evas_object_data_get(combo, COMBOBOX_PREVIOUS); \
   if (value) \
     { \
        Eina_Stringshare *msg = eina_stringshare_printf(_("removed "#PARAM" %s from program %s"), \
                                                        value, \
                                                        pd->attributes.program.program); \
        Change *change = change_add(msg); \
        eina_stringshare_del(msg); \
        editor_program_##PARAM##_del(pd->group->edit_object, change, false, \
                                     pd->attributes.program.program, \
                                     value); \
        history_change_add(pd->group->history, change); \
        current_title = evas_object_data_get(combo, COMBOBOX_PREVIOUS); \
        eina_stringshare_del(current_title); \
        evas_object_data_del(combo, COMBOBOX_PREVIOUS); \
        eina_stringshare_del(value); \
     } \
   /* detele and unpack object if there were more than one item */ \
   if (eina_list_count(items) != 1) \
     { \
        elm_box_unpack(pd->attributes.program.PARAM##_box, item); \
        evas_object_del(item); \
     } \
   /* do a final check, if there are no items (only one) and its empty then \
    * just disable button */ \
   items = elm_box_children_get(pd->attributes.program.PARAM##_box); \
   if (eina_list_count(items) == 1) \
     { \
        first_item = eina_list_data_get(items); \
        combo = elm_layout_content_get(first_item, NULL); \
        current_title = evas_object_data_get(combo, COMBOBOX_PREVIOUS); \
        if (!current_title) \
          { \
             button = elm_layout_content_get(first_item, "swallow.button_del"); \
             elm_object_disabled_set(button, true); \
          } \
     } \
} \
static void \
_on_##PARAM##_change(void *data, \
                     Evas_Object *obj, \
                     void *ei) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   Evas_Object *first_item, *button; \
   Eina_List *items; \
   Eina_Stringshare *old_val = evas_object_data_get(obj, COMBOBOX_PREVIOUS); \
   if ((old_val) && (item->title == old_val)) \
     { \
       eina_stringshare_del(old_val); \
       return; \
     } \
   items = elm_box_children_get(pd->attributes.program.PARAM##_box); \
   if (item->title) \
     { \
        first_item = eina_list_data_get(items); \
        button = elm_layout_content_get(first_item, "swallow.button_del"); \
        elm_object_disabled_set(button, false); \
     } \
   Eina_Stringshare *msg = eina_stringshare_printf(_("changing "#PARAM" from %s to %s in program %s"), \
                                                   old_val, item->title, \
                                                   pd->attributes.program.program); \
   Change *change = change_add(msg); \
   if (old_val) \
     { \
        eina_stringshare_del(msg); \
        editor_program_##PARAM##_del(pd->group->edit_object, change, false, \
                                     pd->attributes.program.program, \
                                     old_val); \
        evas_object_data_del(obj, COMBOBOX_PREVIOUS); \
        eina_stringshare_del(old_val); \
     } \
   evas_object_data_set(obj, COMBOBOX_PREVIOUS, eina_stringshare_add(item->title)); \
   editor_program_##PARAM##_add(pd->group->edit_object, change, false, \
                                pd->attributes.program.program, \
                                item->title); \
   history_change_add(pd->group->history, change); \
} \
static void \
_add_##PARAM(void *data, \
             Evas_Object *obj __UNUSED__, \
             void *event_info __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *) data; \
   Evas_Object *combo, *ic, *button, *item; \
   Eina_List *items = elm_box_children_get(pd->attributes.program.PARAM##_box); \
   Eina_List *l; \
   Resource *program; \
   Part *part; \
   if (eina_list_count(items) == 1) \
     { \
        /* enable the 'del' button of first item, make posible to delete the \
         * first item */ \
        button = elm_layout_content_get(eina_list_data_get(items), "swallow.button_del"); \
        elm_object_disabled_set(button, false); \
     } \
   LAYOUT_PROP_ADD(pd->attributes.program.PARAM##_box, NULL, "tab_home", "item") \
   evas_object_data_set(item, GROUP_PROP_DATA, pd); \
   EWE_COMBOBOX_ADD(item, combo); \
   ewe_combobox_text_set(combo, _("None")); \
   /* fill up with part and program list */ \
   if (ISNOTAFTER) \
     EINA_LIST_FOREACH(pd->group->parts, l, part) \
       { \
          ewe_combobox_item_add(combo, part->name); \
       } \
   Edje_Action_Type type = edje_edit_program_action_get(pd->group->edit_object, \
                                                        pd->attributes.program.program); \
   if ((type == EDJE_ACTION_TYPE_ACTION_STOP) || (!ISNOTAFTER))\
     { \
        EINA_LIST_FOREACH(pd->group->programs, l, program) \
          { \
             if (program->name == pd->attributes.program.program) \
               continue; \
             ewe_combobox_item_add(combo, program->name); \
          } \
     } \
   elm_object_tooltip_text_set(combo, TOOLTIP); \
   elm_layout_content_set(item, NULL, combo); \
   button = elm_button_add(item); \
   ic = elm_icon_add(button); \
   elm_icon_standard_set(ic, "minus"); \
   elm_object_part_content_set(button, NULL, ic); \
   evas_object_smart_callback_add(button, "clicked", _del_##PARAM, item); \
   elm_layout_content_set(item, "swallow.button_del", button); \
   evas_object_smart_callback_add(combo, "selected", \
                                  _on_##PARAM##_change, pd); \
   elm_box_pack_end(pd->attributes.program.PARAM##_box, item); \
} \
static void \
prop_program_##PARAM##s_update(Group_Prop_Data *pd) \
{ \
   Evas_Object *combo, *item, *button; \
   Eina_List *items = elm_box_children_get(pd->attributes.program.PARAM##_box); \
   int i = 0; \
   Eina_List *l; \
   Eina_Stringshare *value; \
   Eina_List *list = edje_edit_program_##PARAM##s_get(pd->group->edit_object, \
                                                      pd->attributes.program.program); \
   list = eina_list_sort(list, eina_list_count(list), (Eina_Compare_Cb) strcmp); \
   int item_count = eina_list_count(items); \
   int list_count = eina_list_count(list); \
   if (item_count < list_count) \
     for (i = 0; i < list_count - item_count; i++) \
       _add_##PARAM(pd, NULL, NULL); \
   button = elm_layout_content_get(eina_list_data_get(items), "swallow.button_del"); \
   if ((list_count > 0) || (item_count > 1)) \
     elm_object_disabled_set(button, false); \
   else \
     elm_object_disabled_set(button, true); \
   /* fill up with part and program list */ \
   Eina_Stringshare *to_del; \
   EINA_LIST_FOREACH(items, l, item) \
     { \
        combo = elm_layout_content_get(item, NULL); \
        value = eina_list_data_get(list); \
        ewe_combobox_text_set(combo, value); \
        to_del = evas_object_data_get(combo, COMBOBOX_PREVIOUS); \
        eina_stringshare_del(to_del); \
        evas_object_data_del(combo, COMBOBOX_PREVIOUS); \
        evas_object_data_set(combo, COMBOBOX_PREVIOUS, eina_stringshare_add(value)); \
        if (!value) \
          ewe_combobox_text_set(combo, _("None")); \
        list = eina_list_next(list); \
     } \
} \
static Evas_Object * \
prop_program_##PARAM##_add(Evas_Object *parent, Group_Prop_Data *pd) \
{ \
   Evas_Object *item, *combo, *button, *ic; \
   Eina_List *l; \
   Resource *program; \
   Part *part; \
   LAYOUT_PROP_ADD(parent, NULL, "tab_home", "item") \
   evas_object_data_set(item, GROUP_PROP_DATA, pd); \
   button = elm_button_add(item); \
   ic = elm_icon_add(button); \
   elm_icon_standard_set(ic, "plus"); \
   elm_object_part_content_set(button, NULL, ic); \
   elm_layout_content_set(item, "swallow.button_add", button); \
   evas_object_smart_callback_add(button, "clicked", _add_##PARAM, pd); \
   EWE_COMBOBOX_ADD(item, combo); \
   ewe_combobox_text_set(combo, _("None")); \
   /* fill up with part and program list */ \
   EINA_LIST_FOREACH(pd->group->parts, l, part) \
     { \
        ewe_combobox_item_add(combo, part->name); \
     } \
   Edje_Action_Type type = edje_edit_program_action_get(pd->group->edit_object, \
                                                        pd->attributes.program.program); \
   if (type == EDJE_ACTION_TYPE_ACTION_STOP) \
     { \
        EINA_LIST_FOREACH(pd->group->programs, l, program) \
          { \
             if (program->name == pd->attributes.program.program) \
               continue; \
             ewe_combobox_item_add(combo, program->name); \
          } \
     } \
   elm_object_tooltip_text_set(combo, TOOLTIP); \
   elm_layout_content_set(item, NULL, combo); \
   button = elm_button_add(item); \
   ic = elm_icon_add(button); \
   elm_icon_standard_set(ic, "minus"); \
   elm_object_part_content_set(button, NULL, ic); \
   elm_layout_content_set(item, "swallow.button_del", button); \
   evas_object_smart_callback_add(button, "clicked", _del_##PARAM, item); \
   elm_object_disabled_set(button, true); \
   evas_object_smart_callback_add(combo, "selected", \
                                  _on_##PARAM##_change, pd); \
   return item; \
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
                                       Group_Prop_Data *pd, \
                                       const char *tooltip1, \
                                       const char *tooltip2 ) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   CHECK_ADD(item, pd->attributes.SUB.VALUE1) \
   elm_object_style_set(pd->attributes.SUB.VALUE1, "toggle"); \
   if (tooltip1) elm_object_tooltip_text_set(pd->attributes.SUB.VALUE1, tooltip1); \
   evas_object_smart_callback_add(pd->attributes.SUB.VALUE1, "changed", _on_part_drag_##VALUE1##_change, pd); \
   elm_layout_content_set(item, "swallow.content1", pd->attributes.SUB.VALUE1); \
   SPINNER_ADD(item, pd->attributes.SUB.VALUE2, 0.0, 9999.0, 1.0, true) \
   elm_spinner_label_format_set(pd->attributes.SUB.VALUE2, N_("%.0f")); \
   if (tooltip2) elm_object_tooltip_text_set(pd->attributes.SUB.VALUE2, tooltip2); \
   evas_object_smart_callback_add(pd->attributes.SUB.VALUE2, "spinner,drag,start", _on_part_drag_##VALUE2##_start, pd); \
   evas_object_smart_callback_add(pd->attributes.SUB.VALUE2, "spinner,drag,stop", _on_part_drag_##VALUE2##_stop, pd); \
   evas_object_smart_callback_add(pd->attributes.SUB.VALUE2, "changed", _on_part_drag_##VALUE2##_change, pd); \
   evas_object_event_callback_priority_add(pd->attributes.SUB.VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   elm_layout_text_set(item, "label.swallow2.start", _("step")); \
   elm_layout_text_set(item, "label.swallow2.end", _("px")); \
   elm_layout_content_set(item, "swallow.content2", pd->attributes.SUB.VALUE2); \
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
prop_part_drag_##VALUE1##_##VALUE2##_update(Group_Prop_Data *pd) \
{ \
   Eina_Bool ch_value; int st_value; \
   ch_value = edje_edit_part_drag_##VALUE1##_get(pd->group->edit_object, pd->part->name); \
   elm_check_state_set(pd->attributes.SUB.VALUE1, ch_value); \
   st_value = edje_edit_part_drag_##VALUE2##_get(pd->group->edit_object, pd->part->name); \
   elm_spinner_value_set(pd->attributes.SUB.VALUE2, st_value); \
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
#define PART_ATTR_DRAG_CALLBACK(SUB, VALUE1, VALUE2, DESCRIPTION1, DESCRIPTION2) \
static void \
_on_part_drag_##VALUE1##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION1, value?_("true"):_("false")); \
   Change *change = change_add(msg); \
   eina_stringshare_del(msg); \
   editor_part_drag_##VALUE1##_set(pd->group->edit_object, change, false, pd->part->name, value); \
   prop_part_drag_control_disable_set(pd, false); \
   history_change_add(pd->group->history, change); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
} \
static void \
_on_part_drag_##VALUE2##_start(void *data, \
                               Evas_Object *obj __UNUSED__, \
                               void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   assert(pd->change == NULL); \
   pd->change = change_add(NULL); \
   pd->old_int_val = edje_edit_##SUB##_##VALUE2##_get(pd->group->edit_object, pd->part->name); \
} \
static void \
_on_part_drag_##VALUE2##_stop(void *data, \
                              Evas_Object *obj __UNUSED__, \
                              void *ei __UNUSED__) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Eina_Stringshare *msg; \
   assert(pd->change != NULL); \
   int new_val = edje_edit_##SUB##_##VALUE2##_get(pd->group->edit_object, pd->part->name); \
   if (new_val != pd->old_int_val) \
     { \
        msg = eina_stringshare_printf(DESCRIPTION2, pd->old_int_val, new_val); \
        change_description_set(pd->change, msg); \
        eina_stringshare_del(msg); \
        history_change_add(pd->group->history, pd->change); \
     } \
   else \
     change_free(pd->change); \
   pd->change = NULL; \
} \
static void \
_on_part_drag_##VALUE2##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   int value = elm_spinner_value_get(obj); \
   if (pd->change) \
     { \
        if (!editor_##SUB##_##VALUE2##_set(pd->group->edit_object, pd->change, true, pd->part->name, value)) \
        { \
           ERR("editor_group_"#SUB"_"#VALUE2"_set failed"); \
           abort(); \
        } \
     } \
   else \
     { \
        _on_part_drag_##VALUE2##_start(data, obj, ei); \
        if (!editor_##SUB##_##VALUE2##_set(pd->group->edit_object, pd->change, true, pd->part->name, value)) \
        { \
           ERR("editor_"#SUB"_"#VALUE2"_set failed"); \
           abort(); \
        } \
        _on_part_drag_##VALUE2##_stop(data, obj, ei); \
     } \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_PARTS_LIST(SUB, VALUE, MEMBER) \
static void \
prop_##MEMBER##_##VALUE##_update(Group_Prop_Data *pd) \
{ \
   Part *part; \
   Eina_List *l; \
   Eina_Stringshare *value; \
   ewe_combobox_items_list_free(pd->attributes.MEMBER.VALUE, true); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object, pd->part->name); \
   if (value) ewe_combobox_text_set(pd->attributes.MEMBER.VALUE, value); \
   else ewe_combobox_text_set(pd->attributes.MEMBER.VALUE, _("None")); \
   ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, _("None")); \
   EINA_LIST_FOREACH(pd->group->parts, l, part) \
     { \
        if (part != pd->part) \
           ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, part->name); \
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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, PART_ITEM_ARGS, DESCRIPTION)

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
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @ingroup Property_Macro
 */
#define PART_ITEM_ATTR_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, DESCRIPTION) \
   COMMON_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, PART_ITEM_ARGS, DESCRIPTION)

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
 * @param MEMBER The combobox member from Group_Prop_Data structure
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
#define PART_ITEM_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, PART_ITEM_ARGS, DESCRIPTION)

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
#define STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
   COMMON_CHECK_CALLBACK(SUB, VALUE, MEMBER, STATE_ARGS, DESCRIPTION)

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
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
#define STATE_ATTR_1SPINNER_ADD(TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                                L_START, L_END, TOOLTIP, MULTIPLIER) \
COMMON_1SPINNER_ADD(STATE, TEXT, SUB, VALUE, MEMBER, MIN, MAX, STEP, FMT, \
                    L_START, L_END, TOOLTIP, MULTIPLIER)

/**
 * Macro defines a function that updates control by STATE_ATTR_1SPINNER_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE1 The first value of state attribute
 * @param MEMBER The spinner member from Group_Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE, MULTIPLIER) \
   COMMON_1SPINNER_UPDATE(SUB, VALUE, MEMBER, TYPE,  MULTIPLIER, STATE_ARGS)

/**
 * Macro defines a callback for STATE_ATTR_1(2)SPINNER_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 * @param TYPE The spinner value type: int, double
 * @param MULTIPLIER The multiplier to convert the value to percent
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, DESCRIPTION) \
   COMMON_SPINNER_CALLBACK(SUB, VALUE, MEMBER, TYPE, MULTIPLIER, STATE_ARGS, DESCRIPTION)

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
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
 * @param MEMBER The spinner member from Group_Prop_Data structure
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
#define STATE_ATTR_2CHECK_CALLBACK(SUB, VALUE1, VALUE2, MEMBER, DESCRIPTION1, DESCRIPTION2) \
   COMMON_CHECK_CALLBACK(SUB, VALUE1, MEMBER, STATE_ARGS, DESCRIPTION1) \
   COMMON_CHECK_CALLBACK(SUB, VALUE2, MEMBER, STATE_ARGS, DESCRIPTION2)

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
 * @param MEMBER The combobox member from Group_Prop_Data structure
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
 * @param MEMBER The combobox member from Group_Prop_Data structure
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
#define STATE_ATTR_1COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, DESCRIPTION) \
   COMMON_COMBOBOX_LIST_CALLBACK(TEXT, SUB, VALUE, TYPE, STATE_ARGS, DESCRIPTION)

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
   COMMON_COMBOBOX_LIST_STRSHARE_CALLBACK(SUB, VALUE, MEMBER, STATE_ARGS)

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
 * @param MEMBER The color member from Group_Prop_Data structure
 * @param TOOLTIP The tooltip for combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_COLOR_ADD(TEXT, SUB, VALUE, MEMBER, TOOLTIP) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Group_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow_subtext") \
   pd->attributes.MEMBER.VALUE = elm_layout_add(item); \
   elm_layout_theme_set(pd->attributes.MEMBER.VALUE, "layout", "property", "color"); \
   evas_object_size_hint_weight_set(pd->attributes.MEMBER.VALUE,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(pd->attributes.MEMBER.VALUE, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   if (TOOLTIP) elm_object_tooltip_text_set(pd->attributes.MEMBER.VALUE, TOOLTIP); \
   pd->attributes.MEMBER.VALUE##_obj = elm_layout_add(parent); \
   elm_layout_theme_set(pd->attributes.MEMBER.VALUE##_obj, "image", "color", "color_set"); \
   elm_layout_signal_callback_add(pd->attributes.MEMBER.VALUE, "clicked", "eflete", \
                                  _on_##MEMBER##_##VALUE##_clicked, pd); \
   elm_layout_content_set(pd->attributes.MEMBER.VALUE, NULL, pd->attributes.MEMBER.VALUE##_obj); \
   elm_layout_content_set(item, NULL, pd->attributes.MEMBER.VALUE); \
   prop_##SUB##_##VALUE##_update(pd); \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_COLOR_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The color member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_COLOR_LIST_UPDATE(SUB, VALUE, MEMBER) \
static void \
prop_##SUB##_##VALUE##_update(Group_Prop_Data *pd) \
{ \
   int r, g, b, a; \
   edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object, pd->part->name, \
                                   pd->part->current_state->parsed_name, pd->part->current_state->parsed_val, \
                                   &r, &g, &b, &a); \
   evas_object_color_set(pd->attributes.MEMBER.VALUE##_obj, r*a/255, g*a/255, b*a/255, a); \
}

/**
 * Macro defines a callback for STATE_ATTR_COLOR_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param MEMBER The color member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_COLOR_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *event_info __UNUSED__) \
{ \
   int r, g, b, a; \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   assert(pd->change != NULL); \
   colorselector_color_get(obj, &r, &g, &b, &a); \
   if (!editor_##SUB##_##VALUE##_set(pd->group->edit_object, pd->change, true, \
                                     pd->part->name, \
                                     pd->part->current_state->parsed_name, \
                                     pd->part->current_state->parsed_val, \
                                     r, g, b, a))\
     { \
       ERR("edje_edit_"#SUB"_"#VALUE"_set failed"); \
       abort(); \
     } \
   evas_object_color_set(pd->attributes.MEMBER.VALUE##_obj, r*a/255, g*a/255, b*a/255, a); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
} \
static void \
_on_##MEMBER##_##VALUE##_dismissed(void *data, \
                                   Evas_Object *obj, \
                                   void *event_info __UNUSED__) \
{ \
   int r, g, b, a; \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   assert(pd->change != NULL); \
   evas_object_smart_callback_del_full(obj, "color,changed", \
                                       _on_##MEMBER##_##VALUE##_change, pd); \
   evas_object_smart_callback_del_full(obj, "palette,item,selected", \
                                       _on_##MEMBER##_##VALUE##_change, pd); \
   evas_object_smart_callback_del_full(obj, "dismissed", \
                                       _on_##MEMBER##_##VALUE##_dismissed, pd); \
   edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object, pd->part->name, \
                                   pd->part->current_state->parsed_name, \
                                   pd->part->current_state->parsed_val, \
                                   &r, &g, &b, &a); \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION, r, g, b, a); \
   change_description_set(pd->change, msg); \
   eina_stringshare_del(msg); \
   history_change_add(pd->group->history, pd->change); \
   pd->change = NULL; \
   evas_object_hide(obj); \
} \
static void \
_on_##MEMBER##_##VALUE##_clicked(void *data, \
                                 Evas_Object *obj, \
                                 const char *emission __UNUSED__, \
                                 const char *source __UNUSED__) \
{ \
   int x, y; \
   int r, g, b, a; \
   Evas_Object *colorsel; \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   assert(pd->change == NULL); \
   colorsel = colorselector_get(); \
   pd->change = change_add(NULL); \
   evas_object_color_get(pd->attributes.MEMBER.VALUE##_obj, &r, &g, &b, &a); \
   colorselector_color_set(colorsel, r, g, b, a); \
   evas_object_smart_callback_add(colorsel, "color,changed", \
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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE, MEMBER, DESCRIPTION) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj __UNUSED__, \
                                void *ei) \
{ \
   Group_Prop_Data *pd = (Group_Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   Eina_Stringshare *old_val = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object, \
         pd->part->name, pd->part->current_state->parsed_name, pd->part->current_state->parsed_val); \
   if (((item->index != 0) && (item->title == old_val)) /*stringshares*/ || \
       ((item->index == 0) && (old_val == NULL))) \
     { \
       eina_stringshare_del(old_val); \
       return; \
     } \
   eina_stringshare_del(old_val); \
   Eina_Stringshare *msg = eina_stringshare_printf(DESCRIPTION, !strcmp(item->title, _("None")) ? NULL : item->title); \
   Change *change = change_add(msg); \
   eina_stringshare_del(msg); \
   editor_##SUB##_##VALUE##_set(pd->group->edit_object, change, false, pd->part->name, \
         pd->part->current_state->parsed_name, pd->part->current_state->parsed_val, \
         !strcmp(item->title, _("None")) ? NULL : item->title); \
   history_change_add(pd->group->history, change); \
   evas_object_smart_callback_call(ap.win, SIGNAL_PROPERTY_ATTRIBUTE_CHANGED, NULL); \
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
 * @param MEMEBER The combobox member from Group_Prop_Data structure
 * @paramram TOOLTIP1 The tooltip for first combobox
 * @paramram TOOLTIP2 The tooltip for second combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, LABEL1, LABEL2) \
   COMMON_ATTR_2COMBOBOX_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, TOOLTIP1, TOOLTIP2, LABEL1, LABEL2)

/*****************************************************************************/
/*                       STATE 1COMBOBOX SOURCE UPDATE                       */
/*****************************************************************************/
/**
 * Macro defines a function that updates any 'source' attribute control macro.
 *
 * @param SUB The prefix of main parameter
 * @param VALUE The 'source' value (ex: text_source)
 * @param MEMBER The entry member from Group_Prop_Data structure
 * @param PART_TYPE The part type that be excluded from dropdown list or
 *        only includeted to dropdown list, depending on the argument COMPARE
 * @param COMPARE The compare argument
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_SOURCE_UPDATE(SUB, VALUE, MEMBER, EXCLUDE_PART, COMPARE) \
static void \
prop_##MEMBER##_##VALUE##_update(Group_Prop_Data *pd) \
{ \
   Part *part; \
   Eina_List *l; \
   Eina_Stringshare *value; \
   ewe_combobox_items_list_free(pd->attributes.MEMBER.VALUE, true); \
   value = edje_edit_##SUB##_##VALUE##_get(pd->group->edit_object, \
                                           pd->part->name, \
                                           pd->part->current_state->parsed_name, \
                                           pd->part->current_state->parsed_val); \
   ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, _("None")); \
   ewe_combobox_text_set(pd->attributes.MEMBER.VALUE, value ? value : _("None")); \
   EINA_LIST_FOREACH(pd->group->parts, l, part) \
     { \
        if ((part != pd->part) && (part->type COMPARE EXCLUDE_PART)) \
          ewe_combobox_item_add(pd->attributes.MEMBER.VALUE, part->name); \
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
 * @see COMMON_ENTRY_ADD
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP) \
   COMMON_ENTRY_ADD(TEXT, SUB, VALUE, MEMBER, VALIDATOR, TOOLTIP)

/**
 * Macro defines a function that updates control by STATE_ATTR_1ENTRY_ADD macro.
 *
 * @see COMMON_ENTRY_UPDATE
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1ENTRY_UPDATE(SUB, VALUE, MEMBER) \
   COMMON_ENTRY_UPDATE(SUB, VALUE, MEMBER, STATE_ARGS) \

/**
 * Macro defines a callback for STATE_ATTR_1ENTRY_ADD.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, DESCRIPTION) \
   COMMON_ENTRY_CALLBACK(SUB, VALUE, VALIDATOR, STATE_ARGS, DESCRIPTION) \

/** @} privatesection */
