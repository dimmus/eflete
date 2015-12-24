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
        if (!pd->group->widget)
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

static Eina_Bool
_live_view_property_load(Evas_Object *property,
                         Live_View *live,
                         Group *group,
                         Evas_Object *parent)
{
   Evas_Object *prop_box, *spinner, *check, *button;
   Evas_Object *item, *ic;
   const char *part_name;
   Eina_List *part_list = NULL, *part = NULL, *l = NULL;
   Edje_Part_Type part_type;
   Eina_Bool swallow_parts_exists = false, text_parts_exists = false, signal_parts_exists = false;
   Evas_Object *image_bg = NULL;
   Evas_Object *radio_group = NULL;
   Evas_Object *radio = NULL;

   PROP_DATA_GET()
   assert(live != NULL);
   assert(live->object != NULL);
   assert(group != NULL);
   assert(group->edit_object != NULL);
   assert(parent != NULL);

   pd->group = group;
   pd->parent = parent;
   pd->live_object = live->object;

   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   elm_scroller_policy_set(pd->visual, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

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
        elm_object_style_set(pd->prop_swallow.frame, "icon");

        CHECK_ADD(prop_box, pd->prop_swallow.check);
        elm_object_part_content_set(pd->prop_swallow.frame, "elm.swallow.icon", pd->prop_swallow.check);

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
        elm_object_style_set(pd->prop_text.frame, "icon");

        CHECK_ADD(prop_box, pd->prop_text.check);
        elm_object_part_content_set(pd->prop_text.frame, "elm.swallow.icon", pd->prop_text.check);

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
   part_list = edje_edit_parts_list_get(group->edit_object);

   EINA_LIST_FOREACH(part_list, part, part_name)
     {
        part_type = edje_edit_part_type_get(group->edit_object, part_name);

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

             elm_box_pack_end(pd->prop_text.texts, check);
          }
     }
   edje_edit_string_list_free(part_list);
   /* setting all signals for current widget or style except mouse-like ones */
   if (!pd->in_prog_edit)
     pd->signals = edje_edit_programs_list_get(group->edit_object);

   Eina_Stringshare *prog_name, *sig_name, *source_name;
   EINA_LIST_FOREACH(pd->signals, l, prog_name)
     {
        sig_name = edje_edit_program_signal_get(group->edit_object, prog_name);
        source_name = edje_edit_program_source_get(group->edit_object, prog_name);
        if (!source_name) source_name = eina_stringshare_add("");
        if ((sig_name) && (strcmp(sig_name, "drag") != 0) &&
            (strncmp(sig_name, "mouse", strlen("mouse")) != 0))
          {
             signal_parts_exists = true;
             LAYOUT_PROP_ADD(pd->prop_signal.signals,
                             sig_name,
                             "live_view",
                             "signal")

             BUTTON_ADD(item, button, NULL);
             ICON_STANDARD_ADD(button, ic, false, "impuls_in");
             elm_object_part_content_set(button, NULL, ic);

             evas_object_smart_callback_add(button, "clicked",
                                            evas_object_data_get(pd->live_object, SIGNAL_FUNC),
                                            pd->live_object);
             evas_object_data_set(button, SIGNAL_NAME, eina_stringshare_add(sig_name));
             evas_object_data_set(button, SIGNAL_SOURCE, eina_stringshare_add(source_name));

             elm_object_part_content_set(item, "elm.swallow.content", button);
             elm_box_pack_end(pd->prop_signal.signals, item);
          }
     }
   edje_edit_string_list_free(pd->signals);

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
live_view_property_part_add(Evas_Object *property, Part_ *part)
{
   Evas_Object *check;
   PROP_DATA_GET();

   if (part->type ==  EDJE_PART_TYPE_SWALLOW)
     {
        CHECK_ADD(pd->parent, check);
        elm_object_part_text_set(check, NULL, part->name);

        evas_object_smart_callback_add(check, "changed",
                                       evas_object_data_get(pd->live_object, SWALLOW_FUNC),
                                       pd);

        elm_object_disabled_set(pd->prop_swallow.check, false);
        elm_object_disabled_set(pd->prop_swallow.frame, false);

        if (!strcmp(part->name, "elm.swallow.action_area")) elm_object_disabled_set(check, true);
        elm_box_pack_end(pd->prop_swallow.swallows, check);
     }
   else if ((part->type ==  EDJE_PART_TYPE_TEXT) ||
            (part->type ==  EDJE_PART_TYPE_TEXTBLOCK))
     {
        CHECK_ADD(pd->parent, check);
        elm_object_part_text_set(check, NULL, part->name);

        evas_object_smart_callback_add(check, "changed",
                                       evas_object_data_get(pd->live_object, TEXT_FUNC),
                                       pd);

        elm_object_disabled_set(pd->prop_text.check, false);
        elm_object_disabled_set(pd->prop_text.frame, false);

        elm_box_pack_end(pd->prop_text.texts, check);
     }

   return true;
}

Eina_Bool
live_view_property_part_del(Evas_Object *property, Part_ *part)
{
   Evas_Object *check, *item_box, *frame, *frame_check;
   Eina_List *items_list = NULL, *l;
   PROP_DATA_GET();
   Eina_Stringshare *part_name = NULL;

   /* texts and swallows are different boxes */
   if (part->type ==  EDJE_PART_TYPE_SWALLOW)
     {
        item_box = pd->prop_swallow.swallows;
        frame = pd->prop_swallow.frame;
        frame_check = pd->prop_swallow.check;
        items_list = elm_box_children_get(item_box);
     }
   else if ((part->type ==  EDJE_PART_TYPE_TEXT) ||
            (part->type ==  EDJE_PART_TYPE_TEXTBLOCK))
     {
        item_box = pd->prop_text.texts;
        frame = pd->prop_text.frame;
        frame_check = pd->prop_text.check;
        items_list = elm_box_children_get(item_box);
     }
   else return false;

   EINA_LIST_FOREACH(items_list, l, check)
     {
        part_name = elm_object_part_text_get(check, NULL);
        if (!strcmp(part->name, part_name))
          {
             /* texts and swallows are deleting in different way */
             if (part->type ==  EDJE_PART_TYPE_SWALLOW)
               {
                  evas_object_smart_callback_del_full(check, "changed",
                                                      evas_object_data_get(pd->live_object, SWALLOW_FUNC),
                                                      pd->live_object);
                  Swallow_Clean_Func clean = (Swallow_Clean_Func)evas_object_data_get(pd->live_object,
                                                                                      SWALLOW_CLEAN_FUNC);
                  if (clean) clean(part_name, pd->live_object);
               }
             else if ((part->type ==  EDJE_PART_TYPE_TEXT) ||
                      (part->type ==  EDJE_PART_TYPE_TEXTBLOCK))
               {
                  evas_object_smart_callback_del_full(check, "changed",
                                                      evas_object_data_get(pd->live_object, TEXT_FUNC),
                                                      pd->live_object);
               }
             elm_box_unpack(item_box, check);
             evas_object_del(check);
             break;
          }
     }

   /* if there were only one item, then because of this function it would be 0.
      After deleting and removing item from box, list, that was returned by
      elm_box_children_get, is not updated, so there are still one object. */
   if (eina_list_count(items_list) == 1)
     {
        elm_object_disabled_set(frame_check, true);
        elm_frame_collapse_go(frame, false);
        elm_object_disabled_set(frame, true);
     }

   eina_list_free(items_list);

   return true;
}

Eina_Bool
live_view_property_part_rename(Evas_Object *property, Part *part, Eina_Stringshare *new_name)
{
   Evas_Object  *item_box = NULL, *check;
   Eina_List *items_list, *l;
   PROP_DATA_GET();
   Eina_Stringshare *part_name;

   if ((part->type != EDJE_PART_TYPE_TEXT) &&
       (part->type != EDJE_PART_TYPE_TEXTBLOCK) &&
       (part->type != EDJE_PART_TYPE_SWALLOW))
     return false;

   if (part->type ==  EDJE_PART_TYPE_SWALLOW)
     item_box = pd->prop_swallow.swallows;
   else if ((part->type ==  EDJE_PART_TYPE_TEXT) ||
            (part->type ==  EDJE_PART_TYPE_TEXTBLOCK))
     item_box = pd->prop_text.texts;

   /* Now lets restack this.
      We need to find moving check and check we need to move above */
   assert(item_box != NULL);
   items_list = elm_box_children_get(item_box);
   EINA_LIST_FOREACH(items_list, l, check)
     {
        part_name = elm_object_part_text_get(check, NULL);
        if (!strcmp(part_name, part->name))
          {
             elm_object_part_text_set(check, NULL, new_name);
             break;
          }
     }

   return true;
}

#define IS_SWALLOW_MOVE (part_move->type == EDJE_PART_TYPE_SWALLOW)
#define IS_SWALLOW_CURR (part_type == EDJE_PART_TYPE_SWALLOW)

#define IS_TEXT_OR_TEXTBLOCK_MOVE \
  ((part_move->type == EDJE_PART_TYPE_TEXT) || (part_move->type == EDJE_PART_TYPE_TEXTBLOCK))

#define IS_TEXT_OR_TEXTBLOCK_CURR \
  ((part_type == EDJE_PART_TYPE_TEXT) || (part_type == EDJE_PART_TYPE_TEXTBLOCK))

Eina_Bool
live_view_property_part_restack_above(Evas_Object *property, Part *part_move, Part *part_above)
{
   Evas_Object *check_nearest = NULL, *check_move = NULL, *item_box = NULL, *check;
   Eina_List *part_list = NULL, *part = NULL, *after_above_list = NULL;
   Eina_List *items_list = NULL, *l;
   PROP_DATA_GET();
   Eina_Stringshare *part_name, *nearest_part = NULL;
   int part_type;

   if ((part_move->type != EDJE_PART_TYPE_TEXT) &&
       (part_move->type != EDJE_PART_TYPE_TEXTBLOCK) &&
       (part_move->type != EDJE_PART_TYPE_SWALLOW))
     return false;

   part_list = edje_edit_parts_list_get(pd->group->edit_object);

   /* start searching for part with same type from part_above point.
      We need to find name of this part. */
   after_above_list = eina_list_data_find_list(part_list, part_above->name);
   EINA_LIST_FOREACH(after_above_list, part, part_name)
     {
        part_type = edje_edit_part_type_get(pd->group->edit_object, part_name);

        if (!strcmp(part_name, part_move->name))
          return false;

        if ((IS_SWALLOW_MOVE && IS_SWALLOW_CURR) ||
            (IS_TEXT_OR_TEXTBLOCK_MOVE && IS_TEXT_OR_TEXTBLOCK_CURR))
          {
             nearest_part = part_name;
             break;
          }
     }
   edje_edit_string_list_free(part_list);

   if (part_move->type ==  EDJE_PART_TYPE_SWALLOW)
     item_box = pd->prop_swallow.swallows;
   else if ((part_move->type ==  EDJE_PART_TYPE_TEXT) ||
            (part_move->type ==  EDJE_PART_TYPE_TEXTBLOCK))
     item_box = pd->prop_text.texts;

   /* Now lets restack this.
      We need to find moving check and check we need to move above */
   assert(item_box != NULL);
   items_list = elm_box_children_get(item_box);
   EINA_LIST_FOREACH(items_list, l, check)
     {
        part_name = elm_object_part_text_get(check, NULL);
        if (nearest_part && !strcmp(part_name, nearest_part))
          check_nearest = check;

        if (!strcmp(part_name, part_move->name))
          check_move = check;

        if (check_move && check_nearest)
          break;
     }

   assert(check_move != NULL);
   elm_box_unpack(item_box, check_move);

   if (nearest_part)
     {
        assert(check_nearest != NULL);
        elm_box_pack_before(item_box, check_move, check_nearest);
     }
   else /* if not found then we drop it to the end */
     elm_box_pack_end(item_box, check_move);

   return true;
}

#undef IS_SWALLOW_MOVE
#undef IS_SWALLOW_CURR
#undef IS_TEXT_OR_TEXTBLOCK_MOVE
#undef IS_TEXT_OR_TEXTBLOCK_CURR

Eina_Bool
live_view_property_part_restack_below(Evas_Object *property, Part *part_move)
{
   Evas_Object *check_move = NULL, *item_box = NULL, *check;
   Eina_List *items_list = NULL, *l;
   PROP_DATA_GET();
   Eina_Stringshare *part_name = NULL;

   if ((part_move->type != EDJE_PART_TYPE_TEXT) &&
       (part_move->type != EDJE_PART_TYPE_TEXTBLOCK) &&
       (part_move->type != EDJE_PART_TYPE_SWALLOW))
     return false;

   /* Now lets restack this */
   if (part_move->type ==  EDJE_PART_TYPE_SWALLOW)
     item_box = pd->prop_swallow.swallows;
   else if ((part_move->type ==  EDJE_PART_TYPE_TEXT) ||
            (part_move->type ==  EDJE_PART_TYPE_TEXTBLOCK))
     item_box = pd->prop_text.texts;

   /* find check that we are moving and pack it to the end of items/box/lists */
   items_list = elm_box_children_get(item_box);
   EINA_LIST_FOREACH(items_list, l, check)
     {
        part_name = elm_object_part_text_get(check, NULL);

        if (!strcmp(part_name, part_move->name))
          check_move = check;

        if (check_move) break;
     }

   elm_box_unpack(item_box, check_move);
   elm_box_pack_end(item_box, check_move);
   return true;
}

Evas_Object *
live_view_property_add(Evas_Object *object, Group *group, Eina_Bool in_prog_edit)
{
   Evas_Object *box, *scroller;
   Prop_Data *pd;

   assert(object != NULL);
   assert(group != NULL);
   assert(ap.project != NULL);

   Live_View *live = evas_object_data_get(object, "live_view_structure");
   assert(live != NULL);
   assert(live->object != NULL);

   pd = mem_calloc(1, sizeof(Prop_Data));

   pd->in_prog_edit = in_prog_edit;
   SCROLLER_ADD(live->panel, scroller);
   BOX_ADD(scroller, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);

   evas_object_show(scroller);
   evas_object_show(box);

   pd->visual = scroller;

   evas_object_data_set(scroller, PROP_DATA, pd);

   /* now filling data */
   _live_view_property_load(scroller, live, group, object);

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
        check = elm_object_part_content_get(pd->prop_swallow.frame, "elm.swallow.icon");
        elm_check_state_set(check, false);
     }

   EINA_LIST_FOREACH(items_list, l, check)
     {
        part_name = elm_object_part_text_get(check, NULL);
        part_type = edje_edit_part_type_get(pd->group->edit_object, part_name);
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
        check = elm_object_part_content_get(pd->prop_text.frame, "elm.swallow.icon");
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

