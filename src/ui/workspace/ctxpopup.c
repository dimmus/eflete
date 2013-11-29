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
#include "ui_groupspace.h"
#include "highlight.h"
#include "logger.h"

static void
_dismissed(void *data __UNUSED__,
           Evas_Object *obj,
           void *event_info __UNUSED__)
{
   evas_object_del(obj);
}

static void
_ctxpopup_item_ruler_cb(void *data,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   Evas_Object *ruler = (Evas_Object*)data;
   Eina_Bool visible = ui_ruler_visible_get(ruler);

   if (visible) ui_ruler_hide (ruler);
   else ui_ruler_show (ruler);

   DBG("Ruler was been %s", visible ? "hiden" : "showed");
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_zoom_in_cb(void *data,
                          Evas_Object *obj,
                          void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_ws_zoom_in(ws);
   DBG("Zoom in (++)");
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_zoom_out_cb(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_ws_zoom_out(ws);
   DBG("Zoom out (--)");
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_separate_cb(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_groupspace_separate(ws);
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_legend_cb(void *data,
                         Evas_Object *obj,
                         void *event_info __UNUSED__)
{
  Workspace *ws = (Workspace *)data;
  ui_ws_legend_visible_set(ws, !ws->legend.visible);
  DBG("Legend is %s", ws->legend.visible ? "showed" : "hiden");
  elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_space_hl_cb(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   Eina_Bool visible;
   if ((ws->highlight.part) && (ws->highlight.space_hl))
    {
       visible = evas_object_visible_get(ws->highlight.space_hl);
       if (visible) evas_object_hide(ws->highlight.space_hl);
       else evas_object_show(ws->highlight.space_hl);
       DBG("Relative highlight was been %s", visible ? "hiden" : "showed");
    }
  elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_abs_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   Eina_Bool visible = ui_ruler_scale_absolute_visible_get(ws->ruler_hor);

   ui_ruler_scale_absolute_visible_set (ws->ruler_hor, !visible);
   ui_ruler_scale_absolute_visible_set (ws->ruler_ver, !visible);
   DBG("Ruler absolute scale was been %s", visible ? "hiden" : "showed");

   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_rel_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   Eina_Bool visible = ui_ruler_scale_relative_visible_get(ws->ruler_hor);

   ui_ruler_scale_relative_visible_set (ws->ruler_hor, !visible);
   ui_ruler_scale_relative_visible_set (ws->ruler_ver, !visible);
   DBG("Ruler relative scale was been %s", visible ? "hiden" : "showed");

   elm_ctxpopup_dismiss(obj);
}

Evas_Object *
_popup_add(Workspace *ws)
{
   Evas_Object *ctxpopup, *check;
   Elm_Object_Item *eoi = NULL;
   Eina_Bool visible;
   ctxpopup = elm_ctxpopup_add(elm_object_parent_widget_get(ws->scroller));
   evas_object_smart_callback_add(ctxpopup, "dismissed", _dismissed, NULL);

   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom +", NULL,
                                  _ctxpopup_item_zoom_in_cb, ws);
   elm_object_item_disabled_set(eoi, true);
   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom -", NULL,
                                  _ctxpopup_item_zoom_out_cb, ws);
   elm_object_item_disabled_set(eoi, true);
   eoi = elm_ctxpopup_item_append(ctxpopup, "separate", NULL,
                                  _ctxpopup_item_separate_cb, ws);
   eoi = elm_ctxpopup_item_append(ctxpopup, "legend", NULL,
                                  _ctxpopup_item_legend_cb, ws);
   eoi = elm_ctxpopup_item_append(ctxpopup, "Highlight", NULL,
                          _ctxpopup_item_space_hl_cb, ws);
   check = elm_check_add(ctxpopup);
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

   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler hor.", NULL,
                            _ctxpopup_item_ruler_cb, ws->ruler_hor);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_visible_get(ws->ruler_hor);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_show(check);

   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler ver.", NULL,
                                  _ctxpopup_item_ruler_cb, ws->ruler_ver);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_show(check);

   eoi = elm_ctxpopup_item_append(ctxpopup, "absolute scale", NULL,
                                  _ctxpopup_item_abs_cb, ws);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_scale_absolute_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_show(check);

   eoi = elm_ctxpopup_item_append(ctxpopup, "relative scale", NULL,
                                  _ctxpopup_item_rel_cb, ws);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_scale_relative_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_show(check);

   return ctxpopup;
}

void
ws_ctxpopup_show(Workspace *ws)
{
   Evas_Coord x,y;
   Evas_Object *popup = NULL;
   popup = _popup_add(ws);
   evas_pointer_canvas_xy_get(evas_object_evas_get(ws->bg), &x, &y);
   evas_object_move(popup, x, y);
   evas_object_show(popup);
}
