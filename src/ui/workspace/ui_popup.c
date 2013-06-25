#include "ui_popup.h"
#include "efl_tet.h"

static void
_ctxpopup_item_ruler_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info)
{
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");
   Evas_Object *ruler = (Evas_Object*)data;

   if (ui_ruler_visible_get(ruler))
     {
        ui_ruler_hide (ruler);
        elm_check_state_set(check, EINA_FALSE);
     }
   else
     {
        ui_ruler_show (ruler);
        elm_check_state_set(check, EINA_TRUE);
     }
}

static void
_ctxpopup_item_zoom_in_cb(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
}

static void
_ctxpopup_item_zoom_out_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
}

static void
_ctxpopup_item_separate_cb(void *data __UNUSED__,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
}

static void
_ctxpopup_item_legend_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
}

static void
_ctxpopup_item_abs_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");

   if (ui_ruler_scale_absolute_visible_get(ws->ruler_hor))
     {
        ui_ruler_scale_absolute_visible_set (ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_absolute_visible_set (ws->ruler_ver, EINA_FALSE);
        elm_check_state_set(check, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_absolute_visible_set (ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_absolute_visible_set (ws->ruler_ver, EINA_TRUE);
        elm_check_state_set(check, EINA_TRUE);
     }

}

static void
_ctxpopup_item_rel_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *check = elm_object_item_part_content_get(event_info, "icon");

   if (ui_ruler_scale_relative_visible_get(ws->ruler_hor))
     {
        ui_ruler_scale_relative_visible_set (ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_relative_visible_set (ws->ruler_ver, EINA_FALSE);
        elm_check_state_set(check, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_relative_visible_set (ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_relative_visible_set (ws->ruler_ver, EINA_TRUE);
        elm_check_state_set(check, EINA_TRUE);
     }
}

static void
_ruler_check_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Evas_Object *ruler = (Evas_Object*)data;

   if (ui_ruler_visible_get(ruler))
     {
        ui_ruler_hide (ruler);
        elm_check_state_set(check, EINA_FALSE);
     }
   else
     {
        ui_ruler_show (ruler);
        elm_check_state_set(check, EINA_TRUE);
     }
}

static void
_ruler_abs_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Workspace *ws = (Workspace *)data;

   if (ui_ruler_scale_absolute_visible_get(ws->ruler_hor))
     {
        ui_ruler_scale_absolute_visible_set (ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_absolute_visible_set (ws->ruler_ver, EINA_FALSE);
        elm_check_state_set(check, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_absolute_visible_set (ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_absolute_visible_set (ws->ruler_ver, EINA_TRUE);
        elm_check_state_set(check, EINA_TRUE);
     }
}

static void
_ruler_rel_cb(void *data, Evas_Object *obj, void *event_info __UNUSED__)
{
   Evas_Object *check = obj;
   Workspace *ws = (Workspace *)data;

   if (ui_ruler_scale_relative_visible_get(ws->ruler_hor))
     {
        ui_ruler_scale_relative_visible_set (ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_relative_visible_set (ws->ruler_ver, EINA_FALSE);
        elm_check_state_set(check, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_relative_visible_set (ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_relative_visible_set (ws->ruler_ver, EINA_TRUE);
        elm_check_state_set(check, EINA_TRUE);
     }
}


void
ui_popup_show (Evas_Object *parent, Evas_Object *popup)
{
   Evas_Coord x,y;
   evas_pointer_canvas_xy_get(evas_object_evas_get(parent), &x, &y);
   evas_object_move(popup, x, y);
   evas_object_show(popup);
}

void
ui_popup_hide (Evas_Object *obj)
{
   elm_ctxpopup_dismiss (obj);
}

Evas_Object *
ui_popup_add (Evas_Object *obj, Workspace *ws)
{
   Evas_Object *ctxpopup, *check;
   Elm_Object_Item *eoi = NULL;

   ctxpopup = elm_ctxpopup_add(obj);
   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom +", NULL,
                            _ctxpopup_item_zoom_out_cb, ws->ruler_hor);
   elm_object_item_disabled_set(eoi, EINA_TRUE);
   eoi = elm_ctxpopup_item_append(ctxpopup, "zoom -", NULL,
                            _ctxpopup_item_zoom_in_cb, ws->ruler_hor);
   elm_object_item_disabled_set(eoi, EINA_TRUE);
   eoi = elm_ctxpopup_item_append(ctxpopup, "separate", NULL,
                            _ctxpopup_item_separate_cb, ws->ruler_ver);
   elm_object_item_disabled_set(eoi, EINA_TRUE);
   eoi = elm_ctxpopup_item_append(ctxpopup, "legend", NULL,
                            _ctxpopup_item_legend_cb, ws->ruler_ver);
   elm_object_item_disabled_set(eoi, EINA_TRUE);


   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler hor.", NULL,
                            _ctxpopup_item_ruler_cb, ws->ruler_hor);
   check = elm_check_add(ctxpopup);
   elm_check_state_set(check, EINA_TRUE);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_check_cb,
                                  ws->ruler_hor);
   evas_object_show(check);


   eoi = elm_ctxpopup_item_append(ctxpopup, "ruler ver.", NULL,
                            _ctxpopup_item_ruler_cb, ws->ruler_ver);
   check = elm_check_add(ctxpopup);
   elm_check_state_set(check, EINA_TRUE);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_check_cb,
                                  ws->ruler_ver);
   evas_object_show(check);


   eoi = elm_ctxpopup_item_append(ctxpopup, "absolute scale", NULL,
                            _ctxpopup_item_abs_cb, ws);
   check = elm_check_add(ctxpopup);
   elm_check_state_set(check, EINA_TRUE);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_abs_cb, ws);
   evas_object_show(check);


   eoi = elm_ctxpopup_item_append(ctxpopup, "relative scale", NULL,
                            _ctxpopup_item_rel_cb, ws);
   check = elm_check_add(ctxpopup);
   elm_check_state_set(check, EINA_FALSE);
   elm_object_item_part_content_set(eoi, "icon", check);
   evas_object_smart_callback_add(check, "changed", _ruler_rel_cb, ws);
   evas_object_show(check);

   evas_object_layer_set(ctxpopup, 15);
   return ctxpopup;
}
