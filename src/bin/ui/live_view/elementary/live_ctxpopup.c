/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "live_elementary_widgets.h"

static void
_dismissed_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info __UNUSED__)
{
   evas_object_del(obj);
}

static Elm_Object_Item *
_item_new(Evas_Object *ctxpopup, int item_num, const char *icon)
{
   Eina_Stringshare *name = NULL;
   Evas_Object *ic = NULL;
   Elm_Object_Item * ret;

   assert(ctxpopup != NULL);

   if (item_num > 0)
     name = eina_stringshare_printf("Item #%d", item_num);
   if (icon)
     {
        ic = elm_icon_add(ctxpopup);
        elm_icon_standard_set(ic, icon);
        elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
     }
   ret = elm_ctxpopup_item_append(ctxpopup, name, ic, NULL, NULL);
   eina_stringshare_del(name);
   return ret;
}

static void
_ctx_popup_create(Evas_Object *parent,
                  Eina_Stringshare *style_name,
                  int item_blocks_count,
                  Eina_Bool horizontal,
                  Eina_Bool with_icons,
                  Eina_Bool with_labels)
{
   int i;
   Evas_Coord x,y;

   assert(parent != NULL);
   assert(style_name != NULL);

   Evas_Object *ctxpopup = elm_ctxpopup_add(parent);
   elm_object_style_set(ctxpopup, style_name);
   evas_object_smart_callback_add(ctxpopup, "dismissed", _dismissed_cb, NULL);

   elm_ctxpopup_horizontal_set(ctxpopup, horizontal);

   for (i = 0; i < item_blocks_count; i++)
     {
        _item_new(ctxpopup, (with_labels)?3*i + 1:-1, (with_icons)?"home":NULL);
        _item_new(ctxpopup, (with_labels)?3*i + 2:-1, (with_icons)?"file":NULL);
        _item_new(ctxpopup, (with_labels)?3*i + 3:-1, (with_icons)?"delete":NULL);
     }

   evas_pointer_canvas_xy_get(evas_object_evas_get(parent), &x, &y);
   evas_object_move(ctxpopup, x, y);
   evas_object_show(ctxpopup);
}

static void
_labels_icons_vertical_3items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 1, false, true, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_nolabels_icons_vertical_3items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 1, false, true, false);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_noicons_vertical_3items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 1, false, false, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_icons_horizontal_3items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 1, true, true, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_nolabels_icons_horizontal_3items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 1, true, true, false);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_noicons_horizontal_3items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 1, true, false, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_icons_vertical_30items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 10, false, true, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_nolabels_icons_vertical_30items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 10, false, true, false);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_noicons_vertical_30items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 10, false, false, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_icons_horizontal_30items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 10, true, true, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_nolabels_icons_horizontal_30items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 10, true, true, false);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

static void
_labels_noicons_horizontal_30items_cb(void *data __UNUSED__, Evas_Object *obj, void *event_info)
{
   Eina_Stringshare *style_name = evas_object_data_get(obj, "style_name");
   _ctx_popup_create(obj, style_name, 10, true, false, true);
   elm_list_item_selected_set(event_info, EINA_FALSE);
}

Evas_Object *
widget_ctxpopup_create(Evas_Object *parent, const Group *group)
{
   assert(parent != NULL);
   assert(group != NULL);
   assert(group->style != NULL);

   Evas_Object *list = elm_list_add(parent);
   evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_list_mode_set(list, ELM_LIST_COMPRESS);
   elm_list_item_append(list, _("Click to show vertical ctxpopup with icons and labels (3 items)"),
                        NULL, NULL, _labels_icons_vertical_3items_cb, NULL);
   elm_list_item_append(list, _("Click to show vertical ctxpopup with icons and labels (30 items)"),
                        NULL, NULL, _labels_icons_vertical_30items_cb, NULL);
   elm_list_item_append(list, _("Click to show vertical ctxpopup with icons and no labels (3 items)"),
                        NULL, NULL, _nolabels_icons_vertical_3items_cb, NULL);
   elm_list_item_append(list, _("Click to show vertical ctxpopup with icons and no labels (30 items)"),
                        NULL, NULL, _nolabels_icons_vertical_30items_cb, NULL);
   elm_list_item_append(list, _("Click to show vertical ctxpopup with labels and no icons (3 items)"),
                        NULL, NULL, _labels_noicons_vertical_3items_cb, NULL);
   elm_list_item_append(list, _("Click to show vertical ctxpopup with labels and no icons (30 items)"),
                        NULL, NULL, _labels_noicons_vertical_30items_cb, NULL);
   elm_list_item_append(list, _("Click to show horizontal ctxpopup with icons and labels (3 items)"),
                        NULL, NULL, _labels_icons_horizontal_3items_cb, NULL);
   elm_list_item_append(list, _("Click to show horizontal ctxpopup with icons and labels (30 items)"),
                        NULL, NULL, _labels_icons_horizontal_30items_cb, NULL);
   elm_list_item_append(list, _("Click to show horizontal ctxpopup with icons and no labels (3 items)"),
                        NULL, NULL, _nolabels_icons_horizontal_3items_cb, NULL);
   elm_list_item_append(list, _("Click to show horizontal ctxpopup with icons and no labels (30 items)"),
                        NULL, NULL, _nolabels_icons_horizontal_30items_cb, NULL);
   elm_list_item_append(list, _("Click to show horizontal ctxpopup with labels and no icons (3 items)"),
                        NULL, NULL, _labels_noicons_horizontal_3items_cb, NULL);
   elm_list_item_append(list, _("Click to show horizontal ctxpopup with labels and no icons (30 items)"),
                        NULL, NULL, _labels_noicons_horizontal_30items_cb, NULL);
   elm_list_go(list);

   evas_object_data_set(list, "style_name", group->style);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_SET, on_swallow_check, list);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_SET, on_text_check, list);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SIGNAL_SEND, send_signal, list);

   return list;
}
