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

	evas_object_show (_rectangle);

	return _rectangle;
}

void
_gs_part_object_coord_get (const char* name, Group *group,
							int *x, int *y, int *x2, int *y2)
{
	int _x, _y, _w, _h;
	Eina_Bool _find_part = EINA_FALSE;
	Part *_part = NULL;
	_x=_y=_w=_h=0;

	EINA_INLIST_FOREACH (group->parts, _part)
	{
		if (!strcmp(_part->name, name))
		{
			evas_object_geometry_get (_part->obj, &_x, &_y, &_w, &_h);
			_find_part = EINA_TRUE;
			break;
		}
	}
	if (_find_part)
	{
		if (x) {*x = _x;}
		if (y) {*y = _y;}
		if (x2) {*x2 = _w+_x;}
		if (y2) {*y2 = _h+_y;}
	}
}

void
_gs_part_geometry_calc (Evas_Object *view_part, Group *group, Part *part,
						int *x, int *y, int *w, int*h)
{
	/*Parametrs for group size*/
	int _x, _y, _w, _h;
	/*Variables for take rel_to parts coordinats corners*/
	int _x1_rel_to, _y1_rel_to, _x2_rel_to, _y2_rel_to;
	/*Results parametr for current part*/
	int _coord_x, _coord_y, _rect_w, _rect_h;
	/*temprory size for situation: size of group less or equal than size of part*/
	int _size_w, _size_h;
	/*Parametrs for virtual group size wich cut with rel1 and rel2 parameters*/
	int _group_x, _group_y, _group_x2, _group_y2;
	/*Current part state structure*/
	Part_State *_current_state = NULL;

	/*initialize local variables*/
	_x = _y = _w = _h = 0;
	_coord_x = _coord_y = _rect_w = _rect_h = -1;
	_x1_rel_to = _y1_rel_to = _x2_rel_to = _y2_rel_to = 0;
	_group_x = _group_y = _group_x2 = _group_y2 = -1;

	_current_state = EINA_INLIST_CONTAINER_GET (part->states, Part_State);

	evas_object_geometry_get (view_part, &_x, &_y, &_w, &_h);
	/*If group size less then 1 its fail*/
	if (!_w || !_h)
	{
		return;
	}

	group->current_w = _w;
	group->current_h = _h;

	/*if "max" parametr not present in EDC file, then max size part will be
	 * equal size of group. 
	*/
	if ((_current_state->max_w == -1) || (_current_state->max_h == -1))
	{
		_size_w = group->current_w;
		_size_h = group->current_h;
	}
	else
	{
		_size_w = _current_state->max_w;
		_size_h = _current_state->max_h;
	}

	if(_current_state->rel1_to_x_name)
	{
		_gs_part_object_coord_get (_current_state->rel1_to_x_name, group,
			&_x1_rel_to, &_y1_rel_to, &_x2_rel_to, &_y2_rel_to);		

		_group_x = _x1_rel_to + (_x2_rel_to - _x1_rel_to) *
			_current_state->rel1_relative_x;
	}
	else
	{
		_group_x = _x + _w * _current_state->rel1_relative_x;
	}

	if(_current_state->rel1_to_y_name)
	{
		_gs_part_object_coord_get (_current_state->rel1_to_y_name, group,
			&_x1_rel_to, &_y1_rel_to, &_x2_rel_to, &_y2_rel_to);

		_group_y = _y1_rel_to + (_y2_rel_to - _y1_rel_to) *
			_current_state->rel1_relative_y;
	}
	else
	{
		_group_y = _y + _h * _current_state->rel1_relative_y;
	}

	if(_current_state->rel2_to_x_name)
	{
		_gs_part_object_coord_get (_current_state->rel2_to_x_name, group,
			&_x1_rel_to, &_y1_rel_to, &_x2_rel_to, &_y2_rel_to);

		_group_x2 = _x1_rel_to + (_x2_rel_to - _x1_rel_to) *
			_current_state->rel2_relative_x;
	}
	else
	{
		_group_x2 = _x + _w * _current_state->rel2_relative_x;
	}

	if(_current_state->rel2_to_y_name)
	{
		_gs_part_object_coord_get (_current_state->rel2_to_y_name, group,
			&_x1_rel_to, &_y1_rel_to, &_x2_rel_to, &_y2_rel_to);

		_group_y2 = _y1_rel_to + (_y2_rel_to - _y1_rel_to) *
			_current_state->rel2_relative_y;
	}
	else
	{
		_group_y2 = _y + _h * _current_state->rel2_relative_y;
	}

	if ((_size_w >= group->current_w) || (_size_h >= group->current_h))
	{
		_coord_x = _group_x;
		_coord_y = _group_y;
		_rect_w =  _w * _current_state->rel2_relative_x - 
					_w * _current_state->rel1_relative_x;
		_rect_h = _h *_current_state->rel2_relative_y - 
					_h * _current_state->rel1_relative_y;

	}
	else
	{
		if(_current_state->align_x <= 0.5)
		{
			_coord_x = _group_x + (_group_x2 - _group_x) * _current_state->align_x -
				_size_w * _current_state->align_x;
		}
		else
		{
			_coord_x = _group_x2 - _size_w;
		}
		
		if(_current_state->align_y <= 0.5)
		{
			_coord_y = _group_y + (_group_y2 - _group_y) * _current_state->align_y -
				_size_h * _current_state->align_y;
		}
		else
		{
			_coord_y = _group_y2 - _size_h;
		}
		_rect_w = _size_w; 
		_rect_h = _size_h;
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

	ui_groupspace_del (ap->ws->groupspace, group);

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
ui_groupspace_del (Evas_Object *obj, Group *group)
{
	Part *_part = NULL;

	EINA_INLIST_FOREACH (group->parts, _part)
		evas_object_del (_part->obj);

	if (obj)
		evas_object_del (obj);

	obj=NULL;
}
