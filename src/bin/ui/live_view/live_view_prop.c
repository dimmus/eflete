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

#include "live_view_prop.h"

#define PROP_DATA "prop_data"
#define PROP_DATA_GET(ret) \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   if (!pd) \
     { \
        ERR("Can not show group data, because object[%p] is not a property object", property); \
        return ret; \
     }

#define FRAME_LIVE_VIEW_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);

/* TODO: remove this hack after scroller would be fixed
 * Hack start
 */
static void
_on_frame_click(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Evas_Object *scroller = (Evas_Object *)data;
   Evas_Object *box, *frame_box;
   int h_box, h_frame_box, h_scr, y_reg, h_reg, y_frame;
   box = elm_object_content_get(scroller);
   evas_object_geometry_get(scroller, NULL, NULL, NULL, &h_scr);
   evas_object_geometry_get(box, NULL, NULL, NULL, &h_box);
   frame_box = elm_object_content_get(obj);
   evas_object_geometry_get(frame_box, NULL, &y_frame, NULL, &h_frame_box);
   elm_scroller_region_get(scroller, NULL, &y_reg, NULL, &h_reg);
   elm_scroller_region_bring_in(scroller, 0.0, y_reg + 1, 0.0, h_reg);
   if (!elm_frame_collapse_get(obj))
     {
        if (h_box == h_scr + y_reg)
          elm_scroller_region_show(scroller, 0.0, y_reg + h_frame_box, 0.0, h_reg);
        else
          elm_scroller_region_bring_in(scroller, 0.0, y_reg + 1, 0.0, h_reg);
     }
}
/* Hack end */

static void
_on_scale_change(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Prop_Data *pd = (Prop_Data *)data;
   pd->current_scale = elm_spinner_value_get(obj) / 100;
   if (pd->live_object)
     {
        if (!strcmp("edje", pd->widget))
          edje_object_scale_set(pd->live_object, pd->current_scale);
        else
          elm_object_scale_set(pd->live_object, pd->current_scale);
     }
}

static void
_on_all_swallow_check(void *data,
                      Evas_Object *obj,
                      void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *item = NULL;
   Eina_List *part_list = NULL, *part = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   part_list = elm_box_children_get(pd->prop_swallow.swallows);
   if (!part_list) return;

   EINA_LIST_FOREACH(part_list, part, item)
     {
        check = elm_object_part_content_get(item, "info");
        elm_check_state_set(check, elm_check_state_get(obj));
        evas_object_smart_callback_call(check, "changed", NULL);
     }

   eina_list_free(part_list);
}

static void
_on_all_text_check(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *check = NULL, *item = NULL;
   Eina_List *part_list = NULL, *part = NULL;

   Prop_Data *pd = (Prop_Data *)data;
   part_list = elm_box_children_get(pd->prop_text.texts);
   if (!part_list) return;

   EINA_LIST_FOREACH(part_list, part, item)
     {
        check = elm_object_part_content_get(item, "info");
        elm_check_state_set(check, elm_check_state_get(obj));
        evas_object_smart_callback_call(check, "changed", NULL);
     }

   eina_list_free(part_list);
}

Eina_Bool
live_view_property_style_set(Evas_Object *property,
                             Evas_Object *object,
                             Style *style,
                             const char *widget)
{
   Evas_Object *prop_box, *spinner, *check, *button;
   Evas_Object *item;
   const char *part_name;
   Eina_List *part_list = NULL, *part = NULL, *l = NULL;
   Signal *sig = NULL;
   Edje_Part_Type part_type;
   Eina_Bool swallow_parts_exists = false, text_parts_exists = false;
   Eina_Bool signals_exists = false;

   if ((!property) || (!object) || (!style) || (!widget) || (!style->obj))
     return false;
   PROP_DATA_GET(false)
   pd->style = style;

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_ON);

   pd->widget = widget;
   pd->live_object = object;

   prop_box = elm_object_content_get(pd->visual);

   if (!pd->scale_spinner)
     {
        item = elm_layout_add(prop_box);
        evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0);
        elm_layout_file_set(item, EFLETE_EDJ, "eflete/prop/container/live_view_spinner");
        elm_object_part_text_set(item, "eflete.text.start", "Scale: ");
        SPINNER_ADD(item, spinner, 1, 500, 1, true);
        elm_object_style_set(spinner, "live_view");
        elm_spinner_label_format_set(spinner, "%3.0f%%");
        evas_object_smart_callback_add(spinner, "changed", _on_scale_change, pd);
        pd->scale_spinner = item;
        elm_object_part_content_set(item, "eflete.content", spinner);
        evas_object_data_set(item, ITEM, spinner);
     }
   spinner = evas_object_data_get(pd->scale_spinner, ITEM);
   elm_spinner_value_set(spinner, 100);
   pd->current_scale = 1.0;
   evas_object_scale_set(pd->live_object, pd->current_scale);
   elm_box_pack_start(prop_box, pd->scale_spinner);
   evas_object_show(pd->scale_spinner);

   /* Swallows UI setting*/
   if (!pd->prop_swallow.swallows)
     {
        FRAME_LIVE_VIEW_ADD(property, pd->prop_swallow.frame, true, _("Swallows"), pd->visual);
        elm_object_style_set(pd->prop_swallow.frame, "live_view");

        CHECK_ADD(prop_box, check);
        elm_object_style_set(check, "live_view");
        elm_object_part_content_set(pd->prop_swallow.frame, "elm.swallow.check", check);

        BOX_ADD(pd->prop_swallow.frame, pd->prop_swallow.swallows, false, false)
        elm_box_align_set(pd->prop_swallow.swallows, 0.5, 0.0);
        elm_object_content_set(pd->prop_swallow.frame, pd->prop_swallow.swallows);
        evas_object_hide(pd->prop_swallow.frame);

        evas_object_smart_callback_add(check, "changed", _on_all_swallow_check, pd);
     }

   /* Texts UI setting*/
   if (!pd->prop_text.texts)
     {
        FRAME_LIVE_VIEW_ADD(property, pd->prop_text.frame, true, _("Texts"), pd->visual);
        elm_object_style_set(pd->prop_text.frame, "live_view");

        CHECK_ADD(prop_box, check);
        elm_object_style_set(check, "live_view");
        elm_object_part_content_set(pd->prop_text.frame, "elm.swallow.check", check);

        BOX_ADD(pd->prop_text.frame, pd->prop_text.texts, false, false)
        elm_box_align_set(pd->prop_text.texts, 0.5, 0.0);
        elm_object_content_set(pd->prop_text.frame, pd->prop_text.texts);
        evas_object_hide(pd->prop_text.frame);

        evas_object_smart_callback_add(check, "changed", _on_all_text_check, pd);
     }

   /* Signals UI setting*/
   if ((!pd->in_prog_edit) && (!pd->prop_signal.signals))
     {
        FRAME_LIVE_VIEW_ADD(property, pd->prop_signal.frame, true, _("Signals"), pd->visual);

        BOX_ADD(pd->prop_signal.frame, pd->prop_signal.signals, false, false)
        elm_box_align_set(pd->prop_signal.signals, 0.5, 0.0);
        elm_object_content_set(pd->prop_signal.frame, pd->prop_signal.signals);
        evas_object_hide(pd->prop_signal.frame);
     }
   evas_object_show(property);

   /* setting all swallows with rectangles */
   part_list = edje_edit_parts_list_get(style->obj);

   EINA_LIST_FOREACH(part_list, part, part_name)
     {
        part_type = edje_edit_part_type_get(style->obj, part_name);

        if (part_type ==  EDJE_PART_TYPE_SWALLOW)
          {
             swallow_parts_exists = true;
             ITEM_ADD(pd->prop_swallow.swallows, item, eina_stringshare_add(part_name), "eflete/property/item/live_view");
             CHECK_ADD(item, check);
             elm_object_style_set(check, "live_view");

             evas_object_smart_callback_add(check, "changed",
                                            evas_object_data_get(pd->live_object, SWALLOW_FUNC),
                                            pd);
             evas_object_data_set(check, PART_NAME, eina_stringshare_add(part_name));

             elm_object_part_content_set(item, "info", check);
             elm_box_pack_end(pd->prop_swallow.swallows, item);
             evas_object_show(item);
          }
        else if ((part_type ==  EDJE_PART_TYPE_TEXT) ||
                 (part_type ==  EDJE_PART_TYPE_TEXTBLOCK))
          {
             text_parts_exists = true;
             ITEM_ADD(pd->prop_text.texts, item, eina_stringshare_add(part_name), "eflete/property/item/live_view");
             CHECK_ADD(item, check);
             elm_object_style_set(check, "live_view");

             evas_object_smart_callback_add(check, "changed",
                                            evas_object_data_get(pd->live_object, TEXT_FUNC),
                                            pd);
             evas_object_data_set(check, PART_NAME, eina_stringshare_add(part_name));

             elm_object_part_content_set(item, "info", check);
             elm_box_pack_end(pd->prop_text.texts, item);
             evas_object_show(item);
          }
     }
   edje_edit_string_list_free(part_list);

   /* setting all signals for current widget or style except mouse-like ones */
   if (!pd->in_prog_edit)
     pd->signals = wm_program_signals_list_get(style);

   EINA_LIST_FOREACH(pd->signals, l, sig)
     {
        if ((strcmp(sig->name, "drag") != 0) &&
            (strncmp(sig->name, "mouse", strlen("mouse")) != 0))
          {
             signals_exists = true;
             ITEM_ADD(pd->prop_signal.signals, item, eina_stringshare_add(sig->name), "eflete/property/item/signals");
             BUTTON_ADD(item, button, "<-");

             evas_object_smart_callback_add(button, "clicked",
                                            evas_object_data_get(pd->live_object, SIGNAL_FUNC),
                                            pd->live_object);
             evas_object_data_set(button, SIGNAL_NAME, eina_stringshare_add(sig->name));
             evas_object_data_set(button, SIGNAL_SOURCE, eina_stringshare_add(sig->source));

             elm_object_part_content_set(item, "elm.swallow.content", button);
             elm_box_pack_end(pd->prop_signal.signals, item);
             evas_object_show(item);
          }
     }

   wm_program_signals_list_free(pd->signals);

   if (swallow_parts_exists)
     {
        elm_box_pack_end(prop_box, pd->prop_swallow.frame);
        evas_object_show(pd->prop_swallow.frame);
        evas_object_show(pd->prop_swallow.swallows);
     }
   if (text_parts_exists)
     {
        elm_box_pack_end(prop_box, pd->prop_text.frame);
        evas_object_show(pd->prop_text.frame);
        evas_object_show(pd->prop_text.texts);
     }
   if (signals_exists)
     {
        elm_box_pack_end(prop_box, pd->prop_signal.frame);
        evas_object_show(pd->prop_signal.frame);
        evas_object_show(pd->prop_signal.signals);
     }

   return true;
}

Evas_Object *
live_view_property_add(Evas_Object *parent, Eina_Bool in_prog_edit)
{
   Evas_Object *box, *scroller, *_bg;
   Prop_Data *pd;

   if (!parent) return NULL;

   pd = mem_calloc(1, sizeof(Prop_Data));

   pd->in_prog_edit = in_prog_edit;
   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   _bg = elm_bg_add(parent);
   elm_bg_file_set(_bg, EFLETE_RESOURCES, "section-item-bg");
   elm_object_part_content_set(scroller, "elm.swallow.background", _bg);
   evas_object_show(_bg);
   evas_object_show(scroller);
   evas_object_show(box);

   pd->visual = scroller;

   evas_object_data_set(scroller, PROP_DATA, pd);

   return scroller;
}

Eina_Bool
live_view_property_free(Evas_Object *property)
{
   if (!property)
     return false;

   Prop_Data *pd = evas_object_data_get(property, PROP_DATA);
   free(pd);
   evas_object_del(property);

   return true;
}

#define UPDATE_PROPERTY_FRAME(BOX, FRAME, OBJ) \
   elm_box_unpack(BOX, FRAME); \
   evas_object_hide(FRAME); \
   items_list = elm_box_children_get(OBJ); \
   elm_box_unpack_all(OBJ);

Eina_Bool
live_view_property_style_unset(Evas_Object *property)
{
   Evas_Object *prop_box, *data, *check, *button;
   Eina_List *items_list = NULL, *l;
   Edje_Part_Type part_type;
   Eina_Stringshare *part_name, *string;

   if (!property) return false;
   PROP_DATA_GET(false)

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack(prop_box, pd->scale_spinner);
   evas_object_hide(pd->scale_spinner);

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_OFF);

   /* Swallows Clear */
   if (pd->prop_swallow.frame)
     {
        UPDATE_PROPERTY_FRAME(prop_box, pd->prop_swallow.frame, pd->prop_swallow.swallows)
        check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");
        elm_check_state_set(check, false);
     }

   EINA_LIST_FOREACH(items_list, l, data)
     {
        check = elm_object_part_content_unset(data, "info");
        part_name = evas_object_data_get(check, PART_NAME);
        part_type = edje_edit_part_type_get(pd->style->obj, part_name);
        evas_object_smart_callback_del_full(check, "changed",
                                            evas_object_data_get(pd->live_object, SWALLOW_FUNC),
                                            pd->live_object);

        if (part_type == EDJE_PART_TYPE_SWALLOW)
          {
             Swallow_Clean_Func clean = (Swallow_Clean_Func)evas_object_data_get(pd->live_object,
                                                                                 SWALLOW_CLEAN_FUNC);
             if (clean) clean(part_name, pd->live_object);
          }

        eina_stringshare_del(part_name);
        evas_object_data_del(check, PART_NAME);

        evas_object_del(check);
        evas_object_del(data);
     }
   items_list = eina_list_free(items_list);

   /* Texts Clear */
   if (pd->prop_text.frame)
     {
        UPDATE_PROPERTY_FRAME(prop_box, pd->prop_text.frame, pd->prop_text.texts)
        check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");
        elm_check_state_set(check, false);
     }

   EINA_LIST_FOREACH(items_list, l, data)
     {
        check = elm_object_part_content_unset(data, "info");
        evas_object_smart_callback_del_full(check, "changed",
                                            evas_object_data_get(pd->live_object, TEXT_FUNC),
                                            pd->live_object);

        string = evas_object_data_get(check, PART_NAME);
        eina_stringshare_del(string);
        evas_object_data_del(check, PART_NAME);

        evas_object_del(check);
        evas_object_del(data);
     }
   items_list = eina_list_free(items_list);

   /* Signals Clear */
   if (pd->prop_signal.frame)
     {
        UPDATE_PROPERTY_FRAME(prop_box, pd->prop_signal.frame, pd->prop_signal.signals)
     }

   EINA_LIST_FOREACH(items_list, l, data)
     {
        button = elm_object_part_content_unset(data, "elm.swallow.content");
        evas_object_smart_callback_del_full(button, "clicked",
                                            evas_object_data_get(pd->live_object, SIGNAL_FUNC),
                                            pd->live_object);

        string = evas_object_data_get(button, SIGNAL_NAME);
        eina_stringshare_del(string);
        evas_object_data_del(button, SIGNAL_NAME);

        string = evas_object_data_get(button, SIGNAL_SOURCE);
        eina_stringshare_del(string);
        evas_object_data_del(button, SIGNAL_SOURCE);

        evas_object_del(button);
        evas_object_del(data);
     }
   eina_list_free(items_list);

   pd->live_object = NULL;

   return true;
}

#undef UPDATE_PROPERTY_FRAME

