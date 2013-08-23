#include "ui_popup.h"

static void
_dismissed(void *data __UNUSED__,
           Evas_Object *obj,
           void *event_info __UNUSED__)
{
   Evas_Object *ctxpopup_data = evas_object_data_get(obj, "im");
   if (ctxpopup_data) evas_object_del(ctxpopup_data);
   evas_object_del(obj);
}

static void
_ruler_check_cb(void *data,
                Evas_Object *obj,
                void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Evas_Object *ruler = (Evas_Object *)data;
   Eina_Bool visible = elm_check_state_get(check);

   if (!visible) ui_ruler_hide (ruler);
   else ui_ruler_show (ruler);
   elm_check_state_set(check, !visible);
   elm_ctxpopup_dismiss(elm_object_parent_widget_get(obj));
}

static void
_hl_check_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Workspace *ws = (Workspace *)data;
   Eina_Bool visible;

   if (ws->highlight.part)
    {
      visible = elm_check_state_get(check);
      hl_highlight_visible_set(ws->highlight.space_hl, !visible);
      elm_check_state_set(check, !visible);
    }
   elm_ctxpopup_dismiss(elm_object_parent_widget_get(obj));
}

static void
_ruler_abs_cb(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Workspace *ws = (Workspace *)data;

   Eina_Bool visible = elm_check_state_get(check);

   ui_ruler_scale_absolute_visible_set (ws->ruler_hor, !visible);
   ui_ruler_scale_absolute_visible_set (ws->ruler_ver, !visible);
   elm_check_state_set(check, !visible);

   elm_ctxpopup_dismiss(elm_object_parent_widget_get(obj));
}

static void
_ruler_rel_cb(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Workspace *ws = (Workspace *)data;

   Eina_Bool visible = elm_check_state_get(check);

   ui_ruler_scale_relative_visible_set (ws->ruler_hor, !visible);
   ui_ruler_scale_relative_visible_set (ws->ruler_ver, !visible);
   elm_check_state_set(check, !visible);

   elm_ctxpopup_dismiss(elm_object_parent_widget_get(obj));
}


static void
_ctxpopup_item_ruler_cb(void *data,
                        Evas_Object *obj,
                        void *event_info)
{
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");
   Evas_Object *ruler = (Evas_Object*)data;
   Eina_Bool visible = ui_ruler_visible_get(ruler);

   elm_check_state_set(check, visible);
   if (visible) ui_ruler_hide (ruler);
   else ui_ruler_show (ruler);
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_zoom_in_cb(void *data,
                          Evas_Object *obj,
                          void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_ws_zoom_in(ws);
   elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_zoom_out_cb(void *data,
                           Evas_Object *obj,
                           void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_ws_zoom_out(ws);
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
  elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_space_hl_cb(void *data, Evas_Object *obj,
                         void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");
   Eina_Bool visible;
   if (ws->highlight.part)
    {
        visible = hl_highlight_visible_get(ws->highlight.space_hl);
        hl_highlight_visible_set(ws->highlight.space_hl, !visible);
        elm_check_state_set(check, !visible);
    }
  elm_ctxpopup_dismiss(obj);
}

static void
_ctxpopup_item_abs_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");
   Eina_Bool visible = ui_ruler_scale_absolute_visible_get(ws->ruler_hor);

   ui_ruler_scale_absolute_visible_set (ws->ruler_hor, !visible);
   ui_ruler_scale_absolute_visible_set (ws->ruler_ver, !visible);
   elm_check_state_set(check, !visible);

   elm_ctxpopup_dismiss(obj);
}


static void
_ctxpopup_item_rel_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");
   Eina_Bool visible = ui_ruler_scale_relative_visible_get(ws->ruler_hor);

   ui_ruler_scale_relative_visible_set (ws->ruler_hor, !visible);
   ui_ruler_scale_relative_visible_set (ws->ruler_ver, !visible);
   elm_check_state_set(check, !visible);

  elm_ctxpopup_dismiss(obj);
}


Evas_Object *
_popup_add (Workspace *ws)
{
   Evas_Object *ctxpopup, *check;
   Elm_Object_Item *eoi = NULL;
   Eina_Bool visible;
   ctxpopup = elm_ctxpopup_add(elm_object_parent_widget_get(ws->scroller));
   evas_object_smart_callback_add(ctxpopup, "dismissed", _dismissed, NULL);

   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom +", NULL,
                                  _ctxpopup_item_zoom_in_cb, ws);
   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom -", NULL,
                                  _ctxpopup_item_zoom_out_cb, ws);
   eoi = elm_ctxpopup_item_append(ctxpopup, "separate", NULL,
                                  _ctxpopup_item_separate_cb, ws);
   eoi = elm_ctxpopup_item_append(ctxpopup, "legend", NULL,
                                  _ctxpopup_item_legend_cb, ws);
   eoi = elm_ctxpopup_item_append(ctxpopup, "Highlight", NULL,
                          _ctxpopup_item_space_hl_cb, ws);
   check = elm_check_add(ctxpopup);
   if (ws->highlight.part)
     {
        visible = hl_highlight_visible_get(ws->highlight.space_hl);
        elm_check_state_set(check, visible);
     }
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _hl_check_cb, ws);
   evas_object_show(check);


   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler hor.", NULL,
                            _ctxpopup_item_ruler_cb, ws->ruler_hor);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_visible_get(ws->ruler_hor);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_check_cb, ws->ruler_hor);
   evas_object_show(check);

   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler ver.", NULL,
                                  _ctxpopup_item_ruler_cb, ws->ruler_ver);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_check_cb, ws->ruler_ver);
   evas_object_show(check);


   eoi = elm_ctxpopup_item_append(ctxpopup, "absolute scale", NULL,
                                  _ctxpopup_item_abs_cb, ws);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_scale_absolute_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_abs_cb, ws);
   evas_object_show(check);

   eoi = elm_ctxpopup_item_append(ctxpopup, "relative scale", NULL,
                                  _ctxpopup_item_rel_cb, ws);
   check = elm_check_add(ctxpopup);
   visible = ui_ruler_scale_relative_visible_get(ws->ruler_ver);
   elm_check_state_set(check, visible);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_rel_cb, ws);
   evas_object_show(check);

   return ctxpopup;
}

void
ui_popup_show (Workspace *ws)
{
   Evas_Coord x,y;
   Evas_Object *popup = NULL;
   popup = _popup_add(ws);
   evas_pointer_canvas_xy_get(evas_object_evas_get(ws->bg), &x, &y);
   evas_object_move(popup, x, y);
   evas_object_show(popup);
}
