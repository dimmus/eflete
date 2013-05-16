#include <ui_workspace.h>
#include <efl_tet.h>


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
_ws_zoom_out (Workspace __UNUSED__ *ws )
{
	int x, y, w, h;
	evas_object_image_fill_get (ws->bg, &x, &y, &w, &h);

	evas_object_image_fill_set (ws->bg, 0, 0,
		w/ws->zoom_step,
		h/ws->zoom_step);
	return EINA_TRUE;
}

static void
_zoom_out_on_click (void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
	_ws_zoom_out ((Workspace *)data);
}

static void
_zoom_in_on_click (void *data, Evas_Object *obj, void *event_info)
{
	_ws_zoom_in ((Workspace *)data);
}

static void
_separate_on_click (void *data, Evas_Object *obj, void *event_info)
{
	//NULL = NULL;
}

Eina_Bool
ws_bf_set (Evas_Object *bg)
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


Workspace *
ws_add (Evas_Object *layout)
{
	Workspace *ws = NULL;
	Evas_Object *_bg, *_button;
	Evas_Object *_icon;
	Eina_Bool _img_load;
	Ecore_Evas *canvas;

	ws = ws_init();
	ws_zoom_step_set (2, ws);
	canvas = evas_object_evas_get (layout);
	ws->canvas = canvas;
	elm_layout_file_set (layout, TET_EDJ, "base/workspace" );
	_bg = evas_object_image_filled_add (canvas);
	evas_object_image_filled_set (_bg, EINA_FALSE);
	evas_object_resize (_bg, 400, 400);
	evas_object_image_file_set (_bg, TET_IMG_PATH"bg_workspace.png", NULL);
	evas_object_image_fill_set (_bg, 0, 0, 32, 32);
	elm_object_part_content_set (layout, "base/workspace/background", _bg);
	evas_object_show (_bg);
	ws->bg = _bg;

	_button = elm_button_add (layout);
	elm_object_part_content_set (layout, "base/workspace/button_zoom_out",
		_button);
	evas_object_smart_callback_add (_button, "clicked", _zoom_out_on_click, ws);
	ws->button_zoom_out = _button;
	elm_object_content_unset (_button);

	_icon = elm_icon_add (_button);
	elm_icon_file_set(_icon, TET_IMG_PATH"zoom_out.png", NULL);
	elm_image_no_scale_set (_icon, EINA_TRUE);
	elm_object_part_content_set(_button, NULL, _icon);

	_button = elm_button_add (layout);
	elm_object_part_content_set (layout, "base/workspace/button_zoom_in",
		_button);
	evas_object_smart_callback_add (_button, "clicked", _zoom_in_on_click, ws);
	ws->button_zoom_in = _button;

	_icon = elm_icon_add (_button);
	elm_icon_file_set(_icon, TET_IMG_PATH"zoom_in.png", NULL);
	elm_image_no_scale_set (_icon, EINA_TRUE);
	elm_object_part_content_set(_button, NULL, _icon);

	_button = elm_button_add (layout);
	elm_object_part_content_set (layout, "base/workspace/button_separate",
		_button);
	evas_object_smart_callback_add (_button, "clicked", _separate_on_click, ws);
	ws->button_separate = _button;
	_icon = elm_icon_add (_button);
	elm_icon_file_set(_icon, TET_IMG_PATH"layer_show.png", NULL);
	elm_image_no_scale_set (_icon, EINA_TRUE);
	elm_object_part_content_set(_button, NULL, _icon);

	return ws;


}

Workspace *
ws_init (void)
{
	Workspace *ws = (Workspace *) calloc (1, sizeof (Workspace));
	if (!ws)
		return NULL;

	ws->canvas = NULL;
	ws->bg = NULL;
	ws->zoom_step = 1;
	ws->ruller_hor = NULL;
	ws->ruller_ver = NULL;
	ws->button_zoom_out = NULL;
	ws->button_zoom_in = NULL;
	ws->button_separate = NULL;

	return ws;
}
