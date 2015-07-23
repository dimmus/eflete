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
#define PROP_DATA_GET() \
   assert(property != NULL); \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   assert(pd != NULL);

#define FRAME_LIVE_VIEW_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE, SCROLLER) \
FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
evas_object_smart_callback_add(FRAME, "clicked", _on_frame_click, SCROLLER);
TODO("remove this hack after scroller would be fixed")
/*
 * Hack start
 */
static void
_on_frame_click(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Evas_Object *scroller = (Evas_Object *)data;

   assert(scroller != NULL);

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

   assert(pd != NULL);

   pd->current_scale = elm_spinner_value_get(obj) / 100;
   if (pd->live_object)
     {
        if (!strcmp("edje", pd->widget))
          {
             edje_object_scale_set(pd->live_object, pd->current_scale);
             edje_object_calc_force(pd->live_object);
          }
        else
          elm_object_scale_set(pd->live_object, pd->current_scale);
     }
}

static void
_on_all_swallow_check(void *data,
                      Evas_Object *obj,
                      void *ei __UNUSED__)
{
   Evas_Object *check = NULL;
   Eina_List *part_list = NULL, *part = NULL;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   part_list = elm_box_children_get(pd->prop_swallow.swallows);
   if (!part_list) return;

   EINA_LIST_FOREACH(part_list, part, check)
     {
        if (!elm_object_disabled_get(check))
          {
             elm_check_state_set(check, elm_check_state_get(obj));
             evas_object_smart_callback_call(check, "changed", NULL);
          }
     }

   eina_list_free(part_list);
}

static void
_on_all_text_check(void *data,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   Evas_Object *check = NULL;
   Eina_List *part_list = NULL, *part = NULL;
   Eina_Bool is_checked;

   Prop_Data *pd = (Prop_Data *)data;

   assert(pd != NULL);

   part_list = elm_box_children_get(pd->prop_text.texts);
   if (!part_list) return;

   is_checked = elm_check_state_get(obj);
   EINA_LIST_FOREACH(part_list, part, check)
     {
        if (elm_check_state_get(check) != is_checked)
          {
             elm_check_state_set(check, is_checked);
             evas_object_smart_callback_call(check, "changed", NULL);
          }
     }

   eina_list_free(part_list);
}


static void
_change_bg_cb(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Evas_Object *property = (Evas_Object *)data;

   assert(property != NULL);

   evas_object_smart_callback_call(property, "bg,changed", obj);
}

#define _RADIO_ADD(RADIO, VALUE, IMAGE, SWALLOW) \
   RADIO = elm_radio_add(item); \
   elm_object_style_set(RADIO, "style_editor"); \
   elm_radio_state_value_set(RADIO, VALUE); \
   evas_object_show(RADIO); \
   IMAGE_ADD_NEW(item, image_bg, "preview", IMAGE); \
   elm_object_part_content_set(RADIO, "bg", image_bg); \
   evas_object_smart_callback_add(RADIO, "changed", _change_bg_cb, property); \
   elm_object_part_content_set(item, SWALLOW, RADIO);

static void
_default_text_hash_free_cb(void *data)
{
   eina_stringshare_del(data);
}

Eina_Bool
live_view_property_style_set(Evas_Object *property,
                             Evas_Object *object,
                             Style *style,
                             const char *widget,
                             Evas_Object *parent)
{
   Evas_Object *prop_box, *spinner, *check, *button;
   Evas_Object *item, *ic;
   const char *part_name;
   Eina_List *part_list = NULL, *part = NULL, *l = NULL;
   Signal *sig = NULL;
   Edje_Part_Type part_type;
   Eina_Bool swallow_parts_exists = false, text_parts_exists = false, signal_parts_exists = false;;
   Evas_Object *image_bg = NULL;
   Evas_Object *radio_group = NULL;
   Evas_Object *radio = NULL;

   PROP_DATA_GET()
   assert(object != NULL);
   assert(style != NULL);
   assert(widget != NULL);
   assert(style->obj != NULL);
   assert(parent != NULL);

   pd->style = style;

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   pd->widget = widget;
   pd->live_object = object;

   prop_box = elm_object_content_get(pd->visual);

   if (!pd->header)
     {
        LAYOUT_PROP_ADD(parent, _("Scale:"), "live_view", "header")

        SPINNER_ADD(item, spinner, 1, 500, 1, true);
        elm_object_style_set(spinner, "live_view");
        elm_spinner_label_format_set(spinner, "%3.0f%%");
        evas_object_smart_callback_add(spinner, "changed", _on_scale_change, pd);
        pd->header = item;
        elm_object_part_content_set(item, "elm.swallow.content", spinner);
        evas_object_data_set(item, ITEM, spinner);

        _RADIO_ADD(radio_group, 0, "bg-tile", "elm.swallow.transparent");
        _RADIO_ADD(radio, 1, "bg-black", "elm.swallow.black");
        elm_radio_group_add(radio, radio_group);
        _RADIO_ADD(radio, 2, "bg-white", "elm.swallow.white");
        elm_radio_group_add(radio, radio_group);
#undef _RADIO_ADD
     }
   spinner = evas_object_data_get(pd->header, ITEM);
   elm_spinner_value_set(spinner, 100);
   evas_object_scale_set(pd->live_object, pd->current_scale);
   if (!pd->in_prog_edit)
     elm_object_part_content_set(parent, "elm.swallow.title", pd->header);
   else
     elm_box_pack_end(prop_box, pd->header);
   evas_object_show(pd->header);

   pd->prop_text.default_text = eina_hash_string_superfast_new(_default_text_hash_free_cb);

   /* Swallows UI setting*/
   if (!pd->prop_swallow.swallows)
     {
        FRAME_LIVE_VIEW_ADD(property, pd->prop_swallow.frame, true, _("Swallows"), pd->visual);
        elm_object_style_set(pd->prop_swallow.frame, "live_view");

        CHECK_ADD(prop_box, pd->prop_swallow.check);
        elm_object_part_content_set(pd->prop_swallow.frame, "elm.swallow.check", pd->prop_swallow.check);

        BOX_ADD(pd->prop_swallow.frame, pd->prop_swallow.swallows, false, false)
        elm_box_padding_set(pd->prop_swallow.swallows, 0, 7);
        elm_box_align_set(pd->prop_swallow.swallows, 0.5, 0.0);
        elm_object_content_set(pd->prop_swallow.frame, pd->prop_swallow.swallows);
        evas_object_hide(pd->prop_swallow.frame);

        evas_object_smart_callback_add(pd->prop_swallow.check, "changed", _on_all_swallow_check, pd);
        elm_layout_signal_emit(pd->prop_swallow.frame, "elm,state,icon,visible", "eflete");
     }

   /* Texts UI setting*/
   if (!pd->prop_text.texts)
     {
        FRAME_LIVE_VIEW_ADD(property, pd->prop_text.frame, true, _("Text"), pd->visual);
        elm_object_style_set(pd->prop_text.frame, "live_view");

        CHECK_ADD(prop_box, pd->prop_text.check);
        elm_object_part_content_set(pd->prop_text.frame, "elm.swallow.check", pd->prop_text.check);

        BOX_ADD(pd->prop_text.frame, pd->prop_text.texts, false, false)
        elm_box_padding_set(pd->prop_text.texts, 0, 7);
        elm_box_align_set(pd->prop_text.texts, 0.5, 0.0);
        elm_object_content_set(pd->prop_text.frame, pd->prop_text.texts);
        evas_object_hide(pd->prop_text.frame);

        evas_object_smart_callback_add(pd->prop_text.check, "changed", _on_all_text_check, pd);
        elm_layout_signal_emit(pd->prop_text.frame, "elm,state,icon,visible", "eflete");
     }

   /* Signals UI setting*/
   if ((!pd->in_prog_edit) && (!pd->prop_signal.signals))
     {
        FRAME_LIVE_VIEW_ADD(property, pd->prop_signal.frame, true, _("Signals"), pd->visual);
        elm_object_style_set(pd->prop_signal.frame, "live_view");

        BOX_ADD(pd->prop_signal.frame, pd->prop_signal.signals, false, false)
        elm_box_align_set(pd->prop_signal.signals, 0.5, 0.0);
        elm_object_content_set(pd->prop_signal.frame, pd->prop_signal.signals);
        evas_object_hide(pd->prop_signal.frame);
     }
   evas_object_show(property);

   elm_box_pack_end(prop_box, pd->prop_swallow.frame);
   evas_object_show(pd->prop_swallow.frame);
   evas_object_show(pd->prop_swallow.swallows);

   elm_box_pack_end(prop_box, pd->prop_text.frame);
   evas_object_show(pd->prop_text.frame);
   evas_object_show(pd->prop_text.texts);

   elm_box_pack_end(prop_box, pd->prop_signal.frame);
   evas_object_show(pd->prop_signal.frame);
   evas_object_show(pd->prop_signal.signals);

   /* setting all swallows with rectangles */
   part_list = edje_edit_parts_list_get(style->obj);

   EINA_LIST_FOREACH(part_list, part, part_name)
     {
        part_type = edje_edit_part_type_get(style->obj, part_name);

        if (part_type ==  EDJE_PART_TYPE_SWALLOW)
          {
             swallow_parts_exists = true;

             /* Weird behaviour!
                If we have box or frame as a parent it doesn't work at all.
                Check just doesn't show it's content (part name) but should.
                This problem appears when we add first text part into group */
             CHECK_ADD(parent, check);
             elm_object_part_text_set(check, NULL, part_name);

             evas_object_smart_callback_add(check, "changed",
                                            evas_object_data_get(pd->live_object, SWALLOW_FUNC),
                                            pd);
             evas_object_data_set(check, PART_NAME, eina_stringshare_add(part_name));

             if (!strcmp(part_name, "elm.swallow.action_area")) elm_object_disabled_set(check, true);
             elm_box_pack_end(pd->prop_swallow.swallows, check);
          }
        else if ((part_type ==  EDJE_PART_TYPE_TEXT) ||
                 (part_type ==  EDJE_PART_TYPE_TEXTBLOCK))
          {
             text_parts_exists = true;

             /* Weird behaviour!
                If we have box or frame as a parent it doesn't work at all.
                Check just doesn't show it's content (part name) but should.
                This problem appears when we add first text part into group */
             CHECK_ADD(parent, check);
             elm_object_part_text_set(check, NULL, part_name);

             evas_object_smart_callback_add(check, "changed",
                                            evas_object_data_get(pd->live_object, TEXT_FUNC),
                                            pd);
             evas_object_data_set(check, PART_NAME, eina_stringshare_add(part_name));

             elm_box_pack_end(pd->prop_text.texts, check);
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
             signal_parts_exists = true;
             LAYOUT_PROP_ADD(pd->prop_signal.signals,
                             sig->name,
                             "live_view",
                             "signal")

             BUTTON_ADD(item, button, NULL);
             ICON_STANDARD_ADD(button, ic, false, "impuls_in");
             elm_object_part_content_set(button, NULL, ic);

             evas_object_smart_callback_add(button, "clicked",
                                            evas_object_data_get(pd->live_object, SIGNAL_FUNC),
                                            pd->live_object);
             evas_object_data_set(button, SIGNAL_NAME, eina_stringshare_add(sig->name));
             evas_object_data_set(button, SIGNAL_SOURCE, eina_stringshare_add(sig->source));

             elm_object_part_content_set(item, "elm.swallow.content", button);
             elm_box_pack_end(pd->prop_signal.signals, item);
          }
     }
   wm_program_signals_list_free(pd->signals);

   if (!swallow_parts_exists)
     {
        elm_object_disabled_set(pd->prop_swallow.check, true);
        elm_frame_collapse_go(pd->prop_swallow.frame, false);
        elm_object_disabled_set(pd->prop_swallow.frame, true);
     }
   else
     {
        elm_object_disabled_set(pd->prop_swallow.check, false);
        elm_object_disabled_set(pd->prop_swallow.frame, false);
     }

   if (!text_parts_exists)
     {
        elm_object_disabled_set(pd->prop_text.check, true);
        elm_frame_collapse_go(pd->prop_text.frame, false);
        elm_object_disabled_set(pd->prop_text.frame, true);
     }
   else
     {
        elm_object_disabled_set(pd->prop_text.check, false);
        elm_object_disabled_set(pd->prop_text.frame, false);
     }

   if (!signal_parts_exists)
     {
        elm_frame_collapse_set(pd->prop_signal.frame, false);
        elm_object_disabled_set(pd->prop_signal.frame, true);
     }
   else elm_object_disabled_set(pd->prop_signal.frame, false);

   return true;
}

TODO("We need implementation here!~~ ")
Eina_Bool
live_view_property_part_add(Evas_Object *property __UNUSED__, Part *part __UNUSED__)
{
   printf("Signal: add part name [%s] \n", part->name);
   return true;
}

Eina_Bool
live_view_property_part_del(Evas_Object *property __UNUSED__, Part *part __UNUSED__)
{
   printf("Signal: del part name [%s] \n", part->name);
   return true;
}

Eina_Bool
live_view_property_part_rename(Evas_Object *property __UNUSED__, Part *part __UNUSED__, Eina_Stringshare *new_name __UNUSED__)
{
   printf("Signal: rename part name from [%s] to [%s] \n", part->name, new_name);
   return true;
}

Eina_Bool
live_view_property_part_restack_above(Evas_Object *property __UNUSED__, Part *part_move __UNUSED__, Part *part_above __UNUSED__)
{
   printf("Signal: moving part [%s] above [%s] \n", part_move->name, part_above->name);
   return true;
}

Eina_Bool
live_view_property_part_restack_below(Evas_Object *property __UNUSED__, Part *part_move, Part *part_below __UNUSED__)
{
   printf("Signal: moving part [%s] below [%s] \n", part_move->name, part_below->name);
   return true;
}

Evas_Object *
live_view_property_add(Evas_Object *parent, Eina_Bool in_prog_edit)
{
   Evas_Object *box, *scroller;
   Prop_Data *pd;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Prop_Data));

   pd->in_prog_edit = in_prog_edit;
   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   evas_object_show(scroller);
   evas_object_show(box);

   pd->visual = scroller;

   evas_object_data_set(scroller, PROP_DATA, pd);

   return scroller;
}

Eina_Bool
live_view_property_free(Evas_Object *property)
{
   PROP_DATA_GET()

   free(pd);
   evas_object_del(property);

   return true;
}

#define UPDATE_PROPERTY_FRAME(BOX, FRAME, OBJ) \
   evas_object_hide(FRAME); \
   items_list = elm_box_children_get(OBJ); \
   elm_box_clear(OBJ);

Eina_Bool
live_view_property_style_unset(Evas_Object *property)
{
   Evas_Object *prop_box, *check, *button, *data;
   Eina_List *items_list = NULL, *l;
   Edje_Part_Type part_type;
   Eina_Stringshare *part_name, *string;

   PROP_DATA_GET()

   prop_box = elm_object_content_get(pd->visual);
   elm_box_unpack_all(prop_box);
   evas_object_hide(pd->header);

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_OFF);

   eina_hash_free(pd->prop_text.default_text);
   pd->prop_text.default_text = NULL;

   /* Swallows Clear */
   if (pd->prop_swallow.frame)
     {
        evas_object_hide(pd->prop_swallow.frame);
        items_list = elm_box_children_get(pd->prop_swallow.swallows);
        check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.check");
        elm_check_state_set(check, false);
     }

   EINA_LIST_FOREACH(items_list, l, check)
     {
        part_name = elm_object_part_text_get(check, NULL);
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

        evas_object_del(check);
     }
   items_list = eina_list_free(items_list);
   elm_box_clear(pd->prop_swallow.swallows);

   /* Texts Clear */
   if (pd->prop_text.frame)
     {
        UPDATE_PROPERTY_FRAME(prop_box, pd->prop_text.frame, pd->prop_text.texts)
        check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.check");
        elm_check_state_set(check, false);
     }

   EINA_LIST_FOREACH(items_list, l, check)
     {
        evas_object_smart_callback_del_full(check, "changed",
                                            evas_object_data_get(pd->live_object, TEXT_FUNC),
                                            pd->live_object);
        evas_object_del(check);
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

