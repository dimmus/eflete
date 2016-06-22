/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "main_window.h"
#include "project_manager.h"
#include "property.h"
#include "property_private.h"

typedef struct {
   Property_Attribute item_description;

   Property_Attribute item_object_title;
   Property_Attribute item_object_color;
   Property_Attribute item_outline_title;
   Property_Attribute item_outline_color;
   Property_Attribute item_shadow_title;
   Property_Attribute item_shadow_color;

   ColorClassData *selected;
} Property_Color_Data;

static Property_Color_Data color_data;

static void
_colorclass_update(ColorClassData *selected)
{
   edje_object_color_class_set(selected->edje_preview,
                               "colorclass_manager/text_example_colorclass",
                               selected->current_ccl->r1, selected->current_ccl->g1,
                               selected->current_ccl->b1, selected->current_ccl->a1,
                               selected->current_ccl->r2, selected->current_ccl->g2,
                               selected->current_ccl->b2, selected->current_ccl->a2,
                               selected->current_ccl->r3, selected->current_ccl->g3,
                               selected->current_ccl->b3, selected->current_ccl->a3);
   edje_edit_color_class_colors_set(ap.project->global_object, selected->current_ccl->name,
                               selected->current_ccl->r1, selected->current_ccl->g1,
                               selected->current_ccl->b1, selected->current_ccl->a1,
                               selected->current_ccl->r2, selected->current_ccl->g2,
                               selected->current_ccl->b2, selected->current_ccl->a2,
                               selected->current_ccl->r3, selected->current_ccl->g3,
                               selected->current_ccl->b3, selected->current_ccl->a3);

   CRIT_ON_FAIL(editor_save(ap.project->global_object));
   ap.project->changed = true;
}

/* Colorselector widget callbacks */
#define COLORSELECTOR_CALLBACK(NUMBER) \
static Eina_Bool \
_update_##NUMBER##_cb(Property_Attribute *pa __UNUSED__, Property_Action *action) \
{ \
   if (color_data.selected) \
     {\
        Colorclass_Item *cc_it = color_data.selected->current_ccl; \
        elm_colorselector_color_set(action->control, \
                                    cc_it->r##NUMBER, \
                                    cc_it->g##NUMBER, \
                                    cc_it->b##NUMBER, \
                                    cc_it->a##NUMBER); \
     }\
   return true; \
} \
static void \
_on_changed_##NUMBER(Property_Attribute *pa __UNUSED__, Property_Action *action) \
{ \
   Colorclass_Item *cc_it = color_data.selected->current_ccl; \
   elm_colorselector_color_get(action->control, \
                               &cc_it->r##NUMBER, \
                               &cc_it->g##NUMBER, \
                               &cc_it->b##NUMBER, \
                               &cc_it->a##NUMBER); \
   _colorclass_update(color_data.selected); \
}

COLORSELECTOR_CALLBACK(1)
COLORSELECTOR_CALLBACK(2)
COLORSELECTOR_CALLBACK(3)

#undef COLORSELECTOR_CALLBACK

static void
_change_cb(Property_Attribute *pa __UNUSED__, Property_Action *action)
{
   Eina_Stringshare *text;

   text = property_entry_get(action->control);

   Colorclass_Item *cc_it = color_data.selected->current_ccl;
   edje_edit_color_class_description_set(ap.project->global_object, cc_it->name, text);

   CRIT_ON_FAIL(editor_save(ap.project->global_object));
   ap.project->changed = true;
   eina_stringshare_del(text);
}

static Eina_Bool
_update_cb(Property_Attribute *pa __UNUSED__, Property_Action *action)
{
   Eina_Stringshare *description;
   if (color_data.selected)
     {
        description = edje_edit_color_class_description_get(ap.project->global_object,
                                                            color_data.selected->current_ccl->name);
        TODO("recheck this case");
        if (action->control)
          {
             property_entry_set(action->control, description);
             elm_object_disabled_set(action->control, false);
          }
        edje_edit_string_free(description);
     }
   return true;
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action __UNUSED__)
{
   if (!color_data.selected)
     elm_object_item_disabled_set(pa->glit, true);
}

static void
_color_class_selected(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Property_Data *pd = data;
   ColorClassData *selected = (ColorClassData *)event_info;

   if (!selected)
     {
        color_data.selected = NULL;
     }
   else
     {
        color_data.selected = selected;

        elm_object_item_disabled_set(color_data.item_object_color.glit, false);
        elm_object_item_disabled_set(color_data.item_outline_color.glit, false);
        elm_object_item_disabled_set(color_data.item_shadow_color.glit, false);

        _update_1_cb(NULL, &color_data.item_object_color.action1);
        _update_2_cb(NULL, &color_data.item_outline_color.action1);
        _update_3_cb(NULL, &color_data.item_shadow_color.action1);

        _update_cb(NULL, &color_data.item_description.action1);
     }
   GENLIST_FILTER_APPLY(pd->genlist);
}

static Eina_Bool
_filter_cb(Property_Attribute *pa __UNUSED__)
{
   return color_data.selected != NULL;
}

void
property_color_class_manager_init(Property_Data *pd)
{
   color_data.item_description.name = "Description";
   color_data.item_description.filter_cb = _filter_cb;
   color_data.item_description.action1.control_type = PROPERTY_CONTROL_ENTRY;
   color_data.item_description.action1.init_cb = _init_cb;
   color_data.item_description.action1.change_cb = _change_cb;
   color_data.item_description.action1.update_cb = _update_cb;
   color_data.item_description.action1.tooltip = eina_stringshare_add(_("Provides a descriptive name for the effect of the color class"));

   color_data.item_object_title.name = "Object color";
   color_data.item_object_title.action1.control_type = PROPERTY_CONTROL_LABEL;
   color_data.item_object_title.filter_cb = _filter_cb;

   color_data.item_object_color.filter_cb = _filter_cb;
   color_data.item_object_color.action1.control_type = PROPERTY_CONTROL_COLORSEL;
   color_data.item_object_color.action1.init_cb = _init_cb;
   color_data.item_object_color.action1.change_cb = _on_changed_1;
   color_data.item_object_color.action1.update_cb = _update_1_cb;

   color_data.item_outline_title.name = "Outline color";
   color_data.item_outline_title.action1.control_type = PROPERTY_CONTROL_LABEL;
   color_data.item_outline_title.filter_cb = _filter_cb;

   color_data.item_outline_color.filter_cb = _filter_cb;
   color_data.item_outline_color.action1.control_type = PROPERTY_CONTROL_COLORSEL;
   color_data.item_outline_color.action1.init_cb = _init_cb;
   color_data.item_outline_color.action1.change_cb = _on_changed_2;
   color_data.item_outline_color.action1.update_cb = _update_2_cb;

   color_data.item_shadow_title.name = "Shadow color";
   color_data.item_shadow_title.action1.control_type = PROPERTY_CONTROL_LABEL;
   color_data.item_shadow_title.filter_cb = _filter_cb;

   color_data.item_shadow_color.filter_cb = _filter_cb;
   color_data.item_shadow_color.action1.control_type = PROPERTY_CONTROL_COLORSEL;
   color_data.item_shadow_color.action1.init_cb = _init_cb;
   color_data.item_shadow_color.action1.change_cb = _on_changed_3;
   color_data.item_shadow_color.action1.update_cb = _update_3_cb;

   evas_object_smart_callback_add(ap.win, SIGNAL_COLOR_SELECTED, _color_class_selected, pd);
}

Eina_List *
property_color_class_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &color_data.item_description);
   items = eina_list_append(items, &color_data.item_object_title);
   items = eina_list_append(items, &color_data.item_object_color);
   items = eina_list_append(items, &color_data.item_outline_title);
   items = eina_list_append(items, &color_data.item_outline_color);
   items = eina_list_append(items, &color_data.item_shadow_title);
   items = eina_list_append(items, &color_data.item_shadow_color);

   return items;
}
