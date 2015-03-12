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

#define PART_ATTR_1COMBOBOX_ADD(TEXT, SUB, VALUE) \
static Evas_Object * \
prop_part_##VALUE##_add(Evas_Object *parent, \
                        Prop_Data *pd, \
                        const char *tooltip) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(item, pd->SUB.VALUE) \
   prop_part_##VALUE##_update(pd); \
   elm_object_tooltip_text_set(pd->SUB.VALUE, tooltip); \
   evas_object_smart_callback_add(pd->SUB.VALUE, "selected", _on_part_##VALUE##_change, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->SUB.VALUE); \
   return item; \
}

#define PART_ATTR_1COMBOBOX_CALLBACK(SUB, VALUE) \
static void \
_on_part_##VALUE##_change(void *data, \
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
