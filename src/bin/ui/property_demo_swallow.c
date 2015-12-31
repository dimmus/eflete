/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "history.h"
#include "validator.h"
#include "colorsel.h"
#include "project_manager.h"

#define DEMO_SWALLOW_PROP_DATA "image_prop_data"

#define DEMO_SWALLOW_PROP_DATA_GET() \
   assert(property != NULL); \
   Demo_Swallow_Prop_Data *pd = evas_object_data_get(property, DEMO_SWALLOW_PROP_DATA); \
   assert(pd != NULL);

struct _Demo_Swallow_Prop_Data
{
   Evas_Object *box;

   Evas_Object *scroller;
   Evas_Object *frame;
   Evas_Object *name;
   Evas_Object *swallow_content;

   Evas_Object *color;
   Evas_Object *color_obj;
   Evas_Object *picture;
   Evas_Object *widget;

   Evas_Object *max_w, *max_h;
   Evas_Object *min_w, *min_h;
   Evas_Object *label_weight;
   Evas_Object *weight_x, *weight_y;
   Evas_Object *label_align;
   Evas_Object *align_x, *align_y;

   Demo_Part *part;
};
typedef struct _Demo_Swallow_Prop_Data Demo_Swallow_Prop_Data;

static const char *swallow_content_type[] = { N_("None"),
                                              N_("Rectangle"),
                                              N_("Picture"),
                                              N_("Widget"),
                                              NULL};

static const char *widget_type[] = { N_("button"),
                                     N_("check"),
                                     N_("slider"),
                                     N_("entry"),
                                     N_("progressbar"),
                                     NULL};
static void
_on_spinner_mouse_wheel(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info)
{
   Evas_Event_Mouse_Wheel *mev = event_info;
   mev->event_flags |= EVAS_EVENT_FLAG_ON_HOLD;
}

#define PROPERTY_SWALLOW_COMMON_2SPINNER_CALLBACKS(MEMBER, VALUE) \
static void \
_on_##MEMBER##_##VALUE##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Demo_Swallow_Prop_Data *pd = (Demo_Swallow_Prop_Data *)data; \
   int value = elm_spinner_value_get(obj); \
   pd->part->MEMBER##_##VALUE = value; \
   evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, pd->part); \
}

#define PROPERTY_SWALLOW_COMMON_2SPINNER_ADD(TEXT, VALUE1, VALUE2, MEMBER, MIN, MAX, LABEL_END) \
PROPERTY_SWALLOW_COMMON_2SPINNER_CALLBACKS(MEMBER, VALUE1) \
PROPERTY_SWALLOW_COMMON_2SPINNER_CALLBACKS(MEMBER, VALUE2) \
static Evas_Object * \
prop_##MEMBER##_##VALUE1##_##VALUE2##_add(Evas_Object *parent, Demo_Swallow_Prop_Data *pd) \
{ \
   PROPERTY_ITEM_ADD(parent, TEXT, "2swallow") \
   SPINNER_ADD(item, pd->MEMBER##_##VALUE1, MIN, MAX, 1.0, true) \
   elm_layout_content_set(item, "swallow.content1", pd->MEMBER##_##VALUE1); \
   elm_layout_text_set(item, "label.swallow1.start", _(""#VALUE1":")); \
   elm_layout_text_set(item, "label.swallow1.end", LABEL_END); \
   evas_object_event_callback_priority_add(pd->MEMBER##_##VALUE1, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                          _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER##_##VALUE1, "changed", _on_##MEMBER##_##VALUE1##_change, pd); \
   SPINNER_ADD(item, pd->MEMBER##_##VALUE2, MIN, MAX, 1.0, true) \
   elm_layout_content_set(item, "swallow.content2", pd->MEMBER##_##VALUE2); \
   elm_layout_text_set(item, "label.swallow2.start", _(""#VALUE2":")); \
   elm_layout_text_set(item, "label.swallow2.end", LABEL_END); \
   evas_object_event_callback_priority_add(pd->MEMBER##_##VALUE2, EVAS_CALLBACK_MOUSE_WHEEL, \
                                           EVAS_CALLBACK_PRIORITY_BEFORE, \
                                           _on_spinner_mouse_wheel, NULL); \
   evas_object_smart_callback_add(pd->MEMBER##_##VALUE2, "changed", _on_##MEMBER##_##VALUE2##_change, pd); \
   return item; \
}

TODO("move from here, property_demo_text and property_group all common stuff")
PROPERTY_SWALLOW_COMMON_2SPINNER_ADD(_("min"), w, h, min, 0, 9999, _("px"));
PROPERTY_SWALLOW_COMMON_2SPINNER_ADD(_("max"), w, h, max, 0, 9999, _("px"));

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   assert(parent != NULL);
   assert(label != NULL);

   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

#define PROPERTY_SWALLOW_COMMON_COMBOBOX_ADD(TYPE, LIST, TEXT) \
static void \
_on_content_##TYPE##_set(void *data, \
                         Evas_Object *obj __UNUSED__, \
                         void *event_info) \
{ \
   Demo_Swallow_Prop_Data *pd __UNUSED__ = (Demo_Swallow_Prop_Data *)data; \
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)event_info; \
   pd->part->TYPE = item->index; \
   pd->part->change = true; \
   evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, pd->part); \
} \
static Evas_Object * \
prop_##TYPE##_add(Evas_Object *parent, Demo_Swallow_Prop_Data *pd) \
{ \
   int i; \
   PROPERTY_ITEM_ADD(parent, TEXT, "1swallow") \
   EWE_COMBOBOX_ADD(parent, pd->TYPE) \
   for (i = 0; LIST[i]; ewe_combobox_item_add(pd->TYPE, LIST[i]), i++) ; \
   evas_object_smart_callback_add(pd->TYPE, "selected", _on_content_##TYPE##_set, pd); \
   elm_layout_content_set(item, "elm.swallow.content", pd->TYPE); \
   ewe_combobox_select_item_set(pd->TYPE, 0); \
   return item; \
}
PROPERTY_SWALLOW_COMMON_COMBOBOX_ADD(swallow_content, swallow_content_type, _("Content"));
PROPERTY_SWALLOW_COMMON_COMBOBOX_ADD(widget, widget_type, _("Widget"));

static void
_on_rectangle_color_change(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   int r, g, b, a;
   Demo_Swallow_Prop_Data *pd = (Demo_Swallow_Prop_Data *)data;
   colorselector_color_get(obj, &r, &g, &b, &a);

   evas_color_argb_premul(a, &r, &g, &b);

   pd->part->r = r;
   pd->part->g = g;
   pd->part->b = b;
   pd->part->a = a;

   evas_object_color_set(pd->color_obj, r, g, b, a);
   if (pd->part->object)
     evas_object_color_set(pd->part->object, r, g, b, a);

   evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, pd->part);
}
static void
_on_rectangle_color_dismissed(void *data,
                              Evas_Object *obj,
                              void *event_info __UNUSED__)
{
   Demo_Swallow_Prop_Data *pd = (Demo_Swallow_Prop_Data *)data;
   evas_object_smart_callback_del_full(obj, "color,changed",
                                       _on_rectangle_color_change, pd);
   evas_object_smart_callback_del_full(obj, "palette,item,selected",
                                       _on_rectangle_color_change, pd);
   evas_object_smart_callback_del_full(obj, "dismissed",
                                       _on_rectangle_color_dismissed, pd);
   evas_object_hide(obj);
}
static void
_on_color_clicked(void *data,
                  Evas_Object *obj,
                  const char *emission __UNUSED__,
                  const char *source __UNUSED__)
{
   int x, y;
   int r, g, b, a;
   Evas_Object *colorsel;
   Demo_Swallow_Prop_Data *pd = (Demo_Swallow_Prop_Data *)data;
   colorsel = colorselector_get();
   evas_object_color_get(pd->color_obj, &r, &g, &b, &a);
   colorselector_color_set(colorsel, r, g, b, a);
   evas_object_smart_callback_add(colorsel, "color,changed",
                                  _on_rectangle_color_change, pd);
   evas_object_smart_callback_add(colorsel, "dismissed",
                                  _on_rectangle_color_dismissed, pd);
   evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y);
   evas_object_move(colorsel, x, y);
   evas_object_show(colorsel);
}

static Evas_Object *
prop_rectangle_color_add(Evas_Object *parent,
                         Demo_Swallow_Prop_Data *pd)
{
   PROPERTY_ITEM_ADD(parent, _("Rectangle Color"), "1swallow_subtext")
   pd->color = elm_layout_add(item);
   elm_layout_theme_set(pd->color, "layout", "property", "color");
   evas_object_size_hint_weight_set(pd->color,  EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pd->color, EVAS_HINT_FILL, EVAS_HINT_FILL);
   pd->color_obj = elm_layout_add(parent);
   elm_layout_theme_set(pd->color_obj, "image", "color", "color_set");
   elm_layout_signal_callback_add(pd->color, "clicked", "eflete",
                                  _on_color_clicked, pd);
   elm_layout_content_set(pd->color, NULL, pd->color_obj);
   elm_layout_content_set(item, NULL, pd->color);
   return item;
}

static Eina_Bool
_on_image_done(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   const char *value;
   const char *selected;
   Eina_List *list_selected = (Eina_List *)event_info;
   Demo_Swallow_Prop_Data *pd = (Demo_Swallow_Prop_Data *)data;

   assert(pd != NULL);

   if (!list_selected) return false;

   value = elm_entry_entry_get(pd->picture);
   selected = eina_list_data_get(list_selected);
   if (strcmp(value, selected) == 0) return true;

   elm_entry_entry_set(pd->picture, selected);

   eina_stringshare_del(pd->part->image_path);
   pd->part->image_path = eina_stringshare_add(selected);

   evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, pd->part);

   return true;
}

static void
_image_selector(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   popup_fileselector_image_helper(NULL,
                                   obj,
                                   NULL,
                                   _on_image_done,
                                   data,
                                   false,
                                   false);
}

static Evas_Object *
prop_image_path_add(Evas_Object *parent,
                    Demo_Swallow_Prop_Data *pd)
{
   Evas_Object *btn;
   PROPERTY_ITEM_ADD(parent, "Picture:", "1swallow")
   ENTRY_ADD(item, pd->picture, true)
   btn = elm_button_add(parent);
   elm_object_style_set(btn, "elipsis");
   evas_object_smart_callback_add(btn, "clicked", _image_selector, pd);
   evas_object_smart_callback_add(pd->picture, "clicked", _image_selector, pd);
   elm_object_part_content_set(pd->picture, "elm.swallow.elipsis", btn);
   elm_entry_editable_set(pd->picture, false);
   evas_object_show(btn);
   elm_layout_content_set(item, NULL, pd->picture);
   return item;
}

void
ui_property_demo_swallow_part_set(Evas_Object *property, Demo_Part *part)
{
   DEMO_SWALLOW_PROP_DATA_GET()

   /* setting */
   if (part)
     {
        elm_object_text_set(pd->name, part->name);
        evas_object_color_set(pd->color_obj,
                              part->r,
                              part->g,
                              part->b,
                              part->a);
        elm_entry_entry_set(pd->picture, part->image_path);

        elm_spinner_value_set(pd->min_w, part->min_w);
        elm_spinner_value_set(pd->min_h, part->min_h);
        elm_spinner_value_set(pd->max_w, part->max_w);
        elm_spinner_value_set(pd->max_h, part->max_h);

        ewe_combobox_select_item_set(pd->swallow_content, part->swallow_content);
        ewe_combobox_select_item_set(pd->widget, part->widget);
     }

   pd->part = part;

   /* showing */
   evas_object_show(property);
}

Evas_Object *
ui_property_demo_swallow_add(Evas_Object *parent)
{
   Demo_Swallow_Prop_Data *pd;
   Evas_Object *item, *scroller, *box;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Demo_Swallow_Prop_Data));

   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);
   pd->scroller = scroller;
   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   evas_object_data_set(pd->scroller, DEMO_SWALLOW_PROP_DATA, pd);
   /* editors */
   FRAME_ADD(box, pd->frame, true, _("Demo Swallow Property"))
   elm_box_pack_end(box, pd->frame);
   BOX_ADD(parent, pd->box, false, false);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);

   /* Frame with info */
   elm_box_align_set(pd->box, 0.5, 0.0);
   item = prop_item_label_add(pd->box, &pd->name, _("name"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_swallow_content_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);
   item = prop_image_path_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);
   item = prop_widget_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);

   item = prop_rectangle_color_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);

   item = prop_min_w_h_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);
   item = prop_max_w_h_add(pd->box, pd);
   elm_box_pack_end(pd->box, item);

   elm_object_content_set(pd->frame, pd->box);

   return pd->scroller;
}
