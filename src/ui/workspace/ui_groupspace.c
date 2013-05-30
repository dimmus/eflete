#include "ui_groupspace.h"

Evas_Object *
_gs_rect_draw (Evas_Object * _view_part,Group *group, Part *part);

void
_gs_group_draw (Evas_Object *view_part , Group *group, Workspace *ws);

void
_gs_move_parts (Evas_Object *view_part , Group *group);

void
_gs_part_geometry_calc (Evas_Object *view_part, Group *group, Part *part,
						int *x, int *y, int *w, int*h);

static void
_gs_mouse_move_cb (void *data, Evas *e,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	int x, y;
	Workspace *ws = (Workspace*)data;
	evas_pointer_output_xy_get (e, &x, &y);
	ui_ruler_pointer_pos_set (ws->ruler_hor, x);
	ui_ruler_pointer_pos_set (ws->ruler_ver, y);
}

static void
_gs_resize_cb (void *data, Evas *e __UNUSED__,
					Evas_Object *obj ,
					void *event_info __UNUSED__)
{
	int x, y, w, h;
	Workspace *ws = (Workspace*)data;

	evas_object_geometry_get (obj, &x, &y, &w, &h);
	ui_ruler_scale_absolute_position_zero_set (ws->ruler_hor, x);
	ui_ruler_scale_absolute_position_zero_set (ws->ruler_ver, y);
	ui_ruler_scale_relative_position_set (ws->ruler_hor, x, x+w);
	ui_ruler_scale_relative_position_set (ws->ruler_ver, y, y+h);

}

static void
_gs_resize_parts_cb (void *data, Evas *e __UNUSED__,
					Evas_Object *obj ,
					void *event_info __UNUSED__)
{
	_gs_move_parts (obj, (Group*)(data));
}

Evas_Object *
_gs_rect_draw (Evas_Object * _view_part, Group *group, Part *part)
{

	Evas_Object *_rectangle;
	Evas *canvas;
	int x, y, w, h;
	Part_State *_current_state = NULL;
	_current_state = EINA_INLIST_CONTAINER_GET (part->states, Part_State);

	canvas = evas_object_evas_get (_view_part);
	_gs_part_geometry_calc (_view_part, group, part, &x, &y, &w, &h);

	_rectangle = evas_object_rectangle_add (canvas);
	evas_object_move (_rectangle, x, y);
	evas_object_resize (_rectangle, w, h);
	evas_object_color_set (_rectangle, _current_state->color[0],
						_current_state->color[1],_current_state->color[2],
						_current_state->color[3]);

/*	if (!(_part->show) || (!_current_state->visible))
		evas_object_hide (_rectangle);
	else*/
		evas_object_show (_rectangle);

	return _rectangle;
}

void
_gs_part_geometry_calc (Evas_Object *view_part, Group *group, Part *part,
						int *x, int *y, int *w, int*h)
{
	int _x, _y, _w, _h;
	int _coord_x, _coord_y;
	int _rect_w, _rect_h;
	int _size_w, _size_h;
	Part *_part = NULL;
	Part_State *_current_state = NULL;

//	fprintf (stdout, "\nDEBAG: name part [%s]\n", part->name);
	_part = part;
	_coord_x =0; _coord_y=0; _rect_w=0; _rect_h=0;
	_current_state = EINA_INLIST_CONTAINER_GET (_part->states, Part_State);

	evas_object_geometry_get (view_part, &_x, &_y, &_w, &_h);
	if (!_w || !_h) return;
/*	fprintf (stdout, "DEBAG: _view_part[%p] x[%d] y[%d] w[%d] h[%d]\n",
		view_part, _x, _y, _w, _h);
*/
	group->current_w = _w;
	group->current_h = _h;

	if ((_current_state->max_w==-1) || (_current_state->max_h<0))
	{
		_size_w = group->current_w;
		_size_h = group->current_h;
	}
	else
	{
		_size_w = _current_state->max_w;
		_size_h = _current_state->max_h;
	}
/*	fprintf (stdout, "DEBAG: max_w[%d] max_h[%d]\n",
		_current_state->max_w,_current_state->max_h);
	fprintf (stdout, "DEBAG: _size_w[%d] _size_h[%d]\n",_size_w, _size_h);
*/
	if (_current_state->align_x<=0.5)
	{
	  	if (group->current_w>_size_w)
		{
			 _coord_x = (_x+_w*_current_state->rel1_relative_x) +
				(_w*_current_state->align_x -_size_w*_current_state->align_x)+
				_current_state->rel1_offset_x;
			 _coord_y = (_y+_h*_current_state->rel1_relative_y)+
				(_h*_current_state->align_y-_size_h*_current_state->align_y) +
				_current_state->rel1_offset_y;

			_rect_w=(_coord_x+_size_w+_current_state->rel2_offset_x)-_coord_x;
			_rect_h=(_coord_y+_size_h+_current_state->rel2_offset_y)-_coord_y;
//				DBG ("group_w>size_w");
		}
		if (group->current_w<=_size_w)
		{
			 _coord_x = (_x+(_w*_current_state->rel1_relative_x)) +
				_current_state->rel1_offset_x;
			 _coord_y = (_y+(_h*_current_state->rel1_relative_y)) +
				_current_state->rel1_offset_y;

			_rect_w=(_x+(_w*_current_state->rel2_relative_x)) - _coord_x +
				_current_state->rel2_offset_x;
			_rect_h=(_y+(_h*_current_state->rel2_relative_y))-_coord_y +
				_current_state->rel2_offset_y;
//			DBG ("group_w<=size_w");
		}
/*fprintf (stdout, "DEBAG: align[%3.2f]<=0.5 _coord_x[%d] _coord_y[%d] _rect_w[%d] rect_h[%d]\n",
			_current_state->align_x,_coord_x, _coord_y, _rect_w, _rect_h);
*/	}
	else
	{
	 if (group->current_w>_size_w)
		{
			 _coord_x = (_x+_w*_current_state->rel1_relative_x) +
				(_w*_current_state->align_x-_size_w) +
				_current_state->rel1_offset_x;
			 _coord_y = (_y+_h*_current_state->rel1_relative_y) +
				(_h*_current_state->align_y-_size_h) +
				_current_state->rel1_offset_y;
			_rect_w=(_coord_x+_size_w+_current_state->rel2_offset_x)-_coord_x;
			_rect_h=(_coord_y+_size_h+_current_state->rel2_offset_y)-_coord_y;
/*fprintf (stdout, "DEBAG: align[%3.2f]<=0.5 _coord_x[%d] _coord_y[%d] _rect_w[%d] rect_h[%d]\n",
			_current_state->align_x,_coord_x, _coord_y, _rect_w, _rect_h);
*/
		}
	}

	*x=_coord_x; *y=_coord_y; *w=_rect_w; *h = _rect_h;
}

void
_gs_move_parts (Evas_Object *view_part, Group *group)
{
	int x, y, w, h;
	Part *_part = NULL;

	EINA_INLIST_FOREACH (group->parts, _part)
	{
		_gs_part_geometry_calc (view_part, group, _part, &x, &y, &w, &h);
		evas_object_move (_part->obj, x, y);
		evas_object_resize (_part->obj, w, h);
	}
}

void
_gs_group_draw (Evas_Object *view_part , Group *group, Workspace *ws)
{
	Part *_part = NULL;
	Evas_Object *_part_object = NULL;

	EINA_INLIST_FOREACH (group->parts, _part)
	{
/*		switch (_part->type)
		{
			case EDJE_PART_TYPE_RECTANGLE:
				_part_object=_gs_rect_draw(view_part, group, _part);
			break;
			defalut:
				DBG ("Cannot recognize part type");
			break;
		}*/
		if (_part->type == EDJE_PART_TYPE_RECTANGLE)
			_part_object=_gs_rect_draw(view_part, group, _part);
		else
			DBG ("Cannot recognize part type");
		evas_object_event_callback_add(_part_object, EVAS_CALLBACK_MOUSE_MOVE,
			_gs_mouse_move_cb, ws);
		_part->obj = _part_object;
	}
}

void
ui_groupspace_add (App_Data *ap, Group *group)
{
	Evas_Object *parent = ap->block_canvas;
	Evas_Object *_groupspace;
	Evas_Object *_part_view;

	ui_groupspace_del (ap->ws->groupspace);

	_groupspace = elm_layout_add (parent);

	elm_object_part_content_set (parent,
		"base/workspace/groupspace",_groupspace);
	elm_layout_file_set(_groupspace, TET_EDJ, "base/groupspace");

	_part_view = elm_layout_add(_groupspace);
	elm_object_part_content_set (_groupspace,
		"base/groupspace/groupspace",_part_view);
	evas_object_show (_part_view);

	evas_object_event_callback_add(_groupspace, EVAS_CALLBACK_MOUSE_MOVE,
		_gs_mouse_move_cb, ap->ws);

	evas_object_event_callback_add  (_part_view, EVAS_CALLBACK_RESIZE,
		_gs_resize_cb, ap->ws);
	evas_object_event_callback_add  (_part_view,EVAS_CALLBACK_RESIZE,
		_gs_resize_parts_cb, group );
	ap->ws->groupspace = _groupspace;
	_gs_group_draw (_part_view, group, ap->ws);
}

void
ui_groupspace_del (Evas_Object *obj)
{
	if (obj)
		evas_object_del (obj);

	obj=NULL;
}
