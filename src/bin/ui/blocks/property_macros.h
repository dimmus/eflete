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

#define PROPERTY_ITEM_ADD(PARENT, NAME, STYLE) \
   Evas_Object *item; \
   item = elm_layout_add(PARENT); \
   elm_layout_theme_set(item, "layout", "property", STYLE); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_object_part_text_set(item, "elm.text", NAME); \
   evas_object_show(item);

/*****************************************************************************/
/*                         PART 1 CHECK CONTROL                              */
/*****************************************************************************/
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

#define PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_##SUB##_##VALUE##_add(Evas_Object *parent, \
                           Prop_Data *pd, \
                           const char *tooltip) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(item, pd->SUB.VALUE) \
   prop_part_##VALUE##_update(pd, pd->SUB.VALUE); \
   elm_object_tooltip_text_set(pd->SUB.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "selected", _on_##SUB##_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->SUB.VALUE); \
   return item; \
}

#define PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE) \
static void \
_on_##SUB##_##VALUE##_change(void *data, \
                          Evas_Object *obj __UNUSED__, \
                          void *ei) \
{ \
   Prop_Data *pd = (Prop_Data *)data; \
   Ewe_Combobox_Item *item = ei; \
   const char *old_value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, \
                                                        pd->wm_part->name);\
   const char *value = NULL; \
   if (item->index != 0) \
     { \
        value = item->title; \
        if (!edje_edit_part_##VALUE##_set(pd->wm_style->obj, \
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
        edje_edit_part_##VALUE##_set(pd->wm_style->obj, pd->wm_part->name, NULL); \
        pd->part.previous_source = 0; \
     } \
   history_diff_add(pd->wm_style->obj, PROPERTY, MODIFY, STRING, old_value, \
                    value, pd->wm_style->full_group_name,\
                    (void*)edje_edit_part_##VALUE##_set,  #SUB"_"#VALUE, \
                    pd->wm_part->name, NULL, 0.0); \
   project_changed(); \
   workspace_edit_object_recalc(pd->workspace); \
   pd->wm_style->isModify = true; \
}

/*****************************************************************************/
/*                     PART 1 COMBOBOX LIST CONTROL                          */
/*****************************************************************************/

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

#define PART_ATTR_1COMBOBOX_LIST_UPDATE(SUB, VALUE, TYPE) \
static void \
prop_##SUB##_##VALUE##_update(Prop_Data *pd) \
{ \
   TYPE value = edje_edit_part_##VALUE##_get(pd->wm_style->obj, pd->wm_part->name); \
   ewe_combobox_select_item_set(pd->SUB.VALUE, value); \
}

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
