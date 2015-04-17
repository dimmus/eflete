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
   elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
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
 * Macro defines functions that create an item with label and 1 combobox for part
 * attribute. A predefined list fill the combobox.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                        Prop_Data *pd, \
                        const char *tooltip, \
                        const char **list) \
{ \
   int i; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(parent, pd->SUB.VALUE) \
   for (i = 0; list[i]; ewe_combobox_item_add(pd->SUB.VALUE, list[i]), i++) ; \
   elm_object_tooltip_text_set(pd->SUB.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->SUB.VALUE); \
   PART_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE) \
   return item; \
}

/**
 * Macro defines a function that updates control by PART_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 * @param TYPE The type of given attribute
 *
 * @ingroup Property_Macro
 */
#define PART_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE) \
   ewe_combobox_select_item_set(pd->SUB.VALUE, \
                                edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name)); \

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
   int value = elm_spinner_value_get(obj); \
   int old_value = edje_edit_part_drag_##VALUE2##_get(pd->wm_style->obj, \
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
/*                           STATE 1 CHECK CONTROL                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 1 check for
 * state attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE The value of state attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1CHECK_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Prop_Data *pd, \
                           const char *tooltip) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   CHECK_ADD(item, pd->SUB.VALUE) \
   elm_object_style_set(pd->SUB.VALUE, "toggle"); \
   elm_object_tooltip_text_set(pd->SUB.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "changed", _on_##SUB##_##VALUE##_change, pd); \
   STATE_ATTR_CHECK_UPDATE(SUB, VALUE) \
   elm_layout_content_set(item, NULL, pd->SUB.VALUE); \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_1CHECK_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE The first value of state attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_CHECK_UPDATE(SUB, VALUE) \
   elm_check_state_set(pd->SUB.VALUE, edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                                                      pd->wm_part->name, \
                                                                      pd->wm_part->curr_state, \
                                                                      pd->wm_part->curr_state_value));

/**
 * Macro defines a callback for STATE_ATTR_1CHEACK_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE The value of state attribute.
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj, \
                             void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Eina_Bool value = elm_check_state_get(obj); \
   Eina_Bool old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                             pd->wm_part->name, pd->wm_part->curr_state, \
                             pd->wm_part->curr_state_value); \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                        pd->wm_part->curr_state, \
                                        pd->wm_part->curr_state_value, \
                                        value)) \
     return; \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

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
   elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
   evas_object_event_callback_priority_add(pd->MEMBER.VALUE, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE, "changed", _on_##MEMBER##_##VALUE##_change, pd); \
   STATE_ATTR_1SPINNER_UPDATE(SUB, VALUE, MEMBER, MULTIPLIER); \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_1SPINNER_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE The first value of state attribute
 * @param MEMBER The spinner member from Prop_Data structure
 * @param MULTIPLIER The multiplier to convert the value to percent. If it not
 *        needed set 1
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1SPINNER_UPDATE(SUB, VALUE, MEMBER, MULTIPLIER) \
   elm_spinner_value_set(pd->MEMBER.VALUE, \
                         MULTIPLIER * edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                                                      pd->wm_part->name, \
                                                                      pd->wm_part->curr_state, \
                                                                      pd->wm_part->curr_state_value)); \

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
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   TYPE value = elm_spinner_value_get(obj); \
   TYPE old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name,\
                                        pd->wm_part->curr_state, \
                                        pd->wm_part->curr_state_value); \
   value /= MULTIPLIER; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name,\
                                        pd->wm_part->curr_state, \
                                        pd->wm_part->curr_state_value, \
                                        value)) \
     return; \
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
#define STATE_ATTR_2SPINNER_ADD(TEXT, SUB, VALUE1, VALUE2, MEMBER, \
                                MIN, MAX, STEP, FMT, \
                                L1_START, L1_END, L2_START, L2_END, \
                                TOOLTIP1, TOOLTIP2, MULTIPLIER) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, \
                                          Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   SPINNER_ADD(item, pd->MEMBER.VALUE1, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->MEMBER.VALUE1, FMT); \
   elm_layout_content_set(item, "swallow.content1", pd->MEMBER.VALUE1); \
   elm_layout_text_set(item, "label.swallow1.start", L1_START); \
   elm_layout_text_set(item, "label.swallow1.end", L1_END); \
   elm_object_tooltip_text_set(pd->MEMBER.VALUE1, TOOLTIP1); \
   evas_object_event_callback_priority_add(pd->MEMBER.VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE1, "changed", _on_##MEMBER##_##VALUE1##_change, pd); \
   SPINNER_ADD(item, pd->MEMBER.VALUE2, MIN, MAX, STEP, true) \
   elm_spinner_label_format_set(pd->MEMBER.VALUE2, FMT); \
   elm_layout_content_set(item, "swallow.content2", pd->MEMBER.VALUE2); \
   elm_layout_text_set(item, "label.swallow2.start", L2_START); \
   elm_layout_text_set(item, "label.swallow2.end", L2_END); \
   elm_object_tooltip_text_set(pd->MEMBER.VALUE2, TOOLTIP2); \
   evas_object_event_callback_priority_add(pd->MEMBER.VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER.VALUE2, "changed", _on_##MEMBER##_##VALUE2##_change, pd); \
   STATE_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, MULTIPLIER); \
   return item; \
}

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
#define STATE_ATTR_2SPINNER_UPDATE(SUB, VALUE1, VALUE2, MEMBER, MULTIPLIER) \
   STATE_ATTR_1SPINNER_UPDATE(SUB, VALUE1, MEMBER, MULTIPLIER) \
   STATE_ATTR_1SPINNER_UPDATE(SUB, VALUE2, MEMBER, MULTIPLIER)

/*****************************************************************************/
/*                          STATE 2 CHECK CONTROLS                           */
/*****************************************************************************/
/**
 * Macro defines a functions that create an item with label and 2 check for
 * state attribute.
 *
 * @param TEXT The label text
 * @param SUB The prefix of main parameter of state attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 * @param TOOLTIP1 The first spinner tooltip
 * @param TOOLTIP2 The second spinner tooltip
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2CHECK_ADD(TEXT, SUB, VALUE1, VALUE2, TOOLTIP1, TOOLTIP2) \
static Evas_Object * \
prop_##SUB##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   CHECK_ADD(item, pd->SUB.VALUE1) \
   elm_object_style_set(pd->SUB.VALUE1, "toggle"); \
   elm_object_tooltip_text_set(pd->SUB.VALUE1, TOOLTIP1); \
   evas_object_smart_callback_add(pd->SUB.VALUE1, "changed", _on_##SUB##_##VALUE1##_change, pd); \
   elm_layout_content_set(item, "swallow.content1", pd->SUB.VALUE1); \
   CHECK_ADD(item, pd->SUB.VALUE2) \
   elm_object_style_set(pd->SUB.VALUE2, "toggle"); \
   elm_object_tooltip_text_set(pd->SUB.VALUE2, TOOLTIP2); \
   evas_object_smart_callback_add(pd->SUB.VALUE2, "changed", _on_##SUB##_##VALUE2##_change, pd); \
   elm_layout_content_set(item, "swallow.content2", pd->SUB.VALUE2); \
   STATE_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2) \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_2CHECK_ADD macro.
 *
 * @param SUB The prefix of main parameter of drag attribute
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2CHECK_UPDATE(SUB, VALUE1, VALUE2) \
   STATE_ATTR_CHECK_UPDATE(SUB, VALUE1) \
   STATE_ATTR_CHECK_UPDATE(SUB, VALUE2)

/**
 * Macro defines a callback for STATE_ATTR_2CHECK_ADD.
 *
 * @param SUB The prefix of main parameter of state attribute;
 * @param VALUE1 The first value of state attribute
 * @param VALUE2 The second value of state attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_2CHECK_CALLBACK(SUB, VALUE1, VALUE2) \
   STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE1) \
   STATE_ATTR_1CHECK_CALLBACK(SUB, VALUE2)

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
 * @param LIST The pointer to array of strings
 * @param TOOLTIP The tooltip for combobox
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_LIST_ADD(TEXT, SUB, VALUE, LIST, TOOLTIP) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Prop_Data *pd) \
{ \
   int i; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(parent, pd->SUB.VALUE) \
   for (i = 0; LIST[i]; ewe_combobox_item_add(pd->SUB.VALUE, LIST[i]), i++) ; \
   elm_object_tooltip_text_set(pd->SUB.VALUE, TOOLTIP); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->SUB.VALUE); \
   STATE_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE) \
   return item; \
}

/**
 * Macro defines a function that updates control by STATE_ATTR_1COMBOBOX_LIST_ADD macro.
 *
 * @param SUB The prefix of main parameter of part attribute
 * @param VALUE The value of part attribute
 *
 * @ingroup Property_Macro
 */
#define STATE_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE) \
   ewe_combobox_select_item_set(pd->SUB.VALUE, \
                                edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, \
                                                                pd->wm_part->name, \
                                                                pd->wm_part->curr_state, \
                                                                pd->wm_part->curr_state_value));

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
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                             Evas_Object *obj __UNUSED__, \
                             void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   int old_value = edje_edit_##SUB##_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name, \
                                                   pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   int value = item->index; \
   if (!edje_edit_##SUB##_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, \
                                        pd->wm_part->curr_state, pd->wm_part->curr_state_value, \
                                        (TYPE)item->index)) \
     { \
        NOTIFY_WARNING(_("Cann't apply value '%s' to attribute "#TEXT". Restore previous value"), item->title); \
        ewe_combobox_select_item_set(obj, old_value); \
        return; \
     } \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, VAL_INT, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_##SUB##_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, pd->wm_part->curr_state, pd->wm_part->curr_state_value); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

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
   Evas_Object *layout; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   layout = elm_layout_add(item); \
   elm_layout_theme_set(layout, "layout", "property", "color"); \
   evas_object_size_hint_weight_set(layout,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_object_tooltip_text_set(layout, TOOLTIP); \
   pd->MEMBER.VALUE##_obj = elm_layout_add(parent); \
   elm_layout_theme_set(pd->MEMBER.VALUE##_obj, "image", "color", "color_set"); \
   evas_object_event_callback_add(layout, EVAS_CALLBACK_MOUSE_DOWN, \
                                  _on_##MEMBER##_##VALUE##_clicked, pd); \
   elm_layout_content_set(layout, NULL, pd->MEMBER.VALUE##_obj); \
   elm_layout_content_set(item, NULL, layout); \
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
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
   edje_edit_string_free(old_value); \
}

/*****************************************************************************/
/*                          STATE 1 ENTRY CONTROL                            */
/****************************************************************************/
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
   elm_object_tooltip_text_set(pd->MEMBER.VALUE, TOOLTIP); \
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
   ewe_entry_entry_set(pd->MEMBER.VALUE, value); \
   edje_edit_string_free(value); \
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
   const char *value = elm_entry_entry_get(obj); \
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
}

/** @} privatesection */
