#include <ui_workspace.h>
#include <efl_tet.h>


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
_zoom_out_on_click (void *data __UNUSED__,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	Workspace *ws = (Workspace *)data;

	ui_ruler_scale_relative_visible_set (ws->ruler_hor,
		!ui_ruler_scale_relative_visible_get (ws->ruler_hor));

	ui_ruler_scale_relative_visible_set (ws->ruler_ver,
		!ui_ruler_scale_relative_visible_get (ws->ruler_ver));

}

static void
_zoom_in_on_click (void *data __UNUSED__,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	Workspace *ws = (Workspace *)data;

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

	if (ev->button ==3 )
		ui_popup_show (ws->bg, ws->popup);
	else
		ui_popup_hide (ws->popup);
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

char *itoa(long n)
{
    int len = n==0 ? 1 : floor(log10l(abs(n)))+1;
    if (n<0) len++;
    char    *buf = (char*) calloc(sizeof(char), len+1);
	snprintf(buf, len, "%ld", n);
    return   buf;
}

Workspace *
ws_add (Evas_Object *parent)
{
	Workspace *ws;
	Evas_Object *_bg, *_button, *_ruler_hor, *_scroller, *_ruler_ver, *_popup;
	Evas_Object *_icon, *_group_space, *_group_space_2;
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

	_scroller  = elm_scroller_add (parent);
	elm_object_part_content_set (parent, "base/workspace/scroller",	_scroller);
	evas_object_show (_scroller);

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

	evas_object_event_callback_add(_bg, EVAS_CALLBACK_MOUSE_MOVE,
		_ws_mouse_move_cb, ws);

	evas_object_event_callback_add(_bg, EVAS_CALLBACK_MOUSE_DOWN,
		_ws_mouse_click_cb, ws);

	//we need to put this code into ui_groupspace_add();
	//and how and where should we call that thingy?
	_group_space = elm_layout_add(parent);
	elm_object_part_content_set (parent,
		"base/workspace/groupspace",_group_space);
	elm_layout_file_set(_group_space, TET_EDJ, "base/groupspace");

	_group_space_2 = elm_layout_add(_group_space);
	elm_object_part_content_set (_group_space,
		"base/groupspace/groupspace",_group_space_2);

	return ws;
}

Workspace *
_ws_init (void)
{
	Workspace *ws = (Workspace *) calloc (1, sizeof (Workspace));
	if (!ws)
		return NULL;

	ws->canvas = NULL;
	ws->bg = NULL;
	ws->zoom_step = 1;
	ws->ruler_hor = NULL;
	ws->ruler_ver = NULL;
	ws->button_zoom_out = NULL;
	ws->button_zoom_in = NULL;
	ws->button_separate = NULL;
	ws->group_space = NULL;

	return ws;
}
