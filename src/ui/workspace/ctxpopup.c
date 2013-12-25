/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "ctxpopup.h"
#include "common_macro.h"
#include "highlight.h"
#include "logger.h"
#include "eflete.h"

static void
_dismissed(void *data __UNUSED__,
           Evas_Object *obj,
           void *event_info __UNUSED__)
{
   evas_object_del(obj);
}

static void
_ctxpopup_item_ruler_h_cb(void *data,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,hide,hor", strdup("hor"));
   elm_ctxpopup_dismiss(obj);
}


static void
_ctxpopup_item_ruler_v_cb(void *data,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,hide,ver", strdup("ver"));
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_zoom_in_cb(void *data,
                          Evas_Object *obj,
                          void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   double current_factor = ws_zoom_factor_get(ws);
   ws_zoom_factor_set(ws, current_factor + 0.1);
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_zoom_out_cb(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   double current_factor = ws_zoom_factor_get(ws);
   ws_zoom_factor_set(ws, current_factor - 0.1);
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_separate_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   //Evas_Object *ws = (Evas_Object *)data;
   //ui_groupspace_separate(ws);
   //elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_legend_cb(void *data,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
  Evas_Object *ws = (Evas_Object *)data;
  ws_legend_visible_set(ws);
  elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_space_hl_cb(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "highlight,visible", NULL);

   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_abs_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,hide,hor", strdup("abs"));
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_rel_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Evas_Object *ws = (Evas_Object *)data;
   evas_object_smart_callback_call(ws, "ruler,hide,hor", strdup("rel"));
   elm_ctxpopup_dismiss(obj);
}

Evas_Object *
ui_popup_add(Evas_Object *obj)
{
   Evas_Object *ctxpopup;/* *check;*/
   Elm_Object_Item *eoi = NULL;
/*   Eina_Bool visible = false;*/
   int x, y;

   ctxpopup = elm_ctxpopup_add(obj);
   evas_object_smart_callback_add(ctxpopup, "dismissed", _dismissed, NULL);

   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom +", NULL,
                                  _ctxpopup_item_zoom_in_cb, obj);
   elm_object_item_disabled_set(eoi, true);
   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom -", NULL,
                                  _ctxpopup_item_zoom_out_cb, obj);
   elm_object_item_disabled_set(eoi, true);
   eoi = elm_ctxpopup_item_append(ctxpopup, "separate", NULL,
                                  _ctxpopup_item_separate_cb, obj);
   eoi = elm_ctxpopup_item_append(ctxpopup, "legend", NULL,
                                  _ctxpopup_item_legend_cb, obj);
   elm_object_item_disabled_set(eoi, true);
   eoi = elm_ctxpopup_item_append(ctxpopup, "Highlight", NULL,
                          _ctxpopup_item_space_hl_cb, obj);
/*    check = elm_check_add(ctxpopup);
    elm_object_item_part_content_set(eoi, "icon", check);
    evas_object_show(check);

   if (ws->highlight.part)
     {
        visible = evas_object_visible_get(ws->highlight.space_hl);
        elm_check_state_set(check, visible);
     }
   if ((!ws->separated) && (!ws->highlight.highlight))
     elm_object_item_disabled_set(eoi, true);
   else
     elm_object_item_disabled_set(eoi, false);
*/
   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler hor.", NULL,
                            _ctxpopup_item_ruler_h_cb, obj);
/*   check = elm_check_add(ctxpopup);
     visible = ui_ruler_visible_get(ws->ruler_hor);
     elm_check_state_set(check, visible);
     elm_object_item_part_content_set(eoi, "icon", check);
     evas_object_show(check);
*/
   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler ver.", NULL,
                                  _ctxpopup_item_ruler_v_cb, obj);
/*   check = elm_check_add(ctxpopup);
     visible = ui_ruler_visible_get(ws->ruler_ver);
     elm_check_state_set(check, visible);
     elm_object_item_part_content_set(eoi, "icon", check);
     evas_object_show(check);
*/
   eoi = elm_ctxpopup_item_append(ctxpopup, "absolute scale", NULL,
                                  _ctxpopup_item_abs_cb, obj);
/*   check = elm_check_add(ctxpopup);
   visible = ui_ruler_scale_absolute_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);

   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_show(check);
*/
   eoi = elm_ctxpopup_item_append(ctxpopup, "relative scale", NULL,
                                  _ctxpopup_item_rel_cb, obj);
/*   check = elm_check_add(ctxpopup);
   visible = ui_ruler_scale_relative_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_show(check);
*/
   evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y);
   evas_object_move(ctxpopup, x, y);
   evas_object_show(ctxpopup);

   return ctxpopup;
}
