#include "ui_popup.h"
#include "efl_tet.h"

static void
_ctxpopup_item_ruler_hor_cb(void *data,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	static Eina_Bool _visible = EINA_TRUE;
	Evas_Object *ruler = (Evas_Object*)data;
	if (_visible)
	{
		ui_ruler_hide (ruler);
		_visible = EINA_FALSE;
	}
	else
	{
		ui_ruler_show (ruler);
		_visible =EINA_TRUE;
	}
}
static void
_ctxpopup_item_ruler_ver_cb(void *data __UNUSED__,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	static Eina_Bool _visible = EINA_TRUE;
	Evas_Object *ruler = (Evas_Object*)data;
	if (_visible)
	{
		ui_ruler_hide (ruler);
		_visible = EINA_FALSE;
	}
	else
	{
		ui_ruler_show (ruler);
		_visible =EINA_TRUE;
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
	Evas_Object *ctxpopup;
//	Workspace *ws = ws_init();

	ctxpopup = elm_ctxpopup_add(obj);
	elm_ctxpopup_item_append(ctxpopup, "zoom +", NULL,
		_ctxpopup_item_zoom_out_cb, NULL);
	elm_ctxpopup_item_append(ctxpopup, "zoom -", NULL,
		_ctxpopup_item_zoom_in_cb, NULL);
	elm_ctxpopup_item_append(ctxpopup, "separate", NULL,
		_ctxpopup_item_separate_cb, NULL);
	elm_ctxpopup_item_append(ctxpopup, "legend", NULL,
		_ctxpopup_item_legend_cb, NULL);
	elm_ctxpopup_item_append(ctxpopup, "ruler hor.", NULL,
		_ctxpopup_item_ruler_hor_cb, ws->ruler_ver);
	elm_ctxpopup_item_append(ctxpopup, "ruler ver.", NULL,
		_ctxpopup_item_ruler_ver_cb, ws->ruler_hor);
/*
	item_new(ctxpopup, "zoom +", "home");
	item_new(ctxpopup, "zoom -", "file");
	item_new(ctxpopup, "separate", "delete");
	it = item_new(ctxpopup, "legend", "folder");
	elm_object_item_disabled_set(it, EINA_TRUE);
	item_new(ctxpopup, "ruler hor.", "edit");
	it = item_new(ctxpopup, "ruler ver", "clock");
	elm_object_item_disabled_set(it, EINA_TRUE);
*/
	return ctxpopup;
}
