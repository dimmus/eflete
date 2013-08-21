#include <ui_workspace.h>
#include <efl_tet.h>
#include "ui_highlight.h"

#define GS_BOX_KEY "gs_box_key"

Evas_Object *
ui_groupspace_add(Evas_Object *parent);

Group *
ui_groupspace_group_get(Evas_Object *groupspace);

Evas_Object *
ui_groupspace_box_get(Evas_Object *groupspace);

void
ui_groupspace_separate(Workspace *ws);

void
ui_popup_show (Workspace *ws);

Workspace *
_ws_init(void);

void
ui_ws_legend_visible_set(Workspace *ws, Eina_Bool visible)
{
  Evas_Object *parent = elm_object_parent_widget_get(ws->scroller);
  char label_text[BUFF_MAX];
  if (!visible)
     elm_layout_signal_emit(parent, "legend,hide", "");
  else
     elm_layout_signal_emit(parent, "legend,show", "");

  snprintf(label_text, sizeof(label_text), "Current scale is %d%%",
            (int)(ws->zoom_step * 100));
  elm_object_text_set(ws->legend.legend, label_text);
  ws->legend.visible = visible;
}

Eina_Bool
ui_ws_zoom_in(Workspace *ws)
{
   Group *group = ui_groupspace_group_get(ws->groupspace);
   Evas_Object *box = ui_groupspace_box_get(ws->groupspace);
   int w, h;
   char label_text[BUFF_MAX];

   if ((!group) || (!box))
     return EINA_FALSE;
   ws->zoom_step += 0.1;
   if (ws->zoom_step > 10)
     {
        ws->zoom_step = 10;
        return EINA_FALSE;
     }
   edje_object_scale_set(group->obj, ws->zoom_step);
   evas_object_smart_calculate(box);
   evas_object_geometry_get(ws->groupspace, NULL, NULL, &w, &h);
   edje_object_scale_set(ws->groupspace, ws->zoom_step);
   ui_ruler_scale_absolute_dashes_step_set(ws->ruler_hor,
                                          (int)(ws->zoom_step * 5));
   ui_ruler_scale_absolute_dashes_step_set(ws->ruler_ver,
                                           (int)(ws->zoom_step * 5));
   ui_ruler_redraw(ws->ruler_hor);
   ui_ruler_redraw(ws->ruler_ver);
   snprintf(label_text, sizeof(label_text), "Current scale is %d%%",
            (int)(ws->zoom_step * 100));
   elm_object_text_set(ws->legend.legend, label_text);
   return EINA_TRUE;
}

Eina_Bool
ui_ws_zoom_out(Workspace *ws)
{
   Group *group = ui_groupspace_group_get(ws->groupspace);
   Evas_Object *box = ui_groupspace_box_get(ws->groupspace);
   int w, h;
   char label_text[BUFF_MAX];
   if ((!group) || (!box))
     return EINA_FALSE;
   ws->zoom_step -= 0.1;
   if (ws->zoom_step < 0)
     {
        ws->zoom_step = 0;
        return EINA_FALSE;
     }
   edje_object_scale_set(group->obj, ws->zoom_step);
   edje_object_scale_set(ws->groupspace, ws->zoom_step);
   evas_object_geometry_get(group->obj, NULL, NULL, &w, &h);
   evas_object_resize(group->obj, (int)(w + w * ws->zoom_step),
                      (int)(h + h * ws->zoom_step));
   evas_object_smart_calculate(box);
   ui_ruler_scale_absolute_dashes_step_set(ws->ruler_hor,
                                          (int)(ws->zoom_step * 5));
   ui_ruler_scale_absolute_dashes_step_set(ws->ruler_ver,
                                           (int)(ws->zoom_step * 5));
   ui_ruler_redraw(ws->ruler_hor);
   ui_ruler_redraw(ws->ruler_ver);

   snprintf(label_text, sizeof(label_text), "Current scale is %d%%",
            (int)(ws->zoom_step * 100));
   elm_object_text_set(ws->legend.legend, label_text);
   return EINA_TRUE;
}

void
_sc_move_cb(void *data,  Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   if (!ws->groupspace) return;
   Group *group = ui_groupspace_group_get(ws->groupspace);
   if (!group) return;
   int bg_x, bg_y, obj_x, obj_y, obj_w, obj_h;
   evas_object_geometry_get(ws->bg, &bg_x, &bg_y, NULL, NULL);
   evas_object_geometry_get(group->obj, &obj_x, &obj_y, &obj_w, &obj_h);
   ui_ruler_scale_absolute_position_zero_set(ws->ruler_hor, obj_x - bg_x - 25);
   ui_ruler_scale_absolute_position_zero_set(ws->ruler_ver, obj_y - bg_y - 25);
   ui_ruler_scale_relative_position_set(ws->ruler_hor, obj_x - bg_x - 25,
                                        obj_x + obj_w - bg_x - 25);
   ui_ruler_scale_relative_position_set(ws->ruler_ver, obj_y - bg_y - 25,
                                        obj_y + obj_h - bg_y - 25);
   ui_ruler_redraw(ws->ruler_hor);
   ui_ruler_redraw(ws->ruler_ver);
}

static void
_zoom_out_on_click(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   if(!ui_ws_zoom_out(ws))
     WARN("Failed zoom out workspace");
}

static void
_zoom_in_on_click(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   if(!ui_ws_zoom_in(ws))
     WARN("Failed zoom in workspace");
}

static void
_separate_on_click(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   ui_groupspace_separate(ws);
}

static void
_ws_mouse_click_cb(void *data ,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Evas_Event_Mouse_Down *ev = event_info;
   Workspace *ws = (Workspace*)data;
   if (ev->button == 3) ui_popup_show (ws);
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
   ui_ruler_pointer_pos_set(ws->ruler_hor, x);
   ui_ruler_pointer_pos_set(ws->ruler_ver, y);
}

Workspace *
ws_add (Evas_Object *parent)
{
   Workspace *ws;
   Evas_Object *_bg, *_button, *_ruler_hor, *_ruler_ver;
   Evas_Object *_icon;
   Evas_Object *_scroller;
   Evas_Object *_legend;
   Evas *canvas;

   ws = _ws_init();
   ws->separated = EINA_TRUE;
   ws_zoom_step_set(1, ws);
   canvas = evas_object_evas_get(parent);
   ws->canvas = canvas;
   elm_layout_file_set(parent, TET_EDJ, "base/workspace" );
   _bg = evas_object_image_filled_add(canvas);
   evas_object_image_filled_set(_bg, EINA_FALSE);
   evas_object_image_file_set(_bg, TET_IMG_PATH"bg.png", NULL);
   evas_object_image_fill_set(_bg, -5, -5, 32, 32);
   elm_object_part_content_set(parent, "base/workspace/background", _bg);
   evas_object_show(_bg);
   ws->bg = _bg;
   _button = elm_button_add(parent);
   elm_object_part_content_set(parent, "base/workspace/button_zoom_out",
                               _button);
   evas_object_smart_callback_add(_button, "clicked", _zoom_out_on_click, ws);
   elm_object_content_unset(_button);

   _icon = elm_icon_add(_button);
   elm_image_file_set(_icon, TET_IMG_PATH"zoom_out.png", NULL);
   elm_image_no_scale_set(_icon, EINA_TRUE);
   elm_object_part_content_set(_button, NULL, _icon);

   _button = elm_button_add(parent);
   elm_object_part_content_set(parent, "base/workspace/button_zoom_in",
                               _button);
   evas_object_smart_callback_add(_button, "clicked", _zoom_in_on_click, ws);
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

   _scroller = elm_scroller_add(parent);
   ws->scroller = _scroller;
   elm_scroller_policy_set(_scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
   elm_scroller_content_min_limit(_scroller, EINA_FALSE, EINA_FALSE);
   elm_object_part_content_set(parent, "base/workspace/groupspace", _scroller);
   ws->groupspace = ui_groupspace_add(parent);
   elm_object_content_set(_scroller, ws->groupspace);
   evas_object_show(ws->groupspace);
   evas_object_show(_scroller);

   _legend = elm_label_add(parent);
   ws->legend.legend = _legend;
   ws->legend.visible = EINA_FALSE;
   elm_object_part_content_set(parent, "legend", _legend);
   evas_object_show(_legend);

   evas_object_smart_callback_add(_scroller, "scroll", _sc_move_cb, ws);
   evas_object_smart_callback_add(_scroller, "scroll,drag,stop", _sc_move_cb, ws);

   evas_object_event_callback_add(_bg, EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb, ws);
   evas_object_event_callback_add(ws->groupspace, EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb, ws);

   evas_object_event_callback_add(_bg, EVAS_CALLBACK_MOUSE_DOWN,
                                  _ws_mouse_click_cb, ws);
   evas_object_event_callback_add(ws->groupspace, EVAS_CALLBACK_MOUSE_DOWN,
                                  _ws_mouse_click_cb, ws);

   return ws;
}

void
ws_free(Workspace *ws)
{
   free(ws);
}

Workspace *
_ws_init (void)
{
   return mem_calloc (1, sizeof (Workspace));
}

/*
static void
_on_hilight_resize(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj,
                   void *ei __UNUSED__)
{
   int w, h;
   double value;
   const char *state;
   Workspace *ws = (Workspace *)data;
   Group *group = ui_groupspace_group_get(ws->groupspace);
   Part * part = ws->highlight.part;
   if ((!group) || (!part)) return;
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   state = edje_edit_part_selected_state_get(group->obj, part->name, &value);
   edje_edit_state_max_w_set(group->obj, part->name, state, value, w);
   edje_edit_state_max_h_set(group->obj, part->name, state, value, h);
}
*/

void
ui_object_highlight_set(Workspace *ws, Part *part)
{
   if ((!ws) || (!part) || (!ws->separated)) return;

   DBG("I am here!");
   if (!ws->highlight.highlight)
     ws->highlight.highlight = hl_highlight_add(ws->groupspace);

   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   evas_object_box_append(box, ws->highlight.highlight);

   int x, y, w, h;
   evas_object_geometry_get(part->obj, &x, &y, &w, &h);
   evas_object_resize(ws->highlight.highlight, w, h);
   evas_object_move(ws->highlight.highlight, x, y);
   evas_object_show(ws->highlight.highlight);

   ws->highlight.part = part;
   evas_object_event_callback_add(ws->highlight.highlight,
                                  EVAS_CALLBACK_MOUSE_MOVE,
                                  _ws_mouse_move_cb, ws);
   /*
   evas_object_event_callback_add(ws->highlight.highlight,
                                  EVAS_CALLBACK_RESIZE,
                                  _on_hilight_resize, ws);
   */
   hl_highlight_handler_color_set(ws->highlight.highlight, 255, 0, 0, 255);
   hl_highlight_border_color_set(ws->highlight.highlight, 0, 255, 0, 255);
}

void
ui_object_highlight_move(Workspace *ws)
{
   int x, y, w, h;

   if (!ws) return;

   evas_object_geometry_get(ws->highlight.part->obj, &x, &y, &w, &h);
   evas_object_move(ws->highlight.highlight, x, y);
   evas_object_resize(ws->highlight.highlight, w, h);
}

void
ui_object_highlight_hide(Workspace *ws)
{
   if (!ws || !ws->highlight.highlight) return;

   evas_object_hide(ws->highlight.highlight);
}
