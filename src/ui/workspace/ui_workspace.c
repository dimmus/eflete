#include <ui_workspace.h>
#include <efl_tet.h>

Evas_Object *
ui_groupspace_add(Evas_Object *parent);

Workspace *
_ws_init (void);

Eina_Bool
_ws_zoom_in (Workspace *ws)
{
   int x, y, w, h;
   evas_object_image_fill_get (ws->bg, &x, &y, &w, &h);
   evas_object_image_fill_set (ws->bg, 0, 0,
                               w*ws->zoom_step,
                               h*ws->zoom_step);
   return EINA_TRUE;
}

Eina_Bool
_ws_zoom_out (Workspace *ws )
{
   int x, y, w, h;
   evas_object_image_fill_get (ws->bg, &x, &y, &w, &h);

   evas_object_image_fill_set (ws->bg, 0, 0,
                               w/ws->zoom_step,
                               h/ws->zoom_step);
   return EINA_TRUE;
}

static void
_zoom_out_on_click (void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;

   ui_ruler_pointer_visible_set(ws->ruler_hor, EINA_FALSE);
   ui_ruler_pointer_visible_set(ws->ruler_ver, EINA_FALSE);

   ui_ruler_scale_relative_visible_set (ws->ruler_hor,
     !ui_ruler_scale_relative_visible_get (ws->ruler_hor));

   ui_ruler_scale_relative_visible_set (ws->ruler_ver,
     !ui_ruler_scale_relative_visible_get (ws->ruler_ver));
}

static void
_zoom_in_on_click (void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_ruler_pointer_visible_set(ws->ruler_hor, EINA_TRUE);
   ui_ruler_pointer_visible_set(ws->ruler_ver, EINA_TRUE);

   ui_ruler_scale_absolute_visible_set (ws->ruler_hor,
     !ui_ruler_scale_absolute_visible_get (ws->ruler_hor));

   ui_ruler_scale_absolute_visible_set (ws->ruler_ver,
     !ui_ruler_scale_absolute_visible_get (ws->ruler_ver));
}

static void
_separate_on_click (void *data __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   if (ui_ruler_step_relative_get (ws->ruler_hor)>=0.5)
     {
        ui_ruler_step_relative_set (ws->ruler_hor ,0.1);
        ui_ruler_step_relative_set (ws->ruler_ver ,0.1);
     }
   else
     {
        ui_ruler_step_relative_set (ws->ruler_hor ,0.5);
        ui_ruler_step_relative_set (ws->ruler_ver ,0.5);
     }
}

static void
_ws_mouse_click_cb (void *data ,
                    Evas *e __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   Workspace *ws = (Workspace*)data;
   elm_object_signal_emit (ws->ruler_hor, "test", "ws");
   if (ev->button ==3) ui_popup_show (ws->bg, ws->popup);
   else ui_popup_hide (ws->popup);
}


Eina_Bool
ws_bf_set (Evas_Object *bg __UNUSED__)
{
   return EINA_TRUE;
}

void
ws_zoom_step_set (unsigned int step, Workspace *ws)
{
   ws->zoom_step = step;
}

int
ws_zoom_step_get (Workspace *ws)
{
   return ws->zoom_step;
}

static void
_ws_mouse_move_cb (void *data, Evas *e,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   int x, y;
   Workspace *ws = (Workspace*)data;
   evas_pointer_output_xy_get (e, &x, &y);
   ui_ruler_pointer_pos_set (ws->ruler_hor, x);
   ui_ruler_pointer_pos_set (ws->ruler_ver, y);
}

Workspace *
ws_add (Evas_Object *parent)
{
   Workspace *ws;
   Evas_Object *_bg, *_button, *_ruler_hor, *_ruler_ver, *_popup;
   Evas_Object *_icon;
   Evas *canvas;

   ws = _ws_init();
   if (!ws)
     {
        ERR ("Unable to crerate Workspace structure");
        return NULL;
     }
   ws_zoom_step_set (2, ws);
   canvas = evas_object_evas_get (parent);
   ws->canvas = canvas;
   elm_layout_file_set (parent, TET_EDJ, "base/workspace" );
   _bg = evas_object_image_filled_add (canvas);
   evas_object_image_filled_set (_bg, EINA_FALSE);
   evas_object_image_file_set (_bg, TET_IMG_PATH"bg_workspace.png", NULL);
   evas_object_image_fill_set (_bg, 0, 0, 32, 32);
   elm_object_part_content_set (parent, "base/workspace/background", _bg);
   evas_object_show (_bg);
   ws->bg = _bg;

   _button = elm_button_add (parent);
   elm_object_part_content_set (parent, "base/workspace/button_zoom_out",
                                _button);
   evas_object_smart_callback_add (_button, "clicked", _zoom_out_on_click, ws);
   ws->button_zoom_out = _button;
   elm_object_content_unset (_button);

   _icon = elm_icon_add (_button);
   elm_image_file_set(_icon, TET_IMG_PATH"zoom_out.png", NULL);
   elm_image_no_scale_set (_icon, EINA_TRUE);
   elm_object_part_content_set(_button, NULL, _icon);

   _button = elm_button_add (parent);
   elm_object_part_content_set (parent, "base/workspace/button_zoom_in",
                                _button);
   evas_object_smart_callback_add (_button, "clicked", _zoom_in_on_click, ws);
   ws->button_zoom_in = _button;

   _icon = elm_icon_add (_button);
   elm_image_file_set(_icon, TET_IMG_PATH"zoom_in.png", NULL);
   elm_image_no_scale_set (_icon, EINA_TRUE);
   elm_object_part_content_set(_button, NULL, _icon);

   _button = elm_button_add (parent);
   elm_object_part_content_set (parent, "base/workspace/button_separate",
                                _button);
   evas_object_smart_callback_add (_button, "clicked", _separate_on_click, ws);
   ws->button_separate = _button;
   _icon = elm_icon_add (_button);
   elm_image_file_set(_icon, TET_IMG_PATH"layer_show.png", NULL);
   elm_image_no_scale_set (_icon, EINA_TRUE);
   elm_object_part_content_set(_button, NULL, _icon);

   _ruler_hor = ui_ruler_add (parent);
   elm_object_part_content_set (parent, "base/workspace/ruler_hor",_ruler_hor);
   ws->ruler_hor = _ruler_hor;

   _ruler_ver = ui_ruler_add (parent);
   ui_ruler_orient_set (_ruler_ver, VERTICAL);
   elm_object_part_content_set (parent, "base/workspace/ruler_ver",_ruler_ver);
   ws->ruler_ver = _ruler_ver;

   _popup = ui_popup_add (parent, ws);
   ws->popup = _popup;

   ws->groupspace = ui_groupspace_add(parent);

   evas_object_event_callback_add(_bg, EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb, ws);

   evas_object_event_callback_add(_bg, EVAS_CALLBACK_MOUSE_DOWN,
                                  _ws_mouse_click_cb, ws);

   return ws;
}

Workspace *
_ws_init (void)
{
   Workspace *ws = (Workspace *) calloc (1, sizeof (Workspace));
   if (!ws) return NULL;

   ws->canvas = NULL;
   ws->bg = NULL;
   ws->zoom_step = 1;
   ws->ruler_hor = NULL;
   ws->ruler_ver = NULL;
   ws->button_zoom_out = NULL;
   ws->button_zoom_in = NULL;
   ws->button_separate = NULL;
   ws->groupspace = NULL;
   ws->highlight.highlight = NULL;
   ws->highlight.obj = NULL;

   return ws;
}

void
ui_object_highlight_set(Workspace *ws, Evas_Object *part)
{
   if ((!ws) || (!part) ) return;

   if (ws->highlight.highlight)
     evas_object_del (ws->highlight.highlight);

   ws->highlight.highlight = elm_label_add(ws->groupspace);
   elm_layout_file_set(ws->highlight.highlight, TET_EDJ,
                            "base/groupspace/part/hightlight");
   ws->highlight.obj = part;
   evas_object_raise(ws->highlight.highlight);
   ui_object_highlight_move(ws);
   evas_object_event_callback_add(ws->highlight.highlight,
                                  EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb, ws);

   evas_object_show(ws->highlight.highlight);


}

void
ui_object_highlight_move(Workspace *ws)
{
   int x, y, w, h;

   if ((!ws)) return;

   evas_object_geometry_get(ws->highlight.obj, &x, &y, &w, &h);
   evas_object_move(ws->highlight.highlight, x, y);
   evas_object_resize(ws->highlight.highlight, w, h);
}

void
ui_object_highlight_hide(Workspace *ws)
{
   if(!ws) return;

   evas_object_hide(ws->highlight.highlight);
}
